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

#include <cstdint>

#include "I2CDevice/I2CDevice.h"
#include "color.h"
#include "display.h"
#include "gsl/span"
#include "point.h"

class SSD1306 : public microhal::graphics::Display {
 public:
    using Error = microhal::I2C::Error;
    using Point = microhal::graphics::Point;
    using Color = microhal::graphics::Color;
    enum PossibleAddress : microhal::I2C::DeviceAddress { Address_0x78 = 0x78, Address_0x7A = 0x7A };
    static constexpr uint_fast8_t width = 128;
    static constexpr uint_fast8_t height = 32;

    SSD1306(microhal::I2C &i2c, PossibleAddress address) : i2c(i2c, address) {}

    void reset();
    bool init();
    void fill(Color color);

    bool setPixel(Point position, Color color) final;
    void redraw();

    void setContrast(uint8_t contrast) {
        sendCMD(0x81);
        sendCMD(contrast);
    }
    void displayOff() { sendCMD(0xAE); }
    void displayOn() { sendCMD(0xAF); }

 private:
    microhal::I2CDevice i2c;
    std::array<uint8_t, width * height / 8> framebuffer;

    void setDisplayOffset(uint8_t offset) {
        sendCMD(0xD3);
        sendCMD(offset);
    }

    bool sendCMD(uint8_t cmd);
};

#endif /* _MICROHAL_SSD1306_H_ */
