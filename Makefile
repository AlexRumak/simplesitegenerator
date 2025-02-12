INCLUDE_DIRS =
LIB_DIRS =
CC=gcc

CDEFS=
CFLAGS= -O3 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lpthread

PRODUCT=simplesitegenerator

HFILES=
CFILES= simplesitegenerator.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~ *.d
	-rm -f ${PRODUCT} ${GARBAGE}

simplesitegenerator:	simplesitegenerator.o
			$(CC) $(LDFLAGS) $(CFLAGS) -o $@ simplesitegenerator.o $(LIBS)

depend:

.c.o:
	$(CC) -MD $(CFLAGS) -c $<
