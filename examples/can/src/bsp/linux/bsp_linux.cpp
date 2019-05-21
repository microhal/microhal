/*
 * bsp_linux.cpp
 *
 *  Created on: May 21, 2019
 *      Author: pokas
 */

#include "bsp.h"
#include "ports/linux/General/consoleIODevice_linux.h"

using namespace microhal;

namespace bsp {
IODevice &debugPort = linux::consoleIODev;
bool init() {
    return true;
}

void deinit() {}

}  // namespace bsp
