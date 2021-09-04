/** \file syscalls.c
 * \brief System calls for newlib's libc
 * \details System calls for newlib's libc
 *
 * \author Freddie Chopin, http://www.freddiechopin.info/
 * \date 2010-11-11
 */

/******************************************************************************
 * project: syscalls
 * chip: (any)
 * compiler: arm-none-eabi-gcc (Sourcery G++ Lite 2010q1-188) 4.4.1
 *
 * prefix: (none)
 *
 * available global functions:
 * 	int _close_r(struct _reent *r, int file)
 * 	int _execve_r(struct _reent *r, const char *path, char *const argv[],
 *char *const envp[])
 * 	void _exit(int status)
 * 	pid_t _fork_r(struct _reent *r)
 * 	int _fstat_r(struct _reent *r, int file, struct stat *st)
 * 	pid_t _getpid_r(struct _reent *r)
 * 	int _isatty_r(struct _reent *r, int file)
 * 	int _kill_r(struct _reent *r, pid_t pid, int signal)
 * 	int _link_r(struct _reent *r, const char *old, const char *new)
 * 	off_t _lseek_r(struct _reent *r, int file, off_t offset, int whence)
 * 	int _open_r(struct _reent *r, const char *pathname, int flags, int mode)
 * 	ssize_t _read_r(struct _reent *r, int file, void *buf, size_t nbyte)
 * 	caddr_t _sbrk_r(struct _reent *r, int size)
 * 	int _stat_r(struct _reent *r, const char *pathname, struct stat *st)
 * 	int _times_r(struct _reent *r, struct tms *buf)
 * 	int _unlink_r(struct _reent *r, const char *pathname)
 * 	pid_t _wait_r(struct _reent *r, int *stat_loc)
 * 	ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t
 *nbyte)
 *
 * available local functions:
 *
 * available interrupt handlers:
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*
 +=============================================================================+
 | configuration
 +=============================================================================+
 */
#ifndef SYSCALLS_HAVE_CLOSE_R
#define SYSCALLS_HAVE_CLOSE_R 1
#endif

#ifndef SYSCALLS_HAVE_EXECVE_R
#define SYSCALLS_HAVE_EXECVE_R 1
#endif

#ifndef SYSCALLS_HAVE_EXIT
#define SYSCALLS_HAVE_EXIT 1
#endif

#ifndef SYSCALLS_HAVE_FORK_R
#define SYSCALLS_HAVE_FORK_R 1
#endif

#ifndef SYSCALLS_HAVE_FSTAT_R
#define SYSCALLS_HAVE_FSTAT_R 1
#endif

#ifndef SYSCALLS_HAVE_GETPID_R
#define SYSCALLS_HAVE_GETPID_R 1
#endif

#ifndef SYSCALLS_HAVE_ISATTY_R
#define SYSCALLS_HAVE_ISATTY_R 1
#endif

#ifndef SYSCALLS_HAVE_KILL_R
#define SYSCALLS_HAVE_KILL_R 1
#endif

#ifndef SYSCALLS_HAVE_LINK_R
#define SYSCALLS_HAVE_LINK_R 1
#endif

#ifndef SYSCALLS_HAVE_LSEEK_R
#define SYSCALLS_HAVE_LSEEK_R 1
#endif

#ifndef SYSCALLS_HAVE_OPEN_R
#define SYSCALLS_HAVE_OPEN_R 1
#endif

#ifndef SYSCALLS_HAVE_READ_R
#define SYSCALLS_HAVE_READ_R 1
#endif

#ifndef SYSCALLS_HAVE_SBRK_R
#define SYSCALLS_HAVE_SBRK_R 1
#endif

#ifndef SYSCALLS_HAVE_STAT_R
#define SYSCALLS_HAVE_STAT_R 1
#endif

#ifndef SYSCALLS_HAVE_TIMES_R
#define SYSCALLS_HAVE_TIMES_R 1
#endif

#ifndef SYSCALLS_HAVE_UNLINK_R
#define SYSCALLS_HAVE_UNLINK_R 1
#endif

#ifndef SYSCALLS_HAVE_WAIT_R
#define SYSCALLS_HAVE_WAIT_R 1
#endif

#ifndef SYSCALLS_HAVE_WRITE_R
#define SYSCALLS_HAVE_WRITE_R 1
#endif
/*
 +=============================================================================+
 | includes
 +=============================================================================+
 */

#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>

/*
 +=============================================================================+
 | module variables
 +=============================================================================+
 */

char *environ_initial[1] = {0};
char **environ = environ_initial;

#undef errno
extern int errno;

/*
 +=============================================================================+
 | local functions' declarations
 +=============================================================================+
 */
int native_handle_type;
/*
 +=============================================================================+
 | global functions
 +=============================================================================+
 */

#if SYSCALLS_HAVE_CLOSE_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Close a file.
 * \details Close a file.
 *
 * \param [in] file indicates the file descriptor to close.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

int _close_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] int file) {
    return -1;
}

#endif

#if SYSCALLS_HAVE_EXECVE_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Transfer control to a new process.
 * \details Transfer control to a new process.
 *
 * \param [in] path specifies the path name of the new process image file.
 * \param [in] argv is the argument list for the new process image. This should
 * contain an array of pointers to character strings, and the array should be
 * terminated by a NULL pointer. The value in argv[0] should point to a file
 * name that is associated with the process being started by the exec()
 * function.
 * \param [in] envp specifies the environment for the new process image. This
 * should contain an array of pointers to character strings, and the array
 * should be terminated by a NULL pointer.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

int _execve_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] const char *path, [[maybe_unused]] char *const argv[],
              [[maybe_unused]] char *const envp[]) {
    // errno = ENOMEM;
    return -1;
}

#endif

#if SYSCALLS_HAVE_EXIT == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Exit a program.
 * \details Exit a program without cleaning up files.
 *
 * \param [in] status represents the termination status.
 */ /*-------------------------------------------------------------------------*/

void _exit([[maybe_unused]] int status) {
    while (1)
        ;
}

#endif

#if SYSCALLS_HAVE_FORK_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Create a new process.
 * \details Create a new process.
 *
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

pid_t _fork_r([[maybe_unused]] struct _reent *r) {
    // errno = EAGAIN;
    return -1;
}

#endif

#if SYSCALLS_HAVE_FSTAT_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Status of an open file.
 * \details Status of an open file.
 *
 * \param [in] file is a file descriptor referring to a file for which status is
 * returned.
 * \param [out] st points to a stat structure where status information about the
 * file is to be placed.
 * \return 0 for success.
 */ /*-------------------------------------------------------------------------*/

int _fstat_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] int file, [[maybe_unused]] struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

#endif

#if SYSCALLS_HAVE_GETPID_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Get calling process ID.
 * \details Get calling process ID.
 *
 * \return 1.
 */ /*-------------------------------------------------------------------------*/

pid_t _getpid_r([[maybe_unused]] struct _reent *r) {
    return 1;
}

#endif

#if SYSCALLS_HAVE_ISATTY_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Query whether output stream is a terminal.
 * \details Query whether output stream is a terminal.
 *
 * \param [in] file is the file descriptor to test.
 * \return 1.
 */ /*-------------------------------------------------------------------------*/

int _isatty_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] int file) {
    return 1;
}

#endif

#if SYSCALLS_HAVE_KILL_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Send a signal.
 * \details Send a signal.
 *
 * \param [in] pid is a process ID or process group ID.
 * \param [in] signal specifies the signal to be sent.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

int _kill_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] pid_t pid, [[maybe_unused]] int signal) {
    return -1;
}

#endif

#if SYSCALLS_HAVE_LINK_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Establish a new name for an existing file.
 * \details Establish a new name for an existing file.
 *
 * \param [in] old points to a path name that names an existing file.
 * \param [in] new points to a path name that names the new directory entry to
 * be created.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

int _link_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] const char *old, [[maybe_unused]] const char *new) {
    return -1;
}

#endif

#if SYSCALLS_HAVE_LSEEK_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Set position in a file.
 * \details Set position in a file.
 *
 * \param [in] file is the file descriptor of an open file.
 * \param [in] offset specifies the number of bytes to offset the file pointer
 * from a specified file origin.
 * \param [in] whence specifies the location from which to start seeking.
 * \return 0 for success.
 */ /*-------------------------------------------------------------------------*/

off_t _lseek_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] int file, off_t offset, [[maybe_unused]] int whence) {
    return 0;
}

#endif

#if SYSCALLS_HAVE_OPEN_R == 1

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

int _open_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] const char *pathname, [[maybe_unused]] int flags, [[maybe_unused]] int mode) {
    return -1;
}

#endif

#if SYSCALLS_HAVE_READ_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Read from a file.
 * \details Read from a file.
 *
 * \param [in] file is the file descriptor that references an open file.
 * \param [in] buf points to the buffer to place the read information into.
 * \param [in] nbyte specifies the maximum number of bytes to attempt to read.
 * \return 0 for success.
 */ /*-------------------------------------------------------------------------*/

ssize_t _read_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] int file, [[maybe_unused]] void *buf, [[maybe_unused]] size_t nbyte) {
    return 0;
}

#endif

#if SYSCALLS_HAVE_SBRK_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Increase program data space.
 * \details Increase program data space. This version of _sbrk_r() requires the
 * heap area to be defined explicitly in linker script with symbols __heap_start
 * and __heap_end.
 *
 * \param [in] size is the requested data space size.
 * \return pointer to new data space.
 */ /*-------------------------------------------------------------------------*/

caddr_t _sbrk_r(struct _reent *r, int size) {
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

#endif

#if SYSCALLS_HAVE_STAT_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Status of a file (by name).
 * \details Status of a file (by name).
 *
 * \param [in] pathname pPoints to a path name that names a file.
 * \param [out] st points to a stat structure where status information about the
 * file is to be placed.
 * \return 0 for success.
 */ /*-------------------------------------------------------------------------*/

int _stat_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] const char *pathname, [[maybe_unused]] struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

#endif

#if SYSCALLS_HAVE_TIMES_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Timing information for current process.
 * \details Timing information for current process.
 *
 * \param [out] buf points to a tms structure where timing information about the
 * process is to be placed.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

int _times_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] struct tms *buf) {
    return -1;
}

#endif

#if SYSCALLS_HAVE_UNLINK_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Remove a file�s directory entry.
 * \details Remove a file�s directory entry.
 *
 * \param [in] pathname points to the path name that names the file to be
 * unlinked.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

int _unlink_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] const char *pathname) {
    return -1;
}

#endif

#if SYSCALLS_HAVE_WAIT_R == 1

/*------------------------------------------------------------------------*/ /**
 * \brief Wait for a child process.
 * \details Wait for a child process.
 *
 * \param [in,out] stat_loc specifies the location where the child process' exit
 * status is stored.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

pid_t _wait_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] int *stat_loc) {
    // errno = ECHILD;
    return -1;
}

#endif

#ifdef __cplusplus
}
#endif

/******************************************************************************
 * END OF FILE
 ******************************************************************************/
