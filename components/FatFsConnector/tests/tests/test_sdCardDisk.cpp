#include <sys/stat.h>
#include <cstdio>
#include <string_view>
#include "CppUTest/TestHarness.h"
#include "bsp.h"
#include "fatFsDisks.h"
#include "ff.h"
#include "sd.h"
#include "sdCardDisk.h"

using namespace microhal;
using namespace std::literals;

TEST_GROUP(FatFsSyscallsTest){};

TEST(FatFsSyscallsTest, SDDiskOpenFile) {
    // FatFs error status into POSIX error codes check

    Sd sdCard(bsp::sdCardSpi, bsp::sdCardCs);
    FatFs::SDCardDisk sdDisk(sdCard);

    FatFs::addDisk(sdDisk, "0");
    {
        FILE* f = fopen("missing.fil", "r");
        CHECK_FALSE(f);
    }
    {
        FILE* f = fopen("missing.fil", "r+");
        CHECK_FALSE(f);
    }
    {
        FILE* f = fopen("file.fil", "w");
        CHECK(f);
        fclose(f);
    }
    {
        FILE* f = fopen("file.fil", "w+");
        CHECK(f);
        fclose(f);
    }
    {
        FILE* f = fopen("file.fil", "a");
        CHECK(f);
        fclose(f);
    }
    {
        FILE* f = fopen("file.fil", "a+");
        CHECK(f);
        fclose(f);
    }

    FatFs::removeDisk(sdDisk, "0");
}

TEST(FatFsSyscallsTest, SDDiskFileWrite) {
    Sd sdCard(bsp::sdCardSpi, bsp::sdCardCs);
    FatFs::SDCardDisk sdDisk(sdCard);

    FatFs::addDisk(sdDisk, "0");

    {
        FILE* f = fopen("file.fil", "w");
        CHECK(f);
        auto testString = "teststring"sv;
        CHECK(fwrite(testString.data(), 0, 0, f) == 0);
        CHECK(fwrite(testString.data(), 1, 0, f) == 0);
        CHECK(fwrite(testString.data(), 0, testString.size(), f) == 0);
        CHECK(fwrite(testString.data(), 1, testString.size(), f) == testString.size());
        fclose(f);
    }
    {
        FILE* f = fopen("file.fil", "w+");
        CHECK(f);
        fclose(f);
    }
    {
        FILE* f = fopen("file.fil", "a");
        CHECK(f);
        fclose(f);
    }
    {
        FILE* f = fopen("file.fil", "a+");
        CHECK(f);
        fclose(f);
    }

    FatFs::removeDisk(sdDisk, "0");
}

TEST(FatFsSyscallsTest, SDDiskFileRead) {
    Sd sdCard(bsp::sdCardSpi, bsp::sdCardCs);
    FatFs::SDCardDisk sdDisk(sdCard);

    FatFs::addDisk(sdDisk, "0");

    {
        FILE* f = fopen("file.fil", "w");
        CHECK(f);
        auto testString = "teststring"sv;
        CHECK(fwrite(testString.data(), 1, testString.size(), f) == testString.size());
        fclose(f);
    }
    {
        FILE* f = fopen("file.fil", "r");
        CHECK(f);
        char buffer[20];
        CHECK(fread(buffer, 1, 0, f) == 0);
        CHECK(fread(buffer, 0, sizeof(buffer), f) == 0);

        auto correctString = "teststring"sv;
        auto readLen = fread(buffer, 1, sizeof(buffer), f);
        CHECK_EQUAL(correctString.size(), readLen);
        STRNCMP_EQUAL(correctString.data(), buffer, correctString.size());
        fclose(f);
    }
    {
        // create directory
        CHECK(mkdir("/logs", 0700) == 0);
        struct stat st = {0};
        CHECK(stat("/logs", &st) == 0);
        CHECK(st.st_mode & S_IFDIR);
    }

    FatFs::removeDisk(sdDisk, "0");
}
