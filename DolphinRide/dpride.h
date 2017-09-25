//
//
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
        char *ControlFile;
        char *FilterFile;
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
void DebugPrint(char *s);

void USleep(int Usec);

void MakePath(char *Path, char *Dir, char *File);

int EoReadFilter(char *File, long *List, int Size);

void EoClearFilter(char *File);

bool EoReceiveFilter(long *FilterList, int FilterCount);

void EoBufferFilter(char *buffer, int id);

void EoSetFilter(bool enable);

bool EoPortOpen();

bool EoPortWrite(byte * Buffer, int Offset, int Length);

bool EoGetHeader(EO_PACKET_TYPE *outPacketType, int *outDataLength, int *outOptionLength);

int EoGetBody(EO_PACKET_TYPE PacketType, int DataLength, int OptionLength,
              byte *Id, byte *Erp2Hdr, byte *Data, byte *Option);

byte Crc8CheckEx(byte *data, size_t offset, size_t count);

byte Crc8Check(byte *data, size_t count);

void EoParameter(int ac, char**av, EO_CONTROL *p);

void EoSetEep(EO_CONTROL *P, byte *Id, byte *Data);

void PrintTelegram(EO_PACKET_TYPE packetType, byte *id, byte erp2hdr, byte *data);

inline void PrintError() {}

inline void DataToEEP(byte *data, uint *pFunc, uint *pType, uint *pMan)
{
	uint func = ((uint)data[0]) >> 2;
	uint type = ((uint)data[0] & 0x03) << 5 | ((uint)data[1]) >> 3;
	int manID = (data[1] & 0x07) << 8 | data[2];
	if (pFunc) *pFunc = func;
	if (pType) *pType = type;
	if (pMan) *pMan = manID;
}
