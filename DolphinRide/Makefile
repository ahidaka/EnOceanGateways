#

TARGET = dpride dptest eotest ptable

CFLAGS = -g -Wall

all: $(TARGET)

dpride: dpride.c ptable.c
	gcc $(CFLAGS) -I/usr/include/libxml2 dpride.c ptable.c -o dpride -lxml2

ptable: ptable.c
	gcc $(CFLAGS) -I/usr/include/libxml2 ptable.c ptablemain.c -o ptable -lxml2

dptest: dptest.c
	gcc $(CFLAGS) -o dptest dptest.c

eotest: eotest.c
	gcc $(CFLAGS) -o eotest eotest.c

dpride.c: dpride.h ptable.h

ptable.c: ptable.h

ptablemain.c: ptable.h

clean:
	/bin/rm -f *.o *~ $(TARGET)
