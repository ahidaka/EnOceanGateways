#

TARGET = dpride ptable

IFLAGS = -I/usr/include/libxml2
CFLAGS = -g -Wall $(IFLAGS)

all: $(TARGET)

dpride: dpride.o ptable.o control.o serial.o esp3.o
	gcc $(CFLAGS) dpride.o esp3.o ptable.o control.o serial.o -o dpride -lpthread -lxml2

ptable: ptable.c ptablemain.c ptable.h
	gcc $(CFLAGS) ptable.c ptablemain.c -o ptable -lxml2

dpride.o: dpride.c dpride.h ptable.h serial.h esp3.h
	gcc $(CFLAGS) -c dpride.c

ptable.o: ptable.c dpride.h ptable.h
	gcc $(CFLAGS) -c ptable.c

control.o: control.c dpride.h
	gcc $(CFLAGS) -c control.c

serial.o: serial.c serial.h esp3.h
	gcc $(CFLAGS) -c serial.c

esp3.o: esp3.c esp3.h
	gcc $(CFLAGS) -c esp3.c

ptablemain.c: ptable.h

clean:
	/bin/rm -f *.o *~ $(TARGET)
