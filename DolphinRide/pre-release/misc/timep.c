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


typedef int BOOL;

typedef struct _eo {
	int Logger;
	int VFlags;
}
EO_CONTROL;

char EoLogMonitorMessage[BUFSIZ / 4];

EO_CONTROL EoControl = {1, 1};

//
//
void DebugPrint(char *p)
{
	printf("%s\n", p);
}

int MonitorMessage(void *p) { return 1; }

void LogMessageStart(uint Id, char *Eep)
{
        EO_CONTROL *p = &EoControl;
        time_t      timep;
        struct tm   *time_inf;
        char        buf[64];

        if (p->Logger > 0) {
                timep = time(NULL);
                time_inf = localtime(&timep);
                //strftime(buf, sizeof(buf), "%x %X", time_inf); // original
		strftime(buf, sizeof(buf), "%Y%m%d-%H%M%S", time_inf);
                if (Eep != NULL) {
                        sprintf(EoLogMonitorMessage, "%s,%08X,%s,", buf, Id, Eep);
                }
                else {
                        sprintf(EoLogMonitorMessage, "%s,%08X,", buf, Id);
                }
        }
}

void LogMessageMonitor(char *Eep, char *Message)
{
        EO_CONTROL *p = &EoControl;
        char buf[BUFSIZ / 4];
        if (p->Logger > 0) {
                sprintf(buf, "%s,%s", Eep ? Eep : NULL, Message);
                strcat(EoLogMonitorMessage, buf);
        }
}


void LogMessageOutput()
{
        EO_CONTROL *p = &EoControl;
        BOOL loggedOK;
        if (p->Logger > 0) {
                //Warn("call MonitorMessage()");
                loggedOK = MonitorMessage(EoLogMonitorMessage);
                DebugPrint(EoLogMonitorMessage);
                if (p->VFlags) {
                        printf("MsgOut: MonitorMessage=%s\n",
                               loggedOK ? "OK" : "FAILED");
                }
        }
}

char *MakeLogFileName(char *Prefix, char *Postfix, char *p)
{
        time_t      timep;
        struct tm   *time_inf;
	const char bufLen = 20;
	char buf[bufLen];

	if (p == NULL) {
		p = calloc(strlen(Prefix) + strlen(Postfix) + bufLen, 1);
	}
	timep = time(NULL);
	time_inf = localtime(&timep);
	strftime(buf, bufLen, "%Y%m%d-%H%M%S", time_inf);
	printf("%s-%s.%s\n", Prefix, buf, Postfix);
	sprintf(p, "%s-%s.%s", Prefix, buf, Postfix);

	return p;
}

int main()
{
	EO_CONTROL *p = &EoControl;
	char *buf;
	
	LogMessageStart(0x1234567, "EEP");
	LogMessageMonitor("eep", "mes");
	LogMessageOutput();

	buf = MakeLogFileName("XX", "log", NULL);

	printf("file=<%s>\n", buf);
		
	return 0;
}
