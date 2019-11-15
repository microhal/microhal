/*
 * IOManager.h
 *
 *  Created on: Nov 13, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STM32F1XX_IOMANAGER_H_
#define SRC_MICROHAL_PORTS_STM32F1XX_IOMANAGER_H_

#include "gpio_stm32f1xx.h"

namespace microhal {

typedef enum {
    Rxd,
    Txd,
} SerialPinType;

typedef enum { MISO, MOSI, SCK } SpiPinType;

namespace stm32f1xx {

class IOManager {
 public:
    IOManager() = delete;

    template <int serial, SerialPinType serialType, stm32f1xx::IOPin::Port port, stm32f1xx::IOPin::Pin pinNr>
    static void routeSerial(stm32f1xx::GPIO::PullType pull = stm32f1xx::GPIO::NoPull, stm32f1xx::GPIO::OutputType type = stm32f1xx::GPIO::PushPull) {
        static_assert(serial < 4, "This MCU have only 3 Serial ports.");
        static_assert(serial != 0, "Serial port numbers starts from 1.");
        constexpr IOPin pin(port, pinNr);

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
                static_assert((port == IOPin::PortB && pinNr == 8) || (port == IOPin::PortB && pinNr == 11) || (port == IOPin::PortC && pinNr == 11),
                              "Serial3 Rxd can be connected only to: PortB.8, PortB.11, PortC.11.");
        }
        if constexpr (serialType == Txd) {
            stm32f1xx::GPIO gpio(pin);
            gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, pull, type);
        }
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
    static void routeSpi(IOPin miso, IOPin mosi, IOPin sck, stm32f1xx::GPIO::OutputType mosiType, stm32f1xx::GPIO::OutputType sckType) {
        (void)miso;
        GPIO gpioMosi(mosi);
        GPIO gpioSck(sck);
        gpioMosi.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::AF5, GPIO::NoPull, mosiType);
        gpioSck.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::AF5, GPIO::NoPull, sckType);
    }

    template <int spiNumber, SpiPinType spiType, stm32f1xx::IOPin::Port port, stm32f1xx::IOPin::Pin pinNr>
    static void routeSPI(stm32f1xx::GPIO::PullType pull = stm32f1xx::GPIO::NoPull, stm32f1xx::GPIO::OutputType type = stm32f1xx::GPIO::PushPull) {
        static_assert(spiNumber != 0, "SPI port numbers starts from 1.");
        static_assert(spiNumber <= 3, "STM32F1xx has only 3 SPI.");
        constexpr IOPin pin(port, pinNr);

        if constexpr (spiNumber == 1) {
            if constexpr (spiType == SCK) {
                static_assert(
                    pin == IOPin{IOPin::PortA, 5} || pin == IOPin{IOPin::PortC, 7} || pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortB, 3},
                    "SPI1 SCK can be connected only to: PortA.5 or PortC.7 or PortA.12 or PortB.3");
            }
            if constexpr (spiType == MISO) {
                static_assert(
                    pin == IOPin{IOPin::PortA, 6} || pin == IOPin{IOPin::PortC, 8} || pin == IOPin{IOPin::PortA, 13} || pin == IOPin{IOPin::PortB, 5},
                    "SPI1 MISO can be connected only to: PortA.6 or PortC.8 or PortA.13 or PortB.5");
            }
            if constexpr (spiType == MOSI) {
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

        if constexpr (spiType == MOSI || spiType == SCK) {
            stm32f1xx::GPIO gpio(pin);
            if (spiNumber == 3) {
                gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::SPI_3, pull, type);
            } else if (spiNumber == 2) {
                gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::SPI_2, pull, type);
            } else if (spiNumber == 1) {
                if ((pinNr == 11) || (pinNr == 12) || (pinNr == 13)) {
                    gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::AF6, pull, type);
                } else {
                    gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::AF5, pull, type);
                }
            }
        }
        if (spiType == MISO) {
            switch (spiNumber) {
                case 1:
#if defined(MICROHAL_USE_SPI1_POLLING) || defined(MICROHAL_USE_SPI1_INTERRUPT) || defined(MICROHAL_USE_SPI1_DMA)
                    // stm32f1xx::SPI::spi1.misoPin.__setIOPin(pin);
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
};
}  // namespace stm32f1xx
}  // namespace microhal
#endif /* SRC_MICROHAL_PORTS_STM32F1XX_IOMANAGER_H_ */
