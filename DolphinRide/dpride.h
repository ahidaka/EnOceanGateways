//
//
#pragma once

typedef unsigned char byte;
typedef int bool;
typedef unsigned int uint;
enum _boolvalue { false = 0, true = 1};

//
typedef enum _eo_file_op
{
        Ignore, Read, Clear
} EO_FILE_OP;

typedef enum _eo_mode
{
        Monitor, Register, Operation
} EO_MODE;

typedef struct _eo_control
{
        EO_MODE Mode;
        int CFlags;
        int VFlags;
        int Timeout;
	EO_FILE_OP FilterOp;
        int Debug;
	int ControlCount;
	int ERP1gw;
        char *ControlFile;
	char *ControlPath;
        char *CommandFile;
	char *CommandPath;
        char *BrokerFile;
        char *BrokerPath;
	char *PidPath;
        char *EEPFile;
        char *BridgeDirectory;
        char *ESPPort;

} EO_CONTROL;

typedef struct _eepdata
{
        int Porg;
        int Func;
        int Type;
        int ManID;
} EEP_DATA;

typedef struct _eo_port
{
	char *ESPPort;
        int Fd;
        int Opened;
} EO_PORT;

typedef enum _eo_packet_type
{
        Radio = 0x01,
        RadioErp1 = 0x01,
        Response = 0x02,
        RadioSubTel = 0x03,
        Event = 0x04,
        CommonCommand = 0x05,
        SmartAckCommand = 0x06,
        RmoteManCommand = 0x07,
        RadioMessage = 0x09,
        RadioErp2 = 0x0A,
        ConfigCommand = 0x0B,
        Radio802_15_4 = 0x10,
        Command2_4 = 0x11,
} EO_PACKET_TYPE;

//
//
#define Warn(msg)  fprintf(stderr, "#WARN %s: %s\n", __FUNCTION__, msg)
#define Error(msg)  fprintf(stderr, "*ERR %s: %s\n", __FUNCTION__, msg)
#define Error2(msg)  fprintf(stderr, "*ERR %s: %s=%s\n", __FUNCTION__, msg, arg)

void CleanUp(int Signum);

void DebugPrint(char *s);

void USleep(int Usec);

char *MakePath(char *Dir, char *File);

int EoReadControl();

void EoClearControl();

//
//
void EoParameter(int ac, char**av, EO_CONTROL *p);

void EoSetEep(EO_CONTROL *P, byte *Id, byte *Data, uint Rorg);

void PrintTelegram(EO_PACKET_TYPE packetType, byte *id, byte erp2hdr, byte *data);

bool CheckTableId(uint Target);

bool CheckTableEep(char *Target);

char *GetNewName(char *Target);

int ReadCsv(char *Filename);

int ReadCmd(char *Filename, int *Mode, char *Param);

uint GetId(int Index);

void WriteRpsBridgeFile(uint Id, byte *Data);

void Write1bsBridgeFile(uint Id, byte *Data);

void Write4bsBridgeFile(uint Id, byte *Data);

void WriteVldBridgeFile(uint Id, byte *Data);

void WriteBridge(char *FileName, double ConvertedData);

static inline void DataToEep(byte *data, uint *pFunc, uint *pType, uint *pMan)
{
        uint func = ((uint)data[0]) >> 2;
        uint type = ((uint)data[0] & 0x03) << 5 | ((uint)data[1]) >> 3;
        int manID = (data[1] & 0x07) << 8 | data[2];
        if (pFunc) *pFunc = func;
        if (pType) *pType = type;
        if (pMan) *pMan = manID;
}

static inline uint StrToId(byte str[4])
{
        char buf[8];
        *((uint *)buf) = *((uint *)&str[0]);
        *((uint *)&buf[4]) = 0UL;
        return(strtoul(buf, NULL, 16));
}

static inline uint ByteToId(byte Bytes[4])
{
        //return(*((uint *) Bytes));
        return((Bytes[0] << 24) |(Bytes[1] << 16) | (Bytes[2] << 8) | Bytes[3]);
}
