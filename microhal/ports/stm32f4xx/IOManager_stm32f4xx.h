/*
 * IOManager_STM32F4xx.h
 *
 *  Created on: 1 kwi 2014
 *      Author: pawel
 */

#ifndef IOMANAGER_STM32F4XX_H_
#define IOMANAGER_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "serialPort.h"
#include "spi_stm32f4xx.h"

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

typedef enum { OTG_FS_SOF, OTG_FS_ID, OTG_FS_VBUS, OTG_FS_DM, OTG_FS_DP } USBPinType;

namespace stm32f4xx {

class IOManager {
 public:
    IOManager();
    template <int serial, stm32f4xx::GPIO::Port port, stm32f4xx::GPIO::Pin pinNr>
    static void routeTimer(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::PushPull) {
        stm32f4xx::GPIO::setAlternateFunction(stm32f4xx::IOPin::PortA, 2, stm32f4xx::GPIO::Timer_1_2, stm32f4xx::GPIO::NoPull,
                                              stm32f4xx::GPIO::PushPull);
    }

    template <int serial, SerialPinType serialType, stm32f4xx::IOPin::Port port, stm32f4xx::IOPin::Pin pinNr>
    static void routeSerial(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::PushPull) {
        // assert for Serial1
        static_assert((serial != 1 || serialType != Txd || ((port == IOPin::PortA && pinNr == 9) || (port == IOPin::PortB && pinNr == 6))),
                      "Serial1 Txd can be connected only to: PortA.9 or PortB.6.");
        static_assert((serial != 1 || serialType != Rxd || ((port == IOPin::PortA && pinNr == 10) || (port == IOPin::PortB && pinNr == 7))),
                      "Serial1 Rxd can be connected only to: PortA.10 or PortB.7.");
// assert for Serial2
#if !defined(STM32F410Tx) && !defined(STM32F410Cx) && !defined(STM32F410Rx)
        static_assert((serial != 2 || serialType != Txd || ((port == IOPin::PortA && pinNr == 2) || (port == IOPin::PortD && pinNr == 5))),
                      "Serial2 Txd can be connected only to: PortA.2 or PortD.5.");
        static_assert((serial != 2 || serialType != Rxd || ((port == IOPin::PortA && pinNr == 3) || (port == IOPin::PortD && pinNr == 6))),
                      "Serial2 Rxd can be connected only to: PortA.3 or PortD.6.");
#else
        static_assert((serial != 2 || serialType != Txd || ((port == GPIO::PortA && pinNr == 2))), "Serial2 Txd can be connected only to: PortA.2.");
        static_assert((serial != 2 || serialType != Rxd || ((port == GPIO::PortA && pinNr == 3))), "Serial2 Rxd can be connected only to: PortA.3.");
#endif
// assert for Serial3
#if !defined(STM32F410Tx) && !defined(STM32F410Cx) && !defined(STM32F410Rx)
        static_assert((serial != 3 || serialType != Txd ||
                       ((port == IOPin::PortB && pinNr == 10) || (port == IOPin::PortC && pinNr == 10) || (port == IOPin::PortD && pinNr == 8))),
                      "Serial3 Txd can be connected only to: PortB.10 or PortC.10 or PortD.8.");
        static_assert((serial != 3 || serialType != Rxd ||
                       ((port == IOPin::PortB && pinNr == 11) || (port == IOPin::PortC && pinNr == 11) || (port == IOPin::PortD && pinNr == 9))),
                      "Serial3 Rxd can be connected only to: PortB.11 or PortC.11 or PortD.9.");
#else
        static_assert((serial != 3 || serialType != Txd || ((port == IOPin::PortB && pinNr == 10) || (port == IOPin::PortC && pinNr == 10))),
                      "Serial3 Txd can be connected only to: PortB.10 or PortC.10.");
        static_assert((serial != 3 || serialType != Rxd || ((port == IOPin::PortB && pinNr == 11) || (port == IOPin::PortC && pinNr == 11))),
                      "Serial3 Rxd can be connected only to: PortB.11 or PortC.11.");
#endif
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
        // check that pin is use only once

        // static_assert(used<port, pinNr>(), "Reuse of pin.");

        // todo call gpio route function

        GPIO::setAlternateFunction(port, pinNr, serial < 4 ? GPIO::Serial : GPIO::Serial_4_5_6, pull, type);
    }

    template <int spiNumber, SpiPinType spiType, stm32f4xx::GPIO::Port port, stm32f4xx::GPIO::Pin pinNr>
    static void routeSPI(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::PushPull) {
        static_assert(spiNumber != 0, "SPI port numbers starts from 1.");
        static_assert(spiNumber <= 3, "STM32F4xx has only 3 SPI.");

        // assert for SPI1
        static_assert((spiNumber != 1 || spiType != SCK || ((port == IOPin::PortA && pinNr == 5) || (port == IOPin::PortB && pinNr == 3))),
                      "SPI1 SCK can be connected only to: PortA.5 or PortB.3.");
        static_assert((spiNumber != 1 || spiType != MISO || ((port == IOPin::PortA && pinNr == 6) || (port == IOPin::PortB && pinNr == 4))),
                      "SPI1 MISO can be connected only to: PortA.6 or PortB.4.");
        static_assert((spiNumber != 1 || spiType != MOSI || ((port == IOPin::PortA && pinNr == 7) || (port == IOPin::PortB && pinNr == 5))),
                      "SPI1 MOSI can be connected only to: PortA.7 or PortB.5.");
        // assert for SPI2
        static_assert((spiNumber != 2 || spiType != SCK || ((port == IOPin::PortB && pinNr == 10) || (port == IOPin::PortB && pinNr == 13))),
                      "SPI2 SCK can be connected only to: PortB.10 or PortB.13.");
        static_assert((spiNumber != 2 || spiType != MISO || ((port == IOPin::PortB && pinNr == 14) || (port == IOPin::PortC && pinNr == 2))),
                      "SPI2 MISO can be connected only to: PortB.14 or PortC.2.");
        static_assert((spiNumber != 2 || spiType != MOSI || ((port == IOPin::PortB && pinNr == 15) || (port == IOPin::PortC && pinNr == 3))),
                      "SPI2 MOSI can be connected only to: PortB.15 or PortC.3.");
        // assert for SPI3
        static_assert((spiNumber != 3 || spiType != SCK || ((port == IOPin::PortB && pinNr == 3) || (port == IOPin::PortC && pinNr == 10))),
                      "SPI3 SCK can be connected only to: PortB.3 or PortC.10.");
        static_assert((spiNumber != 3 || spiType != MISO || ((port == IOPin::PortB && pinNr == 4) || (port == IOPin::PortC && pinNr == 11))),
                      "SPI3 MISO can be connected only to: PortB.4 or PortC.11.");
        static_assert((spiNumber != 3 || spiType != MOSI || ((port == IOPin::PortB && pinNr == 5) || (port == IOPin::PortC && pinNr == 12))),
                      "SPI3 MOSI can be connected only to: PortB.5 or PortC.12.");
        if (spiNumber == 3) {
            stm32f4xx::GPIO::setAlternateFunction(port, pinNr, stm32f4xx::GPIO::SPI_3, pull, type);
        } else {
            stm32f4xx::GPIO::setAlternateFunction(port, pinNr, stm32f4xx::GPIO::SPI, pull, type);
        }

        if (spiType == MISO) {
            switch (spiNumber) {
                case 1:
#if defined(MICROHAL_USE_SPI1_POLLING) || defined(MICROHAL_USE_SPI1_INTERRUPT) || defined(MICROHAL_USE_SPI1_DMA)
                    stm32f4xx::SPI::spi1.misoPin = {port, pinNr};
#endif
                    break;
                case 2:
#if defined(MICROHAL_USE_SPI2_POLLING) || defined(MICROHAL_USE_SPI2_INTERRUPT) || defined(MICROHAL_USE_SPI2_DMA)
                    stm32f4xx::SPI::spi2.misoPin = {port, pinNr};
#endif
                    break;
                case 3:
#if defined(MICROHAL_USE_SPI3_POLLING) || defined(MICROHAL_USE_SPI3_INTERRUPT) || defined(MICROHAL_USE_SPI3_DMA)
                    stm32f4xx::SPI::spi3.misoPin = {port, pinNr};
#endif
                    break;
            }
        }
    }

    template <int i2cNumber, i2cPinType i2cType, GPIO::Port port, GPIO::Pin pinNr>
    static void routeI2C(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OpenDrain) {
        static_assert(i2cNumber != 0, "I2C port numbers starts from 1.");
        static_assert(i2cNumber <= 3, "STM32F4xx has only 3 I2C.");
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

        stm32f4xx::GPIO::setAlternateFunction(port, pinNr, stm32f4xx::GPIO::I2C, pull, type);
    }

    template <USBPinType usbPinType, stm32f4xx::GPIO::Port port, stm32f4xx::GPIO::Pin pinNr>
    static void routeUSB(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::PushPull) {
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

        stm32f4xx::GPIO::setAlternateFunction(port, pinNr, stm32f4xx::GPIO::USB, pull, type, GPIO::Speed::HighSpeed);
    }

    template <int dacNumber, microhal::stm32f4xx::GPIO::Port port, microhal::stm32f4xx::GPIO::Pin pinNr>
    static void routeDAC(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OpenDrain) {
        static_assert((dacNumber != 1 || (port == IOPin::PortA && pinNr == 4)), "DAC 1 can be connected only to: PortA.4.");
        static_assert((dacNumber != 2 || (port == IOPin::PortA && pinNr == 5)), "DAC 2 can be connected only to: PortA.5.");

        stm32f4xx::GPIO::setAnalogFunction(port, pinNr);
    }

    template <int adcNumber, microhal::stm32f4xx::GPIO::Port port, microhal::stm32f4xx::GPIO::Pin pinNr>
    static void routeADC(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OpenDrain) {
        stm32f4xx::GPIO::setAnalogFunction(port, pinNr);
    }
};

}  // namespace stm32f4xx
}  // namespace microhal
#endif  // IOMANAGER_STM32F4XX_H_
