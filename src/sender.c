#include "protocol.h"
#include "transport.h"

// TODO: Make these CLI arguments
int main(void) 
{
    uint8_t buf[max_packet_size];
    create_protocol(???);
    initialize_buffer(*buf, ???);

    int packet_size = encode_protocol(..., buf, sizeof(buf));
    if(packet_size <= 0) { goto failure_occurred; } // Don't attempt to send packets if no packet is encoded

    int fd = cbp_client_connect(CBP_SOCK_PATH);
    if(fd < 0) { goto failure_occurred; } // File domain setup failed

    int size_sent = cbp_send_all(fd, buf, (size_t)packet_size);

    if(size_sent <= 0) // 0 should be unreachable per cbp_send_all's contract; checked anyway — defense against corruption/hardware fault
    {
        close(fd);
        failure_occurred:
            return -1; 
    } 
}

// TODO: Change returning a copy of the data to its caller
struct protocol create_protocol(uint8_t version, 
                                uint8_t type, 
                                uint8_t flags, 
                                uint8_t payload_length)
{
    struct protocol p;
    p.version = version;
    p.type = type;
    p.flags = flags; 
    p.payload_length = payload_length;

    return p;
}

void initialize_buffer(uint8 buf[], ??)
{
    // Initialize buffer
}