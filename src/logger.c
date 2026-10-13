#define _POSIX_C_SOURCE 200809L
#include "logger.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define HEX_MAX_BYTES 32

static FILE *log_fp = NULL;

int cbp_log_init(const char *name)
{
    char path[256];

    if (mkdir("logs", 0755) < 0 && errno != EEXIST)
        return -1;

    int n = snprintf(path, sizeof path, "logs/%s.log", name);
    if (n < 0 || (size_t)n >= sizeof path)
        return -1;

    log_fp = fopen(path, "a");
    if (!log_fp) return -1;
    cbp_log(CBP_LOG_INFO, "---- %s started, pid %d ----", name, (int)getpid());
    return 0;
}

void cbp_log_close(void)
{
    if (log_fp) {
        fclose(log_fp);
        log_fp = NULL;
    }
}

void cbp_log(cbp_log_level lvl, const char *fmt, ...)
{
    if (!log_fp)
        return;

    struct timespec ts;
    struct tm tm;
    char stamp[16];

    clock_gettime(CLOCK_REALTIME, &ts);
    localtime_r(&ts.tv_sec, &tm);
    strftime(stamp, sizeof stamp, "%H:%M:%S", &tm);

    fprintf(log_fp, "%s.%03ld [%s] ", stamp, ts.tv_nsec / 1000000,
            lvl == CBP_LOG_ERROR ? "ERROR" : "INFO");

    va_list ap;
    va_start(ap, fmt);
    vfprintf(log_fp, fmt, ap);
    va_end(ap);

    fputc('\n', log_fp);
    fflush(log_fp); // Normally avoid for performanace. Can rewrite if we scale up
}

void cbp_log_packet(const char *dir, const uint8_t *buf, size_t len)
{
    char hex[HEX_MAX_BYTES * 3 + 1];
    size_t shown = len < HEX_MAX_BYTES ? len : HEX_MAX_BYTES;

    for (size_t i = 0; i < shown; i++)
        snprintf(hex + i * 3, 4, "%02x ", buf[i]);
    hex[shown * 3] = '\0';

    cbp_log(CBP_LOG_INFO, "%s %zu bytes: %s", dir, len, hex);
}