#

TARGET = dpride dppoint ptable testmain models sample-broker eologfile secure

IFLAGS = -I/usr/include/libxml2
CFLAGS = -g -Wall -Wno-parentheses $(IFLAGS)

DPOBJS = dpride.o ptable.o node.o control.o serial.o esp3.o logger.o websocket.o models.o eologfile.o utils.o secure.o

all: $(TARGET)


dpride: $(DPOBJS)
	gcc $(CFLAGS) $(DPOBJS) -o dpride -lpthread -lxml2

sample-broker: node.o sample-broker.c EoControl.c
	gcc $(CFLAGS) sample-broker.c EoControl.c node.o -o sample-broker

ptable: ptable.c ptablemain.c ptable.h
	gcc $(CFLAGS) ptable.c ptablemain.c -o ptable -lxml2

testmain: testmain.o logger.o websocket.o
	gcc $(CFLAGS) testmain.o logger.o websocket.o -o testmain -lpthread

secure: secure.c secure.h eoif.c eoif.h dpride.h
	gcc $(CFLAGS) -DNEED_MAIN secure.c -o secure

dpride.o: dpride.c dpride.h ptable.h serial.h esp3.h models.h
	gcc $(CFLAGS) -c dpride.c

ptable.o: ptable.c dpride.h ptable.h
	gcc $(CFLAGS) -c ptable.c

node.o: node.c dpride.h
	gcc $(CFLAGS) -c node.c

eologfile.o: eologfile.c dpride.h
	gcc $(CFLAGS) -c eologfile.c

control.o: control.c dpride.h
	gcc $(CFLAGS) -c control.c

models.o: models.c common-model.c models.h typedefs.h
	gcc $(CFLAGS) -c models.c

serial.o: serial.c serial.h esp3.h
	gcc $(CFLAGS) -c serial.c

esp3.o: esp3.c esp3.h
	gcc $(CFLAGS) -c esp3.c

utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

secure.o: secure.c secure.h eoif.h eoif.c
	gcc $(CFLAGS) -c secure.c
	#gcc $(CFLAGS) -DSUPERSONIC_SECURE -c secure.c

dppoint: dppoint.c
	gcc $(CFLAGS) -o dppoint dppoint.c

models: models.c utils.o models.h typedefs.h
	gcc $(CFLAGS) -DBITTEST -o models models.c utils.o

eologfile: eologfile.c dpride.h
	gcc -DUNITTEST $(CFLAGS) -o eologfile eologfile.c

ptablemain.c: ptable.h

testmain.o: logger.h

logger.o: logger.c logger.h

websocket.o: aw-base64.h aw-sha1.h list.h websocket.h

install: dpride dppoint
	cp -p dpride dppoint eologfile /usr/local/bin

clean:
	/bin/rm -f *.o *~ $(TARGET)
