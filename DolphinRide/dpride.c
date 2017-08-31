#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termio.h>

typedef unsigned char byte;

#define EO_ESP_PORT "/dev/ttyUSB0"
#define EO_DIRECTORY "/var/tmp/dpride"
#define EO_CONTROL_FILE "eofilter.txt"
#define EO_EEP_FILE "eep2.6.5.xml"
#define EO_FILTER_SIZE 64

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
        char *ControlFile;
        char *EEPFile;
        char *BridgeDirectory;
} EO_CONTOL;

typedef struct _eepdata
{
        int porg;
        int func;
        int type;
        int manID;
} EEP_DATA;

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

typedef struct _eo_serial
{
	int fd;
	int opened;
	int debug;
} EO_PORT;

static EO_CONTROL EoControl;

static EO_PORT EoPort {.debug = 1;};

static long EoFilterList[128];

void DebugPrint()
{
}

void USleep(int Usec)
{
	const int mega = (1000 * 1000);
	struct timespec t = {.tv_sec = 0;};
	int sec = Usec / mega;

	if (sec > 0) {
		t.tv_sec = sec;
	}
	t.tv_nsec = (Usec % mega) * 1000;
	nanosleep(&t, NULL);
}

int EoInitialize(char *FilterList)
//EoInitialize(filterFile, filterList, EO_FILTER_SIZE);
{
        count = EoReadFilter(file, list, 2);

        printf("count=%d\n", count);
}

int EoReadFilter(char *file, long *list, int size)
{
        FILE *fp;
        long id;
        int count = 0;
        char buffer[BUFSIZ];

        fp = fopen(file, "r");
        if (fp == NULL) {
                // open error
                return -1;
        }
        while(fgets(buffer, BUFSIZ, fp)) {
                if (buffer[0] == '#') // skip comment
                        continue;
                id = strtol(buffer, NULL, 16);
                if (id == 0)
                        continue;
                *list++ = id;
                count++;
                if (size != 0 && count >= size)
                        break;
        }
        fclose(fp);
        return (count);
}

int EoClearFilter(char *file, long *list, int size)
{
        FILE *fp;
        fp = fopen(file, "r+w");
        if (fp == NULL) {
                // open error
                return -1;
        }
        if (!ftruncate(fp, 0)) {
		fprintf(stderr, "%s: truncate error\n", __FUNCTION__);
        }
        fclose(fp);
        return (count);
}

bool EoReceiveFilter(long *FilterList, int FilterCount)
{
	int i;
	for(i = 0; i < FilterCount; i++)
	{
		;
	}
}

void EoBufferFilter(char *buffer, int id)
{
        buffer[0] = 0x55; // Sync Byte
        buffer[1] = 0; // Data Length[0]
        buffer[2] = 7; // Data Length[1]
        buffer[3] = 0; // Optional Length
        buffer[4] = 5; // Packet Type = CO (5)
        buffer[5] = Crc8CheckEx(buffer, 1, 4); // CRC8H
        buffer[6] = 11; // Command Code = CO_WR_FILTER_ADD (11)
        buffer[7] = 0;  // FilterType = Device ID (0)
        buffer[8] = (char)((id >> 24) & 0xFF); // ID[0]
        buffer[9] = (char)((id >> 16) & 0xFF); // ID[1]
        buffer[10] = (char)((id >> 8) & 0xFF); // ID[2]
        buffer[11] = (byte)(id & 0xFF); // ID[3]
        buffer[12] = 0x80; // Filter Kind = apply (0x80)
        buffer[13] = Crc8CheckEx(buffer, 6, 7); // CRC8D
}

void EoSetFilter(bool enable)
{
        bool clearFilter = true;
        bool writeFilter = enable;
        char writeBuffer[16];

        if (clearFilter)
        {
                //DebugPrint("Clear all Filters");
                writeBuffer[0] = 0x55; // Sync Byte
                writeBuffer[1] = 0; // Data Length[0]
                writeBuffer[2] = 1; // Data Length[1]
                writeBuffer[3] = 0; // Optional Length
                writeBuffer[4] = 5; // Packet Type = CO (5)
                writeBuffer[5] = Crc8CheckEx(writeBuffer, 1, 4); // CRC8H
                writeBuffer[6] = 13; // Command Code = CO_WR_FILTER_DEL (13)
                writeBuffer[7] = Crc8CheckEx(writeBuffer, 6, 1); // CRC8D
		EoPortWrite(writeBuffer, 0, 8);
		USleep(50*1000);
	}
	if (writeFilter && switchID != 0)
	{
		DebugPrint("SwitchID Add Filter");
		EoBufferFilter(writeBuffer, switchID);
		EoPortWrite(writeBuffer, 0, 14);
		USleep(50*1000);
	}

	if (writeFilter && co2ID != 0)
	{
		DebugPrint("Co2ID Add Filter");
		EoBufferFilter(writeBuffer, co2ID);
		EoPortWrite(writeBuffer, 0, 14);
		USleep(50*1000);
	}

	if (writeFilter && tempID != 0)
	{
		DebugPrint("TempID Add Filter");
		EoBufferFilter(writeBuffer, tempID);
		EoPortWrite(writeBuffer, 0, 14);
		USleep(50*1000);
	}

	if (writeFilter)
	{
		DebugPrint("Enable Filters");
		writeBuffer[0] = 0x55; // Sync Byte
		writeBuffer[1] = 0; // Data Length[0]
		writeBuffer[2] = 3; // Data Length[1]
		writeBuffer[3] = 0; // Optional Length
		writeBuffer[4] = 5; // Packet Type = CO (5)
		writeBuffer[5] = Crc8CheckEx(writeBuffer, 1, 4); // CRC8H
		writeBuffer[6] = 14; // Command Code = CO_WR_FILTER_ENABLE (14)
		writeBuffer[7] = 1;  // Filter Enable = ON (1)
		writeBuffer[8] = 0;  // Filter Operator = OR (0)
		//Writebuffer[8] = 1;  // Filter Operator = AND (1)
		writeBuffer[9] = Crc8CheckEx(writeBuffer, 6, 3); // CRC8D

		EoPort1Write(writeBuffer, 0, 10);
		USleep(50*1000);
	}
}

bool EoPortOpen()
{
	EO_PORT *p = &EoPort;

	if (p->opend) {
		fprintf(stderr, "%s: already opened\n", __FUNCTION__);
		return false;
	}

	if ((fd = open(EO_ESP_PORT, O_RDWR)) < 0) {
		fprintf(stderr, "%s: open error\n", __FUNCTION__);
		return false;
	}
	p->fd = fd;
	return true;
}

bool EoPortWrite(char * Buffer, int Offset, int Length)
{
	EoPortWrite(writeBuffer, 0, 14);
	EO_PORT *p = &EoPort;
	int length;

	if (!p->opend) {
		fprintf(stderr, "%s: fd not opened\n", __FUNCTION__);
		return false;
	}
	length = write(p->fd, &Buffer[Offset], Length);
	if (length != Length) {
		fprintf(stderr, "%s: write error\n", __FUNCTION__);
		return false;
	}
	return true;
}

bool EoGetHeader(EO_PACKET_TYPE *outPacketType, int *outDataLength, int *outOptionLength)
{
	int length;
	EO_PACKET_TYPE outPacketType;
	int dataLength;
	int optionLength;
	char c;
	byte crc8h;
	char header[4];
	EO_PORT *p = &EoPort;
	bool gotHeader;

	if (!p->opend) {
		fprintf(stderr, "%s: fd not opened\n", __FUNCTION__);
		return false;
	}

	// serch for preamble
	while(true) {
		length = read(p->fd, &c, 1);
		if (length == 1 && c == 0x55) {
			//printf("find preamble\n");
			break;
		}
	}

	length = read(p->fd, header, 4);
	if (length < 4) {
		fprintf(stderr, "%s: header error\n", __FUNCTION__);
		return false;
	}

	dataLength = header[0] << 8 | header[1];
	optionLength = header[2];
	packetType = (PacketType)header[3];

	// read crc8 byte
	length = read(p->fd, &crc8h, 1);
	if (length != 1) {
		fprintf(stderr, "%s: read crc8 error\n", __FUNCTION__);
		return false;
	}
	if (outPacketType)
		*outPacketType = outPacketType;
	if (outDataLength)
		*outDataLength = dataLength;
	if (outOptionLength)
		*outOptionLength = optionLength;

	return (gotHeader = crc8h == Crc8Checkcrc8(header, 4));
}

int EoGetBody(EO_PACKET_TYPE PacketType, int DataLength, int OptionLength,
	      char *Data, char *Option)
{
	int length;
	int dataOffset = 0;
	int nu = 0;
	char rorg;
	char crc8d;
	char readBuffer[48];
	char id[4];
	EO_PORT *p = &EoPort;

	if (!p->opend) {
		fprintf(stderr, "%s: fd not opened\n", __FUNCTION__);
		return false;
	}

	length = read(p->fd, readBuffer, DataLength);
	if (length != DataLength) {
		fprintf(stderr, "%s: read data error\n", __FUNCTION__);
		return false;
	}

	if (packetType == Radio || packetType == RadioErp2)
	{
		rorg = readBuffer[0];
		if (rorg == 0x62)
		{
                        dataOffset = 2;
		}
		id[3] = readBuffer[1 + dataOffset];
		id[2] = readBuffer[2 + dataOffset];
		id[1] = readBuffer[3 + dataOffset];
		id[0] = readBuffer[4 + dataOffset];

		if (rorg == 0x20) // RPS
		{
                        //dataSize = 1;
                        nu = (readBuffer[5 + dataOffset] >> 7) & 0x01;
                        data[0] = readBuffer[5 + dataOffset] & 0x0F;
                        data[1] = 0;
                        data[2] = 0;
                        data[3] = 0;
		}
		else if (rorg == 0x22) // 4BS
		{
                        //dataSize = 4;
                        data[0] = readBuffer[5 + dataOffset];
                        data[1] = readBuffer[6 + dataOffset];
                        data[2] = readBuffer[7 + dataOffset];
                        data[3] = readBuffer[8 + dataOffset];
		}
		else if (rorg == 0x62)  // Teach-In
		{
                        //dataSize = 4;
                        data[0] = readBuffer[5 + dataOffset];
                        data[1] = readBuffer[6 + dataOffset];
                        data[2] = readBuffer[7 + dataOffset];
                        data[3] = readBuffer[8 + dataOffset];
		}
		else
		{
                        fprintf(stderr, "%s: Unknown rorg = %02x",
				__FUNCTION__, rorg);
			return false;
		}
	}

	if (OptionLength > 0)
	{
		length = read(p->fd, readBuffer + DataLength, OptionLength);
		if (length != DataLength) {
			fprintf(stderr, "%s: read option error\n", __FUNCTION__);
			return false;
		}
	}

	//check crc8d
	length = read(p->fd, readBuffer + DataLength + OptionLength, 1);
	if (length != 1) {
		fprintf(stderr, "%s: read crc8d error\n", __FUNCTION__);
		return false;
	}
	
	crc8d = readBuffer[DataLength + OptionLength];
	if (crc8d != crc.crc8(readBuffer, DataLength + OptionLength))
	{
		fprintf(stderr, "%s: Invalid data crc", __FUNCTION__);
		return false;
	}

	////////
	// now return data, option ,id

	return true;
}

//
//

// static class crc
//{
static byte crc8Table[] =
{
        0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,
        0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
        0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
        0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
        0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5,
        0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
        0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85,
        0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
        0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
        0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
        0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2,
        0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
        0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32,
        0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
        0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
        0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
        0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c,
        0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
        0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec,
        0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
        0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
        0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
        0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c,
        0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
        0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b,
        0x76, 0x71, 0x78, 0x7f, 0x6A, 0x6d, 0x64, 0x63,
        0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
        0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
        0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb,
        0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8D, 0x84, 0x83,
        0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb,
        0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3
};

int crc8CheckEx(byte *data, size_t offset, size_t count)
{
        byte crc = 0;
        count += offset;
        for (int i = offset; i < count; i++) {
                crc = crc8Table[crc ^ data[i]];
        }
        return crc;
}

int crc8Check(byte *data, size_t count)
{
        return crc8CheckEx(data, 0, count);
}


void EoParameter(int ac, char**av, EO_CONTROL *p)
{
        int mFlags = 1; //default
        int rFlags = 0;
        int oFlags = 0;
        int cFlags = 0;
        int vFlags = 0;
        int opt;
        int i;
        int timeout = 0;
        char *controlFile;
        char *bridgeDirectory;
        char *eepFile;

        while ((opt = getopt(ac, av, "mrocvf:d:t:e:")) != EOF) {
                switch (opt) {
                case 'm':
                        mFlags++;
                        rFlags = oFlags = 0;
                        break;
                case 'r':
                        rFlags++;
                        mFlags = oFlags = 0;
                        break;
                case 'o':
                        oFlags++;
                        mFlags = rFlags = 0;
                        break;
                case 'c':
                        cFlags++;
                        break;
                case 'v':
                        vFlags++;
                        break;

                case 'f':
                        controlFile = optarg;
                        break;
                case 'd':
                        bridgeDirectory = optarg;
                        break;
                case 'e':
                        eepFile = optarg;
                        break;

                case 't': //timeout secs for register
                        timeout = atoi(optarg);
                        break;
                default: /* '?' */
                        fprintf(stderr,
                                "Usage: %s [-m|-r|-o][-c][-v]"
				"[-d Directory][-f Controlfile][-e EEPfile\n"
                                "    -m    Monitor mode\n"
                                "    -r    Register mode\n"
                                "    -o    Operation mode\n"
                                "    -c    Clear settings before register\n"
                                "    -v    View working status\n"
                                "    -d    Bridge file directrory\n"
                                "    -f    Control file\n"
                                ,av[0]);

                        exit(EXIT_FAILURE);
                }
        }
	p->Mode = oFlags ? Operation : rFlags ? Register : Monitor;
        p->CFlags = cFlags;
        p->VFlags = vFlags;
        p->Timeout = timeout;
        p->ControlFile = strdup(controlFile);
        p->EEPFile = strdup(eepFile);
        p->BridgeDirectory = strdup(bridgeDirectory);
}

//
//
//
int main(int ac, char **av)
{
	bool working;
	int currentId;
	int currentEep;
	long filterList[EO_FILTER_SIZE];
	EO_PACKET_TYPE packetType;
	EO_CONTROL *p = &EoControl;
	int filterCount;

	memset(filterList, 0, sizeof(long) * EO_FILTER_SIZE);

	EoParameter(ac, av, p);

	if (p->FilterFile == Read) {
		count = EoInitialize(p->FilterFile, filterList, EO_FILTER_SIZE);
	}
	else if (p->FilterFile == Clear) {
		EoClearFile(p->FilterFile);
	}

	if (!EoPortOpen()) {
		fprintf(stderr, "open error");
		exit(1);
	}
	working = true;

	if (filerCount > 0) {
		if (!EoReceiveFilter(filterList, count)) {
			fprintf(stderr, "open error");
			exit(1);
		}
	}

	while(working) {
		working = EoGetHeader();
		if (!working) {
			printerror();
			exit(1);
		}
		working = EoGetBody();
		if (!working) {
			printerror();
			exit(1);
		}

		working = EoAnalizePacket(&packetType);
		if (!working) {
			printerror();
			exit(1);
		}

		switch(packetType) {
		case TeachIn:
			EoSetEep(id);
			break;
		case Radio:
		case RadioErp2:

			switch(rorg) {
			case RORG_4BS:
				break;
			}

			break;
		case TeachIn:
			break;
		default:
			// ignore other type
			break;
		}

	}
	exit(0);
}
