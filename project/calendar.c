#include "calendar.h"
int clock_format_utc(time_t moment, char *text, size_t size) {
    if (!text || size == 0) return -1;
    struct tm *utc = gmtime(&moment);
    if (!utc) return -1;
    return strftime(text, size, "%Y-%m-%d %H:%M:%S", utc) ? 0 : -1;
}
