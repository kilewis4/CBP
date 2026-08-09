#ifndef TEST_PARSER_H
#define TEST_PARSER_H

#include <stdint.h>

uint8_t round_trip_test(); // Will encode a packet, then decode it and assert every field of the restulting struct matches
uint8_t oversized_buffer_test(); //Encode a small packet into a buffer sized as max_packet_size, then parse it with full max_packet_size as buf_size. 
uint8_t buffer_too_small_test(); // Buffer too small, test the boundary
uint8_t payload_length_exceeds_buffer_test(); // Header claims payload_length is larger than what buf_size has room for, confirm -2 return.
uint8_t checksum_mismatch_test(); //Validly encoded packet, flip a bit in the payload or header after encoding. Confirm we get -3 on parse
uint8_t bit_isolation_test(); // Construct a header with known pattern and confirm fields
uint8_t round_trip_max_payload_test(); //Same as round_trip_test but with payload_length = 15

#endif