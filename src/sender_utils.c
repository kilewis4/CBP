#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#include "sender_utils.h"
#include "protocol.h"

// TODO: Payload only supports hex and paired digits. 
int parse_line(char file_buffer[], 
               uint8_t *version, 
               uint8_t *type, 
               uint8_t *flags,
               uint8_t *sequence_number
               uint8_t *payload, 
               uint8_t *payload_length)
{
    // Pointers for strtoul
    char *start = file_buffer;
    char *end;

    // Grab version
    unsigned long v = strtoul(start, &end, 0);
    if (end == start || v > 15) return -1;  // no digits, or not a nibble
    start = end;
    *version = (uint8_t)v;

    // Grab type
    unsigned long t = strtoul(start, &end, 0);
    if (end == start || t > 15) return -1;  // no digits, or not a nibble
    start = end;
    *type = (uint8_t)t;

    // Grab flags
    unsigned long f = strtoul(start, &end, 0);
    if (end == start || f > 15) return -1;  // no digits, or not a nibble
    start = end;
    *flags = (uint8_t)f;

    unsigned long s = strtoul(start, &end, 0);
    if (end == start || s > 15) return -1;  // no digits, or not a nibble
    start = end;
    *sequence_number = (uint8_t)s;

    // Grab payload contents
    while(*start == ' ' || *start == '\t') { start++; }    // move to the payload token (there may be none)

    size_t n = 0;
    while(isxdigit((unsigned char)start[0]))
    {
        if(n == max_payload_size) { return -1; }                          // would be a 16th byte
        char pair[3] = { start[0], start[1], '\0' };        
        char *e;
        unsigned long byte = strtoul(pair, &e, 16);
        if(e != pair + 2) { return -1; }                    // odd digit count or bad second char
        payload[n++] = (uint8_t)byte;
        start += 2;
    }
    while(isspace((unsigned char)*start)) { start++; }
        if(*start != '\0') { return -1; }       // anything left is junk, including a trailing '#'
        *payload_length = (uint8_t)n;

    return 0;
}
