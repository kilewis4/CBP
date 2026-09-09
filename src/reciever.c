#include "protocol.h"
#include "transport.h"
#include "parser.h"
#include "protocol.h"


int main(void) 
{
    int server_fd = cbp_server_start(CBP_SOCK_PATH);
    int client_fd = cbp_server_accept(server_fd); 

    if((server_fd || client_fd) < 0)
    {
        return -1
    }

    uint8_t buf[max_packet_size];
    struct protocol p;

    if(cbp_recv_exact(client_fd, buf, 2) < 0)                                 // Put payload length into buf
    {
        return -1;
    }  
                                  
    uint8_t payload_length = extract_payload_length(buf[0]);              // Extract payload_length


    if(cbp_recv_exact(client_fd, buf[2], payload_length + checksum_size) < 0) // Pull full packet
    {
        return -1;
    } 

    return parse_protocol(buf, payload_length + header_size + checksum_size, &p);
}