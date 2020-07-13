all: io_generator parser

io_generator: io_generator.c Makefile
	gcc io_generator.c -o io_generator

parser: parser.c Makefile
	gcc parser.c -o parser

clean:
	rm -f output.txt tmp

clean_all: clean
	rm -f parser io_generator
