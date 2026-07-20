// At a high level: Takes a decoded protocol stuct and produces raw bytes.

#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>
#include "protocol.h"

int encode_protocol(struct protocol p, uint8_t *raw_byes, uint8_t buf_size);

#endif