#

TARGET = dpride dptest eotest ptable comma scn profiler

IFLAGS = -I/usr/include/libxml2
CFLAGS = -g -Wall $(IFLAGS)

all: $(TARGET)

dpride: dpride.o ptable.o control.o
	gcc $(CFLAGS) dpride.o ptable.o control.o -o dpride -lxml2

ptable: ptable.c ptablemain.c ptable.h
	gcc $(CFLAGS) ptable.c ptablemain.c -o ptable -lxml2

dptest: dptest.c
	gcc $(CFLAGS) -o dptest dptest.c

eotest: eotest.c
	gcc $(CFLAGS) -o eotest eotest.c

dpride.o: dpride.c dpride.h ptable.h
	gcc $(CFLAGS) -c dpride.c

ptable.o: ptable.c dpride.h ptable.h
	gcc $(CFLAGS) -c ptable.c

control.o: control.c dpride.h
	gcc $(CFLAGS) -c control.c

comma.c: ptable.h dpride.h

ptablemain.c: ptable.h


scn: scn.c dpride.h
	gcc $(CFLAGS) -o scn scn.c

profiler:

clean:
	/bin/rm -f *.o *~ $(TARGET)
