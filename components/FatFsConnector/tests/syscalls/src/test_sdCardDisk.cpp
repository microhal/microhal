#include <cstdio>
#include <string_view>
#include "bsp.h"
#include "doctest.h"
#include "fatFsDisks.h"
#include "ff.h"
#include "sd.h"
#include "sdCardDisk.h"

using namespace microhal;
using namespace std::literals;

Sd sdCard(bsp::sdCardSpi, bsp::sdCardCs);
FatFs::SDCardDisk sdDisk(sdCard);

TEST_CASE("syscalls test SD disk") {
    // FatFs error status into POSIX error codes check

    FatFs::addDisk(sdDisk, "0");
    {
        FILE* f = fopen("nonexisting.fil", "r");
        CHECK_FALSE(f);
    }
    {
        FILE* f = fopen("nonexisting.fil", "r+");
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

TEST_CASE("syscalls test SD disk file write") {
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
