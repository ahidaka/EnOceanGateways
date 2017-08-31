#

TARGET = dpride dptest eotest

CFLAGS = -g -Wall

all: $(TARGET)

dpride: dpride.c
	gcc $(CFLAGS) -I/usr/include/libxml2 dpride.c -o dpride -lxml2

dptest: dptest.c
	gcc $(CFLAGS) -o dptest dptest.c

eotest: eotest.c
	gcc $(CFLAGS) -o eotest eotest.c

clean:
	/bin/rm -f *.o *~ $(TARGET)
