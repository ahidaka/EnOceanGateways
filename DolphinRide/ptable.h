//
// ptable.h -- Profile Table
//

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
        int ScaleMin;
        int ScaleMax;
        char *Unit;
        ENUMTABLE EnumDesc[ENUM_SIZE];
} DATAFIELD;

typedef struct _eep_table
{
        int Size;
        char Eep[EEP_STRSIZE];
        DATAFIELD *Dtable;
} EEP_TABLE;

//
// export functions
//
int InitEEP(char *Profile);
EEP_TABLE *GetEEP(char *EEP);
void PrintEEPAll();
void PrintEEP(char *EEP);
