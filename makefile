# makefile for adis

CC = gcc
CFLAGS = -g -Wall
OBJECTS = common.o dataproc.o multi.o dataswap.o branch.o sdt.o swi.o predicates.o main.o

EXEC = adis

all : ${EXEC}

${EXEC} : ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${EXEC}

predicates.o : predicates.h

common.o : common.h

dataproc.o : dataproc.h common.h

multi.o : multi.h common.h

dataswap.o : dataswap.h common.h

branch.o : branch.c common.h

sdt.o : sdt.h common.h

swi.o : swi.c common.h

main.o : predicates.h dataproc.h multi.h dataswap.h branch.h sdt.h swi.h main.c

clean:
	rm ${EXEC} ${OBJECTS}
