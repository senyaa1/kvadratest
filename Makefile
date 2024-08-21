CC=gcc
CFLAGS=
NAME=kvadratest

SRCDIR=src
OUTDIR=bin

main: ${OUTDIR}/${NAME}

prepare:
	mkdir -p ${OUTDIR} 
 
${OUTDIR}/${NAME}: ${SRCDIR}/*.c
	${CC} -o $@ $^ ${CFLAGS}

clean:
	rm -rf ${OUTDIR} 

all: prepare ${OUTDIR}/${NAME} 


.PHONY: all
