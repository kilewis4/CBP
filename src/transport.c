#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "transport.h"

int cbp_server_start(const char *sock_path) // socket() -> bind() -> listen()
{
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);

    int fd = socket(AF_UNIX, SOCK_STREAM, 0); // TODO: Check AF_UNIX when done local testing, check SOCK_STREAM for correctness 
    
    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) // Assign name to a socket
    {
        close(fd);
        return -1;
    }

    if(listen(fd, 5) < 0) // Listen for connections. 5 is arbitrary, may change
    {
        close(fd);
        return -1;
    }
    
    return fd;
}

ssize_t cbp_send_all(int fd, const uint8_t *buf, size_t len) 
{
    size_t sent = 0;

    while (sent < len) 
    {
        ssize_t n = send(fd, buf + sent, len - sent, 0);

        if (n < 0)                         // Likely error
        {
            if(errno == EINTR) continue; // Interrupted, try again
            return -1;
        } 

        sent += (size_t)n;
    }

    return (ssize_t)sent;
}

ssize_t cbp_recv_exact(int fd, uint8_t *buf, size_t len) 
{
    size_t received = 0;

    while (received < len) 
    {
        ssize_t n = recv(fd, buf + received, len - received, 0);

        if (n == 0) return 0;   // Orderly shutdown, or 0 size datagram. Should not be the requested number of bytes as header is 2
        if (n < 0)              // Likely error
        {
            if(errno == EINTR) continue; // Interrupted, try again
            return -1;
        }   

        received += (size_t)n;
    }

    return (ssize_t)received;
}