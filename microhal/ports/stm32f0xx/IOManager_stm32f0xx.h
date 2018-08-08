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

    template <int serial, SerialPinType serialType, stm32f0xx::GPIO::Port port, stm32f0xx::GPIO::Pin pinNr>
    static void routeSerial(stm32f0xx::GPIO::PullType pull = stm32f0xx::GPIO::NoPull, stm32f0xx::GPIO::OutputType type = stm32f0xx::GPIO::PushPull) {
        // clang-format off
        // assert for Serial1
        static_assert( (serial != 1 || serialType != Txd || ((port == GPIO::PortA && pinNr == 9))), "Serial1 Txd can be connected only to: PortA.9.");
        static_assert( (serial != 1 || serialType != Rxd || ((port == GPIO::PortA && pinNr == 10))), "Serial1 Rxd can be connected only to: PortA.10.");
        // clang-format on

        GPIO::setAlternateFunction(port, pinNr, GPIO::Serial, pull, type);
    }

    template <int i2cNumber, i2cPinType i2cType, GPIO::Port port, GPIO::Pin pinNr>
    static void routeI2C(stm32f0xx::GPIO::PullType pull = stm32f0xx::GPIO::NoPull, stm32f0xx::GPIO::OutputType type = stm32f0xx::GPIO::OpenDrain) {
        // clang-format off
		static_assert (i2cNumber != 0, "I2C port numbers starts from 1.");
		static_assert (i2cNumber <= 1, "STM32F4xx has only 1 I2C.");
		//assert for I2C1
		static_assert( (i2cNumber != 1 || i2cType != SDA || ((port == GPIO::PortF && pinNr == 0))), "I2C1 SDA can be connected only to: PortF.0.");
		static_assert( (i2cNumber != 1 || i2cType != SCL || ((port == GPIO::PortF && pinNr == 1))), "I2C1 SCL can be connected only to: PortF.1.");
        // clang-format on
        stm32f0xx::GPIO::setAlternateFunction(port, pinNr, stm32f0xx::GPIO::I2C, pull, type);
    }

 private:
};
}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_IOMANAGER_STM32F0XX_H_
