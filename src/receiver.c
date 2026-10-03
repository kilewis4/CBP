#include <unistd.h>
#include <stdio.h>

#include "protocol.h"
#include "transport.h"
#include "parser.h"
#include "protocol.h"


int main(void) 
{
    int server_fd = cbp_server_start(CBP_SOCK_PATH);

    if(server_fd == -1) { goto failure_occurred_server; } // Bad server FD

    int client_fd = cbp_server_accept(server_fd); 

    if(client_fd < 0) { goto failure_occurred_all; } // Bad client FD

    uint8_t buf[max_packet_size];
    struct protocol p;

    if(cbp_recv_exact(client_fd, buf, 2) <= 0) { goto failure_occurred_all; } // Put payload length into buf
                                
    uint8_t payload_length = extract_payload_length(buf[1]); // Extract payload_length


    if(cbp_recv_exact(client_fd,                             // Pull full packet
                      buf + header_size, 
                      payload_length + checksum_size) 
                      <= 0) 
    {
        failure_occurred_all:
            close(client_fd);
        failure_occurred_server:
            close(server_fd);
            return -1;
    } 

    int bytes_written = parse_protocol(buf, payload_length + header_size + checksum_size, &p);
    printf("Bytes written: %d\n", bytes_written);
    printf("Version: %d\n", p.version);
    printf("Type: %d\n", p.type);
    printf("Flags: %d\n", p.flags);
    printf("Payload length: %d\n", p.payload_length);
    printf("Checksum: %d\n", p.checksum);
    printf("Payload content: ");
    for (int i = 0; i < p.payload_length; i++) 
    {
        printf("%d ", p.payload[i]);
    }
    printf("\n");

    return bytes_written;
}