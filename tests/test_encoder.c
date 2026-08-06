// Buffer too small, invalid flags, Payload length exceeding max

#include <stdio.h>
#include <stdint.h>
#include "checksum.h"
#include "protocol.h"
#include "encoder.h"

uint8_t flag_test()
{
    struct protocol flag_test_packet;
    flag_test_packet.version = 1;
    flag_test_packet.type = 2;
    flag_test_packet.flags = 0x10; // Invalid nibble
    flag_test_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    uint8_t bytes_written = encode_protocol(flag_test_packet, buffer, sizeof(buffer));
    
    if(bytes_written < 0)
    {
        printf("Error encoding flag test packet: %d\n", bytes_written);
        return bytes_written; // Test failed
    }
    
    flag_test_packet.flags = 0x10; // Max valid nibble
    return -1 != encode_protocol(flag_test_packet, buffer, sizeof(buffer)); 
}

uint8_t buf_size_test()
{
    struct protocol buf_size_test_packet;
    buf_size_test_packet.version = 1;
    buf_size_test_packet.type = 2;
    buf_size_test_packet.flags = 0x10; // Invalid nibble
    buf_size_test_packet.payload_length = 0;
    uint8_t buffer[2 + buf_size_test_packet.payload_length + 1];

    uint8_t bytes_written = encode_protocol(buf_size_test_packet, buffer, sizeof(buffer));

    if(bytes_written < 0)
    {
        printf("Error encoding buf size test packet: %d\n", bytes_written);
        return bytes_written; // Test failed
    }

    // Make the buffer one byte too small
    uint8_t small_buffer[2 + buf_size_test_packet.payload_length]; // One byte

    return -2 == encode_protocol(buf_size_test_packet, small_buffer, sizeof(small_buffer));
}

uint8_t payload_length_test()
{
    struct protocol flag_test_packet;
    flag_test_packet.version = 1;
    flag_test_packet.type = 2;
    flag_test_packet.flags = 0x10; // Invalid nibble
    flag_test_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    
}

uint8_t bit_isolation_test()
{

}