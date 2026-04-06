//
// log.h -> In-house logging solution for PlatinumSrc
//          This solution only relies on the C standard library!
//          This is also self contained, since it's very minimal.
//
//          You can optionally override these two macros:
//              > LOG_LEVEL
//              > LOG_FILE
//
//          By defining them BEFORE you include this header.
//          Example:
//              #define LOG_LEVEL LOG_INFO
//              #define LOG_FILE fopen("latest.log", "a")
//              #include "log/log.h"
//

#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/* Minimum level to emit; set before including log.h to override. */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_DEBUG
#endif

/* Output target; defaults to stderr. Override before including log.h. */
#ifndef LOG_FILE
#define LOG_FILE stderr
#endif

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
} log_t;

static const char *cat_str(log_t cat) {
    switch (cat) {
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default:        return "UNKNOWN";
    }
}

/* Strip leading path, keep filename only. */
static inline const char *base(const char *p) {
    const char *s = p;
    while (*p) { if (*p == '/' || *p == '\\') s = p + 1; p++; }
    return s;
}

#ifdef _MSC_VER
#  define _LOG_FMT_ATTR
#  define _LOG_FMT_PARAM _Printf_format_string_
#elif defined(__GNUC__)
#  define _LOG_FMT_ATTR __attribute__((format(printf, 4, 5)))
#  define _LOG_FMT_PARAM
#else
#  define _LOG_FMT_ATTR
#  define _LOG_FMT_PARAM
#endif

static inline _LOG_FMT_ATTR void wr(
    log_t cat, const char *file, const char *fun, _LOG_FMT_PARAM const char *fmt, ...
) {
    if (cat < LOG_LEVEL) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(LOG_FILE, "%02d:%02d:%02d ~ [%s] [%s] [%s] ",
        t->tm_hour, t->tm_min, t->tm_sec,
        cat_str(cat), base(file), fun
    );

    fputc('[', LOG_FILE);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(LOG_FILE, fmt, ap);
    va_end(ap);
    fputs("]\n", LOG_FILE);
}

#define LOG(cat, ...) wr(cat, __FILE__, __func__, __VA_ARGS__)

#endif