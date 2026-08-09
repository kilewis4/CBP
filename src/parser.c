// Unpacks fields, validates packets, decodes payloads

#include <stdint.h>
#include "protocol.h"
#include "parser.h"
#include "checksum.h"

int parse_protocol(uint8_t *raw_bytes, uint8_t buf_size, struct protocol *p)
{
    // Validate the input

    if(buf_size < 3) // Minimum size: header(2 bytes) + checksum(1 byte)
    {
        return -1; //Buffer too small
    }

    // Reconstruct the 16-bit header from the first two bytes

    uint16_t packed_header = (raw_bytes[0] << 8) | (raw_bytes[1]);

    // Extract each field into the struct

    p->version = extract_version(packed_header);
    p->type = extract_type(packed_header);
    p->flags = extract_flags(packed_header);
    p->payload_length = extract_payload_length(packed_header);

    // Validate the payload length

    if(buf_size < 2 + p->payload_length + 1) // header(2 bytes) + payload + checksum(1 byte)
    {
        return -2; //Invalid payload length
    }

    // Copy the payload bytes into the struct

    for(int i = 0; i < p->payload_length; i++)
    {
        p->payload[i] = raw_bytes[2 + i]; 
    }

    uint8_t packet_size = 2 + p->payload_length + 1;

    // Verify the checksum

    if(compute_checksum(raw_bytes, buf_size - 1) != raw_bytes[packet_size - 1])
    {
        return -3; //Checksum mismatch
    }

    // Return 0 on success, negative on failure
    return 0;
}
