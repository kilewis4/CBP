# CBP
### Custom Binary Protocol 

A low-level systems programming project written in C that implements a custom binary communication protocol, packet encoder, and packet parser.

This project explores:

* Bitwise operations
* Packet serialization/deserialization
* Binary data layouts
* Packed flags and bit fields
* Checksums and validation
* Endianness considerations
* Defensive parsing techniques

The goal of the project is to better understand how real-world systems software, networking protocols, embedded firmware, and binary formats work internally.

---

# Features

## Packet Encoding

Builds structured packets into compact binary representations.

## Packet Parsing

Decodes raw binary data back into structured packet fields.

## Packed Bit Fields

Uses masks and bitwise operations to efficiently store:

* Version information
* Packet types
* Flags
* Payload lengths

## Validation

Detects malformed or invalid packets.

## Hexdump Utility

Displays packet data in a human-readable hexadecimal format.

## Checksum Support

Implements packet integrity verification.

---

# Example Packet Layout

16-bit packet header:

| Field          | Size    |
| -------------- | ------- |
| Version        | 4 bits  |
| Packet Type    | 4 bits  |
| Flags          | 4 bits  |
| Payload Length | 4 bits  |

Example flag definitions:

| Bit | Meaning      |
| --- | ------------ |
| 0   | ACK Required |
| 1   | Encrypted    |
| 2   | Compressed   |
| 3   | Urgent       |

---

# Project Structure

```text
CBP/
│
├── include/
│   ├── protocol.h
│   ├── packet.h
│   ├── parser.h
│   ├── encoder.h
│   ├── checksum.h
│   └── utils.h
│
├── src/
│   ├── main.c
│   ├── packet.c
│   ├── parser.c
│   ├── encoder.c
│   ├── checksum.c
│   └── utils.c
│
├── tests/
│   ├── test_parser.c
│   ├── test_encoder.c
│   └── test_checksum.c
│
├── tools/
│   └── hexdump.c
│
├── docs/
│   └── protocol_spec.md
│
├── Makefile
└── README.md
```

---

# Technologies

* C
* GCC
* Make
* Standard Library

---

# Status

Planned