/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    buleks, Pawel Okas
 * created on: 21-07-2016
 * last modification: 21-07-2016
 *
 * @copyright Copyright (c) 2016-2018, Pawel Okas
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

#ifndef _MICROHAL_IOMANAGER_STM32F3XX_H_
#define _MICROHAL_IOMANAGER_STM32F3XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <microhalPortConfig_stm32f3xx.h>

/* **************************************************************************************************************************************************
 * CLASS
 */

namespace microhal {
typedef enum {
    Rxd,
    Txd,
} SerialPinType;

typedef enum { MISO, MOSI, SCK } SpiPinType;

typedef enum { SCL, SDA } i2cPinType;

typedef enum {
    Spi1 = 1,
    Spi2 = 2,
} SpiPorts;

typedef enum { RX, TX } CanPinType;

typedef enum { OTG_FS_SOF, OTG_FS_ID, OTG_FS_VBUS, OTG_FS_DM, OTG_FS_DP } USBPinType;

namespace stm32f3xx {

class IOManager {
 public:
    IOManager() = delete;

    template <int serial, SerialPinType serialType, stm32f3xx::IOPin pin>
    static void routeSerial(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::PullType::NoPull,
                            stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::OutputType::PushPull) {
        static_assert(serial < 4, "This MCU have only 3 Serial ports.");
        static_assert(serial != 0, "Serial port numbers starts from 1.");

        if constexpr (serial == 1) {
            if constexpr (serialType == Txd)
                static_assert((pin == IOPin{IOPin::PortA, 9}) || (pin == IOPin{IOPin::PortB, 6}) || (pin == IOPin{IOPin::PortC, 4}),
                              "Serial1 Txd can be connected only to: PortA.9, PortB.6 or PortC.4.");
            if constexpr (serialType == Rxd)
                static_assert((pin == IOPin{IOPin::PortA, 10}) || (pin == IOPin{IOPin::PortB, 7}) || (pin == IOPin{IOPin::PortC, 5}),
                              "Serial1 Rxd can be connected only to: PortA.10, PortB.7 or PortC.5.");
        }
        if constexpr (serial == 2) {
            if constexpr (serialType == Txd)
                static_assert((pin == IOPin{IOPin::PortA, 2}) || (pin == IOPin{IOPin::PortA, 14}) || (pin == IOPin{IOPin::PortB, 3}),
                              "Serial2 Txd can be connected only to: PortA.2, PortA.14 or PortB.3.");
            if constexpr (serialType == Rxd)
                static_assert((pin == IOPin{IOPin::PortA, 3}) || (pin == IOPin{IOPin::PortA, 15}) || (pin == IOPin{IOPin::PortB, 4}),
                              "Serial2 Rxd can be connected only to: PortA.3, PortA.15 or PortB.4.");
        }
        if constexpr (serial == 3) {
            if constexpr (serialType == Txd)
                static_assert((pin == IOPin{IOPin::PortB, 9}) || (pin == IOPin{IOPin::PortB, 10}) || (pin == IOPin{IOPin::PortC, 10}),
                              "Serial3 Txd can be connected only to: PortB.9, PortB.10, PortC.10.");
            if constexpr (serialType == Rxd)
                static_assert((pin == IOPin{IOPin::PortB, 8}) || (pin == IOPin{IOPin::PortB, 9}) || (pin == IOPin{IOPin::PortC, 11}),
                              "Serial3 Rxd can be connected only to: PortB.8, PortB.11, PortC.11.");
        }
        stm32f3xx::GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f3xx::GPIO::AlternateFunction::AF7, pull, type);
    }

    template <int spiNumber, SpiPinType spiType, stm32f3xx::IOPin pin>
    static void routeSPI(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::PullType::NoPull,
                         stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::OutputType::PushPull) {
        static_assert(spiNumber != 0, "SPI port numbers starts from 1.");
        static_assert(spiNumber <= 3, "STM32F3xx has only 3 SPI.");

        stm32f3xx::GPIO gpio(pin);
        if (spiNumber == 3) {
            gpio.setAlternateFunction(stm32f3xx::GPIO::AlternateFunction::AF6, pull, type);
        } else if (spiNumber == 2) {
            gpio.setAlternateFunction(stm32f3xx::GPIO::AlternateFunction::AF5, pull, type);
        } else if (spiNumber == 1) {
            if ((pin.pin == 11) || (pin.pin == 12) || (pin.pin == 13)) {
                gpio.setAlternateFunction(stm32f3xx::GPIO::AlternateFunction::AF6, pull, type);
            } else {
                gpio.setAlternateFunction(stm32f3xx::GPIO::AlternateFunction::AF5, pull, type);
            }
        }
    }

    template <int number>
    static void routeSpi(IOPin miso, IOPin mosi, IOPin sck, stm32f3xx::GPIO::OutputType mosiType, stm32f3xx::GPIO::OutputType sckType) {
        GPIO gpioMiso(miso);
        GPIO gpioMosi(mosi);
        GPIO gpioSck(sck);

        stm32f3xx::GPIO::AlternateFunction af = [](IOPin mosi, IOPin miso, IOPin sck) {
            if (number == 3) {
                return stm32f3xx::GPIO::AlternateFunction::AF6;
            } else if (number == 2) {
                return stm32f3xx::GPIO::AlternateFunction::AF5;
            } else if (number == 1) {
                if ((mosi.pin == 11) || (mosi.pin == 12) || (mosi.pin == 13) || (miso.pin == 11) || (miso.pin == 12) || (miso.pin == 13) ||
                    (sck.pin == 11) || (sck.pin == 12) || (sck.pin == 13)) {
                    return stm32f3xx::GPIO::AlternateFunction::AF6;
                } else {
                    return stm32f3xx::GPIO::AlternateFunction::AF5;
                }
            }
        }(mosi, miso, sck);

        gpioMiso.setAlternateFunction(af, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
        gpioMosi.setAlternateFunction(af, GPIO::PullType::NoPull, mosiType);
        gpioSck.setAlternateFunction(af, GPIO::PullType::NoPull, sckType);
    }

    static constexpr bool spiPinAssert(int number, IOPin miso, IOPin mosi, IOPin sck) {
        if (number == 1) {
            if (!(sck == IOPin{IOPin::PortA, 5} || sck == IOPin{IOPin::PortA, 12} || sck == IOPin{IOPin::PortB, 3} || sck == IOPin{IOPin::PortC, 7}))
                return false;
            if (!(miso == IOPin{IOPin::PortA, 6} || miso == IOPin{IOPin::PortA, 13} || miso == IOPin{IOPin::PortB, 4} ||
                  miso == IOPin{IOPin::PortC, 8}))
                return false;
            if (!(mosi == IOPin{IOPin::PortA, 7} || mosi == IOPin{IOPin::PortB, 0} || mosi == IOPin{IOPin::PortB, 5} ||
                  mosi == IOPin{IOPin::PortC, 9} || mosi == IOPin{IOPin::PortF, 6}))
                return false;
        } else if (number == 2) {
            if (!(sck == IOPin{IOPin::PortB, 10} || sck == IOPin{IOPin::PortD, 8} || sck == IOPin{IOPin::PortA, 8} || sck == IOPin{IOPin::PortD, 7} ||
                  sck == IOPin{IOPin::PortB, 8}))
                return false;
            if (!(miso == IOPin{IOPin::PortC, 2} || miso == IOPin{IOPin::PortB, 14} || miso == IOPin{IOPin::PortA, 9} ||
                  miso == IOPin{IOPin::PortD, 3}))
                return false;
            if (!(mosi == IOPin{IOPin::PortC, 3} || mosi == IOPin{IOPin::PortB, 15} || mosi == IOPin{IOPin::PortA, 10} ||
                  mosi == IOPin{IOPin::PortD, 4}))
                return false;
        } else if (number == 3) {
            if (!(sck == IOPin{IOPin::PortA, 1} || sck == IOPin{IOPin::PortB, 3} || sck == IOPin{IOPin::PortC, 10})) return false;
            if (!(miso == IOPin{IOPin::PortA, 2} || miso == IOPin{IOPin::PortB, 4} || miso == IOPin{IOPin::PortC, 11})) return false;
            if (!(mosi == IOPin{IOPin::PortA, 3} || mosi == IOPin{IOPin::PortB, 5} || mosi == IOPin{IOPin::PortC, 12})) return false;
        } else {
            return false;
        }
        return true;
    }

    template <int i2cNumber, i2cPinType i2cType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeI2C(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::PullType::NoPull,
                         stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::OutputType::OpenDrain) {
        static_assert(i2cNumber != 0, "I2C port numbers starts from 1.");
        static_assert(i2cNumber <= 3, "STM32F3xx has only 3 I2C.");
        constexpr IOPin pin(port, pinNr);

        if constexpr (i2cNumber == 1) {
            if constexpr (i2cType == SDA)
                static_assert((port == IOPin::PortB && pinNr == 7) || (port == IOPin::PortB && pinNr == 9),
                              "I2C1 SDA can be connected only to: PortB.7 or PortB.9.");
            if constexpr (i2cType == SCL)
                static_assert((port == IOPin::PortB && pinNr == 6) || (port == IOPin::PortB && pinNr == 8),
                              "I2C1 SCL can be connected only to: PortB.6 or PortB.8.");
        }
        if constexpr (i2cNumber == 2) {
            if constexpr (i2cType == SDA) static_assert(port == IOPin::PortB && pinNr == 11, "I2C2 SDA can be connected only to: PortB.11.");
            if constexpr (i2cType == SCL) static_assert(port == IOPin::PortB && pinNr == 10, "I2C2 SCL can be connected only to: PortB.10.");
        }
        if constexpr (i2cNumber == 3) {
            if constexpr (i2cType == SDA) static_assert(port == IOPin::PortC && pinNr == 9, "I2C3 SDA can be connected only to: PortC.9.");
            if constexpr (i2cType == SCL) static_assert(port == IOPin::PortA && pinNr == 8, "I2C3 SCL can be connected only to: PortA.8.");
        }

        stm32f3xx::GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f3xx::GPIO::AlternateFunction::AF4, pull, type);
    }

    template <int adcNumber, IOPin::Port port, IOPin::Pin pinNr>
    static void routeADC(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::PullType::NoPull) {
        static_assert(adcNumber != 0, "ADC numbers starts from 1.");
        static_assert(adcNumber <= 2, "STM32F3xx has only 2 ADC.");
        constexpr IOPin pin(port, pinNr);

        if constexpr (adcNumber == 1) {
            if constexpr (port == IOPin::PortA) {
                static_assert(pinNr >= 0 && pinNr <= 3, "Incorrect ADC Pin");
            }
        }

        stm32f3xx::GPIO gpio(pin);
        gpio.setAnalogFunction(pull);
    }

    template <int DACNumber, int channel, IOPin::Port port, IOPin::Pin pinNr>
    static void routeDAC() {
        static_assert(DACNumber != 0, "DAC port numbers starts from 1.");
        static_assert(DACNumber <= 2, "STM32F3xx has only 2 DACs.");

        constexpr IOPin pin(port, pinNr);
        if constexpr (DACNumber == 1) {
            static_assert(channel <= 2 && channel > 0, "DAC1 have only channel 1 and channel 2.");
            if constexpr (channel == 1) {
                static_assert(pin == IOPin{IOPin::PortA, 4}, "DAC1 channel1 can be connected only to: PortA.4.");
            }
            if constexpr (channel == 2) {
                static_assert(pin == IOPin{IOPin::PortA, 5}, "DAC1 channel1 can be connected only to: PortA.4.");
            }
        }
        if constexpr (DACNumber == 2) {
            static_assert(channel == 1, "DAC2 have only channel 1.");
            if constexpr (channel == 1) {
                static_assert(pin == IOPin{IOPin::PortA, 6}, "I2C2 SDA can be connected only to: PortB.11.");
            }
        }
        stm32f3xx::GPIO gpio(pin);
        gpio.setAnalogFunction();
    }

    template <int TimerNumber, int channel, IOPin::Port port, IOPin::Pin pinNr>
    static void routeTimer(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::PullType::NoPull,
                           stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::OutputType::PushPull) {
        constexpr IOPin pin(port, pinNr);

        GPIO::AlternateFunction alternateFunction = GPIO::AlternateFunction::AF2;
        // Timer 1
        if constexpr (TimerNumber == 1) {
            if constexpr (channel == 1) {
                static_assert(pin == IOPin{IOPin::PortA, 8} || pin == IOPin{IOPin::PortB, 13} || pin == IOPin{IOPin::PortC, 0},
                              "TIM1 channel1 can be connected only to: PortA.8, PortB.13 or PortC.0.");
            }
            if constexpr (channel == 3) {
                static_assert(pin == IOPin{IOPin::PortB, 15} || pin == IOPin{IOPin::PortC, 2},
                              "TIM1 channel3 can be connected only to: PortB.15 or PortC.2.");
            }
            if constexpr (channel == 4) {
                static_assert(pin == IOPin{IOPin::PortA, 11} || pin == IOPin{IOPin::PortC, 3},
                              "TIM1 channel3 can be connected only to: PortA.11 or PortC.3.");
                if constexpr (pin == IOPin{IOPin::PortA, 11}) {
                    alternateFunction = GPIO::AlternateFunction::AF11;
                }
                if constexpr (pin == IOPin{IOPin::PortC, 3}) {
                    alternateFunction = GPIO::AlternateFunction::AF2;
                }
            }
        }
        // Timer 3
        if constexpr (TimerNumber == 3) {
            if constexpr (channel == 1) {
                static_assert(pin == IOPin{IOPin::PortA, 6} || pin == IOPin{IOPin::PortB, 4} || pin == IOPin{IOPin::PortC, 6},
                              "TIM3 channel1 can be connected only to: PortA.6, PortB.4 or PortC.6.");
                alternateFunction = GPIO::AlternateFunction::AF2;
            }
            if constexpr (channel == 2) {
                static_assert(
                    pin == IOPin{IOPin::PortA, 4} || pin == IOPin{IOPin::PortA, 7} || pin == IOPin{IOPin::PortB, 5} || pin == IOPin{IOPin::PortC, 7},
                    "TIM3 channel2 can be connected only to: PortA.4, PortA.7, PortB.5 or PortC.7.");
                alternateFunction = GPIO::AlternateFunction::AF2;
            }
            if constexpr (channel == 3) {
                static_assert(pin == IOPin{IOPin::PortB, 0} || pin == IOPin{IOPin::PortC, 8},
                              "TIM3 channel3 can be connected only to: PortB.0 or PortC.8.");
                alternateFunction = GPIO::AlternateFunction::AF2;
            }
            if constexpr (channel == 4) {
                static_assert(pin == IOPin{IOPin::PortB, 1} || pin == IOPin{IOPin::PortB, 7} || pin == IOPin{IOPin::PortC, 9},
                              "TIM3 channel4 can be connected only to: PortB.1, PortB.7 or PortC.9.");
                if constexpr (pin == IOPin{IOPin::PortB, 7}) {
                    alternateFunction = GPIO::AlternateFunction::AF2;
                } else {
                    alternateFunction = GPIO::AlternateFunction::AF10;
                }
            }
        }

        if constexpr (pin == IOPin{IOPin::PortB, 14} || pin == IOPin{IOPin::PortB, 13}) {
            alternateFunction = GPIO::AlternateFunction::AF6;
        }
        if constexpr (pin == IOPin{IOPin::PortB, 15}) {
            alternateFunction = GPIO::AlternateFunction::AF4;
        }

        stm32f3xx::GPIO gpio(pin);
        gpio.setAlternateFunction(alternateFunction, pull, type);
    }

    template <CanPinType canPinType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeCAN() {
        constexpr IOPin pin(port, pinNr);
        if constexpr (canPinType == RX) {
            static_assert(pin == IOPin{IOPin::PortA, 11} || pin == IOPin{IOPin::PortB, 8}, "CAN RX can be connected only to: PortA.11 or PortB.8.");
        }
        if constexpr (canPinType == TX) {
            static_assert(pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortB, 9}, "CAN TX can be connected only to: PortA.12 or PortB.9.");
        }

        stm32f3xx::GPIO gpio(pin);
        gpio.setAlternateFunction(GPIO::AlternateFunction::AF9);
    }

 private:
};
}  // namespace stm32f3xx
}  // namespace microhal
#endif  // _MICROHAL_IOMANAGER_STM32F3XX_H_
