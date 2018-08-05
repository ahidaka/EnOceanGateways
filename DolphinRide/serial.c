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
#define _DEBUG if (_GetPacketDebug > 0) 

//
void PacketDump(BYTE *p)
{
	printf("%02X %02X %02X %02X %02X %02X %02X %02X  ",
	       p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
	printf("%02X %02X %02X %02X %02X %02X %02X %02X ",
	       p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
	printf("%02X %02X %02X %02X %02X %02X %02X %02X\n",
	       p[16], p[17], p[18], p[19], p[20], p[21], p[22], p[23]);
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
typedef enum
{
	GET_SYNC,
	GET_HEADER,
	CHECK_CRC8H,
	GET_DATA,
	CHECK_CRC8D,
}
STATES_GET_PACKET;

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

	_DEBUG printf("*** GetPacket fd=%d\n", Fd);

	if (tickMSec == 0) {
		tickMSec = SystemMSec();
	}

	timeout = SystemMSec() - tickMSec;
	if (timeout > TIMEOUT_MSEC) {
		status = GET_SYNC;
		_DEBUG printf("*** Timeout=%d\n", timeout);
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
			printf("*** Read error=%d (%d) (%d)\n",
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
			_DEBUG printf("*** CHECK_CRC8H=%d\n", count);
			crc = Crc8Check(line, HEADER_BYTES);
			if (crc != rxByte) {
				printf("*** CRC8H ERROR %02X:%02X\n", crc, rxByte);
				printf("*** %02X %02X %02X %02X %02X\n",
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
			_DEBUG printf("*** CRC8H OK!\n");

			dataLength = (line[0] << 8) + line[1];
			optionLength = line[2];
			type = line[3];
			dataBuffer = &line[5];

		        if ((dataLength + optionLength) == 0) {
				_DEBUG printf("*** LENGTH ZERO!\n");
				if (rxByte == SYNC_CODE) {
					status = GET_HEADER;
					count = 0;
					break;
				}
				status = GET_SYNC;
				return OUT_OF_RANGE;
			}
			_DEBUG printf("*** datLen=%d optLen=%d type=%02X\n",
				      dataLength, optionLength, type);
			_DEBUG printf("*** %02X %02X %02X %02X\n",
				      line[0], line[1], line[2], line[3]);
			status = GET_DATA;
			count = 0;
			break;

		case GET_DATA:
			if (count < BufferLength) {
				dataBuffer[count] = rxByte;
			}
			else {
				printf("*** dataError=%d %d\n", count, BufferLength);
			}

			if (++count == (dataLength + optionLength)) {
				_DEBUG printf("*** lastData=%02X\n", rxByte);
				status = CHECK_CRC8D;
			}
			break;

		case CHECK_CRC8D:
			_DEBUG printf("*** CHECK_CRC8D=%d\n", count);
			status = GET_SYNC;
			
			if (count > BufferLength) {
				_DEBUG printf("*** return BUFFER_TOO_SMALL\n");
				return BUFFER_TOO_SMALL;
			}
			crc = Crc8Check(dataBuffer, count);
			if (crc == rxByte) {
				_DEBUG printf("*** return OK\n");
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
	_DEBUG printf("*** break while()\n");
	return (status == GET_SYNC) ? NO_RX_DATA : NEW_RX_DATA;
}
