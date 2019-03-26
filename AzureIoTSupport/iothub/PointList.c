#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include "EoControl.c"

extern int EoReflesh(void);
extern char *EoGetPointByIndex(int Index);

int main(int ac, char **av)
{
	char *pe;
	int index;
	int i;

	EoReflesh(); // Read csv
	for(index = 0; index < NODE_TABLE_SIZE; index++)
	{
		i = 0;
		while ((pe = EoGetPointByIndex(index)) != NULL)
		{
			//printf("[%d]:%d=%s\r\n", index, i, pe);
			printf("%s\r\n", pe);
			i++;
		}
	}
	return 0;
}
