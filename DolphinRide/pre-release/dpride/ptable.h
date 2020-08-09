//
// ptable.h -- Profile Table
//
#pragma once

#define ENUM_SIZE (16)
#define EEP_STRSIZE (10)

#define CM_STRSIZE (16)
#define CM_STRBASE (12)
#define CM_STRSUFFIX (3)
#define CM_CACHE_SIZE (256)

#ifndef SC_SIZE
#define SC_SIZE (16)
#endif

#include "typedefs.h"

typedef struct _enumtable
{
        int Index;
        char *Desc;
} ENUMTABLE;

typedef enum _value_type
{
        VT_NotUsed = 0,
        VT_Data = 1,
        VT_Flag = 2,
        VT_Enum = 3
} VALUE_TYPE;

typedef struct _datafield
{
        VALUE_TYPE ValueType; // 0: Not used, 1: Data, 2: Binary Flag, 3: Enumerated data
        char *DataName;
        char *ShortCut;
        int BitOffs;
        int BitSize;
        int RangeMin;
        int RangeMax;
        float ScaleMin;
        float ScaleMax;
        char *Unit;
        ENUMTABLE EnumDesc[ENUM_SIZE];
} DATAFIELD;

typedef struct _eep_table
{
        int Size;
        char Eep[EEP_STRSIZE];
	char *Title;
        DATAFIELD *Dtable;
} EEP_TABLE;


//
// EEP Profile cache
//
typedef struct _unit {
        VALUE_TYPE ValueType;
        char *SCut;
        char *Unit;
        char *DName;
        INT FromBit;
        INT SizeBit;
        double Slope;
        double Offset;
        // char *EnumArray[]; // not implemented now
} UNIT;

typedef struct _profile_cache {
        char StrKey[16];
        UNIT Unit[SC_SIZE];
} PROFILE_CACHE;

//
//
//
typedef struct _cm_table
{
        VOID *CmHandle;       // (CM_HANDLE *)
        char *CmStr;          // ex.[tp8hu12tp8ac12]
        char *Title;          // ex."Temperature, tp, ac, +3"
	INT  Count;
        DATAFIELD *Dtable;
} CM_TABLE;


//
// export functions
//
int InitEep(char *Profile);
EEP_TABLE *GetEep(char *EEP);
void PrintEepAll();
void PrintEep(char *EEP);
int SaveEep(EEP_TABLE *Table, int FieldCount, char *EepString, char *Title, DATAFIELD *Pd);
