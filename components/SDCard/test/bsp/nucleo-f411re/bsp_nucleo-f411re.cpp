/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#include "bsp.h"
#include "microhal.h"
#include "nucleo_f103rb_extension_v0_1_0.h"

using namespace microhal;
using namespace stm32f4xx;

namespace bsp {
// stm32f4xx::GPIO cs_stm({IOPin::PortA, 15}, stm32f4xx::GPIO::Direction::Output);
stm32f4xx::GPIO cs_stm(con1::a::cs, stm32f4xx::GPIO::Direction::Output);

namespace sdCard {
microhal::stm32f4xx::SPI_dma &spi1 = microhal::stm32f4xx::SPI_dma::create<1, con1::a::miso, con1::a::mosi, con1::a::sck>();
microhal::SPI &spi = spi1;
microhal::GPIO &cs = cs_stm;
}  // namespace sdCard

bool init() {
    bsp::debugPort.clear();

    bsp::debugPort.setDataBits(microhal::SerialPort::Data8);
    bsp::debugPort.setStopBits(microhal::SerialPort::OneStop);
    bsp::debugPort.setParity(microhal::SerialPort::NoParity);
    bsp::debugPort.open(microhal::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(microhal::SerialPort::Baud115200);

    sdCard::spi1.init(stm32f4xx::SPI::Mode0, stm32f4xx::SPI::Prescaler128);
    sdCard::spi1.enable();

    return true;
}

void deinit() {}
}  // namespace bsp

// extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
//    return bsp::debugPort.write((const char *)buf, nbyte);
//}

extern "C" int main(int, void *);

static void run_main(void *) {
    main(0, nullptr);
    while (1)
        ;
}

void hardwareConfig(void) {
    // Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<2, Txd, {stm32f4xx::IOPin::PortA, 2}>();
    IOManager::routeSerial<2, Rxd, {stm32f4xx::IOPin::PortA, 3}>();

    //    IOManager::routeSPI<1, SCK, {IOPin::PortA, 5}>();
    //    IOManager::routeSPI<1, MISO, {IOPin::PortA, 6}>();
    //    IOManager::routeSPI<1, MOSI, {IOPin::PortA, 7}>();

    TaskHandle_t xHandle = NULL;

    xTaskCreate(run_main, "main", 5 * 1024, NULL, tskIDLE_PRIORITY, &xHandle);

    vTaskStartScheduler();
}

extern "C" void _fstat_r() {}
extern "C" void _kill_r() {}
extern "C" void _getpid_r() {}
extern "C" void abort() {}
extern "C" void _lseek_r() {}

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    (void)r;  // suppress warning
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "Writing file: " << (int32_t)file << ", size: " << (uint32_t)nbyte << unlock;
    if (file == 10) {
        //	UINT count = 0;
        //	auto res = f_write(&fp, buf, nbyte, &count);
        //	if (res != FR_OK) {
        //		diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs write error: "<< printFatFSResult(res) << unlock;
        //		return -1;
        //	}
        //	return count;
    } else {
        bsp::debugPort.mutex.lock();
        size_t toWrite = nbyte;
        do {
            size_t written = bsp::debugPort.write((const char *)buf, toWrite);
            buf += written;
            toWrite -= written;
        } while (toWrite);
        bsp::debugPort.mutex.unlock();
    }
    return 0;
}

extern "C" int _open_r(struct _reent *r, const char *pathname, int flags, int mode) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "opening file, pathname: " << pathname << ", flags: " << (int32_t)flags
    //           << " mode: " << (int32_t)mode << unlock;
    //    if (f_open(&fp, pathname, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
    //        return 10;
    //    }
    return -1;
}

extern "C" int _close_r(struct _reent *r, int file) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "Closing file: " << (int32_t)file << unlock;
    if (file == 10) {
        //        auto res = f_close(&fp);
        //        if (res == FR_OK) {
        //            return 0;
        //        } else {
        //            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs close error: " << printFatFSResult(res) << unlock;
        //        }
    }

    return -1;
}

extern "C" ssize_t _read_r(struct _reent *r, int file, void *buf, size_t nbyte) {
    //    diagChannel << lock << MICROHAL_INFORMATIONAL << "reading file: " << (int32_t)file << unlock;
    //    if (file == 10) {
    //        UINT count;
    //        auto res = f_read(&fp, buf, nbyte, &count);
    //        if (res != FR_OK) {
    //            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs read error: " << printFatFSResult(res) << unlock;
    //            return -1;
    //        }
    //        return nbyte - count;
    //    }
    return 0;
}
