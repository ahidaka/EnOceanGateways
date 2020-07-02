#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include "typedefs.h"
#include "utils.h"
#include "dpride.h"
#include "ptable.h"
#include "../eolib/models.h"

//#define EDX_DEBUG (1)
//#define CMD_DEBUG (1)
//#define EEP_DEBUG (1)
//#define VLD_DEBUG (1)
//#define CD_DEBUG (1)
//#define MODEL_DEBUG (1)

extern void PrintProfileAll(void);
extern NODE_TABLE NodeTable[];

//
// static table
//
PROFILE_CACHE CacheTable[NODE_TABLE_SIZE];

extern const char *_value_type_string[];

//
//
//
NODE_TABLE *GetTableId(UINT Target);

PROFILE_CACHE *GetEepCache(char *Eep);

char *MakeNewName(char *Original, INT Suffix)
{
	int len = strlen(Original);
	char *buf;

	buf = malloc(len + 4); // strlen(".999") == 4
	if (buf == NULL) {
		Error("cannot alloc buffer");
		return NULL;
	}
	sprintf(buf, "%s%d", Original, Suffix);
#ifdef EEP_DEBUG
	printf("!!MakeNewName:%s!\n", buf);
#endif
	return buf;
}

char *GetTableEep(uint TargetId)
{
	NODE_TABLE *nt = GetTableId(TargetId);
	return (nt == NULL ? NULL : nt->Eep);
}

bool CheckTableSCut(char *SCut)
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
#ifdef EEP_DEBUG
			printf("!!%s: \'%s\'\n", SCut, *ps);
#endif
			if (!strcmp(SCut, *ps++)) {
				//matched
				collision = true;
				break;
			}
		}
		if (collision) {
#ifdef EEP_DEBUG
			printf("!!%s:%s,collision: \n", SCut, *ps);
#endif
			break;
		}
		nt++;
	}
	return collision;
}

bool CheckTableSCutWithCurrent(char *SCut, char **list)
{
	bool collision = false;
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

	while(*list != NULL) {
		if (!strcmp(SCut, *list)) {
			collision = true;
			return collision;
		}
		list++;
	}

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}
		ps = nt->SCuts;
		for(j = 0; j < nt->SCCount && *ps != NULL; j++) {
			//printf("++%s: \'%s\'\n", SCut, *ps);
			if (!strcmp(SCut, *ps++)) {
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

char *GetNewName(char *Original)
{
	int suffix;
	char *newName;

	if (CheckTableSCut(Original)) {
#if EEP_DEBUG
		printf("!!DEBUG:Found collision<%s>\n", Original);
#endif
		for(suffix = 1; suffix <= MAX_SUFFIX; suffix++) {
			newName = MakeNewName(Original, suffix);
			if (!CheckTableSCut(newName)) {
#if EEP_DEBUG
				printf("!!DEBUG: !CheckTableSCut(newName=%s)\n", newName);
#endif
				return newName;
			}
			else {
				printf("!!DEBUG: *CheckTableSCut(newName=%s)\n", newName);
			}
		}
	}
	else {
#if EEP_DEBUG		
		printf("ORG:%s not mached\n", Original);
#endif
	}
	return strdup(Original);
}

char *GetNewNameWithCurrent(char *Original, char **List)
{
	int suffix;
	char *newName;

	if (CheckTableSCutWithCurrent(Original, List)) {
		// Found collision
		for(suffix = 1; suffix <= MAX_SUFFIX; suffix++) {
			newName = MakeNewName(Original, suffix);
			if (!CheckTableSCutWithCurrent(newName, List)) {
				return newName;
			}
		}
	}
	else {
		; //printf("ORG:%s not mached\n", Target);
	}
	return strdup(Original);
}

PROFILE_CACHE *GetEepCache(char *Eep)
{
	int i;
	PROFILE_CACHE *pp = &CacheTable[0];

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (!strcmp(Eep, pp->StrKey)) {
				break;
		}
		else if (pp->StrKey[0] == '\0') {
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

int AddEepCache(char *Eep)
{
	EEP_TABLE *pe = GetEep(Eep);
	CM_TABLE *pmc;
	DATAFIELD *pd;
	PROFILE_CACHE *pp = &CacheTable[0];
	UNIT *pu;
	int i;
	int points = 0;
	int count;
	BOOL isModel = FALSE;

        for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (pp->StrKey[0] == '\0') {
			// empty slot
			break;
		}
		pp++;
	}
	if (i == NODE_TABLE_SIZE) {
		// not found
		return 0;
	}
	strcpy(pp->StrKey, Eep);
	pu = &pp->Unit[0];
	if (pe != NULL && Eep[2] == '-' ) {
		pd = &pe->Dtable[0];
		count = pe->Size;
	}
	else if ((pmc = CmGetCache(Eep)) != NULL) {
		isModel = TRUE;
		pd = &pmc->Dtable[0];
		count = pmc->Count;
#if 0
		////////
		for(i = 0; i < count; i++) {
			printf("******* %d:%s=%s\n", i,
			       pd->ShortCut, pd->DataName);
			pd++;
		}
		////////
#endif
	}
	else {
		Error("Cannot find EEP and CM");
		return 0;
	}

	//printf("*%s: sz:%d pu=%p\n", __FUNCTION__, pe->Size, pu);
	pd = isModel ? &pmc->Dtable[0] : &pe->Dtable[0];
	for(i = 0; i < count; i++) {
		if (pd->ShortCut == NULL) {
			pd++;
			continue;
		}
                else if (!strcmp(pd->DataName, "LRN Bit") || !strcmp(pd->ShortCut, "LRNB")
			 || !strcmp(pd->DataName, "Learn Button")) {
			pd++;
			continue; //Skip Learn bit
		}

		////////
		//printf("*%s: %d=%s,%s pu:%p\n", __FUNCTION__, i, pd->ShortCut, pd->DataName, pu);
		////////
		
		pu->ValueType = pd->ValueType;
		pu->SCut = pd->ShortCut;
		pu->DName = pd->DataName;
		pu->FromBit = pd->BitOffs;
		pu->SizeBit = pd->BitSize;
		pu->Unit = pd->Unit;
		if (pd->ValueType == VT_Data) {
			if (isModel) {
				pu->Slope = (pd->ScaleMax - pd->ScaleMin) / pd->RangeMax;
				pu->Offset = pd->ScaleMin; 
			}
			else {
				pu->Slope = CalcA(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);
				pu->Offset = CalcB(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);
			}
#if 0
			printf("*%s:%d:%s: sl=%f of=%f %f %f %d\n", __FUNCTION__, i, isModel ? "Model" : "EEP",
			       pu->Slope, pu->Offset, pd->ScaleMax, pd->ScaleMin, pd->RangeMax);
#endif
		}
		else {
			pu->Slope = 1;
			pu->Offset = 0;
		}
		pu++, pd++, points++;
	}

	//PrintProfileAll();

	return points;
}

//
// Read profile table, then check and cache EEP and Model cache
//
int CacheProfiles(void)
{
	NODE_TABLE *nt;
	//uint id;
	char *eep;
	int scCount;
	//CM_TABLE *model;
	int lineCount = 0;

	nt = &NodeTable[0];

	while(true) {
		if (nt->Id == 0) { // found EOL 
			break;
		}
		eep = nt->Eep;
		if (!GetEepCache(eep)) {
			scCount = AddEepCache(eep);
			if (scCount == 0) {
				;; //Error("AddEepCache() error");
			}
			//printf("*%s: AddEepCache(%s)=%d\n", __FUNCTION__,
			//       eep, scCount);
		}
		nt++;
		lineCount++;
	}

	return lineCount;
}

int ReadModel(char *Filename)
{
	const int bufferSize = BUFSIZ / 4;
	char buf[bufferSize];
	BYTE binArray[bufferSize];
	FILE *fd;
	char *s;
	CM_TABLE *pmc;
	int length;
	int count = 0;

	fd = fopen(Filename, "r");
	if (fd == NULL) {
		Error("Open error");
		return 0;
	}

	CmCleanUp();

	do {
		memset(buf, 0, bufferSize);
		memset(binArray, 0, bufferSize);
		s = fgets(buf, bufferSize, fd);
		if (s == NULL || *s == '\0')
				break;

		if (buf[strlen(buf) - 1] == '\n') {
				buf[strlen(buf) - 1] = '\0';
		}

		length = CmTextToBin(buf, binArray);
		pmc = CmGetModel((BYTE *)binArray, length);
		if (pmc != NULL && pmc->Count > 0) {
#ifdef MODEL_DEBUG
			int i;
			printf("####[%s] '%s' count=%d len=%d\n\n\n",
				pmc->CmStr, pmc->Title, pmc->Count, length);
			for(i = 0; i < pmc->Count; i++) {
				DATAFIELD *pd = &pmc->Dtable[i];
				if (pd->ShortCut == NULL)
					break;
				printf("#### %d:%s-%s[%s] %d,%d %d,%d %f,%f(%s)\n",
					i, _value_type_string[pd->ValueType & 3],
					pd->DataName,
					pd->ShortCut,
					pd->BitOffs,
					pd->BitSize,
					pd->RangeMin,
					pd->RangeMax,
					pd->ScaleMin,
					pd->ScaleMax,
					pd->Unit);
			}
#endif
			count++;
		}
		else {
			fprintf(stderr, "CmGetModel error count=%d!\n", count);
			break;
		}
	}
	while(s != NULL);

	fclose(fd);

	return count;
}

int ReadCmd(char *Filename, int *Mode, char *Param)
{
	FILE *fd;
	int mode = 0;
	int rtnValue = 0;
	char param  = 0;
	char buffer[BUFSIZ / 8];
	static int lastMode = 0;
	static char lastBuffer[BUFSIZ / 8] = {'\0'};
#if CMD_DEBUG
#define DPRINT(p) printf(p)
#else
#define DPRINT(p)
#endif
	
	fd = fopen(Filename, "r");
	if (fd == NULL) {
		Error("Open error");
		return rtnValue;
	}

	fscanf(fd, "%d %s", &mode, buffer);
	//printf("mode=%d buf=%s\n", mode, buffer);
	if (mode <= 0 || mode > 6) {
#if CMD_DEBUG
		printf("!C!DEBUG: enter=%d\n", mode);
#endif
		fclose(fd);
		return rtnValue;
	}
	else if (mode == lastMode && !strcmp(buffer, lastBuffer)) {
		// the same cmd&option, not changed
#if CMD_DEBUG
		printf("!C!DEBUG: the same cmd&option=%d:%d:%s\n", lastMode, mode, buffer);
#endif
		fclose(fd);
		return rtnValue;
	}

	param = toupper(buffer[0]);
	switch(param) {
	case 'O': //Operation
		DPRINT("!C!0 Operation\n");
		break;
	case 'R': //Register
		DPRINT("!C!1 Register\n");
		break;
	case 'M': //Monitor
		DPRINT("!C!2 Monitor\n");
		break;
	case 'C': //Clear
		DPRINT("!C!3 Clear\n");
		break;
	case 'V': //Verbose
		DPRINT("!C!4 Verbose\n");
		break;
	case 'S': //Silent
		DPRINT("!C!5 Silent\n");
		break;
	case 'D': //Debug
		DPRINT("!C!6 Debug\n");
		break;
	default:
#if CMD_DEBUG
		printf("!C!'%c' Other\n", param);
#endif
		//fclose(fd);
		//return 0;
		break;
	}
#if CMD_DEBUG
	printf("!C! end param='%c'\n", param);
#endif
	if (Param) 
		strcpy(Param, buffer);
	rtnValue = 1;
	lastMode = *Mode = mode;
	strcpy(lastBuffer, buffer);
	fclose(fd);
#if CMD_DEBUG
	printf("!C! NewCmd=%d %c %s\n", mode, param, buffer);
#endif
	return rtnValue;
#undef DPRINT
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
	pp = GetEepCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}
	
	LogMessageStart(Id, pp->StrKey, nt->Secure ? "!" : "");
	
        //F6-02-04
	if (!strcmp(pp->StrKey, "F6-02-04")) {
		pu = &pp->Unit[0];
		rawData = Data[0];
		for(i = 0; i < nt->SCCount; i++) {
			fileName = nt->SCuts[i];
			switchData = (rawData >> pu->FromBit) & (0x01);
			WriteBridge(fileName, switchData, "\0");
			//printf("*%d: %s.%s=%d (%02X)\n", i,
			//       fileName, pu->SCut, switchData, rawData);
			pu++;
		}
	}
	//F6-02-01
	else /*if (!strcmp(pp->StrKey, "F6-02-01")) */ {
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
			//printf("*%d: %s.%s=%d (%02X %02X)\n", i,
			//       fileName, pu->SCut, switchData, Data[0], Data[1]);
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

	pp = GetEepCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}
	
	LogMessageStart(Id, pp->StrKey, nt->Secure ? "!" : "");

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

#ifdef EDX_DEBUG
	printf("*%s: %08X data=%02X %02X %02X %02X scnt=%d\n", __FUNCTION__,
	       Id, Data[0], Data[1], Data[2], Data[3], nt->SCCount);
#endif
	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetEepCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}

	LogMessageStart(Id, pp->StrKey, nt->Secure ? "!" : "");

	pu = &pp->Unit[0];
	for(i = 0; i < nt->SCCount; i++) {
		fileName = nt->SCuts[i];
		partialData = (rawData >> (31 - (pu->FromBit + pu->SizeBit - 1))) & SIZE_MASK(pu->SizeBit);
		convertedData = pu->ValueType == VT_Data ? partialData * pu->Slope + pu->Offset : partialData;
#ifdef EDX_DEBUG
		printf("****%s:R=%u PD=%d fr=%u sz=%u sl=%.2lf of=%.2lf dt=%.2lf\n",
		       fileName, rawData, partialData, pu->FromBit, pu->SizeBit,
		       pu->Slope, pu->Offset, convertedData);
#endif
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
	ULONG64 partialData;
	double convertedData;
	char *fileName;

#if VLD_DEBUG
	printf("*%s: %08X data=%02X %02X %02X %02X %02X %02X scnt=%d\n", __FUNCTION__,
	       Id, Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], nt->SCCount);
#endif
	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetEepCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}

	LogMessageStart(Id, pp->StrKey, nt->Secure ? "!" : "");

	pu = &pp->Unit[0];
	for(i = 0; i < nt->SCCount; i++) {

		fileName = nt->SCuts[i];
		partialData = GetBits(&Data[0], pu->FromBit, pu->SizeBit);
		convertedData = pu->ValueType == VT_Data ? partialData * pu->Slope + pu->Offset : partialData;
#if VLD_DEBUG
		printf("****%s:PD=%llu tp=%u fr=%u sz=%u sl=%.2lf of=%.2lf dt=%.2lf\n",
		       fileName, partialData, pu->ValueType, pu->FromBit, pu->SizeBit,
		       pu->Slope, pu->Offset, convertedData);
#endif
		WriteBridge(fileName, convertedData, pu->Unit);

		//printf("*%d: %s.%s=%lu/%.2lf,f=%u z=%u s=%.2lf o=%.2lf\n", i,
		//       fileName, pu->SCut, partialData, convertedData,
		//       pu->FromBit, pu->SizeBit, 
		//       pu->Slope, pu->Offset);
		pu++;
	}
}

void WriteCdBridgeFile(uint Id, byte *Data)
{
	int i;
	NODE_TABLE *nt = GetTableId(Id);
	PROFILE_CACHE *pp;
	UNIT *pu;
	ULONG partialData;
	double convertedData;
	char *fileName;

#if CD_DEBUG
	CM_TABLE *pmc;
	DATAFIELD *pd;
	if (nt != NULL) {
		printf("**CD*%s: %08X data=%02X %02X %02X %02X %02X %02X scnt=%d\n", __FUNCTION__,
		       Id, Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], nt->SCCount);
		pmc = CmGetCache(nt->Eep);
		if (pmc != NULL) {
			pd = pmc->Dtable;
			for(i = 0; i < pmc->Count; i++) {
				printf("*%s(%d): %d %d =%d %f %f\n",
				       pd->ShortCut, pd->ValueType, pd->BitOffs, pd->BitSize,
				       pd->RangeMax, pd->ScaleMin, pd->ScaleMax);
				pd++;
			}
		}
	}
#endif
	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetEepCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}

	LogMessageStart(Id, pp->StrKey, nt->Secure ? "!" : "");

	pu = &pp->Unit[0];
	for(i = 0; i < nt->SCCount; i++) {
		fileName = nt->SCuts[i];

		partialData = GetBits(Data, pu->FromBit, pu->SizeBit);
		convertedData = pu->ValueType == VT_Data ? partialData * pu->Slope + pu->Offset : partialData;
#if CD_DEBUG
		printf("**CD*%s:TP=%d PD=%lu fr=%u sz=%u sl=%.2lf of=%.2lf dt=%.2lf\n",
		       fileName, pu->ValueType, partialData, pu->FromBit, pu->SizeBit,
		       pu->Slope, pu->Offset, convertedData);
#endif
		WriteBridge(fileName, convertedData, pu->Unit);
		pu++;
	}
}

void WriteSdBridgeFile(uint Id, byte *Data)
{
	int i;
	NODE_TABLE *nt = GetTableId(Id);
	PROFILE_CACHE *pp;
	UNIT *pu;
	ULONG partialData;
	double convertedData;
	char *fileName;
	int index;
	int count;
	int fromBit;
	const int headerSize = 4;
	const int indexSize = 6;

#if CD_DEBUG
	CM_TABLE *pmc = CmGetCache(nt->Eep);
	DATAFIELD *pd = pmc->Dtable;
	
	printf("**SD*%s: %08X data=%02X %02X %02X %02X %02X %02X scnt=%d\n", __FUNCTION__,
	       Id, Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], nt->SCCount);

	for(i = 0; i < pmc->Count; i++) {
		printf("*%s(%d): %d %d =%f %f\n",
		       pd->ShortCut, pd->ValueType, pd->BitOffs, pd->BitSize,
		       pd->RangeMin * pd->ScaleMin,
		       pd->RangeMin * pd->ScaleMax);
		pd++;
	}
#endif
	if (nt == NULL) {
		Error("cannot find id");
		return;
	}
	pp = GetEepCache(nt->Eep);
	if (pp == NULL) {
		Error("cannot find EEP");
		return;
	}

	LogMessageStart(Id, pp->StrKey, nt->Secure ? "!" : "");

	count = GetBits(Data, 0, headerSize); // SD Header
	fromBit = headerSize;

	for(i = 0; i < count; i++) {
		index = GetBits(Data, fromBit, indexSize);
		fromBit += indexSize;

		fileName = nt->SCuts[index];
		pu = &pp->Unit[index];
		partialData = GetBits(Data, fromBit, pu->SizeBit);
		convertedData = pu->ValueType == VT_Data ? partialData * pu->Slope + pu->Offset : partialData;
#if CD_DEBUG
		printf("**SD*%s:count=%d index=%d\n", fileName, count, index);
		printf("**SD*%s:PD=%lu fr=%u sz=%u sl=%.2lf of=%.2lf dt=%.2lf\n",
		       fileName, partialData, fromBit, pu->SizeBit,
		       pu->Slope, pu->Offset, convertedData);
#endif
		WriteBridge(fileName, convertedData, pu->Unit);
		fromBit += pu->SizeBit;
	}
}

int PrintPoint(char *Eep, int Count)
{
        PROFILE_CACHE *pp = GetEepCache(Eep);
        UNIT *pu;
        int i = 0;

	if (pp != NULL) {
		printf("*PP:pp=%p(%s) eep=%s cnt=%d\n", pp, pp->StrKey, Eep, Count); 

		pu = &pp->Unit[0];
		for(; i < Count; i++) {
			printf("#%d(%s:%d) p:%s n:%s u:%s f:%d s:%d S:%.2lf O:%.2lf\n",
			       i, _value_type_string[pu->ValueType & 3], pu->ValueType,
			       pu->SCut,
			       pu->DName,
			       pu->Unit,
			       pu->FromBit,
			       pu->SizeBit,
			       pu->Slope,
			       pu->Offset);
			pu++;
		}
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
		printf("*%s: %d: %s\n", __FUNCTION__, i, pp->StrKey);

		if (pp->StrKey == NULL || pp->StrKey[0] == '\0') {
			printf("No cached data now!\n");
			break;
		}
		pu = &pp->Unit[0];
		for(j = 0; j < SC_SIZE; j++) {
			if (pu->SCut == NULL || *pu->SCut == '\0')
				break;
			printf(" %d: Type=%s SCut=%s Unit=%s\n",
			       j, _value_type_string[pu->ValueType & 3],
			       pu->SCut, pu->Unit);
			pu++;
		}
		pp++;
		if (pp->StrKey[0] =='\0')
			break;
	}
}
