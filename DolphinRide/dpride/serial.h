//
// serial.h
//
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned long ulong;

//
//
#include "esp3.h"

VOID PacketDebug(INT flag);
VOID PacketDump(BYTE *p);
ULONG SystemMSec(VOID);
RETURN_CODE GetPacket(INT Fd, BYTE *Packet, USHORT BufferLength);
