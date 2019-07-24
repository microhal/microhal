/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 06-04-2019
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

#ifndef _MICROHAL_ILI9341_H_
#define _MICROHAL_ILI9341_H_

#include <cstdint>

#include "SPIDevice/SPIDevice.h"
#include "color.h"
#include "display.h"
#include "point.h"

class ILI9341 : public microhal::graphics::Display {
 public:
    using Error = microhal::SPI::Error;
    using Point = microhal::graphics::Point;
    using Color = microhal::graphics::Color;
    static constexpr uint32_t id = 0x0000'9341;

    ILI9341(microhal::SPI &spi, microhal::GPIO &cs, microhal::GPIO &dc) : spi(spi, cs), dc(dc) {}

    uint32_t readID();
    bool init();

    void clear();

    bool setPixel(Point position, Color color) final;
    bool drawFilledRectangle(Point begin, Point end, Color) final;

    void displayOff() { sendCMD(0x28); }
    void displayOn() { sendCMD(0x29); }
    void setPixelFormat(uint8_t pixelFormat) {
        sendCMD(0x3A);
        sendData(pixelFormat);
    }

    void writeDisplayBrighrness(uint8_t brightness) {
        sendCMD(0x51);
        sendData(brightness);
    }
    /**
     * This command is used to select the desired Gamma curve for the current display
     * @param curve allowed values: 0x01, 0x02, 0x04, 0x08
     */
    void selectGammaCurve(uint8_t curve) {
        sendCMD(0x26);
        sendData(curve);
    }

    void enterSleepMode() { sendCMD(0x10); }
    void exitSleepMode() { sendCMD(0x11); }

 private:
    microhal::SPIDevice spi;
    microhal::GPIO &dc;

    void setXY(uint16_t poX, uint16_t poY);
    void setCol(uint16_t StartCol, uint16_t EndCol);
    void setPage(uint16_t StartPage, uint16_t EndPage);

    void memoryWrite() { sendCMD(0x2C); }
    void memoryRead() { sendCMD(0x2E); }

    bool sendCMD(uint8_t cmd) {
        dc.reset();
        return spi.write(cmd);
    }

    bool sendData(uint8_t cmd) {
        dc.set();
        return spi.write(cmd);
    }
    bool sendData(uint16_t cmd) {
        dc.set();
        uint8_t data[2] = {static_cast<uint8_t>(cmd >> 8), static_cast<uint8_t>(cmd & 0xFF)};
        return spi.writeBuffer(data, sizeof(data), true);
    }

    void readRegister(uint8_t address, uint32_t *data);
    uint8_t readRegister(uint8_t address, uint8_t parameter);
};

#endif /* _MICROHAL_ILI9341_H_ */
