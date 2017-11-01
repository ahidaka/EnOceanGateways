#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include "dpride.h"
#include "ptable.h"

#define SC_SIZE 16
#define NODE_TABLE_SIZE 256

//
// Control file node table
//
typedef struct _node_table {
	uint Id;
        char *Eep;
	char *Desc;
	int SCCount;
	char **SCuts;
} NODE_TABLE;

NODE_TABLE NodeTable[NODE_TABLE_SIZE];

//
// Profile cache
//
typedef struct _unit {
        char *SCut;
	char *Unit;
        int FromBit;
        int SizeBit;
        double Slope;
        double Offset;
} UNIT;

typedef struct _profile_cache {
        union _eep {
                char String[8];
                uint Key;
        }  Eep;

	UNIT Unit[SC_SIZE];

} PROFILE_CACHE;

PROFILE_CACHE CacheTable[NODE_TABLE_SIZE];


/* EEP format
//String
//Packed
//Bin3

packed_eep = EepStringToPacked(str);
(bool) EepStringToBin3(str, b1, b2, b3);
(bool) EepBin3ToString();
(bool) EepBin3ToPacked();
(bool) EepPackedToBin3();
*/

NODE_TABLE *GetTableId(uint Target);

PROFILE_CACHE *GetCache(char *Eep);


char *MakeNewName(char *Original, int Suffix)
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
		if (nt->Id == 0) {
			break;
		}
		else if (Target == nt->Id) {
			found = true;
			break;
		}
		nt++;
	}
	return found ? nt : NULL;
}

bool CheckTableId(uint Target)
{
	bool collision = false;
	int i;
	NODE_TABLE *nt = &NodeTable[0];

	printf("Target=%08x\n", Target);

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}
		else if (Target == nt->Id) {
			collision = true;
			break;
		}
		nt++;
	}
	return collision;
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
			//printf("\'%s\'\n", *ps++);
			if (!strcmp(Target, *ps++)) {
				//matched
				collision = true;
				break;
			}
		}
		if (collision)
			break;
		nt++;
	}
	return collision;
}

char *GetNewName(char *Target)
{
	int suffix;
	char *newName;
	char *origName;

	if (CheckTableEep(Target)) {
		// Find collision
		origName = Target;
		Target = NULL; // Clear for new Name
		for(suffix = 1; suffix <= 99; suffix++) {
			newName = MakeNewName(origName, suffix);
			if (!CheckTableEep(newName)) {
				Target = newName;
				printf("NEW:%s not mached\n", Target);
				break;
			}
		}
	}

	else 
		printf("ORG:%s not mached\n", Target);

	return Target;
}


inline bool IsTerminator(char c)
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
		return 0;
	}
	*Eep = item;

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read Desc item");
		return 0;
	}
	p = GetItem(p, &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		return 0;
	}
	*Desc = item;

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read SCut first item");
		return 0;
	}

	for(i = 0; i < SC_SIZE; i++) {
		p = GetItem(p, &item);
		if (p == NULL) {
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
        uint *eepKey = (uint *) Eep;

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

inline double CalcA(double x1, double y1, double x2, double y2)
{
	return (double) (y1 - y2) / (double) (x1 - x2);
}

inline double CalcB(double x1, double y1, double x2, double y2)
{
	return ((double) x1 * y2) - ((double) x2 * y1) / ((double) (x1 - x2)); 
}

int AddCache(char *Eep)
{
	EEP_TABLE *pe = GetEep(Eep);
	DATAFIELD *pd;
	PROFILE_CACHE *pp = &CacheTable[0];
	UNIT *pu;
	int i;

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
	pp->Eep.Key = *((uint *) pe->Eep);
	pu = &pp->Unit[0];
	pd = &pe->Dtable[0];

	for(i = 0; i < pe->Size; i++) {
		pu->SCut = pd->ShortCut;
		pu->FromBit = pd->BitOffs;
		pu->SizeBit = pd->BitSize;
		pu->Unit = pd->Unit;
		pu->Slope = CalcA(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);
		pu->Offset = CalcB(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);

		if (pd->ShortCut == NULL) {
			break;
		}
		pu++,pd++;
	}
	return i;
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
		//printf("*fgets:%s\n", rtn); //DEBUG

		scCount = DecodeLine(buf, &id, &eep, &desc, &scs);
		if (scCount > 0) {
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
				;; //Error("AddCache89 error");
			}
		}
	}
	nt->Id = 0; //mark EOL
	
	fclose(fd);

	return lineCount;
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
	//PROFILE_CACHE *pp;
	char *fileName;
	uint mask = 1;
	uint switchData = 0;

	if (nt == NULL) {
		Error("cannot find id");
		return;
	}

	// Get data
        for (i = 0; i < 4; i++)
        {
                switch (Data[0] & mask)
                {
		case 0x01:
			switchData |= 0x01;
			break;
		case 0x02:
			switchData &= (uint) 0xFFFFFFFE;
			break;
		case 0x04:
			switchData |= 0x02;
			break;
		case 0x08:
			switchData &= (uint)0xFFFFFFFD;
			break;
                } //switch
                mask <<= 1;
        } // for
	fileName = nt->SCuts[0];
	WriteBridge(fileName, switchData);
}

void Write4bsBridgeFile(uint Id, byte *Data)
{
	int i;
	NODE_TABLE *nt = GetTableId(Id);
	PROFILE_CACHE *pp;
	UNIT *pu;
	uint rawData = *((uint *) Data);
	int partialData;
	double convertedData;
	const uint bitMask = 0xFFFFFFFF;
	char *fileName;
#define SIZE_MASK(n) (bitMask >> (32 - (n)))

	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}

	pu = &pp->Unit[0];
	for(i = 0; i < nt->SCCount; i++) {
		fileName = nt->SCuts[i];
		partialData = (rawData << pu->FromBit) & SIZE_MASK(pu->SizeBit);
		convertedData = partialData * pu->Slope + pu->Offset;
		WriteBridge(fileName, convertedData);
		pu++;
	}
#undef SIZE_MASK
}

void PrintItems()
{
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

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
		nt++;
	}
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
