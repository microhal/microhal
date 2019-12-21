/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-08-2018
 * last modification: 02-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_IOMANAGER_STM32F0XX_H_
#define _MICROHAL_IOMANAGER_STM32F0XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "gpio_stm32f0xx.h"
#include "microhalPortConfig_stm32f0xx.h"

namespace microhal {
typedef enum {
    Rxd,
    Txd,
} SerialPinType;

typedef enum { SCL, SDA } i2cPinType;

namespace stm32f0xx {

class IOManager {
 public:
    IOManager() = delete;

    template <int serial, SerialPinType serialType, stm32f0xx::IOPin::Port port, stm32f0xx::IOPin::Pin pinNr>
    static void routeSerial(stm32f0xx::GPIO::PullType pull = stm32f0xx::GPIO::NoPull, stm32f0xx::GPIO::OutputType type = stm32f0xx::GPIO::PushPull) {
        constexpr IOPin pin(port, pinNr);

        if constexpr (serial == 1) {
            static_assert(serialType != Txd || (pin == IOPin{IOPin::PortA, 9} || (pin == IOPin{IOPin::PortB, 6})),
                          "Serial1 Txd can be connected only to: PortA.9 or PortB.6.");
            static_assert(serialType != Rxd || (pin == IOPin{IOPin::PortA, 10}) || (pin == IOPin{IOPin::PortB, 7}),
                          "Serial1 Rxd can be connected only to: PortA.10 or PortB.7.");
        }
        if constexpr (serial == 2) {
            static_assert(serialType != Txd || (pin == IOPin{IOPin::PortA, 1} || (pin == IOPin{IOPin::PortA, 14})),
                          "Serial1 Txd can be connected only to: PortA.1 or PortA.14.");
            static_assert(serialType != Rxd || (pin == IOPin{IOPin::PortA, 3}) || (pin == IOPin{IOPin::PortA, 15}),
                          "Serial1 Rxd can be connected only to: PortA.3 or PortA.15.");
        }

        stm32f0xx::GPIO gpio(pin);
        gpio.setAlternateFunction(GPIO::AlternateFunction::Serial, pull, type);
    }

    template <int i2cNumber, i2cPinType i2cType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeI2C(stm32f0xx::GPIO::PullType pull = stm32f0xx::GPIO::NoPull, stm32f0xx::GPIO::OutputType type = stm32f0xx::GPIO::OpenDrain) {
        IOPin pin(port, pinNr);

        static_assert(i2cNumber != 0, "I2C port numbers starts from 1.");
        static_assert(i2cNumber <= 1, "STM32F4xx has only 1 I2C.");
        // clang-format off
		//assert for I2C1
		static_assert( (i2cNumber != 1 || i2cType != SDA || ((port == IOPin::PortF && pinNr == 0))), "I2C1 SDA can be connected only to: PortF.0.");
		static_assert( (i2cNumber != 1 || i2cType != SCL || ((port == IOPin::PortF && pinNr == 1))), "I2C1 SCL can be connected only to: PortF.1.");
        // clang-format on
        stm32f0xx::GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f0xx::GPIO::AlternateFunction::I2C, pull, type);
    }

    static constexpr bool spiPinAssert(int number, IOPin miso, IOPin mosi, IOPin sck) {
        if (number == 1) {
            if (!(sck == IOPin{IOPin::PortA, 5} || sck == IOPin{IOPin::PortC, 7} || sck == IOPin{IOPin::PortA, 12} || sck == IOPin{IOPin::PortB, 3}))
                return false;

            if (!(miso == IOPin{IOPin::PortA, 6} || miso == IOPin{IOPin::PortC, 8} || miso == IOPin{IOPin::PortA, 13} ||
                  miso == IOPin{IOPin::PortB, 5}))
                return false;

            if (!(mosi == IOPin{IOPin::PortA, 7} || mosi == IOPin{IOPin::PortB, 0} || mosi == IOPin{IOPin::PortB, 4} ||
                  mosi == IOPin{IOPin::PortC, 9} || mosi == IOPin{IOPin::PortF, 6}))
                return false;
        }
        return true;
    }

    template <int number>
    static void routeSpi(IOPin miso, IOPin mosi, IOPin sck, stm32f0xx::GPIO::OutputType mosiType, stm32f0xx::GPIO::OutputType sckType) {
        (void)miso;
        GPIO gpioMosi(mosi);
        GPIO gpioSck(sck);
        gpioMosi.setAlternateFunction(stm32f0xx::GPIO::AlternateFunction::I2C, GPIO::NoPull, mosiType);
        gpioSck.setAlternateFunction(stm32f0xx::GPIO::AlternateFunction::I2C, GPIO::NoPull, sckType);
    }

 private:
};
}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_IOMANAGER_STM32F0XX_H_
