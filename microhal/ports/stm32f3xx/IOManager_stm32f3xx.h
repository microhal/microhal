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
#include "microhalPortConfig_stm32f3xx.h"
#include "spi_stm32f3xx.h"

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

    template <int serial, SerialPinType serialType, stm32f3xx::IOPin::Port port, stm32f3xx::IOPin::Pin pinNr>
    static void routeSerial(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::PushPull) {
        static_assert(serial < 4, "This MCU have only 3 Serial ports.");
        static_assert(serial != 0, "Serial port numbers starts from 1.");
        if constexpr (serial == 1) {
            if constexpr (serialType == Txd)
                static_assert((port == IOPin::PortA && pinNr == 9) || (port == IOPin::PortB && pinNr == 6) || (port == IOPin::PortC && pinNr == 4),
                              "Serial1 Txd can be connected only to: PortA.9, PortB.6 or PortC.4.");
            if constexpr (serialType == Rxd)
                static_assert((port == IOPin::PortA && pinNr == 10) || (port == IOPin::PortB && pinNr == 7) || (port == IOPin::PortC && pinNr == 5),
                              "Serial1 Rxd can be connected only to: PortA.10, PortB.7 or PortC.5.");
        }
        if constexpr (serial == 2) {
            if constexpr (serialType == Txd)
                static_assert((port == IOPin::PortA && pinNr == 2) || (port == IOPin::PortA && pinNr == 14) || (port == IOPin::PortB && pinNr == 3),
                              "Serial2 Txd can be connected only to: PortA.2, PortA.14 or PortB.3.");
            if constexpr (serialType == Rxd)
                static_assert((port == IOPin::PortA && pinNr == 3) || (port == IOPin::PortA && pinNr == 15) || (port == IOPin::PortB && pinNr == 4),
                              "Serial2 Rxd can be connected only to: PortA.3, PortA.15 or PortB.4.");
        }
        if constexpr (serial == 3) {
            if constexpr (serialType == Txd)
                static_assert((port == IOPin::PortB && pinNr == 9) || (port == IOPin::PortB && pinNr == 10) || (port == IOPin::PortC && pinNr == 10),
                              "Serial3 Txd can be connected only to: PortB.9, PortB.10, PortC.10.");
            if constexpr (serialType == Rxd)
                static_assert((port == IOPin::PortB && pinNr == 8) || (port == IOPin::PortB && pinNr == 9) || (port == IOPin::PortC && pinNr == 11),
                              "Serial3 Rxd can be connected only to: PortB.8, PortB.11, PortC.11.");
        }
        GPIO::setAlternateFunction(port, pinNr, GPIO::Serial, pull, type);
    }

    template <int spiNumber, SpiPinType spiType, stm32f3xx::IOPin::Port port, stm32f3xx::IOPin::Pin pinNr>
    static void routeSPI(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::PushPull) {
        static_assert(spiNumber != 0, "SPI port numbers starts from 1.");
        static_assert(spiNumber <= 3, "STM32F3xx has only 3 SPI.");
        constexpr IOPin pin(port, pinNr);

        if constexpr (spiNumber == 1) {
            if constexpr (spiType == SCK) {
                static_assert(
                    pin == IOPin{IOPin::PortA, 5} || pin == IOPin{IOPin::PortC, 7} || pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortB, 3},
                    "SPI1 SCK can be connected only to: PortA.5 or PortC.7 or PortA.12 or PortB.3");
            }
            if constexpr (spiType == MOSI) {
                static_assert(
                    pin == IOPin{IOPin::PortA, 6} || pin == IOPin{IOPin::PortC, 8} || pin == IOPin{IOPin::PortA, 13} || pin == IOPin{IOPin::PortB, 5},
                    "SPI1 MISO can be connected only to: PortA.6 or PortC.8 or PortA.13 or PortB.5");
            }
            if constexpr (spiType == MISO) {
                static_assert(pin == IOPin{IOPin::PortA, 7} || pin == IOPin{IOPin::PortB, 0} || pin == IOPin{IOPin::PortB, 4} ||
                                  pin == IOPin{IOPin::PortC, 9} || pin == IOPin{IOPin::PortF, 6},
                              "SPI1 MOSI can be connected only to: PortA.7 or PortB.0 or PortB.4 or PortC.9 or PortF.6");
            }
        }
        // clang-format off
            //assert for SPI2
            static_assert( (spiNumber != 2 || spiType != SCK || ((port == IOPin::PortB && pinNr == 10) || (port == IOPin::PortD && pinNr == 8)|| (port == IOPin::PortA && pinNr == 8)|| (port == IOPin::PortD && pinNr == 7)|| (port == IOPin::PortB && pinNr == 8))), "SPI2 SCK can be connected only to: PortB.10 or PortD.8 or Port PortA.8 or PortD.7");
            static_assert( (spiNumber != 2 || spiType != MISO ||((port == IOPin::PortC && pinNr == 2) || (port == IOPin::PortB && pinNr == 14) || (port == IOPin::PortA && pinNr == 9) || (port == IOPin::PortD && pinNr == 3))), "SPI2 MISO can be connected only to: PortB.14 or PortC.2 or PortA.9 or PortD.3");
            static_assert( (spiNumber != 2 || spiType != MOSI ||((port == IOPin::PortC && pinNr == 3) || (port == IOPin::PortB && pinNr == 15)|| (port == IOPin::PortA && pinNr == 10) || (port == IOPin::PortD && pinNr == 4))), "SPI2 MOSI can be connected only to: PortB.15 or PortC.3 or PortA.10 or PortD.4");
            //assert for SPI3
            static_assert( (spiNumber != 3 || spiType != SCK || ((port == IOPin::PortA && pinNr == 1) || (port == IOPin::PortC && pinNr == 10) || (port == IOPin::PortB && pinNr == 3))), "SPI3 SCK can be connected only to: PortA.1 or PortC.10 or PortB.3");
            static_assert( (spiNumber != 3 || spiType != MISO || ((port == IOPin::PortA && pinNr == 2) || (port == IOPin::PortC && pinNr == 11)|| (port == IOPin::PortB && pinNr == 4))), "SPI3 MISO can be connected only to: PortA.2 or PortC.11 or PortB.4");
            static_assert( (spiNumber != 3 || spiType != MOSI || ((port == IOPin::PortA && pinNr == 3) || (port == IOPin::PortC && pinNr == 12)|| (port == IOPin::PortB && pinNr == 5))), "SPI3 MOSI can be connected only to: PortA.3 or PortC.12 or PortB.5");
        // clang-format on
        if (spiNumber == 3) {
            stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::SPI_3, pull, type);
        } else if (spiNumber == 2) {
            stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::SPI_2, pull, type);
        } else if (spiNumber == 1) {
            if ((pinNr == 11) || (pinNr == 12) || (pinNr == 13)) {
                stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::AF6, pull, type);
            } else {
                stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::AF5, pull, type);
            }
        }

        if (spiType == MISO) {
            switch (spiNumber) {
                case 1:
#if defined(MICROHAL_USE_SPI1_POLLING) || defined(MICROHAL_USE_SPI1_INTERRUPT) || defined(MICROHAL_USE_SPI1_DMA)
                    stm32f3xx::SPI::spi1.misoPin.__setIOPin(pin);
#endif
                    break;
                case 2:
#if defined(MICROHAL_USE_SPI2_POLLING) || defined(MICROHAL_USE_SPI2_INTERRUPT) || defined(MICROHAL_USE_SPI2_DMA)
                    stm32f3xx::SPI::spi2.misoPin.__setIOPin(pin);
#endif
                    break;
                case 3:
#if defined(MICROHAL_USE_SPI3_POLLING) || defined(MICROHAL_USE_SPI3_INTERRUPT) || defined(MICROHAL_USE_SPI3_DMA)
                    stm32f3xx::SPI::spi1.misoPin.__setIOPin(pin);
#endif
                    break;
            }
        }
    }

    template <int i2cNumber, i2cPinType i2cType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeI2C(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::OpenDrain) {
        static_assert(i2cNumber != 0, "I2C port numbers starts from 1.");
        static_assert(i2cNumber <= 3, "STM32F3xx has only 3 I2C.");
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

        stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::I2C, pull, type);
    }

    template <int adcNumber, IOPin::Port port, IOPin::Pin pinNr>
    static void routeADC(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::OpenDrain) {
        static_assert(adcNumber != 0, "ADC numbers starts from 1.");
        static_assert(adcNumber <= 2, "STM32F3xx has only 2 ADC.");

        if constexpr (adcNumber == 1) {
            if constexpr (port == IOPin::PortA) {
                static_assert(pinNr >= 0 && pinNr <= 3, "Incorrect ADC Pin");
            }
        }

        stm32f3xx::GPIO::setAnalogFunction(port, pinNr, pull);
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

        stm32f3xx::GPIO::setAnalogFunction(pin.port, pin.pin);
    }

    template <int TimerNumber, int channel, IOPin::Port port, IOPin::Pin pinNr>
    static void routeTimer(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::OpenDrain) {
        constexpr IOPin pin(port, pinNr);
        GPIO::AlternateFunction alternateFunction = 0;

        if constexpr (TimerNumber == 1) {
            if constexpr (channel == 1) {
                static_assert(pin == IOPin{IOPin::PortA, 8} || pin == IOPin{IOPin::PortC, 0}, "TIM1 channel1 can be connected only to: PortA.8.");
            }
        }

        stm32f3xx::GPIO::setAlternateFunction(port, pinNr, alternateFunction, pull, type);
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

        stm32f3xx::GPIO::setAlternateFunction(port, pinNr, GPIO::AlternateFunction::CAN_TIM1_TIM15);
    }

 private:
};
}  // namespace stm32f3xx
}  // namespace microhal
#endif  // _MICROHAL_IOMANAGER_STM32F3XX_H_
