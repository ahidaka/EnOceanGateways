#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#include "../dpride/typedefs.h"
#include "../dpride/ptable.h"
#include "../dpride/utils.h"
#include "models.h"

//#define MODEL_DEBUG (1)

#ifndef ENGINEERING_FACT
#include "common-model.c"
#else
void CmPrintTI(BYTE *Buf, BYTE *Eepbuf, BYTE *Data, INT len) {}
void CmPrintTR(BYTE *Buf, BYTE *Data, INT len) {}
void CmPrintCD(BYTE *Buf, BYTE *Data, INT len) {}
void CmPrintSD(BYTE *Buf, BYTE *Data, INT len) {}

INT CmFpSearch(IN CM_TABLE *pmc, IN BYTE *Buf, IN INT Size)
{
	return -1;
}

VOID *CmAnalyze(IN BYTE *Start, IN INT Length, OUT INT *Count)
{
	return NULL;
}

char *CmMakeString(VOID *Handle)
{
	return "";
}
char *CmMakeTitle(VOID *Handle)
{
	return "";
}

DATAFIELD *CmMakeDataField(VOID *Handle)
{
	return NULL;
}

VOID CmDelete(VOID *Handle) {}
#endif

//#define BITTEST 1

//
// Internal cache table and index
//
static int CacheIndex = 0;
static CM_TABLE ModelCache[CM_CACHE_SIZE];

#ifdef BITTEST
static int count = 0;

void print(ULONG ul)
{
        printf("%d: %08lX\n", count++, ul);
}
#endif

INT CmTextToBin(IN char *src, IN BYTE *dst)
{
        int i;
        int length = 0;
        ULONG ul;

        for(i = 0; i < BUFSIZ / 2; i++) {
                ul = strtoul((const char *) src, &src, 16);
                *dst++ = (BYTE) ul & 0xFF;
                length++;
                if (*src == ' ') {
                        src++;
                }
                else break;
        }
        return length;
}

char *CmBinToText(IN BYTE *src, IN INT Length)
{
	int len;
	char *mem = calloc(Length * 3 + 2, 1);
	char *out = mem;

	if (out == NULL)
		return out;
	while(--Length >= 0) {
		len = sprintf(out, "%02X ", *src++);
		out += len;
        }
	*out++ = '\n';
	*out = '\0';
        return mem;
}

//
// convert parameters from DATAFIELD to UNIT table entry for each record
//
#if 0
// not used
static UNIT *DfUnitTable(DATAFIELD *pd, INT Count)
{
        int i;
        UNIT *calcUnit = calloc(Count, sizeof(UNIT));
        UNIT *pu = &calcUnit[0];

        if (calcUnit == NULL) {
                fprintf(stderr, "calloc calcunit error\n");
                return NULL;
        }

        for(i = 0; i < Count; i++) {
                if (pd->ShortCut == NULL) {
                        return calcUnit;
                }
                //printf("*%s: %d=%s,%s pu:%p\n", __FUNCTION__, i, pd->ShortCut, pd->DataName, pu);

                pu->ValueType = pd->ValueType;
		pu->SCut = pd->ShortCut;
                pu->DName = pd->DataName;
                pu->FromBit = pd->BitOffs;
                pu->SizeBit = pd->BitSize;
                pu->Unit = pd->Unit;
                pu->Slope = CalcA(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);
                pu->Offset = CalcB(pd->RangeMin, pd->ScaleMin, pd->RangeMax, pd->ScaleMax);
                pu++, pd++;
        }
        return calcUnit;
}
#endif

//
// Add or return CM_TABLE record index for ModelCache[].
//
CM_TABLE *CmGetCache(char *Cms)
{
	extern CM_TABLE ModelCache[];
        CM_TABLE *pmc;

	pmc = &ModelCache[0];
	while(pmc->CmHandle) {
		if (!strcmp(Cms, pmc->CmStr)) {
			return pmc;
		}
		pmc++;
	}
	return NULL;
}

INT CmCleanUp(void)
{
        extern int CacheIndex;
	extern CM_TABLE ModelCache[];
        CM_TABLE *pmc;
	DATAFIELD *pd;
	int i, j;

	pmc = &ModelCache[0];
	for (i = 0; i < CacheIndex; i++){
		if (pmc->CmHandle == NULL) {
			break;
		}
		IF_EXISTS_FREE(pmc->CmStr);
		IF_EXISTS_FREE(pmc->Title);
		if (pmc->Dtable != NULL) {
			//pd = &pmc->Dtable[0];
			//for(j = 0; j < pmc->Count; j++) {
				//printf("***** %d: DataName=%s\n", j, pd->DataName);
				//printf("***** %d: ShortCut=%s\n", j, pd->ShortCut);
				//pd++;
			//}		
			pd = &pmc->Dtable[0];
			for(j =0; j < pmc->Count; j++) {
				IF_EXISTS_FREE(pd->DataName);
				IF_EXISTS_FREE(pd->ShortCut);
				pd++;
			}		
			IF_EXISTS_FREE(pmc->Dtable);
			pmc->Count = 0;
		}
		CmDelete(pmc->CmHandle);
		pmc->CmHandle = NULL;
		pmc++;
	}
	CacheIndex = 0;
	return i;
}

//
// Add or return CM_TABLE record index for ModelCache[].
//
CM_TABLE *CmGetModel(BYTE *Buf, INT Size)
{
        extern int CacheIndex;
        extern CM_TABLE ModelCache[];
        INT index;
        CM_TABLE *pmc;
        CM_TABLE *ptmp;
        INT fpSize;
        VOID *modelHandle;
	char *baseName;
	BOOL matched;
	char tempName[CM_STRSIZE];
        int i;

#if MODEL_DEBUG
	printf("##MD: Enter CmGetModel Size=%d CI=%d\n", Size, CacheIndex);
#endif
        index = CmFpSearch(&ModelCache[0], Buf, Size);
        if (index < 0) {
                // new fingerprint for new model
                //printf("** New fingerprint for new model\n");
                modelHandle = CmAnalyze(Buf, Size, &fpSize);
                if (modelHandle == NULL) {
                        fprintf(stderr, "##Analyze failed\n");
                        return NULL;
                }

                if (CacheIndex == CM_CACHE_SIZE) {
                        fprintf(stderr, "##CACHE size overflow=%d\n", CacheIndex);
                        return NULL;
                }
                pmc = &ModelCache[CacheIndex++];
                pmc->CmHandle = modelHandle;
		baseName = CmMakeString(modelHandle);

		for(i = 1; i < 999; i++) {
			sprintf(tempName, "%s-%d", baseName, i);
			ptmp = &ModelCache[0];
			matched = FALSE;
#if MODEL_DEBUG
			printf("##MD: %d:old CmStr=%s\n", i, pmc->CmStr);
#endif
			while(ptmp != NULL && ptmp->CmStr != NULL) {
				if (!strcmp(tempName, ptmp->CmStr)) {
					// Is mached existing name?
					matched = TRUE;
					break;
				}
				ptmp++;
			}
			if (!matched)
				break;
		}
#if MODEL_DEBUG
		printf("##MD: new CmStr=%s\n", pmc->CmStr);
#endif
                pmc->CmStr = strdup(tempName);
		pmc->Title = CmMakeTitle(modelHandle);
                pmc->Count = fpSize;
		pmc->Dtable = CmMakeDataField(modelHandle);
		////////
#if MODEL_DEBUG
		for(i = 0; i < fpSize; i++) {
			printf("**CmGetModel: %d:%d %s=%s\n", i, (pmc->Dtable+i)->ValueType,
			       (pmc->Dtable+i)->ShortCut, (pmc->Dtable+i)->DataName);
		}
#endif
		////////
        }
        else {
                pmc = &ModelCache[index];
                //printf("** Matched fingerprint for index %d\n", index);
        }
        //printf("** Fingerprint [%s] '%s'\n", pmc->CmStr, pmc->Title);

        return pmc;
}

//
//
//
#ifdef BITTEST
int main(int ac, char *av[])
{
	const int bufferSize = BUFSIZ / 4;
	FILE *f;
	char *s;
	char buf[bufferSize];
	BYTE binArray[bufferSize];
	CM_TABLE *pmc;
	int length;

	if (ac > 1) {
		f = fopen(av[1], "r");
	}
	else {
		f = stdin;
	}

	do {
		memset(buf, 0, bufferSize);
		memset(binArray, 0, bufferSize);
		s = fgets(buf, BUFSIZ, f);
		if (s == NULL || *s == '\0')
			break;
		
		if (buf[strlen(buf) - 1] == '\n') {
			buf[strlen(buf) - 1] = '\0';
		}
		
		length = CmTextToBin(buf, binArray);

		pmc = CmGetModel((BYTE *)binArray, length);
		if (pmc != NULL) {
			printf("[%s] '%s' count=%d len=%d\n\n\n",
			       pmc->CmStr, pmc->Title, pmc->Count, length);
		}
		else {
			printf("CmGetModel error!\n");
		}
	}
	while(s != NULL);
	
	fclose(f);

	return 0;
}
#endif
