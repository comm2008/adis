# makefile for adis

CC = gcc
CFLAGS = -g -Wall
OBJECTS = common.o dataproc.o multi.o dataswap.o branch.o bdt.o sdt.o swi.o predicates.o main.o

EXEC = adis

all : ${EXEC}

${EXEC} : ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${EXEC}

predicates.o : predicates.c

common.o : common.c

dataproc.o : dataproc.c common.c

multi.o : multi.c common.c

dataswap.o : dataswap.c common.c

branch.o : branch.c common.c

bdt.o : bdt.c common.c

sdt.o : sdt.c common.c

swi.o : swi.c common.c

main.o : predicates.c dataproc.c multi.c dataswap.c branch.c sdt.c swi.c main.c

clean:
	rm ${EXEC} ${OBJECTS}
