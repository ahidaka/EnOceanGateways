#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
//#include <>
//#include <>

const unsigned long ff = (unsigned long)(-1);

long Min(int NumBit)
{
	int i;
	const unsigned long mask = ~1UL;
	unsigned long target = ff;
	const int keyBit = NumBit - 1;

	for(i = 0; i < keyBit; i++) {
		target &= mask << i;
		printf("%d: %ld(0x%08lX)\n", i, target, target);
	}

	return (long) target;
}

long Max(int NumBit)
{
	int i;
	const unsigned long mask = 1UL;
	unsigned long target = 0;
	const int keyBit = NumBit - 1;

	for(i = 0; i < keyBit; i++) {
		target |= mask << i;
		printf("%d: %ld(0x%08lX)\n", i, target, target);
	}

	return (long) target;
}

int main()
{
	int val;
	long max, min;

	printf("long=%d int=%d long long=%d\n",
	       (int) sizeof(long), (int) sizeof(int), (int) sizeof(long long));
	
	while(1) {
		val = 0;
		printf("? ");
		scanf("%d", &val);

		min = Min(val);
		max = Max(val);

		printf("min=%ld(0x%08lX) max=%ld(0x%08lX)\n",
		       min, min, max, max);
	}
	return 0;
}
