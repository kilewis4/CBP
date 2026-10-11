#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"
#include "transport.h"
#include "encoder.h"
#include "sender_utils.h"

int send_packet(int fd,
                uint8_t version, 
                uint8_t type, 
                uint8_t flags, 
                uint8_t payload[], 
                uint8_t payload_length)
{
    struct protocol p;
    uint8_t buf[max_payload_size];

    if(create_protocol(version, type, flags, payload_length, payload, &p) < 0)
    {
        fprintf(stderr, "Error: Payload length too high\n"); //TODO: Log errors

        return -1;
    }

    int packet_size = encode_protocol(p, buf, sizeof(buf));
    if(packet_size <= 0) { return -1; } // Don't attempt to send packets if no packet is encoded

    return cbp_send_all(fd, buf, (size_t)packet_size);
}


// TODO: Make these CLI arguments
int main(int argc, char *argv[]) 
{
    int rc = -1;                              // Value to be returned, -1 if it faults
    int fd = -1;                              // File domain reports -1 before defined
    FILE *fptr = NULL;                        // File pointer starts as NULL
    char file_buffer[128];                    // File buffer as 128 bytes for trailing characters
    int line_no = 0;                          // Line we have error on


    // Requires file name
    if(argc != 2) { fprintf(stderr, "usage: %s <script>\n", argv[0]); return -1; }

    fptr = fopen(argv[1], "r");
    if (fptr == NULL) { perror(argv[1]); goto cleanup; }

    fd = cbp_client_connect(CBP_SOCK_PATH);
    if(fd < 0) { goto cleanup; } // File domain setup failed

    while(fgets(file_buffer, sizeof(file_buffer), fptr) != NULL)
    {
        line_no++; // Start on first line

        size_t len = strlen(file_buffer); 
        if(len > 0 && file_buffer[len - 1] != '\n' && !feof(fptr))
        {
            fprintf(stderr, "line %d: too long\n", line_no);
            goto cleanup;
        }

        char *p = file_buffer;
        while(isspace((unsigned char)*p)) { p++; }      // skip leading whitespace
        if(*p == '#' || *p == '\0') { continue; }       // comment, or nothing but whitespace

        uint8_t version, type, flags, payload[max_payload_size], payload_len;
        if(parse_line(file_buffer, &version, &type, &flags, payload, &payload_len) < 0)
        {
            fprintf(stderr, "line %d: bad packet\n", line_no);
            goto cleanup;
        }

        if(send_packet(fd, version, type, flags, payload, payload_len) <= 0) { goto cleanup; }
    }

    rc = 0; // Marks success

    cleanup:
        if(fd >= 0) { close(fd); }
        if (fptr != NULL) { fclose(fptr); }
        return rc; 
}
