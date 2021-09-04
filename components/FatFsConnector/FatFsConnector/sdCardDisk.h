/*
 * sdCardDisk.h
 *
 *  Created on: May 5, 2019
 *      Author: pokas
 */

#ifndef SDCARDDISK_H_
#define SDCARDDISK_H_

#include <cstdint>
#include "disk.h"
#include "diskio.h"
#include "ff.h"
#include "sd.h"

namespace microhal {
namespace FatFs {

class SDCardDisk final : public Disk {
 public:
    SDCardDisk(Sd& sd) : sdCard(sd) {}

    DSTATUS initialize();
    DRESULT read(BYTE* buff, DWORD sector, UINT count);
    DRESULT write(const BYTE* buff, DWORD sector, UINT count);
    DRESULT ioctl(BYTE cmd, void* buff);

 private:
    Sd sdCard;
};

}  // namespace FatFs
}  // namespace microhal

#endif /* SDCARDDISK_H_ */
