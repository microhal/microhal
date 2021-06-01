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
#include "mcuCapabilities.h"

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
    static void routeTimer(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
                           stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::PushPull) {
        constexpr IOPin pin(port, pinNr);
        GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f4xx::GPIO::AlternateFunction::AF1, stm32f4xx::GPIO::PullType::NoPull, stm32f4xx::GPIO::OutputType::PushPull);
    }

    template <int serialNumber, SerialPinType serialPinType, stm32f4xx::IOPin pin>
    static void routeSerial(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
                            stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::PushPull) {
        static_assert(serialNumber != 0, "Serial Port numbers starts from 1.");
        static_assert(serialNumber <= 6, "STM32F4XX family have up to 6 Serial Ports.");
        auto af = stm32f4xx::GPIO::AlternateFunction::AF0;

        if constexpr (serialNumber == 1) {
            static_assert(_MICROHAL_STM32F4XX_HAS_USART1 && serialNumber == 1, "Serial Port 1 unavailable in this MCU.");
            if constexpr (serialPinType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 3} || pin == IOPin{IOPin::PortB, 7},
                              "Serial Port 1 Rxd can be connected only to: PortA.10, PortB.3, PortB.7");
                af = stm32f4xx::GPIO::AlternateFunction::AF7;
            }
            if constexpr (serialPinType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 9} || pin == IOPin{IOPin::PortA, 15} || pin == IOPin{IOPin::PortB, 6},
                              "Serial Port 1 Txd can be connected only to: PortA.9, PortA.15, PortB.6");
                af = stm32f4xx::GPIO::AlternateFunction::AF7;
            }
        }
        if constexpr (serialNumber == 2) {
            static_assert(_MICROHAL_STM32F4XX_HAS_USART2 && serialNumber == 2, "Serial Port 2 unavailable in this MCU.");
            if constexpr (serialPinType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 3} || pin == IOPin{IOPin::PortD, 6},
                              "Serial Port 2 Rxd can be connected only to: PortA.3, PortD.6");
                af = stm32f4xx::GPIO::AlternateFunction::AF7;
            }
            if constexpr (serialPinType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 2} || pin == IOPin{IOPin::PortD, 5},
                              "Serial Port 2 Txd can be connected only to: PortA.2, PortD.5");
                af = stm32f4xx::GPIO::AlternateFunction::AF7;
            }
        }
        if constexpr (serialNumber == 3) {
            static_assert(_MICROHAL_STM32F4XX_HAS_USART3 && serialNumber == 3, "Serial Port 3 unavailable in this MCU.");
            if constexpr (serialPinType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortC, 7},
                              "Serial Port 3 Rxd can be connected only to: PortA.12, PortC.7");
                af = stm32f4xx::GPIO::AlternateFunction::AF8;
            }
            if constexpr (serialPinType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 11} || pin == IOPin{IOPin::PortC, 6},
                              "Serial Port 3 Txd can be connected only to: PortA.11, PortC.6");
                af = stm32f4xx::GPIO::AlternateFunction::AF8;
            }
        }
        if constexpr (serialNumber == 6) {
            static_assert(_MICROHAL_STM32F4XX_HAS_USART6 && serialNumber == 6, "Serial Port 6 unavailable in this MCU.");
            if constexpr (serialPinType == Rxd) {
                static_assert(pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortC, 7},
                              "Serial Port 3 Rxd can be connected only to: PortA.12, PortC.7");
                af = stm32f4xx::GPIO::AlternateFunction::AF8;
            }
            if constexpr (serialPinType == Txd) {
                static_assert(pin == IOPin{IOPin::PortA, 11} || pin == IOPin{IOPin::PortC, 6},
                              "Serial Port 3 Txd can be connected only to: PortA.11, PortC.6");
                af = stm32f4xx::GPIO::AlternateFunction::AF8;
            }
        }

        stm32f4xx::GPIO gpio(pin);
        gpio.setAlternateFunction(af, pull, type);
    }

    static constexpr bool spiPinAssert(int number, IOPin miso, IOPin mosi, IOPin sck) {
        //        if (number == 1) {
        //            if (!(sck == IOPin{IOPin::PortA, 5} || sck == IOPin{IOPin::PortB, 3})) return false;
        //            if (!(miso == IOPin{IOPin::PortA, 6} || miso == IOPin{IOPin::PortB, 4})) return false;
        //            if (!(mosi == IOPin{IOPin::PortA, 7} || mosi == IOPin{IOPin::PortB, 5})) return false;
        //        } else if (number == 2) {
        //            if (!(sck == IOPin{IOPin::PortB, 10} || sck == IOPin{IOPin::PortB, 13})) return false;
        //            if (!(miso == IOPin{IOPin::PortB, 14} || miso == IOPin{IOPin::PortC, 2})) return false;
        //            if (!(mosi == IOPin{IOPin::PortB, 15} || mosi == IOPin{IOPin::PortC, 3})) return false;
        //        } else if (number == 3) {
        //            if (!(sck == IOPin{IOPin::PortB, 3} || sck == IOPin{IOPin::PortC, 10})) return false;
        //            if (!(miso == IOPin{IOPin::PortB, 4} || miso == IOPin{IOPin::PortC, 11})) return false;
        //            if (!(mosi == IOPin{IOPin::PortB, 5} || mosi == IOPin{IOPin::PortC, 12})) return false;
        //        } else {
        //            return false;
        //        }
        return true;
    }

    template <int spiNumber, SpiPinType spiPinType, stm32f4xx::IOPin pin>
    static void routeSPI(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
                         stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::PushPull) {
        static_assert(spiNumber != 0, "SPI numbers starts from 1.");
        static_assert(spiNumber <= 6, "STM32F4XX family have up to 6 SPIs.");
        auto af = stm32f4xx::GPIO::AlternateFunction::AF0;

        if constexpr (spiNumber == 1) {
            static_assert(_MICROHAL_STM32F4XX_HAS_SPI1 && spiNumber == 1, "SPI 1 unavailable in this MCU.");
            if constexpr (spiPinType == MOSI) {
                static_assert(pin == IOPin{IOPin::PortA, 7} || pin == IOPin{IOPin::PortB, 5},
                              "SPI 1 MOSI can be connected only to: PortA.7, PortB.5");
                af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
            if constexpr (spiPinType == MISO) {
                static_assert(pin == IOPin{IOPin::PortA, 6} || pin == IOPin{IOPin::PortB, 4},
                              "SPI 1 MISO can be connected only to: PortA.6, PortB.4");
                af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
            if constexpr (spiPinType == SCK) {
                static_assert(pin == IOPin{IOPin::PortA, 5} || pin == IOPin{IOPin::PortB, 3}, "SPI 1 SCK can be connected only to: PortA.5, PortB.3");
                af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
        }
        if constexpr (spiNumber == 2) {
            static_assert(_MICROHAL_STM32F4XX_HAS_SPI2 && spiNumber == 2, "SPI 2 unavailable in this MCU.");
            if constexpr (spiPinType == MOSI) {
                static_assert(pin == IOPin{IOPin::PortB, 15} || pin == IOPin{IOPin::PortC, 3},
                              "SPI 2 MOSI can be connected only to: PortB.15, PortC.3");
                af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
            if constexpr (spiPinType == MISO) {
                static_assert(pin == IOPin{IOPin::PortB, 14} || pin == IOPin{IOPin::PortC, 2},
                              "SPI 2 MISO can be connected only to: PortB.14, PortC.2");
                af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
            if constexpr (spiPinType == SCK) {
                static_assert(pin == IOPin{IOPin::PortB, 10} || pin == IOPin{IOPin::PortB, 13} || pin == IOPin{IOPin::PortC, 7} ||
                                  pin == IOPin{IOPin::PortD, 3},
                              "SPI 2 SCK can be connected only to: PortB.10, PortB.13, PortC.7, PortD.3");
                af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
        }
        if constexpr (spiNumber == 3) {
            static_assert(_MICROHAL_STM32F4XX_HAS_SPI3 && spiNumber == 3, "SPI 3 unavailable in this MCU.");
            if constexpr (spiPinType == MOSI) {
                static_assert(pin == IOPin{IOPin::PortB, 5} || pin == IOPin{IOPin::PortC, 12} || pin == IOPin{IOPin::PortD, 6},
                              "SPI 3 MOSI can be connected only to: PortB.5, PortC.12, PortD.6");
                if constexpr (pin == IOPin{IOPin::PortD, 6})
                    af = stm32f4xx::GPIO::AlternateFunction::AF5;
                else
                    af = stm32f4xx::GPIO::AlternateFunction::AF6;
            }
            if constexpr (spiPinType == MISO) {
                static_assert(pin == IOPin{IOPin::PortB, 4} || pin == IOPin{IOPin::PortC, 11},
                              "SPI 3 MISO can be connected only to: PortB.4, PortC.11");
                af = stm32f4xx::GPIO::AlternateFunction::AF6;
            }
            if constexpr (spiPinType == SCK) {
                static_assert(pin == IOPin{IOPin::PortB, 3} || pin == IOPin{IOPin::PortB, 12} || pin == IOPin{IOPin::PortC, 10},
                              "SPI 3 SCK can be connected only to: PortB.3, PortB.12, PortC.10");
                af = stm32f4xx::GPIO::AlternateFunction::AF6;
            }
        }
        if constexpr (spiNumber == 4) {
            static_assert(_MICROHAL_STM32F4XX_HAS_SPI4 && spiNumber == 4, "SPI 4 unavailable in this MCU.");
            if constexpr (spiPinType == MOSI) {
                static_assert(pin == IOPin{IOPin::PortA, 1} || pin == IOPin{IOPin::PortE, 6} || pin == IOPin{IOPin::PortE, 14},
                              "SPI 4 MOSI can be connected only to: PortA.1, PortE.6, PortE.14");
                af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
            if constexpr (spiPinType == MISO) {
                static_assert(pin == IOPin{IOPin::PortA, 11} || pin == IOPin{IOPin::PortE, 5} || pin == IOPin{IOPin::PortE, 13},
                              "SPI 4 MISO can be connected only to: PortA.11, PortE.5, PortE.13");
                if constexpr (pin == IOPin{IOPin::PortA, 11})
                    af = stm32f4xx::GPIO::AlternateFunction::AF6;
                else
                    af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
            if constexpr (spiPinType == SCK) {
                static_assert(pin == IOPin{IOPin::PortB, 13} || pin == IOPin{IOPin::PortE, 2} || pin == IOPin{IOPin::PortE, 12},
                              "SPI 4 SCK can be connected only to: PortB.13, PortE.2, PortE.12");
                if constexpr (pin == IOPin{IOPin::PortB, 13})
                    af = stm32f4xx::GPIO::AlternateFunction::AF6;
                else
                    af = stm32f4xx::GPIO::AlternateFunction::AF5;
            }
        }
        if constexpr (spiNumber == 5) {
            static_assert(_MICROHAL_STM32F4XX_HAS_SPI5 && spiNumber == 5, "SPI 5 unavailable in this MCU.");
            if constexpr (spiPinType == MOSI) {
                static_assert(pin == IOPin{IOPin::PortA, 10} || pin == IOPin{IOPin::PortB, 8} || pin == IOPin{IOPin::PortE, 6} ||
                                  pin == IOPin{IOPin::PortE, 14},
                              "SPI 5 MOSI can be connected only to: PortA.10, PortB.8, PortE.6, PortE.14");
                af = stm32f4xx::GPIO::AlternateFunction::AF6;
            }
            if constexpr (spiPinType == MISO) {
                static_assert(pin == IOPin{IOPin::PortA, 12} || pin == IOPin{IOPin::PortE, 5} || pin == IOPin{IOPin::PortE, 13},
                              "SPI 5 MISO can be connected only to: PortA.12, PortE.5, PortE.13");
                af = stm32f4xx::GPIO::AlternateFunction::AF6;
            }
            if constexpr (spiPinType == SCK) {
                static_assert(pin == IOPin{IOPin::PortB, 0} || pin == IOPin{IOPin::PortE, 2} || pin == IOPin{IOPin::PortE, 12},
                              "SPI 5 SCK can be connected only to: PortB.0, PortE.2, PortE.12");
                af = stm32f4xx::GPIO::AlternateFunction::AF6;
            }
        }

        stm32f4xx::GPIO gpio(pin);
        gpio.setAlternateFunction(af, pull, type);
    }

    template <int i2cNumber, i2cPinType i2cType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeI2C(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
                         stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::OpenDrain) {
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
        gpio.setAlternateFunction(stm32f4xx::GPIO::AlternateFunction::AF4, pull, type);
    }

    template <USBPinType usbPinType, IOPin::Port port, IOPin::Pin pinNr>
    static void routeUSB(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
                         stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::PushPull) {
        constexpr IOPin pin(port, pinNr);
        static_assert((usbPinType != OTG_FS_SOF || (port == IOPin::PortA && pinNr == 8)), "USB Fast Speed SOF pin can be connected only to PortA.8");
        static_assert((usbPinType != OTG_FS_VBUS || (port == IOPin::PortA && pinNr == 9)),
                      "USB Fast Speed VBus pin can be connected only to PortA.9");
        static_assert((usbPinType != OTG_FS_ID || (port == IOPin::PortA && pinNr == 10)), "USB Fast Speed ID pin can be connected only to PortA.10");
        static_assert((usbPinType != OTG_FS_DM || (port == IOPin::PortA && pinNr == 11)), "USB Fast Speed DM pin can be connected only to PortA.11");
        static_assert((usbPinType != OTG_FS_DP || (port == IOPin::PortA && pinNr == 12)), "USB Fast Speed DP pin can be connected only to PortA.12");

        if (usbPinType == OTG_FS_ID) {
            pull = stm32f4xx::GPIO::PullType::PullUp;
            type = GPIO::OutputType::OpenDrain;
        }

        GPIO gpio(pin);
        gpio.setAlternateFunction(stm32f4xx::GPIO::AlternateFunction::AF10, pull, type, GPIO::Speed::HighSpeed);
    }

    template <int dacNumber, IOPin::Port port, IOPin::Pin pinNr>
    static void routeDAC(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
                         stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::OpenDrain) {
        constexpr IOPin pin(port, pinNr);
        static_assert((dacNumber != 1 || (port == IOPin::PortA && pinNr == 4)), "DAC 1 can be connected only to: PortA.4.");
        static_assert((dacNumber != 2 || (port == IOPin::PortA && pinNr == 5)), "DAC 2 can be connected only to: PortA.5.");

        GPIO gpio(pin);
        gpio.setAnalogFunction();
    }

    template <int adcNumber, IOPin::Port port, IOPin::Pin pinNr>
    static void routeADC(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
                         stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::OpenDrain) {
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
        gpio.setAlternateFunction(GPIO::AlternateFunction::AF9);
    }
};  // namespace stm32f4xx

}  // namespace stm32f4xx
}  // namespace microhal
#endif  // IOMANAGER_STM32F4XX_H_
