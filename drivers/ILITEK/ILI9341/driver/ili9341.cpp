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

#include "ili9341.h"
#include <algorithm>
#include <chrono>
#include <thread>

uint8_t ILI9341::readRegister(uint8_t address, uint8_t parameter) {
    sendCMD(0xd9);
    sendData(uint8_t{0x10 + parameter});
    dc.reset();
    spi.writeBuffer(&address, 1, false);
    std::this_thread::sleep_for(std::chrono::microseconds{500});
    dc.set();
    uint8_t data[2];
    spi.readBuffer(data, 1, true);
    return data[0];
}

void ILI9341::readRegister(uint8_t address, uint32_t *data) {
    uint8_t i = 0;
    union {
        uint8_t rxBuff[4];
        uint32_t buf;
    };

    for (i = 0; i < 4; i++) {
        rxBuff[3 - i] = readRegister(0xd3, i);
    }
    *data = buf;
}

uint32_t ILI9341::readID() {
    uint32_t id;
    readRegister(0xd3, &id);

    return id & 0x00FF'FFFF;
}

bool ILI9341::init() {
    dc.set();
    // reset.set()
    // std::this_thread::sleep_for(std::chrono::microseconds{10});
    // reset.reset()

    if (readID() != id) return false;

    sendCMD(0xCB);
    sendData(uint8_t{0x39});
    sendData(uint8_t{0x2C});
    sendData(uint8_t{0x00});
    sendData(uint8_t{0x34});
    sendData(uint8_t{0x02});

    sendCMD(0xCF);
    sendData(uint8_t{0x00});
    sendData(uint8_t{0XC1});
    sendData(uint8_t{0X30});

    sendCMD(0xE8);
    sendData(uint8_t{0x85});
    sendData(uint8_t{0x00});
    sendData(uint8_t{0x78});

    sendCMD(0xEA);
    sendData(uint8_t{0x00});
    sendData(uint8_t{0x00});

    sendCMD(0xED);
    sendData(uint8_t{0x64});
    sendData(uint8_t{0x03});
    sendData(uint8_t{0X12});
    sendData(uint8_t{0X81});

    sendCMD(0xF7);
    sendData(uint8_t{0x20});

    sendCMD(0xC0);            // Power control
    sendData(uint8_t{0x23});  // VRH[5:0]

    sendCMD(0xC1);            // Power control
    sendData(uint8_t{0x10});  // SAP[2:0];BT[3:0]

    sendCMD(0xC5);            // VCM control
    sendData(uint8_t{0x3e});  // Contrast
    sendData(uint8_t{0x28});

    sendCMD(0xC7);            // VCM control2
    sendData(uint8_t{0x86});  //--

    sendCMD(0x36);            // Memory Access Control
    sendData(uint8_t{0x48});  //

    setPixelFormat(0x55);

    sendCMD(0xB1);
    sendData(uint8_t{0x00});
    sendData(uint8_t{0x18});

    sendCMD(0xB6);  // Display Function Control
    sendData(uint8_t{0x08});
    sendData(uint8_t{0x82});
    sendData(uint8_t{0x27});

    sendCMD(0xF2);  // 3Gamma Function Disable
    sendData(uint8_t{0x00});

    selectGammaCurve(0x01);

    sendCMD(0xE0);  // Set Gamma
    sendData(uint8_t{0x0F});
    sendData(uint8_t{0x31});
    sendData(uint8_t{0x2B});
    sendData(uint8_t{0x0C});
    sendData(uint8_t{0x0E});
    sendData(uint8_t{0x08});
    sendData(uint8_t{0x4E});
    sendData(uint8_t{0xF1});
    sendData(uint8_t{0x37});
    sendData(uint8_t{0x07});
    sendData(uint8_t{0x10});
    sendData(uint8_t{0x03});
    sendData(uint8_t{0x0E});
    sendData(uint8_t{0x09});
    sendData(uint8_t{0x00});

    sendCMD(0XE1);  // Set Gamma
    sendData(uint8_t{0x00});
    sendData(uint8_t{0x0E});
    sendData(uint8_t{0x14});
    sendData(uint8_t{0x03});
    sendData(uint8_t{0x11});
    sendData(uint8_t{0x07});
    sendData(uint8_t{0x31});
    sendData(uint8_t{0xC1});
    sendData(uint8_t{0x48});
    sendData(uint8_t{0x08});
    sendData(uint8_t{0x0F});
    sendData(uint8_t{0x0C});
    sendData(uint8_t{0x31});
    sendData(uint8_t{0x36});
    sendData(uint8_t{0x0F});

    exitSleepMode();
    std::this_thread::sleep_for(std::chrono::microseconds{120});

    displayOn();
    clear();
    return true;
}

void ILI9341::setCol(uint16_t StartCol, uint16_t EndCol) {
    sendCMD(0x2A);
    sendData(StartCol);
    sendData(EndCol);
}

void ILI9341::setPage(uint16_t StartPage, uint16_t EndPage) {
    sendCMD(0x2B);
    sendData(StartPage);
    sendData(EndPage);
}

void ILI9341::clear() {
    setCol(0, 239);
    setPage(0, 319);
    memoryWrite();
    dc.set();
    uint8_t data[4] = {0, 0, 0, 0};
    for (size_t i = 0; i < 38399; i++) {
        spi.writeBuffer(data, sizeof(data), false);
    }
    spi.writeBuffer(data, sizeof(data), true);
}

void ILI9341::setXY(uint16_t poX, uint16_t poY) {
    setCol(poX, poX);
    setPage(poY, poY);
    memoryWrite();
}

uint16_t colorToValue(microhal::graphics::Color color) {
    uint8_t red = color.r >> 3;
    uint8_t green = color.g >> 2;
    uint8_t blue = color.b >> 3;

    uint8_t lowBits = blue | ((green & 0b111) << 5);
    uint8_t hiBits = (red << 3) | (green >> 3);
    uint16_t colorValue = (hiBits << 8) | lowBits;
    return colorValue;
}

bool ILI9341::setPixel(Point position, Color color) {
    setXY(position.x, position.y);
    sendData(colorToValue(color));
    return true;
}

bool ILI9341::drawFilledRectangle(Point begin, Point end, Color color) {
    if (begin.x > end.x) std::swap(begin.x, end.x);
    if (begin.y > end.y) std::swap(begin.y, end.y);
    setCol(begin.x, end.x);
    setPage(begin.y, end.y);
    memoryWrite();

    dc.set();
    uint8_t colorInBytes[] = {colorToValue(color) >> 8, colorToValue(color) & 0xFF};
    size_t pixelsCount = (end.x - begin.x + 1) * (end.y - begin.y + 1);
    if (pixelsCount == 0) return true;  // nothing to do
    pixelsCount -= 1;
    while (pixelsCount--) {
        spi.writeBuffer(colorInBytes, sizeof(colorInBytes), false);
    }
    spi.writeBuffer(colorInBytes, sizeof(colorInBytes), true);

    return true;
}
