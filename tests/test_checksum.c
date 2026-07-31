// Test short and long packets, a buffer of all 0s, changing a byte after computing the checsum

#include <stdio.h>
#include <stdint.h>
#include "checksum.h"
#include "protocol.h"

uint8_t short_packets()
{
    struct protocol short_packet;
    short_packet.version = 1;
    short_packet.type = 2;
    short_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    short_packet.payload_length = 0;

    return compute_checksum((uint8_t *)&short_packet, sizeof(short_packet) - 1); // Exclude checksum byte
}

uint8_t long_packets()
{
    struct protocol long_packet;
    long_packet.version = 1;
    long_packet.type = 2;
    long_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    long_packet.payload_length = max_payload_size;

    for (uint8_t i = 0; i < max_payload_size; i++)
    {
        long_packet.payload[i] = i + 1; // Fill payload with some test data
    }

    return compute_checksum((uint8_t *)&long_packet, sizeof(long_packet) - 1); // Exclude checksum byte
}

uint8_t buffer_of_zeroes()
{
    struct protocol zero_packet;
    zero_packet.version = 0;
    zero_packet.type = 0;
    zero_packet.flags = 0;
    zero_packet.payload_length = 0;

    for (uint8_t i = 0; i < max_payload_size; i++)
    {
        zero_packet.payload[i] = 0; // Fill payload with zeros
    }

    return compute_checksum((uint8_t *)&zero_packet, sizeof(zero_packet) - 1); // Exclude checksum byte
}

uint8_t checksum_corruption()
{
    struct protocol corrupt_packet;
    corrupt_packet.version = 1;
    corrupt_packet.type = 2;
    corrupt_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    corrupt_packet.payload_length = 3;

    for (uint8_t i = 0; i < corrupt_packet.payload_length; i++)
    {
        corrupt_packet.payload[i] = i + 1; // Fill payload with some test data
    }

    uint8_t original_checksum = compute_checksum((uint8_t *)&corrupt_packet, sizeof(corrupt_packet) - 1); // Exclude checksum byte

    // Change a byte in the payload
    corrupt_packet.payload[0] ^= 0xFF; // Flip bits of the first byte

    uint8_t new_checksum = compute_checksum((uint8_t *)&corrupt_packet, sizeof(corrupt_packet) - 1); // Exclude checksum byte

    return new_checksum; // Return the new checksum after corruption
}

int main()
{
    // Test short packets
    if(short_packets() != 0xC) // Expected checksum for short packet
    {
        printf("Short packet checksum test failed\n");
        return -1;
    }
    else
    {
        printf("Short packet checksum test passed\n");
    }

    // Test long packets
    if(long_packets() != 0xC) // Expected checksum for long packet
    {
        printf("Long packet checksum test failed\n");
        return -1;
    }
    else
    {
        printf("Long packet checksum test passed\n");
    }

    // Test a buffer of all 0s
    if(buffer_of_zeros() != 0xC) // Expected checksum for buffer of all 0s
    {
        printf("Buffer of zeros checksum test failed\n");
        return -1;
    }
    else
    {
        printf("Buffer of zeros checksum test passed\n");
    }

    // Test changing a byte after computing the checksum
    if(checksum_corruption() != 0xC) // Expected checksum for change in in checksum after changing a byte
    {
        printf("Checksum corruption test failed\n");
        return -1;
    }
    else
    {
        printf("Checksum corruption test passed\n");
    }
    return 0;
}
