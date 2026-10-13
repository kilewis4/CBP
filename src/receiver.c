#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "protocol.h"
#include "transport.h"
#include "parser.h"
#include "protocol.h"
#include "logger.h"

// n is cbp_recv_exact's return value
static void log_recv_failure(const char *what, int n)
{
    if(n == 0) { cbp_log(CBP_LOG_ERROR, "%s: peer closed connection", what); }
    else       { cbp_log(CBP_LOG_ERROR, "%s: recv failed: %s", what, strerror(errno)); }
}

int main(void) 
{
    int rc = -1;
    int server_fd = -1;
    int client_fd = -1;
    int received = 0;
    int rejected = 0;

    if(cbp_log_init("receiver") < 0) { perror("cbp_log_init"); return -1; }

    server_fd = cbp_server_start(CBP_SOCK_PATH);
    if(server_fd < 0) // Bad server FD
    { 
        cbp_log(CBP_LOG_ERROR, "server_start failed: %s", strerror(errno));
        goto cleanup; 
    } 

    client_fd = cbp_server_accept(server_fd); 
    if(client_fd < 0) // Bad client FD
    { 
        cbp_log(CBP_LOG_ERROR, "accept failed: %s", strerror(errno));
        goto cleanup; 
    } 

    for(;;)
    {
        uint8_t buf[max_packet_size];
        struct protocol p;

        int n = cbp_recv_exact(client_fd, buf, header_size);
        if(n == 0)
        {
            cbp_log(CBP_LOG_INFO, "sender closed connection");
            rc = 0;                      // clean end: EOF between packets
            break;
        }
        if(n < 0) { log_recv_failure("header", n); break; }

        uint8_t payload_length = extract_payload_length(buf[1]);

        n = cbp_recv_exact(client_fd, buf + header_size, payload_length + checksum_size);
        if(n <= 0) { log_recv_failure("body", n); break; }   // EOF here = truncated packet

        int total = header_size + payload_length + checksum_size;
        cbp_log_packet("RX", buf, (size_t)total);

        int bytes_written = parse_protocol(buf, total, &p);
        if(bytes_written < 0)
        {
            cbp_log(CBP_LOG_ERROR, "parse_protocol failed (returned %d)", bytes_written);
            rejected++;
            break;                    // Something is off, immediately stop
        }

        received++;
    }


    cleanup:
        cbp_log(rc == 0 ? CBP_LOG_INFO : CBP_LOG_ERROR,
                "done: %d packet(s) parsed, %d rejected, rc=%d", received, rejected, rc);
        if(client_fd >= 0) { close(client_fd); }
        if(server_fd >= 0) { close(server_fd); }
        cbp_log_close();
        return rc;
}