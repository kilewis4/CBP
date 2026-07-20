// Checksum algorithms
#include <stdint.h>
#include "checksum.h"

uint8_t compute_checksum(uint8_t *raw_bytes, uint8_t length)
{
    uint8_t checksum = 0;

    for (uint8_t i = 0; i < length; i++)
    {
        checksum ^= raw_bytes[i];
    }
    
    return checksum;
}