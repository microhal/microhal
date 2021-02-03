/*
 * IOManager_STM32F4xx.h
 *
 *  Created on: 1 kwi 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_IOMANAGER_STM32F4XX_H_
#define _MICROHAL_IOMANAGER_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <algorithm>
#include <array>
#include "serialPort.h"
//#include "spi_stm32f4xx.h"

namespace microhal {
typedef enum {
    Rxd,
    Txd,
} SerialPinType;

typedef enum { MISO, MOSI, SCK } SpiPinType;

typedef enum { SCL, SDA } i2cPinType;

typedef enum { RX, TX } CanPinType;

typedef enum {
    Spi1 = 1,
    Spi2 = 2,
} SpiPorts;

typedef enum { OTG_FS_SOF, OTG_FS_ID, OTG_FS_VBUS, OTG_FS_DM, OTG_FS_DP } USBPinType;

namespace stm32f4xx {

template <auto T>
constexpr static bool contain(const std::array<IOPin, T> &list, const IOPin &pin) {
    return std::find(list.begin(), list.end(), pin) != list.end();
}

class IOManager {
 public:
    IOManager();
    template <int serial, IOPin::Port port, IOPin::Pin pinNr>
    static void routeTimer(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::PushPull) {
        constexpr IOPin pin(port, pinNr);
        GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f4xx::GPIO::AlternateFunction::Timer_1_2, stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::PushPull);
    }

    template <int serial, SerialPinType serialType, stm32f4xx::IOPin::Port port, stm32f4xx::IOPin::Pin pinNr>
    static void routeSerial(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::PushPull) {
        constexpr IOPin pin(port, pinNr);

        // assert for Serial1
        if constexpr (serial == 1) {
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortA, 15} || pin == IOPin{IOPin::PortB, 6},
                              "Serial1 Txd can be connected only to: PortA.9 or PortB.6.");
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 3} || pin == IOPin{IOPin::PortB, 7},
                              "Serial1 Rxd can be connected only to: PortA.10 or PortB.7.");
            }
        }
        // assert for Serial2
        if constexpr (serial == 2) {
#if !defined(STM32F410Tx) && !defined(STM32F410Cx) && !defined(STM32F410Rx)
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 2} || pin == IOPin{IOPin::PortD, 5},
                              "Serial2 Txd can be connected only to: PortA.2 or PortD.5.");
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 3} || pin == IOPin{IOPin::PortD, 6},
                              "Serial2 Rxd can be connected only to: PortA.3 or PortD.6.");
            }
#else
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 2}, "Serial2 Txd can be connected only to: PortA.2.");
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 3}, "Serial2 Rxd can be connected only to: PortA.3.");
            }
#endif
        }
// assert for Serial3
#if !defined(STM32F410Tx) && !defined(STM32F410Cx) && !defined(STM32F410Rx)
        if constexpr (serial == 3) {
            if constexpr (serialType == Txd) {
                static_assert(pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortC, 10} || pin == IOPin{IOPin::PortD, 8},
                              "Serial3 Txd can be connected only to: PortB.10 or PortC.10 or PortD.8.");
            }
            if constexpr (serialType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortB, 11} || pin == IOPin{IOPin::PortC, 11} || pin == IOPin{IOPin::PortD, 9},
                              "Serial3 Rxd can be connected only to: PortB.11 or PortC.11 or PortD.9.");
            }
#else
        if constexpr (serialType == Txd) {
            static_assert(pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortC, 10},
                          "Serial3 Txd can be connected only to: PortB.10 or PortC.10.");
        }
        if constexpr (serialType == Rxd) {
            static_assert(pin == IOPin{IOPin::PortB, 11} || pin == IOPin{IOPin::PortC, 11},
                          "Serial3 Rxd can be connected only to: PortB.11 or PortC.11.");
        }
#endif
        }
        // assert for Serial4
        static_assert((serial != 4 || serialType != Txd || (port == IOPin::PortA && pinNr == 0) || (port == IOPin::PortC && pinNr == 10)),
                      "Serial4 Txd can be connected to: PortA.0 or PortC.10.");
        static_assert((serial != 4 || serialType != Rxd || (port == IOPin::PortA && pinNr == 1) || (port == IOPin::PortC && pinNr == 11)),
                      "Serial4 Rxd can be connected to: PortA.1 or PortC.11.");
        // assert for Serial5
        static_assert((serial != 5 || serialType != Txd || ((port == IOPin::PortC && pinNr == 12) || (port == IOPin::PortC && pinNr == 3))),
                      "Serial5 Txd can be connected only to: PortC.12 or PortC.3.");
#if !defined(STM32F410Tx) && !defined(STM32F410Cx) && !defined(STM32F410Rx)
        static_assert((serial != 5 || serialType != Rxd || ((port == IOPin::PortD && pinNr == 2) || (port == IOPin::PortA && pinNr == 3))),
                      "Serial5 Rxd can be connected only to: PortD.2, PortA.3.");
#else
    static_assert((serial != 5 || serialType != Rxd || ((port == IOPin::PortA && pinNr == 2))), "Serial5 Rxd can be connected only to: PortA.3.");
#endif
        // assert for Serial6
        if constexpr (serial == 6) {
            static_assert((serialType != Txd || ((port == IOPin::PortC && pinNr == 6) || (port == IOPin::PortA && pinNr == 11))),
                          "Serial6 Txd can be connected to: PortA.11 or PortC.6.");
            static_assert((serialType != Rxd || ((port == IOPin::PortC && pinNr == 7) || (port == IOPin::PortA && pinNr == 12))),
                          "Serial6 Rxd can be connected to: PortA.12 or PortC.7.");
        }
// assert for Serial7
#if defined(GPIOE) && defined(GPIOF)
        static_assert((serial != 7 || serialType != Txd || ((port == IOPin::PortF && pinNr == 7) || (port == IOPin::PortE && pinNr == 8))),
                      "Serial7 Txd can be connected to: PortE.8, PortF.7.");
        static_assert((serial != 7 || serialType != Rxd || ((port == IOPin::PortF && pinNr == 6) || (port == IOPin::PortE && pinNr == 7))),
                      "Serial7 Rxd can be connected to: PortE.7, PortF.6.");
#endif
// assert for Serial8
#ifdef GPIOE
        static_assert((serial != 8 || serialType != Txd || ((port == IOPin::PortE && pinNr == 1))), "Serial8 Txd can be connected only to: PortE.1.");
        static_assert((serial != 8 || serialType != Rxd || ((port == IOPin::PortE && pinNr == 0))), "Serial8 Rxd can be connected only to: PortE.0.");
#endif

        GPIO gpio(pin);
        gpio.setAlternateFunction(serial < 4 ? GPIO::AlternateFunction::Serial : GPIO::AlternateFunction::Serial_4_5_6, pull, type);
    }

    template <int number>
    static void routeSpi(IOPin miso, IOPin mosi, IOPin sck, stm32f4xx::GPIO::OutputType mosiType, stm32f4xx::GPIO::OutputType sckType) {
        static_assert(number != 0, "SPI port numbers starts from 1.");
        static_assert(number <= 3, "STM32F4xx has only 3 SPI.");
        GPIO gpioMiso(miso);
        GPIO gpioMosi(mosi);
        GPIO gpioSck(sck);
        gpioMiso.setAlternateFunction(number == 3 ? GPIO::AlternateFunction::SPI_3 : GPIO::AlternateFunction::SPI, GPIO::NoPull, GPIO::PushPull);
        gpioMosi.setAlternateFunction(number == 3 ? GPIO::AlternateFunction::SPI_3 : GPIO::AlternateFunction::SPI, GPIO::NoPull, mosiType);
        gpioSck.setAlternateFunction(number == 3 ? GPIO::AlternateFunction::SPI_3 : GPIO::AlternateFunction::SPI, GPIO::NoPull, sckType);
    }

    static constexpr bool spiPinAssert(int number, IOPin miso, IOPin mosi, IOPin sck) {
        if (number == 1) {
            if (!(sck == IOPin{IOPin::PortA, 5} || sck == IOPin{IOPin::PortB, 3})) return false;
            if (!(miso == IOPin{IOPin::PortA, 6} || miso == IOPin{IOPin::PortB, 4})) return false;
            if (!(mosi == IOPin{IOPin::PortA, 7} || mosi == IOPin{IOPin::PortB, 5})) return false;
        } else if (number == 2) {
            if (!(sck == IOPin{IOPin::PortB, 10} || sck == IOPin{IOPin::PortB, 13})) return false;
            if (!(miso == IOPin{IOPin::PortB, 14} || miso == IOPin{IOPin::PortC, 2})) return false;
            if (!(mosi == IOPin{IOPin::PortB, 15} || mosi == IOPin{IOPin::PortC, 3})) return false;
        } else if (number == 3) {
            if (!(sck == IOPin{IOPin::PortB, 3} || sck == IOPin{IOPin::PortC, 10})) return false;
            if (!(miso == IOPin{IOPin::PortB, 4} || miso == IOPin{IOPin::PortC, 11})) return false;
            if (!(mosi == IOPin{IOPin::PortB, 5} || mosi == IOPin{IOPin::PortC, 12})) return false;
        } else {
            return false;
        }
        return true;
    }

    template <int i2cNumber, i2cPinType i2cType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeI2C(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OpenDrain) {
        static_assert(i2cNumber != 0, "I2C port numbers starts from 1.");
        static_assert(i2cNumber <= 3, "STM32F4xx has only 3 I2C.");

        constexpr IOPin pin(port, pinNr);
        // assert for I2C1
        static_assert((i2cNumber != 1 || i2cType != SDA || ((port == IOPin::PortB && pinNr == 7) || (port == IOPin::PortB && pinNr == 9))),
                      "I2C1 SDA can be connected only to: PortB.7 or PortB.9.");
        static_assert((i2cNumber != 1 || i2cType != SCL || ((port == IOPin::PortB && pinNr == 6) || (port == IOPin::PortB && pinNr == 8))),
                      "I2C1 SCL can be connected only to: PortB.6 or PortB.8.");
        // assert for I2C2
        static_assert((i2cNumber != 2 || i2cType != SDA || (port == IOPin::PortB && pinNr == 11)), "I2C2 SDA can be connected only to: PortB.11.");
        static_assert((i2cNumber != 2 || i2cType != SCL || (port == IOPin::PortB && pinNr == 10)), "I2C2 SCL can be connected only to: PortB.10.");
        // assert for I2C3
        static_assert((i2cNumber != 3 || i2cType != SDA || (port == IOPin::PortC && pinNr == 9)), "I2C3 SDA can be connected only to: PortC.9.");
        static_assert((i2cNumber != 3 || i2cType != SCL || (port == IOPin::PortA && pinNr == 8)), "I2C3 SCL can be connected only to: PortA.8.");

        GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f4xx::GPIO::AlternateFunction::I2C, pull, type);
    }

    template <USBPinType usbPinType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeUSB(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::PushPull) {
        constexpr IOPin pin(port, pinNr);
        static_assert((usbPinType != OTG_FS_SOF || (port == IOPin::PortA && pinNr == 8)), "USB Fast Speed SOF pin can be connected only to PortA.8");
        static_assert((usbPinType != OTG_FS_VBUS || (port == IOPin::PortA && pinNr == 9)),
                      "USB Fast Speed VBus pin can be connected only to PortA.9");
        static_assert((usbPinType != OTG_FS_ID || (port == IOPin::PortA && pinNr == 10)), "USB Fast Speed ID pin can be connected only to PortA.10");
        static_assert((usbPinType != OTG_FS_DM || (port == IOPin::PortA && pinNr == 11)), "USB Fast Speed DM pin can be connected only to PortA.11");
        static_assert((usbPinType != OTG_FS_DP || (port == IOPin::PortA && pinNr == 12)), "USB Fast Speed DP pin can be connected only to PortA.12");

        if (usbPinType == OTG_FS_ID) {
            pull = stm32f4xx::GPIO::PullUp;
            type = GPIO::OpenDrain;
        }

        GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f4xx::GPIO::AlternateFunction::USB, pull, type, GPIO::Speed::HighSpeed);
    }

    template <int dacNumber, IOPin::Port port, IOPin::Pin pinNr>
    static void routeDAC(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OpenDrain) {
        constexpr IOPin pin(port, pinNr);
        static_assert((dacNumber != 1 || (port == IOPin::PortA && pinNr == 4)), "DAC 1 can be connected only to: PortA.4.");
        static_assert((dacNumber != 2 || (port == IOPin::PortA && pinNr == 5)), "DAC 2 can be connected only to: PortA.5.");

        GPIO gpio(pin);
        gpio.setAnalogFunction();
    }

    template <int adcNumber, IOPin::Port port, IOPin::Pin pinNr>
    static void routeADC(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OpenDrain) {
        constexpr IOPin pin(port, pinNr);
        GPIO gpio(pin);
        gpio.setAnalogFunction();
    }

    template <int canNumber, CanPinType canPinType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeCAN() {
        constexpr IOPin pin(port, pinNr);

        if constexpr (canNumber == 1) {
            if constexpr (canPinType == RX) {
                if constexpr (IOPin::isPortIPresent() == true) {
                    // constexpr std::array<IOPin, 4> supportedPins = {IOPin{IOPin::PortA, 11}, IOPin{IOPin::PortB, 8}, IOPin{IOPin::PortD, 0},
                    //                                                 IOPin{IOPin::PortI, 9}};
                    // static_assert(std::find(supportedPins.begin(), supportedPins.end(), pin) != supportedPins.end(),
                    //               "CAN1 RX can be connected only to: PortA.11 or PortB.8.");
                } else {
                    constexpr std::array<IOPin, 3> supportedPins = {IOPin{IOPin::PortA, 11}, IOPin{IOPin::PortB, 8}, IOPin{IOPin::PortD, 0}};
                    static_assert(std::find(supportedPins.begin(), supportedPins.end(), pin) != supportedPins.end(),
                                  "CAN1 RX can be connected only to: PortA.11 or PortB.8.");
                }
            }
            if constexpr (canPinType == TX) {
                static_assert(pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortB, 9} || pin == IOPin{IOPin::PortD, 1} ||
                                  pin == IOPin{IOPin::PortH, 13},
                              "CAN1 TX can be connected only to: PortA.12 or PortB.9.");
            }
        }

        if constexpr (canNumber == 2) {
            if constexpr (canPinType == RX) {
                static_assert(pin == IOPin{IOPin::PortB, 5} || pin == IOPin{IOPin::PortB, 12},
                              "CAN2 RX can be connected only to: PortA.11 or PortB.8.");
            }
            if constexpr (canPinType == TX) {
                static_assert(pin == IOPin{IOPin::PortB, 6} || pin == IOPin{IOPin::PortB, 13},
                              "CAN2 TX can be connected only to: PortA.12 or PortB.9.");
            }
        }
        GPIO gpio(pin);
        gpio.setAlternateFunction(GPIO::AlternateFunction::CAN1_2_TIM12_13_14);
    }
};  // namespace stm32f4xx

}  // namespace stm32f4xx
}  // namespace microhal
#endif  // IOMANAGER_STM32F4XX_H_
