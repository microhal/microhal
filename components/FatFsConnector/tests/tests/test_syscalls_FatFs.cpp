#include "CppUTest/TestHarness.h"
#include "bsp.h"
#include "cstdio"
#include "ff.h"
#include "string_view"

using namespace microhal;
using namespace std::literals;

BYTE g_mode;
const TCHAR* g_path;

extern "C" FRESULT f_open(FIL* fp,           /* Pointer to the blank file object */
                          const TCHAR* path, /* Pointer to the file name */
                          BYTE mode          /* Access mode and file open mode flags */
) {
    g_path = path;
    g_mode = mode;
    return FR_OK;
}

extern "C" FRESULT f_close(FIL* fp /* Pointer to the file object to be closed */
) {
    return FR_OK;
}

extern "C" FRESULT f_read(FIL* fp,    /* Pointer to the file object */
                          void* buff, /* Pointer to data buffer */
                          UINT btr,   /* Number of bytes to read */
                          UINT* br    /* Pointer to number of bytes read */
) {
    return FR_OK;
}

extern "C" FRESULT f_write(FIL* fp,          /* Pointer to the file object */
                           const void* buff, /* Pointer to the data to be written */
                           UINT btw,         /* Number of bytes to write */
                           UINT* bw          /* Pointer to number of bytes written */
) {
    return FR_OK;
}

extern "C" FRESULT f_lseek(FIL* fp,    /* Pointer to the file object */
                           FSIZE_t ofs /* File pointer from top of file */
) {
    return FR_OK;
}

TEST_GROUP(FatFsFopenMockTest){};

TEST(FatFsFopenMockTest, TestFopen) {
    /**
     * This test check if correct values are passed to FatFs f_open function
     */
    auto filename = "file.file"sv;
    {
        FILE* f = fopen("file.file", "r");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == FA_READ);
    }

    {
        FILE* f = fopen("file.file", "r+");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == (FA_READ | FA_WRITE));
    }
    {
        FILE* f = fopen("file.file", "w");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == (FA_CREATE_ALWAYS | FA_WRITE));
    }
    {
        FILE* f = fopen("file.file", "w+");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == (FA_CREATE_ALWAYS | FA_WRITE | FA_READ));
    }
    {
        FILE* f = fopen("file.file", "a");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == (FA_OPEN_APPEND | FA_WRITE));
    }
    {
        FILE* f = fopen("file.file", "a+");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == (FA_OPEN_APPEND | FA_WRITE | FA_READ));
    }
    {
        FILE* f = fopen("file.file", "wx");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == (FA_CREATE_NEW | FA_WRITE));
    }
    {
        FILE* f = fopen("file.file", "w+x");
        CHECK(f);
        CHECK(filename == g_path);
        CHECK(g_mode == (FA_CREATE_NEW | FA_WRITE | FA_READ));
    }
}
