# CBP — Custom Binary Protocol

A systems programming project written in C that implements a custom binary communication protocol from the ground up.

CBP provides packet serialization and deserialization, binary header manipulation, checksum validation, and inter-process communication over POSIX Unix domain sockets.

The project focuses on understanding how communication protocols operate at a lower level, including how structured data is represented, transmitted, received, and validated as raw bytes.

**Key areas:** C programming · Binary protocols · Bitwise operations · Data serialization · POSIX sockets · Defensive parsing

---

## Technical Highlights

- **Binary serialization:** Manually packs structured packet fields into a compact binary representation using bitwise operations, masks, and shifts.
- **Packet parsing:** Reconstructs structured packet data from raw byte streams, with validation of packet lengths and integrity.
- **Data integrity:** Implements an XOR-based checksum calculated over the packet header and payload.
- **Defensive programming:** Validates buffer capacities, payload lengths, and checksums before accepting packets.
- **Socket programming:** Implements client/server communication using POSIX Unix domain sockets and `SOCK_STREAM`.
- **Reliable I/O:** Implements loop-based send and receive operations to account for partial transfers and interrupted system calls (`EINTR`).
- **Modular architecture:** Separates protocol definitions, encoding, parsing, transport, logging, and utility functions into independent modules.
- **Unit testing:** Includes tests for the encoder, parser, and checksum implementations.

## Protocol Specification

Each CBP packet consists of a 16-bit header, a variable-length payload, and a trailing 8-bit checksum.

```text
+----------------------+-------------------+-------------------+
| Header (16 bits)     | Payload (0–15 B)  | Checksum (8 bits) |
+----------------------+-------------------+-------------------+
```

### Header Layout

The 16-bit header contains four 4-bit fields:

| Field | Size | Description |
|---|---:|---|
| Version | 4 bits | Protocol version |
| Packet Type | 4 bits | Identifies packet type |
| Flags | 4 bits | Packet control flags |
| Payload Length | 4 bits | Payload size in bytes |

### Flag Definitions

| Bit | Flag | Mask |
|---:|---|---|
| 3 | ACK Required | `0x8` |
| 2 | Encrypted | `0x4` |
| 1 | Compressed | `0x2` |
| 0 | Urgent | `0x1` |

Header fields are packed and extracted using bitwise operations defined in `protocol.h`. The in-memory packet structure is kept separate from its serialized wire representation.

The header is serialized in big-endian byte order.

### Packet Constraints

- Header: 2 bytes
- Maximum payload: 15 bytes
- Checksum: 1 byte
- Maximum packet size: 18 bytes

The checksum is calculated using XOR across the serialized header and payload. The parser derives the expected packet size from the header rather than assuming the entire supplied buffer belongs to one packet.

---

## Architecture and Components

### Packet Encoder
Converts structured packet data into a serialized byte sequence. Packs header fields, copies the payload, and appends the calculated checksum.

### Packet Parser
Reconstructs packet structures from raw bytes. Validates minimum buffer size, payload length, and checksum integrity before accepting a packet.

### Checksum Module
Provides XOR-based checksum generation for packet integrity verification.

### Transport Layer
Implements local client/server communication using POSIX Unix domain sockets (`AF_UNIX`, `SOCK_STREAM`).

Includes:
- `cbp_server_start()` — Creates, binds, and listens on a server socket.
- `cbp_server_accept()` — Accepts incoming client connections.
- `cbp_client_connect()` — Establishes a client connection.
- `cbp_send_all()` — Loops until the requested data has been sent or an error occurs.
- `cbp_recv_exact()` — Receives the requested number of bytes, accounting for partial reads and interrupted system calls.

### Sender and Receiver
Two separate executables demonstrate communication using the CBP packet format.

- **Sender:** Reads packet definitions from a script file, constructs and encodes packets, and transmits them over a Unix domain socket.
- **Receiver:** Accepts a client connection, reads packet headers and payloads, validates received packets, and records transmission activity.

### Logging
A shared logging module provides consistent logging across the sender and receiver.

Records packet transmission, reception, and errors, with separate log files for each executable.

---

## Project Structure

```text
CBP/
├── include/
│   ├── checksum.h
│   ├── encoder.h
│   ├── logger.h
│   ├── parser.h
│   ├── protocol.h
│   ├── sender_utils.h
│   ├── transport.h
│   └── utils.h
│
├── src/
│   ├── checksum.c
│   ├── encoder.c
│   ├── logger.c
│   ├── main.c
│   ├── parser.c
│   ├── protocol.c
│   ├── receiver.c
│   ├── sender.c
│   ├── sender_utils.c
│   ├── transport.c
│   └── utils.c
│
├── tests/
│   ├── test_checksum.c
│   ├── test_checksum.h
│   ├── test_encoder.c
│   ├── test_encoder.h
│   ├── test_parser.c
│   ├── test_parser.h
│   └── test_main.c
│
├── usage/
├── docs/
├── Makefile
└── README.md
```

Generated build artifacts, executables, and runtime logs are excluded from the source tree above.

---

## Building and Testing

### Requirements

- Linux
- GCC
- GNU Make
- POSIX-compatible system calls and sockets

### Build

```bash
git clone https://github.com/kilewis4/CBP.git
cd CBP

make
```

Executables are generated in `bin/`, with intermediate object files stored in `build/`.

### Run Tests

```bash
make test
./bin/test_runner
```

The test suite covers packet encoding, parsing, and checksum functionality.

### Run Sender and Receiver

Start the receiver in one terminal:

```bash
./bin/receiver
```

In another terminal, run the sender with a packet script:

```bash
./bin/sender <script_file>
```

The sender connects to the receiver through a Unix domain socket and transmits the packets defined in the script.

---

## Technologies

| Technology | Purpose |
|---|---|
| C (C99) | Core implementation |
| GCC | Compilation |
| GNU Make | Build automation |
| POSIX API | Socket and system-level I/O |
| Unix Domain Sockets | Inter-process communication |
| Linux | Development and testing environment |

---

## Development Status

**Current milestone: Core protocol implementation and local communication.**

| Component | Status |
|---|---|
| Packet encoding | Complete |
| Packet parsing | Complete |
| XOR checksum | Complete |
| Packet validation | Complete |
| Unit tests | Implemented |
| Unix domain socket transport | Implemented |
| Sender | Implemented |
| Receiver | Implemented; single-client handling |
| Logging | Implemented |

### Roadmap

- [-] Sequence numbers and packet ordering(In progress)
- [ ] Multi-packet session management and protocol state machine
- [ ] Improved error handling and named error codes
- [ ] Stronger checksum algorithm
- [ ] Expanded protocol validation and test coverage
- [ ] Network transport using TCP/IP
- [ ] UART-based communication for embedded applications

---

*CBP is an ongoing exploration of systems programming, binary communication, and low-level software design in C.*