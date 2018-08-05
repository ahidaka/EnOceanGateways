//
// serial.h
//
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned long ulong;

//
//
#include "esp3.h"

VOID PacketDump(byte *p);
ULONG SystemMSec(void);
RETURN_CODE GetPacket(INT Fd, BYTE *Packet, USHORT BufferLength);
