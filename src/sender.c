#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "protocol.h"
#include "transport.h"
#include "encoder.h"
#include "sender_utils.h"
#include "logger.h"

int send_packet(int fd,
                int line_no,
                uint8_t version, 
                uint8_t type, 
                uint8_t flags, 
                uint8_t sequence_number,
                uint8_t payload[], 
                uint8_t payload_length)
{
    struct protocol p;
    uint8_t buf[max_packet_size];

    if(create_protocol(version, type, flags, payload_length, payload, sequence_number, &p) < 0)
    {
        fprintf(stderr, "Error: Payload length too high\n");
        cbp_log(CBP_LOG_ERROR, "line %d: create_protocol failed (payload_length=%u)",
                line_no, payload_length);
        return -1;
    }

    int packet_size = encode_protocol(p, buf, sizeof(buf));

    if(packet_size <= 0) // Don't attempt to send packets if no packet is encoded
    { 
        cbp_log(CBP_LOG_ERROR, "line %d: encode_protocol failed (returned %d)",
                line_no, packet_size);
        return -1; 
    } 

    int sent = cbp_send_all(fd, buf, (size_t)packet_size);

    if(sent <= 0)
    {
        cbp_log(CBP_LOG_ERROR, "line %d: send_all failed (returned %d): %s",
                line_no, sent, sent < 0 ? strerror(errno) : "no error code");
        return sent;
    }

    char dir[24];
    snprintf(dir, sizeof dir, "TX line %d", line_no);
    cbp_log_packet(dir, buf, (size_t)packet_size);
    return sent;
}


int main(int argc, char *argv[]) 
{
    int rc = -1;                              // Value to be returned, -1 if it faults
    int fd = -1;                              // File domain reports -1 before defined
    FILE *fptr = NULL;                        // File pointer starts as NULL
    char file_buffer[128];                    // File buffer as 128 bytes for trailing characters
    int line_no = 0;                          // Line we have error on
    int packets_sent = 0;
    signal(SIGPIPE, SIG_IGN);

    // Requires file name
    if(argc != 2) { fprintf(stderr, "usage: %s <script>\n", argv[0]); return -1; }

    if(cbp_log_init("sender") < 0) { perror("cbp_log_init"); return -1; }

    fptr = fopen(argv[1], "r");
    if (fptr == NULL) 
    { 
        cbp_log(CBP_LOG_ERROR, "fopen(%s) failed: %s", argv[1], strerror(errno));
        perror(argv[1]); 
        goto cleanup; 
    }

    fd = cbp_client_connect(CBP_SOCK_PATH);
    if(fd < 0)      // File domain setup failed
    { 
        cbp_log(CBP_LOG_ERROR, "connect to %s failed: %s", CBP_SOCK_PATH, strerror(errno));
        goto cleanup; 
    } 

    while(fgets(file_buffer, sizeof(file_buffer), fptr) != NULL)
    {
        line_no++; // Start on first line

        size_t len = strlen(file_buffer); 
        if(len > 0 && file_buffer[len - 1] != '\n' && !feof(fptr))
        {
            fprintf(stderr, "line %d: too long\n", line_no);
            cbp_log(CBP_LOG_ERROR, "line %d: too long", line_no);
            goto cleanup;
        }

        char *p = file_buffer;
        while(isspace((unsigned char)*p)) { p++; }      // skip leading whitespace
        if(*p == '#' || *p == '\0') { continue; }       // comment, or nothing but whitespace

        uint8_t version, type, flags, payload[max_payload_size], sequence_number, payload_len;
        if(parse_line(file_buffer, &version, &type, &flags, payload, &payload_len) < 0)
        {
            fprintf(stderr, "line %d: bad packet\n", line_no);
            cbp_log(CBP_LOG_ERROR, "line %d: bad packet", line_no);
            goto cleanup;
        }

        if(send_packet(fd, line_no, version, type, flags, sequence_number, payload, payload_len) <= 0) { goto cleanup; }

        packets_sent++;
    }

    rc = 0; // Marks success

    cleanup:
        cbp_log(rc == 0 ? CBP_LOG_INFO : CBP_LOG_ERROR,
                "done: %d packet(s) sent, stopped at line %d, rc=%d",
                packets_sent, line_no, rc);
        if(fd >= 0) { close(fd); }
        if (fptr != NULL) { fclose(fptr); }
        cbp_log_close();
        return rc; 
}
