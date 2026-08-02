CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

all: main.o encoder.o checksum.o parser.o utils.o
	$(CC) $(CFLAGS) -o cbp main.o encoder.o checksum.o parser.o utils.o

main.o: src/main.c include/parser.h include/protocol.h include/checksum.h include/encoder.h include/utils.h
	$(CC) $(CFLAGS) -c -Iinclude src/main.c

encoder.o: src/encoder.c include/encoder.h include/checksum.h include/protocol.h
	$(CC) $(CFLAGS) -c -Iinclude src/encoder.c

checksum.o: src/checksum.c include/checksum.h
	$(CC) $(CFLAGS) -c -Iinclude src/checksum.c

parser.o: src/parser.c include/parser.h include/protocol.h include/checksum.h
	$(CC) $(CFLAGS) -c -Iinclude src/parser.c

utils.o: src/utils.c include/utils.h
	$(CC) $(CFLAGS) -c -Iinclude src/utils.c

# --- Tests ---

test_checksum.o: tests/test_checksum.c tests/test_checksum.h include/checksum.h include/encoder.h include/parser.h include/protocol.h
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_checksum.c

test_main.o: tests/test_main.c tests/test_checksum.h
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_main.c

test: encoder.o checksum.o parser.o utils.o test_checksum.o test_main.o
	$(CC) $(CFLAGS) -o test_runner encoder.o checksum.o parser.o utils.o test_checksum.o test_main.o
	
clean:
	rm -f *.o cbp test_runner

help:
	@echo "Makefile for CBP project"
	@echo "Usage:"
	@echo "  make        - Compile the project"
	@echo "  make test   - Build and run tests"
	@echo "  make clean  - Remove compiled files"
	@echo "  make help   - Show this help message"