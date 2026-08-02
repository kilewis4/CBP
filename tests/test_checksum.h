#ifndef TEST_CHECKSUM_H
#define TEST_CHECKSUM_H

#include <stdint.h>

uint8_t short_packets_test();
uint8_t long_packets_test();
uint8_t buffer_of_zeroes_test();
uint8_t no_corruption_test();

#endif