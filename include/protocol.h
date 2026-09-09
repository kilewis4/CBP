// Defines packet layouts, masks, constants enums, protocol limits

#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdint.h>

#define ACK_REQUIRED_FLAG 0x8
#define ENCRYPTED_FLAG 0x4
#define COMPRESSED_FLAG 0x2
#define URGENT_FLAG 0x1

#define extract_version(x) (x >> 12 & 0x0F)
#define extract_type(x) (x >> 8 & 0x0F)
#define extract_flags(x) (x >> 4 & 0x0F)
#define extract_payload_length(x) (x & 0x0F)

#define max_payload_size 15
#define max_packet_size 18 // header(2 bytes) + payload(15 bytes) + checksum(1 byte)
#define header_size 2
#define checksum_size 1

struct protocol {
    uint8_t version; // bits 15-12
    uint8_t type; // bits 11-8
    uint8_t flags; // bits 7-4
    uint8_t payload_length; // bits 3-0
    uint8_t payload[max_payload_size];
    uint8_t checksum;
};

#endif