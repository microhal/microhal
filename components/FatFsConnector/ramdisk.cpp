#include "ramdisk.h"

namespace microhal {
namespace FatFs {

DSTATUS RAMDisk::initialize() {
    if (status() & STA_NOINIT) {
        memset(data, 0x00, sectorSize * sectorCount);
        _status &= ~STA_NOINIT;
        return RES_OK;
    }
    return RES_OK;
    // return RES_ERROR;
}

DRESULT RAMDisk::read(BYTE* buff, DWORD sector, UINT count) {
    if ((sector + count) > sectorCount) return RES_ERROR;
    memcpy(buff, &data[sector * sectorSize], count * sectorSize);
    return RES_OK;
}
DRESULT RAMDisk::write(const BYTE* buff, DWORD sector, UINT count) {
    if ((sector + count) > sectorCount) return RES_ERROR;
    memcpy(&data[sector * sectorSize], buff, count * sectorSize);
    return RES_OK;
}
DRESULT RAMDisk::ioctl(BYTE cmd, void* buff) {
    DRESULT result;

    switch (cmd) {
        case CTRL_SYNC:
            result = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            *(DWORD*)buff = sectorCount;
            result = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            *(WORD*)buff = sectorSize;
            result = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1;
            result = RES_OK;
            break;

        default:
            result = RES_ERROR;
            break;
    }

    return result;
}

}  // namespace FatFs
}  // namespace microhal
