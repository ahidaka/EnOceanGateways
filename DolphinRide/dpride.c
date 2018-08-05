#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> //offsefof
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <termio.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h> //PATH_MAX

#include "dpride.h"
#include "ptable.h"
#include "queue.h"
#include "serial.h"
#include "esp3.h"

static const char copyright[] = "\n(c) 2017 Device Drivers, Ltd.\n";
static const char version[] = "\n@ dpride Version 1.00\n";

#define EO_ESP_PORT_USB "/dev/ttyUSB0"
#define EO_ESP_PORT_S0 "/dev/ttyS0"
#define EO_ESP_PORT_AMA0 "/dev/ttyAMA0"
#define EO_DIRECTORY "/var/tmp/dpride"
#define EO_CONTROL_FILE "eofilter.txt"
#define EO_COMMAND_FILE "eoparam.txt"
#define EO_EEP_FILE "eep.xml"
#define EO_FILTER_SIZE (128)

#define BROKER_FILE "brokers.txt"
#define MAX_BROKER (16)
#define PID_FILE "dpride.pid"
#define SIG_BROKERS (SIGRTMIN + 6)

//
#define msleep(a) usleep((a) * 1000)

#define DATABUFSIZ (256)
#define HEADER_SIZE (5)
#define CRC8D_SIZE (1)

#define RESPONSE_TIMEOUT (90)
//
//
//
#define msleep(a) usleep((a) * 1000)

#define DATABUFSIZ (256)
#define HEADER_SIZE (5)
#define CRC8D_SIZE (1)
//
//
//

#if defined(STAILQ_HEAD)
#undef STAILQ_HEAD

#define STAILQ_HEAD(name, type)                                 \
        struct name {                                                           \
        struct type *stqh_first;        /* first element */                     \
        struct type **stqh_last;        /* addr of last next element */         \
        int num_control;       \
        pthread_mutex_t lock; \
        }
#endif

#define INCREMENT(a) ((a) = (((a)+1) & 0x7FFFFFFF))

//typedef struct QueueHead {...} QEntry;
STAILQ_HEAD(QueueHead, QEntry);
typedef struct QueueHead QUEUE_HEAD;

struct QEntry {
        int Number;
        struct timespec TimeStamp;
        BYTE Data[DATABUFSIZ];
        STAILQ_ENTRY(QEntry) Entries;
};
typedef struct QEntry QUEUE_ENTRY;

QUEUE_HEAD DataQueue;     // Received Data
QUEUE_HEAD ResponseQueue; // CO Command Responce
QUEUE_HEAD ExtraQueue;    // Event, Smack, etc currently not used

//
//
//
BOOL stop_read;
BOOL stop_action;
BOOL stop_job;
BOOL read_ready;

typedef struct _THREAD_BRIDGE {
        pthread_t ThRead;
        pthread_t ThAction;
}
THREAD_BRIDGE;

////
// Command -- interface to Web API, other module, or command line
//

typedef enum {
        CMD_NOOP = 0,
        CMD_SHUTDOWN=1,
        CMD_REBOOT=2,
        CMD_FILTER_ADD=3,
        CMD_FILTER_CLEAR=4,
        CMD_CHANGE_MODE=5, //Monitor, Register, Operation
        CMD_CHANGE_OPTION=6, //Silent, Verbose, Debug,
}
JOB_COMMAND;

typedef struct _CMD_PARAM
{
        int Num;
        char Data[BUFSIZ];
}
CMD_PARAM;

void SetFd(int fd);
int GetFd(void);
static int _Fd;
void SetFd(int fd) { _Fd = fd; }
int GetFd(void) { return _Fd; }

void SetThdata(THREAD_BRIDGE Tb);
THREAD_BRIDGE *GetThdata(void);
static THREAD_BRIDGE _Tb;
void SetThdata(THREAD_BRIDGE Tb) { _Tb = Tb; }
THREAD_BRIDGE *GetThdata(void) { return &_Tb; }

//
static EO_CONTROL EoControl;
static long EoFilterList[EO_FILTER_SIZE];

//
//
//
//
//
JOB_COMMAND GetCommand(CMD_PARAM *Param);

//
static const char *CmdName[] = {
	/*0*/ "CMD_NOOP",
	/*1*/ "CMD_SHUTDOWN",
	/*2*/ "CMD_REBOOT",
	/*3*/ "CMD_FILTER_ADD",
	/*4*/ "CMD_FILTER_CLEAR",
	/*5*/ "CMD_CHANGE_MODE", //Monitor, Register, Operation
	/*6*/ "CMD_CHANGE_OPTION", //Silent, Verbose, Debug,
	/*7*/ NULL
};

static inline char *CommandName(int index)
{
	return (char *) CmdName[index & 0x7];
}

JOB_COMMAND GetCommand(CMD_PARAM *Param)
{
	EO_CONTROL *p = &EoControl;
	int mode = 0;
	char param[BUFSIZ];
	const char *OptionName[] = {
		"Auto",
		"Break",
		"Clear",
		"Debug",
		"Execute",
		"File",
		"Go",
		"Help",
		"Info",
		"Join",
		"Kill",
		"List",
		"Monitor",
		"Nutral",
		"Operation",
		"Print",
		"Quit",
		"Register",
		"Silent",
		"Test",
		"Unseen",
		"Verbose",
		"Wrong",
		"X\'mas",
		"Yeld",
		"Zap",
		NULL, NULL, NULL, NULL, NULL
	};

	if (p->CommandPath == NULL) {
		p->CommandPath = MakePath(p->BridgeDirectory, p->CommandFile); 
	}
	if (p->Debug > 0)
		printf("**%s: path=%s\n", __FUNCTION__, p->CommandPath);
	if (!ReadCmd(p->CommandPath, &mode, param)) {
		return CMD_NOOP;
	}
        if (Param != NULL) {
                Param->Num = mode;
		if (Param->Data)
			strcpy(Param->Data, param);
	}
	if (p->Debug > 0)
		printf("**%s: cmd:%d:%s opt:%s\n", __FUNCTION__,
		       mode, CommandName(mode), isdigit(*param) ? param : OptionName[(*param - 'A') & 0x1F]);
        return (JOB_COMMAND) mode;
}

void StartUp(void);
void SendCommand(BYTE *cmdBuffer);
bool MainJob(BYTE *Buffer);

//
//
//
int Enqueue(QUEUE_HEAD *Queue, BYTE *Buffer)
{
        QUEUE_ENTRY *newEntry = NULL;

        //printf("**Enqueue:%p\n", Buffer);
        newEntry = (struct QEntry *) malloc(sizeof(struct QEntry));
        if (newEntry == NULL) {
                printf("**ERROR at Enqueue\n");
                return 0;
        }
        pthread_mutex_lock(&Queue->lock);
        newEntry->Number = INCREMENT(Queue->num_control);
        memcpy(newEntry->Data, Buffer, DATABUFSIZ);

        STAILQ_INSERT_TAIL(Queue, newEntry, Entries);
        pthread_mutex_unlock(&Queue->lock);
        //printf("**Enqueue:%p=%d (%p)\n", newEntry->Data, newEntry->Number, newEntry);

        return Queue->num_control;
}

BYTE *Dequeue(QUEUE_HEAD *Queue)
{
        QUEUE_ENTRY *entry;
        BYTE *buffer;
        //int num;

        if (STAILQ_EMPTY(Queue)) {
                //printf("**Dequeue Empty!\n");
                return NULL;
        }
        pthread_mutex_lock(&Queue->lock);
        entry = STAILQ_FIRST(Queue);
        buffer = entry->Data;
        //num = entry->Number;
        STAILQ_REMOVE(Queue, entry, QEntry, Entries);
        pthread_mutex_unlock(&Queue->lock);

        //printf("**Dequeue list(%d):%p\n", num, buffer);
        return buffer;
}

void StartJobs(CMD_PARAM *Param)
{
        printf("**StartJobs\n");
}

void StopJobs()
{
        printf("**StopJobs\n");
}

void Shutdown(CMD_PARAM *Param)
{
        printf("**Shutdown\n");
}

//
void QueueData(QUEUE_HEAD *Queue, BYTE *DataBuffer, int Length)
{
        BYTE *queueBuffer;

        queueBuffer = calloc(DATABUFSIZ, 1);
        if (queueBuffer == NULL) {
                fprintf(stderr, "calloc error\n");
                return;
        }
        memcpy(queueBuffer, DataBuffer, Length);
        Enqueue(Queue, queueBuffer);
}

void *ReadThread(void *arg)
{
        EO_CONTROL *p = &EoControl;
        int fd = GetFd();
        ESP_STATUS rType;
        BYTE dataBuffer[DATABUFSIZ];
        USHORT  dataLength = 0;
        BYTE   optionLength = 0;
        BYTE   packetType = 0;
        BYTE   dataType;
        int    totalLength;

        //printf("**ReadThread()\n");
        while(!stop_read) {
                read_ready = TRUE;
                rType = GetPacket(fd, dataBuffer, (USHORT) DATABUFSIZ);
                if (stop_job) {
                        //printf("**ReadThread breaked by stop_job-1\n");
                        break;
                }
                else if (rType == OK) {

                        dataLength = (dataBuffer[0] << 8) + dataBuffer[1];
                        optionLength = dataBuffer[2];
                        packetType = dataBuffer[3];
                        dataType = dataBuffer[5];
                        totalLength = HEADER_SIZE + dataLength + optionLength + CRC8D_SIZE;

			if (p->Debug > 0)
				printf("dLen=%d oLen=%d tot=%d typ=%02X dat=%02X\n",
				       dataLength,
				       optionLength,
				       totalLength,
				       packetType,
				       dataType);
                }
                else {
                        printf("invalid rType==%02X\n\n", rType);
                }

                if (stop_job) {
                        printf("**ReadThread breaked by stop_job-2\n");
                        break;
                }
                //printf("**ReadTh: process=%d\n", packetType);

                switch (packetType) {
                case RADIO_ERP1: //1  Radio telegram
                case RADIO_ERP2: //0x0A ERP2 protocol radio telegram
                        QueueData(&DataQueue, dataBuffer, totalLength);
                        break;
                case RESPONSE: //2 Response to any packet
                        QueueData(&ResponseQueue, dataBuffer, totalLength);
                        break;
                case RADIO_SUB_TEL: //3 Radio subtelegram
                case EVENT: //4 Event message
                case COMMON_COMMAND: //5 Common command
                case SMART_ACK_COMMAND: //6 Smart Ack command
                case REMOTE_MAN_COMMAND: //7 Remote management command
                case RADIO_MESSAGE: //9 Radio message
                case CONFIG_COMMAND: //0x0B ESP3 configuration
                default:
                        QueueData(&ExtraQueue, dataBuffer, totalLength);
                        fprintf(stderr, "Unknown packet=%d\n", packetType);
                        break;
                }
        }

        //printf("ReadThread end=%d stop_read=%d\n", stop_job, stop_read);
        return (void*) NULL;
}

void Analyze(BYTE *Buffer)
{
        BYTE *p = Buffer;
        //USHORT dLen = p[0] << 8 | p[1];
        //USHORT oLen = p[2];

        PacketDump(p);
#if 0
        printf("##A:dLen=%d oLen=%d Typ=%X D0=%X D1=%X S=%X\n",
               dLen, oLen, p[3], p[5], p[6], p[dLen + oLen]);
#endif
}

//
void *ActionThread(void *arg)
{
        byte *data;
        size_t offset;
        QUEUE_ENTRY *qentry;
        BYTE buffer[DATABUFSIZ];

        while(!stop_action && !stop_job) {
                data = Dequeue(&DataQueue);
                if (data == NULL)
                        msleep(1);
                else {
                        memcpy(buffer, data, DATABUFSIZ);
                        offset = offsetof(QUEUE_ENTRY, Data);
                        qentry = (QUEUE_ENTRY *)(data - offset);
                        //printf("##%s: free=%p\n", __func__, qentry);
                        free(qentry);

                        Analyze(buffer);
			MainJob(buffer);
                }
        }
        //printf("##ActionThread() end=%d %d \n", stop_action, stop_job);
        return OK;

}

BOOL InitSerial(OUT int *pFd)
{
	static char *ESP_PORTS[] = {
		EO_ESP_PORT_USB,
		EO_ESP_PORT_S0,
		EO_ESP_PORT_AMA0,
		NULL
	};
	char *pp;
	int i;
        EO_CONTROL *p = &EoControl;
        int fd;
        struct termios tio;

	if (p->ESPPort[0] == '\0') {
		// default, check for available port
		pp = ESP_PORTS[0];
		for(i = 0; pp != NULL && *pp != '\0'; i++) {
			if ((fd = open(pp, O_RDWR)) >= 0) {
				close(fd);
				p->ESPPort = pp;
				//printf("##%s: found=%s\n", __func__, pp);
				break;
			}
			pp = ESP_PORTS[i+1];
		}
	}

        if (p->ESPPort && p->ESPPort[0] == '\0') {
                fprintf(stderr, "PORT access admission needed.\n");
		return 1;
	}
        else if ((fd = open(p->ESPPort, O_RDWR)) < 0) {
                fprintf(stderr, "open error:%s\n", p->ESPPort);
                return 1 ;
        }
        bzero((void *) &tio, sizeof(tio));
        //tio.c_cflag = B57600 | CRTSCTS | CS8 | CLOCAL | CREAD;
        tio.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
        tio.c_cc[VTIME] = 0; // no timer
        tio.c_cc[VMIN] = 1; // at least 1 byte
        //tcsetattr(fd, TCSANOW, &tio);
        cfsetispeed( &tio, B57600 );
        cfsetospeed( &tio, B57600 );

        cfmakeraw(&tio);
        tcsetattr( fd, TCSANOW, &tio );
        ioctl(fd, TCSETS, &tio);
        *pFd = fd;

        return 0;
}

//
// support ESP3 functions
// Common response
//
ESP_STATUS GetResponse(OUT BYTE *Buffer)
{
        INT startMSec;
        INT timeout;
        BYTE *data;
        size_t offset;
        QUEUE_ENTRY *qentry;
        ESP_STATUS responseMessage;

        startMSec = SystemMSec();
        do {
                data = Dequeue(&ResponseQueue);
                timeout = SystemMSec() - startMSec;
                if (data != NULL) {
                        //printf("**GetResponse() Got=%d\n", timeout);
                        memcpy(Buffer, data, DATABUFSIZ);
                        offset = offsetof(QUEUE_ENTRY, Data);
                        qentry = (QUEUE_ENTRY *)(data - offset);
                        //printf("**%s: free=%p\n", __func__, qentry);
                        free(qentry);
                        break;
                }
                if (timeout > RESPONSE_TIMEOUT)
                {
                        printf("****GetResponse() Timeout=%d\n", timeout);
                        return TIMEOUT;
                }
                msleep(1);
        }
        while(1);

        switch(Buffer[5]) {
        case 0:
        case 1:
        case 2:
        case 3:
                responseMessage = Buffer[5];
                break;
        default:
                responseMessage = INVALID_STATUS;
                break;
        }

        PacketDump(Buffer);
        printf("**GetResponse=%d\n", responseMessage);
        return responseMessage;
}

//
//
void SendCommand(BYTE *cmdBuffer)
{
        int length = cmdBuffer[2] + 7;
        //printf("**SendCommand fd=%d len=%d\n", GetFd(), length);

        write(GetFd(), cmdBuffer, length);
}

//
//
//
void DebugPrint(char *s)
{
	if (EoControl.Debug) {
		printf("##debug:%s\n", s);
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
	t.tv_nsec = (Usec % mega) * 1000 * 2; ////DEBUG////
	nanosleep(&t, NULL);
}

//
//
char *MakePath(char *Dir, char *File)
{
	char path[PATH_MAX];
	char *pathOut;

	if (File[0] == '/') {
		/* Assume absolute path */
		return(strdup(File));
	}
	strcpy(path, Dir);
	if (path[strlen(path) - 1] != '/') {
		strcat(path, "/");
	}
	strcat(path, File);
	pathOut = strdup(path);
	if (!pathOut) {
		Error("strdup() error");
	}
	return pathOut;
}

//
//
int EoReadControl()
{
	EO_CONTROL *p = &EoControl;

	if (p->ControlPath == NULL) {
		p->ControlPath = MakePath(p->BridgeDirectory, p->ControlFile); 
	}
	return( ReadCsv(p->ControlPath) );
}

void EoClearControl()
{
        EO_CONTROL *p = &EoControl;

 	if (p->ControlPath == NULL) {
		p->ControlPath = MakePath(p->BridgeDirectory, p->ControlFile); 
	}
        if (truncate(p->ControlPath, 0)) {
		fprintf(stderr, "%s: truncate error=%s\n", __FUNCTION__,
			p->ControlPath);
        }
	p->ControlCount = 0;
}


bool EoApplyFilter()
{
        INT i;
        UINT id;
	BYTE pid[4];
        EO_CONTROL *p = &EoControl;

        CO_WriteFilterDelAll(); //Filter clear

        for(i = 0; i < p->ControlCount; i++) {
                id = GetId(i);
                if (id != 0) {
			pid[0] = ((BYTE *)&id)[3];
			pid[1] = ((BYTE *)&id)[2];
			pid[2] = ((BYTE *)&id)[1];
			pid[3] = ((BYTE *)&id)[0]; 
			printf("**%s: id:%02X%02X%02X%02X\n",
			       __FUNCTION__, pid[0], pid[1], pid[2], pid[3]); 
			CO_WriteFilterAdd(pid);
                }
                else {
                        Warn("EoApplyFilter() id is 0");
                        return false;
                }
        }
        //FilterEnable();
	CO_WriteFilterEnable(TRUE);
        return true;
}

//
//
//
void EoParameter(int ac, char**av, EO_CONTROL *p)
{
        int mFlags = 1; //default
        int rFlags = 0;
        int oFlags = 0;
        int cFlags = 0;
        int vFlags = 0;
        int opt;
        int timeout = 0;
        char *controlFile = EO_CONTROL_FILE;
        char *commandFile = EO_COMMAND_FILE;
        char *brokerFile = BROKER_FILE;
        char *bridgeDirectory = EO_DIRECTORY;
        char *eepFile = EO_EEP_FILE;
        char *serialPort = "\0";

        while ((opt = getopt(ac, av, "mrocvf:d:t:e:s:z:")) != EOF) {
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
                case 'z':
                        commandFile = optarg;
                        break;
                case 'b':
                        brokerFile = optarg;
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
                case 's': //ESP serial port
                        serialPort = optarg;
                        break;
                default: /* '?' */
                        fprintf(stderr,
                                "Usage: %s [-m|-r|-o][-c][-v]\n"
				"  [-d Directory][-f Controlfile][-e EEPfile][-b BrokerFile]\n"
				"  [-s SeriaPort][-z CommandFile]\n"
                                "    -m    Monitor mode\n"
                                "    -r    Register mode\n"
                                "    -o    Operation mode\n"
                                "    -c    Clear settings before register\n"
                                "    -v    View working status\n"
                                "    -d    Bridge file directrory\n"
                                "    -f    Control file\n"
                                ,av[0]);

			CleanUp(0);
                        exit(EXIT_FAILURE);
                }
        }
	p->Mode = oFlags ? Operation : rFlags ? Register : Monitor;
        p->CFlags = cFlags;
        p->VFlags = vFlags;
        p->Timeout = timeout;
        p->ControlFile = strdup(controlFile);
        p->CommandFile = strdup(commandFile);
        p->BrokerFile = strdup(brokerFile);
        p->EEPFile = strdup(eepFile);
        p->BridgeDirectory = strdup(bridgeDirectory);
	p->ESPPort = serialPort;
}

void EoSetEep(EO_CONTROL *P, byte *Id, byte *Data, uint Rorg)
{
	char eep[10];
	uint func, type, man = 0;
	EEP_TABLE *eepTable;
        EEP_TABLE *pe;
	DATAFIELD *pd;
	char idBuffer[10];
	FILE *f;
	struct stat sb;
	int rtn, i;
	BOOL isUTE = FALSE;

	sprintf(idBuffer, "%02X%02X%02X%02X", Id[0], Id[1], Id[2], Id[3]);
	if (P->VFlags) {
		printf("<%s>\n", idBuffer);
	}

	switch (Rorg) {
	case 0xF6: // RPS
		func = 0x02;
		type = P->ERP1gw ? 0x01 : 0x04;
		man = 0xb00;
		break;

	case 0xD5: // 1BS
		func = 0x00;
		type = 0x01;
		man = 0xb00;
		break;

	case 0xA5: // 4BS
		DataToEep(Data, &func, &type, &man);
		break;

	case 0xD2: //VLD
		func = 0x03;
		type = 0x20;
		man = 0xb00;
		break;

	case 0xD4: // UTE:
		Rorg = Data[6];
		func = Data[5];
		type = Data[4];
		man = Data[3] & 0x07;
		isUTE = TRUE;
		break;
	default:
		func = 0x00;
		type = 0x00;
		break;
	}
	if (!isUTE && man == 0) {
		fprintf(stderr, "EoSetEep: %s no man ID is set\n", idBuffer);
		return;
	}

	//printf("DTeep:%02X-%02X-%02X->", Data[0], Data[1], Data[2]);
	//printf("%02X-%02X-%02X\n", Rorg, func, type);
	sprintf(eep, "%02X-%02X-%02X", Rorg, func, type);
	eepTable = GetEep(eep);
	if (eepTable == NULL) {
		fprintf(stderr, "EoSetEep: %s EEP is not found=%s\n",
			idBuffer, eep);
		return;
	}
	if (P->ControlPath == NULL) {
		P->ControlPath = MakePath(P->BridgeDirectory, P->ControlFile); 
	}
	rtn = stat(P->BridgeDirectory, &sb);
	if (rtn < 0){
		mkdir(P->BridgeDirectory, 0777);
	}
	rtn = stat(P->BridgeDirectory, &sb);
	if (!S_ISDIR(sb.st_mode)) {
		fprintf(stderr, "EoSetEep: Directory error=%s\n", P->BridgeDirectory);
		return;
	}

	printf("EoSetEep:<%s %s>\n", idBuffer, eep);

	f = fopen(P->ControlPath, "a+");
	if (f == NULL) {
		fprintf(stderr, "EoSetEep: cannot open control file=%s\n",
			P->ControlPath);
		return;
	}

	// SetNewEep //
	fprintf(f, "%s,%s,%s", idBuffer, eep, eepTable->Title);
	pe = eepTable;
	pd = pe->Dtable;
	fflush(f);
	for(i = 0; i < pe->Size; i++, pd++) {
		char *pointName; //newShotCutName
		if (f == NULL) {
			fprintf(stderr, "EoSetEep: cannot open control file=%s\n",
				P->ControlPath);
			return;
		}
		
		if (pd->DataName == NULL)
			continue;
		else if (!strcmp(pd->DataName, "LRN Bit") || !strcmp(pd->ShortCut, "LRNB")
			 || !strcmp(pd->DataName, "Learn Button")) {
			continue; // Skip Learn bit
		}

		pointName = GetNewName(pd->ShortCut);
		if (pointName == NULL) {
			Error("GetNewName error");
			      pointName = "ERR";
		}
		fprintf(f, ",%s", pointName);
		fflush(f);
		//printf("******* Wrote newpoint=%s\n", pointName);
	}
	fprintf(f, "\r\n");
	fflush(f);
	fclose(f);
	P->ControlCount = -1; //Mark to updated

	if (P->VFlags) {
		fprintf(stderr, "%s %s registered for EEP(%s) pcnt=%d\n",
			__FUNCTION__, idBuffer, eep, i);
	}
}

//
//
void PrintTelegram(EO_PACKET_TYPE PacketType, byte *Id, byte ROrg, byte *Data)
{
	UINT func, type, man;
	BOOL teachIn = FALSE;

	printf("P:%02X,%02X%02X%02X%02X,%02X=",
	       PacketType, Id[0], Id[1], Id[2], Id[3], ROrg);

	switch(ROrg) {
	case 0xF6: //RPS
		teachIn = TRUE;
		printf("RPS:%02X,%02X\n", Data[0], Data[1]);
		break;
		
	case 0xD5: //1BS
		teachIn = (Data[0] & 0x08) == 0;
		printf("1BS:%02X,%02X %s", Data[0], Data[1], teachIn ? "T" : "");
		break;

	case 0xA5: //4BS
		teachIn = (Data[3] & 0x08) == 0;
		printf("4BS:%02X %02X %02X %02X,%02X %s",
		       Data[0], Data[1], Data[2], Data[3], Data[4], teachIn ? "T" : "");
		break;

	case 0xD2: //VLD
		printf("VLD:%02X %02X", Data[0], Data[1]);
		break;

	case 0xD4: // UTE:
		teachIn = 1;
		printf("UTE:%02X %02X %02X %02X %02X %02X %02X UT\n",
		       Data[0], Data[1], Data[2], Data[3], Data[4], Data[5], Data[6]);
		break;
	default:
		break;
	}
	printf("\n");

	if ((ROrg == 0xA5 || ROrg == 0xD5) && teachIn) {
		DataToEep(Data, &func, &type, &man);
		printf("*PT TeachIn:%02X-%02X-%02X %03x\n",
		       ROrg, func, type, man);
	}
	else if (ROrg == 0xD4) { //UTE
		func = Data[5];
		type = Data[4];
		man = Data[3] & 0x07;
		printf("*PT TeachIn:%02X-%02X-%02X %03x\n",
		       ROrg, func, type, man);
	}
}

void EoReloadControlFile()
{
	EO_CONTROL *p = &EoControl;

	if (p->ControlPath == NULL) {
		p->ControlPath = MakePath(p->BridgeDirectory, p->ControlFile); 
	}
	//if (p->ControlCount < 0) {
	// ControlFile is updated 
	p->ControlCount = ReadCsv(p->ControlPath);
	//}
}

//
void WriteBridge(char *FileName, double ConvertedData)
{
	EO_CONTROL *p = &EoControl;
	FILE *f;
	char *bridgePath = MakePath(p->BridgeDirectory, FileName);

	f = fopen(bridgePath, "w");
	fprintf(f, "%.2f\r\n", ConvertedData);
	fflush(f);
	fclose(f);
}

//
//
//
void CleanUp(int Signum)
{
        EO_CONTROL *p = &EoControl;

	if (p->PidPath) {
		unlink(p->PidPath);
	}
	exit(0);
}

void SignalAction(int signo, void (*func)(int))
{
        struct sigaction act, oact;

        act.sa_handler = func;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        if (sigaction(signo, &act, &oact) < 0) {
                fprintf(stderr, "error at sigaction\n");
        }
}

void SignalCatch(int Signum)
{
	SignalAction(SIGUSR2, SIG_IGN);
}

//
// Brokers
//
typedef struct _brokers
{
	char *Name;
	char *PidPath;
	pid_t pid;

} BROKERS;

BROKERS BrokerTable[MAX_BROKER];

RETURN_CODE InitBrokers()
{
	EO_CONTROL *p = &EoControl;
	BROKERS *pb = &BrokerTable[0];
	char *pt;
	int i;
	FILE *f;
	char *rtn;
	char buf[BUFSIZ];

	p->BrokerPath = MakePath(p->BridgeDirectory, p->BrokerFile);
	f = fopen(p->BrokerPath, "r");
	if (f == NULL) {
		printf("%s: no broker file=%s\n", __FUNCTION__, p->BrokerPath);
		return NO_FILE;
	}

	memset(&BrokerTable[0], 0, sizeof(BROKERS) * MAX_BROKER);

	for(i = 0; i < MAX_BROKER; i++) {
		rtn = fgets(buf, BUFSIZ, f);
		if (rtn == NULL) {
			break;
		}
		pt = &buf[strlen(buf)] - 1;
		if (*pt == '\n')
			*pt = '\0'; // clear the last CR
		pb->Name = strdup(buf);
		strcat(buf, ".pid");
		pb->PidPath = MakePath(p->BridgeDirectory, buf);
		pb++;
	}
	fclose(f);

	return (i > 0 ? OK : NO_FILE);
}

void NotifyBrokers(long num)
{
	enum { PIDLEN = 32 };
	BROKERS *pb = &BrokerTable[0];
	int i;
	FILE *f;
	char *rtn;
	char buf[PIDLEN];

	for(i = 0; i < MAX_BROKER; i++, pb++) {
		if (pb->Name == NULL || pb->Name[0] == '\0')
			break;

		printf("$%s:%d name=%s path=%s\n",
		       __FUNCTION__, i, pb->Name, pb->PidPath);
		
		f = fopen(pb->PidPath, "r");
		if (f == NULL) {
			Warn("no pid file");
			continue;
		}
		rtn = fgets(buf, PIDLEN, f);
		if (rtn == NULL) {
			Warn("pid file read error");
			fclose(f);
			continue;
		}
		pb->pid = (pid_t) strtol(buf, NULL, 10);
		fclose(f);

		// Notify to each broker
                if (sigqueue(pb->pid, SIG_BROKERS, (sigval_t) ((void *) num)) < 0){
                        Warn("sigqueue error");
                }
	}
}

//
bool MainJob(BYTE *Buffer)
{
	EO_CONTROL *p = &EoControl;
	EO_PACKET_TYPE packetType;
	INT dataLength;
	INT optionLength;
	BYTE id[4];
	BYTE data[BUFSIZ];
	//byte option[BUFSIZ];
	BYTE rOrg;
	BYTE status;
	BOOL teachIn = FALSE;
	extern void PrintItems(); //// DEBUG
	//extern void PrintEepAll();
	extern void PrintProfileAll();
	extern int GetTableIndex(UINT Id);

	dataLength = (int) (Buffer[0] << 8 | Buffer[1]);
	optionLength = (int) Buffer[2];
	packetType = (EO_PACKET_TYPE) Buffer[3];
	rOrg = Buffer[5];
	bzero(data, 16);

	id[0] = Buffer[dataLength];
	id[1] = Buffer[dataLength + 1];
	id[2] = Buffer[dataLength + 2];
	id[3] = Buffer[dataLength + 3];
	status = Buffer[dataLength + 4];

	switch(rOrg) {
	case 0xF6: // RPS
		data[0] = Buffer[6];
		data[1] = status;
		data[2] = 0;
		data[3] = 0;
		teachIn = TRUE; // RPS telegram. always teach In
		break;
	case 0xD5: // 1BS
		data[0] = Buffer[6];
		data[1] = status;
		data[2] = 0;
		data[3] = 0;
                teachIn = (data[0] & 0x08) == 0;
		break;
	case 0xA5: // 4BS
		data[0] = Buffer[6];
		data[1] = Buffer[7];
		data[2] = Buffer[8];
		data[3] = Buffer[9];
                teachIn = (data[3] & 0x08) == 0;
		break;
	case 0xD2:  // VLD
		memcpy(&data[0], &Buffer[6],
		       dataLength > 20 ? 20 : dataLength);
		if (dataLength == 7 && optionLength == 7
		    && data[0] == 0x80 && status == 0x80) {
			// D2-03-20
			printf("**VLD teachIn=D2-03-20\n");
			teachIn = TRUE;
		}
		break;
	case 0xD4:  // UTE
		data[0] = Buffer[6];
		data[1] = Buffer[7];
		data[2] = Buffer[8];
		data[3] = Buffer[9];
		data[4] = dataLength > 7 ? Buffer[10] : 0;
		data[5] = dataLength > 8 ? Buffer[11] : 0;
		data[6] = dataLength > 9 ? Buffer[12] : 0;
                teachIn = 1;
		break;
	default:
		fprintf(stderr, "%s: Unknown rOrg = %02X (%d %d)\n",
			__FUNCTION__, rOrg, dataLength, optionLength);
		return false;
		break;
	}

	if (packetType != RadioErp1) {
		fprintf(stderr, "%s: Unknown type = %02X (%d %d)\n",
			__FUNCTION__, packetType, dataLength, optionLength);
		return false;
	}

	if (p->Mode == Monitor || p->VFlags) {
		PrintTelegram(packetType, id, rOrg, data);
	}

	if (p->VFlags)
		printf("id:%02X%02X%02X%02X %s\n", id[0] , id[1] , id[2] , id[3], teachIn ? "T" : "");

	if (p->Mode == Register && teachIn) {
		switch(rOrg) {
		case 0xF6: //RPS:
			if (!CheckTableId(ByteToId(id))) {
				// is not duplicated, then register
				EoSetEep(p, id, data, rOrg);
			}
			break;

		case 0xD5: //1BS:
			if (!CheckTableId(ByteToId(id))) {
				// is not duplicated, then register
				EoSetEep(p, id, data, rOrg);
			}
			break;


		case 0xD2: //VLD:
			if (!CheckTableId(ByteToId(id))) {
				// is not duplicated, then register
				EoSetEep(p, id, data, rOrg);
			}
			break;

		case 0xA5: //4BS
			if (!CheckTableId(ByteToId(id))) {
				// is not duplicated, then register
				EoSetEep(p, id, data, rOrg);
			}
			break;

		case 0xD4: //UTE
			if (!CheckTableId(ByteToId(id))) {
				// is not duplicated, then register
				EoSetEep(p, id, data, rOrg);
			}
		default:
			break;
		}

		EoReloadControlFile();
		
		//PrintEepAll();
		//PrintProfileAll();
	}
	else if (p->Mode == Operation) {
		int nodeIndex = -1;

		switch(rOrg) {
		case 0xF6: //RPS:
			if (CheckTableId(ByteToId(id))) {
				WriteRpsBridgeFile(ByteToId(id), data);
				nodeIndex = GetTableIndex(ByteToId(id));
				if (nodeIndex >= 0) {
					NotifyBrokers((long) nodeIndex);
				}
			}
		case 0xD5: //1BS:
			if (!teachIn && CheckTableId(ByteToId(id))) {
				Write1bsBridgeFile(ByteToId(id), data);
				nodeIndex = GetTableIndex(ByteToId(id));
				if (nodeIndex >= 0) {
					NotifyBrokers((long) nodeIndex);
				}
			}
			break;
			
		case 0xD2: //VLD:
			if (CheckTableId(ByteToId(id))) {
				WriteVldBridgeFile(ByteToId(id), data);
				nodeIndex = GetTableIndex(ByteToId(id));
				if (nodeIndex >= 0) {
					if (!teachIn)
						NotifyBrokers((long) nodeIndex);
				}
			}
			break;

		case 0xA5: //4BS:
			if (CheckTableId(ByteToId(id))) {
				Write4bsBridgeFile(ByteToId(id), data);
				nodeIndex = GetTableIndex(ByteToId(id));
				if (nodeIndex >= 0) {
					if (!teachIn)
						NotifyBrokers((long) nodeIndex);
				}
			}
			break;

		case 0xD4: //UTE
		default:
			break;
		}
	}
	PrintItems(); //// DEBUG

	return TRUE;
}

//
//
//
int main(int ac, char **av)
{
        pid_t myPid = getpid();
        CMD_PARAM param;
        JOB_COMMAND cmd;
        int fd;
        int rtn;
	FILE *f;
        pthread_t thRead;
        pthread_t thAction;
        //unsigned char writeBuffer[12];
        THREAD_BRIDGE *thdata;
	bool working;
	bool initStatus;
	BYTE versionString[64];
	EO_CONTROL *p = &EoControl;
	//extern void PrintEepAll();
	extern void PrintProfileAll();

        printf("**main() pid=%d\n", myPid);
        printf("%s%s", version, copyright);

	// Signal handling
        SignalAction(SIGINT, CleanUp);
        SignalAction(SIGTERM, CleanUp);
        SignalAction(SIGUSR2, SIG_IGN);
	////////////////////////////////
	// Stating parateters, profiles, and files

	memset(EoFilterList, 0, sizeof(long) * EO_FILTER_SIZE);

	p->Mode = Monitor; // Default mode
	p->Debug = true; // for Debug

	EoParameter(ac, av, p);
	p->PidPath = MakePath(p->BridgeDirectory, PID_FILE);
	f = fopen(p->PidPath, "w");
	if (f == NULL) {
		fprintf(stderr, ": cannot create pid file=%s\n",
			p->PidPath);
		exit(0);
	}
	fprintf(f, "%d\n", myPid);
	fclose(f);
	if (InitBrokers() != OK) {
		fprintf(stderr, "No broker notify mode\n");
	}

	switch(p->Mode) {
	case Monitor:
		p->FilterOp = Ignore;
		if (p->VFlags)
			fprintf(stderr, "Start monitor mode.\n");
		break;

	case Register:
		p->FilterOp = p->CFlags ? Clear : Ignore;
		if (p->VFlags)
			fprintf(stderr, "Start register mode.\n");
		break;

	case Operation:
		p->FilterOp = Read;
		if (p->VFlags)
			fprintf(stderr, "Start operation mode.\n");
		break;
	}

	if (!InitEep(p->EEPFile)) {
                fprintf(stderr, "InitEep error=%s\n", p->EEPFile);
		CleanUp(0);
                exit(1);
        }

	if (p->FilterOp == Clear) {
		EoClearControl();
	}
	else {
		p->ControlCount = EoReadControl();
	}
	//PrintEepAll();
	//PrintProfileAll();

	////////////////////////////////
	// Threads, queues, and serial pot

        STAILQ_INIT(&DataQueue);
        STAILQ_INIT(&ResponseQueue);
        STAILQ_INIT(&ExtraQueue);
        pthread_mutex_init(&DataQueue.lock, NULL);
        pthread_mutex_init(&ResponseQueue.lock, NULL);
        pthread_mutex_init(&ExtraQueue.lock, NULL);

        if (InitSerial(&fd)) {
                fprintf(stderr, "InitSerial error\n");
		CleanUp(0);
                exit(1);
        }

        thdata = calloc(sizeof(THREAD_BRIDGE), 1);
        if (thdata == NULL) {
                printf("calloc error\n");
		CleanUp(0);
                exit(1);
        }
        SetFd(fd);

        rtn = pthread_create(&thAction, NULL, ActionThread, (void *) thdata);
        if (rtn != 0) {
                fprintf(stderr, "pthread_create() ACTION failed for %d.",  rtn);
		CleanUp(0);
                exit(EXIT_FAILURE);
        }

        rtn = pthread_create(&thRead, NULL, ReadThread, (void *) thdata);
        if (rtn != 0) {
                fprintf(stderr, "pthread_create() READ failed for %d.",  rtn);
		CleanUp(0);
                exit(EXIT_FAILURE);
        }

        thdata->ThAction = thAction;
        thdata->ThRead = thRead;
        SetThdata(*thdata);

	if (p->Debug > 0)
        printf("**main() started fd=%d(%s) mode=%d(%s) Clear=%d\n",
	       fd, p->ESPPort, p->Mode, CommandName(p->Mode), p->CFlags);

	while(!read_ready)
        {
                printf("**main() Wait read_ready\n");
                msleep(100);
        }
	if (p->Debug > 0)
		printf("**main() read_ready\n");

	p->ERP1gw = 0;	
	rtn = 0;
	do {
		initStatus = CO_WriteReset();
		if (initStatus == OK) {
			msleep(200);
			initStatus = CO_WriteMode(0);
		}
		if (initStatus == RET_NOT_SUPPORTED) {
			printf("**main() Oops! this GW should be ERP1, and mark it.\n");
			p->ERP1gw = 1;
			break;
		}
		else if (initStatus != OK) {
			printf("**main() Reset/WriteMode status=%d\n", initStatus);
			msleep(300);
			rtn++;
			if (rtn > 5) {
				printf("**main() Error! Reset/WriteMode failed=%d\n", rtn);
				CleanUp(0);
				exit(EXIT_FAILURE);
			}
		}
	}
	while(initStatus != OK);

	if (p->VFlags) {
		CO_ReadVersion(versionString);
	}

	if (p->Mode == Operation && p->ControlCount > 0) {
		EoApplyFilter();
	}

	//PrintEepAll();
	//PrintProfileAll();

	///////////////////
	// Interface loop
	working = TRUE;
	while(working) {
		int newMode;

                printf("Wait...\n");
		SignalAction(SIGUSR2, SignalCatch);
#if 1 // wait forever
		while(sleep(60*60*24*365) == 0)
			;
#else // old way
		sigNo = sigwaitinfo(&sigmask, &siginfo);
                if (sigNo < 0){
                        if(errno != EINTR){
                                perror("sigwaitinfo error");
                                exit(1);
                        }
                        continue;
                }
                printf("Get sigNo=%d\n", sigNo);
#endif
                cmd = GetCommand(&param);
                switch(cmd) {
		case CMD_NOOP:
			if (p->Debug > 0)
				printf("cmd:NOOP\n");
			sleep(1);
			break;
                case CMD_FILTER_ADD: //Manual-add, not tested, not implemented
			/* Now! need debug */
			//BackupControl();
			//FilterAdd() and
                        //CO_WriteFilterAdd(param.Data);
                        //CO_WriteFilterEnable
                        break;

                case CMD_FILTER_CLEAR:
			/* Now! need debug */
			//BackupControl();
			EoClearControl();
			//CO_WriteReset();
			CO_WriteFilterDelAll();
			CO_WriteFilterEnable(OFF);
                        break;

                case CMD_CHANGE_MODE:
			newMode = param.Data[0];
			if (p->Debug > 0) {
				printf("newMode=%c\n", newMode);
			}

			if (newMode == 'M' /*Monitor*/) {
				/* Now! need debug */
				p->Mode = Monitor;
				CO_WriteFilterDelAll();
				CO_WriteFilterEnable(OFF);
			}
			else if (newMode == 'R' /*Register*/) {
				/* Now! need debug */
				p->Mode = Register;
				CO_WriteFilterEnable(OFF);
				/* Now! Enable Teach IN */
			}
			else if (newMode == 'C' /*Clear and Register*/) {
				/* Now! need debug */
				EoClearControl();
				CO_WriteFilterDelAll();
				CO_WriteFilterEnable(OFF);
				p->ControlCount = 0;
				//CO_WriteReset();

				p->Mode = Register;
			}
			else if (newMode == 'O' /* Operation */) {
				p->Mode = Operation; 
				p->FilterOp = Read;
				p->ControlCount = EoReadControl();

				printf("p->ControlCount=%d\n", p->ControlCount);
				if (p->ControlCount > 0) {
					if (!EoApplyFilter()) {
						fprintf(stderr, "EoInitPortError\n");
					}
				}
			}
                        //StartJobs(&param);
                        break;

                case CMD_CHANGE_OPTION:
			newMode = param.Data[0];
			if (p->Debug > 0) {
				printf("newOpt=%c\n", newMode);
			}

			if (newMode == 'S' /*Silent*/) {
				p->VFlags = FALSE;
				p->Debug  = 0;
			}
			else if (newMode == 'V' /*Verbose*/) {
				p->VFlags  = TRUE;
			}
			else if (newMode == 'D' /*Debug*/) {
				p->Debug++;
			}
                        break;

                case CMD_SHUTDOWN:
                case CMD_REBOOT:
                        Shutdown(&param);
                        break;

                default:
			printf("**Unknown command=%c\n", cmd);
                        break;
                }
		if (p->Debug > 0) {
			printf("cmd=%d\n", cmd);
		}
		sleep(1);
	}
	return 0;
}
