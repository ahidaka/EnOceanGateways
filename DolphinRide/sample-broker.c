#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#ifdef EXTERNAL_BROKER
#include "../dprode/typedefs.h"
#include "../dprode/utils.h"
#include "../dprode/dpride.h"
#else
#include "typedefs.h"
#include "utils.h"
#include "dpride.h"
#endif


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

INT EoReflesh(void);
EO_DATA *EoGetDataByIndex(int Index);

int main(int ac, char **av)
{
	int i, nodeCount;
	EO_DATA *pe;
	
	while(1) {
		nodeCount = EoReflesh();
		for(i = 0; i < nodeCount; i++) {
			pe = EoGetDataByIndex(i);
			printf("%d: %08X [%s] <%s><%s> %d %d <%s>\n",
			       pe->Index,
			       pe->Id,
			       pe->Eep,
			       pe->Name,
			       pe->Desc,
			       pe->PIndex,
			       pe->PCount,
			       pe->Data);
		}
		sleep(10);
	}
	
	return 0;
}
