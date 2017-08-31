#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char **av)
{
	int mFlags = 1; //default
	int rFlags = 0;
	int oFlags = 0;
	int cFlags = 0;
	int vFlags = 0;
	int opt;
	int i;
	int timeout = 0;
	char *controlFile;
	char *bridgeDirectory;
	char *eepFile;

	while ((opt = getopt(ac, av, "mrocvf:d:t:e:")) != EOF) {
		switch (opt) {
		case 'm':
			mFlags++;
			rFlags = oFlags = 0;
			break;
		case 'r':
			rFlags++;
			mFlags = oFlags = 0;
			break;
		case 'o':
			oFlags++;
			mFlags = rFlags = 0;
			break;
		case 'c':
			cFlags++;
			break;
		case 'v':
			vFlags++;
			break;

		case 'f':
			controlFile = optarg;
			break;
		case 'd':
			bridgeDirectory = optarg;
			break;
		case 'e':
			eepFile = optarg;
			break;
		case 't': //timeout secs for register
			timeout = atoi(optarg);
			break;
		default: /* '?' */
			fprintf(stderr,
				"Usage: %s [-m|-r|-o][-c][-v][-d Directory][-f Controlfile][-e EEPfile\n"
				"    -m    Monitor mode\n"
				"    -r    Register mode\n"
				"    -o    Operation mode\n"
				"    -c    Clear settings before register\n"
				"    -v    View working status\n"
				"    -d    Bridge file directrory\n"
				"    -f    Control file\n"
				"    -e    EEP file\n"
				,av[0]);

			exit(EXIT_FAILURE);
		}
	}

	printf("mFlags=%d rFlags=%d oFlags=%d cFlags=%d timeout=%d optind=%d <%s><%s><%s>\n",
	       mFlags, rFlags, oFlags, cFlags, timeout, optind,
	       controlFile, bridgeDirectory, eepFile);

	for(i = optind; i < ac; i++) {
		printf("[%d]=%s\n", i, av[i]);
	}
	printf("name argument = %s\n", av[optind]);

	exit(EXIT_SUCCESS);
}
