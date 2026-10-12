#ifndef CBP_LOGGER_H
#define CBP_LOGGER_H

#include <stddef.h>
#include <stdint.h>

typedef enum { CBP_LOG_INFO, CBP_LOG_ERROR } cbp_log_level;

int  cbp_log_init(const char *name);   /* opens ./logs/<name>.log, 0 on success */
void cbp_log_close(void);
void cbp_log(cbp_log_level lvl, const char *fmt, ...);
void cbp_log_packet(const char *dir, const uint8_t *buf, size_t len);

#endif