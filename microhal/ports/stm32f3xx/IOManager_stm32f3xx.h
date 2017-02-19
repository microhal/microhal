/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 21-07-2016
 * last modification: 21-07-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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
#include "spi_stm32f3xx.h"
#include "microhalPortConfig_stm32f3xx.h"

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

typedef enum { OTG_FS_SOF, OTG_FS_ID, OTG_FS_VBUS, OTG_FS_DM, OTG_FS_DP } USBPinType;

namespace stm32f3xx {

class IOManager {
 public:
    IOManager() = delete;

    template <int serial, SerialPinType serialType, stm32f3xx::GPIO::Port port, stm32f3xx::GPIO::Pin pinNr>
    static void routeSerial(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::PushPull) {
        // clang-format off

          // assert for Serial1
          static_assert( (serial != 1 || serialType != Txd || ((port == GPIO::PortA && pinNr == 9) || (port == GPIO::PortB && pinNr == 6))), "Serial1 Txd can be conected only to: PortA.9 or PortB.6.");
          static_assert( (serial != 1 || serialType != Rxd || ((port == GPIO::PortA && pinNr == 10) || (port == GPIO::PortB && pinNr == 7))), "Serial0 Rxd can be conected only to: PortA.10 or PortB.7.");
          // assert for Serial2
          static_assert( (serial != 2 || serialType != Txd || ((port == GPIO::PortA && pinNr == 2) || (port == GPIO::PortD && pinNr == 5))), "Serial2 Txd can be conected only to: PortA.2 or PortD.5.");
          static_assert( (serial != 2 || serialType != Rxd || ((port == GPIO::PortA && pinNr == 3) || (port == GPIO::PortD && pinNr == 6))), "Serial2 Rxd can be conected only to: PortA.3 or PortD.6.");
          // assert for Serial3
          static_assert( (serial != 3 || serialType != Txd || ((port == GPIO::PortB && pinNr == 10) || (port == GPIO::PortC && pinNr == 10)|| (port == GPIO::PortD && pinNr == 8))), "Serial3 Txd can be conected only to: PortB.10 or PortC.10 or PortD.8.");
          static_assert( (serial != 3 || serialType != Rxd || ((port == GPIO::PortB && pinNr == 9) || (port == GPIO::PortC && pinNr == 11)|| (port == GPIO::PortD && pinNr == 9))), "Serial3 Rxd can be conected only to: PortB.11 or PortC.11 or PortD.9.");


          GPIO::setAlternateFunction(port, pinNr, GPIO::Serial, pull, type);
      }

    template<int spiNumber, SpiPinType spiType, stm32f3xx::GPIO::Port port,stm32f3xx::GPIO::Pin pinNr>
    static void routeSPI(stm32f3xx::GPIO::PullType pull =stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type = stm32f3xx::GPIO::PushPull)
    {
            static_assert (spiNumber != 0, "SPI port numbers starts from 1.");
            static_assert (spiNumber <= 3, "STM32F3xx has only 3 SPI.");

            //assert for SPI1
            static_assert( (spiNumber != 1 || spiType != SCK || ((port == GPIO::PortA && pinNr == 5) || (port == GPIO::PortC && pinNr == 7) || (port == GPIO::PortA && pinNr == 12 ) || (port == GPIO::PortB && pinNr == 3) )), "SPI1 SCK can be connected only to: PortA.5 or PortC.7 or PortA.12 or PortB.3");
            static_assert( (spiNumber != 1 || spiType != MISO || ((port == GPIO::PortA && pinNr == 6) || (port == GPIO::PortC && pinNr == 8)|| (port == GPIO::PortA && pinNr == 13)|| (port == GPIO::PortB && pinNr == 4))), "SPI1 MISO can be connected only to: PortA.6 or PortC.8 or PortA.13 or PortB.4");
            static_assert( (spiNumber != 1 || spiType != MOSI || ((port == GPIO::PortA && pinNr == 7) || (port == GPIO::PortB && pinNr == 0)|| (port == GPIO::PortC && pinNr == 9)|| (port == GPIO::PortF && pinNr == 6))), "SPI1 MOSI can be connected only to: PortA.7 or PortB.0 or PortC.9 or PortF.6");
            //assert for SPI2
            static_assert( (spiNumber != 2 || spiType != SCK || ((port == GPIO::PortB && pinNr == 10) || (port == GPIO::PortD && pinNr == 8)|| (port == GPIO::PortA && pinNr == 8)|| (port == GPIO::PortD && pinNr == 7)|| (port == GPIO::PortB && pinNr == 8))), "SPI2 SCK can be connected only to: PortB.10 or PortD.8 or Port PortA.8 or PortD.7");
            static_assert( (spiNumber != 2 || spiType != MISO ||((port == GPIO::PortC && pinNr == 2) || (port == GPIO::PortB && pinNr == 14) || (port == GPIO::PortA && pinNr == 9) || (port == GPIO::PortD && pinNr == 3))), "SPI2 MISO can be connected only to: PortB.14 or PortC.2 or PortA.9 or PortD.3");
            static_assert( (spiNumber != 2 || spiType != MOSI ||((port == GPIO::PortC && pinNr == 3) || (port == GPIO::PortB && pinNr == 15)|| (port == GPIO::PortA && pinNr == 10) || (port == GPIO::PortD && pinNr == 4))), "SPI2 MOSI can be connected only to: PortB.15 or PortC.3 or PortA.10 or PortD.4");
            //assert for SPI3
            static_assert( (spiNumber != 3 || spiType != SCK || ((port == GPIO::PortA && pinNr == 1) || (port == GPIO::PortC && pinNr == 10) || (port == GPIO::PortB && pinNr == 3))), "SPI3 SCK can be connected only to: PortA.1 or PortC.10 or PortB.3");
            static_assert( (spiNumber != 3 || spiType != MISO || ((port == GPIO::PortA && pinNr == 2) || (port == GPIO::PortC && pinNr == 11)|| (port == GPIO::PortB && pinNr == 4))), "SPI3 MISO can be connected only to: PortA.2 or PortC.11 or PortB.4");
            static_assert( (spiNumber != 3 || spiType != MOSI || ((port == GPIO::PortA && pinNr == 3) || (port == GPIO::PortC && pinNr == 12)|| (port == GPIO::PortB && pinNr == 5))), "SPI3 MOSI can be connected only to: PortA.3 or PortC.12 or PortB.5");
            if(spiNumber == 3){
                stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::SPI_3, pull, type);
            }else if(spiNumber == 2)
            {
               stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::SPI_2, pull, type);
            }else if(spiNumber == 1)
            {
                if((pinNr = 11)||(pinNr = 12)|| (pinNr = 13))
                {
                    stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::AF6, pull, type);
                }else
                {
                    stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::AF5, pull, type);
                }
            }

            if(spiType == MISO){
                switch(spiNumber){
                case 1:
    #if defined(MICROHAL_USE_SPI1_POLLING) || defined (MICROHAL_USE_SPI1_INTERRUPT) || defined (MICROHAL_USE_SPI1_DMA)
                    stm32f3xx::SPI::spi1.misoPort = port;
                    stm32f3xx::SPI::spi1.misoPin = pinNr;
    #endif
                    break;
                case 2:
    #if defined(MICROHAL_USE_SPI2_POLLING) || defined (MICROHAL_USE_SPI2_INTERRUPT) || defined (MICROHAL_USE_SPI2_DMA)
                    stm32f3xx::SPI::spi2.misoPort = port;
                    stm32f3xx::SPI::spi2.misoPin = pinNr;
    #endif
                    break;
                case 3:
    #if defined(MICROHAL_USE_SPI3_POLLING) || defined (MICROHAL_USE_SPI3_INTERRUPT) || defined (MICROHAL_USE_SPI3_DMA)
                    stm32f3xx::SPI::spi3.misoPort = port;
                    stm32f3xx::SPI::spi3.misoPin = pinNr;
    #endif
                    break;
                }
            }
        }
	template<int i2cNumber, i2cPinType i2cType, GPIO::Port port, GPIO::Pin pinNr>
	static void routeI2C(stm32f3xx::GPIO::PullType pull =
			stm32f3xx::GPIO::NoPull, stm32f3xx::GPIO::OutputType type =
			stm32f3xx::GPIO::OpenDrain) {
		static_assert (i2cNumber != 0, "I2C port numbers starts from 1.");
		static_assert (i2cNumber <= 3, "STM32F4xx has only 3 I2C.");
		//assert for I2C1
		static_assert( (i2cNumber != 1 || i2cType != SDA || ((port == GPIO::PortB && pinNr == 7) || (port == GPIO::PortB && pinNr == 9))), "I2C1 SDA can be connected only to: PortB.7 or PortB.9.");
		static_assert( (i2cNumber != 1 || i2cType != SCL || ((port == GPIO::PortB && pinNr == 6) || (port == GPIO::PortB && pinNr == 8))), "I2C1 SCL can be connected only to: PortB.6 or PortB.8.");
		//assert for I2C2
		static_assert( (i2cNumber != 2 || i2cType != SDA || (port == GPIO::PortB && pinNr == 11)), "I2C2 SDA can be connected only to: PortB.11.");
		static_assert( (i2cNumber != 2 || i2cType != SCL || (port == GPIO::PortB && pinNr == 10)), "I2C2 SCL can be connected only to: PortB.10.");
		//assert for I2C3
		static_assert( (i2cNumber != 3 || i2cType != SDA || (port == GPIO::PortC && pinNr == 9) ), "I2C3 SDA can be connected only to: PortC.9.");
		static_assert( (i2cNumber != 3 || i2cType != SCL || (port == GPIO::PortA && pinNr == 8) ), "I2C3 SCL can be connected only to: PortA.8.");

		stm32f3xx::GPIO::setAlternateFunction(port, pinNr, stm32f3xx::GPIO::I2C, pull, type);
	}

    // clang-format on

 private:
};
}
}
#endif  // _MICROHAL_IOMANAGER_STM32F3XX_H_
