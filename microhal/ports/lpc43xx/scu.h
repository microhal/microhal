/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 23-03-2019
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

#ifndef _MICROHAL_SCU_H_
#define _MICROHAL_SCU_H_

#include <cstdint>
#include <exception>
#include "IOPin.h"

namespace microhal {
namespace lpc43xx {

class SCU final {
    typedef union {
        struct {
            uint32_t mode : 3;
            uint32_t epd : 1;
            uint32_t epun : 1;
            uint32_t ehs : 1;
            uint32_t ezi : 1;
            uint32_t zif : 1;
            uint32_t ehd : 2;
        } bitfields;
        uint32_t raw;
    } SFSP;

    SCU() = delete;
    ~SCU() = delete;

 public:
    enum class Mode { Function0 = 0, Function1, Function2, Function3, Function4, Function5, Function6, Function7 };
    enum class SlewRate { Slow = 0, Fast };
    enum class DriveStrength { Normal_4mA = 0, Medium_8mA, High_14mA, Ultra_21mA };

    void setMode(IOPin pin, Mode mode) { sfsp[pin.port][pin.pin].bitfields.mode = static_cast<uint32_t>(mode); }

    void enablePullDown(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.epd = 1; }
    void disablePullDown(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.epd = 0; }

    void enablePullUp(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.epun = 0; }
    void disablePullUp(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.epun = 1; }

    void setSlewRate(IOPin pin, SlewRate slewRate) { sfsp[pin.port][pin.pin].bitfields.ehs = static_cast<uint32_t>(slewRate); }

    void enableInputBuffer(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.ezi = 1; }
    void disableInputBuffer(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.ezi = 0; }

    void enableGlitchFiter(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.zif = 0; }
    void disableGlitchFiter(IOPin pin) { sfsp[pin.port][pin.pin].bitfields.zif = 1; }

    void setDriveStrength(IOPin pin, DriveStrength strength) {
        if ((pin.port == IOPin::Port1 && pin.pin == 17)                         // P1_17
            || (pin.port == IOPin::Port2 && (pin.pin >= 3 && pin.pin <= 5))     // P2_3 to P3_5
            || (pin.port == IOPin::Port8 && pin.pin <= 2)                       // P8_0 to P8_2
            || (pin.port == IOPin::PortA && (pin.pin >= 1 && pin.pin <= 3))) {  // PA_1 to PA_3
            sfsp[pin.port][pin.pin].bitfields.ehd = static_cast<uint32_t>(strength);
        } else {
            // using pin that don't have Drive Strength setting capability
            std::terminate();
        }
    }

 private:
    volatile SFSP sfsp[16][32];
    volatile uint32_t RESERVED0[256];
    volatile uint32_t SFSCLK[4]; /*!< Pin configuration register for pins CLK0-3 */
    volatile uint32_t RESERVED16[28];
    volatile uint32_t SFSUSB;   /*!< Pin configuration register for USB */
    volatile uint32_t SFSI2C0;  /*!< Pin configuration register for I2C0-bus pins */
    volatile uint32_t ENAIO[3]; /*!< Analog function select registerS */
    volatile uint32_t RESERVED17[27];
    volatile uint32_t EMCDELAYCLK; /*!< EMC clock delay register */
    volatile uint32_t RESERVED18[63];
    volatile uint32_t PINTSEL0; /*!< Pin interrupt select register for pin interrupts 0 to 3. */
    volatile uint32_t PINTSEL1; /*!< Pin interrupt select register for pin interrupts 4 to 7. */
};

extern SCU &scu;

}  // namespace lpc43xx
}  // namespace microhal

#endif  // _MICROHAL_SCU_H_
