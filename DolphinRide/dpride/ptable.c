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

#define FIELD_SIZE (256)
#define EEP_SIZE (384)

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
const char *_value_type_string[4] = {
        "Not used", "Data", "Flag", "Enum"
};

#define _DD if (debug > 1)
#define _D if (debug > 0)

//
//inline void StringCopy(char **dst, char *src)
void StringCopy(char **dst, char *src)  
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

//inline void IntegerCopy(int *dst, char *src)
void IntegerCopy(int *dst, char *src)  
{
	if (dst != NULL && src != NULL)
		*dst = (int) strtol(src, NULL, 10);
	else *dst = 0;
}

void FloatCopy(float *dst, char *src)
{
	if (dst != NULL && src != NULL)
		*dst = atof(src);
	else *dst = 0.0F;
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


int SaveEep(EEP_TABLE *Table, int FieldCount, char *EepString, char *Title, DATAFIELD *Pd)
{
	int tableSize;
	DATAFIELD *table = NULL;
	DATAFIELD *pt;
	char *pp;
	int i, j;

	_D printf("SaveEep: %d <%s><%s><%s>\n",
		  FieldCount, EepString, Title, Pd ? Pd->DataName : "");

	if (EepString == NULL || EepString[0] == '\0' || strlen(EepString) < 8) {
		//Warn("NULL or short EEPString");
		return 0;
	}
	if (!strcmp("D0-00-00", EepString)) {
		// something troubled at SmartACK MailBox
		return 0;
	}
	
	if (GetEep(EepString) != NULL) {
		//Warn("alredy exists at saved EEP table");
		return 0;
	}

	Table->Size = FieldCount;
	strcpy(Table->Eep, EepString);
	if (Title != NULL && *Title != '\0') {
		Table->Title = strdup(Title);
		if (Table->Title == NULL) {
			Warn("Title: strdup() error");
		}
		while((pp = strchr(Table->Title, ',')) != NULL) {
			*pp = '-';
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
	_DD printf("%s: sz=%u c=%d t=%d\n",
		   EepString, (uint) sizeof(DATAFIELD), FieldCount, tableSize);
	for(i = 0; i < FieldCount; i++) {
		if (Pd->DataName) {
			pt->ValueType = Pd->ValueType;
			pt->DataName = strdup(Pd->DataName);
			if (pt->DataName == NULL) {
				Warn("DataName: strdup() error");
			}
			if (Pd->ShortCut) {
				pt->ShortCut = strdup(Pd->ShortCut);
				if (pt->ShortCut == NULL)
					Warn("ShortCut: strdup() error");
			}
			//pt->ValueType = Pd->ValueType ? Pd->ValueType : VT_Data;
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
			if (Pd->EnumDesc[0].Desc != NULL) {
				for(j = 0; j < ENUM_SIZE; j++) {
					if (Pd->EnumDesc[j].Desc == NULL) {
						break;
					}
					pt->EnumDesc[j].Index = Pd->EnumDesc[j].Index;
					pt->EnumDesc[j].Desc = strdup(Pd->EnumDesc[j].Desc);
					if (pt->EnumDesc[j].Desc == NULL)
						Warn("Unit: strdup() error");
				}
			}
		}
		pt++, Pd++;
	}
	Table->Dtable = table;

	return(FieldCount);
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
				    pd->ValueType = VT_NotUsed;
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
				    dataTable[dataTableIndex].ValueType = VT_Data;
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
			    if (pd->ValueType) {
				    //this field is reserved
				    continue;
			    }
			    _D printf("%d[%s]:%s %d ofs=%d siz=%d rmin=%d rmax=%d smin=%.3f smax=%.3f u=%s\n",
				      i,
				      pd->DataName,
				      pd->ShortCut,
				      pd->ValueType,
				      pd->BitOffs,
				      pd->BitSize,
				      pd->RangeMin,
				      pd->RangeMax,
				      pd->ScaleMin,
				      pd->ScaleMax,
				      pd->Unit);

			    if (pd->EnumDesc[0].Desc != NULL) {
				    pd->ValueType = VT_Enum;
				    _D printf("EnumDesc[%s]:", eepString);
				    for(j = 0; j < ENUM_SIZE; j++) {
					    if (pd->EnumDesc[j].Desc == NULL)
						    break;
					    _D printf("[%d]%d:%s,", j,
						   pd->EnumDesc[j].Index, pd->EnumDesc[j].Desc);
				    }
				    _D printf(".\n");
			    }
			    else {
				     pd->ValueType = VT_Data;
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
			    (void) SaveEep(Table, fieldCount, eepString, FuncTitle, &dataTable[0]);
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
			    FloatCopy(&pd->ScaleMin, ScaleMin);
			    FloatCopy(&pd->ScaleMax, ScaleMax);
			    StringCopy(&pd->Unit, TagTable[TAG_UNIT].value);

			    if (EnumTableIndex > 0 && EnumTable[0].Desc != NULL) {
				    for(j = 0; j < EnumTableIndex; j++) {
					    StringCopy(&pd->EnumDesc[j].Desc, EnumTable[j].Desc);
					    pd->EnumDesc[j].Index = EnumTable[j].Index;
					    _D printf("EnumCopied(%d): %s,%s\n", j, pd->EnumDesc[j].Desc, EnumTable[j].Desc);
				    }

				    EnumTableIndex = 0;
			    }
			    else {
				    _D printf("No Enum data: %d\n", pd->ValueType);
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
	int i, j;

	for(i = 0; i < Size; i++) {
		if (pd->DataName)
			printf(" %s:%s(%s) %d %d %d %d %.3f %.3f  %s\n",
			       _value_type_string[pd->ValueType],
			       pd->DataName, pd->ShortCut,
			       pd->BitOffs, pd->BitSize,
			       pd->RangeMin, pd->RangeMax,
			       pd->ScaleMin, pd->ScaleMax,
			       pd->Unit);
		
		//penum = &pd->EnumDesc[0];
		//if (penum->Desc != NULL) {
		if (pd->EnumDesc[0].Desc != NULL) {
			for(j = 0; j < EnumTableIndex; j++) {
				if (pd->EnumDesc[j].Desc == NULL)
					break;
				printf("  ENUM %d: %d<%s>\n", j,
				       pd->EnumDesc[j].Index,
				       pd->EnumDesc[j].Desc);
			}
		}
		pd++;
	}
}

void PrintEepAll()
{
	EEP_TABLE *pe = EepTable;
	int fcnt = 0;

	while(pe->Eep[0] != '\0' ) {
		if (pe->Size > 0) {
			printf("**%s %d %s: %d <%s>\n", __FUNCTION__, fcnt, pe->Eep, pe->Size, pe->Title);
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
			printf("%s: %d <%s>\n", pe->Eep, pe->Size, pe->Title);
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
	xmlTextReaderPtr reader = NULL;
	int ret, count;
	int numField;

	static DATAFIELD A5_02_05[2] =
	{
		{
			1,
			"Temperature",
			"TMP",
			16, //Bitoffs
			8, //Bitsize
			255, //RangeMin
			0, //RangeMax
			0, //ScaleMin
			40, //ScaleMax
			"℃", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};

	static DATAFIELD A5_04_01[4] =
	{
		{
			1,
			"Humidity",
			"HUM",
			8, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			250, //RangeMax
			0, //ScaleMin
			100, //ScaleMax
			"%", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Temperature",
			"TMP",
			16, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			250, //RangeMax
			0, //ScaleMin
			40, //ScaleMax
			"℃", //Unit
			{{0, NULL}}, //Enum			
	    },
		{
			3,
			"T-Sensor",
			"TSN",
			30, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //
			{{0, "not available"},{1, "available"}}, //Enum
	    },
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};
	
	static DATAFIELD A5_04_03[4] =
	{
		{
			1,
			"Humidity",
			"HUM",
			0, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			255, //RangeMax
			0, //ScaleMin
			100, //ScaleMax
			"%", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Temperature",
			"TMP",
			14, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1023, //RangeMax
			-20, //ScaleMin
			60, //ScaleMax
			"℃", //Unit
			{{0, NULL}}, //Enum
	    },
		{
			3,
			"Telegram Type",
			"TTP",
			31, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //
			{{0, "Heartbeat"},{1, "Event triggered"}}, //Enum
	    },
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};
	
	static DATAFIELD A5_06_02[5] =
	{
		{
			1,
			"Supply voltage",
			"SVC",
			0, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			255, //RangeMax
			0, //ScaleMin
			5.1, //ScaleMax
			"V", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Illumination 2",
			"ILL2",
			8, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			255, //RangeMax
			0, //ScaleMin
			510, //ScaleMax
			"lx", //Unit
			{{0, NULL}}, //Enum
	    },
		{
			1,
			"Illumination 1",
			"ILL1",
			16, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			255, //RangeMax
			0, //ScaleMin
			1020, //ScaleMax
			"lx", //Unit
			{{0, NULL}}, //Enum
	    },
		{
			3,
			"Range select",
			"RS",
			31, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //
			{{0, "Range acc. to DB_1 (ILL1)"},{1, "Range acc. to DB_2 (ILL2)"}}, //Enum
	    },
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};
	
	static DATAFIELD A5_06_03[3] =
	{
		{
			1,
			"Supply voltage",
			"SVC",
			0, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			255, //RangeMax
			0, //ScaleMin
			5.0, //ScaleMax
			"V", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Illumination",
			"ILL",
			8, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			0, //ScaleMin
			510, //ScaleMax
			"lx", //Unit
			{{0, NULL}}, //Enum
	    },
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};
	
	static DATAFIELD A5_14_05[3] =
	{
		{
			1,
			"Supply voltage",
			"SVC",
			0, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			255, //RangeMax
			0, //ScaleMin
			5.0, //ScaleMax
			"V", //Unit
			{{0, NULL}}, //Enum
		},
		{
			3,
			"Vibration",
			"VIB",
			30, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //
			{{0, "No vibration detected"},{1, "Vibration detected"}}, //Enum
	    },
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};
	
	static DATAFIELD D2_03_20_ES =
	{
		3,
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

	static DATAFIELD D2_14_40[9] =
	{
		{
			1,
			"Temperature 10",
			"TP",
			0, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-40, //ScaleMin
			60, //ScaleMax
			"℃", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Humidity",
			"HU",
			10, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			200, //RangeMax
			0, //ScaleMin
			100, //ScaleMax
			"%", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Illumination",
			"IL",
			18, //Bitoffs
			17, //Bitsize
			0, //RangeMin
			100000, //RangeMax
			0, //ScaleMin
			100000, //ScaleMax
			"lx", //Unit
			{{0, NULL}}, //Enum
		},
		{
			3,
			"Acceleration Status",
			"AS",
			35, //Bitoffs
			2, //Bitsize
			0, //RangeMin
			3, //RangeMax
			0, //ScaleMin
			3, //ScaleMax
			"", //Unit
			{{0, "Periodic Update"},{1, "Threshold 1 exceeded"}, {2, "Threshold 2 exceeded"}}, //Enum
		},
		{
			1,
			"Acceleration X",
			"AX",
			37, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-2.5, //ScaleMin
			2.5, //ScaleMax
			"g", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Acceleration Y",
			"AY",
			47, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-2.5, //ScaleMin
			2.5, //ScaleMax
			"g", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Acceleration Z",
			"AZ",
			57, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-2.5, //ScaleMin
			2.5, //ScaleMax
			"g", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};						

	static DATAFIELD D2_14_41[10] =
	{
		{
			1,
			"Temperature 10",
			"TP",
			0, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-40, //ScaleMin
			60, //ScaleMax
			"℃", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Humidity",
			"HU",
			10, //Bitoffs
			8, //Bitsize
			0, //RangeMin
			200, //RangeMax
			0, //ScaleMin
			100, //ScaleMax
			"%", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Illumination",
			"IL",
			18, //Bitoffs
			17, //Bitsize
			0, //RangeMin
			100000, //RangeMax
			0, //ScaleMin
			100000, //ScaleMax
			"lx", //Unit
			{{0, NULL}}, //Enum
		},
		{
			3,
			"Acceleration Status",
			"AS",
			35, //Bitoffs
			2, //Bitsize
			0, //RangeMin
			3, //RangeMax
			0, //ScaleMin
			3, //ScaleMax
			"", //Unit
			{{0, "Periodic Update"},{1, "Threshold 1 exceeded"}, {2, "Threshold 2 exceeded"}}, //Enum
		},
		{
			1,
			"Acceleration X",
			"AX",
			37, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-2.5, //ScaleMin
			2.5, //ScaleMax
			"g", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Acceleration Y",
			"AY",
			47, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-2.5, //ScaleMin
			2.5, //ScaleMax
			"g", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Acceleration Z",
			"AZ",
			57, //Bitoffs
			10, //Bitsize
			0, //RangeMin
			1000, //RangeMax
			-2.5, //ScaleMin
			2.5, //ScaleMax
			"g", //Unit
			{{0, NULL}}, //Enum
		},
		{
			3,
			"Contact",
			"CO",
			67, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //Unit
			{{0, "Open"}, {1, "Close"}}, //Enum
		},
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};						

	static DATAFIELD D2_32_00[4] =
	{
		{
			3,
			"Power Fail",
			"PF",
			0, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //Unit
			{{0, "False"},{1, "True"}}, //Enum
		},
		{
			3,
			"Divisor for all channels",
			"DIV",
			1, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //Unit
			{{0, "x/1"},{1, "x/10"}}, //Enum
		},
		{
			1,
			"Current value",
			"CH",
			8, //Bitoffs
			12, //Bitsize
			0, //RangeMin
			0xFFF, //RangeMax
			0, //ScaleMin
			4095, //ScaleMax
			"A", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};						

	static DATAFIELD D2_32_01[5] =
	{
		{
			3,
			"Power Fail",
			"PF",
			0, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //Unit
			{{0, "False"},{1, "True"}}, //Enum
		},
		{
			3,
			"Divisor for all channels",
			"DIV",
			1, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //Unit
			{{0, "x/1"},{1, "x/10"}}, //Enum
		},
		{
			1,
			"Current value",
			"CH",
			8, //Bitoffs
			12, //Bitsize
			0, //RangeMin
			0xFFF, //RangeMax
			0, //ScaleMin
			4095, //ScaleMax
			"A", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Current value",
			"CH",
			20, //Bitoffs
			12, //Bitsize
			0, //RangeMin
			0xFFF, //RangeMax
			0, //ScaleMin
			4095, //ScaleMax
			"A", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};						

	static DATAFIELD D2_32_02[6] =
	{
		{
			3,
			"Power Fail",
			"PF",
			0, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //Unit
			{{0, "False"},{1, "True"}}, //Enum
		},
		{
			3,
			"Divisor for all channels",
			"DIV",
			1, //Bitoffs
			1, //Bitsize
			0, //RangeMin
			1, //RangeMax
			0, //ScaleMin
			1, //ScaleMax
			"", //Unit
			{{0, "x/1"},{1, "x/10"}}, //Enum
		},
		{
			1,
			"Current value",
			"CH",
			8, //Bitoffs
			12, //Bitsize
			0, //RangeMin
			0xFFF, //RangeMax
			0, //ScaleMin
			4095, //ScaleMax
			"A", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Current value",
			"CH",
			20, //Bitoffs
			12, //Bitsize
			0, //RangeMin
			0xFFF, //RangeMax
			0, //ScaleMin
			4095, //ScaleMax
			"A", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,
			"Current value",
			"CH",
			32, //Bitoffs
			12, //Bitsize
			0, //RangeMin
			0xFFF, //RangeMax
			0, //ScaleMin
			4095, //ScaleMax
			"A", //Unit
			{{0, NULL}}, //Enum
		},
		{
			1,"","",0,0,0,0,0,0,"",{{0, NULL}},
		}
	};						

	static DATAFIELD D5_00_01_CO =
	{
		3,
		"Contact",
		"CO",
		7, //Bitoffs
		1, //Bitsize
		0, //RangeMin
		1, //RangeMax
		0, //ScaleMin
		1, //ScaleMax
		"", //Unit
		{{0, "open"},{1, "closed"}}, //Enum
	}; 

	stateEep = 0;
	stateProfile = 0;
	stateRorg = 0;
	stateFunc = 0;
	count = 0;
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
	}
	else {
		_D printf("<<start eep>>\n"); 
		
		while((ret = xmlTextReaderRead(reader)) > 0) {
			numField = ProcessNode(reader, &EepTable[count]);
			//if (numField > 0) {
			//if (EepTable[count].Eep[0] != '\0' && EepTable[i].Dtable != NULL && numField > 0) {
			if (EepTable[count].Eep[0] != '\0') {
				_DD printf("<<count:%d %d %s>>\n", count, numField, EepTable[count].Eep);
				count++;
			}
		}
		_D printf("<<end count=%d>>\n", count);
	}
#if 0 // DEBUG
	do {
		int i;

		for(i = 0; i < count; i++) {
			printf("%d: EEP=%s,%s t=%p\n",
			       i, EepTable[i].Eep, EepTable[i].Title, EepTable[i].Dtable);
		}
	}
	while(0);
#endif

	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 1, "A5-02-05",
		    "Temperature Sensor Range 0°C to +40°C",
		    &A5_02_05[0]) > 0) {
		count++;
	}

	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 3, "A5-04-01",
		    "Temperature and Humidity Sensor",
		    &A5_04_01[0]) > 0) {
		count++;
	}
    
	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 3, "A5-04-03",
		    "Temperature and Humidity Sensor",
		    &A5_04_03[0]) > 0) {
		count++;
	}
    
	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 4, "A5-06-02",
		    "Light Sensor",
		    &A5_06_02[0]) > 0) {
		count++;
	}
   
	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 2, "A5-06-03",
		    "Light Sensor",
		    &A5_06_03[0]) > 0) {
		count++;
	}
   
	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 2, "A5-14-05",
		    "Vibration/Tilt, Supply voltage monitor",
		    &A5_14_05[0]) > 0) {
		count++;
	}
	
	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 1, "D2-03-20",
		    "Beacon with Vibration Detection",
		    &D2_03_20_ES) > 0) { //Add Custom "D2-03-20",
		count++;
	}

	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 8, "D2-14-40",
		    "Multi Function Sensors",
		    &D2_14_40[0]) > 0) {
		count++;
	}
	
	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 9, "D2-14-41",
		    "Multi Function Sensors",
		    &D2_14_41[0]) > 0) {
		count++;
	}

	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 3, "D2-32-00",
		    "A.C. Current Clamp",
		    &D2_32_00[0]) > 0) {
		count++;
	}

	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 4, "D2-32-01",
		    "A.C. Current Clamp",
		    &D2_32_01[0]) > 0) {
		count++;
	}

	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 5, "D2-32-02",
		    "A.C. Current Clamp",
		    &D2_32_02[0]) > 0) {
		count++;
	}

	_D printf("AddEEP count=%d\n", count);
	if (SaveEep(&EepTable[count], 1, "D5-00-01",
		    "Single Input Contact",
		    &D5_00_01_CO) > 0) {
		count++;
	}

	_D printf("AddEEP LAST count=%d\n", count);
	SaveEep(&EepTable[count], 0, "\0", "\0", NULL); //Add end if table mark

	if (reader != NULL) {
		xmlFreeTextReader(reader);
	}
	if (ret == -1) {
		fprintf(stderr, "Parse error.\n");
		return 0;
	}
	return 1; // success
}
