/*
 * IOManager.h
 *
 *  Created on: Nov 13, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STM32F1XX_IOMANAGER_H_
#define SRC_MICROHAL_PORTS_STM32F1XX_IOMANAGER_H_

#include "gpio_stm32f1xx.h"
#include "mcuCapabilities.h"

namespace microhal {

typedef enum {
    Rxd,
    Txd,
} SerialPinType;

typedef enum { MISO, MOSI, SCK } SpiPinType;

typedef enum { DP, DM } USBPinType;

typedef enum { SCL, SDA } i2cPinType;

namespace stm32f1xx {

class IOManager {
 public:
    IOManager() = delete;

    template <int adcNumber, int channel, stm32f1xx::IOPin pin>
    static void routeADC() {
        if constexpr (adcNumber == 1) {
            static_assert(_MICROHAL_STM32F1XX_HAS_ADC1 && adcNumber == 1, "ADC 1 unavailable in this MCU.");
            //            static_assert((pin == IOPin{IOPin::PortA, 11} && pin == IOPin{IOPin::PortA, 12}) ||
            //                              (pin == IOPin{IOPin::PortB, 8} && pin == IOPin{IOPin::PortB, 9}) ||
            //                              (pin == IOPin{IOPin::PortD, 0} && pin == IOPin{IOPin::PortD, 1}),
            //                          "Unable to use selected pin with ADC 1 peripheral.");
        }
        if constexpr (adcNumber == 2) {
            static_assert(_MICROHAL_STM32F1XX_HAS_CAN2 && adcNumber == 2, "CAN 2 unavailable in this MCU.");
        }
        GPIO gpio(pin);
        gpio.configureAsAnalogInput();
    }

    template <int canNumber, stm32f1xx::IOPin rxPin, stm32f1xx::IOPin txPin>
    static void routeCAN() {
        if constexpr (canNumber == 1) {
            static_assert(_MICROHAL_STM32F1XX_HAS_CAN1 && canNumber == 1, "CAN 1 unavailable in this MCU.");
            static_assert((rxPin == IOPin{IOPin::PortA, 11} && txPin == IOPin{IOPin::PortA, 12}) ||
                              (rxPin == IOPin{IOPin::PortB, 8} && txPin == IOPin{IOPin::PortB, 9}) ||
                              (rxPin == IOPin{IOPin::PortD, 0} && txPin == IOPin{IOPin::PortD, 1}),
                          "Unable to use selected pin with CAN1 peripheral.");
        }
        if constexpr (canNumber == 2) {
            static_assert(_MICROHAL_STM32F1XX_HAS_CAN2 && canNumber == 2, "CAN 2 unavailable in this MCU.");
        }
        GPIO txGpio(txPin);
        txGpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::AF5, GPIO::NoPull, stm32f1xx::GPIO::PushPull);
    }

    template <int serial, SerialPinType serialType, stm32f1xx::IOPin pin>
    static void routeSerial(stm32f1xx::GPIO::PullType pull = stm32f1xx::GPIO::NoPull, stm32f1xx::GPIO::OutputType type = stm32f1xx::GPIO::PushPull) {
        static_assert(serial < 6, "This MCU have only 5 Serial ports.");
        static_assert(serial != 0, "Serial port numbers starts from 1.");

        if constexpr (serial == 1) {
            static_assert(_MICROHAL_STM32F1XX_HAS_USART1 && serial == 1, "Serial Port 1 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortB, 6} || pin == IOPin{IOPin::PortC, 4},
                              "Serial1 Txd can be connected only to: PortA.9, PortB.6 or PortC.4.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 7} || pin == IOPin{IOPin::PortC, 5},
                              "Serial1 Rxd can be connected only to: PortA.10, PortB.7 or PortC.5.");
        }
        if constexpr (serial == 2) {
            static_assert(_MICROHAL_STM32F1XX_HAS_USART2 && serial == 2, "Serial Port 2 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortA, 2} || pin == IOPin{IOPin::PortA, 14} || pin == IOPin{IOPin::PortB, 3},
                              "Serial2 Txd can be connected only to: PortA.2, PortA.14 or PortB.3.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortA, 3} || pin == IOPin{IOPin::PortA, 15} || pin == IOPin{IOPin::PortB, 4},
                              "Serial2 Rxd can be connected only to: PortA.3, PortA.15 or PortB.4.");
        }
        if constexpr (serial == 3) {
            static_assert(_MICROHAL_STM32F1XX_HAS_USART3 && serial == 3, "Serial Port 3 unavailable in this MCU.");
            if constexpr (serialType == Txd)
                static_assert(pin == IOPin{IOPin::PortB, 9} || pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortC, 10},
                              "Serial3 Txd can be connected only to: PortB.9, PortB.10, PortC.10.");
            if constexpr (serialType == Rxd)
                static_assert(pin == IOPin{IOPin::PortB, 8} || pin == IOPin{IOPin::PortB, 11} || pin == IOPin{IOPin::PortC, 11},
                              "Serial3 Rxd can be connected only to: PortB.8, PortB.11, PortC.11.");
        }
        if constexpr (serial == 4) {
            static_assert(_MICROHAL_STM32F1XX_HAS_USART4 && serial == 4, "Serial Port 4 unavailable in this MCU.");
            if constexpr (serialType == Txd) static_assert(pin == IOPin{IOPin::PortC, 10}, "Serial4 Txd can be connected only to: PortC.10.");
            if constexpr (serialType == Rxd) static_assert(pin == IOPin{IOPin::PortC, 11}, "Serial4 Rxd can be connected only to: PortC.11.");
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
                    // stm32f3xx::SPI::spi2.misoPin.__setIOPin(pin);
#endif
                    break;
                case 3:
#if defined(MICROHAL_USE_SPI3_POLLING) || defined(MICROHAL_USE_SPI3_INTERRUPT) || defined(MICROHAL_USE_SPI3_DMA)
                    //  stm32f3xx::SPI::spi1.misoPin.__setIOPin(pin);
#endif
                    break;
            }
        }
    }

    template <int TimerNumber, int channel, IOPin::Port port, IOPin::Pin pinNr>
    static void routeTimer(stm32f1xx::GPIO::PullType pull = stm32f1xx::GPIO::NoPull,
                           stm32f1xx::GPIO::OutputType type = stm32f1xx::GPIO::OutputType::PushPull) {
        constexpr IOPin pin(port, pinNr);

        GPIO::AlternateFunction alternateFunction = GPIO::AlternateFunction::AF2;

        stm32f1xx::GPIO gpio(pin);
        gpio.setAlternateFunctionOutput(alternateFunction, pull, type);
    }

    template <stm32f1xx::IOPin mcoPin>
    static void routeMCO() {
        static_assert(mcoPin == IOPin{IOPin::PortA, 8}, "USB1 DP can be connected only to: PortA.12.");

        stm32f1xx::GPIO mcoGpio(mcoPin);
        mcoGpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                           stm32f1xx::GPIO::OutputType::PushPull);
    }

    template <stm32f1xx::IOPin dmPin, stm32f1xx::IOPin dpPin>
    static void routeUSB() {
        static_assert(dmPin == IOPin{IOPin::PortA, 11}, "USB1 DM can be connected only to: PortA.11.");
        static_assert(dpPin == IOPin{IOPin::PortA, 12}, "USB1 DP can be connected only to: PortA.12.");

        stm32f1xx::GPIO dmGpio(dmPin);
        dmGpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull, stm32f1xx::GPIO::OutputType::PushPull);
        stm32f1xx::GPIO dpGpio(dpPin);
        dpGpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull, stm32f1xx::GPIO::OutputType::PushPull);
    }

    template <stm32f1xx::IOPin rxd0, stm32f1xx::IOPin rxd1, stm32f1xx::IOPin rxd2, stm32f1xx::IOPin rxd3, stm32f1xx::IOPin rxClk,
              stm32f1xx::IOPin rxDv, stm32f1xx::IOPin rxEr, stm32f1xx::IOPin txd0, stm32f1xx::IOPin txd1, stm32f1xx::IOPin txd2,
              stm32f1xx::IOPin txd3, stm32f1xx::IOPin txClk, stm32f1xx::IOPin txEn, stm32f1xx::IOPin crs, stm32f1xx::IOPin mdio, stm32f1xx::IOPin mdc,
              stm32f1xx::IOPin col>
    static void routeMII() {
        stm32f1xx::GPIO mdcGpio(mdc);
        mdcGpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                           stm32f1xx::GPIO::OutputType::PushPull);

        stm32f1xx::GPIO txd2Gpio(txd2);
        txd2Gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                            stm32f1xx::GPIO::OutputType::PushPull);

        stm32f1xx::GPIO mdioGpio(mdio);
        mdioGpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                            stm32f1xx::GPIO::OutputType::PushPull);

        stm32f1xx::GPIO txEnGpio(txEn);
        txEnGpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                            stm32f1xx::GPIO::OutputType::PushPull);

        stm32f1xx::GPIO txd0Gpio(txd0);
        txd0Gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                            stm32f1xx::GPIO::OutputType::PushPull);

        stm32f1xx::GPIO txd1Gpio(txd1);
        txd1Gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                            stm32f1xx::GPIO::OutputType::PushPull);

        stm32f1xx::GPIO txd3Gpio(txd3);
        txd3Gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, stm32f1xx::GPIO::NoPull,
                                            stm32f1xx::GPIO::OutputType::PushPull);
    }

    template <int i2cNumber, i2cPinType i2cType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeI2C(stm32f1xx::GPIO::PullType pull = stm32f1xx::GPIO::NoPull, stm32f1xx::GPIO::OutputType type = stm32f1xx::GPIO::OpenDrain) {
        constexpr IOPin pin(port, pinNr);

        static_assert(i2cNumber != 0, "I2C port numbers starts from 1.");
        static_assert(i2cNumber <= 2, "STM32F1xx has only 1 I2C.");
        // clang-format off
		//assert for I2C1
        if constexpr (i2cNumber == 1) {
            if constexpr (i2cType == SDA) static_assert(pin == IOPin{IOPin::PortB, 7}, "I2C1 SDA can be connected only to: PortB.7.");
            if constexpr (i2cType == SCL) static_assert(pin == IOPin{IOPin::PortB, 6}, "I2C1 SCL can be connected only to: PortB.6.");
        }
        //assert for I2C1
        if constexpr (i2cNumber == 2) {
            if constexpr (i2cType == SDA) static_assert(pin == IOPin{IOPin::PortB, 11}, "I2C1 SDA can be connected only to: PortB.11.");
            if constexpr (i2cType == SCL) static_assert(pin == IOPin{IOPin::PortB, 10}, "I2C1 SCL can be connected only to: PortB.10.");
        }
        // clang-format on
        // stm32f0xx::GPIO gpio(pin);
        // gpio.setAlternateFunction(stm32f0xx::GPIO::AlternateFunction::I2C, pull, type);
        stm32f1xx::GPIO gpio(pin);
        gpio.setAlternateFunctionOutput(stm32f1xx::GPIO::AlternateFunction::Serial, pull, type);
    }
};
}  // namespace stm32f1xx
}  // namespace microhal
#endif /* SRC_MICROHAL_PORTS_STM32F1XX_IOMANAGER_H_ */
