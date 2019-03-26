#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int main(int ac, char **av)
{
	int i, j, prime, opt, num;
	int from = 2, to = 1000, count = 0;
	char *trailer = "\n";

        while ((opt = getopt(ac, av, "nf:t:c:")) != EOF) {
                switch (opt) {
                case 'f':
			from = atoi(optarg);
                        break;

                case 't':
			to = atoi(optarg);
                        break;
			
                case 'c':
			count = atoi(optarg);
                        break;
			
                case 'n':
			trailer = " ";
			break;
			
                default: /* '?' */
                        fprintf(stderr, "Usage: %s [-f from][[-t to]|[-c count]][-n]\n",
                                av[0]);
                        exit(EXIT_FAILURE);
                }
        }

	if (count > 0) {
		to = 0x7FFFFFFF;
	}
	num = 0;
	for(i = 2; i <= to; i++) {
		prime = 1;
		for(j = 2; j < i; j++){
			if((i % j) == 0) {
				prime = 0;
				break;
			}
		}
		if(prime && i >= from) {
			num++;
			if (count == 0 || num <= count) {
				printf("%d%s", i, trailer);
				if (num == count)
					break;
			}
		}
	}
	return 0;
}
