//
// models.h -- Models Table
//

#pragma once

void CmPrintTI(BYTE *Buf, BYTE *Eepbuf, BYTE *Data, INT len);
void CmPrintTR(BYTE *Buf, BYTE *Data, INT len);
void CmPrintCD(BYTE *Buf, BYTE *Data, INT len);
void CmPrintSD(BYTE *Buf, BYTE *Data, INT len);

INT CmTextToBin(IN char *src, IN BYTE *dst);
char *CmBinToText(IN BYTE *src, IN INT Length);
INT CmFpSearch(IN CM_TABLE *pmc, IN BYTE *Buf, IN INT Size);
VOID *CmAnalyze(IN BYTE *Start, IN INT Length, OUT INT *Count);
char *CmMakeString(VOID *Handle);
char *CmMakeTitle(VOID *Handle);
DATAFIELD *CmMakeDataField(VOID *Handle);
CM_TABLE *CmGetCache(char *Cms);
CM_TABLE *CmGetModel(BYTE *Buf, INT Size);
VOID CmDelete(VOID *Handle);
INT CmCleanUp(VOID);
