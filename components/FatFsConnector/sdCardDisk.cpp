#ifndef SDCARDDISK_CPP_
#define SDCARDDISK_CPP_

#include "sdCardDisk.h"

namespace microhal {
namespace FatFs {

DSTATUS SDCardDisk::initialize() {
    if (sdCard.init()) {
        // bsp::sdCardSpi.prescaler(stm32f4xx::SPI::Prescaler16);
        _status &= ~STA_NOINIT;
        return RES_OK;
    }
    return RES_ERROR;
}

DRESULT SDCardDisk::read(BYTE* buff, DWORD sector, UINT count) {
    Sd::Error result;
    if (count == 1) {
        result = sdCard.readBlock(buff, sector);
    } else {
        result = sdCard.readMultipleBlock(buff, sector, count);
    }
    if (result == Sd::Error::None) return RES_OK;
    return RES_ERROR;
}

DRESULT SDCardDisk::write(const BYTE* buff, DWORD sector, UINT count) {
    Sd::Error result;
    if (count == 1)
        result = sdCard.writeBlock(buff, sector);
    else
        result = sdCard.writeMultipleBlock(buff, sector, count);
    if (result == Sd::Error::None) return RES_OK;
    return RES_ERROR;
}

DRESULT SDCardDisk::ioctl(BYTE cmd, void* buff) {
    DRESULT res = RES_ERROR;

    switch (cmd) {
        case CTRL_SYNC: /* Wait for end of internal write process of the drive */
                        //	if (sdCard.)
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT: /* Get drive capacity in unit of sector (DWORD) */
            *(DWORD*)buff = sdCard.getSecrorCount();
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            *(DWORD*)buff = sdCard.getSectorSize();
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE: /* Get erase block size in unit of sector (DWORD) */
            *(DWORD*)buff = sdCard.getBlockSize() / sdCard.getSectorSize();
            res = RES_OK;
            break;
        //
        //			case CTRL_TRIM :	/* Erase a block of sectors (used when _USE_ERASE == 1) */
        //
        //				if (!(CardType & CT_SDC)) break;				/* Check if the card is SDC */
        //				if (disk_ioctl(drv, MMC_GET_CSD, csd)) break;	/* Get CSD */
        //				if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;	/* Check if sector erase can be applied to the card */
        //				dp = buff; st = dp[0]; ed = dp[1];				/* Load sector block */
        //				if (!(CardType & CT_BLOCK)) {
        //					st *= 512; ed *= 512;
        //				}
        //				if (send_cmd(CMD32, st) == 0 && send_cmd(CMD33, ed) == 0 && send_cmd(CMD38, 0) == 0 &&
        // wait_ready(30000))
        //{
        ///* Erase sector block */
        //					res = RES_OK;	/* FatFs does not check result of this command */
        //				}
        //				break;
        //
        default:
            res = RES_PARERR;
    }
    //
    //			deselect();

    return res;
}

}  // namespace FatFs
}  // namespace microhal

#endif /* SDCARDDISK_CPP_ */
