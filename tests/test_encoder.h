#ifndef TEST_ENCODER_H
#define TEST_ENCODER_H

#include <stdint.h>

uint8_t flag_test(); // Test max nibble works and anything past that doesn't
uint8_t buf_size_test(); // Will contain buffer being correct2  and one byte short of 2 + payload length + 1
uint8_t payload_length_test(); // Will test payload length == 0 and payload length == max_payload_size
// possible test(s) in current validation for version, type, and payload length
//no_buffer__overrun_test(); ???
uint8_t bit_isolation_test(); // Set only one field non-zero at a time and confirm others are exactly 0 to confirm shifts work

#endif