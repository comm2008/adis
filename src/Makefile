# Makefile for adis
include ../Makefile.inc

OBJECTS = common.o dataproc.o multi.o dataswap.o branch.o dt_block.o dt_single.o \
          dt_coproc.o rt_coproc.o dataop_coproc.o sw_interrupt.o main.o

.PHONY: all
all : ${EXEC}

${EXEC} : ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${EXEC}

common.o : common.c

dataproc.o : dataproc.c common.c

multi.o : multi.c common.c

dataswap.o : dataswap.c common.c

branch.o : branch.c common.c

dt_block.o : dt_block.c common.c

dt_single.o : dt_single.c common.c

dt_coproc.o : dt_coproc.c common.c

rt_coproc.o : rt_coproc.c common.c

dataop_coproc.o: dataop_coproc.c common.c

sw_interrupt.o : sw_interrupt.c common.c

main.o : dataproc.c multi.c dataswap.c branch.c dt_single.c \
         dt_block.c dt_coproc.c rt_coproc.c dataop_coproc.c \
         sw_interrupt.c main.c

.PHONY: clean
clean:
	rm ${EXEC} ${OBJECTS}
