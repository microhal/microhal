/*
 * syscals_FatFs.cpp
 *
 *  Created on: May 3, 2019
 *      Author: pokas
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <ctime>
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "fatFsStatusToString.h"
#include "ff.h"
#include "ffconf.h"
#include "ports/manager/hardware.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::literals;

std::time_t getTimeFromFattie(uint32_t fattime);

struct FileTable {
    FIL fp;
    bool isOpen = false;
};
static FileTable fileTable[MICROHAL_FATFS_MAXFILES_IN_USE];

extern "C" int _isatty_r(struct _reent *r, int file) {
    if (file > 0) {
        if (file < 3) {  // file number from 0 to 2 are reserved for stdin, stdout and stderr
            return 1;
        } else if (file < (MICROHAL_FATFS_MAXFILES_IN_USE + 3)) {
            if (fileTable[file - 3].isOpen) {  // not a tty file
                r->_errno = ENOTTY;
            } else {  // invalid file descriptor
                r->_errno = EBADF;
            }
        }
    }
    return 0;
}

/*------------------------------------------------------------------------*//**
 * \brief Status of a file (by name).
 * \details Status of a file (by name).
 *
 * \param [in] pathname pPoints to a path name that names a file.
 * \param [out] st points to a stat structure where status information about the
 * file is to be placed.
 * \return 0 for success.
 *//*-------------------------------------------------------------------------*/
extern "C" int _stat_r(struct _reent *r, const char *pathname, struct stat *st) {
    FILINFO fileinfo;
    if (f_stat(pathname, &fileinfo) == FR_OK) {
        decltype(st->st_mode) st_mode = 0;
        if (fileinfo.fattrib == AM_RDO) { /* Read only */
        }
        if (fileinfo.fattrib == AM_HID) { /* Hidden */
        }
        if (fileinfo.fattrib == AM_SYS) { /* System */
        }
        if (fileinfo.fattrib == AM_DIR) { /* Directory */
            st_mode |= S_IFDIR;
        } else {
            st->st_mode |= S_IFCHR;  // S_IFREG
        }
        if (fileinfo.fattrib == AM_ARC) { /* Archive */
        }

        st->st_size = fileinfo.fsize;
        st->st_blksize = 512;
        st->st_nlink = 0;
        //   st->st_mtime = getTimeFromFattie(fileinfo.fdate, fileinfo.ftime);
        return 0;
    }

    return -1;
}

extern "C" int _fstat_r(struct _reent *r, int file, struct stat *st) {
    (void)r;

    if (file > 2 && file < (MICROHAL_FATFS_MAXFILES_IN_USE + 3) && fileTable[file - 3].isOpen) {
        st->st_mode = S_IFCHR;
        st->st_blksize = 512;
        st->st_nlink = 0;
        return 0;
    }
    r->_errno = EBADF;
    return -1;
}

/*------------------------------------------------------------------------*/ /**
 * \brief Open a file.
 * \details Open a file.
 *
 * \param [in] pathname is the name of the file to open.
 * \param [in] flags is the bitwise inclusive-OR of the file access modes and
 * file status flags.
 * \param [in] mode specifies what permissions the file has when it is created.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

BYTE convertFileFlagsFromPOSIXToFatFs(int flags) {
    // FA_READ Specifies read access to the object. Data can be read from the file.
    // FA_WRITE Specifies write access to the object. Data can be written to the file. Combine with FA_READ for read-write access.
    // FA_OPEN_EXISTING Opens the file. The function fails if the file is not existing.
    // FA_CREATE_NEW Creates a new file. The function fails with FR_EXIST if the file is existing.
    // FA_CREATE_ALWAYS Creates a new file. If the file is existing, it will be truncated and overwritten.
    // FA_OPEN_ALWAYS Opens the file if it is existing. If not, a new file will be created.
    // FA_OPEN_APPEND Same as FA_OPEN_ALWAYS except the read/write pointer is set end of the file.

    //  POSIX	FatFs
    // "r"	FA_READ
    // "r+"	FA_READ | FA_WRITE
    // "w"	FA_CREATE_ALWAYS | FA_WRITE
    // "w+"	FA_CREATE_ALWAYS | FA_WRITE | FA_READ
    // "a"	FA_OPEN_APPEND | FA_WRITE
    // "a+"	FA_OPEN_APPEND | FA_WRITE | FA_READ
    // "wx"	FA_CREATE_NEW | FA_WRITE
    // "w+x"	FA_CREATE_NEW | FA_WRITE | FA_READ

    BYTE fatFsMode = 0;

    if ((flags & 0b11) == O_RDONLY) fatFsMode |= FA_READ;  /* read enabled */
    if ((flags & 0b11) == O_WRONLY) fatFsMode |= FA_WRITE; /* write enabled */
    if ((flags & 0b11) == O_RDWR) fatFsMode |= FA_READ | FA_WRITE;
    if (flags & _FAPPEND) fatFsMode |= FA_OPEN_APPEND; /* append (writes guaranteed at the end) */
    if (flags & _FMARK) {                              /* internal; mark during gc() */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FMARK, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FDEFER) { /* internal; defer for next gc pass */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FDEFER, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FASYNC) { /* signal pgrp when data ready */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FASYNC, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FSHLOCK) { /* BSD flock() shared lock present */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FSHLOCK, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FEXLOCK) { /* BSD flock() exclusive lock present */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FEXLOCK, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FTRUNC) { /* open with truncation */
    }
    if (flags & _FCREAT) {               /* open with file create */
        if (flags & _FEXCL)              /* error on open if file exists */
            fatFsMode |= FA_CREATE_NEW;  // create new file, if file exist return error
        else {
            if (flags & _FAPPEND)               /* open with append */
                fatFsMode |= FA_OPEN_APPEND;    // create new file, if file exist append data
            else if (flags & _FTRUNC)           /* open with truncation */
                fatFsMode |= FA_CREATE_ALWAYS;  // create new file, if file exist all existing data will be truncated
            else {
                diagChannel << lock << MICROHAL_ERROR << "i: _FTRUNC, flags value: " << toHex((uint32_t)flags) << unlock;
            }
        }
    } else {
        fatFsMode |= FA_OPEN_EXISTING;
    }
    if (flags & _FNBIO) { /* non blocking I/O (sys5 style) */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FNBIO, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FSYNC) { /* do all writes synchronously */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FSYNC, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FNONBLOCK) { /* non blocking I/O (POSIX style) */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FNONBLOCK, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FNDELAY) { /* non blocking I/O (4.2 style) */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FNDELAY, flags value: " << toHex((uint32_t)flags) << unlock;
    }
    if (flags & _FNOCTTY) { /* don't assign a ctty on this open */
        diagChannel << lock << MICROHAL_WARNING << "Unsupported file option: _FNOCTTY, flags value: " << toHex((uint32_t)flags) << unlock;
    }

    return fatFsMode;
}

extern "C" int _open_r(struct _reent *r, const char *pathname, int flags, int mode) {
    diagChannel << lock << MICROHAL_INFORMATIONAL << "opening file, pathname: " << pathname << ", flags: " << (int32_t)flags
                << " mode: " << (int32_t)mode << unlock;
    for (size_t i = 0; i < MICROHAL_FATFS_MAXFILES_IN_USE; i++) {
        if (fileTable[i].isOpen) continue;
        if (auto result = f_open(&fileTable[i].fp, pathname, convertFileFlagsFromPOSIXToFatFs(flags)); result == FR_OK) {
            fileTable[i].isOpen = true;
            return i + 3;
        } else {
            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs open error: " << printFatFSResult(result) << unlock;
            return -1;
        }
    }
    return -1;
}

/*------------------------------------------------------------------------*/ /**
 * \brief Close a file.
 * \details Close a file.
 *
 * \param [in] file indicates the file descriptor to close.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

extern "C" int _close_r(struct _reent *r, int file) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "Closing file: " << (int32_t)file << unlock;
    if (file > 2 && file < (MICROHAL_FATFS_MAXFILES_IN_USE + 3) && fileTable[file - 3].isOpen) {
        auto res = f_close(&fileTable[file - 3].fp);
        if (res == FR_OK) {
            fileTable[file - 3].isOpen = false;
            return 0;
        } else {
            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs close error: " << printFatFSResult(res) << unlock;
            return -1;
        }
    }
    r->_errno = EBADF;
    return -1;
}

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "Writing file: " << (int32_t)file << ", size: " << (uint32_t)nbyte << unlock;
    if (file > 0) {
        if (file == 1 || file == 2) {  // file number from 1 to 2 are reserved for  stdout and stderr
            bsp::debugPort.mutex.lock();
            size_t toWrite = nbyte;
            do {
                size_t written = bsp::debugPort.write((const char *)buf, toWrite);
                buf += written;
                toWrite -= written;
            } while (toWrite);
            bsp::debugPort.mutex.unlock();
            return nbyte;
        } else if (file < (MICROHAL_FATFS_MAXFILES_IN_USE + 3) && fileTable[file - 3].isOpen) {
            UINT count = 0;
            auto res = f_write(&fileTable[file - 3].fp, buf, nbyte, &count);
            if (res != FR_OK) {
                diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs write error: " << printFatFSResult(res) << unlock;
                return -1;
            }
            return count;
        }
    }
    r->_errno = EBADF;
    return -1;
}

extern "C" ssize_t _read_r(struct _reent *r, int file, void *buf, size_t nbyte) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "reading file: " << (int32_t)file << unlock;
    if (file == 0) {  // read form stdin
                      // todo implement
    } else if (file > 2 && file < (MICROHAL_FATFS_MAXFILES_IN_USE + 3) && fileTable[file - 3].isOpen) {
        UINT count;
        auto res = f_read(&fileTable[file - 3].fp, buf, nbyte, &count);
        if (res != FR_OK) {
            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs read error: " << printFatFSResult(res) << unlock;
            return -1;
        }
        return count;
    }
    r->_errno = EBADF;
    return -1;
}

/*------------------------------------------------------------------------*//**
 * \brief Set position in a file.
 * \details Set position in a file.
 *
 * \param [in] file is the file descriptor of an open file.
 * \param [in] offset specifies the number of bytes to offset the file pointer
 * from a specified file origin.
 * \param [in] whence specifies the location from which to start seeking. 0 - excact value, 1 - offset from current file position, 2 - offset from end of file
 * \return 0 for success.
 *//*-------------------------------------------------------------------------*/
extern "C" off_t _lseek_r(struct _reent *r, int file, off_t offset, int whence) {
    if (file > 2 && file < (MICROHAL_FATFS_MAXFILES_IN_USE + 3) && fileTable[file - 3].isOpen) {
        switch (whence) {
            case SEEK_SET:
                r->_errno = EINVAL;
                break;
            case SEEK_CUR:
                r->_errno = EINVAL;
                break;
            case SEEK_END:
                if (f_lseek(&fileTable[file - 3].fp, offset) == FR_OK) {
                    return fileTable[file - 3].fp.fptr;
                }
                break;
        }
    } else {
        r->_errno = EBADF;
    }
    return -1;
}

extern "C" DWORD get_fattime(void) {
    std::time_t t = std::time(nullptr);
    const auto time = std::gmtime(&t);

    uint_fast8_t sec = time->tm_sec / 2;
    uint_fast8_t min = time->tm_min;
    uint_fast8_t hour = time->tm_hour;
    uint_fast8_t day = time->tm_mday;
    uint_fast8_t month = time->tm_mon;
    uint_fast8_t year = time->tm_year >= 80 ? time->tm_year - 80 : 0;

    uint32_t fattime = (year << 25) | (month << 21) | (day << 16) | (hour << 11) | (min << 5) | (sec);
    return fattime;
}

std::time_t getTimeFromFattie(uint32_t fattime) {
    uint_fast8_t sec = (fattime & 0b11111);
    uint_fast8_t min = (fattime >> 5) & 0b111111;
    uint_fast8_t hour = (fattime >> 11) & 0b11111;
    uint_fast8_t day = (fattime >> 16) & 0b111111;
    uint_fast8_t month = (fattime >> 21) & 0b1111;
    uint_fast8_t year = (fattime >> 25);
    std::tm time;
    time.tm_sec = sec * 2;
    time.tm_min = min;
    time.tm_hour = hour;
    time.tm_mday = day;
    time.tm_mon = month;
    time.tm_year = year + 80;
    return mktime(&time);
}

extern "C" void abort() {
    microhal::hardware::Device::breakpoint();
}

extern "C" caddr_t _sbrk_r(struct _reent *r, int size) {
    extern char __heap_start;  // imported from linker script
    extern char __heap_end;    // imported from linker script
    static char *current_heap_end = &__heap_start;
    char *previous_heap_end;

    (void)r;

    previous_heap_end = current_heap_end;

    if (current_heap_end + size > &__heap_end)  // is there enough space on the heap left?
    {
        errno = ENOMEM;      // not enough memory left
        return (caddr_t)-1;  // return immediatelly
    }

    current_heap_end += size;  // increase data space

    return (caddr_t)previous_heap_end;  // return requested data space
}
struct _reent dummy;
extern "C" caddr_t _sbrk(int size) {
    return _sbrk_r(&dummy, size);
}

extern "C" int _isatty(struct _reent *r, int file) {
    return _isatty_r(&dummy, file);
}

extern "C" void _kill_r() {
    microhal::hardware::Device::breakpoint();
    std::terminate();
}
extern "C" void _getpid_r() {
    microhal::hardware::Device::breakpoint();
    std::terminate();
}

#if FF_USE_LFN == 3 /* Dynamic memory allocation */

/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/

void *ff_memalloc(           /* Returns pointer to the allocated memory block (null if not enough core) */
                  UINT msize /* Number of bytes to allocate */
) {
    return malloc(msize); /* Allocate a new memory block with POSIX API */
}

/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree(void *mblock /* Pointer to the memory block to free (nothing to do if null) */
) {
    free(mblock); /* Free the memory block with POSIX API */
}

#endif

#if FF_FS_REENTRANT
/*------------------------------------------------------------------------*/
/* Create a Synchronization Object
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to create a new
/  synchronization object, such as semaphore and mutex. When a 0 is returned,
/  the f_mount() function fails with FR_INT_ERR.
*/
extern "C" int ff_cre_syncobj(                /* 1:Function succeeded, 0:Could not create the sync object */
                              BYTE vol,       /* Corresponding volume (logical drive number) */
                              FF_SYNC_t *sobj /* Pointer to return the created sync object */
) {
    int ret;
    *sobj = xSemaphoreCreateMutex(); /* FreeRTOS */
    ret = (int)(*sobj != NULL);

    return ret;
}

/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount() function to delete a synchronization
/  object that created with ff_cre_syncobj() function. When a 0 is returned,
/  the f_mount() function fails with FR_INT_ERR.
*/

extern "C" int ff_del_syncobj(               /* 1:Function succeeded, 0:Could not delete due to any error */
                              FF_SYNC_t sobj /* Sync object tied to the logical drive to be deleted */
) {
    vSemaphoreDelete(sobj); /* FreeRTOS */
    return 1;
}

/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a 0 is returned, the file function fails with FR_TIMEOUT.
*/

extern "C" int ff_req_grant(               /* 1:Got a grant to access the volume, 0:Could not get a grant */
                            FF_SYNC_t sobj /* Sync object to wait */
) {
    int ret;
    ret = (int)(xSemaphoreTake(sobj, FF_FS_TIMEOUT) == pdTRUE); /* FreeRTOS */

    return ret;
}

/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
 */

extern "C" void ff_rel_grant(FF_SYNC_t sobj /* Sync object to be signaled */
) {
    xSemaphoreGive(sobj); /* FreeRTOS */
}

#endif
