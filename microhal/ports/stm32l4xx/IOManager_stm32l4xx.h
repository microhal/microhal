/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 11-09-2016
 * last modification: 11-09-2016
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

#ifndef IOMANAGER_STM32L4XX_H_
#define IOMANAGER_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "serialPort.h"
#include "spi.h"

namespace microhal {
typedef enum {
	Rxd, Txd,
} SerialPinType;

typedef enum {
	MISO, MOSI, SCK
} SpiPinType;

typedef enum {
	SCL, SDA
} i2cPinType;

typedef enum {
	Spi1 = 1, Spi2 = 2,
} SpiPorts;

typedef enum {
	OTG_FS_SOF,
	OTG_FS_ID,
	OTG_FS_VBUS,
	OTG_FS_DM,
	OTG_FS_DP
} USBPinType;

namespace stm32l4xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class IOManager {
public:
	IOManager();
	// virtual ~IOManager();

	template<int serial, SerialPinType serialType, stm32l4xx::GPIO::Port port,
			stm32l4xx::GPIO::Pin pinNr>
	static void routeSerial(stm32l4xx::GPIO::PullType pull =
			stm32l4xx::GPIO::NoPull, stm32l4xx::GPIO::OutputType type =
			stm32l4xx::GPIO::PushPull) {
//assert for Serial1
		static_assert( (serial != 1 || serialType != Txd || ((port == GPIO::PortA && pinNr == 9) || (port == GPIO::PortB && pinNr == 6))), "Serial1 Txd can be conected only to: PortA.9 or PortB.6.");
		static_assert( (serial != 1 || serialType != Rxd || ((port == GPIO::PortA && pinNr == 10) || (port == GPIO::PortB && pinNr == 7))), "Serial0 Rxd can be conected only to: PortA.10 or PortB.7.");
//assert for Serial2
		static_assert( (serial != 2 || serialType != Txd || ((port == GPIO::PortA && pinNr == 2) || (port == GPIO::PortD && pinNr == 5))), "Serial2 Txd can be conected only to: PortA.2 or PortD.5.");
		static_assert( (serial != 2 || serialType != Rxd || ((port == GPIO::PortA && pinNr == 3) || (port == GPIO::PortD && pinNr == 6))), "Serial2 Rxd can be conected only to: PortA.3 or PortD.6.");
//assert for Serial3
		static_assert( (serial != 3 || serialType != Txd || ((port == GPIO::PortB && pinNr == 10) || (port == GPIO::PortC && pinNr == 10) || (port == GPIO::PortC && pinNr == 4) || (port == GPIO::PortD && pinNr == 8))), "Serial3 Txd can be conected only to: PortB.10 or PortC.10 or PortD.8.");
		static_assert( (serial != 3 || serialType != Rxd || ((port == GPIO::PortB && pinNr == 11) || (port == GPIO::PortC && pinNr == 11) || (port == GPIO::PortC && pinNr == 5) || (port == GPIO::PortD && pinNr == 9))), "Serial3 Rxd can be conected only to: PortB.11 or PortC.11 or PortD.9.");
//assert for Serial4
		static_assert( (serial != 4 || serialType != Txd || ((port == GPIO::PortC && pinNr == 10) || (port == GPIO::PortA && pinNr == 0))), "Serial4 Txd can be conected only to: PortC.10.");
		static_assert( (serial != 4 || serialType != Rxd || ((port == GPIO::PortC && pinNr == 11) || (port == GPIO::PortA && pinNr == 1))), "Serial4 Rxd can be conected only to: PortC.11.");
//assert for Serial5
		static_assert( (serial != 5 || serialType != Txd || ((port == GPIO::PortC && pinNr == 12))), "Serial5 Txd can be conected only to: PortC.12 or PortC.3.");
		static_assert( (serial != 5 || serialType != Rxd || ((port == GPIO::PortD && pinNr == 2))), "Serial5 Rxd can be conected only to: PortD.2, PortA.3.");
//assert for Serial6
		static_assert( (serial != 6 || serialType != Txd || ((port == GPIO::PortC && pinNr == 1))), "Serial6 Txd can be conected only to: PortC.1 or PortB.11.");
		static_assert( (serial != 6 || serialType != Rxd || ((port == GPIO::PortC && pinNr == 0))), "Serial6 Rxd can be conected only to: PortC.0 or PortB.10.");

//check that pin is use only once

//static_assert(used<port, pinNr>(), "Reuse of pin.");

//todo call gpio route function
		GPIO::setAlternateFunction(port, pinNr, GPIO::Serial, pull, type);
	}

	template<int spiNumber, SpiPinType spiType, stm32l4xx::GPIO::Port port,
			stm32l4xx::GPIO::Pin pinNr>
	static void routeSPI(stm32l4xx::GPIO::PullType pull =
			stm32l4xx::GPIO::NoPull, stm32l4xx::GPIO::OutputType type =
			stm32l4xx::GPIO::PushPull) {
		static_assert (spiNumber != 0, "SPI port numbers starts from 1.");
		static_assert (spiNumber <= 3, "STM32L4xx has only 3 SPI.");

		//assert for SPI1
		static_assert( (spiNumber != 1 || spiType != SCK || ((port == GPIO::PortA && pinNr == 5) || (port == GPIO::PortB && pinNr == 3) || (port == GPIO::PortE && pinNr == 13))), "SPI1 SCK can be connected only to: PortA.5 or PortB.3 or PortE.13.");
		static_assert( (spiNumber != 1 || spiType != MISO || ((port == GPIO::PortA && pinNr == 6) || (port == GPIO::PortB && pinNr == 4) || (port == GPIO::PortE && pinNr == 14))), "SPI1 MISO can be connected only to: PortA.6 or PortB.4 or PortE.14.");
		static_assert( (spiNumber != 1 || spiType != MOSI || ((port == GPIO::PortA && pinNr == 7) || (port == GPIO::PortB && pinNr == 5) || (port == GPIO::PortE && pinNr == 15))), "SPI1 MOSI can be connected only to: PortA.7 or PortB.5 or PortE.15.");
		//assert for SPI2
		static_assert( (spiNumber != 2 || spiType != SCK || ((port == GPIO::PortB && pinNr == 10) || (port == GPIO::PortB && pinNr == 13) || (port == GPIO::PortD && pinNr == 1))), "SPI2 SCK can be connected only to: PortB.10 or PortB.13 or PortD.1.");
		static_assert( (spiNumber != 2 || spiType != MISO ||((port == GPIO::PortB && pinNr == 14) || (port == GPIO::PortC && pinNr == 2) || (port == GPIO::PortD && pinNr == 3))), "SPI2 MISO can be connected only to: PortB.14 or PortC.2 or PortD.3.");
		static_assert( (spiNumber != 2 || spiType != MOSI ||((port == GPIO::PortB && pinNr == 15) || (port == GPIO::PortC && pinNr == 3) || (port == GPIO::PortD && pinNr == 4))), "SPI2 MOSI can be connected only to: PortB.15 or PortC.3 or PortD.4.");
		//assert for SPI3
		static_assert( (spiNumber != 3 || spiType != SCK || ((port == GPIO::PortB && pinNr == 3) || (port == GPIO::PortC && pinNr == 10))), "SPI3 SCK can be connected only to: PortB.3 or PortC.10.");
		static_assert( (spiNumber != 3 || spiType != MISO || ((port == GPIO::PortB && pinNr == 4) || (port == GPIO::PortC && pinNr == 11))), "SPI3 MISO can be connected only to: PortB.4 or PortC.11.");
		static_assert( (spiNumber != 3 || spiType != MOSI || ((port == GPIO::PortB && pinNr == 5) || (port == GPIO::PortC && pinNr == 12))), "SPI3 MOSI can be connected only to: PortB.5 or PortC.12.");
		if(spiNumber == 3){
			stm32l4xx::GPIO::setAlternateFunction(port, pinNr, stm32l4xx::GPIO::SPI_3, pull, type);
		}else{
			stm32l4xx::GPIO::setAlternateFunction(port, pinNr, stm32l4xx::GPIO::SPI, pull, type);
		}

		if(spiType == MISO){
			switch(spiNumber){
			case 1:
#if defined(MICROHAL_USE_SPI1_POLLING) || defined (MICROHAL_USE_SPI1_INTERRUPT) || defined (MICROHAL_USE_SPI1_DMA)
				stm32l4xx::SPI::spi1.misoPort = port;
				stm32l4xx::SPI::spi1.misoPin = pinNr;
#endif
				break;
			case 2:
#if defined(MICROHAL_USE_SPI2_POLLING) || defined (MICROHAL_USE_SPI2_INTERRUPT) || defined (MICROHAL_USE_SPI2_DMA)
				stm32l4xx::SPI::spi2.misoPort = port;
				stm32l4xx::SPI::spi2.misoPin = pinNr;
#endif
				break;
			case 3:
#if defined(MICROHAL_USE_SPI3_POLLING) || defined (MICROHAL_USE_SPI3_INTERRUPT) || defined (MICROHAL_USE_SPI3_DMA)
				stm32l4xx::SPI::spi3.misoPort = port;
				stm32l4xx::SPI::spi3.misoPin = pinNr;
#endif
				break;
			}
		}
	}

private:
};

} // namespace stm32l4xx
} // namespace microhal
#endif  // IOMANAGER_STM32L4XX_H_
