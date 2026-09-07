#include <stdio.h>
#include "test_checksum.h"
#include "test_encoder.h"
#include "test_parser.h"

// other test headers as you add them

int main()
{
    int passed = 0, failed = 0;

    if (short_packets_test())                     { passed++; printf("PASS: short_packets\n");} else { failed++; printf("FAIL: short_packets\n"); }
    if (long_packets_test())                      { passed++; printf("PASS: long_packets\n");} else { failed++; printf("FAIL: long_packets\n"); }
    if (no_corruption_test())                     { passed++; printf("PASS: no_corruption_test\n");} else { failed++; printf("FAIL: no_corruption_test\n"); }
    if (buffer_of_zeroes_test())                  { passed++; printf("PASS: buffer_of_zeroes\n");} else { failed++; printf("FAIL: buffer_of_zeroes\n"); }
    if (flag_test())                              { passed++; printf("PASS: flag_test\n");} else { failed++; printf("FAIL: flag_test\n"); }
    if (buf_size_test())                          { passed++; printf("PASS: buf_size_test\n");} else { failed++; printf("FAIL: buf_size_test\n"); }
    if (payload_length_test())                    { passed++; printf("PASS: payload_length_test\n");} else { failed++; printf("FAIL: payload_length_test\n"); }
    if (no_buffer_overrun_test())                 { passed++; printf("PASS: no_buffer_overrun_test\n");} else { failed++; printf("FAIL: no_buffer_overrun_test\n"); }
    if (bit_isolation_test())                     { passed++; printf("PASS: bit_isolation_test\n");} else { failed++; printf("FAIL: bit_isolation_test\n"); }
    if (round_trip_test())                        { passed++; printf("PASS: round_trip_test\n");} else { failed++; printf("FAIL: round_trip_test\n"); }
    if (oversized_buffer_test())                  { passed++; printf("PASS: oversized_buffer_test\n");} else { failed++; printf("FAIL: oversized_buffer_test\n"); }
    if (buffer_too_small_test())                  { passed++; printf("PASS: buffer_too_small_test\n");} else { failed++; printf("FAIL: buffer_too_small_test\n"); }
    if (payload_length_exceeds_buffer_test())     { passed++; printf("PASS: payload_length_exceeds_buffer_test\n");} else { failed++; printf("FAIL: payload_length_exceeds_buffer_test\n"); }
    if (checksum_mismatch_test())                 { passed++; printf("PASS: checksum_mismatch_test\n");} else { failed++; printf("FAIL: checksum_mismatch_test\n"); }
    if (bit_isolation_test())                     { passed++; printf("PASS: bit_isolation_test\n");} else { failed++; printf("FAIL: bit_isolation_test\n"); }
    if (round_trip_max_payload_test())            { passed++; printf("PASS: round_trip_max_payload_test\n");} else { failed++; printf("FAIL: round_trip_max_payload_test\n"); }

    printf("%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}