#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> //offsefof
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <linux/limits.h> //PATH_MAX
#include <errno.h>
#include <sys/stat.h> //stat

#ifdef EXTERNAL_BROKER
#include "../dpride/typedefs.h"
#include "../dpride/dpride.h"
#else
#include "typedefs.h"
#include "dpride.h"
#endif

#ifndef EO_LOG_DIRECTORY
#define EO_LOG_DIRECTORY "/var/tmp/dpride/logs"
#endif

#ifndef EO_DEFAULT_LOGNAME
#define EO_DEFAULT_LOGNAME "eo";
#endif

#ifndef EO_DEFAULT_EXTENSION
#define EO_DEFAULT_EXTENSION ".log";
#endif

//
//
//
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define __LITTLE_ENDIAN__
#elif __BYTE_ORDER == __BIG_ENDIAN
#define __BIG_ENDIAN__
#endif

static FILE *eologf;
static int DebugLog = 0;

static char logFileName[PATH_MAX];
static char savePrefix[32];
static char saveExtension[16];

//
//
//
static char *MakeLogFileName(char *Prefix, char *Postfix, char *p)
{
        time_t      timep;
        struct tm   *time_inf;
        const char bufLen = 20;
        char buf[PATH_MAX];

        if (p == NULL) {
                p = calloc(strlen(Prefix) + strlen(Postfix) + bufLen, 1);
        }
        timep = time(NULL);
        time_inf = localtime(&timep);
        strftime(buf, bufLen, "%Y%m%d-%H%M%S", time_inf);
        //printf("%s-%s.%s\n", Prefix, buf, Postfix);
        sprintf(p, "%s/%s-%s%s", EO_LOG_DIRECTORY, Prefix, buf, Postfix);

        return p;
}

static char *MakeLogFileNameHourly(char *Prefix, char *Postfix, char *p)
{
        time_t      timep;
        struct tm   *time_inf;
        const char bufLen = 20;
        char buf[PATH_MAX];

        if (p == NULL) {
                p = calloc(strlen(Prefix) + strlen(Postfix) + bufLen, 1);
        }
        timep = time(NULL);
        time_inf = localtime(&timep);
        strftime(buf, bufLen, "%Y%m%d-%H0000", time_inf);
        //printf("%s-%s.%s\n", Prefix, buf, Postfix);
        sprintf(p, "%s/%s-%s%s", EO_LOG_DIRECTORY, Prefix, buf, Postfix);

        return p;
}

FILE *EoLogInitBase(char *Prefix, char *Extension,
		    char *(*MakeFileNameFunc)(char *Prefix, char *Postfix, char *p))
{
	struct stat sb;
	int rtn;
	const char *logDirectory = EO_LOG_DIRECTORY;
	
	if (Prefix == NULL || *Prefix == '\0' ) {
		Prefix = EO_DEFAULT_LOGNAME;
	}
	if (Extension == NULL || *Extension == '\0' ) {
		Extension = EO_DEFAULT_EXTENSION;
	}
	(void) (*MakeFileNameFunc)(Prefix, Extension, logFileName);

        rtn = stat(logDirectory, &sb);
	if (rtn < 0){
		mkdir(logDirectory, 0777);
	}
	rtn = stat(logDirectory, &sb);
	if (!S_ISDIR(sb.st_mode)) {
		fprintf(stderr, "EoLogInit: Directory error=%s\n", logDirectory);
		return NULL;
	}
	
	//printf("<<%s>>\n", logFileName);
        eologf = fopen(logFileName, "a+");
        if (eologf == NULL) {
                fprintf(stderr, "EoLogInit: cannot open logfile=%s\n", logFileName);
                return NULL;
        }
	// for recovery when file was deleted.
	strncpy(savePrefix, Prefix, 32);
	strncpy(saveExtension, Extension, 16);
        return eologf;
}

FILE *EoLogInit(char *Prefix, char *Extension)
{
	return(EoLogInitBase(Prefix, Extension, MakeLogFileName));
}

FILE *EoLogInitHourly(char *Prefix, char *Extension)
{
	return(EoLogInitBase(Prefix, Extension, MakeLogFileNameHourly));
}

//
//
//
static int CheckLogFile(void)
{
	struct stat sb;
	int rtn;

	rtn = stat(logFileName, &sb);
	if (rtn < 0) {
		//fprintf(stderr, "stat error=%s\n", logFileName);
		//perror("EoLog");
		eologf = EoLogInit(savePrefix, saveExtension);
		//return errno;
	}
	return 0;
}

void EoLog(char *id, char *eep, char *msg)
{
        time_t      timep;
        struct tm   *time_inf;
	const char null = '\0';
	enum {idBufferSize = 11, eepBufferSize = 31};
	char idBuffer[idBufferSize + 1];
	char eepBuffer[eepBufferSize + 1];
	char timeBuf[64];
	char buf[BUFSIZ / 4];

	(void) CheckLogFile();
	
	idBuffer[0] = null;
	eepBuffer[0] = null;

	if (id != NULL && *id != '\0') {
		strncpy(idBuffer, id, idBufferSize);
		idBuffer[idBufferSize] = null;
	}
	if (eep != NULL && *eep != '\0') {
		strncpy(eepBuffer, eep, eepBufferSize);
		eepBuffer[eepBufferSize] = null;
	}
	
        timep = time(NULL);
        time_inf = localtime(&timep);
        strftime(timeBuf, sizeof(timeBuf), "%x %X", time_inf);
        sprintf(buf, "%s,%s,%s,%s\r\n", timeBuf, idBuffer, eepBuffer, msg);

        fwrite(buf, strlen(buf), 1, eologf);
        fflush(eologf);
	
	if (DebugLog > 0) {
		//printf("Debug:%s,%s,%s,%s\n", timeBuf, idBuffer, eepBuffer, msg);
		printf("Debug:<%s>\n", buf);
	}
}

void EoLogRaw(char *Msg)
{
	(void) CheckLogFile();
	
	strcat(Msg, "\r\n");
        fwrite(Msg, strlen(Msg), 1, eologf);
        fflush(eologf);
	
	if (DebugLog > 0) {
		//printf("Debug:%s,%s,%s,%s\n", timeBuf, idBuffer, eepBuffer, msg);
		printf("Debug:<%s>\n", Msg);
	}
}

//
//
//
#ifdef UNITTEST
int main(int ac, char **av)
{
	char *module = NULL;
	char *action = NULL;
	char *msg = NULL;
	//char logname[BUFSIZ / 2];
	//char bytes[6];
	//int id = 0x12345678;
	int i;
	FILE *logf;
	//int sleepFlag = 0;
	//int times = 3;

	for(i = 1; i < ac; i++) {
		//if (!strcmp(av[i], "-s")) {
		//	sleepFlag++;
		//}
		//else if (isdigit(av[i][0])) {
		//	times = atoi(av[i]);
		//}
		if (!module) {
			module = av[i];
		}
		else if (!action) {
			action = av[i];
		}
		else {
			msg = av[i];
		}
	}

	//logf = EoLogInit(NULL, NULL);
	logf = EoLogInitHourly("azr", NULL);
	if (logf == NULL) {
                fprintf(stderr, ": cannot open logfile=%s\n", "azr-9999");
                return 0;
        }
#if 0
	for(i = 0; i < times; i++) {
		if (sleepFlag > 0) {
			sleep(3 * sleepFlag);
		}
		EoLog("01234567", "A5-02-04", msg);
		printf("%d\n", i);
	}
#endif
	EoLog(module, action ? action : "", msg ? msg : "");
	fclose(logf);
	
	return 0;
}
#endif //UNITTEST
