#include <unistd.h>
#include <stdio.h>

#include "protocol.h"
#include "transport.h"
#include "encoder.h"

int create_protocol(uint8_t version, 
                                uint8_t type, 
                                uint8_t flags, 
                                uint8_t payload_length,
                                uint8_t payload[],
                                struct protocol *out)
{
    if(max_payload_size < payload_length)
    {
        return -1; // Signal failure
    }

    out->version = version;
    out->type = type;
    out->flags = flags; 
    out->payload_length = payload_length;

    for(int i = 0; i < payload_length; i++)
    {
        out->payload[i] = payload[i];
    }

    return 0;
}

int send_packet(uint8_t version, 
                uint8_t type, 
                uint8_t flags, 
                uint8_t payload[], 
                uint8_t payload_length, 
                struct protocol p)
{
    if(create_protocol(version, type, flags, payload_length, payload , &p) < 0)
    {
        fprintf(stderr, "Error: Payload length too high\n", "../Error/error.txt");

        goto file_domain_closure;
    }

    int packet_size = encode_protocol(p, buf, sizeof(buf));
    if(packet_size <= 0) { goto file_domain_closure; } // Don't attempt to send packets if no packet is encoded

    int fd = cbp_client_connect(CBP_SOCK_PATH);
    if(fd < 0) { goto file_domain_closure; } // File domain setup failed

    int size_sent = cbp_send_all(fd, buf, (size_t)packet_size);

}

// TODO: Make these CLI arguments
int main(int argc, char *argv[]) 
{
    // uint8_t version = 1;
    // uint8_t type = 3;
    // uint8_t flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    // uint8_t payload[] = {0x99, 0x43, 0x43, 0x44, 0x45};
    // uint8_t payload_length = sizeof(payload);
    // uint8_t buf[max_packet_size];
    // struct protocol p;

    // Requires file name
    if(argc != 2)
    {
        goto failure_occurred;
    }

    struct protocol packets[10]; // Arbitrary 10 packet max sent

    char file_buffer[128];
    char *endptr;

    FILE *fptr = NULL;
    fptr = fopen(argv[1], "r");
    if (fd < 0) { goto failure_occured; }


    uint8_t file_contents = (file_buffer, endptr, 0);

    while(fgets(file_buffer, sizeof(file_buffer), fptr) != NULL)
    {
        // Pointers for strtoul
        char *start = line;
        char *end;

        // Grab version
        unsigned long version = strtoul(start, &end, 0);
        if (end == start || version > 15) return -1;  // no digits, or not a nibble
        start = end;

        // Grab type
        unsigned long type = strtoul(start, &end, 0);
        if (end == start || type > 15) return -1;  // no digits, or not a nibble
        start = end;

        // Grab flags
        unsigned long flags = strtoul(start, &end, 0);
        if (end == start || flags > 15) return -1;  // no digits, or not a nibble
        start = end;

        // Grab payload contents
        unsigned long flags = strtoul(start, &end, 0);
        if (end == start || flags > 15) return -1;  // no digits, or not a nibble
        start = end;
    }

    if(endptr == 0)
    {
        goto close_file_failure;
    }

    send_packet();

    fclose(fptr);
    close(fd);

    if(size_sent <= 0) // 0 should be unreachable per cbp_send_all's contract; checked anyway — defense against corruption/hardware fault
    {
        file_domain_closure:
            close(fd);
        close_file_failure:
            fclose(fptr);
        failure_occurred:
            return -1; 
    } 
}
