/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 17-04-2019
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

#include "ssd1306.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;

namespace ssd1306 {

uint16_t colorToValue(microhal::graphics::Color color) {
    if (color.r || color.g || color.b) return 1;
    return 0;
}

bool SPIInterface::writeCMD(uint8_t cmd) {
    dataCommandSelect.reset();
    cs.reset();
    delay();
    spi.write(cmd, true);
    delay();
    cs.set();
    return true;
}

bool SPIInterface::writeData(gsl::span<uint8_t> data) {
    dataCommandSelect.set();
    cs.reset();
    spi.write(data.data(), data.size(), true);
    cs.set();
    return true;
}

void SPIInterface::delay() {
    std::this_thread::sleep_for(std::chrono::milliseconds{1});
}

bool I2CInterface::writeData(gsl::span<uint8_t> data) {
    return i2c.write(0x40, data) == I2CInterface::Error::None;
}

bool I2CInterface::writeCMD(uint8_t cmd) {
    const auto result = i2c.write(0x80, cmd);
    if (result != microhal::I2C::Error::None) {
        diagChannel << lock << MICROHAL_ERROR << "Unable to send CMD, error: " << microhal::I2C::toString(result) << unlock;
    }
    return result == I2CInterface::Error::None;
}

template <class Interface>
bool SSD1306<Interface>::setPixel(Point position, Color color) {
    if (position.x > width || position.y > height) return false;
    const auto bitPos = (rot == Rotation::Rot0Deg) ? (position.y % 8) : 7 - (position.y % 8);
    const auto bytePos =
        (rot == Rotation::Rot0Deg) ? (position.x + (position.y / 8 * width)) : framebuffer.size() - 1 - (position.x + (position.y / 8 * width));
    if (colorToValue(color)) {
        framebuffer[bytePos] |= 1 << bitPos;
    } else {
        framebuffer[bytePos] &= ~(1 << bitPos);
    }
    return true;
}

template <class Interface>
void SSD1306<Interface>::fill(Color color) {
    if (colorToValue(color))
        framebuffer.fill(0xFF);
    else
        framebuffer.fill(0x00);
}

template <class Interface>
void SSD1306<Interface>::reset() {
    //    rst.set();
    //    std::this_thread::sleep_for(std::chrono::milliseconds{1});
    //    rst.reset();
    //    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    //    rst.set();
    //    std::this_thread::sleep_for(std::chrono::milliseconds{50});
}

template <class Interface>
bool SSD1306<Interface>::init() {
    reset();

    displayOff();
    setStartLine(0);
    setMemoryAddressingMode(MemoryAddressingMode::HorizontalAddressingMode);
    setContrast(0x01);
    sendCMD(0xA1);
    if (height == 32) sendCMD(0xC0);
    sendCMD(0xA8);
    sendCMD(0x3F);
    sendCMD(0xD5);
    sendCMD(0x80);
    sendCMD(0xD9);
    sendCMD(0xF1);

    if (height == 64) {
        setDisplayOffset(0);
        setCOMPins(0x12);
    } else if (height == 32) {
        setDisplayOffset(32);
        setCOMPins(0x02);
    } else {
        setCOMPins(0x02);
    }
    sendCMD(0xDB);
    sendCMD(0x40);
    sendCMD(0x8D);
    sendCMD(0x14);
    sendCMD(0xA4);
    sendCMD(0xA6);
    displayOn();

    fill({0, 0, 0});
    redraw();
    return true;
}

template void SSD1306<SPIInterface>::reset();
template bool SSD1306<SPIInterface>::init();
template bool SSD1306<SPIInterface>::setPixel(Point position, Color color);
template void SSD1306<SPIInterface>::fill(Color color);

template void SSD1306<I2CInterface>::reset();
template bool SSD1306<I2CInterface>::init();
template bool SSD1306<I2CInterface>::setPixel(Point position, Color color);
template void SSD1306<I2CInterface>::fill(Color color);

}  // namespace ssd1306
