#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include "protocol.h"

int mini_encode(struct protocol p, uint8_t *raw_bytes);
uint8_t short_packets_test();
uint8_t long_packets_test();
uint8_t buffer_of_zeroes_test();
uint8_t no_corruption_test();

#endif