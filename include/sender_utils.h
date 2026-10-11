#ifndef SENDER_UTIL_H
#define SENDER_UTIL_H

int parse_line(char file_buffer[], 
               uint8_t *version, 
               uint8_t *type, 
               uint8_t *flags, 
               uint8_t *payload, 
               uint8_t *payload_length);

#endif