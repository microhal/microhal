/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      board support package for stm32f4Discovery board

 @authors    Pawel Okas
 created on: 16-04-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 	 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 	 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 	 	in the documentation and/or other materials provided with the distribution.
 	 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 	 	from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *//* ========================================================================================================================== */

#ifndef MICROHALPORTCONFIG_H_
#define MICROHALPORTCONFIG_H_

#include <cstdint>
//***********************************************************************************************//
//                                     Hardware Configuration                                    //
//***********************************************************************************************//
static constexpr bool externalClockPresent = true;
static constexpr uint32_t externalClockFrequency = 8000000; // this is HSE frequency
static constexpr bool externalLSEPresent = false;
static constexpr uint32_t externalLSEFrequency = 32768;
//***********************************************************************************************//
//                                       configure interfaces                                    //
//***********************************************************************************************//
//***********************************************************************************************//
//                                    Serial Port configurations                                 //
//***********************************************************************************************//
#define MICROHAL_USE_SERIAL_PORT1_DMA 		//available settings are MICROHAL_USE_SERIAL_PORTx_POLLING
													//						 MICROHAL_USE_SERIAL_PORTx_INTERRUPT
													//						 MICROHAL_USE_SERIAL_PORTx_DMA
#define MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE 128
// serial port 1 TX stream can be connected only to DMA2 Stream 7
#define MICROHAL_SERIAL_PORT1_DMA_RX_STREAM 5		// serial port 1 RX stream can be connected to DMA2 Stream 2 or 5

//#define MICROHAL_USE_SERIAL_PORT2_DMA
//#define MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE 1024
//#define MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE 128
// serial port 2 TX stream can be connected only to DMA1 Stream 6.
// serial port 2 RX stream can be connected only to DMA1 Stream 5

//#if !defined(STM32F401xC) && !defined(STM32F401xE) && !defined(STM32F411xE) && !defined(STM32F412Cx) && !defined(STM32F410Cx) && !defined(STM32F410Rx) && !defined(STM32F410Tx)
//#define MICROHAL_USE_SERIAL_PORT3_DMA
//#define MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE 1024
//#define MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE 128
//#define MICROHAL_SERIAL_PORT3_DMA_TX_STREAM 3 		// Serial port 3 Tx stream can be connected to DMA1 Stream 3 or 4.
//// Serial port 3 RX stream can be connected only to DMA1 Stream 1.
//#endif
//
//#if !defined(STM32F401xC) && !defined(STM32F401xE) && !defined(STM32F411xE) && !defined(STM32F412Cx) && !defined(STM32F412Zx) && !defined(STM32F412Vx) && !defined(STM32F412Rx) && !defined(STM32F410Cx) && !defined(STM32F410Rx) && !defined(STM32F410Tx)
//#define MICROHAL_USE_SERIAL_PORT4_DMA
//#define MICROHAL_SERIAL_PORT4_TX_BUFFER_SIZE 1024
//#define MICROHAL_SERIAL_PORT4_RX_BUFFER_SIZE 128
//// serial port 4 TX stream can be connected only to DMA1 Stream 4.
//// serial port 4 RX stream can be connected only to DMA1 Stream 2
//#endif
//
//#if !defined(STM32F401xC) && !defined(STM32F401xE) && !defined(STM32F411xE) && !defined(STM32F412Cx) && !defined(STM32F412Zx) && !defined(STM32F412Vx) && !defined(STM32F412Rx) && !defined(STM32F410Cx) && !defined(STM32F410Rx) && !defined(STM32F410Tx)
//#define MICROHAL_USE_SERIAL_PORT5_DMA
//#define MICROHAL_SERIAL_PORT5_TX_BUFFER_SIZE 1024
//#define MICROHAL_SERIAL_PORT5_RX_BUFFER_SIZE 128
//#endif
//
//#if !defined(STM32F410Tx)
//#define MICROHAL_USE_SERIAL_PORT6_DMA
//#define MICROHAL_SERIAL_PORT6_TX_BUFFER_SIZE 1024
//#define MICROHAL_SERIAL_PORT6_RX_BUFFER_SIZE 128
//#define MICROHAL_SERIAL_PORT6_DMA_RX_STREAM 1 // serial port 6 RX stream can be connected to DMA2 Stream 1 or 2
//#define MICROHAL_SERIAL_PORT6_DMA_TX_STREAM 7 // serial port 6 TX stream can be connected to DMA2 Stream 6 or 7
//#endif
//***********************************************************************************************//
//                                        I2C configurations                                     //
//***********************************************************************************************//
#define MICROHAL_USE_I2C1_DMA		//available settings are MICROHAL_USE_I2Cx_POLLING
										//						 MICROHAL_USE_I2Cx_INTERRUPT
										//						 MICROHAL_USE_I2Cx_DMA
#define MICROHAL_I2C1_DMA_RX_STREAM 0	//possible streams are 0 and 5
#define MICROHAL_I2C1_DMA_TX_STREAM 7	//possible streams are 6 and 7

//#define MICROHAL_USE_I2C2_DMA
//#define MICROHAL_I2C2_DMA_RX_STREAM 3	//possible streams are 2 and 3
//tx stream can be connected only to stream 7
//#if !defined(STM32F410Cx) && !defined(STM32F410Rx) && !defined(STM32F410Tx)
//#define MICROHAL_USE_I2C3_DMA
////I2C 3 DMA can be connected only to stream 2 and 4
//#endif
//***********************************************************************************************//
//                                        SPI configurations                                     //
//***********************************************************************************************//
#define MICROHAL_USE_SPI1_DMA			//available settings are MICROHAL_USE_SPIx_POLLING
										//						 MICROHAL_USE_SPIx_INTERRUPT
										//						 MICROHAL_USE_SPIx_DMA
#define MICROHAL_SPI1_DMA_RX_STREAM 2	//possible streams are 0 and 2 this options are valid only when MICROHAL_USE_SPIx_DMA is defined
#define MICROHAL_SPI1_DMA_TX_STREAM 3   //possible streams are 3 and 5

//#if !defined(STM32F410Tx)
//#define MICROHAL_USE_SPI2_DMA
////SPI 2 DMA can be connected only to stream 3 and 4
//#endif
//
//#if !defined(STM32F410Tx) && !defined(STM32F410Cx)
//#define MICROHAL_USE_SPI3_DMA
//#define MICROHAL_SPI3_DMA_RX_STREAM 0	//possible streams are 0 and 2
//#define MICROHAL_SPI3_DMA_TX_STREAM 7	//possible streams are 5 and 7
//#endif
//
//#if !defined(STM32F405xx) && !defined(STM32F407xx) && !defined(STM32F410Tx) && !defined(STM32F415xx) && !defined(STM32F417xx)
//#define MICROHAL_USE_SPI4_DMA
//#define MICROHAL_SPI4_DMA_RX_STREAM 0	//possible streams are 0 and 3
//#define MICROHAL_SPI4_DMA_TX_STREAM 1	//possible streams are 1 and 4
//#endif
//
//#if !defined(STM32F401xC) && !defined(STM32F401xE) && !defined(STM32F405xx) && !defined(STM32F407xx) && !defined(STM32F410Cx) && !defined(STM32F410Tx) && !defined(STM32F415xx) && !defined(STM32F417xx) && !defined(STM32F446xx)
//#define MICROHAL_USE_SPI5_DMA
//#define MICROHAL_SPI5_DMA_RX_STREAM 3	//possible streams are 3 and 5
//#define MICROHAL_SPI5_DMA_TX_STREAM 4 //possible streams are 4 and 6
//#endif
//
//#if !defined(STM32F401xC) && !defined(STM32F401xE) && !defined(STM32F405xx) && !defined(STM32F407xx) && !defined(STM32F410Cx) && !defined(STM32F410Tx) && !defined(STM32F411xE) && !defined(STM32F412Cx) && !defined(STM32F412Rx) && !defined(STM32F412Zx) && !defined(STM32F412Vx) && !defined(STM32F415xx) && !defined(STM32F417xx) && !defined(STM32F446xx)
//#define MICROHAL_USE_SPI6_DMA
////SPI 6 DMA can be connected only to stream 5 and 6
//#endif

#endif /* MICROHALPORTCONFIG_H_ */