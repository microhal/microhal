/*
 * ramdisk.h
 *
 *  Created on: May 5, 2019
 *      Author: pokas
 */

#ifndef RAMDISK_H_
#define RAMDISK_H_

#include <cstdint>
#include <cstring>  // for size_t and memcpy
#include "disk.h"
#include "diskio.h"
#include "ff.h"

namespace microhal {
namespace FatFs {

class RAMDisk final : public Disk {
 public:
    static constexpr const size_t sectorSize = 512;

    RAMDisk(uint8_t* dataPtr, size_t dataSize) : data(dataPtr), sectorCount(dataSize / sectorSize) { _status = STA_NOINIT; }

    DSTATUS initialize() final;
    DRESULT read(BYTE* buff, DWORD sector, UINT count) final;
    DRESULT write(const BYTE* buff, DWORD sector, UINT count) final;
    DRESULT ioctl(BYTE cmd, void* buff) final;

 private:
    uint8_t* data;
    size_t sectorCount;
};

}  // namespace FatFs
}  // namespace microhal

#endif /* RAMDISK_H_ */
