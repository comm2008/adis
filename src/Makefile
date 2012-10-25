# Makefile for adis
include ../Makefile.inc

objs := $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: all
all : ${EXEC}

${EXEC} : ${objs}
	${CC} ${CFLAGS} -o ${EXEC} ${objs}

.PHONY: clean
clean:
	@rm -f ${EXEC} ${objs}

