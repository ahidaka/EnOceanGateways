#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> //offsefof
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "dpride.h"

void EoPoint(char *filname)
{
	int itemCount;
	char buffer[BUFSIZ];
	
	FILE *f = fopen(filname, "r");
	if (f == NULL) {
                fprintf(stderr, ": cannot open eofile=%s\n", filname);
                return;
        }

	while(fgets(buffer, BUFSIZ, f)) {
		int lastChar;
		char *p;
		char *pstart;
		
		p = buffer;
		itemCount = 0;
		do {
			pstart = p;
			while(1) {
				lastChar = *p;
				if (lastChar != ',' && lastChar != '\n' &&lastChar != '\r' && lastChar != '\0') {
					p++;
				}
				else break;
			}
			//printf("#%s#\n", p);
			itemCount++;
			if (itemCount > 3) {
				*p = '\0';
				//printf("last=%02X:<<%s>>\n", lastChar, pstart);
				printf("%s\n", pstart);
			}
			p++;
		}
		while(lastChar == ',');
	}
	fclose(f);
}

void usage(char *myname)
{
	fprintf(stderr, "%s: Usage [filter-file]\n", myname);
}

int main(int ac, char **av)
{
	char *describedPath = NULL;
	char fname[256];
	char *EoFileName = EO_CONTROL_FILE;

	if (ac > 1) {
		if (*av[1] == '-') {
			usage(av[0]);
			return 0;
		}
		else if (*av[1] == '/' || *av[1] == '.') {
			describedPath = av[1];
		}
		else {
			EoFileName = av[1];
		}
	}
	
	strcpy(fname, EO_DIRECTORY);
	strcat(fname, "/");
	strcat(fname, EoFileName);

	EoPoint(describedPath ? describedPath : fname);

	return 0;
}
