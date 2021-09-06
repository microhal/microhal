/**
 * @file
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      stm32g0xx port configuration file
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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

#ifndef MICROHALPORTCONFIG_H_
#define MICROHALPORTCONFIG_H_

#include <cstdint>
//***********************************************************************************************//
//                                       MCU Specification                                       //
//***********************************************************************************************//
#define MICROHAL_STM32G071RB
//***********************************************************************************************//
//                                     Hardware Configuration                                    //
//***********************************************************************************************//
static constexpr bool externalClockPresent = false;
static constexpr uint32_t externalClockFrequency = 8000000;  // this is HSE frequency
static constexpr bool externalLSEPresent = false;
static constexpr uint32_t externalLSEFrequency = 32768;
//***********************************************************************************************//
//                                       configure interfaces                                    //
//***********************************************************************************************//
//***********************************************************************************************//
//                                    Serial Port configurations                                 //
//***********************************************************************************************//
#define MICROHAL_USE_SERIAL_PORT1_INTERRUPT 0  // available settings are MICROHAL_USE_SERIAL_PORTx_POLLING
//						 MICROHAL_USE_SERIAL_PORTx_INTERRUPT
//						 MICROHAL_USE_SERIAL_PORTx_DMA
#define MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE 128
#define MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE 128

#define MICROHAL_USE_SERIAL_PORT2_INTERRUPT 1
#define MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE 16

#define MICROHAL_USE_SERIAL_PORT3_INTERRUPT 0
#define MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE 256
#define MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE 256
//***********************************************************************************************//
//                                        I2C configurations                                     //
//***********************************************************************************************//
//#define MICROHAL_USE_I2C1_POLLING		//available settings are MICROHAL_USE_I2Cx_POLLING
//						 MICROHAL_USE_I2Cx_INTERRUPT
//						 MICROHAL_USE_I2Cx_DMA
#define MICROHAL_I2C1_DMA_RX_STREAM 0  // possible streams are 0 and 5
#define MICROHAL_I2C1_DMA_TX_STREAM 6  // possible streams are 6 and 7

//#define MICROHAL_USE_I2C2_DMA
#define MICROHAL_I2C2_DMA_RX_STREAM 3  // possible streams are 2 and 3
// tx stream can be connected only to stream 7

//#define MICROHAL_USE_I2C3_POLLING
// I2C 3 DMA can be connected only to stream 2 and 4

//***********************************************************************************************//
//                                        SPI configurations                                     //
//***********************************************************************************************//
#define MICROHAL_USE_SPI1_INTERRUPT 1  // available settings are MICROHAL_USE_SPIx_POLLING
//						 MICROHAL_USE_SPIx_INTERRUPT
//						 MICROHAL_USE_SPIx_DMA
#define MICROHAL_SPI1_DMA_RX_STREAM 2  // possible streams are 0 and 2 this options are valid only when MICROHAL_USE_SPIx_DMA is defined
#define MICROHAL_SPI1_DMA_TX_STREAM 5  // possible streams are 3 and 5

//#define MICROHAL_USE_SPI2_POLLING
// SPI 2 DMA can be connected only to stream 3 and 4

//#define MICROHAL_USE_SPI3_POLLING
#define MICROHAL_SPI3_DMA_RX_STREAM 0  // possible streams are 0 and 2
#define MICROHAL_SPI3_DMA_TX_STREAM 7  // possible streams are 5 and 7

//#define MICROHAL_USE_SPI4_DMA
#define MICROHAL_SPI4_DMA_RX_STREAM 0  // possible streams are 0 and 3
#define MICROHAL_SPI4_DMA_TX_STREAM 1  // possible streams are 1 and 4

//#define MICROHAL_USE_SPI5_DMA
#define MICROHAL_SPI5_DMA_RX_STREAM 3  // possible streams are 3 and 5
#define MICROHAL_SPI5_DMA_TX_STREAM 4  // possible streams are 4 and 6

//#define MICROHAL_USE_SPI6_DMA
// SPI 6 DMA can be connected only to stream 5 and 6
//***********************************************************************************************//
//                                        LIN configurations                                     //
//***********************************************************************************************//
#define MICROHAL_USE_LIN1 0

#define MICROHAL_USE_TIMER_SIGNAL 0

#endif /* MICROHALPORTCONFIG_H_ */
