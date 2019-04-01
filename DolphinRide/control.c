#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include "typedefs.h"
#include "dpride.h"
#include "ptable.h"

#define SC_SIZE 16
#define NODE_TABLE_SIZE 256

extern void PrintProfileAll(void);

//
// Control file node table
//
typedef struct _node_table {
	UINT Id;
        char *Eep;
	char *Desc;
	INT SCCount;
	char **SCuts;
} NODE_TABLE;

NODE_TABLE NodeTable[NODE_TABLE_SIZE];

//
// Profile cache
//
typedef struct _unit {
        char *SCut;
	char *Unit;
	char *DName;
        INT FromBit;
        INT SizeBit;
        double Slope;
        double Offset;
} UNIT;

typedef struct _profile_cache {
        union _eep {
                char String[8];
                UINT64 Key;
        }  Eep;
	UINT Padding;

	UNIT Unit[SC_SIZE];

} PROFILE_CACHE;

PROFILE_CACHE CacheTable[NODE_TABLE_SIZE];

NODE_TABLE *GetTableId(UINT Target);

PROFILE_CACHE *GetCache(char *Eep);


char *MakeNewName(char *Original, INT Suffix)
{
	int len = strlen(Original);
	char *buf;

	buf = malloc(len + 3);
	if (buf == NULL) {
		Error("cannot alloc buffer");
		return NULL;
	}
	sprintf(buf, "%s%d",Original, Suffix);
	return buf;
}

NODE_TABLE *GetTableId(uint Target)
{
	bool found = false;
	int i;
	NODE_TABLE *nt = &NodeTable[0];

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		//printf("*%s:T=%08X N=%08X\n",
		//       __FUNCTION__, Target, nt->Id);
		if (nt->Id == 0) {
			break;
		}
		else if (Target == nt->Id) {
			//printf("*%s:Found T=%08X\n", __FUNCTION__, Target);
			found = true;
			break;
		}
		nt++;
	}
	return found ? nt : NULL;
}

bool CheckTableId(uint Target)
{
	//printf("*%s:Target=%08X\n", __FUNCTION__, Target);
	return(GetTableId(Target) != NULL);
}

int GetTableIndex(uint Target)
{
        NODE_TABLE *nt = &NodeTable[0];
        NODE_TABLE *targetNT;

	//printf("*%s:Target=%08X\n", __FUNCTION__, Target);
	targetNT = GetTableId(Target);
	return (targetNT == NULL ? -1 : targetNT - nt);
}

char *GetTableEep(uint Target)
{
	NODE_TABLE *nt = GetTableId(Target);
	return (nt == NULL ? NULL : nt->Eep);
}

bool CheckTableEep(char *Target)
{
	bool collision = false;
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}
		ps = nt->SCuts;
		for(j = 0; j < nt->SCCount && *ps != NULL; j++) {
			//printf("++%s: \'%s\'\n", Target, *ps);
			if (!strcmp(Target, *ps++)) {
				//matched
				collision = true;
				break;
			}
		}
		if (collision) {
			//printf("collision: \n");
			break;
		}
		nt++;
	}
	return collision;
}

char *GetNewName(char *Target)
{
	int suffix;
	char *newName;
	char *origName;
	extern void EoReloadControlFile();

	EoReloadControlFile();
	if (CheckTableEep(Target)) {
		// Find collision
		origName = Target;
		Target = NULL; // Clear for new Name
		for(suffix = 1; suffix <= MAX_SUFFIX; suffix++) {
			newName = MakeNewName(origName, suffix);
			if (!CheckTableEep(newName)) {
				Target = newName;
				//printf("NEW:%s not mached\n", Target);
				break;
			}
		}
	}
	else 
		; //printf("ORG:%s not mached\n", Target);

	return Target;
}


//inline bool IsTerminator(char c)
bool IsTerminator(char c)
{
	return (c == '\n' || c == '\r' || c == '\0' || c == '#');
}

char *DeBlank(char *p)
{
	while(isblank(*p)) {
		p++;
	}
	return p;
}

char *CheckNext(char *p)
{
	if (IsTerminator(*p)) {
		// Oops, terminated suddenly
		return NULL;
	}
	p = DeBlank(p);
	if (IsTerminator(*p)) {
		// Oops, terminated suddenly again
		return NULL;
	}
	return p;
}

char *GetItem(char *p, char **item)
{
	char buf[BUFSIZ];
	char *base;
	char *duped;
	char *pnext;
	int i;

	//printf("*** GetIem: p=%s\n", p); //DEBUG
	base = &buf[0];
	for(i = 0; i < BUFSIZ; i++) {
		if (*p == ',' || IsTerminator(*p))
			break;
		*base++ = *p++;
	}
	pnext = p + (*p == ','); // if ',', forward one char
	*base = '\0';
	duped = strdup(buf);
	if (duped == NULL) {
		Error("duped == NULL");
	}
	else {
		*item = duped;
	}
	return pnext;
}

int DecodeLine(char *Line, uint *Id, char **Eep, char **Desc, char ***SCuts)
{
	char *p;
	char *item;
	int scCount = 0;
	char **scTable;
	int i;

	scTable = (char **) malloc(sizeof(char *) * SC_SIZE);
	if (scTable == NULL) {
		Error("cannot alloc scTable");
		return 0;
	}

	p = GetItem(DeBlank(Line), &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		if (item)
			free(item);
		return 0;
	}
	//printf("**0: <%s><%s>\n", item, p);  //DEBUG
	*Id = strtoul(item, NULL, 16);

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read EEP item");
		return 0;
	}
	p = GetItem(p, &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		if (item)
			free(item);
		return 0;
	}
	*Eep = item;

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read Desc item");
                if (item)
                        free(item);
		return 0;
	}
	p = GetItem(p, &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		if (item)
			free(item);
		return 0;
	}
	*Desc = item;

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read SCut first item");
		if (item)
                        free(item);
		return 0;
	}

	for(i = 0; i < SC_SIZE; i++) {
		p = GetItem(p, &item);
		if (p == NULL) {
			if (item)
				free(item);
			break;
		}
		scTable[i] = item;

		if ((p = CheckNext(p)) == NULL) {
			//End of line
			break;
		}
	}
	*SCuts = (char **) scTable;
	scCount = i + 1;
	return scCount;
}

PROFILE_CACHE *GetCache(char *Eep)
{
	int i;
	PROFILE_CACHE *pp = &CacheTable[0];
        UINT64 *eepKey = (UINT64 *) Eep;

        for(i = 0; i < NODE_TABLE_SIZE; i++) {
                if (*eepKey == pp->Eep.Key) {
                        break;
                }
                else if (pp->Eep.Key == 0ULL) {
			// reach to end, not found
                        return NULL;
                }
		pp++;
        }
	if (i == NODE_TABLE_SIZE) {
		// not found
		pp = NULL;
	}
	return pp;
}

//inline double CalcA(double x1, double y1, double x2, double y2)
double CalcA(double x1, double y1, double x2, double y2)  
{
	return (double) (y1 - y2) / (double) (x1 - x2);
}

//inline double CalcB(double x1, double y1, double x2, double y2)
double CalcB(double x1, double y1, double x2, double y2)
{
	return (((double) x1 * y2 - (double) x2 * y1) / (double) (x1 - x2)); 
}

int AddCache(char *Eep)
{
	EEP_TABLE *pe = GetEep(Eep);
	DATAFIELD *pd;
	PROFILE_CACHE *pp = &CacheTable[0];
	UNIT *pu;
	int i;
	int points = 0;

	if (pe == NULL) {
		// not found
		return 0;
	}

        for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (pp->Eep.Key == 0ULL) {
			// empty slot
			break;
		}
		pp++;
	}
	if (i == NODE_TABLE_SIZE) {
		// not found
		return 0;
	}
	pp->Eep.Key = *((UINT64 *) pe->Eep);
	pp->Padding = 0;
	pu = &pp->Unit[0];
	pd = &pe->Dtable[0];

	//printf("*%s: sz:%d pu=%p\n", __FUNCTION__, pe->Size, pu);
	pd = &pe->Dtable[0];
	for(i = 0; i < pe->Size; i++) {
		if (pd->ShortCut == NULL) {
			pd++;
			continue;
		}
                else if (!strcmp(pd->DataName, "LRN Bit") || !strcmp(pd->ShortCut, "LRNB")
			 || !strcmp(pd->DataName, "Learn Button")) {
			pd++;
			continue; //Skip Learn bit
		}
		//printf("*%s: %d=%s,%s pu:%p\n", __FUNCTION__, i, pd->ShortCut, pd->DataName, pu);

		pu->SCut = pd->ShortCut;
		pu->DName = pd->DataName;
		pu->FromBit = pd->BitOffs;
		pu->SizeBit = pd->BitSize;
		pu->Unit = pd->Unit;
		pu->Slope = CalcA(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);
		pu->Offset = CalcB(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);
		pu++, pd++, points++;
	}

	PrintProfileAll();

	return points;
}

int ReadCsv(char *Filename)
{
	char buf[BUFSIZ];
	FILE *fd;
	NODE_TABLE *nt;
	uint id;
	char *eep;
	char *desc;
	char **scs;
	int scCount;
	int lineCount = 0;

	nt = &NodeTable[0];
	fd = fopen(Filename, "r");
	if (fd == NULL) {
		Error("Open error");
		return 0;
	}
	while(true) {
		char *rtn = fgets(buf, BUFSIZ, fd);
		if (rtn == NULL) {
			//printf("*fgets: EOF found\n");
			break;
		}
		scCount = DecodeLine(buf, &id, &eep, &desc, &scs);
		if (scCount > 0) {
			if (nt->SCuts) {
				//purge old shortcuts
				; ////free(nt->SCuts);
			}
			nt->Id = id;
			nt->Eep = eep;
			nt->Desc = desc;
			nt->SCuts = scs;
			nt->SCCount = scCount;
			nt++;
			lineCount++;
			if (lineCount >= NODE_TABLE_SIZE) {
				Error("Node Table overflow");
				break;
			}
		}
		if (!GetCache(eep)) {
			scCount = AddCache(eep);
			if (scCount == 0) {
				;; //Error("AddCache() error");
			}
			//printf("*%s: AddCache(%s)=%d\n", __FUNCTION__,
			//       eep, scCount);
		}
	}
	nt->Id = 0; //mark EOL
	fclose(fd);

	return lineCount;
}

int ReadCmd(char *Filename, int *Mode, char *Param)
{
	FILE *fd;
	int mode = 0;
	char param  = 0;
	char buffer[BUFSIZ];
	static int lastMode = 0;
	static char lastBuffer[BUFSIZ] = {'\0'};

	fd = fopen(Filename, "r");
	if (fd == NULL) {
		Error("Open error");
		return 0;
	}

	fscanf(fd, "%d %s", &mode, buffer);
	//printf("mode=%d buf=%s\n", mode, buffer);
	if (mode <= 0 || mode > 6) {
		fclose(fd);
		return 0;
	}
	else if (mode == lastMode && !strcmp(buffer, lastBuffer)) {
		// the same cmd&option, not changed
		fclose(fd);
		return 0;
	}

	param = toupper(buffer[0]);
	switch(param) {
	case 'O': //Operation
		//printf("0\n");
		break;
	case 'R': //Register
		//printf("1\n");
		break;
	case 'M': //Monitor
		//printf("2\n");
		break;
	case 'C': //Clear
		//printf("3\n");
		break;
	case 'V': //Verbose
		//printf("4\n");
		break;
	case 'S': //Silent
		//printf("5\n");
		break;
	case 'D': //Debug
		//printf("6\n");
		break;
	default:
		//fclose(fd);
		//return 0;
		break;
	}
	//printf("end param=%c\n", param);
	if (Param) 
		strcpy(Param, buffer);
	lastMode = *Mode = mode;
	strcpy(lastBuffer, buffer);
	fclose(fd);
	//printf("$NewCmd=%d %c %s\n", mode, param, buffer);
	return 1;
}

uint GetId(int Index)
{
	NODE_TABLE *nt = &NodeTable[Index];
	return (nt->Id);
}

void WriteRpsBridgeFile(uint Id, byte *Data)
{
	int i;
	NODE_TABLE *nt = GetTableId(Id);
	PROFILE_CACHE *pp;
	UNIT *pu;
	char *fileName;
	BYTE rawData;
	BYTE switchData = 0;
	
	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}
	
	LogMessageStart(Id, pp->Eep.String);
	
        //F6-02-04
	if (!strcmp(pp->Eep.String, "F6-02-04")) {
		pu = &pp->Unit[0];
		rawData = Data[0];
		for(i = 0; i < nt->SCCount; i++) {
			fileName = nt->SCuts[i];
			switchData = (rawData >> pu->FromBit) & (0x01);
			WriteBridge(fileName, switchData, "\0");
			printf("*%d: %s.%s=%d (%02X)\n", i,
			       fileName, pu->SCut, switchData, rawData);
			pu++;
		}
	}
	//F6-02-01
	else /*if (!strcmp(pp->Eep.String, "F6-02-01")) */ {
		BYTE nu = Data[1] & 0x10;
		BYTE first, second;
		BYTE eb, sa;

		first = (Data[0] >> 5) & 0x07;
		eb = (Data[0] >> 4) & 0x01;
		if (nu) {
			second = (Data[0] >> 1) & 0x07;
			sa = Data[0] & 0x01;
		}
		else { /* !nu */
			second = sa = 0;
		}
		
		pu = &pp->Unit[0];
		for(i = 0; i < 6; i++) {
			fileName = nt->SCuts[i];
			switch(i) {
			case 0:
				switchData = nu ? first : 0;
				break;
			case 1:
				switchData = nu ? eb : 0;
				break;
			case 2:
				switchData = second;
				break;
			case 3:
				switchData = sa;
				break;
			case 4:
				switchData = nu ? 0 : first;
				break;
			case 5:
				switchData = nu ? 0 :sa;
				break;
			}
			WriteBridge(fileName, switchData, "\0");
			printf("*%d: %s.%s=%d (%02X %02X)\n", i,
			       fileName, pu->SCut, switchData, Data[0], Data[1]);
			pu++;
		}
		
	}
}

void Write1bsBridgeFile(uint Id, byte *Data)
{
	int i;
	NODE_TABLE *nt = GetTableId(Id);
	PROFILE_CACHE *pp;
	UNIT *pu;
	char *fileName;
	BYTE rawData;
	uint switchData = 0;
	
	if (nt == NULL) {
		Error("cannot find id");
		return;
	}

	pp = GetCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}
	
	LogMessageStart(Id, pp->Eep.String);

        //D5-00-01
	pu = &pp->Unit[0];
	rawData = Data[0];
	for(i = 0; i < nt->SCCount; i++) {
		fileName = nt->SCuts[i];
		switchData = rawData & 0x01;
		WriteBridge(fileName, switchData, "\0");
		//printf("*%d: %s.%s=%d (%02X)\n", i,
		//       fileName, pu->SCut, switchData, rawData);
		pu++;
	}
}

void Write4bsBridgeFile(uint Id, byte *Data)
{
	int i;
	NODE_TABLE *nt = GetTableId(Id);
	PROFILE_CACHE *pp;
	UNIT *pu;
	uint rawData = (Data[0]<<24) | (Data[1]<<16) | (Data[2]<<8) | Data[3];
	int partialData;
	double convertedData;
	const uint bitMask = 0xFFFFFFFF;
	char *fileName;
#define SIZE_MASK(n) (bitMask >> (32 - (n)))

	//printf("*%s: %08X data=%02X %02X %02X %02X scnt=%d\n", __FUNCTION__,
	//       Id, Data[0], Data[1], Data[2], Data[3], nt->SCCount);

	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}

	LogMessageStart(Id, pp->Eep.String);

	pu = &pp->Unit[0];
	for(i = 0; i < nt->SCCount; i++) {
		fileName = nt->SCuts[i];
		partialData = (rawData >> (31 - (pu->FromBit + pu->SizeBit - 1))) & SIZE_MASK(pu->SizeBit);
		convertedData = partialData * pu->Slope + pu->Offset;

		//printf("****%s:R=%u PD=%d fr=%u sz=%u sl=%.2lf of=%.2lf dt=%.2lf\n",
		//       fileName, rawData, partialData, pu->FromBit, pu->SizeBit,
		//       pu->Slope, pu->Offset, convertedData);

		WriteBridge(fileName, convertedData, pu->Unit);

		//printf("*%d: %s.%s=%d/%.2lf,f=%d z=%d s=%.2lf o=%.2lf\n", i,
		//       fileName, pu->SCut, partialData, convertedData,
		//       pu->FromBit, pu->SizeBit, 
		//       pu->Slope, pu->Offset);
		pu++;
	}
#undef SIZE_MASK
}

void WriteVldBridgeFile(uint Id, byte *Data)
{
	int i;
	NODE_TABLE *nt = GetTableId(Id);
	PROFILE_CACHE *pp;
	UNIT *pu;
	ULONG rawData;
	ULONG partialData;
	double convertedData;
	enum { bitMask = 0xFFFFFFFFFFFFFFFFULL };
	UINT maxBitOffset;
	UINT tmpBitOffset;
	UINT actualByteSize;
	char *fileName;
#define SIZE_MASK(n) (bitMask >> (64 - (n)))

	//printf("*%s: %08X data=%02X %02X %02X %02X %02X %02X scnt=%d\n", __FUNCTION__,
	//       Id, Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], nt->SCCount);

	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}

	LogMessageStart(Id, pp->Eep.String);

	// Examine the total byte counts in VLD data packet
	maxBitOffset = 0;
	pu = &pp->Unit[0];
	for(i = 0; i < nt->SCCount; i++) {
		tmpBitOffset = pu->FromBit + pu->SizeBit - 1;
		if (maxBitOffset < tmpBitOffset) {
			maxBitOffset = tmpBitOffset;
		}
		pu++;
	}
	actualByteSize = (maxBitOffset + 7) / 8;

	rawData = Data[0];
	// Caluculate the raw data word;
	for(i = 1; i < actualByteSize; i++) {
		//printf("**** rawData=%lx\n", rawData);
		rawData <<= 8;
		//printf("**** rawData=%lx\n", rawData);
		rawData |= Data[i];
		//printf("**** rawData=%lx\n", rawData);
	}
	//printf("**** maxBitOffset=%u actualByteSize=%u rawData=%lu(0x%0lx)\n",
	//       maxBitOffset, actualByteSize, rawData, rawData);
	pu = &pp->Unit[0];
	for(i = 0; i < nt->SCCount; i++) {
		int maxBitPos = actualByteSize * 8 -1;
		int newFromBit = maxBitPos - (pu->FromBit + pu->SizeBit - 1);

		fileName = nt->SCuts[i];
		partialData = (rawData >> newFromBit) & SIZE_MASK(pu->SizeBit);

                // SIZE_MASK(n) (bitMask >> (64 - (n)))		
		//printf("****SIZE_MASK=%lx\n", SIZE_MASK(pu->SizeBit));

		convertedData = partialData * pu->Slope + pu->Offset;
		//printf("****%s:R=%lu PD=%lu fr=%u nw=%u sz=%u sl=%.2lf of=%.2lf dt=%.2lf\n",
		//       fileName, rawData, partialData, pu->FromBit, newFromBit, pu->SizeBit,
		//       pu->Slope, pu->Offset, convertedData);
		WriteBridge(fileName, convertedData, pu->Unit);

		//printf("*%d: %s.%s=%lu/%.2lf,f=%u z=%u s=%.2lf o=%.2lf\n", i,
		//       fileName, pu->SCut, partialData, convertedData,
		//       pu->FromBit, pu->SizeBit, 
		//       pu->Slope, pu->Offset);
		pu++;
	}
#undef SIZE_MASK
}

int PrintPoint(char *Eep, int Count)
{
        PROFILE_CACHE *pp = GetCache(Eep);
        UNIT *pu;
        int i;

	printf("*PP:pp=%p(%s) eep=%s cnt=%d\n", pp, pp->Eep.String, Eep, Count); 

        pu = &pp->Unit[0];
        for(i = 0; i < Count; i++) {
                printf("#%d(%p) p:%s n:%s u:%s f:%d s:%d S:%.2lf O:%.2lf\n",
		       i, pu,
                       pu->SCut,
                       pu->DName,
                       pu->Unit,
                       pu->FromBit,
                       pu->SizeBit,
                       pu->Slope,
                       pu->Offset);
		pu++;
        }
        return i;
}

void PrintItems()
{
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

	printf(">>%s\n", __FUNCTION__);
	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}
		printf("%d: %08X %s <%s> ",
		       i, nt->Id, nt->Eep, nt->Desc);

		ps = nt->SCuts;
		for(j = 0; j < nt->SCCount && *ps != NULL; j++) {
			printf("\'%s\'", *ps++);
		}
		printf("\n");

		PrintPoint(nt->Eep, nt->SCCount);

		nt++;
	}
	printf("<<%s\n", __FUNCTION__);
}

void PrintSCs()
{
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}

		ps = nt->SCuts;
		for(j = 0; j < nt->SCCount && *ps != NULL; j++) {
			printf("\'%s\'\n", *ps++);
		}
		nt++;
	}
}

void PrintProfileAll()
{
	int i, j;
	PROFILE_CACHE *pp = &CacheTable[0];
	UNIT *pu;

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		printf("*%s: %d: %s %p\n", __FUNCTION__, i, pp->Eep.String, &pp->Unit[0]);
		
		pu = &pp->Unit[0];
		for(j = 0; j < SC_SIZE; j++) {
			if (pu->SCut == NULL || *pu->SCut == '\0')
				break;
			printf(" %d: SCut=%s Unit=%s\n",
			       j, pu->SCut, pu->Unit);
			pu++;
		}
		pp++;
		if (pp->Eep.Key == 0ULL)
			break;
	}
}

#include <linux/limits.h> //PATH_MAX

//#define EO_DIRECTORY "/var/tmp/dpride"
//#define EO_CONTROL_FILE "eofilter.txt"
//#define EO_FILTER_SIZE (128)

typedef struct _eodata {
	int  Index;
	int  Id;
	char *Eep;
	char *Name;
	char *Desc;
	int  PIndex;
	int  PCount;
	double Value;
}
EO_DATA;

char *EoMakePath(char *Dir, char *File);
void EoReflesh(void);
EO_DATA *EoGetDataByIndex(int Index);

static EO_DATA EoData;
static int LastIndex;
static int LastPoint;

char *EoMakePath(char *Dir, char *File)
{
        char path[PATH_MAX];
        char *pathOut;

        if (File[0] == '/') {
                /* Assume absolute path */
                return(strdup(File));
        }
        strcpy(path, Dir);
        if (path[strlen(path) - 1] != '/') {
                strcat(path, "/");
        }
        strcat(path, File);
        pathOut = strdup(path);
        if (!pathOut) {
                Error("strdup() error");
        }
        return pathOut;
}

void EoReflesh(void)
{
	char *fname = EoMakePath(EO_DIRECTORY, EO_CONTROL_FILE);
	if (fname) {
		ReadCsv(fname);
		////free(fname);
	}
}

EO_DATA *EoGetDataByIndex(int Index)
{
        NODE_TABLE *nt = &NodeTable[Index];
	char *fileName;
        FILE *f;
	char buf[BUFSIZ];
        char *bridgePath;
	char *pt;
	double value = 0.0;
	EO_DATA *pe = &EoData;
	int i;

	//printf("%s: %u:id=%08u eep=%s cnt=%d\n", __FUNCTION__,
	//       Index, nt->Id, nt->Eep, /*nt->Desc,*/ nt->SCCount);
	if (Index != LastIndex || LastPoint >= nt->SCCount) {
		LastPoint = 0;
	}

	i = LastPoint;
	fileName = nt->SCuts[i];
	if (fileName == NULL) {
		//printf("%s:%d fileName is NULL\n",
		//       __FUNCTION__, i);
		LastPoint = 0;
		return NULL;
	}
	bridgePath = EoMakePath(EO_DIRECTORY, fileName);
	if (bridgePath) {
		f = fopen(bridgePath, "r");
		if (f == NULL) {
			fprintf(stderr, "Cannot open=%s\n", bridgePath);
			LastPoint = 0;
			return NULL;
		}
		////free(bridgePath);
	}
	else {
		fprintf(stderr, "EoMakePath error %s\n", bridgePath);
		return NULL;
	}
	while(fgets(buf, BUFSIZ, f) != NULL) {
		value = strtod(buf, &pt);
		if (pt != buf)
			break;
	}
	fclose(f);
	pe->Index = Index;
	pe->Id = nt->Id;
	pe->Eep = nt->Eep;
	pe->Name = fileName;
	pe->Desc = nt->Desc;
	pe->PIndex = i;
	pe->PCount = nt->SCCount;
	pe->Value = value;

	LastPoint++;

	return pe;
}
