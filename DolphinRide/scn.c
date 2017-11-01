#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include "dpride.h"

#define SC_SIZE 16
#define NODE_TABLE_SIZE 256

//typedef int bool;
//#define false 0
//#define true 1

typedef struct _node_table {
	uint Id;
        char *Eep;
	char *Desc;
	int SCCount;
	char **SCuts;
} NODE_TABLE;

NODE_TABLE NodeTable[NODE_TABLE_SIZE];

/* EEP format
//String
//Packed
//Bin3

packed_eep = EepStringToPacked(str);
(bool) EepStringToBin3(str, b1, b2, b3);
(bool) EepBin3ToString();
(bool) EepBin3ToPacked();
(bool) EepPackedToBin3();
*/

inline void error() { fprintf(stderr, "ERROR!\n"); }

#if 0
char *tables[]  = {
	"ABC",
	"TMP",
	"HUM",
	"SCN",
	"FUT",
	"FUT1",
	"FUT2",
	"FUT4",
	"FUT3",
	"FUT5",
	"FUT6",
	"FUT7",
	"FUT8",
	"FUT9",
	"FUT10",
	"FUT12",
	"FUT13",
	"FUT22",
	"FUT23",
	NULL
};
#endif

char *MakeNewName(char *Original, int Suffix)
{
	int len = strlen(Original);
	char *buf;

	buf = malloc(len + 3);
	if (buf == NULL) {
		fprintf(stderr, "cannot alloc buffer\n");
		return NULL;
	}
	sprintf(buf, "%s%d",Original, Suffix);
	return buf;
}

bool CheckTableID(uint Target)
{
	bool collision = false;
	int i;
	NODE_TABLE *nt = &NodeTable[0];

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}

		if (Target == nt->Id) {
			collision = true;
			break;
		}
		nt++;
	}
	return collision;
}

bool CheckTableEEP(char *Target)
{
	bool collision = false;
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}

		ps = nt->SCuts;
		for(j = 0; j < nt->SCCount && *ps != NULL; j++) {
			//printf("\'%s\'\n", *ps++);
			if (!strcmp(Target, *ps++)) {
				//matched
				collision = true;
				break;
			}
		}
		if (collision)
			break;
		nt++;
	}
	return collision;
}

char *GetNewName(char *Target)
{
	int suffix;
	char *newName;
	char *origName;

	if (CheckTableEEP(Target)) {
		// Find collision
		origName = Target;
		Target = NULL; // Clear for new Name
		for(suffix = 1; suffix <= 99; suffix++) {
			newName = MakeNewName(origName, suffix);
			if (!CheckTableEEP(newName)) {
				Target = newName;
				printf("NEW:%s not mached\n", Target);
				break;
			}
		}
	}

	else 
		printf("ORG:%s not mached\n", Target);

	return Target;
}


inline bool IsTerminator(char c)
{
	return (c == '\n' || c == '\r' || c == '\0' || c == '#');
}

char *DeBlank(char *p)
{
	while(isblank(*p)) {
		p++;
	}


	return p;
}

char *CheckNext(char *p)
{
	if (IsTerminator(*p)) {
		// Oops, terminated suddenly
		return NULL;
	}
	p = DeBlank(p);
	if (IsTerminator(*p)) {
		// Oops, terminated suddenly again
		return NULL;
	}
	return p;
}

char *GetItem(char *p, char **item)
{
	char buf[BUFSIZ];
	char *base;
	char *duped;
	char *pnext;
	int i;

	printf("*** GetIem: p=%s\n", p); //DEBUG

	base = &buf[0];
	for(i = 0; i < BUFSIZ; i++) {
		if (*p == ',' || IsTerminator(*p))
			break;
		*base++ = *p++;
	}
	pnext = p + (*p == ','); // if ',', forward one char
	*base = '\0';
	duped = strdup(buf);
	if (duped == NULL) {
		error();
	}
	else {
		*item = duped;
	}
	return pnext;
}

int DecodeLine(char *Line, uint *Id, char **Eep, char **Desc, char ***SCuts)
{
	char *p;
	char *item;
	int scCount = 0;
	char **scTable;
	int i;

	scTable = (char **) malloc(sizeof(char *) * SC_SIZE);
	if (scTable == NULL) {
		fprintf(stderr, "cannot alloc scTable\n");
		error();
	}

	p = GetItem(DeBlank(Line), &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		return 0;
	}
	printf("**0: <%s><%s>\n", item, p);  //DEBUG
	*Id = strtoul(item, NULL, 16);

	if ((p = CheckNext(p)) == NULL) {
		fprintf(stderr, "cannot read EEP item\n");
		error();
	}
	p = GetItem(p, &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		return 0;
	}
	*Eep = item;

	if ((p = CheckNext(p)) == NULL) {
		fprintf(stderr, "cannot read Desc item\n");
		error();
	}
	p = GetItem(p, &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		return 0;
	}
	*Desc = item;

	if ((p = CheckNext(p)) == NULL) {
		fprintf(stderr, "cannot read SCut first item\n");
		error();
	}

	for(i = 0; i < SC_SIZE; i++) {
		p = GetItem(p, &item);
		if (p == NULL) {
			break;
		}
		scTable[i] = item;

		if ((p = CheckNext(p)) == NULL) {
			//End of line
			break;
		}
	}
	*SCuts = (char **) scTable;
	scCount = i + 1;
	return scCount;
}

int ReadCSV(char *Filename)
{
	char buf[BUFSIZ];
	FILE *fd;
	NODE_TABLE *nt;
	uint id;
	char *eep;
	char *desc;
	char **scs;
	int scCount;
	int lineCount = 0;

	nt = &NodeTable[0];

	fd = fopen(Filename, "r");
	if (fd == NULL) {
		fprintf(stderr, "open error\n");
		return 0;
	}

	//while(fgets(buf, BUFSIZ, fd) != NULL) {
	while(1) {
		char *rtn = fgets(buf, BUFSIZ, fd);
		if (rtn == NULL) {
			printf("*fgets: EOF found\n");
			break;
		}
		printf("*fgets:%s\n", rtn); //DEBUG

		scCount = DecodeLine(buf, &id, &eep, &desc, &scs);
		if (scCount > 0) {
			nt->Id = id;
			nt->Eep = eep;
			nt->Desc = desc;
			nt->SCuts = scs;
			nt->SCCount = scCount;
			nt++;
			lineCount++;
			if (lineCount >= NODE_TABLE_SIZE) {
				fprintf(stderr, "Node Table overflow");
				break;
			}
		}

	}
	nt->Id = 0; //mark EOL
	
	fclose(fd);

	return lineCount;
}

void PrintItems()
{
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}
		printf("%d: %08X %s <%s> ",
		       i, nt->Id, nt->Eep, nt->Desc);

		ps = nt->SCuts;
		for(j = 0; j < nt->SCCount && *ps != NULL; j++) {
			printf("\'%s\'", *ps++);
		}
		printf("\n");
		nt++;
	}
}

void PrintSCs()
{
	int i, j;
	NODE_TABLE *nt = &NodeTable[0];
	char **ps;

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}

		ps = nt->SCuts;
		for(j = 0; j < nt->SCCount && *ps != NULL; j++) {
			printf("\'%s\'\n", *ps++);
		}
		nt++;
	}
}

int main(int ac, char **av)
{
	char *key = "FUT";
	char *fname = "test.csv";
	char *newkey = "";

	if (ac > 1) {
		key = av[1];
	}
	if (ac > 2) {
		fname = av[2];
	}

	ReadCSV(fname);

	PrintItems(); // for debug

	//PrintSCs();
	newkey = GetNewName(key);
	printf("key: <%s><%s>\n", key, newkey);

	return 0;
}
