CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

all: main.o encoder.o checksum.o parser.o
	$(CC) $(CFLAGS) -o cbp main.o encoder.o checksum.o parser.o

main.o: src/main.c include/parser.h include/protocol.h include/checksum.h include/encoder.h
	$(CC) $(CFLAGS) -c -Iinclude src/main.c

encoder.o: src/encoder.c include/encoder.h include/checksum.h include/protocol.h
	$(CC) $(CFLAGS) -c -Iinclude src/encoder.c

checksum.o: src/checksum.c include/checksum.h
	$(CC) $(CFLAGS) -c -Iinclude src/checksum.c

parser.o: src/parser.c include/parser.h include/protocol.h include/checksum.h
	$(CC) $(CFLAGS) -c -Iinclude src/parser.c

clean:
	rm -f *.o cbp

help:
	@echo "Makefile for CBP project"
	@echo "Usage:"
	@echo "  make        - Compile the project"
	@echo "  make clean  - Remove compiled files"
	@echo "  make help   - Show this help message"