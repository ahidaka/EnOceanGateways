#

TARGET = $(LIBS)

CC = gcc
AR = ar
IFLAGS = 
CFLAGS = -g -Wall -Wno-parentheses $(IFLAGS)

LIBS = eolib.a
TESTS = secure models
OBJS = models.o secure.o

INCDIR = ../dpride

all: $(TARGET)
	touch make.date

eolib.a:
	cp -p bin/`arch`/eolib.a .

secure: secure.c secure.h $(INCDIR)/dpride.h
	$(CC) $(CFLAGS) -DNEED_MAIN secure.c -o secure

models.o: models.c models.h $(INCDIR)/typedefs.h $(INCDIR)/ptable.h  $(INCDIR)/utils.h
	$(CC) $(CFLAGS) -DENGINEERING_FACT -c models.c

secure.o: secure.c secure.h
	$(CC) $(CFLAGS) -c secure.c

models: models.c models.h $(INCDIR)/typedefs.h $(INCDIR)/ptable.h  $(INCDIR)/utils.h $(INCDIR)/utils.o
	$(CC) $(CFLAGS) -DBITTEST -DENGINEERING_FACT -o models models.c $(INCDIR)/utils.o

clean:
	/bin/rm -f *.o *~ core $(TARGET) make.date

