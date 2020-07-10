all: generator parser

generator: source.c Makefile
	gcc source.c -o generator

parser: parser.c Makefile
	gcc parser.c -o parser

clean:
	rm -f output.txt tmp

clean_all: clean
	rm -f parser generator
