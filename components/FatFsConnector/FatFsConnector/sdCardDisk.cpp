/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      FatFS connector SDCard disk implementation
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2019-2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SDCARDDISK_CPP_
#define SDCARDDISK_CPP_

#include "sdCardDisk.h"

namespace microhal {
namespace FatFs {

DSTATUS SDCardDisk::initialize() {
    if (sdCard.init()) {
        const uint32_t requestedCLKFrquency = 20'000'000;  // in [Hz]
        const auto newSpeed = sdCard.setSpeed(std::min(requestedCLKFrquency, sdCard.getMaxSpeed()));
        assert(newSpeed <= requestedCLKFrquency);
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
