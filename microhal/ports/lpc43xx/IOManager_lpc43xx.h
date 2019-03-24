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

#ifndef _MICROHAL_IOMANAGER_LPC43XX_H_
#define _MICROHAL_IOMANAGER_LPC43XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "IOPin.h"
#include "gpio_lpc43xx.h"
#include "scu.h"
#include "spi_polling_lpc43xx.h"

namespace microhal {
namespace lpc43xx {

class IOManager {
 public:
    using GPIO = lpc43xx::GPIO;
    IOManager() = delete;

    template <SPI_polling::Number spiNumber, const IOPin &miso, const IOPin &mosi, const IOPin &sck>
    static void routeSPI(GPIO::PullType pull = GPIO::NoPull) {
        static_assert(miso != mosi, "Can't connect MISO and MOSI pins to the same IO pin.");
        static_assert(miso != sck, "Can't connect MISO and SCK pins to the same IO pin.");
        static_assert(mosi != sck, "Can't connect MOSU and SCK pins to the same IO pin.");

        if constexpr (spiNumber == SPI_polling::Number::SSP0) {
            static_assert(miso == IOPin{IOPin::Port1, 1} || miso == IOPin{IOPin::Port3, 6} || miso == IOPin{IOPin::Port3, 7} ||
                              miso == IOPin{IOPin::Port9, 1} || miso == IOPin{IOPin::PortF, 2},
                          "Incorrect MISO pin. SPI0 (SSP0) MISO pin can be connected to pins: Port1.1, Port3.6, Port3,7, Port9.1, PortF.2");
            static_assert(mosi == IOPin{IOPin::Port1, 2} || mosi == IOPin{IOPin::Port3, 7} || mosi == IOPin{IOPin::Port3, 8} ||
                              mosi == IOPin{IOPin::Port9, 2} || mosi == IOPin{IOPin::PortF, 3},
                          "Incorrect MOSI pin. SPI0 (SSP0) MOSI pin can be connected to pins: Port1.2, Port3.7, Port3.8, Port9.2, PortF.3");
            static_assert(sck == IOPin{IOPin::Port3, 0} || sck == IOPin{IOPin::Port3, 3} || sck == IOPin{IOPin::PortF, 0},
                          "Incorrect SCK pin. SPI0 (SSP0) SCK pin can be connected to pins: Port3.0, Port3.3, PortF.0. ");
        }
        if constexpr (spiNumber == SPI_polling::Number::SSP1) {
            static_assert(miso == IOPin{IOPin::Port0, 0} || miso == IOPin{IOPin::Port1, 3} || miso == IOPin{IOPin::PortF, 6},
                          "Incorrect MISO pin. SPI0 (SSP0) MISO pin can be connected to pins: Port0.0, Port1.3, PortF.6.");
            static_assert(mosi == IOPin{IOPin::Port0, 1} || mosi == IOPin{IOPin::Port1, 4} || mosi == IOPin{IOPin::PortF, 7},
                          "Incorrect MOSI pin. SPI0 (SSP0) MOSI pin can be connected to pins: Port0.1, Port1.4, PortF.7");
            static_assert(sck == IOPin{IOPin::Port1, 19} || sck == IOPin{IOPin::PortF, 4} || sck == IOPin{IOPin::CLK, 0},
                          "Incorrect SCK pin. SPI0 (SSP0) SCK pin can be connected to pins: Port1.19, PortF4, CLK.0");
        }

        scu.setMode(miso, spiFunc(miso, true, false));
        scu.setMode(mosi, spiFunc(mosi, false, true));
        scu.setMode(sck, spiFunc(sck, false, false));
    }

 private:
    static constexpr SCU::Mode spiFunc(IOPin pin, bool miso, bool mosi) {
        if (pin.port == IOPin::Port0) return SCU::Mode::Function1;
        if (pin.port == IOPin::Port1) {
            if (pin.pin < 7)
                return SCU::Mode::Function5;
            else
                return SCU::Mode::Function1;
        }
        if (pin.port == IOPin::Port3) {
            if (pin.pin == 0) return SCU::Mode::Function4;
            if (pin.pin == 2) return SCU::Mode::Function2;
            if (pin.pin == 6 && miso)
                return SCU::Mode::Function5;
            else
                return SCU::Mode::Function2;
            if (pin.pin == 7 && miso)
                return SCU::Mode::Function2;
            else
                return SCU::Mode::Function5;
            if (pin.pin == 8 && mosi)
                return SCU::Mode::Function2;
            else
                return SCU::Mode::Function5;
        }
        if (pin.port == IOPin::Port9) return SCU::Mode::Function7;
        if (pin.port == IOPin::PortF) {
            if (pin.pin == 0 || pin.pin == 4)
                return SCU::Mode::Function0;
            else
                return SCU::Mode::Function2;
        }
        if (pin.port == IOPin::CLK) return SCU::Mode::Function6;
    }
};  // namespace lpc43xx
}  // namespace lpc43xx
}  // namespace microhal

#endif  // _MICROHAL_IOMANAGER_LPC43XX_H_
