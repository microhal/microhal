/* ========================================================================================================================== */ /**
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

 */ /* ==========================================================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       */

#ifndef MICROHALPORTCONFIG_H_
#define MICROHALPORTCONFIG_H_

#include <cstdint>
//***********************************************************************************************//
//                                     Hardware Configuration                                    //
//***********************************************************************************************//
static constexpr bool externalClockPresent = true;
static constexpr uint32_t externalClockFrequency = 8000000;  // this is HSE frequency
static constexpr bool externalLSEPresent = false;
static constexpr uint32_t externalLSEFrequency = 32768;
//***********************************************************************************************//
//                                       configure interfaces                                    //
//***********************************************************************************************//
//***********************************************************************************************//
//                                    Serial Port configurations                                 //
//***********************************************************************************************//
// clang-format off
#define MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE 1024
// serial port 1 TX stream can be connected only to DMA2 Stream 7
#define MICROHAL_SERIAL_PORT1_DMA_RX_STREAM 5		// serial port 1 RX stream can be connected to DMA2 Stream 2 or 5

#define MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE 1024
// serial port 2 TX stream can be connected only to DMA1 Stream 6.
// serial port 2 RX stream can be connected only to DMA1 Stream 5

#define MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT3_DMA_TX_STREAM 3 		// Serial port 3 Tx stream can be connected to DMA1 Stream 3 or 4.
// Serial port 3 RX stream can be connected only to DMA1 Stream 1.

#define MICROHAL_SERIAL_PORT4_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT4_RX_BUFFER_SIZE 1024
// serial port 4 TX stream can be connected only to DMA1 Stream 4.
// serial port 4 RX stream can be connected only to DMA1 Stream 2

#define MICROHAL_SERIAL_PORT5_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT5_RX_BUFFER_SIZE 1024
// serial port 5 TX stream can be connected only to DMA1 Stream 0.
// serial port 5 RX stream can be connected only to DMA1 Stream 7.

#define MICROHAL_SERIAL_PORT6_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT6_RX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT6_DMA_TX_STREAM 6 		// Serial port 6 Tx stream can be connected to DMA2 Stream 6 or 7.
#define MICROHAL_SERIAL_PORT6_DMA_RX_STREAM 2 		// Serial port 6 Rx stream can be connected to DMA2 Stream 1 or 2.

#define MICROHAL_SERIAL_PORT7_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT7_RX_BUFFER_SIZE 1024
// serial port 7 TX stream can be connected only to DMA1 Stream 3.
// serial port 7 RX stream can be connected only to DMA1 Stream 1.

#define MICROHAL_SERIAL_PORT8_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT8_RX_BUFFER_SIZE 1024
// serial port 8 TX stream can be connected only to DMA1 Stream 0.
// serial port 8 RX stream can be connected only to DMA1 Stream 6.
// clang-format on

#endif /* MICROHALPORTCONFIG_H_ */
