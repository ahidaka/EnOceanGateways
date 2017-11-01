#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

typedef int bool;
typedef unsigned int uint;

typedef struct _unit {
	char *SCut;

	int FormBit;
	int SizeBit;
	double Slope;
	double Differ;
} UNIT;

typedef struct _profiler
{
	union _eep {
		uint uint;
		char string[8];
	}  Eep;

	UNIT Unit[16];
} PROFILER;

PROFILER ptable[256];

int MakeProfile()
{
	int i;
	PROFILER *p;

	p = &ptable[0];

	for(i = 0; i < 256; i++) {
		p->Eep.uint = 0;
	}

	return 0;
}

void PrintProfile()
{
}

int main(int ac, char **av)
{
	MakeProfile();

	PrintProfile();

        return 0;
}
