#include <cstdint>

#include "bsp.h"

extern "C" ssize_t _write_r(struct _reent *, int, const void *buf, size_t nbyte) {
    return bsp::debugPort.write((const char *)buf, nbyte);
}

extern "C" int _isatty(int file) {
    return 1;
}
