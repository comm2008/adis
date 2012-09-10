# Makefile for adis
include ../Makefile.inc

OBJECTS = common.o dataproc.o multi.o sync.o branch.o dt_block.o          \
          dt_single.o dt_extra.o dt_coproc.o rt_coproc.o dataop_coproc.o  \
          sw_interrupt.o misc.o main.o

.PHONY: all
all : ${EXEC}

${EXEC} : ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} -o ${EXEC}

common.o : common.c

dataproc.o : dataproc.c common.c

multi.o : multi.c common.c

sync.o : sync.c common.c

branch.o : branch.c common.c

dt_block.o : dt_block.c common.c

dt_single.o : dt_single.c common.c

dt_extra.o: dt_extra.c common.c

dt_coproc.o : dt_coproc.c common.c

rt_coproc.o : rt_coproc.c common.c

dataop_coproc.o: dataop_coproc.c common.c

sw_interrupt.o : sw_interrupt.c common.c

misc.o : misc.c common.c

main.o : dataproc.c multi.c sync.c branch.c dt_single.c          \
         dt_block.c dt_extra.c dt_coproc.c rt_coproc.c           \
         dataop_coproc.c sw_interrupt.c misc.o main.c

.PHONY: clean
clean:
	@rm -f ${EXEC} ${OBJECTS}

