// Helper functions, binary printing, bit visualization, endian helpers, alignment helpers. Should include hexdump

#include <stdio.h>
#include <stdint.h>

void hexdump(uint8_t *data, uint8_t length)
{
    for(int current_byte = 0; current_byte < length; current_byte++)
    {
        printf("%02X ", data[current_byte]);
    }
    printf("\n");
}