//
// ptable.h -- Profile Table
//
#pragma once

#define ENUM_SIZE 32
#define EEP_STRSIZE 10

typedef struct _enumtable
{
        int Index;
        char *Desc;
} ENUMTABLE;

typedef struct _datafield
{
        int Reserved; // means not used
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
// export functions
//
int InitEep(char *Profile);
EEP_TABLE *GetEep(char *EEP);
void PrintEepAll();
void PrintEep(char *EEP);
void SaveEep(EEP_TABLE *Table, int FieldCount, char *EepString, char *Title, DATAFIELD *Pd);

