/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      FatFS connector Disks manipulation functions implementation
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

#include "fatFsDisks.h"
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "disk.h"
#include "diskio.h" /* FatFs lower layer API */
#include "fatFsStatusToString.h"
#include "ff.h"

using namespace microhal;
using namespace diagnostic;

static FATFS FS[FF_VOLUMES];
std::array<FatFs::Disk *, FF_VOLUMES> disks;

namespace microhal {
namespace FatFs {

bool addDisk(Disk &disk, std::string_view name) {
    if (auto value = fatFs_get_ldnumber(name.data()); value >= 0) {
        disks[value] = &disk;
        f_mount(&FS[value], name.data(), 0);
        return true;
    }
    return false;
}

bool removeDisk(Disk &disk, std::string_view name) {
    if (auto value = fatFs_get_ldnumber(name.data()); value >= 0) {
        if (auto status = f_mount(nullptr, name.data(), 0); status == FR_OK) {
            disks[value] = nullptr;
            return true;
        } else {
            diagChannel << lock << MICROHAL_ERROR << "FatFs disk remove: " << printFatFSResult(status) << unlock;
        }
    }
    return false;
}

bool format(std::string_view name, Format format) {
    BYTE work[FF_MAX_SS];
    if (auto status = f_mkfs(name.data(), static_cast<BYTE>(format), 0, work, sizeof(work)); status != FR_OK) {
        diagChannel << lock << MICROHAL_ERROR << "FatFs disk format: " << printFatFSResult(status) << unlock;
        return false;
    }
    return true;
}

}  // namespace FatFs
}  // namespace microhal
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv /* Physical drive number to identify the drive */
) {
    if (disks[pdrv]) {
        auto status = disks[pdrv]->status();
        diagChannel << lock << MICROHAL_INFORMATIONAL << "Checking disk: " << pdrv << " status: " << status << unlock;
        return status;
    }
    diagChannel << lock << MICROHAL_ERROR << "Checking status of non existing disk: " << pdrv << unlock;
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv /* Physical drive number to identify the drive */
) {
    if (disks[pdrv]) {
        diagChannel << lock << MICROHAL_INFORMATIONAL << "Initializing disk: " << pdrv << unlock;
        return disks[pdrv]->initialize();
    }
    diagChannel << lock << MICROHAL_ERROR << "Initializing non existing disk: " << pdrv << unlock;
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv,    /* Physical drive nmuber to identify the drive */
                  BYTE *buff,   /* Data buffer to store read data */
                  DWORD sector, /* Start sector in LBA */
                  UINT count    /* Number of sectors to read */
) {
    if (disks[pdrv]) {
        return disks[pdrv]->read(buff, sector, count);
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE pdrv,        /* Physical drive nmuber to identify the drive */
                   const BYTE *buff, /* Data to be written */
                   DWORD sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
) {
    if (disks[pdrv]) {
        return disks[pdrv]->write(buff, sector, count);
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
                   BYTE cmd,  /* Control code */
                   void *buff /* Buffer to send/receive control data */
) {
    if (disks[pdrv]) {
        return disks[pdrv]->ioctl(cmd, buff);
    }
    return RES_PARERR;
}
