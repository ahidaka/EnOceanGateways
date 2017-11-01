#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <termio.h>

#include "dpride.h"

#define EO_ESP_PORT "/dev/ttyUSB0"
#define EO_DIRECTORY "/var/tmp/dpride"
#define EO_CONTROL_FILE "eofilter.txt"
#define EO_EEP_FILE "eep2.6.5.xml"
#define EO_FILTER_SIZE 128

//
static EO_CONTROL EoControl;

static EO_PORT EoPort;

static long EoFilterList[EO_FILTER_SIZE];

//
//
//
void DebugPrint(char *s)
{
	if (EoControl.Debug) {
		printf("*debug:%s\n", s);
	}
}

void USleep(int Usec)
{
	const int mega = (1000 * 1000);
	struct timespec t;
	t.tv_sec = 0;

	int sec = Usec / mega;

	if (sec > 0) {
		t.tv_sec = sec;
	}
	t.tv_nsec = (Usec % mega) * 1000;
	nanosleep(&t, NULL);
}

int EoReadFilter(char *File, long *List, int Size)
{
        FILE *fp;
        long id;
        int count = 0;
        byte buffer[BUFSIZ];

        fp = fopen(File, "r");
        if (fp == NULL) {
                // open error
                return -1;
        }
        while(fgets((char*)buffer, BUFSIZ, fp)) {
                if (buffer[0] == '#') // skip comment
                        continue;
                id = strtol((char*)buffer, NULL, 16);
                if (id == 0)
                        continue;
                *List++ = id;
                count++;
                if (Size != 0 && count >= Size)
                        break;
        }
        fclose(fp);
        return (count);
}

void EoClearFilter(char *File)
{
        if (truncate(File, 0)) {
		fprintf(stderr, "%s: truncate error\n", __FUNCTION__);
        }
}

bool EoReceiveFilter(long *FilterList, int FilterCount)
{
	int i;
	for(i = 0; i < FilterCount; i++)
	{
		;
	}
	return true;
}

void EoBufferFilter(char *buffer, int id)
{
        buffer[0] = 0x55; // Sync Byte
        buffer[1] = 0; // Data Length[0]
        buffer[2] = 7; // Data Length[1]
        buffer[3] = 0; // Optional Length
        buffer[4] = 5; // Packet Type = CO (5)
        buffer[5] = Crc8CheckEx((byte*)buffer, 1, 4); // CRC8H
        buffer[6] = 11; // Command Code = CO_WR_FILTER_ADD (11)
        buffer[7] = 0;  // FilterType = Device ID (0)
        buffer[8] = (char)((id >> 24) & 0xFF); // ID[0]
        buffer[9] = (char)((id >> 16) & 0xFF); // ID[1]
        buffer[10] = (char)((id >> 8) & 0xFF); // ID[2]
        buffer[11] = (byte)(id & 0xFF); // ID[3]
        buffer[12] = 0x80; // Filter Kind = apply (0x80)
        buffer[13] = Crc8CheckEx((byte*)buffer, 6, 7); // CRC8D
}

void EoSetFilter(bool enable)
{
        bool clearFilter = true;
        bool writeFilter = enable;
        byte writeBuffer[16];

        if (clearFilter)
        {
                //DebugPrint("Clear all Filters");
                writeBuffer[0] = 0x55; // Sync Byte
                writeBuffer[1] = 0; // Data Length[0]
                writeBuffer[2] = 1; // Data Length[1]
                writeBuffer[3] = 0; // Optional Length
                writeBuffer[4] = 5; // Packet Type = CO (5)
                writeBuffer[5] = Crc8CheckEx((byte*)writeBuffer, 1, 4); // CRC8H
                writeBuffer[6] = 13; // Command Code = CO_WR_FILTER_DEL (13)
                writeBuffer[7] = Crc8CheckEx((byte*)writeBuffer, 6, 1); // CRC8D
		EoPortWrite(writeBuffer, 0, 8);
		USleep(50*1000);
	}
#if 0
	if (writeFilter)
	{
		DebugPrint("SwitchID Add Filter");
		EoBufferFilter(writeBuffer, switchID);
		EoPortWrite(writeBuffer, 0, 14);
		USleep(50*1000);
	}
#endif
	if (writeFilter)
	{
		DebugPrint("Enable Filters");
		writeBuffer[0] = 0x55; // Sync Byte
		writeBuffer[1] = 0; // Data Length[0]
		writeBuffer[2] = 3; // Data Length[1]
		writeBuffer[3] = 0; // Optional Length
		writeBuffer[4] = 5; // Packet Type = CO (5)
		writeBuffer[5] = Crc8CheckEx((byte*)writeBuffer, 1, 4); // CRC8H
		writeBuffer[6] = 14; // Command Code = CO_WR_FILTER_ENABLE (14)
		writeBuffer[7] = 1;  // Filter Enable = ON (1)
		writeBuffer[8] = 0;  // Filter Operator = OR (0)
		//Writebuffer[8] = 1;  // Filter Operator = AND (1)
		writeBuffer[9] = Crc8CheckEx((byte*)writeBuffer, 6, 3); // CRC8D

		EoPortWrite(writeBuffer, 0, 10);
		USleep(50*1000);
	}
}

bool EoPortOpen()
{
	EO_PORT *p = &EoPort;
	int fd;
	struct termios tio;

	if (p->Opened) {
		fprintf(stderr, "%s: already opened\n", __FUNCTION__);
		return false;
	}

	if ((fd = open(EO_ESP_PORT, O_RDWR)) < 0) {
		fprintf(stderr, "%s: open error\n", __FUNCTION__);
		return false;
	}
	p->Fd = fd;
	p->Opened = true;

	bzero((void *) &tio, sizeof(tio));
	tio.c_cflag = B57600 | CRTSCTS | CS8 | CLOCAL | CREAD;
	tio.c_cc[VMIN] = 1;
	tcsetattr(fd, TCSANOW, &tio);

	return true;
}

bool EoPortWrite(byte * Buffer, int Offset, int Length)
{
	EoPortWrite(Buffer, 0, 14);
	EO_PORT *p = &EoPort;
	int length;

	if (!p->Opened) {
		fprintf(stderr, "%s: fd not opened\n", __FUNCTION__);
		return false;
	}
	length = write(p->Fd, &Buffer[Offset], Length);
	if (length != Length) {
		fprintf(stderr, "%s: write error\n", __FUNCTION__);
		return false;
	}
	return true;
}

bool EoGetHeader(EO_PACKET_TYPE *outPacketType, int *outDataLength, int *outOptionLength)
{
	int length;
	EO_PACKET_TYPE packetType;
	int dataLength;
	int optionLength;
	byte c;
	byte crc8h;
	byte header[4];
	int i;
	EO_PORT *p = &EoPort;
	//bool gotHeader;

	if (!p->Opened) {
		fprintf(stderr, "%s: fd not opened\n", __FUNCTION__);
		return false;
	}

	// serch for preamble
	while(true) {
		length = read(p->Fd, &c, 1);
		if (length == 1 && c == 0x55) {
			//printf("find preamble\n");
			break;
		}
	}

	for(i = 0; i < 4; i++) {
		length = read(p->Fd, &header[i], 1);
		if (length <= 0) {
			fprintf(stderr, "%s: header error=%d\n", __FUNCTION__, length);
			return false;
		}
	}
	dataLength = header[0] << 8 | header[1];
	optionLength = header[2];
	packetType = (EO_PACKET_TYPE)header[3];

	// read crc8 byte
	length = read(p->Fd, &crc8h, 1);
	if (length != 1) {
		fprintf(stderr, "%s: read crc8 error\n", __FUNCTION__);
		return false;
	}
	if (outPacketType)
		*outPacketType = packetType;
	if (outDataLength)
		*outDataLength = dataLength;
	if (outOptionLength)
		*outOptionLength = optionLength;

	return (crc8h == Crc8Check((byte*)header, 4));
}

int EoGetBody(EO_PACKET_TYPE PacketType, int DataLength, int OptionLength,
	      byte *Id, byte *Erp2Hdr, byte *Data, byte *Option)
{
	int length;
	int dataOffset = 0;
	int nu = 0;
	byte erp2hdr;
	byte crc8d;
	byte readBuffer[48];
	byte *id = &Id[0];
	byte *data = &Data[0];
	int i;
	EO_PORT *p = &EoPort;

	if (!p->Opened) {
		fprintf(stderr, "%s: fd not opened\n", __FUNCTION__);
		return false;
	}

	for(i = 0; i < DataLength; i++) {
		length = read(p->Fd, &readBuffer[i], 1);
		if (length <= 0) {
			fprintf(stderr, "%s: read data error=%d\n",
				__FUNCTION__, length);
			return false;
		}
	}

	if (PacketType == Radio || PacketType == RadioErp2)
	{
		erp2hdr = readBuffer[0];
		if (erp2hdr == 0x62)
		{
                        dataOffset = 2;
		}
		id[3] = readBuffer[1 + dataOffset];
		id[2] = readBuffer[2 + dataOffset];
		id[1] = readBuffer[3 + dataOffset];
		id[0] = readBuffer[4 + dataOffset];

		if (erp2hdr == 0x20) // RPS
		{
                        //dataSize = 1;
                        nu = (readBuffer[5 + dataOffset] >> 7) & 0x01;
                        data[0] = readBuffer[5 + dataOffset] & 0x0F;
                        data[1] = 0;
                        data[2] = 0;
                        data[3] = 0;
		}
		else if (erp2hdr == 0x22) // 4BS
		{
                        //dataSize = 4;
                        data[0] = readBuffer[5 + dataOffset];
                        data[1] = readBuffer[6 + dataOffset];
                        data[2] = readBuffer[7 + dataOffset];
                        data[3] = readBuffer[8 + dataOffset];
		}
		else if (erp2hdr == 0x62)  // Teach-In
		{
                        //dataSize = 4;
                        data[0] = readBuffer[5 + dataOffset];
                        data[1] = readBuffer[6 + dataOffset];
                        data[2] = readBuffer[7 + dataOffset];
                        data[3] = readBuffer[8 + dataOffset];
		}
		else
		{
                        fprintf(stderr, "%s: Unknown erp2hdr = %02x",
				__FUNCTION__, erp2hdr);
			return false;
		}
	}

	if (OptionLength > 0)
	{
		for(i = 0; i < OptionLength; i++) {
			length = read(p->Fd, readBuffer + DataLength + i, 1);
			if (length <= 0) {
				fprintf(stderr, "%s: read option error=%d\n",
					__FUNCTION__, length);
				return false;
			}
		}
	}

	//check crc8d
	length = read(p->Fd, readBuffer + DataLength + OptionLength, 1);
	if (length != 1) {
		fprintf(stderr, "%s: read crc8d error\n", __FUNCTION__);
		return false;
	}
	
	crc8d = (byte)readBuffer[DataLength + OptionLength];
	if (crc8d != Crc8Check((byte*)readBuffer, DataLength + OptionLength))
	{
		fprintf(stderr, "%s: Invalid data crc\n", __FUNCTION__);
		return false;
	}

	////////
	// now return data, option ,id
	do {
		char temp[BUFSIZ];
		sprintf(temp, "id=%02x%02x%02x%02x nu=%02x",
		       id[3],id[2],id[1],id[0],nu);
		DebugPrint(temp);
	}
	while(0);

	if (Erp2Hdr != NULL) {
		*Erp2Hdr = erp2hdr;
	}
	return true;
}

//
//

// static class crc
//{
static byte Crc8Table[] =
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

byte Crc8CheckEx(byte *data, size_t offset, size_t count)
{
	int i;
        byte crc = 0;
        count += offset;
        for (i = offset; i < count; i++) {
                crc = Crc8Table[crc ^ data[i]];
        }
        return crc;
}

byte Crc8Check(byte *data, size_t count)
{
        return Crc8CheckEx(data, 0, count);
}

void EoParameter(int ac, char**av, EO_CONTROL *p)
{
        int mFlags = 1; //default
        int rFlags = 0;
        int oFlags = 0;
        int cFlags = 0;
        int vFlags = 0;
        int opt;
        int timeout = 0;
        char *controlFile;
        char *bridgeDirectory;
        char *eepFile;

        while ((opt = getopt(ac, av, "mrocvf:d:t:e:")) != EOF) {
                switch (opt) {
                case 'm': //Monitor mode
                        mFlags++;
                        rFlags = oFlags = 0;
                        break;
                case 'r': //Register mode
                        rFlags++;
                        mFlags = oFlags = 0;
                        break;
                case 'o': //Operation mode
                        oFlags++;
                        mFlags = rFlags = 0;
                        break;
                case 'c': //clear before register
                        cFlags++;
                        break;
                case 'v': //Verbose
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


void PrintTelegram(EO_PACKET_TYPE packetType, byte *id, byte erp2hdr, byte *data)
{
	uint func, type, man, rorg;

	printf("%02x,%02x%02x%02x%02x,%02x,",
	       packetType, id[3], id[2], id[1], id[0], erp2hdr);

	switch(erp2hdr) {
	case 0x20: //RPS:
		rorg = 0xF6;
		printf("%02x,%02x", rorg, data[0]);
		break;
	case 0x21: //1BS:
		rorg = 0xD5;
		printf("%02x,%02x", rorg, data[0]);
		break;

	case 0x22: //4BS:
		rorg = 0xA5;
		printf("%02x,%02x %02x %02x %02x",
		       rorg, data[3], data[2], data[1], data[0]);
		break;

	case 0x24: //VLD:
		rorg = 0xD2;
		printf("%02x,%02x", rorg, data[0]);
		break;

	case 0x62: //4BS Teach-In:
		rorg = 0xA5;
		DataToEep(data, &func, &type, &man);
		printf("%02x,%02x-%02x-%02x %03x",
		       rorg, rorg, func, type, man);
		break;

	default:
		break;
	}
	printf("\n");
}

//
bool MainJob()
{
	EO_CONTROL *p = &EoControl;
	bool working;
	EO_PACKET_TYPE packetType;
	int dataLength;
	int optionLength;
	byte id[4];
	byte data[BUFSIZ];
	byte option[BUFSIZ];
	byte erp2hdr;

	working = EoGetHeader(&packetType, &dataLength, &optionLength);
	if (!working) {
                Error("!Working at EoGetHeader()");
		exit(1);
	}
	working = EoGetBody(packetType, dataLength, optionLength, id, &erp2hdr, data, option);
	if (!working) {
                Error("!Working at EoGetBody()");
		exit(1);
	}

	switch(packetType) {

	case Radio:
	case RadioErp2:

		if (p->Mode == Monitor) {
			PrintTelegram(packetType, id, erp2hdr, data);
		}

		switch(erp2hdr) {
		case 0x20: //RPS:
			break;

		case 0x22: //4BS:
			break;

		case 0x62: //4BS-Teach in:
			if (p->Mode == Register) {
                                if (p->VFlags) {
                                        PrintTelegram(packetType, id, erp2hdr, data);
                                }
				EoSetEep(p, id, data);
			}
			break;
		}
		break;

	default:
		// ignore other type
		break;
	}
	return working;
}

void EoSetEep(EO_CONTROL *P, byte *Id, byte *Data)
{
}

//
//
//
int main(int ac, char **av)
{
	bool working;
	EO_CONTROL *p = &EoControl;
	int filterCount = 0;

	memset(EoFilterList, 0, sizeof(long) * EO_FILTER_SIZE);

	p->Mode = Monitor; // Default mode
	p->Debug = true; // for Debug

	EoParameter(ac, av, p);

	switch(p->Mode) {
	case Monitor:
		p->FilterOp = Ignore;
		break;

	case Register:
		p->FilterOp = p->CFlags ? Clear : Ignore;
		break;

	case Operation:
		p->FilterOp = Read;
		break;
	}

	if (p->FilterOp == Read) {
		filterCount = EoReadFilter(p->FilterFile, EoFilterList, EO_FILTER_SIZE);
	}
	else if (p->FilterOp == Clear) {
		EoClearFilter(p->FilterFile);
	}

	if (!EoPortOpen()) {
		fprintf(stderr, "port open error\n");
		exit(1);
	}
	working = true;

	if (filterCount > 0) {
		if (!EoReceiveFilter(EoFilterList, filterCount)) {
			fprintf(stderr, "filter error\n");
			exit(1);
		}
	}

	while(working) {
		working = MainJob();
	}
	return 0;
}
