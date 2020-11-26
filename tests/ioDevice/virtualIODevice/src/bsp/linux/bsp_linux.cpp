/*
 * bsp_linux.cpp
 *
 *  Created on: May 21, 2019
 *      Author: pokas
 */

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "ports/linux/General/consoleIODevice_linux.h"

using namespace microhal;
using namespace diagnostic;

namespace bsp {
IODevice &debugPort = linux::consoleIODev;
bool init() {
    diagChannel.setOutputDevice(debugPort);
    return true;
}

void deinit() {}

}  // namespace bsp
