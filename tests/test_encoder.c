// Buffer too small, invalid flags, Payload length exceeding max

#include <stdio.h>
#include <stdint.h>
#include <string.h>
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

    int bytes_written = encode_protocol(flag_test_packet, buffer, sizeof(buffer));
    
    if(bytes_written != -1)
    {
        printf("Error encoding flag test packet: %d\n", bytes_written);
        return 0; // Test failed
    }
    
    flag_test_packet.flags = 0x0F; // Max valid nibble
    int valid_result = encode_protocol(flag_test_packet, buffer, sizeof(buffer));
    if(valid_result < 0)
    {
        printf("Error: valid flags (0x0F) were rejected, got %d\n", valid_result);
        return 0; // Test failed — encoder rejected good input
    }


    return -1 != encode_protocol(flag_test_packet, buffer, sizeof(buffer)); 
}

uint8_t buf_size_test()
{
    struct protocol buf_size_test_packet;
    buf_size_test_packet.version = 1;
    buf_size_test_packet.type = 2;
    buf_size_test_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    buf_size_test_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];
    uint8_t correct_size = 2 + buf_size_test_packet.payload_length + 1;

    int bytes_written = encode_protocol(buf_size_test_packet, buffer, correct_size);

    if(bytes_written < 0)
    {
        printf("Error encoding buf size test packet: %d\n", bytes_written);
        return 0; // Test failed
    }

    // Make the buffer one byte too small
    uint8_t too_small_size = 2 + buf_size_test_packet.payload_length;

    return -2 == encode_protocol(buf_size_test_packet, buffer, too_small_size);
}

uint8_t payload_length_test()
{
    struct protocol payload_test_packet;
    payload_test_packet.version = 1;
    payload_test_packet.type = 2;
    payload_test_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    payload_test_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    int length_zero_bytes_written = encode_protocol(payload_test_packet, buffer, sizeof(buffer));

    if(length_zero_bytes_written < 0)
    {
        printf("Error encoding payload test packet: %d\n", length_zero_bytes_written);
        return 0; // Test failed
    }


    payload_test_packet.payload_length = max_payload_size;
    uint8_t max_size_buffer[max_packet_size]; // fixed
    uint8_t needed_size = 2 + payload_test_packet.payload_length + 1;

    for(int i = 0; i < payload_test_packet.payload_length; i++)
    {
        payload_test_packet.payload[i] = i;
    }

    int length_max_bytes_written = encode_protocol(payload_test_packet, max_size_buffer, needed_size);

    if(length_max_bytes_written < 0)
    {
        printf("Error encoding payload test packet: %d\n", length_max_bytes_written);
        return 0; // Test failed
    }

    for(int i = 0; i < payload_test_packet.payload_length; i++)
    {
        if(max_size_buffer[2 + i] != i)
        {
            printf("Error: Payload data mismatch at index %d\n", i);
            return 0; // Test failed
        }
    }

    return 0 < length_max_bytes_written; // Test passed if both lengths are valid
}

uint8_t no_buffer_overrun_test()
{
    struct protocol p;
    p.version = 1; 
    p.type = 2; 
    p.flags = 3; 
    p.payload_length = 4;
    p.payload[0] = 0xAA;
    p.payload[1] = 0xBB;
    p.payload[2] = 0xCC;
    p.payload[3] = 0xDD;

    uint8_t buffer[max_packet_size];
    memset(buffer, 0xAA, sizeof(buffer)); // sentinel

    int written = encode_protocol(p, buffer, sizeof(buffer));
    if (written < 0) return 0; // setup failed

    for (uint32_t i = written; i < sizeof(buffer); i++)
    {
        if (buffer[i] != 0xAA) return 0; // something wrote out of bounds
    }
    return 1;
}

uint8_t bit_isolation_test()
{
    struct protocol payload_test_packet;
    payload_test_packet.version = 1;
    payload_test_packet.type = 0;
    payload_test_packet.flags = 0; 
    payload_test_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    int non_zero_version_bytes_written = encode_protocol(payload_test_packet, buffer, sizeof(buffer));

    if(buffer[0] != 0x10 || buffer[1] != 0x00 || non_zero_version_bytes_written < 0) // version = 1, type = 0, flags = 0, payload_length = 0
    {
        printf("Error encoding bit isolation test packet: on version field\n");
        return 0; // Test failed
    }

    payload_test_packet.version = 0;
    payload_test_packet.type = 1;

    int zero_type_bytes_written = encode_protocol(payload_test_packet, buffer, sizeof(buffer));

    if(buffer[0] != 0x01 || buffer[1] != 0x00 || zero_type_bytes_written < 0) // version = 0, type = 1, flags = 0, payload_length = 0
    {
        printf("Error encoding bit isolation test packet: on type field\n");
        return 0; // Test failed
    }

    payload_test_packet.type = 0;
    payload_test_packet.flags = 1; 

    int zero_flag_bytes_written = encode_protocol(payload_test_packet, buffer, sizeof(buffer));

    if(buffer[0] != 0x00 || buffer[1] != 0x10 || zero_flag_bytes_written < 0) // version = 0, type = 0, flags = 1, payload_length = 0
    {
        printf("Error encoding bit isolation test packet: on flags field\n");
        return 0; // Test failed
    }

    payload_test_packet.flags = 0;
    payload_test_packet.payload_length = 1; 

    int zero_payload_length_bytes_written = encode_protocol(payload_test_packet, buffer, sizeof(buffer));

    if(buffer[0] != 0x00 || buffer[1] != 0x01 || zero_payload_length_bytes_written < 0) // version = 0, type = 0, flags = 1, payload_length = 1
    {
        printf("Error encoding bit isolation test packet: on payload length field\n");
        return 0; // Test failed
    }

    return 1; // Test passed
}