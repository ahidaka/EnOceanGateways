#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include "typedefs.h"
#include "utils.h"
#include "dpride.h"
#include "ptable.h"
#include "../eolib/models.h"

typedef enum {
	JsonData = 0,
	JsonTeachIn = 1
}
TELEGRAM_TYPE;

typedef struct QueueHead QUEUE_HEAD;

BOOL JsonCreate(TELEGRAM_TYPE Type, UINT Id, CHAR *Profile, BYTE Rorg, UINT Secure);
VOID JsonAddData(CHAR *Key, DOUBLE Value, CHAR *Unit);
VOID JsonAddInt(CHAR *Key, INT Value);
VOID JsonAddDbm(INT Dbm);
VOID JsonAddManId(USHORT ManId);
VOID JsonAddInfo(VOID);
VOID JsonTimeStamp(CHAR *TimeStamp);
INT JsonRelease(QUEUE_HEAD *Queue);
VOID JsonSetup(USHORT Port, QUEUE_HEAD *Queue);
INT JsonStart(VOID);
VOID JsonStop(VOID);

extern INT Enqueue(QUEUE_HEAD *Queue, BYTE *Buffer);
extern BYTE *Dequeue(QUEUE_HEAD *Queue);

