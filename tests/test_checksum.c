// Test short and long packets, a buffer of all 0s, changing a byte after computing the checsum

#include <stdio.h>
#include <stdint.h>
#include "checksum.h"
#include "protocol.h"
#include "encoder.h"

uint8_t short_packets_test()
{
    struct protocol short_packet;
    short_packet.version = 1;
    short_packet.type = 2;
    short_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    short_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    int bytes_written = encode_protocol(short_packet, buffer, sizeof(buffer));

    if(bytes_written < 0)
    {
        printf("Error encoding short packet: %d\n", bytes_written);
        return 0; // Test failed
    }

    uint8_t correct_checksum = compute_checksum(buffer, bytes_written - 1); // Exclude checksum byte
    buffer[0] = buffer[0] ^ 1; 

    return correct_checksum != compute_checksum(buffer, bytes_written - 1); // Exclude checksum byte
}

uint8_t long_packets_test()
{
    struct protocol long_packet;
    long_packet.version = 1;
    long_packet.type = 2;
    long_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    long_packet.payload_length = max_payload_size;
    uint8_t buffer[max_packet_size];

    for (uint8_t i = 0; i < max_payload_size; i++)
    {
        long_packet.payload[i] = i + 1; // Fill payload with some test data
    }

    int bytes_written = encode_protocol(long_packet, buffer, sizeof(buffer));

    if(bytes_written < 0)
    {
        printf("Error encoding long packet: %d\n", bytes_written);
        return 0; // Test failed
    }

    uint8_t correct_checksum = compute_checksum(buffer, bytes_written - 1);
    buffer[0] = buffer[0] ^ 1; 

    return correct_checksum != compute_checksum(buffer, bytes_written - 1); // Exclude checksum byte
}

uint8_t buffer_of_zeroes_test()
{
    struct protocol zero_packet;
    zero_packet.version = 0;
    zero_packet.type = 0;
    zero_packet.flags = 0;
    zero_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    for (uint8_t i = 0; i < max_payload_size; i++)
    {
        zero_packet.payload[i] = 0; // Fill payload with zeros
    }

    int bytes_written = encode_protocol(zero_packet, buffer, sizeof(buffer));

    if(bytes_written < 0)
    {
        printf("Error encoding buffer of zeroes packet: %d\n", bytes_written);
        return 0; // Test failed
    }

    return compute_checksum(buffer, bytes_written - 1) == buffer[bytes_written - 1]; // Exclude checksum byte
}

uint8_t no_corruption_test()
{
    struct protocol test_packet;
    test_packet.version = 1;
    test_packet.type = 2;
    test_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    test_packet.payload_length = 5;
    uint8_t buffer[max_packet_size];

    for (uint8_t i = 0; i < test_packet.payload_length; i++)
    {
        test_packet.payload[i] = i + 1; // Fill payload with some test data
    }

    int bytes_written = encode_protocol(test_packet, buffer, sizeof(buffer));

    if(bytes_written < 0)
    {
        printf("Error encoding no corruption packet: %d\n", bytes_written);
        return 0; // Test failed
    }


    return buffer[bytes_written - 1] == compute_checksum(buffer, bytes_written - 1); // Exclude checksum byte
}
