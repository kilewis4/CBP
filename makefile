CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

BUILD = build
BIN = bin

all: $(BIN)/cbp

$(BIN)/cbp: $(BUILD)/main.o $(BUILD)/encoder.o $(BUILD)/checksum.o $(BUILD)/parser.o $(BUILD)/utils.o | $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/main.o: src/main.c include/parser.h include/protocol.h include/checksum.h include/encoder.h include/utils.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/main.c -o $@

$(BUILD)/encoder.o: src/encoder.c include/encoder.h include/checksum.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/encoder.c -o $@

$(BUILD)/checksum.o: src/checksum.c include/checksum.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/checksum.c -o $@

$(BUILD)/parser.o: src/parser.c include/parser.h include/protocol.h include/checksum.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/parser.c -o $@

$(BUILD)/utils.o: src/utils.c include/utils.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/utils.c -o $@

# --- Tests ---

$(BUILD)/test_checksum.o: tests/test_checksum.c tests/test_checksum.h include/checksum.h include/encoder.h include/parser.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_checksum.c -o $@

$(BUILD)/test_encoder.o: tests/test_encoder.c tests/test_encoder.h include/encoder.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_encoder.c -o $@

$(BUILD)/test_main.o: tests/test_main.c tests/test_checksum.h tests/test_encoder.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_main.c -o $@

test: $(BIN)/test_runner

$(BIN)/test_runner: $(BUILD)/encoder.o $(BUILD)/checksum.o $(BUILD)/parser.o $(BUILD)/utils.o $(BUILD)/test_checksum.o $(BUILD)/test_encoder.o $(BUILD)/test_main.o | $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD) $(BIN):
	mkdir -p $@

clean:
	rm -rf $(BUILD) $(BIN)

help:
	@echo "Makefile for CBP project"
	@echo "Usage:"
	@echo "  make        - Compile the project"
	@echo "  make test   - Build the test runner"
	@echo "  make clean  - Remove compiled files"
	@echo "  make help   - Show this help message"