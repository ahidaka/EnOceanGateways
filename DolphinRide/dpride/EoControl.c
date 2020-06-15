#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <linux/limits.h> //PATH_MAX
#include <stdbool.h>

#ifdef EXTERNAL_BROKER
#include "../dpride/typedefs.h"
#include "../dpride/utils.h"
#include "../dpride/dpride.h"
#else
#include "typedefs.h"
#include "utils.h"
#include "dpride.h"
#endif

extern NODE_TABLE NodeTable[];

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

char *EoMakePath(char *Dir, char *File);
INT EoReflesh(void);
EO_DATA *EoGetDataByIndex(int Index);

static EO_DATA EoData;
static int LastIndex;
static int LastPoint;

//
//
//
char *EoMakePath(char *Dir, char *File)
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

INT EoReflesh(void)
{
	int count;
	char *fname = EoMakePath(EO_DIRECTORY, EO_CONTROL_FILE);
	count = ReadCsv(fname);
	////free(fname);
	return count;
}

EO_DATA *EoGetDataByIndex(int Index)
{
        NODE_TABLE *nt = &NodeTable[Index];
	char *fileName;
	char *p;
        FILE *f;
	char buf[BUFSIZ];
        char *bridgePath;
	EO_DATA *pe = &EoData;
	int i;

	//printf("%s: %u:%u:id=%08X eep=%s cnt=%d\n", __FUNCTION__,
	//       Index, LastPoint, nt->Id, nt->Eep, /*nt->Desc,*/ nt->SCCount);

	if (Index != LastIndex || LastPoint > nt->SCCount) {
		// renew, from start
		LastPoint = 0;
		LastIndex = Index;
	}

	i = LastPoint;
	if (i == nt->SCCount) {
		LastPoint++;
		return NULL;
	}
	fileName = nt->SCuts[i];
	if (fileName == NULL || fileName[0] == '\0') {
		fprintf(stderr, "%s:%d fileName is NULL\n",
		       __FUNCTION__, i);
		LastPoint = 0;
		return NULL;
	}
	bridgePath = EoMakePath(EO_DIRECTORY, fileName);

	f = fopen(bridgePath, "r");
	if (f == NULL) {
		fprintf(stderr, "Cannot open=%s\n", bridgePath);
		LastPoint = 0;
		return NULL;
	}
	while(fgets(buf, BUFSIZ, f) != NULL) {
		if (isdigit(buf[0]))
			break;
	}
	fclose(f);
	// omit '\r' '\n' of line end
	p = strchr(buf, '\n');
	if (p != NULL)
		*p = '\0';
	p = strchr(buf, '\r');
	if (p != NULL)
		*p = '\0';
	//
	pe->Index = Index;
	pe->Id = nt->Id;
	pe->Eep = nt->Eep;
	pe->Name = fileName;
	pe->Desc = nt->Desc;
	pe->PIndex = i;
	pe->PCount = nt->SCCount;
	strncpy(pe->Data, buf, EO_DATSIZ);

	LastPoint++;

	return pe;
}
