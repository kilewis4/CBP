#ifndef CBP_TRANSPORT_H
#define CBP_TRANSPORT_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define CBP_SOCK_PATH "/tmp/cbp.sock"

int cbp_server_start(const char *sock_path);   // socket+bind+listen, returns listening fd
int cbp_server_accept(int server_fd);           // blocks, returns connected client fd

int cbp_client_connect(const char *sock_path);  // socket+connect, returns connected fd

ssize_t cbp_send_all(int fd, const uint8_t *buf, size_t len);
ssize_t cbp_recv_exact(int fd, uint8_t *buf, size_t len);

#endif