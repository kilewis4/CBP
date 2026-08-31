#include <stdio.h>
#include <stdint.h>
#include "checksum.h"
#include "protocol.h"
#include "encoder.h"

uint8_t round_trip_test() // Will encode a packet, then decode it and assert every field of the resulting struct matches
{
    struct protocol round_trip_packet;
    round_trip_packet.version = 1;
    round_trip_packet.type = 2;
    round_trip_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    round_trip_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    int written = encode_protocol(round_trip_packet, buffer, sizeof(buffer));

    parse_protocol(round_trip_packet, 
                   written,
                   &round_trip_packet);

    if(round_trip_packet.version != 1 ||
       round_trip_packet.type != 2 ||
        round_trip_packet.flags != (ACK_REQUIRED_FLAG | ENCRYPTED_FLAG) ||
        round_trip_packet.payload_length != 0)
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
    uint8_t buffer[max_packet_size];

    int written = encode_protocol(oversized_buffer_packet, buffer, sizeof(buffer));

    parse_protocol(oversized_buffer_packet, 
                  written, 
                  &oversized_buffer_packet);

    // not sure what the pass/fail condition is here
}

uint8_t buffer_too_small_test() // Buffer too small, test the boundary
{
    struct protocol small_buffer_packet;
    small_buffer_packet.version = 1;
    small_buffer_packet.type = 2;
    small_buffer_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    small_buffer_packet.payload_length = 2;
    uint8_t buffer[3];

    int written = encode_protocol(small_buffer_packet, buffer, sizeof(buffer));

    uint8_t result = parse_protocol(small_buffer_packet, 
                                    written, 
                                    &small_buffer_packet);

    if(result != -1)
    {
        printf("Error: Buffer too small test failed\n");
        return 0; // Test failed
    }

    uint8_t buffer[2]; // Buffer too small, test the boundary

    uint8_t result = parse_protocol(
                  encode_protocol(small_buffer_packet, buffer, sizeof(buffer)), 
                  written, 
                  &small_buffer_packet);

    return result == 0; // Test passed
}

uint8_t payload_length_exceeds_buffer_test() // Header claims payload_length is larger than what buf_size has room for, confirm -2 return.
{
    struct protocol small_buffer_packet;
    small_buffer_packet.version = 1;
    small_buffer_packet.type = 2;
    small_buffer_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    small_buffer_packet.payload_length = max_payload_size;
    uint8_t buffer[2];

    int written = encode_protocol(small_buffer_packet, buffer, sizeof(buffer));

    uint8_t result = parse_protocol(
                  written, 
                  &small_buffer_packet);
 
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

    uint8_t result = parse_protocol(buffer, sizeof(buffer), &mismatch_packet);

    return result == -3;
}


uint8_t bit_isolation_test() // Construct a header with known pattern and confirm fields
{
    struct protocol small_buffer_packet;
    small_buffer_packet.version = 1;
    small_buffer_packet.type = 2;
    small_buffer_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    small_buffer_packet.payload_length = max_payload_size;
    uint8_t buffer[max_packet_size];

    // TODO: I'm a bit confused here. Will come back later
}


uint8_t round_trip_max_payload_test() //Same as round_trip_test but with payload_length = 15
{
    struct protocol round_trip_packet;
    round_trip_packet.version = 1;
    round_trip_packet.type = 2;
    round_trip_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    round_trip_packet.payload_length = max_payload_size;
    uint8_t buffer[max_packet_size];

    parse_protocol(
                  encode_protocol(round_trip_packet, buffer, sizeof(buffer)), 
                  sizeof(buffer), 
                  &round_trip_packet);

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