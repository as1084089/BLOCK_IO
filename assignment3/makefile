CC=gcc
SRC=sources
EXEC=executables
OUT=outputs

all: io_generator parser

io_generator: ${SRC}/io_generator.c makefile
	${CC} ${SRC}/io_generator.c -o ${EXEC}/$@

parser: ${SRC}/parser.c makefile
	${CC} ${SRC}/parser.c -o ${EXEC}/$@

clean:
	rm -f ${OUT}/output.txt ${OUT}/.tmp

clean_all: clean
	rm -f ${EXEC}/parser ${EXEC}/io_generator
