CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

ParseGPX: parse_gpx.o
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f *.o ParseGPX
