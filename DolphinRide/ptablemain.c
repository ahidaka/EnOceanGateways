#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ptable.h"

int main(int argc, char *argv[])
{
	extern int debug;
	int printall = 0;

        int opt;
        char *eep = NULL;
        char *profile = "eep2.6.5.xml";

        while ((opt = getopt(argc, argv, "ade:")) != EOF) {
                switch (opt) {
                case 'a':
                        printall++;
                        break;
                case 'd':
                        debug++;
                        break;
                case 'e':
                        eep = optarg;
                        break;
                default: /* '?' */
                        fprintf(stderr, "Usage: %s: [-a][-d][-e eep]\n", argv[0]);
                        exit(EXIT_FAILURE);
                        break;
                }
        }

        //printf("optind=%d argc=%d\n", optind, argc);

        if (argv[optind] != NULL) {
                profile = argv[optind];
        }

        if (!InitEep(profile)) {
                fprintf(stderr, "InitEep error\n");
                exit(1);
        }

        if (printall) {
                PrintEepAll();
        }
        else if (eep) {
                PrintEep(eep);
        }

        //_D printf("end.\n");
        return 0;
}
