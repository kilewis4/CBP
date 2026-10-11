# --- Flags ---
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

BUILD = build
BIN = bin

# --- Main ---
all: $(BIN)/cbp $(BIN)/sender $(BIN)/receiver

$(BIN)/cbp: $(BUILD)/main.o $(BUILD)/encoder.o $(BUILD)/checksum.o $(BUILD)/parser.o $(BUILD)/utils.o $(BUILD)/protocol.o | $(BIN)
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

$(BUILD)/protocol.o: src/protocol.c include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/protocol.c -o $@

# --- Sockets ---
$(BUILD)/transport.o: src/transport.c include/transport.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/transport.c -o $@

$(BUILD)/sender.o: src/sender.c include/sender_utils.h include/transport.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/sender.c -o $@

$(BUILD)/receiver.o: src/receiver.c include/transport.h include/protocol.h include/parser.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/receiver.c -o $@

$(BIN)/sender: $(BUILD)/sender.o $(BUILD)/sender_utils.o $(BUILD)/transport.o $(BUILD)/encoder.o $(BUILD)/checksum.o $(BUILD)/utils.o $(BUILD)/protocol.o | $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/receiver: $(BUILD)/receiver.o $(BUILD)/transport.o $(BUILD)/parser.o $(BUILD)/checksum.o $(BUILD)/utils.o $(BUILD)/protocol.o | $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/sender_utils.o: src/sender_utils.c include/sender_utils.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude src/sender_utils.c -o $@

# --- Tests ---

$(BUILD)/test_checksum.o: tests/test_checksum.c tests/test_checksum.h include/checksum.h include/encoder.h include/parser.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_checksum.c -o $@

$(BUILD)/test_encoder.o: tests/test_encoder.c tests/test_encoder.h include/encoder.h include/protocol.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_encoder.c -o $@

$(BUILD)/test_parser.o: tests/test_parser.c tests/test_parser.h include/parser.h include/protocol.h  include/checksum.h include/encoder.h| $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_parser.c -o $@

$(BUILD)/test_main.o: tests/test_main.c tests/test_checksum.h tests/test_encoder.h tests/test_parser.h | $(BUILD)
	$(CC) $(CFLAGS) -c -Iinclude -Itests tests/test_main.c -o $@

test: $(BIN)/test_runner

$(BIN)/test_runner: $(BUILD)/encoder.o $(BUILD)/checksum.o $(BUILD)/parser.o $(BUILD)/utils.o $(BUILD)/protocol.o $(BUILD)/test_checksum.o $(BUILD)/test_encoder.o $(BUILD)/test_parser.o $(BUILD)/test_main.o | $(BIN)
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