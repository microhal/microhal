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

typedef enum { MISO, MOSI, SCK } SpiPinType;

typedef enum { SCL, SDA } I2cPinType;

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

    template <int i2cNumber, I2cPinType i2cPinType, stm32g0xx::IOPin pin>
    static void routeI2C(stm32g0xx::GPIO::PullType pull = stm32g0xx::GPIO::NoPull, stm32g0xx::GPIO::OutputType type = stm32g0xx::GPIO::OpenDrain) {
        static_assert(i2cNumber != 0, "I2C numbers starts from 1.");
        static_assert(i2cNumber <= 6, "STM32G0XX family have up to 6 I2Cs.");
        auto af = stm32g0xx::GPIO::AlternateFunction::AF0;

        if constexpr (i2cNumber == 1) {
            static_assert(_MICROHAL_STM32G0XX_HAS_I2C1 && i2cNumber == 1, "I2C 1 unavailable in this MCU.");
            if constexpr (i2cPinType == SDA) {
                static_assert(pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 7} || pin == IOPin{IOPin::PortB, 9},
                              "I2C 1 SDA can be connected only to: PortA.10, PortB.7, PortB.9");
                af = stm32g0xx::GPIO::AlternateFunction::AF6;
            }
            if constexpr (i2cPinType == SCL) {
                static_assert(pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortB, 6} || pin == IOPin{IOPin::PortB, 8},
                              "I2C 1 SCL can be connected only to: PortA.9, PortB.6, PortB.8");
                af = stm32g0xx::GPIO::AlternateFunction::AF6;
            }
        }
        if constexpr (i2cNumber == 2) {
            static_assert(_MICROHAL_STM32G0XX_HAS_I2C2 && i2cNumber == 2, "I2C 2 unavailable in this MCU.");
            if constexpr (i2cPinType == SDA) {
                static_assert(pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortB, 11} || pin == IOPin{IOPin::PortB, 14},
                              "I2C 2 SDA can be connected only to: PortA.12, PortB.11, PortB.14");
                af = stm32g0xx::GPIO::AlternateFunction::AF6;
            }
            if constexpr (i2cPinType == SCL) {
                static_assert(pin == IOPin{IOPin::PortA, 11} || pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortB, 13},
                              "I2C 2 SCL can be connected only to: PortA.11, PortB.10, PortB.13");
                af = stm32g0xx::GPIO::AlternateFunction::AF6;
            }
        }

        stm32g0xx::GPIO gpio(pin);
        gpio.setAlternateFunction(af, pull, type);
    }
    constexpr static bool spiPinAssert([[maybe_unused]] int number, [[maybe_unused]] stm32g0xx::IOPin miso, [[maybe_unused]] stm32g0xx::IOPin mosi,
                                       [[maybe_unused]] stm32g0xx::IOPin sck) {
        // this function is used only on some MCUs where miso, mosi and sck can't be set independently.
        return true;
    }

    template <int spiNumber, SpiPinType spiType, IOPin pin>
    static void routeSPI(stm32g0xx::GPIO::PullType pull = stm32g0xx::GPIO::NoPull, stm32g0xx::GPIO::OutputType type = stm32g0xx::GPIO::PushPull) {
        static_assert(spiNumber != 0, "SPI port numbers starts from 1.");
        static_assert(spiNumber <= 2, "STM32G0xx has only 2 SPI.");

        if constexpr (spiNumber == 1) {
            if constexpr (spiType == SCK) {
                static_assert(
                    pin == IOPin{IOPin::PortA, 1} || pin == IOPin{IOPin::PortA, 5} || pin == IOPin{IOPin::PortB, 3} || pin == IOPin{IOPin::PortD, 8},
                    "SPI1 SCK can be connected only to: PortA.1 or PortA.5 or PortB.3 or PortD.8");
            }
            if constexpr (spiType == MISO) {
                static_assert(
                    pin == IOPin{IOPin::PortA, 6} || pin == IOPin{IOPin::PortA, 11} || pin == IOPin{IOPin::PortB, 4} || pin == IOPin{IOPin::PortD, 5},
                    "SPI1 MISO can be connected only to: PortA.6 or PortA.11 or PortB.4 or PortD.5");
            }
            if constexpr (spiType == MOSI) {
                static_assert(pin == IOPin{IOPin::PortA, 2} || pin == IOPin{IOPin::PortA, 7} || pin == IOPin{IOPin::PortA, 12} ||
                                  pin == IOPin{IOPin::PortB, 5} || pin == IOPin{IOPin::PortD, 6},
                              "SPI1 MOSI can be connected only to: PortA.7 or PortB.0 or PortB.4 or PortC.9 or PortF.6");
            }
        }
        if constexpr (spiNumber == 2) {
            if constexpr (spiType == SCK) {
                static_assert(pin == IOPin{IOPin::PortA, 0} || pin == IOPin{IOPin::PortB, 8} || pin == IOPin{IOPin::PortB, 10} ||
                                  pin == IOPin{IOPin::PortB, 13} || pin == IOPin{IOPin::PortD, 1},
                              "SPI1 SCK can be connected only to: PortA.0 or PortB.8 or PortB.10 or PortB.13 or PortD.1");
            }
            if constexpr (spiType == MISO) {
                static_assert(pin == IOPin{IOPin::PortA, 3} || pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortB, 2} ||
                                  pin == IOPin{IOPin::PortB, 6} || pin == IOPin{IOPin::PortB, 14} || pin == IOPin{IOPin::PortC, 2} ||
                                  pin == IOPin{IOPin::PortD, 3},
                              "SPI1 MISO can be connected only to: PortA.3 or PortA.9 or PortB.2 or PortB.6 or PortB.14 or PortC.2 or PortD.3");
            }
            if constexpr (spiType == MOSI) {
                static_assert(pin == IOPin{IOPin::PortA, 4} || pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 7} ||
                                  pin == IOPin{IOPin::PortB, 11} || pin == IOPin{IOPin::PortB, 15} || pin == IOPin{IOPin::PortC, 3} ||
                                  pin == IOPin{IOPin::PortD, 4},
                              "SPI1 MOSI can be connected only to: PortA.4 or PortA.10 or PortB.7 or PortB.11 or PortB.15 or PortC.3 or PortD.4");
            }
        }

        stm32g0xx::GPIO gpio(pin);
        if constexpr (pin == IOPin{IOPin::PortB, 10}) {
            gpio.setAlternateFunction(stm32g0xx::GPIO::AlternateFunction::AF5, pull, type);
        } else if constexpr (pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortB, 6}) {
            gpio.setAlternateFunction(stm32g0xx::GPIO::AlternateFunction::AF4, pull, type);
        } else if constexpr (pin.port == IOPin::PortD || pin.port == IOPin::PortC || pin == IOPin{IOPin::PortA, 4} || pin == IOPin{IOPin::PortB, 2} ||
                             pin == IOPin{IOPin::PortB, 7} || pin == IOPin{IOPin::PortB, 8}) {
            gpio.setAlternateFunction(stm32g0xx::GPIO::AlternateFunction::AF1, pull, type);
        } else {
            gpio.setAlternateFunction(stm32g0xx::GPIO::AlternateFunction::AF0, pull, type);
        }
    }

    template <int serial, SerialPinType serialType, stm32g0xx::IOPin pin>
    static void routeSerial(stm32g0xx::GPIO::PullType pull = stm32g0xx::GPIO::NoPull, stm32g0xx::GPIO::OutputType type = stm32g0xx::GPIO::PushPull) {
        static_assert(serial != 0, "Serial port numbers starts from 1.");
        static_assert(serial <= 4, "STM32G0 family have up to 4 SerialPorts.");
        auto af = stm32g0xx::GPIO::AlternateFunction::AF1;

        if constexpr (serial == 1) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART1 && serial == 1, "Serial Port 1 unavailable in this MCU.");
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortB, 6} || pin == IOPin{IOPin::PortC, 4},
                              "Serial1 TxD can be connected only to: PortA.9, PortB.6, PortC.4");
                if constexpr (pin == IOPin{IOPin::PortB, 6})
                    af = stm32g0xx::GPIO::AlternateFunction::AF0;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 7} || pin == IOPin{IOPin::PortC, 5},
                              "Serial1 RxD can be connected only to: PortA.10, PortB.7, PortC.5");
                if constexpr (pin == IOPin{IOPin::PortB, 7})
                    af = stm32g0xx::GPIO::AlternateFunction::AF0;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
        }
        if constexpr (serial == 2) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART1 && serial == 2, "Serial Port 2 unavailable in this MCU.");
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 2} || pin == IOPin{IOPin::PortA, 14} || pin == IOPin{IOPin::PortD, 5},
                              "Serial2 TxD can be connected only to: PortA.2, PortA.14, PortD.5");
                if constexpr (pin == IOPin{IOPin::PortD, 5})
                    af = stm32g0xx::GPIO::AlternateFunction::AF0;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 3} || pin == IOPin{IOPin::PortA, 15} || pin == IOPin{IOPin::PortD, 6},
                              "Serial2 RxD can be connected only to: PortA.3, PortA.15, PortD.6");
                if constexpr (pin == IOPin{IOPin::PortD, 6})
                    af = stm32g0xx::GPIO::AlternateFunction::AF0;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
        }
        if constexpr (serial == 3) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART1 && serial == 3, "Serial Port 3 unavailable in this MCU.");
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 5} || pin == IOPin{IOPin::PortB, 2} || pin == IOPin{IOPin::PortB, 8} ||
                                  pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortC, 4} || pin == IOPin{IOPin::PortC, 10} ||
                                  pin == IOPin{IOPin::PortD, 8},
                              "Serial3 TxD can be connected only to: PortA.5, PortB.2, PortB.8, PortB.10, PortC.4, PortC.10, PortD.8");
                if constexpr (pin == IOPin{IOPin::PortA, 5})
                    af = stm32g0xx::GPIO::AlternateFunction::AF3;
                else if constexpr (pin == IOPin{IOPin::PortC, 4} || pin == IOPin{IOPin::PortC, 10} || pin == IOPin{IOPin::PortD, 8})
                    af = stm32g0xx::GPIO::AlternateFunction::AF0;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF4;
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortB, 0} || pin == IOPin{IOPin::PortB, 9} || pin == IOPin{IOPin::PortB, 11} ||
                                  pin == IOPin{IOPin::PortC, 5} || pin == IOPin{IOPin::PortC, 11} || pin == IOPin{IOPin::PortD, 9},
                              "Serial3 RxD can be connected only to: PortB.0, PortB.9, PortB.11, PortC.5, PortC.11, PortD.9");
                if constexpr (pin == IOPin{IOPin::PortB, 0} || pin == IOPin{IOPin::PortB, 9} || pin == IOPin{IOPin::PortB, 11})
                    af = stm32g0xx::GPIO::AlternateFunction::AF4;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF0;
            }
        }
        if constexpr (serial == 4) {
            static_assert(_MICROHAL_STM32G0XX_HAS_USART1 && serial == 4, "Serial Port 4 unavailable in this MCU.");
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 0} || pin == IOPin{IOPin::PortC, 10},
                              "Serial4 TxD can be connected only to: PortA.0, PortC.10");
                if constexpr (pin == IOPin{IOPin::PortA, 0})
                    af = stm32g0xx::GPIO::AlternateFunction::AF4;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 1} || pin == IOPin{IOPin::PortC, 11},
                              "Serial4 RxD can be connected only to: PortA.1, PortC.11");
                if constexpr (pin == IOPin{IOPin::PortA, 1})
                    af = stm32g0xx::GPIO::AlternateFunction::AF4;
                else
                    af = stm32g0xx::GPIO::AlternateFunction::AF1;
            }
        }

        stm32g0xx::GPIO gpio(pin);
        gpio.setAlternateFunction(af, pull, type);
    }
};

}  // namespace stm32g0xx
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STM32G0XX_IOMANAGER_STM32G0XX_H_ */
