#ifndef CHECKSUM_H
#define CHECKSUM_H
#include <stdint.h>

uint8_t compute_checksum(uint8_t *raw_bytes, uint8_t length);

#endif