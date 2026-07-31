// Declare and populate a protocol struct with some test values 
// Declare a uint8_t buffer of max_packet_size to encode into
// Call encode_protocol and check the return value
// Declare a second empty protocol struct for the parsed result
// Call parse_protocol on the buffer and check the return value
// Compare the fields of the original and parsed protocol structs to ensure they match

#include <stdio.h>
#include <stdint.h>
#include "protocol.h"
#include "encoder.h"
#include "parser.h"
#include "utils.h"



int main()
{
    struct protocol p;
    p.version = 1;
    p.type = 2;
    p.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    p.payload_length = 3;
    uint8_t buffer[max_packet_size];

    for (int i = 0; i < p.payload_length; i++)
    {
        p.payload[i] = i + 1; // Fill payload with some test data
    }


    if(encode_protocol(p, buffer, max_packet_size) < 0)
    {
        printf("Encoding failed\n");
        return -1;
    }

    hexdump(buffer, p.payload_length + 3); // header(2 bytes) + payload + checksum(1 byte)

    struct protocol p2;

    if(parse_protocol(buffer, max_packet_size, &p2) < 0)
    {
        printf("Parsing failed\n");
        return -1;
    }

    if(p.version == p2.version)
    {
        printf("Version matches\n");
    }
    else
    {
        printf("Version does not match\n");
        printf("Original: %d, Parsed: %d\n", p.version, p2.version);
    }

    if(p.type == p2.type)
    {
        printf("Type matches\n");
    }
    else
    {
        printf("Type does not match\n");
        printf("Original: %d, Parsed: %d\n", p.type, p2.type);
    }
    

    if(p.flags == p2.flags)
    {
        printf("Flags matches\n");
    }
    else
    {
        printf("Flags does not match\n");
        printf("Original: %d, Parsed: %d\n", p.flags, p2.flags);
    }

    if(p.payload_length == p2.payload_length)
    {
        printf("Length matches\n");
    }
    else
    {
        printf("Payload length does not match\n");
        printf("Original: %d, Parsed: %d\n", p.payload_length, p2.payload_length);
    }

    return 0;
}