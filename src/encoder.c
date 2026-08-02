// Packs fields, builds packets, serializes payloads

#include <stdint.h>
#include "encoder.h"
#include "checksum.h"

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

    packed_header = (p.version << 12) | (p.type << 8) | (p.flags << 4) | p.payload_length;

    //Write the header bytes into the output buffer
    uint8_t total_bytes_written = 0;

    raw_bytes[0] = packed_header >> 8;
    total_bytes_written++;
    raw_bytes[1] = packed_header & 0xFF;
    total_bytes_written++;

    //Copy the payload into the output buffer
    for(int i = 0; i < p.payload_length; i++)
    {
        raw_bytes[2 + i] = p.payload[i]; 
        total_bytes_written++;
    }

    //Compute and append the checksum

    raw_bytes[total_bytes_written] = compute_checksum(raw_bytes, total_bytes_written); // Exclude the checksum byte itself
    total_bytes_written++;

    //Return the total number of bytes written to the output buffer

    return total_bytes_written;
}
