#ifndef TEST_ENCODER_H
#define TEST_ENCODER_H

#include <stdint.h>

uint8_t flag_test(); // Test max nibble works and anything past that doesn't
uint8_t buf_size_test(); // Will contain buffer being correct2  and one byte short of 2 + payload length + 1
uint8_t payload_length_test(); // Will test payload length == 0 and payload length == max_payload_size
uint8_t no_buffer_overrun_test(); // Will fill buffer with sentinel value and confirm nothing is written past the end of the packet
uint8_t bit_isolation_test(); // Set only one field non-zero at a time and confirm others are exactly 0 to confirm shifts work

#endif