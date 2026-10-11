#include <stdint.h>

#include <protocol.h>

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
