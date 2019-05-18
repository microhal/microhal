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

#ifndef _MICROHAL_ILI9325_H_
#define _MICROHAL_ILI9325_H_

#include <cstdint>

#include "color.h"
#include "display.h"
#include "gsl/span"
#include "interfaces/dataBus_interface.h"
#include "interfaces/gpio_interface.h"
#include "point.h"

class ILI9325 : public microhal::graphics::Display {
 public:
    using Point = microhal::graphics::Point;
    using Color = microhal::graphics::Color;
    static constexpr uint16_t id = 0x9325;

    ILI9325(microhal::GPIO &reset, microhal::GPIO &cs, microhal::GPIO &rs, microhal::DataBus &databus)
        : rst(reset), cs(cs), rs(rs), dataBus(databus) {
        rst.set();
        cs.set();
        rs.set();
    }

    void reset();
    uint16_t readID();
    bool init();
    void fill(Color color);

    bool setPixel(Point position, Color color) final;
    bool drawLine(Point begin, Point end, Color) final;
    bool drawFilledRectangle(Point begin, Point end, Color) final;

    void enterSleepMode();
    void exitSleepMode();

    bool gramWriteReadTest();
    void registersDump();

 private:
    microhal::GPIO &rst;  // reset select
    microhal::GPIO &cs;   // chip select
    microhal::GPIO &rs;   // register select signal
    microhal::DataBus &dataBus;

    void setActiveArea(Point begin, Point end);
    void setXY(Point point);

    void writeGRAM(gsl::span<Color> pixels);
    void writeGRAM(Color pixel, uint32_t count);
    void readGRAM(gsl::span<Color> pixels);

    void readRegister(uint16_t address, uint16_t *data);
    bool writeRegister(uint16_t address, uint16_t data);
};

#endif /* _MICROHAL_ILI9325_H_ */
