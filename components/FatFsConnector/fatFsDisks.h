/*
 * fatFsDisks.h
 *
 *  Created on: May 5, 2019
 *      Author: pokas
 */

#ifndef FATFSDISKS_H_
#define FATFSDISKS_H_

#include <string_view>
#include "disk.h"

namespace microhal {
namespace FatFs {

bool addDisk(Disk &disk, std::string_view name);
bool removeDisk(Disk &disk, std::string_view name);
bool format(std::string_view name);

}  // namespace FatFs
}  // namespace microhal

#endif /* FATFSDISKS_H_ */
