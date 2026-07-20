// Packs fields, builds packets, serializes payloads

#include <stdint.h>
#include "encoder.h"
#include "checksum.c"

int encode_protocol(struct protocol p, uint8_t *raw_bytes, uint8_t buf_size)
{
    //Start with validating protocol

    if(p.flags > 0x0F) //Flag is a nibble
    {
        return -1; //Invalid flags
    }

    if(buf_size < 2 + p.payload_length + 1) // header(2 bytes) + payload + checksum(1 byte)
    {
        return -2; //Buffer too small
    }

    //Pack the header fields into the first two bytes
    uint16_t packed_header = 0;

    packed_header = (packed_header | p.version) << 12;
    packed_header = (packed_header | p.type) << 8;
    packed_header = (packed_header | p.flags) << 4;
    packed_header = packed_header | p.payload_length;

    //Write the header bytes into the output buffer
    uint8_t total_bytes_written = 0;

    raw_bytes[0] = packed_header >> 8;
    total_bytes_written++;
    raw_bytes[1] = packed_header & 0xFF;
    total_bytes_written++;

    //Copy the payload into the output buffer
    for(int i = 0; i < p.payload_length; i++)
    {
        p.payload[i] = raw_bytes[2 + i];
        total_bytes_written++;
    }

    //Compute and append the checksum

    compute_checksum(raw_bytes, buf_size - 1); // Exclude the checksum byte itself

    

    //Return the total number of bytes written to the output buffer

    return 0;
}
