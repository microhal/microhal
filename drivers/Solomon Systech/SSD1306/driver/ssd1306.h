/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 19-04-2019
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

#ifndef _MICROHAL_SSD1306_H_
#define _MICROHAL_SSD1306_H_

#include <concepts>
#include <cstdint>
#include <span>
#include <thread>
#include "I2CDevice/I2CDevice.h"
#include "SPIDevice/SPIDevice.h"
#include "color.h"
#include "display.h"
#include "point.h"

namespace ssd1306 {

class SPIInterface {
 public:
    using Error = microhal::SPI::Error;

    SPIInterface(microhal::SPI &spi, microhal::GPIO &cs, microhal::GPIO &DC) : spi(spi), cs(cs), dataCommandSelect(DC) {}

    bool writeCMD(uint8_t cmd);
    bool writeData(gsl::span<uint8_t> data);

 private:
    microhal::SPI &spi;
    microhal::GPIO &cs;
    microhal::GPIO &dataCommandSelect;  // when low -> command, when high -> data

    void delay();
};

class I2CInterface {
 public:
    using Error = microhal::I2C::Error;
    enum PossibleAddress : microhal::I2C::DeviceAddress { Address_0x78 = 0x78, Address_0x7A = 0x7A };

    I2CInterface(microhal::I2C &i2c, PossibleAddress address) : i2c(i2c, address) {}

    bool writeData(gsl::span<uint8_t> data);
    bool writeCMD(uint8_t cmd);

 private:
    microhal::I2CDevice i2c;
};

template <class Interface>
class SSD1306 : public microhal::graphics::Display {
 public:
    using Point = microhal::graphics::Point;
    using Color = microhal::graphics::Color;
    using PossibleAddress = typename I2CInterface::PossibleAddress;

    enum class Rotation { Rot0Deg, Rot180Deg };

    enum class MemoryAddressingMode {  // see datasheet: 10.1.3 Set Memory Addressing Mode (20h)
        PageAddressingMode = 0b10,
        HorizontalAddressingMode = 0b00,
        VerticalAddressingMode = 0b01
    };

    static constexpr uint_fast8_t width = 128;
    static constexpr uint_fast8_t height = 64;

    template <typename I = Interface>
    requires std::same_as<I, I2CInterface> SSD1306(microhal::I2C &i2c, PossibleAddress address) : interface(i2c, address) {}

    template <typename I = Interface>
    requires std::same_as<I, SPIInterface> SSD1306(microhal::SPI &spi, microhal::GPIO &cs, microhal::GPIO &DC) : interface(spi, cs, DC) {}

    void reset();
    bool init();
    void fill(Color color);

    bool setPixel(Point position, Color color) final;
    bool redraw() { return writeData(framebuffer); }

    void setContrast(uint8_t contrast) {
        sendCMD(0x81);
        sendCMD(contrast);
    }
    void displayOff() { sendCMD(0xAE); }
    void displayOn() { sendCMD(0xAF); }

    void rotation(Rotation rot) { this->rot = rot; }

 private:
    Interface interface;
    Rotation rot;
    std::array<uint8_t, width * height / 8> framebuffer;

    void setDisplayOffset(uint8_t offset) {
        sendCMD(0xD3);
        sendCMD(offset);
    }

    void setCOMPins(uint8_t comPinsConfig) {
        sendCMD(0xDA);
        sendCMD(comPinsConfig);
    }

    void setMemoryAddressingMode(MemoryAddressingMode mode) {
        sendCMD(0x20);
        sendCMD(static_cast<uint8_t>(mode));
    }

    void setStartLine(uint8_t startLine) { sendCMD(0x40 | startLine); }

    bool sendCMD(uint8_t cmd) { return interface.writeCMD(cmd); }
    bool writeData(gsl::span<uint8_t> data) { return interface.writeData(data); }
};

using SSD1306_SPI = SSD1306<SPIInterface>;
using SSD1306_I2C = SSD1306<I2CInterface>;

}  // namespace ssd1306

#endif /* _MICROHAL_SSD1306_H_ */
