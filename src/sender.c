#include "protocol.h"
#include "transport.h"


int main(void) 
{
    uint8_t buf[max_packet_size];
    int packet_size = encode_protocol(..., buf, sizeof(buf));

    int fd = cbp_client_connect(CBP_SOCK_PATH);
    cbp_send_all(fd, buf, (size_t)packet_size);
}