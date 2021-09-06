/*
 * bsp_linux.cpp
 *
 *  Created on: May 22, 2021
 *      Author: pokas
 */

#include <ports/linux/General/consoleIODevice_linux.h>
#include "bsp.h"

using namespace microhal;

namespace bsp {

microhal::IODevice &debugPort = linux::consoleIODev;

bool init() {
    return true;
}

}  // namespace bsp
