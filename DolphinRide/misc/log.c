#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> //offsefof
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void EoLog(FILE *f, char *id, char *eep, char *msg)
{
        time_t      timep;
        struct tm   *time_inf;
	char idBuffer[12];
	char eepBuffer[12];
	char timeBuf[64];
	char buf[BUFSIZ];

	if (id)
		strcpy(idBuffer, id);
	if (eep)
		strcpy(eepBuffer, eep);
	
        timep = time(NULL);
        time_inf = localtime(&timep);
        strftime(timeBuf, sizeof(timeBuf), "%x %X", time_inf);
        sprintf(buf, "%s,%s,%s,%s\r\n", timeBuf, idBuffer, eepBuffer, msg);

        fwrite(buf, strlen(buf), 1, f);
}

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define __LITTLE_ENDIAN__
#elif __BYTE_ORDER == __BIG_ENDIAN
#define __BIG_ENDIAN__
#endif

static inline void IdToByte(char *p, unsigned int id)
{
	const unsigned int mask = 0xF;
	const char zero = '0';
#define mkchar(n, shift) ((((n) >> (shift)) & mask) + zero)
	if (p) {
		*p++ = mkchar(id, 28);
		*p++ = mkchar(id, 24);
		*p++ = mkchar(id, 20);
		*p++ = mkchar(id, 16);
		*p++ = mkchar(id, 12);
		*p++ = mkchar(id, 8);
		*p++ = mkchar(id, 4);
		*p++ = mkchar(id, 0);
		*p = '\0';
	}
#undef mkchar
}

int main(int ac, char **av)
{
#define EO_DIRECTORY "/var/tmp/dpride"
	char *LogFileName = "eo_etk.log";
	char *msg = "message";
	char logname[BUFSIZ / 2];
	FILE *logf;
	char bytes[6];
	int id = 0x12345678;
	int i;
	
	if (ac > 1) {
		msg = av[1];
	}

	strcpy(logname, EO_DIRECTORY);
	strcat(logname, "/");
	strcat(logname, LogFileName);

	logf = fopen(logname, "a+");
	if (logf == NULL) {
                fprintf(stderr, ": cannot open logfile=%s\n", logname);
                return 0;
        }

	for(i = 0; i < 3; i++) {
		EoLog(logf, "01234567", "A5-02-04", msg);
	}
	fclose(logf);

	IdToByte(bytes, (unsigned int)id);
	printf("id=%s(0x%08x)\n", bytes, id);
	
	return 0;
}
