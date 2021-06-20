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

bool format(std::string_view name) {
    BYTE work[FF_MAX_SS];
    if (auto status = f_mkfs(name.data(), FM_ANY, 0, work, sizeof(work)); status != FR_OK) {
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
