#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#include "typedefs.h"
#include "ptable.h"
#include "utils.h"

//
// defines
//
typedef enum _cm_durection
{
	CM_Unidirectional = 0,
	CM_Bidirectional = 1
} CM_DIRECTION;

typedef enum _cm_purpose
{
	CM_TeachIn = 0,
	CM_Deletion = 1,
	CM_Toggle = 2,
	CM_NotUsed = 3
} CM_PURPOSE;

typedef enum _cm_channel_type
{
	CM_Info = 0,
	CM_Data = 1,
	CM_Flag = 2,
	CM_Enum = 3
} CM_CHANNEL_TYPE;

typedef enum _cm_value_type
{
	CM_Reserved = 0,
	CM_CurrentValue = 1,
	CM_SetPointAbsolute = 2,
	CM_SetPointRelative = 3
} CM_VALUE_TYPE;

enum _cm_constants
{
	NumInfoData = 4,
	NumCommonModel = 32,
};

typedef struct _data_channel {
	int Type; // 2 bits
	int Signal; // 8bits
	int Value; // 2bits
	int Resolution; // 4bits
	int EngMin; // 8bits
	int ScaleMin; // 4bits
	int EngMax; // 8bits
	int ScaleMax; // 4bits
	int Length;   // 8bits, only for info_channel
        int BitOffset;  //
        int BitLength;  //
} DATA_CHANNEL, COMMON_CHANNEL;

typedef struct _flag_channel {
	int Type; // 2 bits
	int Signal; // 8bits
	int Value; // 2bits
} FLAG_CHANNEL;

typedef struct _enum_channel {
	int Type; // 2 bits
	int Signal; // 8bits
	int Value; // 2bits
	int Resolution; // 4bits
} ENUM_CHANNEL;

typedef struct _info_channel {
	int Type; // 2 bits
	int Signal; // 6bits
	int Length; // 8bits
	int Data[NumInfoData]; // Nbits
} INFO_CHANNEL;

typedef struct _common_model {
	int ManufactureID;
	int DataDirection;
	int Purpose;
	int Count;
	COMMON_CHANNEL *Models;
	int FpLength;
	BYTE *FingerPrint;
} COMMON_MODEL;

//
// Common models fuctions
//
void DecodeDebug(COMMON_MODEL *m);
void DecodeShort(BYTE *buf, COMMON_MODEL *m);

void PrintIt(char *name, char *dst, BYTE *src, INT len);
#if 0
void CmPrintTI(BYTE *Buf, BYTE *Eepbuf, BYTE *Data, INT len);
void CmPrintTR(BYTE *Buf, BYTE *Data, INT len);
void CmPrintCD(BYTE *Buf, BYTE *Data, INT len);
void CmPrintSD(BYTE *Buf, BYTE *Data, INT len);

VOID *CmAnalyze(IN BYTE *Start, IN INT Length, OUT INT *Count);
char *CmMakeString(VOID *Handle);
char *CmMakeTitle(VOID *Handle);
DATAFIELD *CmMakeDataField(VOID *Handle);
#endif

//
// constants
//
enum { DIRECTION_TABLE_SIZE = 2 };
static const char * _cm_direction_table[DIRECTION_TABLE_SIZE] =
{
	"Unidirectional",
	"Bidirectional"	
};

static inline const char *GetDirectionName(INT Index)
{
	return _cm_direction_table[Index & (DIRECTION_TABLE_SIZE - 1)];
}

enum { PURPOSE_TABLE_SIZE = 4 };
static const char * _cm_purpose_table[PURPOSE_TABLE_SIZE] =
{
	"TeachIn",
	"Deletion",
	"Toggle",
	"Not Used"
};

static inline const char *GetPurposeName(INT Index)
{
	return _cm_purpose_table[Index & (PURPOSE_TABLE_SIZE - 1)];
}

enum { TYPE_TABLE_SIZE = 4 };
static const char * _cm_type_table[TYPE_TABLE_SIZE] =
{
	"Info",
	"Data",
	"Flag",
	"Enum"
};

static inline const char *GetTypeName(INT Index)
{
	return _cm_type_table[Index & (TYPE_TABLE_SIZE - 1)];
}

enum { VALUE_TABLE_SIZE = 4 };
static const char * _cm_value_table[VALUE_TABLE_SIZE] =
{
	"Reserved",
	"CurrentValue",
	"SetPointAbsolute",
	"SetPointRelative" 
};

static inline const char *GetValueName(INT Index)
{
	return _cm_value_table[Index & (VALUE_TABLE_SIZE - 1)];
}

enum { RESOLUTION_TABLE_SIZE = 16 };
static const int _cm_resolution_table[RESOLUTION_TABLE_SIZE] =
{
	-1, // 0: Reserved
	2,  // 1: 2bit
	3,  // 2: 3bit
	4,  // 3: 4bit
	5,  // 4: 5bit
	6,  // 5: 6bit
	8,  // 6: 8bit
	10, // 7: 10bit
	12, // 8: 12bit
	16, // 9: 16bit
	20, // 10: 20bit
	24, // 11: 24bit
	32, // 12: 32bit
	-1, // 13: Reserved
	-1, // 14: Reserved
	-1, // 15: Reserved
};

static inline const int GetResolutionValue(INT Index)
{
	return _cm_resolution_table[Index & (RESOLUTION_TABLE_SIZE - 1)];
}

enum { SCALING_TABLE_SIZE = 16 };
static const double _cm_scaling_table[SCALING_TABLE_SIZE] =
{
	-1,          // 0: Reserved
	1.0,         // 1: x1
	10.0,        // 2: x10
	100.0,       // 3: x100
	10000.0,     // 4: x1,000
	100000.0,    // 5: x10,000
	1000000.0,   // 6: x100,000
	10000000.0,  // 7: x1,000,000
	100000000.0, // 8: x10,000,000
	0.1,         // 9: x0.1
	0.01,        // 10: x0.01
	0.001,       // 11: x0.001
	0.000001,    // 12: x0.000001
	0.000000001, // 13: x0.000000001
	-1,          // 14: Reserved
	-1           // 15: Reserved
};

static inline const double GetScalingValue(INT Index)
{
	return _cm_scaling_table[Index & (SCALING_TABLE_SIZE - 1)];
}

enum { DATA_NAME_SIZE = 64 };
static const char * _cm_data_name[DATA_NAME_SIZE] =
{
	"Reserved",          // 0 ""
	"Acceleration",      // 1 "m/s2"
	"Angle",             // 2 "°"
	"Angular_velocity",  // 3 "rad/s"
	"Area",              // 4 "㎡"
	"Concentration",     // 5 "ppm"
	"Current",           // 6 "A"
	"Distance",          // 7 "m"
	"Electric_field_strength", // 8 "V/m"
	"Energy",            // 9 "J"
	"Number",            // 10 ""
	"Force",             // 11 "N"
	"Frequency",         // 12 "Hz"
	"Heat_flux_density", // 13 "W/㎡"
	"Impulse",           // 14 "Ns"
	"Luminance",         // 15 "lux"
	"Magnetic_field_strength", // 16 "A/m"
	"Mass",              // 17 "kg"
	"Mass_density",      // 18 "kg/m3"
	"Mass_flow",         // 19 "kg/s"
	"Power",             // 20 "W"
	"Pressure",          // 21 "Pa"
	"Humidity",          // 22 "%"
	"Resistance",        // 23 "Ω"
	"Temperature",       // 24 "℃"
	"Time",              // 25 "s"
	"Torque",            // 26 "Nm"
	"Velocity",          // 27 "m/s"
	"Voltage",           // 28 "V"
	"Volume",            // 29 "m3"
	"Volumetric_Flow",   // 30 "m3/s"
	"Sound_Pressure_Level", // 31 "dB"
	"Correlated_Color_Temperature", // 32 "Kelvin"
	"", // 33 ""
	"", // 34 ""
	"", // 35 ""
	"", // 36 ""
	"", // 37 ""
	"", // 38 ""
	"", // 39 ""
	"", // 40 ""
	"", // 41 ""
	"", // 42 ""
	"", // 43 ""
	"", // 44 ""
	"", // 45 ""
	"", // 46 ""
	"", // 47 ""
	"", // 48 ""
	"", // 49 ""
	"", // 50 ""
	"", // 51 ""
	"", // 52 ""
	"", // 53 ""
	"", // 54 ""
	"", // 55 ""
	"", // 56 ""
	"", // 57 ""
	"", // 58 ""
	"", // 59 ""
	"", // 60 ""
	"", // 61 ""
	"", // 62 ""
	"", // 63 ""
};

static inline const char *GetDataName(INT Index)
{
	return _cm_data_name[Index & (DATA_NAME_SIZE - 1)];
}

enum { DATA_SHORTCUT_SIZE = 64 };
static const char * _cm_data_shortcut[DATA_SHORTCUT_SIZE] =
{
	"",       // 0 "Reserved",
	"ac",  // 1 "Acceleration",
	"ag",  // 2 "Angle",
	"av",  // 3 "Angular_velocity",
	"ar",  // 4 "Area",
	"co",  // 5 "Concentration",
	"cu",  // 6 "Current",
	"di",  // 7 "Distance",
	"ef",  // 8 "Electric_field_strength",
	"en",  // 9 "Energy",
	"nm",  // 10 "Number",
	"fo",  // 11 "Force",
	"fq",  // 12 "Frequency",
	"hf",  // 13 "Heat_flux_density",
	"ip",  // 14 "Impulse",
	"lu",  // 15 "Luminance",
	"mf",  // 16 "Magnetic_field_strength",
	"ms",  // 17 "Mass",
	"md",  // 18 "Mass_density",
	"mf",  // 19 "Mass_flow",
	"pw",  // 20 "Power",
	"pr",  // 21 "Pressure",
	"hu",  // 22 "Humidity",
	"rs",  // 23 "Resistance",
	"tp",  // 24 "Temperature",
	"tm",  // 25 "Time",
	"tq",  // 26 "Torque",
	"vc",  // 27 "Velocity",
	"vl",  // 28 "Voltage",
	"vo",  // 29 "Volume",
	"vf",  // 30 "Volumetric_Flow",
	"sp",  // 31 "Sound_Pressure_Level",
	"ct",  // 32 "Correlated_Color_Temperature",
	"", // 33 ""
	"", // 34 ""
	"", // 35 ""
	"", // 36 ""
	"", // 37 ""
	"", // 38 ""
	"", // 39 ""
	"", // 40 ""
	"", // 41 ""
	"", // 42 ""
	"", // 43 ""
	"", // 44 ""
	"", // 45 ""
	"", // 46 ""
	"", // 47 ""
	"", // 48 ""
	"", // 49 ""
	"", // 50 ""
	"", // 51 ""
	"", // 52 ""
	"", // 53 ""
	"", // 54 ""
	"", // 55 ""
	"", // 56 ""
	"", // 57 ""
	"", // 58 ""
	"", // 59 ""
	"", // 60 ""
	"", // 61 ""
	"", // 62 ""
	"", // 63 ""
};

static inline const char *GetDataShortcut(INT Index)
{
	return _cm_data_shortcut[Index & (DATA_SHORTCUT_SIZE - 1)];
}

enum { DATA_UNIT_SIZE = 64 };
static const char * _cm_data_unit[DATA_UNIT_SIZE] =
{
	"",       // 0 "Reserved",
	"m/s2",   // 1 "Acceleration",
	"°",     // 2 "Angle",
	"rad/s",  // 3 "Angular_velocity",
	"㎡",     // 4 "Area",
	"ppm",    // 5 "Concentration",
	"A",      // 6 "Current",
	"m",      // 7 "Distance",
	"V/m",    // 8 "Electric_field_strength",
	"J",      // 9 "Energy",
	"",       // 10 "Number",
	"N",      // 11 "Force",
	"Hz",     // 12 "Frequency",
	"W/㎡",   // 13 "Heat_flux_density",
	"Ns",     // 14 "Impulse",
	"lux",    // 15 "Luminance",
	"A/m",    // 16 "Magnetic_field_strength",
	"kg",     // 17 "Mass",
	"kg/m3",  // 18 "Mass_density",
	"kg/s",   // 19 "Mass_flow",
	"W",      // 20 "Power",
	"Pa",     // 21 "Pressure",
	"%",      // 22 "Humidity",
	"Ω",     // 23 "Resistance",
	"℃",     // 24 "Temperature",
	"s",      // 25 "Time",
	"Nm",     // 26 "Torque",
	"m/s",    // 27 "Velocity",
	"V",      // 28 "Voltage",
	"m3",     // 29 "Volume",
	"m3/s",   // 30 "Volumetric_Flow",
	"dB",     // 31 "Sound_Pressure_Level",
	"Kelvin", // 32 "Correlated_Color_Temperature",
	"", // 33 ""
	"", // 34 ""
	"", // 35 ""
	"", // 36 ""
	"", // 37 ""
	"", // 38 ""
	"", // 39 ""
	"", // 40 ""
	"", // 41 ""
	"", // 42 ""
	"", // 43 ""
	"", // 44 ""
	"", // 45 ""
	"", // 46 ""
	"", // 47 ""
	"", // 48 ""
	"", // 49 ""
	"", // 50 ""
	"", // 51 ""
	"", // 52 ""
	"", // 53 ""
	"", // 54 ""
	"", // 55 ""
	"", // 56 ""
	"", // 57 ""
	"", // 58 ""
	"", // 59 ""
	"", // 60 ""
	"", // 61 ""
	"", // 62 ""
	"", // 63 ""
};

static inline const char *GetDataUnit(INT Index)
{
	return _cm_data_unit[Index & (DATA_UNIT_SIZE - 1)];
}

enum { FLAG_NAME_SIZE = 16 };
static const char * _cm_flag_name[FLAG_NAME_SIZE] =
{
	"Reserved", // 0
	"Automatic / manual", // 1
	"Button pressed", // 2
	"Button changed", // 3
	"Day / night", // 4
	"Down -", // 5
	"General alarm", // 6
	"Heat / cool", // 7
	"High / low", // 8
	"Occupancy", // 9
	"On / off", // 10
	"Open / closed", // 11
	"Power alarm", // 12
	"Start / stop", // 13
	"Up +", // 14
	""  // 15
};

static inline const char *GetFlagName(INT Index)
{
	return _cm_flag_name[Index & (FLAG_NAME_SIZE - 1)];
}

enum { FLAG_SHORTCUT_SIZE = 16 };
static const char * _cm_flag_shortcut[FLAG_SHORTCUT_SIZE] =
{
	"", // 0
	"am", // 1
	"bp", // 2
	"bc", // 3
	"dn", // 4
	"dw", // 5
	"ga", // 6
	"hc", // 7
	"hl", // 8
	"op", // 9
	"of", // 10
	"oc", // 11
	"pa", // 12
	"ss", // 13
	"up", // 14
	""  // 15
};

static inline const char *GetFlagShortcut(INT Index)
{
	return _cm_flag_shortcut[Index & (FLAG_SHORTCUT_SIZE - 1)];
}

enum { ENUM_NAME_SIZE = 16 };
static const char * _cm_enum_name[ENUM_NAME_SIZE] =
{
	"Reserved", // 0
	"Multipurpose", // 1
	"Building Mode", // 2
	"Occupancy Mode", // 3
	"HVAC Mode", // 4
	"Changeover Mode", // 5
	"Time", // 6
	"Battery", // 7
	"", // 8
	"", // 9
	"", // 10
	"", // 11
	"", // 12
	"", // 13
	"", // 14
	""  // 15
};

static inline const char *GetEnumName(INT Index)
{
	return _cm_enum_name[Index & (ENUM_NAME_SIZE - 1)];
}

enum { ENUM_SHORTCUT_SIZE = 16 };
static const char * _cm_enum_shortcut[ENUM_SHORTCUT_SIZE] = 
{
	"", // 0
	"ml", // 1
	"bm", // 2
	"om", // 3
	"hm", // 4
	"cm", // 5
	"tm", // 6
	"bt", // 7
	"", // 8
	"", // 9
	"", // 10
	"", // 11
	"", // 12
	"", // 13
	"", // 14
	""  // 15
};

static inline const char *GetEnumShortcut(INT Index)
{
	return _cm_enum_shortcut[Index & (ENUM_SHORTCUT_SIZE - 1)];
}

enum { INFO_NAME_SIZE = 16 };
static const char * _cm_info_name[INFO_NAME_SIZE] = 
{
	"Reserved", // 0
	"Inbound Channels description following",  // 1
	"Product ID",  // 2
	"Connected GSI Sensor IDs",  // 3
	"",  // 4
	"",  // 5
	"",  // 6
	"",  // 7
	"",  // 8
	"",  // 9
	"",  // 10
	"",  // 11
	"",  // 12
	"",  // 13
	"",  // 14
	""   // 15
};

static inline const char *GetInfoName(INT Index)
{
	return _cm_info_name[Index & (INFO_NAME_SIZE - 1)];
}

enum { INFO_SHORTCUT_SIZE = 16 };
static const char * _cm_info_shortcut[INFO_SHORTCUT_SIZE] = 
{
	"", // 0
	"ic",  // 1
	"pt",  // 2
	"cs",  // 3
	"",  // 4
	"",  // 5
	"",  // 6
	"",  // 7
	"",  // 8
	"",  // 9
	"",  // 10
	"",  // 11
	"",  // 12
	"",  // 13
	"",  // 14
	""   // 15
};

static inline const char *GetInfoShortcut(INT Index)
{
	return _cm_info_shortcut[Index & (INFO_SHORTCUT_SIZE - 1)];
}

static const char * _cm_enum_value_name[8][6] =
{
	{NULL}, //  0
	{NULL}, // 1
	{"in use", "not used", "protection", NULL, NULL, NULL}, // 2
	{"Occupied", "Standby", "Not occupied", NULL, NULL, NULL}, // 3
	{"Auto", "Comfort", "Standby", "Economy", "Building Protection", NULL}, // 4
	{"Auto", "Cooling only", "Heating only", NULL, NULL, NULL}, //5
	{NULL}, //6
	{NULL}  //7
};

//
// Read string from "Start" and generate "common model" record.
// Return model handle, output Count is number of channels.
//
VOID *CmAnalyze(IN BYTE *Start, IN INT Length, OUT INT *Count)
{
	const INT headerLen = 2;
	INT bitOffset = 0;
	INT i, numBytes;
	INT manId, dataDir, purpose;
	BYTE *p = Start;
	INT dataBitOffset = 0;
	COMMON_MODEL *m;
	COMMON_CHANNEL *pm;
	COMMON_CHANNEL channels[NumCommonModel];

	if (Length < 4) {
		fprintf(stderr, "length error = %d\n", Length);
		return NULL;
	}
	//Check ManufactureID, DataDirection, Purpose
        //printf("%02X %02X %02X %02X %02X %02X\n",
	//       *p, *(p + 1), *(p + 2), *(p + 3), *(p + 4), *(p + 5));
	manId = GetBits(p, 0, 11);
	dataDir = GetBits(p, 11, 1);
	purpose = GetBits(p, 12, 2);
	if (manId == 0 || purpose == 3) {
		fprintf(stderr, "manId or purpose error=%03X,%d\n", manId, purpose);
		return NULL;
	}

	m = calloc(sizeof(COMMON_MODEL), 1);
	if (m == NULL) {
		fprintf(stderr, "calloc error\n");
		return NULL;
	}
	m->ManufactureID = manId;
	m->DataDirection = dataDir;
	m->Purpose = purpose;
	p += headerLen; // skip header to channel definition
	for(i = 0; i < NumCommonModel; i++) {

		memset(&channels[i], 0, sizeof(COMMON_CHANNEL));
		numBytes = headerLen + (bitOffset + 7) / 8;
		if (numBytes >= Length) {
			break;
		}
		pm = &channels[i];
		pm->Type = GetBits(p, bitOffset + 0, 2);
		pm->Signal = 0;
		//printf("bitOffset=%d dataBitOffset=%d, type=%d\n",
		//       bitOffset, dataBitOffset, pm->Type);
		
		switch(pm->Type) {
		case CM_Info:
			pm->Signal = GetBits(p, bitOffset + 2, 6);
			pm->Length = GetBits(p, bitOffset + 8, 8);
			if (pm->Signal == 0) {
				// End of models data
				goto FINAL;
			}
			bitOffset += (16 + pm->Length * 8);
			pm->BitOffset = -1;
			pm->BitLength = 0;
			break;
			
		case CM_Data:
			pm->Signal = GetBits(p, bitOffset + 2, 8);
			pm->Value = GetBits(p, bitOffset + 10, 2);
			pm->Resolution = GetBits(p, bitOffset + 12, 4);
			pm->EngMin = GetBits(p, bitOffset + 16, 8);
			if (pm->EngMin >= 128)
				pm->EngMin = -(256 - pm->EngMin);
			pm->ScaleMin = GetBits(p, bitOffset + 24, 4);
			pm->EngMax = GetBits(p, bitOffset + 28, 8);
			if (pm->EngMax >= 128)
				pm->EngMax = -(256 - pm->EngMax);
			pm->ScaleMax = GetBits(p, bitOffset + 36, 4);
			bitOffset += 40;
			pm->BitOffset = dataBitOffset;
			pm->BitLength = GetResolutionValue(pm->Resolution);
			dataBitOffset += pm->BitLength;
			break;
			
		case CM_Flag:
			pm->Signal = GetBits(p, bitOffset + 2, 8);
			pm->Value =  GetBits(p, bitOffset + 10, 2);
			bitOffset += 12;
			pm->BitOffset = dataBitOffset;
			pm->BitLength = 1;
			dataBitOffset++;
			break;
			
		case CM_Enum:
			pm->Signal = GetBits(p, bitOffset + 2, 8);
			pm->Value = GetBits(p, bitOffset + 10, 2);
			pm->Resolution = GetBits(p, bitOffset + 12, 4);
			bitOffset += 16;
			pm->BitOffset = dataBitOffset;
			pm->BitLength = GetResolutionValue(pm->Resolution);
			dataBitOffset += pm->BitLength;
			break;
		}
	}

FINAL:
	m->Count = i;
	m->Models = MemDup(&channels[0], sizeof(COMMON_CHANNEL) * (i + 1));
	m->FpLength = numBytes;
	m->FingerPrint = MemDup(Start, numBytes);
	if (Count != NULL) {
		*Count = m->Count;
	}
	//printf("##numBytes=%d\n", numBytes);
	return (VOID *) m;
}

//
// Debug print for whole structure member of CM.
//
void DecodeDebug(IN COMMON_MODEL *m)
{
	COMMON_CHANNEL *pm;
	int i, fpSize;
	const int limit = 16;
	
	printf("ED: ManufactureID=%03X\n", m->ManufactureID);
	printf("ED: DataDirection=%s(%d)\n", GetDirectionName(m->DataDirection), m->DataDirection);
	printf("ED: Purpose=%s(%d)\n", GetPurposeName(m->Purpose), m->Purpose);
	printf("ED: Count=%d\n", m->Count);
	printf("ED: Length=%d\nFP:\n", m->FpLength);
	fpSize = limit;
	if (m->FpLength < limit)
		fpSize = m->FpLength;

	for(i = 0; i < fpSize + 2; i++) {
		printf("%02X ", m->FingerPrint[i]);
	}
	printf("\n");

	for(i = 0; i < NumCommonModel; i ++) {
		pm = &m->Models[i];

		printf("ED: *%d* %s(%d):\n", i, GetTypeName(pm->Type), pm->Type);

		switch(pm->Type) {
		case CM_Info:
			if (pm->Signal == 0) {
				printf("\n");
				return;
			}			
			printf("ED: %s %s(%d) Length=%d\n",
			       GetInfoShortcut(pm->Signal),
			       GetInfoName(pm->Signal),
			       pm->Signal, pm->Length);
			break;
			
		case CM_Data:
			printf("ED: Data Value=%s(%d) Signal=%s %s(%d) (%s) ",
			       GetValueName(pm->Value), pm->Value,
			       GetDataShortcut(pm->Signal),
			       GetDataName(pm->Signal),
			       pm->Signal, GetDataUnit(pm->Signal));
			printf("    Resolution=%d bit(%d) Min=%d*%9.9f(%d)[%9.9f] Max=%d*%9.9f(%d)[%9.9f]\n",
			       GetResolutionValue(pm->Resolution), pm->Resolution,
			       pm->EngMin, GetScalingValue(pm->ScaleMin),
			       pm->ScaleMin, pm->EngMin * GetScalingValue(pm->ScaleMin),
			       pm->EngMax, GetScalingValue(pm->ScaleMax), pm->ScaleMax,
			       pm->EngMax * GetScalingValue(pm->ScaleMax));
			break;
			
		case CM_Flag:
			printf("ED: Flag Value=%s(%d) Signal=%s %s(%d)\n",
			       GetValueName(pm->Value), pm->Value,
			       GetFlagShortcut(pm->Signal),
			       GetFlagName(pm->Signal), pm->Signal);
			break;

		case CM_Enum:
			printf("ED: Enum Value=%s(%d) Signal=%s %s(%d) ",
			       GetValueName(pm->Value), pm->Value,
			       GetEnumShortcut(pm->Signal), GetEnumName(pm->Signal), pm->Signal);
			printf("ED: Resolution=%d bit(%d)\n",
			       GetResolutionValue(pm->Resolution), pm->Resolution);
			break;
		}
		printf("ED: BitOffset=%d BitLength=%d\n\n", pm->BitOffset, pm->BitLength);
	}
}

//
// Debug only
//
void DecodeShort(IN BYTE *out, IN COMMON_MODEL *m)
{
	COMMON_CHANNEL *pm;
	int i;
	int len;

	if (out == NULL) {
		fprintf(stderr, "******** out is NULL!\n");
		return;
	}
	
	for(i = 0; i < NumCommonModel; i ++) {
		pm = &m->Models[i];

		switch(pm->Type) {
		case CM_Info:
			if (pm->Signal == 0) {
				// Last
				return;
			}
			len = sprintf((char *) out, "%s%d", GetInfoShortcut(pm->Signal), pm->Length);
			out += len;
			break;
			
		case CM_Data:
			len = sprintf((char *) out, "%s%d", GetDataShortcut(pm->Signal),
				      GetResolutionValue(pm->Resolution));
			out += len;
			break;
			
		case CM_Flag:
			len = sprintf((char *) out, "%s1", GetFlagShortcut(pm->Signal));
			out += len;
			break;

		case CM_Enum:
                        len = sprintf((char *) out, "%s%d", GetEnumShortcut(pm->Signal),
				      GetResolutionValue(pm->Resolution));
			out += len;
			break;
		}
	}
}

//
//
//
void PrintIt(char *name, char *dst, BYTE *src, INT len)
{
        int i;

        sprintf(&dst[0], "%s: ", name);
        dst += 4;
        for(i = 0; i < len; i++) {
                sprintf(&dst[i * 2], "%02X ", src[i]);
                dst++;
        }
}

void CmPrintTI(BYTE *Buf, BYTE *Eepbuf, BYTE *Data, INT len)
{
	COMMON_MODEL *model;
	int count;
	
        PrintIt("TI", (char *) Buf, Data, len);
	model = CmAnalyze(Data, len, &count);
	if (model != NULL) {
		DecodeShort(Eepbuf, model);
	}
	else {
		strcpy((char *) Eepbuf, "[unknown]");
	}
}

void CmPrintTR(BYTE *Buf, BYTE *Data, INT len)
{
        PrintIt("TR", (char *) Buf, Data, len);
}

void CmPrintCD(BYTE *Buf, BYTE *Data, INT len)
{
        PrintIt("CD", (char *) Buf, Data, len);
}

void CmPrintSD(BYTE *Buf, BYTE *Data, INT len)
{
        PrintIt("SD", (char *) Buf, Data, len);
}

//
//
//
INT CmFpSearch(IN CM_TABLE *pmc, IN BYTE *Buf, IN INT Size)
{
	int i, len;
	BYTE *p;

	for(i = 0; i < CM_CACHE_SIZE; i++) {
		if (pmc->CmHandle == NULL) {
			// end of entry
			break;
		}
		
		len = ((COMMON_MODEL *) pmc->CmHandle)->FpLength;
		p = ((COMMON_MODEL *) pmc->CmHandle)->FingerPrint;
		if (p == NULL) {
			// End of data
			break;
		}
		else if (Size != len) {
			; // not matched length
		}
		else if (!memcmp(p, Buf, Size))  {
			// got it!
			return i;
		}
		pmc++;
	}
	return -1;
}

//
// Make "tp8hu12tp8ac12" style CM-string
// Return enw pointer to new CM-string
//
char *CmMakeString(IN VOID *Handle)
{
	int i, len;
	COMMON_MODEL *model = (COMMON_MODEL *) Handle;
	COMMON_CHANNEL *pm;
	char string[CM_STRSIZE];
	char *out = string;
	
	for(i = 0; i < NumCommonModel; i ++) {
		pm = &model->Models[i];

		switch(pm->Type) {
		case CM_Info:
			if (pm->Signal == 0) {
				// Last
				return(strdup(string));
			}
			len = sprintf((char *) out, "%s%d", GetInfoShortcut(pm->Signal), pm->Length);
			out += len;
			break;
			
		case CM_Data:
			len = sprintf((char *) out, "%s%d", GetDataShortcut(pm->Signal),
				      GetResolutionValue(pm->Resolution));
			out += len;
			break;
			
		case CM_Flag:
			len = sprintf((char *) out, "%s1", GetFlagShortcut(pm->Signal));
			out += len;
			break;

		case CM_Enum:
                        len = sprintf((char *) out, "%s%d", GetEnumShortcut(pm->Signal),
				      GetResolutionValue(pm->Resolution));
			out += len;
			break;
		}
		
		if ((out - string) >= CM_STRBASE) {
			// exceeded, truncate it and return
			break;
		}
	}
	string[CM_STRBASE] = '\0';
	return(strdup(string));
}

//
// Make "Temperature. tp. ac. +3" style Title from Model
// return new pointer to net Title
//
char *CmMakeTitle(IN VOID *Handle)
{
	int i, len;
	int remainCount;
	COMMON_MODEL *model = (COMMON_MODEL*) Handle;
	COMMON_CHANNEL *pm;
	char string[128];
	char *out = string;
	
	for(i = 0; i < NumCommonModel; i++) {
		pm = &model->Models[i];

		if (i >= 3) {
			remainCount = model->Count - i;
			if (remainCount > 0) {
				len = sprintf((char *) out, ". +%d", remainCount);
			}
			return(strdup(string));
		}
		
		switch(pm->Type) {
		case CM_Info:
			if (pm->Signal == 0) {
				// Last
				return(strdup(string));
			}
			if (i == 0) {
				// The first item
				len = sprintf((char *) out, "%s", GetInfoName(pm->Signal));
				out += len;
			}
			else {
				len = sprintf((char *) out, ". %s", GetInfoShortcut(pm->Signal));
				out += len;
			}
			break;
			
		case CM_Data:
			if (i == 0) {
				// The first item
				len = sprintf((char *) out, "%s", GetDataName(pm->Signal));
				out += len;
			}
			else {
				len = sprintf((char *) out, ". %s", GetDataShortcut(pm->Signal));
				out += len;
			}
			break;
			
		case CM_Flag:
			if (i == 0) {
				// The first item
				len = sprintf((char *) out, "%s", GetFlagName(pm->Signal));
				out += len;
			}
			else {
				len = sprintf((char *) out, ". %s", GetFlagShortcut(pm->Signal));
				out += len;
			}
			break;
			
		case CM_Enum:
			if (i == 0) {
				// The first item
				len = sprintf((char *) out, "%s", GetEnumName(pm->Signal));
				out += len;
			}
			else {
				len = sprintf((char *) out, ". %s", GetEnumShortcut(pm->Signal));
				out += len;
			}
			break;
		}
	}
	return(strdup(string)); // maybe not used
}

//
// convert parameters from model handle to DATAFIELD for each record
// Return DATAFIELD pointer
//
static const unsigned long ff = (unsigned long)(-1);
#if 0
static long MakeMin(int NumBit)
{
        int i;
        const unsigned long mask = ~1UL;
        unsigned long target = ff;
        const int keyBit = NumBit - 1;

        for(i = 0; i < keyBit; i++) {
                target &= mask << i;
                //printf("%d: %ld(0x%08lX)\n", i, target, target);
        }

        return (long) target;
}

static long MakeMax(int NumBit)
{
        int i;
        const unsigned long mask = 1UL;
        unsigned long target = 0;
        const int keyBit = NumBit - 1;

        for(i = 0; i < keyBit; i++) {
                target |= mask << i;
                //printf("%d: %ld(0x%08lX)\n", i, target, target);
        }

        return (long) target;
}
#endif
DATAFIELD *CmMakeDataField(IN VOID *Handle)
{
	COMMON_MODEL *model = ((COMMON_MODEL *) Handle);
	COMMON_CHANNEL *pm;
	DATAFIELD *dataFields = calloc(model->Count + 1, sizeof(DATAFIELD));
	DATAFIELD *pd;
	int i, j;
	//long minBitValue;
	long maxBitValue;

	if (dataFields == NULL) {
		fprintf(stderr, "calloc datafield error\n");
		return NULL;
	}

	for(i = 0; i < model->Count; i++) {
		pm = &model->Models[i];
		pd = &dataFields[i];
		pd->ValueType = pm->Type;
		maxBitValue = (1 << pm->BitLength) - 1;
		
		switch(pm->Type) {
		case CM_Info:
			if (pm->Signal == 0) {
				return dataFields;
			}
			pd->DataName = strdup(GetInfoName(pm->Signal));
			pd->ShortCut = strdup(GetInfoShortcut(pm->Signal));

			pd->BitOffs = 0;
			pd->BitSize = 0;
			pd->RangeMin = 0;
			pd->RangeMax = 0;
			pd->ScaleMin = 0;
			pd->ScaleMax = 0;
			pd->Unit = NULL;
			pd->ValueType = Enum; // mark this is not end of data.
			break;
			
		case CM_Data:
			pd->DataName = strdup(GetDataName(pm->Signal));
			pd->ShortCut = strdup(GetDataShortcut(pm->Signal));

			pd->BitOffs = pm->BitOffset;
			pd->BitSize = pm->BitLength;
			pd->RangeMin = 0;
			pd->RangeMax = maxBitValue;
			pd->ScaleMin = pm->EngMin * GetScalingValue(pm->ScaleMin);
			pd->ScaleMax = pm->EngMax * GetScalingValue(pm->ScaleMax);
			pd->Unit = (char *) GetDataUnit(pm->Signal);
			
			//printf("###CmMakeDataField<%s> %f %d %f %f %d %f\n", pd->DataName,
			//       pd->ScaleMin, pm->EngMin, GetScalingValue(pm->ScaleMin), 
			//       pd->ScaleMax, pm->EngMax, GetScalingValue(pm->ScaleMax));
			
			break;
			
		case CM_Flag:
			pd->DataName = strdup(GetFlagName(pm->Signal));
			pd->ShortCut = strdup(GetFlagShortcut(pm->Signal));

			pd->BitOffs = pm->BitOffset;
			pd->BitSize = 1;
			pd->RangeMin = 0;
			pd->RangeMax = 0;
			pd->ScaleMin = 0;
			pd->ScaleMax = 0;
			pd->Unit = NULL;
			break;

		case CM_Enum:
			pd->DataName = strdup(GetEnumName(pm->Signal));
			pd->ShortCut = strdup(GetEnumShortcut(pm->Signal));

			pd->BitOffs = pm->BitOffset;
			pd->BitSize = pm->BitLength;
			pd->RangeMin = 0;
			pd->RangeMax = 0;
			pd->ScaleMin = 0;
			pd->ScaleMax = 0;
			pd->Unit = NULL;

			switch (pm->Signal) {
			case 2: // Building Mode
				for(j = 0; j < 3; j++) {
					pd->EnumDesc[j].Index = j;
					pd->EnumDesc[j].Desc = (char *) _cm_enum_value_name[pm->Signal][j];
				}
				break;
			case 3: // Occupancy Mode
				for(j = 0; j < 3; j++) {
					pd->EnumDesc[j].Index = j;
					pd->EnumDesc[j].Desc = (char *) _cm_enum_value_name[pm->Signal][j];
				}
				break;
			case 4: // HVAC Mode
				for(j = 0; j < 5; j++) {
					pd->EnumDesc[j].Index = j;
					pd->EnumDesc[j].Desc = (char *) _cm_enum_value_name[pm->Signal][j];
				}
				break;
			case 5: // Changeover Mode
				for(j = 0; j < 3; j++) {
					pd->EnumDesc[j].Index = j;
					pd->EnumDesc[j].Desc = (char *) _cm_enum_value_name[pm->Signal][j];
				}
				break;
			default:
				break;
			}
			break;
		}
	}
	return dataFields;
}

//
// Delete COMMON_CHANNEL
//
VOID CmDelete(VOID *Handle)
{
	COMMON_MODEL *m = ((COMMON_MODEL *) Handle);

        if (Handle == NULL) {
                fprintf(stderr, "*CmDelete: Handle == NULL\n");
        }
	IF_EXISTS_FREE(m->Models);
	IF_EXISTS_FREE(m->FingerPrint);

	free(m);
}
