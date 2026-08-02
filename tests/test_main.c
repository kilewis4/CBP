#include <stdio.h>
#include "test_checksum.h"
// other test headers as you add them

int main()
{
    int passed = 0, failed = 0;

    if (short_packets_test())      { passed++; } else { failed++; printf("FAIL: short_packets\n"); }
    if (long_packets_test())       { passed++; } else { failed++; printf("FAIL: long_packets\n"); }
    if (no_corruption_test()) { passed++; } else { failed++; printf("FAIL: no_corruption_test\n"); }
    if (buffer_of_zeroes_test())   { passed++; } else { failed++; printf("FAIL: buffer_of_zeroes\n"); }

    printf("%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}