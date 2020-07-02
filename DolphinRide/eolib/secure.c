//
// secure.c -- EnOcean security module header
//
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "../dpride/typedefs.h"
#include "../dpride/dpride.h"
#include "secure.h"

#define SECURE_DEBUG 1

#ifdef SECURE_DEBUG
#define DEBUG_LOG(p) printf("+%s: %s\n", __func__, p);
#else
#define DEBUG_LOG(p)
#endif

SECURE_REGISTER SecureRegisterTable[SECURE_REGISTER_SIZE];

void InitSecureRegister(void)
{
	INT i;
	
	for(i = 0; i < SECURE_REGISTER_SIZE; i++) {
		SecureRegisterTable[i].Id = 0UL;
		SecureRegisterTable[i].Status = NO_ENTRY;
	}
}

SECURE_REGISTER *NewSecureRegister(void)
{
	INT i;
	
	for(i = 0; i < SECURE_REGISTER_SIZE; i++) {
		if (SecureRegisterTable[i].Id == 0UL) {
			return &SecureRegisterTable[i];
		}
	}
	return NULL;
}

SECURE_REGISTER *GetSecureRegister(UINT Id)
{
	INT i;
	
	for(i = 0; i < SECURE_REGISTER_SIZE; i++) {
		if (SecureRegisterTable[i].Id == Id) {
			return &SecureRegisterTable[i];
		}
	}
	return NULL;
}

SECURE_REGISTER *ClearSecureRegister(UINT Id)
{
	INT i;

	for(i = 0; i < SECURE_REGISTER_SIZE; i++) {
		if (SecureRegisterTable[i].Id == Id) {
			SecureRegisterTable[i].Id = 0UL;
			SecureRegisterTable[i].Status = NO_ENTRY;
			return &SecureRegisterTable[i];
		}
	}
	return NULL;
}

//
// Publickey
//
PUBLICKEY *PublickeyTable;
//static FILE *PublickeyFile;

typedef enum {
	Error = 0, Start = 1, GotId = 2, GotRlc = 3, GotKey = 4
} SCAN_STATUS;

//inline int ToHexDecimal(BYTE a)
//{
//	return(isdigit(a) ? a - '0' : toupper(a) - 'A' + 0xA);
//}

static INT ToHexDecimal(IN char *src, IN BYTE *dst)
{
	int i;
	int length = 0;
	ULONG ul;
	char work[4];
	work[2] = '\0';

#ifdef SECURE_DEBUG
	printf("+%s: Enter src=%s\n", __func__, src);
#endif
	for(i = 0; i < BUFSIZ / 4; i++) {
		work[0] = src[0];
		work[1] = src[1] ? src[1] : '0';
		src += 2;
		ul = strtoul((const char *) work, NULL, 16);
		*dst++ = (BYTE) ul & 0xFF;
		length++;
		if (*src == ' ') {
			src++;
		}
		else if (*src == '\0') {
			break;
		}
	}
	return length;
}

static SCAN_STATUS ScanLine(CHAR *Buffer, PUBLICKEY *pt)
{
	INT i = 0;
	SCAN_STATUS status = Start;
	CHAR *p;
	CHAR target[64];
	
	DEBUG_LOG("Enter");
#ifdef SECURE_DEBUG
	printf("<%s>\n", Buffer);
#endif
	p = Buffer;
	while(status != GotKey || status != Error) {
		switch (*p) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'a':
		case 'B':
		case 'b':
		case 'C':
		case 'c':
		case 'D':
		case 'd':
		case 'E':
		case 'e':
		case 'F':
		case 'f':
			target[i++] = *p;
			//status = Continued;
			break;
		case '\n':
		case '\r':
		case '\0':
		case '#':
		case ',':
			target[i] = '\0';

			switch(status) {
			case Start:
				pt->Id = strtoul(target, NULL, 16);
				status = GotId;
				i = 0;
				break;
			case GotId:
				ToHexDecimal(target, pt->Rlc);
				status = GotRlc;
				i = 0;
				break;
			case GotRlc:
				ToHexDecimal(target, pt->Key);
				status = GotKey;
#ifdef SECURE_DEBUG
				printf("status=%d char=%02X length=%ld\n",
				       status, p ? *p : -1, (long) (p - &Buffer[0]));
#endif
				i = 0;
				break;
			default:
				status = Error;
				break;
			}
			break;

		default:
			break;
		}
		if (status == GotKey) {
			break;
		}
		else if (p - &Buffer[0] > 60) {
			Error("ScanLine: Text is too long");
#ifdef SECURE_DEBUG
			printf("status=%d char=%02X\n", status, p ? *p : -1);
#endif
			status = Error;
			break;
		}
		p++;
	}
	return status;
}

VOID ReadRlc(PUBLICKEY *pt)
{
	const int srcLength = 8;
	INT length;
	CHAR buffer[12];
	FILE *f = fopen(pt->RlcPath, "r");
	
	DEBUG_LOG("Enter");
	if(f == NULL) {
		fprintf(stderr, "ReadRlc: Cannot read RlcFile=%s\n", pt->RlcPath);
		Error("ReadRlc");
		return;
	}
	buffer[srcLength] = '\0';
	fread(buffer, srcLength, 1, f);
	length = ToHexDecimal(buffer, pt->Rlc);
	if (length != RLC_SIZE) { //RLC_SIZE == 4
		Error("ReadRlc length!=4");
	}
	fclose(f);
}

VOID WriteRlc(PUBLICKEY *pt)
{
	FILE *f = fopen(pt->RlcPath, "w");
	
	DEBUG_LOG("Enter");
#ifdef SECURE_DEBUG
	printf("%02X%02X%02X%02X\r\n", pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);
#endif
	if(f == NULL) {
		fprintf(stderr, "WriteRlc: Cannot create RlcFile=%s\n", pt->RlcPath);
		Error("WriteRlc");
		return;
	}
	fprintf(f, "%02X%02X%02X%02X\r\n", pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);
	fclose(f);
}

VOID DeletePublickey(char *PublickeyPath)
{
	INT i;
	PUBLICKEY *pt;

	DEBUG_LOG("Enter");
	if (PublickeyTable != NULL) {
		for(i = 0; i < PUBLICKEY_TABLE_SIZE; i++) {
			pt = &PublickeyTable[i];
			pt->Id = 0UL;
		}
	}
	unlink(PublickeyPath);	
}

#ifndef NEED_MAIN
VOID ReloadPublickey(char *PublickeyPath)
{
	INT i;
	CHAR buffer[BUFSIZ / 8];
	SCAN_STATUS status;
	FILE *f;
	PUBLICKEY *pt;
	SEC_HANDLE sec;
	NODE_TABLE *nt;
	
	DEBUG_LOG("Enter");
	if (PublickeyTable == NULL) {
		PublickeyTable = calloc(PUBLICKEY_TABLE_SIZE, sizeof(PUBLICKEY));
		if (PublickeyTable == NULL) {
			Error("calloc PublickeyTable");
			return;
		}
	}
	// Read old keyfile
	f = fopen(PublickeyPath, "r");
	if (f != NULL) {
#ifdef SECURE_DEBUG
 		printf("Opened=%s\n", PublickeyPath);
#endif
		pt =  &PublickeyTable[0];
		for(i = 0; i < PUBLICKEY_TABLE_SIZE; i++) {
			if (fgets(buffer, sizeof(buffer), f) == NULL) {
				break;
			}
			status = ScanLine(buffer, pt);
			if (status != GotKey) {
				// Something error
				Error("Publickey ScanLine error");
				break; // continue ??
			}
			nt = GetTableId(pt->Id);
			if (nt != NULL) {
				sec = SecCreate(pt->Rlc, pt->Key);
				if (sec != NULL) {
#ifdef SECURE_DEBUG
					CHAR str[12];
					IdToString(nt->Id, str);
					printf("Linked=%s\n", str);
#endif
					nt->Secure = sec;
				}
				else {
					Error("SecCreate for GetTableId");
				}
			}
			else {
				Error("nt == NULL for ReloadPublickey");
			}
			pt++;
		}
		fclose(f);
	}
	else {
		Warn("No publickey file");
	}
}

static VOID WriteLine(FILE *f, PUBLICKEY *pt)
{
	int i;
	const int length = 8 + 1 + 8 + 1 + 32 + 4;  
	CHAR buffer[BUFSIZ / 8];
	CHAR *p = &buffer[0];
	
	DEBUG_LOG("Enter");
#ifdef SECURE_DEBUG
	printf("ID:%08X RLC=%02X %02X %02X %02X KEY=%02X %02X %02X %02X\n",
	       pt->Id, pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3],
	       pt->Key[0], pt->Key[1], pt->Key[2], pt->Key[3]);
#endif
	//Write ID
	IdToString(pt->Id, &buffer[0]);
	buffer[8] = ',';
	buffer[9] = '\0';
	
	//Write RLC
	sprintf(&buffer[9], "%02X%02X%02X%02X,",
		pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);

	//Write KEY
	p = &buffer[18];
	for(i = 0; i < 16; i++) {
		sprintf(p, "%02X%02X", pt->Key[i*2], pt->Key[i*2+1]);
		p += 4;
	}
	p = &buffer[50];
	*p++ = '\r';
	*p++ = '\n';
	*p++ = '\0';
	*p   = '\0';
	fwrite(buffer, length, 1, f);
}

VOID RewritePublickey(EO_CONTROL *p)
{
	PUBLICKEY *pt;
	FILE *f;
	INT i;

	DEBUG_LOG("Enter");
	f = fopen(p->PublickeyPath, "w");
	for(i = 0; i < PUBLICKEY_TABLE_SIZE; i++) {
		pt = &PublickeyTable[i];
		if (pt->Id != 0UL) {
			WriteLine(f, pt);
		}
		else {
			fclose(f);
			break;
		}
		pt++;
	}
}

PUBLICKEY *AddPublickey(EO_CONTROL *p, UINT Id, BYTE *Rlc, BYTE *Key)
{
	INT i;
	PUBLICKEY *pt = NULL;
	CHAR RlcFile[20];

	DEBUG_LOG("Enter");
#ifdef SECURE_DEBUG
	printf("RLC=%02X %02X %02X %02X KEY=%02X %02X %02X %02X\n",
	       Rlc[0], Rlc[1], Rlc[2], Rlc[3], Key[0], Key[1], Key[2], Key[3]);
#endif
	for(i = 0; i < PUBLICKEY_TABLE_SIZE; i++) {
#ifdef SECURE_DEBUG
		printf("++%d: \n", i);
#endif
		pt = &PublickeyTable[i];
		if (pt->Id == Id || pt->Id == 0UL) {
			pt->Id = Id;
			pt->Option = 0;
#if 0			
			strcpy(RlcFile, "RLC-");
			IdToString(Id, &RlcFile[4]);
			strcat(RlcFile, ".txt");	
			pt->RlcPath = MakePath(p->BridgeDirectory, RlcFile);
#endif
			memcpy(pt->Rlc, Rlc, RLC_SIZE);
			memcpy(pt->Key, Key, KEY_SIZE);
#ifdef SECURE_DEBUG
			printf("++%d-0:RLC=%02X%02X%02X%02X %02X%02X%02X%02X\n",
			       i, Rlc[0], Rlc[1], Rlc[2], Rlc[3],
			       pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);
#endif

#if 1			
			strcpy(RlcFile, "RLC-");
			printf("++%d-1:RLC=%02X %02X %02X %02X\n",
			       i, pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);
			IdToString(Id, &RlcFile[4]);
			printf("++%d-2:RLC=%02X %02X %02X %02X\n",
			       i, pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);

			strcat(RlcFile, ".txt");	

			printf("++%d-3:RLC=%02X %02X %02X %02X\n",
			       i, pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);
			pt->RlcPath = MakePath(p->BridgeDirectory, RlcFile);
#endif
#ifdef SECURE_DEBUG
			printf("++%d-4:RLC=%02X %02X %02X %02X\n",
			       i, pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3]);
#endif
			WriteRlc(pt);
			RewritePublickey(p);
			break;
		}
	}
	return pt;
}

PUBLICKEY *GetPublickey(UINT Id)
{
	INT i;
	PUBLICKEY *pt;
	
	DEBUG_LOG("Enter");
	for(i = 0; i < PUBLICKEY_TABLE_SIZE; i++) {
		pt = &PublickeyTable[i];
		if (pt->Id == Id) {
			return pt;
		}
	}
	return NULL;
}

PUBLICKEY *UpdateRlc(UINT Id,  BYTE *Rlc)
{
	PUBLICKEY *pt = GetPublickey(Id);

	DEBUG_LOG("Enter");
	if (pt != NULL) {
		memcpy(pt->Rlc, Rlc, RLC_SIZE);
		WriteRlc(pt);
	}
	else {
		Error("UpdateRlc notfound");
	}
	return pt;
}

PUBLICKEY *ClearPublickey(UINT Id)
{
	PUBLICKEY *pk = GetPublickey(Id);

	DEBUG_LOG("Enter");
	if (pk != NULL) {
		pk->Id = 0;
	}
	return pk;
}
#endif

//
//
//
#ifdef SUPERSONIC_SECURE
#include "eoif.c"
#else
INT SecInit(void){ return 0; }
SEC_HANDLE SecCreate(BYTE *Rlc, BYTE *Key) { return NULL; }
void SecFree(SEC_HANDLE h) {}

INT SecUpdate(SEC_HANDLE h) {return 0;}
INT SecCheck(SEC_HANDLE h, BYTE *Rlc) {return 0;}
INT SecInspect(SEC_HANDLE h, BYTE *Packet) {return 0;}

INT SecEncrypt(SEC_HANDLE h, BYTE *Data, INT Length, BYTE *Cypher) {return 0;}
INT SecDecrypt(SEC_HANDLE h, BYTE *Packet, INT Length, BYTE *Data) {return 0;}

INT SecGetRlc(SEC_HANDLE h, BYTE *Rlc)  {return 0;}
#endif

#ifdef NEED_MAIN
int main(int ac, char **av)
{
	PUBLICKEY pKey;
	PUBLICKEY *pt = &pKey;
	SCAN_STATUS s;

	pt->Id = 0UL;
	pt->Rlc[0] = pt->Rlc[1] = pt->Rlc[2] = pt->Rlc[3] = 0;
	pt->Key[0] = pt->Key[1] = pt->Key[2] = pt->Key[3] = 0;

	s = ScanLine(av[1], pt);

	printf("id=%08X rlc=%02X%02X%02X%02X key=%02X%02X%02X%02X\n", pt->Id,
	       pt->Rlc[0], pt->Rlc[1], pt->Rlc[2], pt->Rlc[3],
	       pt->Key[0], pt->Key[1], pt->Key[2], pt->Key[3]);

	printf("status=%d\n", s);
	
	return 0;
}
#endif
