#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#ifdef EXTERNAL_BROKER
#include "../dpride/typedefs.h"
#include "../dpride/utils.h"
#include "../dpride/dpride.h"
#else
#include "typedefs.h"
#include "utils.h"
#include "dpride.h"
#endif

#define EO_DIRECTORY "/var/tmp/dpride"
#define AZ_PID_FILE "azure.pid"
#define AZ_BROKER_FILE "brokers.txt"

#define SIGENOCEAN (SIGRTMIN + 6)

#define EO_DATSIZ (8)

typedef struct _eodata {
        int  Index;
        int  Id;
        char *Eep;
        char *Name;
        char *Desc;
        int  PIndex;
        int  PCount;
        char Data[EO_DATSIZ];
}
EO_DATA;

void EoSignalAction(int signo, void (*func)(int));
void ExamineEvent(int Signum, siginfo_t *ps, void *pt);
char *EoMakePath(char *Dir, char *File);
INT EoReflesh(void);
EO_DATA *EoGetDataByIndex(int Index);

typedef FILE* HANDLE;
typedef char TCHAR;
typedef int BOOL;

enum EventStatus {
	NoEntry = 0,
	NoData = 1,
	DataExists = 2
};

enum EventStatus PatrolTable[NODE_TABLE_SIZE];

static int running;
static char *PidPath;
static char *BrokerPath;

//
//
//
void EoSignalAction(int signo, void (*func)(int))
{
	struct sigaction act, oact;

	if (signo == SIGENOCEAN) {
		act.sa_sigaction = (void(*)(int, siginfo_t *, void *)) func;
		sigemptyset(&act.sa_mask);
		act.sa_flags = SA_SIGINFO;
	}
	else {
		act.sa_handler = func;
		sigemptyset(&act.sa_mask);
		act.sa_flags = SA_RESTART;
	}
	if (sigaction(signo, &act, &oact) < 0) {
		fprintf(stderr, "error at sigaction\n");
	}
}

void ExamineEvent(int Signum, siginfo_t *ps, void *pt)
{
	int index;
	char message[6] = {">>>0\n"};
	index = (unsigned long) ps->si_value.sival_int;
	PatrolTable[index] = DataExists;
	message[3] = '0' + index;
	write(1, message, 6);
}

static void stopHandler(int sign)
{
	if (PidPath) {
		unlink(PidPath);
	}
	running = 0;
}


int main(int ac, char **av)
{
	int i, nodeCount;
	pid_t myPid = getpid();
	FILE *f;
	EO_DATA *pe;

    PidPath = EoMakePath(EO_DIRECTORY, AZ_PID_FILE);
    f = fopen(PidPath, "w");
    if (f == NULL)
    {
        fprintf(stderr, ": cannot create pid file=%s\n",
			PidPath);
        return 1;
    }
    fprintf(f, "%d\n", myPid);
    fclose(f);

    BrokerPath = EoMakePath(EO_DIRECTORY, AZ_BROKER_FILE);
    f = fopen(BrokerPath, "w");
    if (f == NULL)
    {
        fprintf(stderr, ": cannot create broker file=%s\n",
			BrokerPath);
        return 1;
    }
    fprintf(f, "azure\r\n");
    fclose(f);

    printf("PID=%d file=%s proker=%s\n", myPid, PidPath, BrokerPath);

	signal(SIGINT, stopHandler); /* catches ctrl-c */
	signal(SIGTERM, stopHandler); /* catches kill -15 */
	EoSignalAction(SIGENOCEAN, (void(*)(int)) ExamineEvent);

	nodeCount = EoReflesh();
	for(i = 0; i < nodeCount; i++) {
		PatrolTable[i] = NoData;
	}

	printf("nodeCount = %d\n", nodeCount);
	running = 1;

	while(running) {
		for(i = 0; i < nodeCount; i++) {
			if (PatrolTable[i] == NoEntry) {
				break;
			}
			else if (PatrolTable[i] == NoData) {
				continue;
			}

			while((pe = EoGetDataByIndex(i)) != NULL) {
				printf("%d: %d: %08X [%s] <%s><%s> <%s>\n",
					pe->Index,
					pe->PIndex,
					pe->Id,
					pe->Eep,
					pe->Name,
					pe->Desc,
					//pe->PCount,
					pe->Data);
			}
			PatrolTable[i] = NoData;
		}
		sleep(1);
	}
	
	return 0;
}
