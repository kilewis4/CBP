// Test short and long packets, a buffer of all 0s, changing a byte after computing the checsum

#include <stdio.h>
#include <stdint.h>
#include "checksum.h"
#include "protocol.h"

int mini_encode(struct protocol p, uint8_t *raw_bytes, uint8_t buf_size)
{
    uint16_t packed_header = 0;
    int total_bytes_written = 0;

    packed_header = (p.version << 12) | (p.type << 8) | (p.flags << 4) | p.payload_length;
    raw_bytes[0] = packed_header >> 8;
    total_bytes_written++;
    raw_bytes[1] = packed_header & 0xFF;
    total_bytes_written++;

    for(int i = 0; i < p.payload_length; i++)
    {
        raw_bytes[2 + i] = p.payload[i]; 
        total_bytes_written++;
    }

    return total_bytes_written;
}

uint8_t short_packets()
{
    struct protocol short_packet;
    short_packet.version = 1;
    short_packet.type = 2;
    short_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    short_packet.payload_length = 0;
    uint8_t buffer[max_packet_size];

    int bytes_written = mini_encode(short_packet, buffer, sizeof(buffer));
    uint8_t correct_checksum = compute_checksum(buffer, bytes_written - 1); // Exclude checksum byte
    buffer[0] = buffer[0] ^ 1; 

    return correct_checksum != compute_checksum(buffer, bytes_written - 1); // Exclude checksum byte
}

uint8_t long_packets()
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

    int bytes_written = mini_encode(long_packet, buffer, sizeof(buffer));
    uint8_t correct_checksum = compute_checksum(buffer, bytes_written - 1);
    buffer[0] = buffer[0] ^ 1; 

    return correct_checksum != compute_checksum(buffer, bytes_written - 1); // Exclude checksum byte
}

uint8_t buffer_of_zeroes()
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

    int bytes_written = mini_encode(zero_packet, buffer, sizeof(buffer));

    return compute_checksum(buffer, sizeof(buffer) - 1) != 0x00; // Exclude checksum byte
}

uint8_t checksum_corruption()
{
    struct protocol corrupt_packet;
    corrupt_packet.version = 1;
    corrupt_packet.type = 2;
    corrupt_packet.flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    corrupt_packet.payload_length = 3;
    uint8_t buffer[max_packet_size];

    for (uint8_t i = 0; i < corrupt_packet.payload_length; i++)
    {
        corrupt_packet.payload[i] = i + 1; // Fill payload with some test data
    }

    mini_encode(corrupt_packet, buffer, sizeof(buffer));
    uint8_t original_checksum = compute_checksum(buffer, sizeof(buffer) - 1); // Exclude checksum byte

    // Change a byte in the payload
    corrupt_packet.payload[0] ^= 0xFF; // Flip bits of the first byte

    mini_encode(corrupt_packet, buffer, sizeof(buffer)); // Re-encode after corruption

    uint8_t new_checksum = compute_checksum(buffer, sizeof(buffer) - 1); // Exclude checksum byte

    return new_checksum != original_checksum; // Return the new checksum after corruption
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
    if(buffer_of_zeros())
    {
        printf("Buffer of zeros checksum test failed\n");
        return -1;
    }
    else
    {
        printf("Buffer of zeroes checksum test passed\n");
    }

    // Test changing a byte after computing the checksum
    if(checksum_corruption())
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
