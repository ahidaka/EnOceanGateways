#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>

#include <errno.h>
#include "serial.h"

extern BYTE Crc8Check(BYTE *data, size_t count);

static INT _GetPacketDebug = 0;
#define _FULL_DUMP if (_GetPacketDebug > 1) // -pp: _PD_FULLDATA
#define _DEBUG if (_GetPacketDebug > 2)     // -ppp: _PD_VERBOSE
#define _DEBUG2 if (_GetPacketDebug > 3)    // -pppp: _PD_NOISY
#define _DEBUG3 if (_GetPacketDebug > 4)    // -ppppp: _PD_SUPER_NOISY
#define _DEBUG0 if (0) //_GetPacketDebug > 2)    // //NOT WORKED

VOID DumpIt(BYTE *Start, BYTE *p)
{
	INT i;

	printf("DumpIt: offset=%u\n", p - Start);
	for(i = 0; i < 10; i++) {
		printf("%02X ", p[i]);
	}
	printf("\n");
}

//
// _GetPacketDebug
//
// 1: Only ans simple Packet dump
// 2: Full Packet dump
// 3: Verbose messages
// 4: Noisy messages
//
VOID PacketDebug(INT flag)
{
	_GetPacketDebug = flag;
}

INT _PacketAnalyze(BYTE *p, INT DumpOption, INT ConvertOption);

VOID PacketDump(BYTE *p)
{
	INT dumpOption = _GetPacketDebug;
	_DEBUG3 printf("*** PacketDump=%02X\n", _GetPacketDebug);

	if (dumpOption == 0)
		dumpOption = 1;
	(VOID) _PacketAnalyze(p, dumpOption, 0);
}

INT PacketAnalyze(BYTE *p)
{
	return _PacketAnalyze(p, _GetPacketDebug, 1);
}

INT _PacketAnalyze(BYTE *Packet, INT DumpOption, INT ConvertOption)
{
	const INT headerLength = 5;
	const INT erp1IdLength = 4;
	const INT idSize = 8;
	const INT erp1AddrOffset = -5;
	INT dataLength = Packet[0] << 8 | Packet[1];
	INT optionalLength = Packet[2];
	INT printLength = headerLength + dataLength + optionalLength;
	INT packetType = 0;
	INT telegramType = 0;
	INT extendedHeader = 0;
	INT extendedTelegramType = 0;
	INT addressControl = 0;
	INT originatorLength;
	INT destinationLength = 0;
	INT idIndex;
	INT rORG = 0;
	INT i;
	BOOL isERP2 = FALSE;
	BOOL extendedHeaderAvailable = FALSE;
	BOOL extendedTelegramTypeAvailable = FALSE;
	//BOOL converToERP2 = FALSE;
	BYTE *data = &Packet[headerLength];
	BYTE id[idSize];
	BYTE destId[idSize];
	const BYTE ExtendedTelegramTypes[8] = {
		0xC5, // 0:SYS_EX
		0xC6, // 1:SmackReq
		0xC7, // 2:SmackAns
		0x40, // 3:Chained Data
		0x32, // 4:Secure
		0xB0, // 5:GP_TI
		0xB1, // 6:GP_TI_Resp
		0xB2, // 7:GP_CD 
	};

	_DEBUG3 printf("*** _PacketAnalyze=%02X\n", _GetPacketDebug);

	for(i = 0; i < idSize; i++) {
		id[i] = 0;
	}

	if ((_GetPacketDebug < _PD_FULLDATA)) {
		if (printLength > 16)
			printLength = 16;
	}

	packetType = Packet[3];
	if (isERP2 = packetType == 0x0A) {
		telegramType = *data++;
		addressControl = telegramType >> 5;
		switch(addressControl) {
		case 0:
			originatorLength = 3;
			break;
		case 1:
			originatorLength = 4;
			break;
		case 2:
			originatorLength = 4;
			destinationLength = 4;
			break;
		case 3:
			originatorLength = 6;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		default:
			originatorLength = 0; // means error
			break;
		}

		if (extendedHeaderAvailable = (telegramType >> 4) & 0x01) {
			extendedHeader = *data++;
		}

		switch(telegramType & 0xF) {
		case 0:
			rORG = 0xF6; // RPS
			break;
		case 1:
			rORG = 0xD5; //1BS
			break;
		case 2:
			rORG = 0xA5; //4BS
			break;
		case 3:
			rORG = 0xD0; //Smack
			break;
		case 4:
			rORG = 0xD2; //VLD
			break;
		case 5:
			rORG = 0xD4; //UTE
			break;
		case 6:
			rORG = 0xD1; //MSC
			break;
		case 7:
			rORG = 0x30; //SEC
			break;
		case 8:
			rORG = 0x31; //SECD
			break;
		case 9:
			rORG = 0x35; //SEC_TI
			break;
		case 0xA:
			rORG = 0xB3; //GP_SEL
			break;
		case 0xB:
			rORG = 0xA8; //ACK
			break;
		case 0xF:
			extendedTelegramTypeAvailable = TRUE;
			extendedTelegramType = *data++;
			rORG = ((extendedTelegramType & 0xF8) != 0x00) // has original type ?
				? extendedTelegramType
				: ExtendedTelegramTypes[extendedTelegramType & 0x7];
			break;
		case 0xC:
		case 0xD:
		case 0xE:
		default:
			rORG = 0x0; //Unknown
			break;
		}

		// originatorLength is 3, 4, 6,... convet them to 32bit ID
		for(i = 0; i < erp1IdLength; i++) {
			idIndex = originatorLength - (erp1IdLength - i);
			id[i] = idIndex < 0 ? 0 : data[idIndex];
		}
		data += originatorLength;
		*((UINT *)&destId[0]) = 0UL;
		for (i = 0; i < destinationLength; i++) {
			destId[i] = data[i];
		} 
		data += destinationLength;
	}
	else { //ERP1
		rORG = data[0];
		for(i = 0; i < erp1IdLength; i ++) {
			id[i] = data[dataLength + erp1AddrOffset + i];
		}
		data++;
	}
	_DEBUG2 printf("** Analyze:INPUT is ERP%s\n", isERP2 ? "2" : "1");

	if (DumpOption > 0 || _GetPacketDebug > 3) {
		BYTE *p;
		BOOL completed = FALSE;
		// Print raw packet data
		// Header and basic information
		printf("[%02X%02X%02X%02X:%d:%d:%02X] %02X %02X %02X %02X %02X|",
			id[0], id[1], id[2], id[3], dataLength, optionalLength, rORG,
			Packet[0], Packet[1], Packet[2], Packet[3], Packet[4]);

		p = Packet + headerLength;
		for(i = 0; i < dataLength; i++) {
			if (i + headerLength >= printLength) {
				completed = TRUE;
				break;
			}
			printf("%02X ", *p++);
		}
		if (!completed) {
			printf("|");
			for(i = 0; i < optionalLength; i++) {
				if (i + headerLength + dataLength >= printLength)
					break;
				printf("%02X ", *p++);
			}
		}
		printf("\n");

		if (isERP2) {
			printf("ERP2 Extra: TTyp:%02X rOrg:%02X sLen:%d dLen:%d ",
				telegramType, rORG, originatorLength, destinationLength);
			if (extendedHeaderAvailable) {
				printf("exHD:%02X ", extendedHeader);
			}
			if (extendedTelegramTypeAvailable) {
				printf("exTT:%02X ", extendedTelegramType);
			}
			if (destinationLength > 0) {
				printf("dID:%02X%02X%02X%02X ",
					destId[0], destId[1], destId[2], destId[3]);
			}
			printf("\n");
		}
	}

	if (isERP2 && ConvertOption) {
		// Have to convert to ERP1
		BYTE converted[528];
		BYTE subTel = Packet[headerLength + dataLength];
		BYTE dBm = Packet[headerLength + dataLength + 1];
		BYTE secLevel = optionalLength > 2 ? Packet[headerLength + dataLength + 2] : 0;

		dataLength -= (extendedHeaderAvailable + extendedTelegramTypeAvailable
						+ originatorLength + destinationLength );
		dataLength += erp1IdLength;
		optionalLength += (erp1IdLength + 1);  //ToID_ERP1(4)+Subtel(1)

		_DEBUG printf("ERP2Convert: rOrg=[%02X %02X]->%02X new dLen=%d oLen=%d dBm=%02X sec=%02X\n",
			telegramType, extendedTelegramType, rORG, dataLength, optionalLength, dBm, secLevel);

		_DEBUG0 DumpIt(Packet, data);	

		// New ERP1 Header
		converted[0] = dataLength >> 8;
		converted[1] = dataLength & 0xFF;
		converted[2] = optionalLength;
		converted[3] = 0x01; //RADIO_ERP1
		converted[4] = 0x00; //CRC8H

		// Data
		converted[headerLength] = rORG;
		for (i = 0; i < (dataLength - 1 - erp1IdLength); i++) {
			converted[headerLength + 1 + i] = *data++;
		}
		// Source address
		for (i = 0; i < erp1IdLength; i++) {
			converted[headerLength + dataLength - 1 - erp1IdLength + i] = id[i];
		}
		// Status byte
		converted[headerLength + dataLength - 1] = 0x80; ////*data++;

		// Optional Data
		converted[headerLength + dataLength] = subTel;
		for(i = 0; i < erp1IdLength; i++) { // Destination ID
			converted[headerLength + dataLength + 1 + i] = 0xFF;
		}
		////
		converted[headerLength + dataLength + 1 + erp1IdLength] = dBm;
		converted[headerLength + dataLength + 1 + erp1IdLength + 1] = secLevel;

		for(i = 0; i < headerLength + dataLength + optionalLength; i++) {
			Packet[i] = converted[i];
		}
	}
	return(isERP2);
}

//
//
ULONG SystemMSec(void)
{
	unsigned long now_msec;
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	now_msec = now.tv_nsec / 1000000;
	now_msec += now.tv_sec * 1000;

	return now_msec;
}

//
//
//
RETURN_CODE GetPacket(INT Fd, BYTE *Packet, USHORT BufferLength)
{
	typedef enum {
		GET_SYNC,
		GET_HEADER,
		CHECK_CRC8H,
		GET_DATA,
		CHECK_CRC8D,
	} STATES_GET_PACKET;

#define TIMEOUT_MSEC (10)
#define SYNC_CODE (0x55)
#define HEADER_BYTES (4)

	static BYTE crc = 0;
	static USHORT count = 0;
	static STATES_GET_PACKET status = GET_SYNC;
	static ULONG tickMSec = 0;
	BYTE *line = (BYTE*)Packet;
	BYTE rxByte;
	INT  timeout;
	BYTE i;
	INT  a;
	USHORT  dataLength;
	BYTE   optionLength;
	BYTE   type;
	BYTE   *dataBuffer;
	INT    readLength;

	_DEBUG3 printf("*** GetPacket fd=%d\n", Fd);

	if (tickMSec == 0) {
		tickMSec = SystemMSec();
	}

	timeout = SystemMSec() - tickMSec;
	if (timeout > TIMEOUT_MSEC) {
		status = GET_SYNC;
		_DEBUG printf("*** GetPacket:Timeout=%d\n", timeout);
	}

	while(TRUE) {
		readLength = read(Fd, &rxByte, 1);
		if (readLength == 0) {
			usleep(1000 * 1000 / 5760);
			continue;
		}
		else if (readLength == 1) {
			//OK
		}
		else {
			fprintf(stderr, "*** GetPacket:Read error=%d (%d) (%d)\n",
			       readLength, errno, Fd);
			perror("*** ERRNO");
			continue;
		}
		tickMSec = SystemMSec();

		switch(status) {
		case GET_SYNC:
			if (rxByte == SYNC_CODE) {
				status = GET_HEADER;
				count = 0;
			}
			break;

		case GET_HEADER:
			line[count++] = rxByte;
			if (count == HEADER_BYTES) {
				status = CHECK_CRC8H;
			}
			break;

		case CHECK_CRC8H:
			_DEBUG0 printf("*** GetPacket:CHECK_CRC8H=%d\n", count);
			crc = Crc8Check(line, HEADER_BYTES);
			if (crc != rxByte) {
				fprintf(stderr, "*** GetPacket:CRC8H ERROR %02X:%02X\n", crc, rxByte);
				fprintf(stderr, "*** GetPacket:%02X %02X %02X %02X %02X\n",
				       line[0], line[1], line[2], line[3], line[4]);

				a = -1;
				for (i = 0 ; i < HEADER_BYTES ; i++) {
					if (line[i] == SYNC_CODE) {
						a = i + 1;
						break;
					};
				}

				if (a == -1) {
					status = rxByte == SYNC_CODE ?
						GET_HEADER : GET_SYNC;
					break;
				}
				
				for (i = 0 ; i < (HEADER_BYTES - a) ; i++) {
					line[i] = line[a + i];
				}
				count = HEADER_BYTES - a;
				line[count++] = rxByte;
				if (count < HEADER_BYTES) {
					status = GET_HEADER;
					break;
				}
				break;
			}
			_DEBUG0 printf("*** GetPacket: CRC8H OK!\n");

			dataLength = (line[0] << 8) + line[1];
			optionLength = line[2];
			type = line[3];
			dataBuffer = &line[5];

			if ((dataLength + optionLength) == 0) {
				_DEBUG printf("*** GetPacket: LENGTH ZERO!\n");
				if (rxByte == SYNC_CODE) {
					status = GET_HEADER;
					count = 0;
					break;
				}
				status = GET_SYNC;
				return OUT_OF_RANGE;
			}
			_DEBUG0 printf("*** GetPacket: datLen=%d optLen=%d type=%02X\n",
				dataLength, optionLength, type);
			_DEBUG0 printf("*** GetPacket: %02X %02X %02X %02X\n",
				line[0], line[1], line[2], line[3]);
			status = GET_DATA;
			count = 0;
			break;

		case GET_DATA:
			if (count < BufferLength) {
				dataBuffer[count] = rxByte;
			}
			else {
				fprintf(stderr, "*** dataError=%d %d\n", count, BufferLength);
			}

			if (++count == (dataLength + optionLength)) {
				_DEBUG3 printf("*** GetPacket: lastData=%02X\n", rxByte);
				status = CHECK_CRC8D;
			}
			break;

		case CHECK_CRC8D:
			_DEBUG0 printf("*** GetPacket: CHECK_CRC8D=%d\n", count);
			status = GET_SYNC;
			
			if (count > BufferLength) {
				_DEBUG printf("*** GetPacket: return BUFFER_TOO_SMALL\n");
				return BUFFER_TOO_SMALL;
			}
			crc = Crc8Check(dataBuffer, count);
			if (crc == rxByte) {
				_DEBUG0 printf("*** GetPacket: return OK(%02X) xlen=%d\n", crc, count);
				return OK; // Correct packet received
			}
			else if (rxByte == SYNC_CODE) {
				status = GET_HEADER;
				count = 0;
			}
			return INVALID_CRC;

		default:
			status = GET_SYNC;
			break;
		}
		//printf("*** break switch()\n");
	}
	_DEBUG3 printf("*** GetPacket: break while return status=%d\n", status);
	return (status == GET_SYNC) ? NO_RX_DATA : NEW_RX_DATA;
}
