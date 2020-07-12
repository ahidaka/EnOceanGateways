//
// secure.h -- EnOcean security module header
//
#pragma once

#include "../dpride/typedefs.h"

//#include "eoif.h"

#define KEY_SIZE (128/8) //16
#define MAX_RLC_SIZE (4)
#define CMAC_SIZE (4)

typedef enum {
	NO_ENTRY = 0,
	FIRST_COME = 1,
	REGISTERED = 2
} PACKET_ENTRY;

typedef void *SEC_HANDLE;

INT SecInit(void);

SEC_HANDLE SecCreate(BYTE *Rlc, BYTE *Key, INT RlcLength);
void SecFree(SEC_HANDLE h);

INT SecUpdate(SEC_HANDLE h);
INT SecCheck(SEC_HANDLE h, BYTE *Rlc);
INT SecInspect(SEC_HANDLE h, BYTE *Packet);
INT SecGetRlc(SEC_HANDLE h, BYTE *Rlc);

INT SecEncrypt(SEC_HANDLE h, BYTE *Data, INT Length, BYTE *Cypher);
INT SecDecrypt(SEC_HANDLE h, BYTE *Packet, INT Length, BYTE *Data);

//
// Publickey supporting stuff
// Security supporting stuff
//
typedef struct _pulickey {
	UINT Id;
	UINT RlcLength;
	UINT Slf;
	BYTE Rlc[MAX_RLC_SIZE];
	BYTE Key[KEY_SIZE];
	CHAR *RlcPath;
	INT Option; // for advanced use to hide keys
	PACKET_ENTRY Status;
	INT Info;
	SEC_HANDLE Sec;
} PUBLICKEY, SECURE_REGISTER;

#define SECURE_REGISTER_SIZE (4)

void InitSecureRegister(void);
SECURE_REGISTER *NewSecureRegister(void);
SECURE_REGISTER *GetSecureRegister(UINT Id);
SECURE_REGISTER *ClearSecureRegister(UINT Id);

INT RlcLength(INT Slf);

#define PUBLICKEY_TABLE_SIZE (128)
extern PUBLICKEY *PublickeyTable;

VOID ReloadPublickey(char *PublickeyPath);
VOID DeletePublickey(char *PublickeyPath);

PUBLICKEY *AddPublickey(EO_CONTROL *p, UINT Id, SECURE_REGISTER *ps);
PUBLICKEY *GetPublickey(UINT Id);
PUBLICKEY *ClearPublickey(UINT Id);

void PrintKey(SECURE_REGISTER *ps);

#if SEC_DEVELOP	
VOID ReadRlc(PUBLICKEY *pt);
VOID WriteRlc(PUBLICKEY *pt);
PUBLICKEY *UpdateRlc(UINT Id, BYTE *Rlc);
#endif
