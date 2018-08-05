#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termio.h>
#include <pthread.h>
#include <time.h>
#include "serial.h"
#include "esp3.h"

#define UNUSED_VARIABLE(x) (void)(x)
#define UNREFERENCED_PARAMETER(x) (void)(x)
#define IN     /* input parameter */
#define OUT    /* output parameter */
#define INOUT  /* input/output parameter */
#define IN_OPT     /* input parameter, optional */
#define OUT_OPT    /* output parameter, optional */
#define INOUT_OPT  /* input/output parameter, optional */

//
//#define UARTPORT "/dev/ttyS0"
#define UARTPORT "/dev/ttyUSB0"

//
//
//
#define msleep(a) usleep((a) * 1000)

#define DATABUFSIZ (256)
#define HEADER_SIZE (5)
#define CRC8D_SIZE (1)

#define RESPONSE_TIMEOUT (60)

#define TRUE (1)
#define FALSE (0)
typedef void VOID;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef int INT;
typedef unsigned int UINT;
typedef long LONG;
typedef unsigned long ULONG;

//
// CO Command / Response
//
ESP_STATUS CO_WriteSleep(IN INT Period) //sleep msec
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	BYTE msBuffer[4];
	int ms10 = Period / 10;

	printf("%s: ENTER\n", __func__);
	msBuffer[3] = ms10 % 0xFF;
	ms10 >>= 8;
	msBuffer[2] = ms10 % 0xFF;
	ms10 >>= 8;
	msBuffer[1] = ms10 % 0xFF;
	ms10 >>= 8;
	msBuffer[0] = 0;

	SetCommand(CO_WR_SLEEP, buffer, msBuffer);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CO_WriteReset(VOID) //no params
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];

	printf("%s: ENTER\n", __func__);
	SetCommand(CO_WR_RESET, buffer, 0);
	SendCommand(buffer);
	msleep(1);
	//PacketDump(buffer);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CO_ReadVersion(OUT BYTE *VersionStr)
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	BYTE *p = &buffer[6];

	printf("%s: ENTER\n", __func__);
	SetCommand(CO_RD_VERSION, buffer, 0);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	if (status == OK) {
		printf("ver=%d.%d.%d.%d %d.%d.%d.%d id=%02X%02X%02X%02X\n",
		       p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],
		       p[8],p[9],p[10],p[11]);
		p[32] = '\0';
		printf("chip=%02X%02X%02X%02X App=<%s>\n",
		       p[12],p[13],p[14],p[15],&p[16]);
	}
	return status;
}

ESP_STATUS CO_WriteFilterAdd(IN BYTE *Id) //add filter id
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	BYTE param[6];

	printf("%s: ENTER add=%02X%02X%02X%02X\n", __func__, Id[0], Id[1], Id[2], Id[3]);
	if (Id != NULL) {
		param[0] = 0; // Device source ID
		param[1] = Id[0];
		param[2] = Id[1];
		param[3] = Id[2];
		param[4] = Id[3];
		param[5] = 0x80; // Apply radio
		SetCommand(CO_WR_FILTER_ADD, buffer, param);
		SendCommand(buffer);
		msleep(1);
		status = GetResponse(buffer);
	}
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CO_WriteFilterDel(IN BYTE *Id) //delete filter id
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];

	printf("%s: ENTER del=%02X%02X%02X%02X\n", __func__, Id[0], Id[1], Id[2], Id[3]);
	SetCommand(CO_WR_FILTER_DEL, buffer, Id);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CO_WriteFilterDelAll(VOID) //no params
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];

	printf("%s: ENTER\n", __func__);
	SetCommand(CO_WR_FILTER_DEL_ALL, buffer, 0);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CO_WriteFilterEnable(IN BOOL On) //enable or disable
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	BYTE param[2];

	printf("%s: ENTER opt=%s\n", __func__, On ? "ON" : "OFF");
	param[0] = (BYTE) On;  //enable(1) or disable(0)
	param[1] = 0;  // OR condition
	SetCommand(CO_WR_FILTER_ENABLE, buffer, param);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CO_ReadFilter(OUT INT *count, OUT BYTE *Ids)
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	INT length;
	INT i;
	BYTE *p;
	
	printf("%s: ENTER\n", __func__);
	SetCommand(CO_RD_FILTER, buffer, 0);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);

	if (status == OK) {
		length = buffer[1];
		if (count != NULL) {
			*count = (length - 1) / 5;
		}
		if (Ids != NULL) {
			p = Ids;
			for(i = 0; i < *count; i++) {
				*p++ = buffer[i * 5 + 7]; 
				*p++ = buffer[i * 5 + 8]; 
				*p++ = buffer[i * 5 + 9]; 
				*p++ = buffer[i * 5 + 10]; 
				*p++ = '\0'; 
			}
		}
		printf("length=%d count=%d Id=%02X%02X%02X%02X\n",
		       buffer[1], *count, Ids[0], Ids[1], Ids[2], Ids[3]);
	}
	return status;
}

ESP_STATUS CO_WriteMode(IN INT Mode) //1:ERP1, 2:ERP2
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	BYTE bMode = Mode;

	printf("%s: ENTER\n", __func__);
	SetCommand(CO_WR_MODE, buffer, &bMode);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CFG_WriteESP3Mode(IN INT Mode) //1:ERP1, 2:ERP2
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	BYTE bMode = Mode;

	printf("%s: ENTER\n", __func__);
	SetCommand(CFG_WR_ESP3_MODE, buffer, &bMode);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d\n", __func__, status);
	return status;
}

ESP_STATUS CFG_ReadESP3Mode(OUT INT *Mode) //1:ERP1, 2:ERP2
{
	ESP_STATUS status = OK;
	BYTE buffer[DATABUFSIZ];
	byte *p = &buffer[6];

	printf("%s: ENTER\n", __func__);
	SetCommand(CFG_RD_ESP3_MODE, buffer, 0);
	SendCommand(buffer);
	msleep(1);
	status = GetResponse(buffer);
	printf("%s: status=%d mode=%d\n", __func__, status, *p);
	return status;
}

//
//
//
void SetCommand(ESP3_CMD Cmd, BYTE *Buffer, BYTE *Param)
{
	enum LENGTH { NOT_SUPPORTED = -1};
	int i;
	int length = 1;

	switch(Cmd) {
        case CO_WR_SLEEP: /*1 Order to enter in energy saving mode */
		length = 5;
		break;
        case CO_WR_RESET: /*2 Order to reset the device */
        case CO_RD_VERSION: /*3 Read the device (SW) version / (HW) version, chip ID etc. */
        case CO_RD_SYS_LOG: /*4 Read system log from device databank */
        case CO_WR_SYS_LOG: /*5 Reset System log from device databank */
        case CO_WR_BIST: /*6 Perform Flash BIST operation */
        case CO_WR_IDBASE: /*7 Write ID range base number */
		break;
        case CO_RD_IDBASE: /*8 Read ID range base number */
		length = NOT_SUPPORTED;
		break;
        case CO_WR_REPEATER: /*9 Write Repeater Level off,1,2 */
		length = 3;
		break;
        case CO_RD_REPEATER: /*10 Read Repeater Level off,1,2 */
		break;
        case CO_WR_FILTER_ADD: /*11 Add filter to filter list */
		length = 7;
		break;
        case CO_WR_FILTER_DEL: /*12 Delete filter from filter list */
		length = 6;
		break;
        case CO_WR_FILTER_DEL_ALL: /*13 Delete all filter */
		break;
        case CO_WR_FILTER_ENABLE: /*14 Enable/Disable supplied filters */
		length = 3;
		break;
        case CO_RD_FILTER: /*15 Read supplied filters */
		break;
        case CO_WR_WAIT_MATURITY: /*16 Waiting till end of maturity time before received radio telegrams will transmitted */
        case CO_WR_SUBTEL: /*17 Enable/Disable transmitting additional subtelegram info */
        case CO_WR_MEM: /*18 Write x bytes of the Flash, XRAM, RAM0 …. */
        case CO_RD_MEM: /*19 Read x bytes of the Flash, XRAM, RAM0 …. */
        case CO_RD_MEM_ADDRESS: /*20 Feedback about the used address and length of the config area and the Smart Ack Table */
        case CO_RD_SECURITY: /*21 Read own security information (level, key) */
        case CO_WR_SECURITY: /*22 Write own security information (level, key) */
        case CO_WR_LEARNMODE: /*23 Enable/disable learn mode */
        case CO_RD_LEARNMODE: /*24 Read learn mode */
        case CO_WR_SECUREDEVICE_ADD: /*25 Add a secure device */
        case CO_WR_SECUREDEVICE_DEL: /*26 Delete a secure device */
        case CO_RD_SECUREDEVICE_BY_INDEX: /*27 Read secure device by index */
		length = NOT_SUPPORTED;
		break;
        case CO_WR_MODE: /*28 Sets the gateway transceiver mode */
		length = 2;
		break;

	/* Special function */
        case CFG_WR_ESP3_MODE: /*28 Sets the gateway transceiver mode */
		length = 2;
        case CFG_RD_ESP3_MODE: /*28 Sets the gateway transceiver mode */
		length = 1;

	default:
		length = NOT_SUPPORTED;
		break;
	}

	if (length != NOT_SUPPORTED) {
		Buffer[0] = 0x55; // Sync Byte
		Buffer[1] = 0; // Data Length[0]
		Buffer[2] = length; // Data Length[1]
		Buffer[3] = 0; // Optional Length
		// check Config command
		Buffer[4] = Cmd >= CFG_WR_ESP3_MODE ? 11 : 5; // Packet Type = CO (5) or CFG(11)
		Buffer[5] = Crc8CheckEx(Buffer, 1, 4); // CRC8H
		Buffer[6] = Cmd; // Command Code
		if (Cmd >= CFG_WR_ESP3_MODE) {
			/* Set Config commang */
			Buffer[6] = Cmd - CFG_COMMAND_BASE; // Command Code
		}
		for(i = 0; i < (length - 1); i++)
			Buffer[7 + i] = Param[i];
		Buffer[6 + length] = Crc8CheckEx(Buffer, 6, length); // CRC8D
	}
}

//
// static class crc
//{
static BYTE Crc8Table[] =
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

BYTE Crc8CheckEx(BYTE *data, size_t offset, size_t count)
{
  int i;
  BYTE crc = 0;
  count += offset;
  for (i = offset; i < count; i++) {
    crc = Crc8Table[crc ^ data[i]];
  }
  return crc;
}

BYTE Crc8Check(BYTE *data, size_t count)
{
  return Crc8CheckEx(data, 0, count);
}
