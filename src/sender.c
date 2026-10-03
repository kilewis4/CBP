#include <unistd.h>

#include "protocol.h"
#include "transport.h"
#include "encoder.h"

// TODO: Change returning a copy of the data to its caller
// TODO: Check payload length
struct protocol create_protocol(uint8_t version, 
                                uint8_t type, 
                                uint8_t flags, 
                                uint8_t payload_length,
                                uint8_t payload[])
{
    struct protocol p;
    p.version = version;
    p.type = type;
    p.flags = flags; 
    p.payload_length = payload_length;

    for(int i = 0; i < payload_length; i++)
    {
        p.payload[i] = payload[i];
    }

    return p;
}


// TODO: Make these CLI arguments
int main(void) 
{
    uint8_t version = 1;
    uint8_t type = 3;
    uint8_t flags = ACK_REQUIRED_FLAG | ENCRYPTED_FLAG; // 0xC
    uint8_t payload[] = {0x99, 0x43, 0x43, 0x44, 0x45};
    uint8_t payload_length = sizeof(payload);

    uint8_t buf[max_packet_size];
    struct protocol p = create_protocol(version, type, flags, payload_length, payload);

    int packet_size = encode_protocol(p, buf, sizeof(buf));
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
