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

#include "ili9325.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;

uint16_t colorToValue(microhal::graphics::Color color) {
    uint8_t red = color.r >> 3;
    uint8_t green = color.g >> 2;
    uint8_t blue = color.b >> 3;

    uint8_t lowBits = blue | ((green & 0b111) << 5);
    uint8_t hiBits = (red << 3) | (green >> 3);
    uint16_t colorValue = (hiBits << 8) | lowBits;
    return colorValue;
}

bool ILI9325::setPixel(Point position, Color color) {
    setActiveArea(position, position);
    setXY(position);
    return writeRegister(0x0022, colorToValue(color));
}

bool ILI9325::drawLine(Point begin, Point end, Color color) {
    // if line is horizontal or vertical we can draw it faster by drawing rectangle
    if ((begin.x == end.x) || (begin.y == end.y)) {
        drawFilledRectangle(begin, end, color);
    } else {
        // use base (more generic) function do draw line
        Display::drawLine(begin, end, color);
    }
    return true;
}

bool ILI9325::drawFilledRectangle(Point begin, Point end, Color color) {
    if (begin.x > end.x) std::swap(begin.x, end.x);
    if (begin.y > end.y) std::swap(begin.y, end.y);
    setActiveArea(begin, end);
    setXY(begin);
    uint32_t xdiff = end.x - begin.x + 1;
    uint32_t ydiff = end.y - begin.y + 1;
    uint32_t pixelCount = xdiff * ydiff;
    writeGRAM(color, pixelCount);
    return true;
}

void ILI9325::fill(Color color) {
    Point begin = {0, 0};
    Point end = {240, 320};
    drawFilledRectangle(begin, end, color);
}

void ILI9325::reset() {
    cs.set();
    rs.set();

    rst.set();
    std::this_thread::sleep_for(std::chrono::milliseconds{1});
    rst.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    rst.set();
    std::this_thread::sleep_for(std::chrono::milliseconds{50});
}

uint16_t ILI9325::readID() {
    uint16_t id;
    readRegister(0x00, &id);
    return id;
}

void ILI9325::exitSleepMode() {
    //*************Power On sequence ******************//
    writeRegister(0x0010, 0x0080);                                //  SAP,  BT[3:0],  AP,  DSTB,  SLP
    writeRegister(0x0011, 0x0000);                                //  DC1[2:0],  DC0[2:0],  VC[2:0]
    writeRegister(0x0012, 0x0000);                                //  VREG1OUT  voltage
    writeRegister(0x0013, 0x0000);                                //  VDV[4:0]  for  VCOM  amplitude
    std::this_thread::sleep_for(std::chrono::milliseconds{200});  //  Dis-charge  capacitor  power  voltage
    writeRegister(0x0010, 0x1490);                                //  SAP,  BT[3:0],  AP,  DSTB,  SLP,  STB
    writeRegister(0x0011, 0x0227);                                //  DC1[2:0],  DC0[2:0],  VC[2:0]
    std::this_thread::sleep_for(std::chrono::milliseconds{50});   //
    writeRegister(0x0012, 0x001C);                                // Inernal  reference  voltage  =Vci;
    std::this_thread::sleep_for(std::chrono::milliseconds{50});   //
    writeRegister(0x0013, 0x1A00);                                //  VDV[4:0]  for  VCOM  amplitude
    writeRegister(0x0029, 0x0025);                                //  VCM[5:0]  for  VCOMH
    std::this_thread::sleep_for(std::chrono::milliseconds{50});   //
    writeRegister(0x0007, 0x0133);                                // 262K color and display ON
}

void ILI9325::enterSleepMode() {
    writeRegister(0x0007, 0x0131);                               //  Set  D1=0,  D0=1
    std::this_thread::sleep_for(std::chrono::milliseconds{10});  //
    writeRegister(0x0007, 0x0130);                               //  Set  D1=0,  D0=0
    std::this_thread::sleep_for(std::chrono::milliseconds{10});  //
    writeRegister(0x0007, 0x0000);                               //  display  OFF
    //************* Power OFF sequence **************//
    writeRegister(0x0010, 0x0080);                                //  SAP,  BT[3:0],  APE,  AP,  DSTB,  SLP
    writeRegister(0x0011, 0x0000);                                //  DC1[2:0],  DC0[2:0],  VC[2:0]
    writeRegister(0x0012, 0x0000);                                //  VREG1OUT  voltage
    writeRegister(0x0013, 0x0000);                                //  VDV[4:0]  for  VCOM  amplitude
    std::this_thread::sleep_for(std::chrono::milliseconds{200});  //  Dis-charge  capacitor  power  voltage
    writeRegister(0x0010, 0x0082);                                //  SAP,  BT[3:0],  APE,  AP,  DSTB,  SLP
}

bool ILI9325::init() {
    // initializatin sequence according to ILITEK Application Note v0.22 for CPT 2.4”
    if (readID() != id) return false;
    reset();

    writeRegister(0x00E3, 0x3008);  //  Set  internal  timing
    writeRegister(0x00E7, 0x0012);  //  Set  internal  timing
    writeRegister(0x00EF, 0x1231);  //  Set  internal  timing

    // writeRegister(0x0000, 0x0001);  // set SS and SM bit
    writeRegister(0x0001, 0x0100);  // set SS and SM bit
    writeRegister(0x0002, 0x0700);  // set 1 line inversion
    writeRegister(0x0003, 0x1030);  // set  GRAM  write  direction  and  BGR=1.
    writeRegister(0x0004, 0x0000);  // No resizing
    writeRegister(0x0008, 0x0207);  // set the back porch and front porch
    writeRegister(0x0009, 0x0000);  // set non-display area refresh cycle ISC[3:0]
    writeRegister(0x000A, 0x0000);  // FMARK function
    writeRegister(0x000C, 0x0000);  // RGB interface setting
    writeRegister(0x000D, 0x0000);  // Frame marker Position
    writeRegister(0x000F, 0x0000);  // RGB interface polarity

    //*************Power  On  sequence  ****************//
    writeRegister(0x0010, 0x0000);                                //  SAP,  BT[3:0],  AP,  DSTB,  SLP,  STB
    writeRegister(0x0011, 0x0007);                                //  DC1[2:0],  DC0[2:0],  VC[2:0]
    writeRegister(0x0012, 0x0000);                                //  VREG1OUT  voltage
    writeRegister(0x0013, 0x0000);                                //  VDV[4:0]  for  VCOM  amplitude
    std::this_thread::sleep_for(std::chrono::milliseconds{200});  //  Dis-charge  capacitor  power  voltage
    writeRegister(0x0010, 0x1190); /*1490*/                       //  SAP,  BT[3:0],  AP,  DSTB,  SLP,  STB
    writeRegister(0x0011, 0x0221); /*0227*/                       //  DC1[2:0],  DC0[2:0],  VC[2:0]
    std::this_thread::sleep_for(std::chrono::milliseconds{50});   // Delay 50ms
    writeRegister(0x0012, 0x001A); /*001C*/                       //  Internal  reference  voltage=  Vci;
    std::this_thread::sleep_for(std::chrono::milliseconds{50});   // Delay 50ms
    writeRegister(0x0013, 0x1A00); /*001A*/                       //  Set  VDV[4:0]  for  VCOM  amplitude
    writeRegister(0x0029, 0x0025); /*002A*/                       //  Set  VCM[5:0]  for  VCOMH
    writeRegister(0x002B, 0x000C);                                //  Set  Frame  Rate
    std::this_thread::sleep_for(std::chrono::milliseconds{50});   // Delay 50ms
    writeRegister(0x0020, 0x0000);                                //  GRAM  horizontal  Address
    writeRegister(0x0021, 0x0000);                                //  GRAM  Vertical  Address

    // ----------- Adjust the Gamma    Curve ----------//
    writeRegister(0x0030, 0x0000);
    writeRegister(0x0031, 0x0506);
    writeRegister(0x0032, 0x0104);
    writeRegister(0x0035, 0x0207);
    writeRegister(0x0036, 0x000F);
    writeRegister(0x0037, 0x0306);
    writeRegister(0x0038, 0x0102);
    writeRegister(0x0039, 0x0707);
    writeRegister(0x003C, 0x0702);
    writeRegister(0x003D, 0x1604);
    //------------------ Set GRAM area ---------------//
    writeRegister(0x0050, 0x0000);           //  Horizontal  GRAM  Start  Address
    writeRegister(0x0051, 0x00EF);           //  Horizontal  GRAM  End  Address
    writeRegister(0x0052, 0x0000);           //  Vertical  GRAM  Start  Address
    writeRegister(0x0053, 0x013F);           //  Vertical  GRAM  Start  Address
    writeRegister(0x0060, 0xA700);           //  Gate  Scan  Line
    writeRegister(0x0061, 0x0001); /*0003*/  //  NDL,VLE,  REV
    writeRegister(0x006A, 0x0000);           //  set  scrolling  line
    //-------------- Partial Display Control ---------//
    writeRegister(0x0080, 0x0000);
    writeRegister(0x0081, 0x0000);
    writeRegister(0x0082, 0x0000);
    writeRegister(0x0083, 0x0000);
    writeRegister(0x0084, 0x0000);
    writeRegister(0x0085, 0x0000);
    //-------------- Panel Control -------------------//
    writeRegister(0x0090, 0x0010);
    writeRegister(0x0092, 0x0600);

    writeRegister(0x0007, 0x0133);  //  262K  color  and  display  ON

    fill({0, 0, 0});
    return true;
}

void ILI9325::setActiveArea(Point begin, Point end) {
    writeRegister(0x0050, begin.x); /* Horizontal GRAM Start Address */
    writeRegister(0x0051, end.x);   /* Horizontal GRAM End Address */
    writeRegister(0x0052, begin.y); /* Vertical GRAM Start Address */
    writeRegister(0x0053, end.y);   /* Vertical GRAM End Address */
}

void ILI9325::setXY(Point point) {
    writeRegister(0x0020, point.x); /* GRAM horizontal Address */
    writeRegister(0x0021, point.y); /* GRAM Vertical Address */
}

void ILI9325::writeGRAM(gsl::span<Color> pixels) {
    cs.reset();
    rs.reset();
    dataBus.write(0x0022);
    rs.set();
    for (const auto pixelColor : pixels) {
        dataBus.write(colorToValue(pixelColor));
    }
    cs.set();
}

void ILI9325::writeGRAM(Color pixelColor, uint32_t count) {
    cs.reset();
    rs.reset();
    dataBus.write(0x0022);
    rs.set();
    while (count--) {
        dataBus.write(colorToValue(pixelColor));
    }
    cs.set();
}

void ILI9325::readGRAM(gsl::span<Color> pixels) {
    cs.reset();
    uint8_t i = 0;
    for (auto &pixel : pixels) {
        setXY({i++, 0});
        cs.reset();
        rs.reset();
        dataBus.write(0x0022);
        rs.set();
        dataBus.read();  // dummy data read
        // for (auto &pixel : pixels) {
        auto tmp = dataBus.read();
        pixel.r = tmp >> 8;
        pixel.g = tmp & 0xFF;
        //}
    }
    cs.set();
}

void ILI9325::readRegister(uint16_t address, uint16_t *data) {
    cs.reset();  // activate data buss
    rs.reset();  // enter command mode
    dataBus.write(address);
    rs.set();  // enter data mode
    *data = dataBus.read();
    cs.set();
}

bool ILI9325::writeRegister(uint16_t address, uint16_t data) {
    cs.reset();
    rs.reset();  // enter command mode
    dataBus.write(address);
    rs.set();  // enter data mode
    dataBus.write(data);
    cs.set();
    return true;
}

bool ILI9325::gramWriteReadTest() {
    std::array<Color, 16> dataWrite;
    uint_fast16_t i = 0;
    for (auto &pixel : dataWrite) {
        pixel.r = i;
        pixel.g = i + 2;
        i += 10;
    }
    // write pixels value into GRAM
    cs.reset();
    setXY({0, 0});
    writeGRAM(dataWrite);
    // read pixels value from GRAM
    std::array<Color, 16> dataRead;
    cs.reset();
    setXY({0, 0});
    readGRAM(dataRead);
    for (auto data : dataRead) {
        diagChannel << Debug << "GRAM: " << data.r << " " << data.g << " " << data.b << endl;
    }
    return dataWrite == dataRead;
}

void ILI9325::registersDump() {
    for (uint16_t registerAddress = 0; registerAddress < 256; registerAddress++) {
        uint16_t data;
        cs.reset();
        readRegister(registerAddress, &data);
        cs.set();
        diagChannel << Debug << "Address: " << toHex(registerAddress) << ", data: " << toHex(data) << endl;
    }
}
