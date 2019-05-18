#include <cstdint>

#include "bsp.h"

extern "C" ssize_t _write_r(struct _reent *, int, const void *buf, size_t nbyte) {
    return bsp::debugPort.write((const char *)buf, nbyte);
}

extern "C" ssize_t _read_r(struct _reent *r, int file, void *buf, size_t nbyte) {
    return 0;
}

extern "C" int _isatty(int file) {
    return 1;
}

extern "C" caddr_t _sbrk_r(struct _reent *r, int size) {
    extern char __heap_start;  // imported from linker script
    extern char __heap_end;    // imported from linker script
    static char *current_heap_end = &__heap_start;
    char *previous_heap_end;

    r = r;  // suppress warning

    previous_heap_end = current_heap_end;

    if (current_heap_end + size > &__heap_end)  // is there enough space on the heap left?
    {
        errno = ENOMEM;      // not enough memory left
        return (caddr_t)-1;  // return immediatelly
    }

    current_heap_end += size;  // increase data space

    return (caddr_t)previous_heap_end;  // return requested data space
}

extern "C" int _stat_r(struct _reent *r, const char *pathname, struct stat *st) {
    r = r;                // suppress warning
    pathname = pathname;  // suppress warning

    //    st->st_mode = S_IFCHR;
    return 0;
}

extern "C" int _fstat_r(struct _reent *r, int file, struct stat *st) {
    //   st->st_mode = S_IFCHR;
    return 0;
}

extern "C" int _kill_r(struct _reent *r, pid_t pid, int signal) {
    r = r;            // suppress warning
    pid = pid;        // suppress warning
    signal = signal;  // suppress warning

    // errno = EINVAL;
    return -1;
}

extern "C" pid_t _getpid_r(struct _reent *r) {
    return 1;
}

extern "C" off_t _lseek_r(struct _reent *r, int file, off_t offset, int whence) {
    return 0;
}

extern "C" int _close_r(struct _reent *, int) {
    return -1;
}

extern "C" int _open_r(struct _reent *, const char *, int, int) {
    return -1;
}

extern "C" void _exit(int status) {
    status = status;  // suppress warning

    while (1)
        ;
}
