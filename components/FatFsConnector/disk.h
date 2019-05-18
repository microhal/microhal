/*
 * disk.h
 *
 *  Created on: May 5, 2019
 *      Author: pokas
 */

#ifndef SRC_DISK_H_
#define SRC_DISK_H_

#include "diskio.h"

namespace microhal {
namespace FatFs {

class Disk {
 public:
    virtual ~Disk() {}
    virtual DSTATUS initialize() = 0;
    DSTATUS status() const { return _status; }
    virtual DRESULT read(BYTE* buff, DWORD sector, UINT count) = 0;
    virtual DRESULT write(const BYTE* buff, DWORD sector, UINT count) = 0;
    virtual DRESULT ioctl(BYTE cmd, void* buff) = 0;

 protected:
    DSTATUS _status = STA_NOINIT;
};

}  // namespace FatFs
}  // namespace microhal

#endif /* SRC_DISK_H_ */
