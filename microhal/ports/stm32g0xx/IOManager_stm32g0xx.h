/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-02-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#ifndef SRC_MICROHAL_PORTS_STM32G0XX_IOMANAGER_STM32G0XX_H_
#define SRC_MICROHAL_PORTS_STM32G0XX_IOMANAGER_STM32G0XX_H_

#include "gpio_stm32g0xx.h"
#include "mcuCapabilities.h"

namespace microhal {

typedef enum {
    Rxd,
    Txd,
} SerialPinType;

namespace stm32g0xx {

class IOManager {
 public:
    IOManager() = delete;

    template <int adcNumber, int channel, stm32g0xx::IOPin pin>
    static void routeADC(stm32g0xx::GPIO::PullType pullUpOrDown = stm32g0xx::GPIO::NoPull) {
        static_assert(adcNumber > 0, "ADC numbering starts from 1.");
        static_assert(adcNumber == 1, "STM32G0xx has only 1 ADC");
        static_assert(channel != 12 && channel != 13 && channel != 14,
                      "Channels 12, 13 and 14 are used internally and can't be routed outside chip.");

        static_assert(channel != 0 || pin == IOPin{IOPin::PortA, 0}, "ADC1 Channel 0 can be connected only to PortA.0");
        static_assert(channel != 1 || pin == IOPin{IOPin::PortA, 1}, "ADC1 Channel 1 can be connected only to PortA.1");
        static_assert(channel != 2 || pin == IOPin{IOPin::PortA, 2}, "ADC1 Channel 2 can be connected only to PortA.2");
        static_assert(channel != 3 || pin == IOPin{IOPin::PortA, 3}, "ADC1 Channel 3 can be connected only to PortA.3");
        static_assert(channel != 4 || pin == IOPin{IOPin::PortA, 4}, "ADC1 Channel 4 can be connected only to PortA.4");
        static_assert(channel != 5 || pin == IOPin{IOPin::PortA, 5}, "ADC1 Channel 5 can be connected only to PortA.5");
        static_assert(channel != 6 || pin == IOPin{IOPin::PortA, 6}, "ADC1 Channel 6 can be connected only to PortA.6");
        static_assert(channel != 7 || pin == IOPin{IOPin::PortA, 7}, "ADC1 Channel 7 can be connected only to PortA.7");
        static_assert(channel != 8 || pin == IOPin{IOPin::PortB, 0}, "ADC1 Channel 8 can be connected only to PortB.0");
        static_assert(channel != 9 || pin == IOPin{IOPin::PortB, 1}, "ADC1 Channel 9 can be connected only to PortB.1");
        static_assert(channel != 10 || pin == IOPin{IOPin::PortB, 2}, "ADC1 Channel 10 can be connected only to PortB.2");
        static_assert(channel != 11 || pin == IOPin{IOPin::PortB, 10}, "ADC1 Channel 11 can be connected only to PortB.10");
        static_assert(channel != 15 || pin == IOPin{IOPin::PortB, 11}, "ADC1 Channel 15 can be connected only to PortB.11");
        static_assert(channel != 16 || pin == IOPin{IOPin::PortB, 12}, "ADC1 Channel 16 can be connected only to PortB.12");
        static_assert(channel != 17 || pin == IOPin{IOPin::PortC, 4}, "ADC1 Channel 17 can be connected only to PortC.4");
        static_assert(channel != 18 || pin == IOPin{IOPin::PortC, 5}, "ADC1 Channel 18 can be connected only to PortC.5");

        GPIO gpio(pin);
        gpio.configureAsInput(pullUpOrDown);
    }

    template <int serial, SerialPinType serialType, stm32g0xx::IOPin pin>
    static void routeSerial(stm32g0xx::GPIO::PullType pull = stm32g0xx::GPIO::NoPull, stm32g0xx::GPIO::OutputType type = stm32g0xx::GPIO::PushPull) {
        static_assert(serial != 0, "Serial port numbers starts from 1.");
        static_assert(serial < 7, "STM32G0 family have up to 7 SerialPorts.");
        auto af = stm32g0xx::GPIO::AlternateFunction::AF1;

        if constexpr (serial == 1) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART1 && serial == 1, "Serial Port 1 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortB, 6} || pin == IOPin{IOPin::PortC, 4},
                              "Serial1 Txd can be connected only to: PortA.9, PortB.6 or PortC.4.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 7} || pin == IOPin{IOPin::PortC, 5},
                              "Serial1 Rxd can be connected only to: PortA.10, PortB.7 or PortC.5.");
            if constexpr (pin == IOPin{IOPin::PortB, 6} || pin == IOPin{IOPin::PortB, 7}) {
                af = stm32g0xx::GPIO::AlternateFunction::AF0;
            } else {
                af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
        }
        if constexpr (serial == 2) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART2 && serial == 2, "Serial Port 2 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortA, 2} || pin == IOPin{IOPin::PortA, 14} || pin == IOPin{IOPin::PortD, 5},
                              "Serial2 Txd can be connected only to: PortA.2, PortA.14 or PortD.5.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortA, 3} || pin == IOPin{IOPin::PortA, 15} || pin == IOPin{IOPin::PortD, 6},
                              "Serial2 Rxd can be connected only to: PortA.3, PortA.15 or PortD.6.");
            if constexpr (pin == IOPin{IOPin::PortD, 5} || pin == IOPin{IOPin::PortD, 6}) {
                af = stm32g0xx::GPIO::AlternateFunction::AF0;
            } else {
                af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
        }
        if constexpr (serial == 3) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART3 && serial == 3, "Serial Port 3 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortA, 5} || pin == IOPin{IOPin::PortB, 2} || pin == IOPin{IOPin::PortB, 8} ||
                                  pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortC, 4} || pin == IOPin{IOPin::PortC, 10} ||
                                  pin == IOPin{IOPin::PortD, 8},
                              "Serial3 Txd can be connected only to: PortA.5, PortB.2, PortB.8, PortB.10, PortC.4, PortC.10, PortD.8.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortB, 0} || pin == IOPin{IOPin::PortB, 9} || pin == IOPin{IOPin::PortB, 11} ||
                                  pin == IOPin{IOPin::PortC, 5} || pin == IOPin{IOPin::PortC, 11} || pin == IOPin{IOPin::PortD, 9},
                              "Serial3 Rxd can be connected only to: PortB.0, PortB.9, PortB.11, PortC.5, PortC.11, PortD.9.");
            if constexpr (pin.port == IOPin::PortA || pin.port == IOPin::PortB) {
                af = stm32g0xx::GPIO::AlternateFunction::AF4;
            } else {
                af = stm32g0xx::GPIO::AlternateFunction::AF0;
            }
        }
        if constexpr (serial == 4) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART4 && serial == 4, "Serial Port 3 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortA, 0} || pin == IOPin{IOPin::PortC, 10},
                              "Serial4 Txd can be connected only to: PortA.0, PortC.10.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortA, 1} || pin == IOPin{IOPin::PortC, 11},
                              "Serial4 Rxd can be connected only to: PortA.1, PortC.11.");
            if constexpr (pin.port == IOPin::PortA) {
                af = stm32g0xx::GPIO::AlternateFunction::AF4;
            } else {
                af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
        }
        if constexpr (serial == 5) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART5 && serial == 5, "Serial Port 3 unavailable in this MCU.");
            if constexpr (serialType == Txd) static_assert(serialType == Rxd, "Unimplemented");
            if constexpr (serialType == Rxd) static_assert(serialType == Txd, "Unimplemented");
        }
        if constexpr (serial == 6) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART6 && serial == 6, "Serial Port 3 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortB, 9} || pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortC, 10},
                              "Serial3 Txd can be connected only to: PortB.9, PortB.10, PortC.10.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortB, 8} || pin == IOPin{IOPin::PortB, 11} || pin == IOPin{IOPin::PortC, 11},
                              "Serial3 Rxd can be connected only to: PortB.8, PortB.11, PortC.11.");
        }
        stm32g0xx::GPIO gpio(pin);
        gpio.setAlternateFunction(af, pull, type);
    }
};

}  // namespace stm32g0xx
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STM32G0XX_IOMANAGER_STM32G0XX_H_ */
