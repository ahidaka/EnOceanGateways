#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <libxml/xmlreader.h>

#include "dpride.h"
#include "ptable.h"

typedef enum {
	TAG_NONE=-1,
	TAG_EEP=0,
	TAG_PROFILE,
	TAG_RORG,
	TAG_FUNC,
	TAG_TEACHIN,
	TAG_NUMBER,
	TAG_TITLE,
	TAG_TELEGRAM,
	TAG_TYPE,
	TAG_STATUS,
	TAG_CASE,
	TAG_DATAFIELD,
	TAG_RANGE,
	TAG_SCALE,
	TAG_BITOFFS,
	TAG_BITSIZE,
	TAG_UNIT,
	TAG_MIN,
	TAG_MAX,
	TAG_DATA,
	TAG_RESERVED,
	TAG_ENUM,
	TAG_ITEM,
	TAG_VALUE,
	TAG_DESC,
	TAG_SCUT,
	TAG_INFO,
} TagState;

typedef struct _tagname {
	char *name;
	char *value;
} TAGNAME;

#define FIELD_SIZE 128
#define EEP_SIZE 512

static TAGNAME TagTable[] = {
	{"eep", 0}, //0
	{"profile", 0}, //1
	{"rorg", 0}, //2
	{"func", 0}, //3
	{"teachin", 0}, //4

	{"number", 0}, //5
	{"title", 0}, //6
	{"telegram", 0}, //7
	{"type", 0}, //8
	{"status", 0},//9
	{"case", 0},//10
	{"datafield", 0},//11
	{"range", 0}, //12
	{"scale", 0}, //13
	{"bitoffs", 0}, //14
	{"bitsize", 0}, //15
	{"unit", 0}, //16
	{"min", 0}, //17
	{"max", 0}, //18
	{"data", 0}, //19
	{"reserved", 0}, //20
	{"enum", 0},
	{"item", 0},
	{"value", 0},
	{"description", 0},
	{"shortcut", 0},
	{"info", 0},
	{NULL, 0}
};

static EEP_TABLE *EepTable; //[EEP_SIZE];
static DATAFIELD *dataTable; //[FIELD_SIZE];
static int dataTableIndex = 0;
static int enumOverflow = 0;

//
static int stateEep = 0;
static int stateProfile = 0;
static int stateRorg = 0;
static int stateFunc = 0;
static int stateType = 0;
static int stateDatafield = 0;
static int stateEnum = 0;
static int stateItem = 0;

static char *RorgNumber;
static char *RorgTelegram;

static char *FuncNumber;
static char *FuncTitle;

static char *TypeNumber;
//static char *typeTitle;
//static char *typeStatus;

static char *RangeMin;
static char *RangeMax;
static char *ScaleMin;
static char *ScaleMax;

static ENUMTABLE EnumTable[ENUM_SIZE];
static int EnumTableIndex;

int debug = 0;

#define _DD if (debug > 1)
#define _D if (debug > 0)

//
inline void StringCopy(char **dst, char *src)
{
	if (*dst != NULL) {
		free((void *) *dst);
		*dst = NULL;
	}
	if (src != NULL && strlen(src) > 0) {
		*dst = strdup(src);
		if (*dst == NULL) {
			Warn("strdup() error");
		}
	}
}

inline void IntegerCopy(int *dst, char *src)
{
	if (dst != NULL && src != NULL)
		*dst = (int) strtol(src, NULL, 10);
	else *dst = 0;
}

int HexTrim(char *dst, char *src)
{
        int i;
        int len = strlen(src);

	if (len > 4)
		len = 4;
        if (len > 2 &&
            (src[0] == '0' && toupper(src[1]) == 'X'))
        {
                src += 2;
                len -= 2;
        }
        for(i = 0; i < len; i++) {
                if (isxdigit(*src))
                        *dst++ = *src;
                src++;
        }
        *dst = '\0';
        return i;
}


void SaveEep(EEP_TABLE *Table, int FieldCount, char *EepString, char *Title, DATAFIELD *Pd)
{
	int tableSize;
	DATAFIELD *table = NULL;
	DATAFIELD *pt;
	int i;

	_D printf("SaveEep: %d <%s><%s><%s>\n",
		  FieldCount, EepString, Title, Pd ? Pd->DataName : "");

	Table->Size = FieldCount;
	strcpy(Table->Eep, EepString);
	if (Title != NULL && *Title != '\0') {
		Table->Title = strdup(Title);
		if (Table->Title == NULL) {
			Warn("Title: strdup() error");
		}
	}
	else Table->Title = "";

	if (FieldCount > 0) {
		tableSize = sizeof(DATAFIELD) * FieldCount;
		pt = table = malloc(tableSize);
		if (table == NULL) {
			fprintf(stderr, "Cannot alloc table=%d", tableSize);
			exit(1);
		}
		memset(table, 0, tableSize);
	}
	_DD printf("%s: sz=%lu c=%d t=%d\n",
		  EepString, sizeof(DATAFIELD), FieldCount, tableSize);
	for(i = 0; i < FieldCount; i++) {
		if (Pd->DataName) {
			if (Pd->DataName) {
				pt->DataName = strdup(Pd->DataName);
				if (pt->DataName == NULL)
					Warn("DataName: strdup() error");
			}
			if (Pd->ShortCut) {
				pt->ShortCut = strdup(Pd->ShortCut);
				if (pt->ShortCut == NULL)
					Warn("ShortCut: strdup() error");
			}
			pt->BitOffs = Pd->BitOffs;
			pt->BitSize = Pd->BitSize;
			pt->RangeMin = Pd->RangeMin;
			pt->RangeMax = Pd->RangeMax;
			pt->ScaleMin = Pd->ScaleMin;
			pt->ScaleMax = Pd->ScaleMax;
			if (Pd->Unit) {
				pt->Unit = strdup(Pd->Unit);
				if (pt->Unit == NULL)
					Warn("Unit: strdup() error");
			}
		}
		pt++, Pd++;
	}
	Table->Dtable = table;
}

//
int ProcessNode(xmlTextReaderPtr Reader, EEP_TABLE *Table)
{
    static TagState state = TAG_NONE;
    xmlElementType nodeType;
    xmlChar *name, *value;
    int i, j;
    TAGNAME *pt;
    DATAFIELD *pd;
    int fieldCount = 0;
    char rorg[4];
    char func[4];
    char type[4];
    char eepString[10];

    nodeType = xmlTextReaderNodeType(Reader);
    name = xmlTextReaderName(Reader);
    if (!name) {
        name = xmlStrdup(BAD_CAST "");
    }

    if (nodeType == (xmlElementType)XML_READER_TYPE_ELEMENT) {

	    pt = &TagTable[0];
	    for(i = 0; pt->name != NULL; i++, pt++) {
		    if ( xmlStrcmp(name, BAD_CAST pt->name) == 0 ) {
			    state = i;
			    break;
		    }
	    }

	    switch(state) {
	    case TAG_EEP:
		    stateEep = 1;
		    break;
	    case TAG_PROFILE:
		    stateProfile = 1;
		    break;
	    case TAG_RORG:
		    stateRorg = 1;
		    break;
	    case TAG_FUNC:
		    _DD printf("---> <<func>>\n");
		    stateFunc = 1;
		    break;
	    case TAG_TYPE:
		    if (stateFunc) {
			    _DD printf("---> <<type>>\n");
			    stateType = 1;
			    dataTableIndex = 0;
			    EnumTableIndex = 0;
			    enumOverflow = 0;

			    pd = &dataTable[0];
			    for(i = 0; i < FIELD_SIZE; i++, pd++) {
				    pd->Reserved = 0;
				    if (pd->DataName) {
					    free(pd->DataName);
					    pd->DataName = NULL;
				    }
				    if (pd->ShortCut) {
					    free(pd->ShortCut);
					    pd->ShortCut = NULL;
				    }
				    pd->BitOffs = 0;
				    pd->BitSize = 0;
				    pd->RangeMin = 0;
				    pd->RangeMax = 0;
				    pd->ScaleMin = 0;
				    pd->ScaleMax = 0;
				    pd->Unit = NULL;
				    for(j = 0; j < ENUM_SIZE; j++) {
					    if (pd->EnumDesc[j].Desc) {
						    free((void *) pd->EnumDesc[j].Desc);
						    pd->EnumDesc[j].Index = 0;
						    pd->EnumDesc[j].Desc = NULL;
					    }
				    }
			    }
		    }
		    break;
	    case TAG_DATAFIELD:
		    if (stateFunc) {
			    _DD printf("---> <<datafield>>\n");
			    stateDatafield = 1;
			    pt = &TagTable[0];
			    for(i = 0; pt->name != NULL ; i++, pt++) {
				    if (pt->value) 
					    free(pt->value);
				    pt->value = NULL;
			    }
		    }
		    break;

	    case TAG_ENUM:
		    if (stateDatafield) {
			    _DD printf("---> <<enum>>\n");
			    stateEnum = 1;
		    }
		    break;

	    case TAG_ITEM:
		    if (stateEnum) {
			    _DD printf("---> <<item>>\n");
			    stateItem = 1;
		    }
		    break;

	    default:
		    if (stateFunc) {
			    _DD printf("---> <%s>\n", TagTable[state].name);
			    if (state == TAG_RESERVED) {
				    dataTable[dataTableIndex].Reserved = 1;
			    }
		    }
		    break;
	    }

    }
    else if (nodeType == (xmlElementType)XML_READER_TYPE_END_ELEMENT) {

	    pt = &TagTable[0];
	    for(i = 0; pt->name != NULL; i++, pt++) {
		    if ( xmlStrcmp(name, BAD_CAST pt->name) == 0 ) {
			    state = i;
			    break;
		    }
	    }

	    if (stateFunc && pt->name != NULL) {
		    _DD printf("-----<end element:%s>-%s-%s-%s-%s--\n",
			   TagTable[state].name,
			   stateRorg ? "R" : "",
			   stateFunc ? "F" : "",
			   stateType ? "T" : "",
			   stateDatafield ? "D" : "");
	    }

	    switch(state) {
	    case TAG_EEP:
		    stateEep = 0;
		    break;
	    case TAG_PROFILE:
		    stateProfile = 0;
		    break;
	    case TAG_RORG:
		    stateRorg = 0;
		    break;
	    case TAG_FUNC:
		    _DD printf("<--- <<func>>\n");
		    stateFunc = 0;
		    break;
	    case TAG_TYPE:
		    if (stateFunc)
			    _DD printf("<--- <<type>>\n");
		    stateType = 0;
		    // print summary of this type //
		    HexTrim(rorg, RorgNumber);
		    HexTrim(func, FuncNumber);
		    HexTrim(type, TypeNumber);
		    sprintf(eepString, "%s-%s-%s", rorg, func, type);
		    _D printf("*EEP:%s (%d) <%s>\n", eepString, dataTableIndex, FuncTitle);
		    pd = &dataTable[0];
		    for(i = 0; i < dataTableIndex; i++, pd++){
			    if (pd->Reserved) {
				    //this field is reserved
				    continue;
			    }
			    _D printf("%d[%s]:%s ofs=%d siz=%d rmin=%d rmax=%d smin=%d smax=%d u=%s\n",
				   i,
				   pd->DataName,
				   pd->ShortCut,
				   pd->BitOffs,
				   pd->BitSize,
				   pd->RangeMin,
				   pd->RangeMax,
				   pd->ScaleMin,
				   pd->ScaleMax,
				   pd->Unit);

			    if (pd->EnumDesc[0].Desc != NULL) {
				    _D printf("EnumDesc:");
				    for(j = 0; j < ENUM_SIZE; j++) {
					    if (pd->EnumDesc[j].Desc == NULL)
						    break;
					    _D printf("[%d]%d:%s,", j,
						   pd->EnumDesc[j].Index, pd->EnumDesc[j].Desc);
				    }
				    _D printf(".\n");
			    }
		    }
                    _DD printf("<<end index=%d>>\n", i);
		    fieldCount = i;

		    // Output rorg='A5' only
		    if (fieldCount > 0 && 
			    (!strncmp(eepString, "F6", 2) // RPS
			     || !strncmp(eepString, "D5", 2) // 1BS
			     || !strncmp(eepString, "A5", 2) // 4BS
			     || !strncmp(eepString, "D2", 2) // VLD
				    )) {
				SaveEep(Table, fieldCount, eepString, FuncTitle, &dataTable[0]);
		    }
		    else {
			    fieldCount = 0;
		    }
		    break;

	    case TAG_DATAFIELD:
		    if (stateFunc && stateType) {
			    _DD printf("<--- <<datafield>>\n");

			    // summary of each datafiled
			    pd = &dataTable[dataTableIndex];
			    StringCopy(&pd->DataName, TagTable[TAG_DATA].value);
			    StringCopy(&pd->ShortCut, TagTable[TAG_SCUT].value);
			    IntegerCopy(&pd->BitOffs, TagTable[TAG_BITOFFS].value);
			    IntegerCopy(&pd->BitSize, TagTable[TAG_BITSIZE].value);
			    IntegerCopy(&pd->RangeMin, RangeMin);
			    IntegerCopy(&pd->RangeMax, RangeMax);
			    IntegerCopy(&pd->ScaleMin, ScaleMin);
			    IntegerCopy(&pd->ScaleMax, ScaleMax);
			    StringCopy(&pd->Unit, TagTable[TAG_UNIT].value);

			    if (EnumTableIndex > 0 && EnumTable[0].Desc != NULL) {
				    for(j = 0; j < EnumTableIndex; j++) {
					    StringCopy(&pd->EnumDesc[j].Desc, EnumTable[j].Desc);
					    pd->EnumDesc[j].Index = EnumTable[j].Index;
				    }
				    EnumTableIndex = 0;
			    }
			    stateDatafield = 0;
			    dataTableIndex++;
			    if (dataTableIndex >= FIELD_SIZE) {
				    fprintf(stderr, "data corrupted: num of field=%d\n",
					   dataTableIndex);
				    exit(1);
			    }
		    }
		    break;

	    case TAG_RANGE:
		    StringCopy(&RangeMin, TagTable[TAG_MIN].value);
		    StringCopy(&RangeMax, TagTable[TAG_MAX].value);
		    _DD printf("<<range: min=%s max=%s>>\n", RangeMin, RangeMax);
		    break;

	    case TAG_SCALE:
		    StringCopy(&ScaleMin, TagTable[TAG_MIN].value);
		    StringCopy(&ScaleMax, TagTable[TAG_MAX].value);
		    _DD printf("<<scale: min=%s max=%s>>\n", ScaleMin, ScaleMax);
		    break;

	    case TAG_ENUM:
		    if (stateDatafield && stateType) {
			    _DD printf("<--- <<enum>>\n");
			    stateEnum = 0;
		    }
		    break;

	    case TAG_ITEM:
		    if (stateEnum && !enumOverflow) {
			    if (TagTable[TAG_VALUE].value != NULL) {
				    // index found //
				    IntegerCopy(&i, TagTable[TAG_VALUE].value);
			    }
			    EnumTable[EnumTableIndex].Index = i;
			    StringCopy(&EnumTable[EnumTableIndex].Desc, TagTable[TAG_DESC].value);
			    EnumTableIndex++;
			    if (EnumTableIndex >= ENUM_SIZE) {
				    _D printf("** ignore too many num of enum=%d\n",
					   EnumTableIndex);
				    enumOverflow++;
			    }
			    _DD printf("<--- <<item>>\n");
			    stateItem = 0;
		    }
		    break;

	    default:
		    break;
	    }

    }
    else if (nodeType == (xmlElementType)XML_READER_TYPE_TEXT) {
        value = xmlTextReaderValue(Reader);
        
        if (!value) {
            value = xmlStrdup(BAD_CAST "");
	}

	if (stateEep && stateProfile) {
		if (stateRorg) {
			if (stateFunc) {
				if (stateType) {
					_DD printf("<%d:%s> %s\n", state, TagTable[state].name, value);
					StringCopy(&TagTable[state].value, (char*)value);
					if (state == TAG_NUMBER) {
						StringCopy(&TypeNumber, (char*)value);
					}
				}
				else {
					if (state == TAG_NUMBER) {
						StringCopy(&FuncNumber, (char*)value);
						_DD printf("<func-number:%s>\n", FuncNumber);
					}
					else if (state == TAG_TITLE) {
						StringCopy(&FuncTitle, (char*)value);
						_DD printf("<func-title:%s>\n", FuncTitle);
					}
				}
			}
			else {
				if(state == TAG_NUMBER) {
					StringCopy(&RorgNumber, (char*)value);
				}
				else if(state == TAG_TELEGRAM) {
					StringCopy(&RorgTelegram, (char*)value);
				}
			}
		}
	}
        xmlFree(value);
    }

    xmlFree(name);
    return fieldCount;
}

void PrintNode(DATAFIELD *pd, int Size)
{
	int i;

	for(i = 0; i < Size; i++) {
		if (pd->DataName)
			printf(" %s:%s %d %d %d %d %d %d  %s\n",
			       pd->DataName, pd->ShortCut,
			       pd->BitOffs, pd->BitSize,
			       pd->RangeMin, pd->RangeMax,
			       pd->ScaleMin, pd->ScaleMax,
			       pd->Unit);
		pd++;
	}
}

void PrintEepAll()
{
	EEP_TABLE *pe = EepTable;
	int fcnt = 0;

	while(pe->Eep[0] != '\0' ) {
		if (pe->Size > 0) {
			printf("%d %s: %d\n", fcnt, pe->Eep, pe->Size);
			PrintNode(pe->Dtable, pe->Size);
		}
		pe++, fcnt++;
	}
}

void PrintEep(char *EEP)
{
	EEP_TABLE *pe = EepTable;
	while(pe->Eep[0] != '\0' ) {
		if (!strcmp(EEP, pe->Eep)) {
			printf("%s: %d\n", pe->Eep, pe->Size);
			if (pe->Size > 0) {
				PrintNode(pe->Dtable, pe->Size);
			}
		}
		pe++;
	}
}

EEP_TABLE *GetEep(char *EEP)
{
	EEP_TABLE *pe = EepTable;
	while(pe->Eep[0] != '\0' ) {
		if (!strcmp(EEP, pe->Eep)) {
			return(pe);
		}
		pe++;
	}
	return NULL;
}

int InitEep(char *Profile)
{
	xmlTextReaderPtr reader;
	int ret, count;
	int numField;
	static DATAFIELD D2_03_20_ES = {
		0,
		"Energy Supply",
		"ES",
		0, //Bitoffs
		1, //Bitsize
		0, //RangeMin
		1, //RangeMax
		0, //ScaleMin
		1, //ScaleMax
		"", //Unit
		{{0, NULL}}, //Enum
	}; 

	EepTable = malloc(sizeof(EEP_TABLE) * EEP_SIZE);
	if (!EepTable) {
		fprintf(stderr, "cannot malloc EepTable\n");
		return 0;
	}
	dataTable = malloc(sizeof(DATAFIELD) * FIELD_SIZE);
	if (!dataTable) {
		fprintf(stderr, "cannot malloc dataTable\n");
		return 0;
	}

	reader = xmlNewTextReaderFilename(Profile);
	if ( !reader ) {
		fprintf(stderr, "Failed to open XML file=%s.\n", Profile);
		return 0;
	}

	_D printf("<<start>>\n"); 
	
	stateEep = 0;
	stateProfile = 0;
	stateRorg = 0;
	stateFunc = 0;

	count = 0;
	while((ret = xmlTextReaderRead(reader)) > 0) {
		numField = ProcessNode(reader, &EepTable[count]);
		if (numField > 0) {
			_DD printf("<<count:%d %d>>\n", count, numField);
			count++;
		}
	}
    
	_D printf("<<end count=%d>>\n", count);
	SaveEep(&EepTable[count++], 1, "D2-03-20",
		"Beacon with Vibration Detection",
		&D2_03_20_ES); //Add Custom "D2-03-20",

	SaveEep(&EepTable[count], 0, "\0", "\0", NULL); //Add end if table mark

	xmlFreeTextReader(reader);

	if (ret == -1) {
		fprintf(stderr, "Parse error.\n");
		return 0;
	}
	return 1; // success
}
