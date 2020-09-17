#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h> // offsetof
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include "queue.h"
#include "json.h"

#define DATA_TELEGRAM	"dataTelegram"
#define TEACHIN_TELEGRAM "teachInTelegram"
#define DEVICE_ID		"deviceId"
#define FRIENDLY_ID		"friendlyId"
#define TIME_STAMP		"timestamp"
#define DIRECTION		"direction"
#define FROM 			"from"
#define TO 				"to"
#define SECURITY		"security"
#define PROFILE			"profile"
#define FUNCTIONS		"functions"
#define KEY				"key"
#define VALUE			"value"
#define UNIT			"unit"
#define TELEGRAN_INFO	"telegramInfo"
#define DATA			"data"
#define STATUS			"status"
#define DBM				"dbm"
#define RORG			"rorg"
#define MAN_ID			"manId"

//#define PRINTF(fmt, ...) printf("#%s: " fmt, __func__, ##__VA_ARGS__)
#define PRINTF(fmt, ...) ////

typedef struct _json_writer {
	CHAR JsonString[8 * 1024];
	INT LastPosition;
	INT FuncCount;
	UINT Secure;
	BYTE Rorg;
	BYTE Status;
	BYTE Data[16];
	BYTE TimeStamp[16];
	INT Dbm;
	USHORT Port;
	QUEUE_HEAD *Queue;
	INT Running;
	INT Stop;
} JSON_WRITER;

JSON_WRITER Json;

#define JSON_STR (Json.JsonString)

CHAR *JsonGetBuffer(VOID);
VOID *JsonServer(VOID *Message);

static pthread_t JsonHandle = (pthread_t) NULL;

//const CHAR SPACES[] = "  ";
//const CHAR L_BRACKET[] = "[";
//const CHAR R_BRACKET[] = "]";
//const CHAR L_BRACES[] = "{";
//const CHAR R_BRACES[] = "}";

//
// JSON String processor
//
CHAR *GetTitle(CHAR *Profile)
{
	EEP_TABLE *eepTable;
	CM_TABLE *pmc;
	CHAR *title;

	if (Profile[0] >= 'A' && Profile[0] <= 'F') {
		// EEP
		eepTable = GetEep(Profile);
		title = eepTable->Title;
	}
	else {
		// GP
		pmc = CmGetCache(Profile);
		title = pmc->Title;
	}
	return title;
}

BOOL JsonCreate(TELEGRAM_TYPE Type, UINT Id, CHAR *Profile, BYTE Rorg, UINT Secure)
{
	const CHAR *types[2] = {DATA_TELEGRAM, TEACHIN_TELEGRAM};
	INT length;
	
	length = sprintf(JSON_STR,
		"{\n  \"%s\" : {\n"
		"    \"deviceId\" : \"%08X\",\n",
		types[Type % 2], Id);
	length += sprintf(JSON_STR + length,
		"    \"friendlyId\" : \"%s\",\n", GetTitle(Profile));
	length += sprintf(JSON_STR + length,
		"    \"direction\" : \"from\",\n"
		"    \"security\" : \"%u\",\n", Secure);
	length += sprintf(JSON_STR + length,
		"    \"profile\" : \"%s\",\n", Profile);

	Json.LastPosition = length;
	Json.FuncCount = 0;
	Json.Rorg = Rorg;
	Json.Secure = Secure;

	return FALSE;
}

VOID JsonAddFunc(CHAR *Key, CHAR *Value, CHAR *Unit)
{
	INT length = Json.LastPosition;

	if (Json.FuncCount == 0) {
		length += sprintf(JSON_STR + length,
			"    \"functions\" : [\n");
	}
	else {
		length += sprintf(JSON_STR + length, ",\n");
	}
	length += sprintf(JSON_STR + length,
		"      {\n"
		"        \"key\" : \"%s\",\n"
		"        \"value\" : \"%s\",\n"
		"        \"unit\" : \"%s\"\n"
		"      }",
		Key, Value, Unit);

	Json.FuncCount++;
	Json.LastPosition = length;
}

VOID JsonAddData(CHAR *Key, DOUBLE Value, CHAR *Unit)
{
	CHAR buffer[32];
	sprintf(buffer, "%.2lf", Value);
	JsonAddFunc(Key, buffer, Unit);
}

VOID JsonAddInt(CHAR *Key, INT Value)
{
	CHAR buffer[8];
	sprintf(buffer, "%d", Value);
	JsonAddFunc(Key, buffer, "");
}

VOID JsonAddManId(USHORT ManId)
{
	INT length = Json.LastPosition;
	length += sprintf(JSON_STR + length,
		"    \"manId\" : \"%03x\",\n", ManId);
	Json.LastPosition = length;
}

VOID JsonAddInfo(VOID)
{
	INT length = Json.LastPosition;
	length += sprintf(JSON_STR + length,
		"\n"
		"    ],\n"
		"    \"telegramInfo\" : {\n"
		"      \"dbm\" : -%d,\n"
		"      \"rorg\" : \"%02X\"\n"
		"    }\n",
		Json.Dbm, Json.Rorg);
	Json.LastPosition = length;
}

VOID JsonFinalize(VOID)
{
	INT length = Json.LastPosition;

	length += sprintf(JSON_STR + length,
		"  }\n"
		"}\n");
#if 0
	printf("**Finalize**pos=%d length=%d strlen=%d\n", Json.LastPosition, length, strlen(JSON_STR));
#endif

	Json.LastPosition = length;
}

VOID JsonAddDbm(INT Dbm)
{
	Json.Dbm = Dbm;
	JsonAddInfo();
	JsonFinalize();
}

VOID JsonTimeStamp(CHAR *TimeStamp)
{
	INT length = Json.LastPosition;
	length += sprintf(JSON_STR + length,
		"    \"timestamp\" : \"%s\",\n", TimeStamp);
	Json.LastPosition = length;
}

//
// JSON Server
//
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define JSON_BUFFER_COUNT (4)
#define JSON_BUFFER_SIZE (8 * 1024)

typedef struct QEntry {
	STAILQ_ENTRY(QEntry) Entries;
	INT Number;
	INT Length;
	CHAR Data[JSON_BUFFER_SIZE];
}
QUEUE_ENTRY;

#define QueueSetLength(Buf, Len) \
	((QUEUE_ENTRY *)((Buf) - offsetof(QUEUE_ENTRY, Data)))->Length = (Len)

#define QueueGetLength(Buf) \
	(((QUEUE_ENTRY *)((Buf) - offsetof(QUEUE_ENTRY, Data)))->Length)

static QUEUE_ENTRY *JsonBuffer[JSON_BUFFER_COUNT];
static INT JsonIndex;

//
//
//
INT JsonRelease(QUEUE_HEAD *Queue)
{
	CHAR *buffer;
	INT length = Json.LastPosition;

	JsonFinalize();
	buffer = JsonGetBuffer();
	memcpy(buffer, JSON_STR, length);
	buffer[length] = '\0';
	QueueSetLength(buffer, length + 1);
	Enqueue(Queue, (BYTE *) buffer);
	Json.Queue = Queue;

	return length;
}

VOID JsonSetup(USHORT Port, QUEUE_HEAD *Queue)
{
	INT i;

	Json.Port = Port;
	Json.Queue = Queue;
	for(i = 0; i < JSON_BUFFER_COUNT; i++) {
		JsonBuffer[i] = malloc(sizeof(QUEUE_ENTRY));
		if (JsonBuffer[i] == NULL) {
			fprintf(stderr, "Cannot allocate JSON buffer=%d,%d\n",
				i, sizeof(QUEUE_ENTRY));
		}
		// Clear queue management area
		memset(JsonBuffer[i], 0, offsetof(QUEUE_ENTRY, Data));
	}
}

CHAR *JsonGetBuffer(VOID)
{
	QUEUE_ENTRY *p = (QUEUE_ENTRY *) JsonBuffer[(JsonIndex++) & (JSON_BUFFER_COUNT - 1)];
	return(p->Data);
}

INT JsonStart(VOID)
{
	if (Json.Running > 0) {
		return 0;
	}
	if (JsonHandle != (pthread_t) NULL) {
		fprintf(stderr, "JSON Server already started\n");
		return 0;
	}
	Json.Stop = 0;
	pthread_create(&JsonHandle, NULL, JsonServer, NULL);
	PRINTF("Accepted\n");
	return 1;	
}

VOID JsonStop(VOID)
{
	PRINTF("Received\n");
	Json.Stop = 1;
}

VOID *JsonServer(VOID *Message)
{
	struct sockaddr_in sinme;
	struct sockaddr_in from;
	socklen_t len = sizeof(from);
	INT sock, sock2;
	CHAR *buffer;
	SHORT port = Json.Port;
	(VOID) Message;

	PRINTF("Will start\n");

	sinme.sin_family = PF_INET;
	sinme.sin_port = htons(port);
	sinme.sin_addr.s_addr = htonl(INADDR_ANY);

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if(bind(sock, (struct sockaddr *)&sinme, sizeof(sinme)) < 0) {
		fprintf(stderr, "JSON Server Can't bind.\n");
		return (VOID *) 1;
	}
	listen(sock, SOMAXCONN);

	PRINTF("Waiting for Connection Request.\n");

	sock2 = accept(sock, (struct sockaddr *)&from, &len);
	if (sock2 < 0) {
		fprintf(stderr, "JSON Server Can't accepted.\n");
		return (VOID *) 1;
	}
	else {
		PRINTF("Connected from %s.\n", inet_ntoa(from.sin_addr));
	}

	while(!Json.Stop) {
		INT length;
		ssize_t result;

		buffer = (CHAR *) Dequeue(Json.Queue);
		if (buffer != NULL) {
			length = QueueGetLength(buffer);
#if 0
			printf("**ServerSend**strlen=%d length=%d\n", strlen(buffer), length);
#endif
			result = send(sock2, buffer, length, 0);
			if (result != length) {
				fprintf(stderr, "JServer: send error=%d(%s)\n", errno, strerror(errno));
			}
		}
		sleep(1);
	}
	close(sock);
	PRINTF("Will exit\n");
	Json.Running = 0;
	return 0;
}
