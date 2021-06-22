
#include <cstdio>
#include <string_view>
#include "CppUTest/TestHarness.h"
#include "fatFsDisks.h"
#include "ff.h"
#include "ramdisk.h"

using namespace microhal;
using namespace std::literals;

// create ramdisk area
constexpr const size_t size = 129 * 512;
uint8_t disk[size];
FatFs::RAMDisk ramdisk(disk, size);

TEST_GROUP(FatFsRamdiskTest){};

TEST(FatFsRamdiskTest, ramdisk) {
    // this test is testing two things, proper implementation of RAM Disk and proper syscalls implementation, mainly we are testing conversion from
    // FatFs error status into POSIX error codes

    CHECK(FatFs::addDisk(ramdisk, "0:"));
    CHECK(FatFs::format("0:", FatFs::Format::FAT | FatFs::Format::SFD));

    {
        FILE* f = fopen("file.fil", "w");
        CHECK(f);
    }

    CHECK(FatFs::removeDisk(ramdisk, "0:"));
}
