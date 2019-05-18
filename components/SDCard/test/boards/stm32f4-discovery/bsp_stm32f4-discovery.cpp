/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic example main file
 *
 * @authors    Pawel Okas
 * created on: 2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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

#include <bsp.h>
#include "SPIDevice/SPIDevice.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;

stm32f4xx::GPIO cs_stm(bsp::con1::b::io4, stm32f4xx::GPIO::Direction::Output);
namespace bsp {
namespace sdCard {
microhal::GPIO &cs = cs_stm;
}
}  // namespace bsp
extern "C" int main(int, void *);

static void run_main(void *) {
    main(0, nullptr);
}

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, IOPin::PortD, 9>();

    IOManager::routeSerial<2, Txd, IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, IOPin::PortA, 3>();

    IOManager::routeSPI<1, SCK, IOPin::PortA, 5>();
    IOManager::routeSPI<1, MISO, IOPin::PortA, 6>();
    IOManager::routeSPI<1, MOSI, IOPin::PortA, 7>();

    IOManager::routeSPI<3, SCK, IOPin::PortC, 10>();
    IOManager::routeSPI<3, MISO, IOPin::PortB, 4>(stm32f4xx::GPIO::PullType::PullUp);
    IOManager::routeSPI<3, MOSI, IOPin::PortB, 5>();

    IOManager::routeI2C<2, SDA, IOPin::PortB, 11>();
    IOManager::routeI2C<2, SCL, IOPin::PortB, 10>();

    bsp::debugPort.setDataBits(stm32f4xx::SerialPort::Data8);
    bsp::debugPort.setStopBits(stm32f4xx::SerialPort::OneStop);
    bsp::debugPort.setParity(stm32f4xx::SerialPort::NoParity);
    bsp::debugPort.open(stm32f4xx::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(stm32f4xx::SerialPort::Baud115200);

    stm32f4xx::SPI::spi3.init(stm32f4xx::SPI::Mode0, stm32f4xx::SPI::Prescaler128);
    stm32f4xx::SPI::spi3.enable();

    TaskHandle_t xHandle = NULL;
    xTaskCreate(run_main, "Main", 11.5 * 1024, NULL, tskIDLE_PRIORITY, &xHandle);

    vTaskStartScheduler();
}

/*------------------------------------------------------------------------*/ /**
 * \brief Wait for a child process.
 * \details Wait for a child process.
 *
 * \param [in] file is the file descriptor of an open file to write to.
 * \param [in] buf is an array of data to write to the open file.
 * \param [in] nbyte is the number of bytes to write to the file.
 * \return 0 for success.
 */ /*-------------------------------------------------------------------------*/

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

// extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
//	size_t toWrite = nbyte;
//	do {
//		size_t written = debugPort.write((const char*)buf, toWrite);
//		buf += written;
//		toWrite -= written;
//	} while(toWrite);
//
//	(void)r;									// suppress warning
//    (void)file;							// suppress warning
//
//    return 0;
//}
