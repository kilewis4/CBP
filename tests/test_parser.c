#include <stdio.h>
#include <stdint.h>
#include "checksum.h"
#include "protocol.h"
#include "encoder.h"
#include "parser.h"

uint8_t round_trip_test() // Will encode a packet, then decode it and assert every field of the resulting struct matches
{
    struct protocol round_trip_packet;
    round_trip_packet.version = 1;
    round_trip_packet.type = 2;
    round_trip_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    round_trip_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    int written = encode_protocol(round_trip_packet, buffer, sizeof(buffer));

    int result = parse_protocol(buffer, 
                                written,
                                &round_trip_packet);

    if(round_trip_packet.version != 1 ||
       round_trip_packet.type != 2 ||
       round_trip_packet.flags != (ACK_REQUIRED_FLAG | ENCRYPTED_FLAG) ||
       round_trip_packet.payload_length != 0 ||
       result != 0)
    {
        printf("Error: Round trip test failed\n");
        return 0; // Test failed
    }
    return 1; // Test passed
}


uint8_t oversized_buffer_test() //Encode a small packet into a buffer sized as max_packet_size, then parse it with full max_packet_size as buf_size. 
{
    struct protocol oversized_buffer_packet;
    oversized_buffer_packet.version = 1;
    oversized_buffer_packet.type = 2;
    oversized_buffer_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    oversized_buffer_packet.payload_length = 2;
    oversized_buffer_packet.payload[0] = 0xAA;
    oversized_buffer_packet.payload[1] = 0xBB;
    uint8_t buffer[max_packet_size];

    int written = encode_protocol(oversized_buffer_packet, buffer, sizeof(buffer));

    struct protocol parsed_packet;
    int result = parse_protocol(buffer, 
                                written, 
                                &parsed_packet);

    if(result != 0)
    {
        printf("Error: oversized buffer test failed to parse, got %d\n", result);
        return 0;
    }

    return parsed_packet.version == 1 &&
           parsed_packet.type == 2 &&
           parsed_packet.flags == (ACK_REQUIRED_FLAG | ENCRYPTED_FLAG) &&
           parsed_packet.payload_length == 2 &&
           parsed_packet.payload[0] == 0xAA &&
           parsed_packet.payload[1] == 0xBB;
}

uint8_t buffer_too_small_test() // Buffer too small, test the boundary
{
    struct protocol small_buffer_packet;
    small_buffer_packet.version = 1;
    small_buffer_packet.type = 2;
    small_buffer_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    small_buffer_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    int written = encode_protocol(small_buffer_packet, buffer, sizeof(buffer));

    if(written < 0)
    {
        printf("Error: Encoding failed, got %d\n", written);
        return 0; // Test failed
    }

    struct protocol parsed_packet;

    // One byte too short - should fail
    if(parse_protocol(buffer, 2, &parsed_packet) != -1)
    {
        printf("Error: Buffer too small test failed, expected -1 return\n");
        return 0;
    }

    // Exactly the minimum - should succeed
    if(parse_protocol(buffer, 3, &parsed_packet) != 0)
    {
        printf("Error: Buffer too small test failed, expected 0 return\n");
        return 0;
    }

    return 1; // Test passed
}

uint8_t payload_length_exceeds_buffer_test() // Header claims payload_length is larger than what buf_size has room for, confirm -2 return.
{
    uint8_t raw_bytes[3];
    raw_bytes[0] = 0x00; // version=0, type=0
    raw_bytes[1] = 0x0F; // flags=0, payload_length=15 (claims max payload)
    raw_bytes[2] = 0x00; // checksum byte — irrelevant, we should fail before reaching it

    struct protocol p;

    int result = parse_protocol(raw_bytes, sizeof(raw_bytes), &p);
 
    return result == -2;
}

uint8_t checksum_mismatch_test() //Validly encoded packet, flip a bit in the payload or header after encoding. Confirm we get -3 on parse
{
    struct protocol mismatch_packet;
    mismatch_packet.version = 1;
    mismatch_packet.type = 2;
    mismatch_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    mismatch_packet.payload_length = max_payload_size;
    uint8_t buffer[max_packet_size];

    encode_protocol(mismatch_packet, buffer, sizeof(buffer));

    buffer[0] = buffer[0] ^ 1; // Flip a bit in the header

    int result = parse_protocol(buffer, sizeof(buffer), &mismatch_packet);

    return result == -3;
}


uint8_t parse_bit_isolation_test() // Construct a header with known pattern and confirm fields
{
    struct protocol p;
    p.version = 1;
    p.type = 0;
    p.flags = 0; // 0xC
    p.payload_length = 0;
    uint8_t buffer[max_packet_size];
    struct protocol parsed_packet;

    int written = encode_protocol(p, buffer, sizeof(buffer));
    if(written < 0 || parse_protocol(buffer, written, &parsed_packet) != 0)
    {
        printf("Error: parse bit isolation setup failed on version field\n");
        return 0;
    }

    printf("Parsed packet: version=%d, type=%d, flags=%d, payload_length=%d\n", 
           parsed_packet.version, 
           parsed_packet.type, 
           parsed_packet.flags, 
           parsed_packet.payload_length);

    if(parsed_packet.version != 1 || 
        parsed_packet.type != 0 || 
        parsed_packet.flags != 0 || 
        parsed_packet.payload_length != 0)
    {
        printf("Error: parse bit isolation failed on version field\n");
        return 0;
    }

    p.version = 0; 
    p.type = 1;

    written = encode_protocol(p, buffer, sizeof(buffer));

    if(written < 0 || parse_protocol(buffer, written, &parsed_packet) != 0) 
    { 
        printf("Error: parse bit isolation setup failed on type field\n");
        return 0; 
    }

    if(parsed_packet.version != 0 || 
        parsed_packet.type != 1 || 
        parsed_packet.flags != 0 || 
        parsed_packet.payload_length != 0)
    {
        printf("Error: parse bit isolation failed on type field\n");
        return 0;
    }
 
    p.type = 0;
    p.flags = ACK_REQUIRED_FLAG; // 0x8

    written = encode_protocol(p, buffer, sizeof(buffer));

    if(written < 0 || parse_protocol(buffer, written, &parsed_packet) != 0) 
    { 
        printf("Error: parse bit isolation setup failed on flags field\n");
        return 0; 
    }

    if(parsed_packet.version != 0 || 
        parsed_packet.type != 0 || 
        parsed_packet.flags != ACK_REQUIRED_FLAG || 
        parsed_packet.payload_length != 0)
    {
        printf("Error: parse bit isolation failed on flags field\n");
        return 0;
    }

    p.flags = 0;
    p.payload_length = max_payload_size; // 0xF 

    written = encode_protocol(p, buffer, sizeof(buffer));

    if(written < 0 || parse_protocol(buffer, written, &parsed_packet) != 0) 
    { 
        printf("Error: parse bit isolation setup failed on payload_length field\n");
        return 0; 
    }

    if(parsed_packet.version != 0 || 
        parsed_packet.type != 0 || 
        parsed_packet.flags != 0 || 
        parsed_packet.payload_length != max_payload_size)
    {
        printf("Error: parse bit isolation failed on payload_length field\n");
        return 0;
    }

    return 1;
}


uint8_t round_trip_max_payload_test() //Same as round_trip_test but with payload_length = 15
{
    struct protocol round_trip_packet;
    round_trip_packet.version = 1;
    round_trip_packet.type = 2;
    round_trip_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    round_trip_packet.payload_length = max_payload_size;
    uint8_t buffer[max_packet_size];

    for(int i = 0; i < max_payload_size; i++)
    {
        round_trip_packet.payload[i] = i;
    }

    int written = encode_protocol(round_trip_packet, buffer, sizeof(buffer));

    int result = parse_protocol(buffer, 
                                written, 
                                &round_trip_packet);

    if(result != 0)
    {
        printf("Error: Round trip test failed to parse, got %d\n", result);
        return 0;
    }

    for(int i = 0; i < max_payload_size; i++)
    {
        if(round_trip_packet.payload[i] != i)
        {
            printf("Error: Round trip test failed on payload, index %d\n", i);
            return 0; // Test failed
        }
    }

    if(round_trip_packet.version != 1 ||
       round_trip_packet.type != 2 ||
       round_trip_packet.flags != (ACK_REQUIRED_FLAG | ENCRYPTED_FLAG) ||
       round_trip_packet.payload_length != max_payload_size)
    {
        printf("Error: Round trip test failed\n");
        return 0; // Test failed
    }


    return 1; // Test passed
}