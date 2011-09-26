# RBFSUM utility
# Copyright (C) 2011 - Ardhan Madras <ajhwb@knac.com>

SOURCES      = gui.c callback.c station.c queue.c main.c
OBJS         = ${SOURCES:.c=.o}
CFLAGS       = -Wall -g -I/usr/include/pgsql `pkg-config --cflags gtk+-2.0`
LDFLAGS      = `pkg-config --libs gtk+-2.0`
GLIBCFLAGS   = `pkg-config --cflags glib-2.0`
GLIBLDFLAGS  = `pkg-config --libs glib-2.0`
PSQLLDFLAGS  = -lpq
PACKAGE      = rbfsum-util
CC           = gcc

${PACKAGE}: ${OBJS}
	${CC} -o ${PACKAGE} ${OBJS} ${LDFLAGS}

.c.o:
	${CC} ${CFLAGS} -c $<

xml: station.o queue.o rbfsum2xml.o
	${CC} ${GLIBLDFLAGS} queue.o station.o rbfsum2xml.o -o rbfsum2xml

rbfsum2db: station.o queue.o rbfsum2db.o
	${CC} ${PSQLLDFLAGS} ${GLIBLDFLAGS} station.o queue.o rbfsum2db.o -o rbfsum2db

clean:
	rm -rf *.o ${PACKAGE} rbfsum2db rbfsum2xml

