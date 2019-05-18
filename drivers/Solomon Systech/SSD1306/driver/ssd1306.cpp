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

uint16_t colorToValue(microhal::graphics::Color color) {
    if (color.r || color.g || color.b) return 1;
    return 0;
}

bool SSD1306::setPixel(Point position, Color color) {
    if (position.x > width || position.y > height) return false;
    if (colorToValue(color)) {
        framebuffer[position.x + (position.y / 8 * width)] |= 1 << position.y % 8;
    } else {
        framebuffer[position.x + (position.y / 8 * width)] &= ~(1 << position.y % 8);
    }
    return true;
}

void SSD1306::fill(Color color) {
    if (colorToValue(color))
        framebuffer.fill(0xFF);
    else
        framebuffer.fill(0x00);
}

void SSD1306::reset() {
    //    rst.set();
    //    std::this_thread::sleep_for(std::chrono::milliseconds{1});
    //    rst.reset();
    //    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    //    rst.set();
    //    std::this_thread::sleep_for(std::chrono::milliseconds{50});
}

bool SSD1306::init() {
    reset();

    displayOff();
    sendCMD(0x00);
    sendCMD(0x10);
    sendCMD(0x40);
    sendCMD(0x20);  // page addressing mode
    sendCMD(0x00);
    setContrast(0x01);
    sendCMD(0xA1);
    sendCMD(0xC0);
    sendCMD(0xA8);
    sendCMD(0x3F);
    sendCMD(0xD5);
    sendCMD(0x80);
    sendCMD(0xD9);
    sendCMD(0xF1);

    if (height == 64) {
        setDisplayOffset(0);
        sendCMD(0xDA);
        sendCMD(0x12);
    }
    if (height == 32) {
        setDisplayOffset(32);
        sendCMD(0xDA);
        sendCMD(0x02);
    } else {
        sendCMD(0xDA);
        sendCMD(0x02);
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

void SSD1306::redraw() {
    i2c.write(0x40, framebuffer);
}

bool SSD1306::sendCMD(uint8_t cmd) {
    i2c.write(0x80, cmd);
    return true;
}
