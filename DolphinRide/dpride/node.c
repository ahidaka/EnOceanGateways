#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#ifdef EXTERNAL_BROKER
#include "../dpride/typedefs.h"
#include "../dpride/utils.h"
#include "../dpride/dpride.h"
#else
#include "typedefs.h"
#include "utils.h"
#include "dpride.h"
//#include "ptable.h"
#endif

//#define NODE_DEBUG 1

NODE_TABLE NodeTable[NODE_TABLE_SIZE];

//
//
//

NODE_TABLE *GetTableId(uint Target)
{
	bool found = false;
	int i;
	NODE_TABLE *nt = &NodeTable[0];

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
#if NODE_DEBUG
		printf("*N*%s:T=%08X N=%08X\n",
		       __FUNCTION__, Target, nt->Id);
#endif
		if (nt->Id == 0) {
			break;
		}
		else if (Target == nt->Id) {
#if NODE_DEBUG
			printf("*N*%s:Found T=%08X\n", __FUNCTION__, Target);
#endif
			found = true;
			break;
		}
		nt++;
	}
	return found ? nt : NULL;
}

bool CheckTableId(uint Target)
{
	NODE_TABLE *nt = GetTableId(Target);
	BOOL result = nt != NULL;
#if NODE_DEBUG
	printf("*N*%s:Target=%08X %s\n", __FUNCTION__, Target,
	       result ? "Found" : "Not found");
#endif
	return result;
}

void ClearTableId(void)
{
	int i;
	NODE_TABLE *nt = &NodeTable[0];

	for(i = 0; i < NODE_TABLE_SIZE; i++) {
		if (nt->Id == 0) {
			break;
		}
		else {
			nt->Id = 0;
		}
		nt++;
	}
}

int GetTableIndex(uint Target)
{
        NODE_TABLE *nt = &NodeTable[0];
        NODE_TABLE *targetNT;

#if NODE_DEBUG
	printf("*N*%s:Target=%08X\n", __FUNCTION__, Target);
#endif
	targetNT = GetTableId(Target);
	return (targetNT == NULL ? -1 : targetNT - nt);
}

//inline bool IsTerminator(char c)
bool IsTerminator(char c)
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
	char buf[BUFSIZ / 4];
	char *base;
	char *duped;
	char *pnext;
	int i;

#if NODE_DEBUG
	printf("*N* GetIem: p=%s\n", p); //DEBUG
#endif
	base = &buf[0];
	for(i = 0; i < BUFSIZ / 4; i++) {
		if (*p == ',' || IsTerminator(*p))
			break;
		*base++ = *p++;
	}
	pnext = p + (*p == ','); // if ',', forward one char
	*base = '\0';
	duped = strdup(buf);
	if (duped == NULL) {
		Error("duped == NULL");
	}
	else {
		*item = duped;
	}
	return pnext;
}

int DecodeLine(char *Line, uint *Id, char **Eep, char **Desc, char ***SCuts)
{
	char *p;
	char *item = NULL;
	int scCount = 0;
	char **scTable;
	int i;

	scTable = (char **) malloc(sizeof(char *) * SC_SIZE);
	if (scTable == NULL) {
		Error("cannot alloc scTable");
		return 0;
	}

	p = GetItem(DeBlank(Line), &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		IF_EXISTS_FREE(item);
		return 0;
	}
#if NODE_DEBUG
	printf("*N*0: <%s><%s>\n", item, p);  //DEBUG
#endif
	*Id = strtoul(item, NULL, 16);

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read EEP item");
		return 0;
	}
	p = GetItem(p, &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		IF_EXISTS_FREE(item);
		return 0;
	}
	*Eep = item;

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read Desc item");
		IF_EXISTS_FREE(item);
		return 0;
	}
	p = GetItem(p, &item);
	if (p == NULL || IsTerminator(*p) || *p == ',') {
		if (item)
			free(item);
		return 0;
	}
	*Desc = item;

	if ((p = CheckNext(p)) == NULL) {
		Error("cannot read SCut first item");
		IF_EXISTS_FREE(item);
		return 0;
	}
#if NODE_DEBUG
	printf("*N*%08X: %s=%s\n", *Id, *Eep, *Desc);
#endif
	for(i = 0; i < SC_SIZE; i++) {
		p = GetItem(p, &item);
		if (p == NULL) {
			IF_EXISTS_FREE(item);
			break;
		}
		scTable[i] = item;
#if NODE_DEBUG
		printf("*N* SC-%d:<%s>\n", i, item);
#endif
		if ((p = CheckNext(p)) == NULL) {
			//End of line
			break;
		}
	}
	*SCuts = (char **) scTable;
	scCount = i + 1;
	return scCount;
}

int ReadCsv(char *Filename)
{
	char buf[BUFSIZ / 4];
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
		Error("Open error");
		return 0;
	}
	while(true) {
		char *rtn = fgets(buf, BUFSIZ / 4, fd);
		if (rtn == NULL) {
#if NODE_DEBUG
			fprintf(stderr, "*N*fgets: EOF found\n");
#endif
			break;
		}
		eep = NULL;
		scCount = DecodeLine(buf, &id, &eep, &desc, &scs);
		if (scCount > 0) {
			////////
#if NODE_DEBUG
			int i;
#endif
			//if (nt->SCuts) {
			//	//purge old shortcuts
			//	; ////free(nt->SCuts);
			//}
			nt->Id = id;
			nt->Eep = eep;
			nt->Desc = desc;
			nt->SCuts = scs;
			nt->SCCount = scCount;
			if (lineCount >= NODE_TABLE_SIZE) {
				Error("Node Table overflow");
				break;
			}
			/////////
#if NODE_DEBUG
			printf("*N*ReadCsv %s=<%s>(%d)\n", nt->Eep, nt->Desc, nt->SCCount);
			for(i = 0; i < nt->SCCount; i++) {
				if (nt->SCuts[i] == NULL)
					break;
				printf("          %d:%s\n", i, nt->SCuts[i]);
			}
#endif			/////////			
		}
		nt++;
		lineCount++;
	}
	nt->Id = 0; //mark EOL
	fclose(fd);

	return lineCount;
}

