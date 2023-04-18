/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-11-2017
 * last modification: 30-11-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_SPI_LINUX_H_
#define _MICROHAL_SPI_LINUX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <type_traits>

#include "interfaces/spi_interface.h"

namespace microhal {
namespace linux {

/* **************************************************************************************************************************************************
 * CLASS
 */
class SPI : public microhal::SPI {
 public:
    SPI(const char *name) { std::copy_n(name, std::strlen(name), deviceName); }

    bool setMode(Mode mode) final {
        auto modeToNum = [](Mode mode) {
            switch (mode) {
                case Mode::Mode0:
                    return 0;
                    break;
                case Mode::Mode1:
                    return 1;
                    break;
                case Mode::Mode2:
                    return 2;
                    break;
                case Mode::Mode3:
                    return 3;
                    break;
            }
            std::terminate();
        };
        if (isEnabled()) {
            uint8_t modeToSet = modeToNum(mode) | SPI_3WIRE;
            if (ioctl(fd, SPI_IOC_WR_MODE, &modeToSet) < 0) {
                return false;
            }
            if (ioctl(fd, SPI_IOC_RD_MODE, &modeToSet) < 0) {
                // unable to set read mode, try to restore previous mode in
                uint8_t modeToSet = modeToNum(this->mode);
                if (ioctl(fd, SPI_IOC_WR_MODE, &modeToSet) < 0) {
                    // critical error, unable to restore previous configuration
                    std::terminate();
                }
                return false;
            }
        } else {
            this->mode = mode;
        }
        return true;
    }

    bool getMISOstate() final { /*return microhal::stm32f4xx::GPIO::get(misoPort, misoPin); */
        std::terminate();
    }

    bool isEnabled() { return enabled; }

    void enable() final {
        fd = open(deviceName, O_RDWR);
        if (fd < 0) std::terminate();
        enabled = true;

        // uint8_t bits = 8;
        // if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) std::terminate();
        // if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) std::terminate();
    }
    void disable() final {
        if (fd) ::close(fd);
        enabled = false;
    }
    /**@brief Set SPI CLK frequency.
     *
     * @param speed - CLK frequency in Hz
     * @return speed value that was set, this is closest value that was possible to set with current bus speed.
     */
    uint32_t speed(uint32_t speed) final {
        if (isEnabled()) {
            if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) return false;
            if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) return false;

            this->busSpeed = speed;
        } else {
            this->busSpeed = speed;
        }
        return speed;
    }

    [[nodiscard]] uint32_t speed() const final { return busSpeed; }
    [[nodiscard]] uint32_t maxCLKFrequency() const final { return 1; }

    SPI::Error write(const void *data, size_t len, bool last) final {
        if (len > sizeof(tmpBuf)) std::terminate();
        return writeRead(tmpBuf, data, len);
    }
    SPI::Error read(void *data, size_t len, uint8_t write = 0x00) final {
        if (len > sizeof(tmpBuf)) std::terminate();
        for (size_t i = 0; i < len; i++) {
            tmpBuf[i] = write;
        }
        return writeRead(data, tmpBuf, len);
    }

    SPI::Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final {
        if (isEnabled() == false) return SPI::Error::Unknown;

        SPI::Error error = SPI::Error::Unknown;
        struct spi_ioc_transfer tr;
        memset(&tr, 0, sizeof(tr));
        tr.tx_buf = reinterpret_cast<unsigned long>(dataWrite);
        tr.rx_buf = reinterpret_cast<unsigned long>(dataRead);
        tr.len = readWriteLength;
        tr.delay_usecs = 0;
        tr.speed_hz = 0;
        tr.bits_per_word = 0;

        auto status = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        using unsignedStatusType = std::make_unsigned<decltype(status)>::type;
        if (status > 0 && static_cast<unsignedStatusType>(status) == readWriteLength) error = SPI::Error::None;

        return error;
    }

    const char *device() const { return deviceName; }

 private:
    char deviceName[50];
    int fd = 0;
    Mode mode = Mode::Mode0;
    uint32_t busSpeed = 100000;
    bool enabled = false;
    uint8_t tmpBuf[256];
};

}  // namespace linux
}  // namespace microhal

#endif  // _MICROHAL_SPI_LINUX_H_
