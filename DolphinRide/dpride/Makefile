#

TARGET = dpride dppoint ptable testmain sample-broker eologfile websocketmain

CC = gcc
IFLAGS = -I/usr/include/libxml2
CFLAGS = -g -Wall -Wno-parentheses -Wno-unused-function $(IFLAGS)

DPOBJS = dpride.o ptable.o node.o control.o serial.o esp3.o logger.o websocket.o eologfile.o utils.o
LIBDIR = ../eolib
EOLIB = $(LIBDIR)/eolib.a

all: $(TARGET)
	touch make.date

dpride: $(DPOBJS) $(EOLIB)
	$(CC) $(CFLAGS) $(DPOBJS) -o dpride $(EOLIB) -lpthread -lxml2

sample-broker: node.o sample-broker.c EoControl.c
	$(CC) $(CFLAGS) sample-broker.c EoControl.c node.o -o sample-broker

ptable: ptable.c ptablemain.c ptable.h
	$(CC) $(CFLAGS) ptable.c ptablemain.c -o ptable -lxml2

testmain: testmain.o logger.o websocket.o
	$(CC) $(CFLAGS) testmain.o logger.o websocket.o -o testmain -lpthread

dpride.o: dpride.c dpride.h ptable.h serial.h esp3.h $(LIBDIR)/models.h $(LIBDIR)/secure.h
	$(CC) $(CFLAGS) -c dpride.c

ptable.o: ptable.c dpride.h ptable.h
	$(CC) $(CFLAGS) -c ptable.c

node.o: node.c dpride.h
	$(CC) $(CFLAGS) -c node.c

eologfile.o: eologfile.c dpride.h
	$(CC) $(CFLAGS) -c eologfile.c

control.o: control.c dpride.h $(LIBDIR)/models.h
	$(CC) $(CFLAGS) -c control.c

serial.o: serial.c serial.h esp3.h
	$(CC) $(CFLAGS) -c serial.c

esp3.o: esp3.c esp3.h
	$(CC) $(CFLAGS) -c esp3.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

dppoint: dppoint.c
	$(CC) $(CFLAGS) -o dppoint dppoint.c

eologfile: eologfile.c dpride.h
	$(CC) -DUNITTEST $(CFLAGS) -o eologfile eologfile.c

ptablemain.c: ptable.h

testmain.o: logger.h

logger.o: logger.c logger.h

websocket.o: aw-base64.h aw-sha1.h list.h websocket.h

websocketmain: websocket.o
	$(CC) -o websocketmain main.c websocket.o -lpthread

install: dpride dppoint
	cp -p dpride dppoint eologfile /usr/local/bin

clean:
	/bin/rm -f *.o *~ $(TARGET) make.date
