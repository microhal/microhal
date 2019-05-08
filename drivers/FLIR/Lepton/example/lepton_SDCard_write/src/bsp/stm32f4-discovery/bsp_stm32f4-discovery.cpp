/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      board support package for stm32f4Discovery board

 @authors    Pawel Okas
 created on: 16-04-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 	 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 	 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 	 	in the documentation and/or other materials provided with the distribution.
 	 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 	 	from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */ /* ==========================================================================================================================
                                                                                                                                        */

#include <ctime>
#include "SPIDevice/SPIDevice.h"
#include "bsp.h"
#include "ff.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;
using namespace diagnostic;

stm32f4xx::GPIO sdCardCs({microhal::stm32f4xx::IOPin::PortE, 11}, stm32f4xx::GPIO::Direction::Output);

stm32f4xx::GPIO leptonCS({microhal::stm32f4xx::IOPin::PortE, 6}, stm32f4xx::GPIO::Direction::Output);
stm32f4xx::GPIO leptonPower({microhal::stm32f4xx::IOPin::PortD, 12}, stm32f4xx::GPIO::Direction::Output);
stm32f4xx::GPIO leptonReset({microhal::stm32f4xx::IOPin::PortD, 12}, stm32f4xx::GPIO::Direction::Output);

extern "C" int main(int, void *);
static void run_main(void *) {
    main(0, nullptr);
}

void hardwareConfig(void) {
    (void)leptonSPI;
    (void)leptonI2C;
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    IOManager::routeSerial<2, Txd, stm32f4xx::IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f4xx::IOPin::PortA, 3>();

    IOManager::routeSPI<1, SCK, stm32f4xx::IOPin::PortA, 5>();
    IOManager::routeSPI<1, MISO, stm32f4xx::IOPin::PortA, 6>();
    IOManager::routeSPI<1, MOSI, stm32f4xx::IOPin::PortA, 7>();

    IOManager::routeSPI<3, SCK, stm32f4xx::IOPin::PortC, 10>();
    IOManager::routeSPI<3, MISO, stm32f4xx::IOPin::PortB, 4>(stm32f4xx::GPIO::PullType::PullUp);
    IOManager::routeSPI<3, MOSI, stm32f4xx::IOPin::PortB, 5>();

    IOManager::routeI2C<2, SDA, stm32f4xx::IOPin::PortB, 11>();
    IOManager::routeI2C<2, SCL, stm32f4xx::IOPin::PortB, 10>();

    debugPort.setDataBits(stm32f4xx::SerialPort::Data8);
    debugPort.setStopBits(stm32f4xx::SerialPort::OneStop);
    debugPort.setParity(stm32f4xx::SerialPort::NoParity);
    debugPort.open(stm32f4xx::SerialPort::ReadWrite);
    debugPort.setBaudRate(stm32f4xx::SerialPort::Baud115200);

    diagnostic::diagChannel.setOutputDevice(debugPort);

    stm32f4xx::SPI::spi1.init(stm32f4xx::SPI::Mode3, stm32f4xx::SPI::Prescaler8);
    stm32f4xx::SPI::spi1.enable();

    stm32f4xx::SPI::spi3.init(stm32f4xx::SPI::Mode0, stm32f4xx::SPI::Prescaler128);
    stm32f4xx::SPI::spi3.enable();

    TaskHandle_t xHandle = NULL;
    xTaskCreate(run_main, "NAME", 5 * 1024, NULL, tskIDLE_PRIORITY, &xHandle);
    vTaskStartScheduler();
}

const char *printFatFSResult(FRESULT result) {
    switch (result) {
        case FR_OK:
            return "OK";
        case FR_DISK_ERR:
            return "Disk Error";
        case FR_INT_ERR:
            return "internal error";
        case FR_NOT_READY:
            return "Not ready";
        case FR_NO_FILE:
            return "Could not find the file.";
        case FR_NO_PATH:
            return "Could not find the path.";
        case FR_INVALID_NAME:
            return "he given string is invalid as the path name.";
        case FR_DENIED:
            return "The required access was denied";
        case FR_EXIST:
            return "Name collision. An object with the same name is already existing.";
        case FR_INVALID_OBJECT:
            return "The file/directory object is invalid or a null pointer is given.";
        case FR_WRITE_PROTECTED:
            return "A write mode operation against the write-protected media.";
        case FR_INVALID_DRIVE:
            return "Invalid drive number is specified in the path name.";
        case FR_NOT_ENABLED:
            return "Work area for the logical drive has not been registered by f_mount function.";
        case FR_NO_FILESYSTEM:
            return "There is no valid FAT volume on the drive.";
        case FR_MKFS_ABORTED:
            return "The f_mkfs function aborted.";
        case FR_TIMEOUT:
            return "The function was canceled due to a timeout of thread-safe control.";
        case FR_LOCKED:
            return "The operation to the object was rejected by file sharing control.";
        case FR_NOT_ENOUGH_CORE:
            return "Not enough memory for the operation.";
        case FR_TOO_MANY_OPEN_FILES:
            return "Number of open objects has been reached maximum value and no more object can be opened.";
        case FR_INVALID_PARAMETER:
            return "The given parameter is invalid or there is an inconsistent for the volume.";
        default:
            return "Undefined result code";
    }
}

/*------------------------------------------------------------------------*/ /**
 * \brief Open a file.
 * \details Open a file.
 *
 * \param [in] pathname is the name of the file to open.
 * \param [in] flags is the bitwise inclusive-OR of the file access modes and
 * file status flags.
 * \param [in] mode specifies what permissions the file has when it is created.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

FIL fp;
extern "C" int _open_r(struct _reent *r, const char *pathname, int flags, int mode) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "opening file, pathname: " << pathname  << ", flags: " << (int32_t)flags << " mode: " <<
    // (int32_t)mode << unlock;
    if (f_open(&fp, pathname, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
        return 10;
    }
    return -1;
}

/*------------------------------------------------------------------------*/ /**
 * \brief Close a file.
 * \details Close a file.
 *
 * \param [in] file indicates the file descriptor to close.
 * \return -1 for failure.
 */ /*-------------------------------------------------------------------------*/

extern "C" int _close_r(struct _reent *r, int file) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "Closing file: " << (int32_t)file << unlock;
    if (file == 10) {
        auto res = f_close(&fp);
        if (res == FR_OK) {
            return 0;
        } else {
            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs close error: " << printFatFSResult(res) << unlock;
        }
    }

    return -1;
}

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "Writing file: " << (int32_t)file << ", size: " << (uint32_t)nbyte << unlock;
    if (file == 10) {
        UINT count = 0;
        auto res = f_write(&fp, buf, nbyte, &count);
        if (res != FR_OK) {
            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs write error: " << printFatFSResult(res) << unlock;
            return -1;
        }
        return count;
    } else {
        debugPort.mutex.lock();
        size_t toWrite = nbyte;
        do {
            size_t written = debugPort.write((const char *)buf, toWrite);
            buf += written;
            toWrite -= written;
        } while (toWrite);
        debugPort.mutex.unlock();
        (void)r;     // suppress warning
        (void)file;  // suppress warning
    }
    return 0;
}

extern "C" ssize_t _read_r(struct _reent *r, int file, void *buf, size_t nbyte) {
    // diagChannel << lock << MICROHAL_INFORMATIONAL << "reading file: " << (int32_t)file << unlock;
    if (file == 10) {
        UINT count;
        auto res = f_read(&fp, buf, nbyte, &count);
        if (res != FR_OK) {
            diagChannel << lock << MICROHAL_INFORMATIONAL << "FatFs read error: " << printFatFSResult(res) << unlock;
            return -1;
        }
        return nbyte - count;
    }
    return 0;
}

extern "C" DWORD get_fattime(void) {
    std::time_t t = std::time(nullptr);
    const auto time = std::gmtime(&t);

    uint_fast8_t sec = time->tm_sec / 2;
    uint_fast8_t min = time->tm_min;
    uint_fast8_t hour = time->tm_hour;
    uint_fast8_t day = time->tm_mday;
    uint_fast8_t month = time->tm_mon;
    uint_fast8_t year = time->tm_year >= 80 ? time->tm_year - 80 : 0;

    uint32_t fattime = (year << 25) | (month << 21) | (day << 16) | (hour << 11) | (min << 5) | (sec);
    return fattime;
}
