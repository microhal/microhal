/*
 * flash_stmCommon.cpp
 *
 *  Created on: Feb 27, 2021
 *      Author: pokas
 */

#include "flash_stmCommon.h"
#include <cstddef>
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace Flash {

static void busyWait();
static bool isBusy();
void readErrors();

void unlock() {
    registers::flash->keyr.volatileStore(0x4567'0123);
    registers::flash->keyr.volatileStore(0xCDEF'89AB);
}

void lock() {
    auto cr = registers::flash->cr.volatileLoad();
    cr.LOCK.set();
    registers::flash->cr.volatileStore(cr);
}

void erasePage(uint16_t pageNumber) {
    busyWait();
    readErrors();
    auto cr = registers::flash->cr.volatileLoad();
    cr.PER.set();
    cr.PNB = pageNumber;
    registers::flash->cr.volatileStore(cr);
    cr.STRT.set();
    registers::flash->cr.volatileStore(cr);
    busyWait();
}

void writePage(uint16_t pageNumber, uint32_t *data, uint32_t size) {
    busyWait();
    readErrors();
    auto cr = registers::flash->cr.volatileLoad();
    cr.PG.set();
    cr.PNB = pageNumber;
    registers::flash->cr.volatileStore(cr);

    volatile uint32_t *ptr = reinterpret_cast<uint32_t *>(flashBegin + (uint32_t(pageNumber) * pageSize));
    for (size_t i = 0; i < size; i += 2) {
        ptr[i] = data[i];
        ptr[i + 1] = data[i + 1];
        busyWait();
        auto sr = registers::flash->sr.volatileLoad();
        diagChannel << diagnostic::lock << MICROHAL_DEBUG << "EOP: " << sr.EOP.get() << diagnostic::unlock;
        sr.EOP.clear();
        registers::flash->sr.volatileStore(sr);
    }
    cr = registers::flash->cr.volatileLoad();
    cr.PG.clear();
    registers::flash->cr.volatileStore(cr);
}

void readErrors() {
    auto sr = registers::flash->sr.volatileLoad();
    diagChannel << diagnostic::lock << MICROHAL_DEBUG << "Flash Errors:" << endl
                << "\tOPTVERR: Option and Engineering bits loading validity error: " << sr.OPTVERR.get()
                << "\tRDERR: PCROP read error: " << sr.RDERR.get() << endl
                << "\tFASTERR: Fast programming error: " << sr.FASTERR << endl
                << "\tMISSERR: Fast programming data miss error: " << sr.MISERR.get() << endl
                << "\tPGSERR: Programming sequence error: " << sr.PGSERR.get() << endl
                << "\tSIZERR: Size error: " << sr.SIZERR.get() << endl
                << "\tPGAERR: Programming alignment error: " << sr.PGAERR.get() << endl
                << "\tWRPERR: Write protection error: " << sr.WRPERR.get() << endl
                << "\tPROGERR: Programming error: " << sr.PROGERR.get() << endl
                << "\tOPERR: Operation error" << sr.OPERR.get() << diagnostic::unlock;
}

void busyWait() {
    while (isBusy()) {
    }
}

bool isBusy() {
    return registers::flash->sr.volatileLoad().BSY;
}

}  // namespace Flash
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
