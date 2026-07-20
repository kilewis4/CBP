#ifndef PARSER_H
#define PARSER_H
#include <stdint.h>
#include "protocol.h"

int parse_protocol(uint8_t *raw_bytes, uint8_t buf_size, struct protocol *p);

#endif