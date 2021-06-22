/*
 * microhalPortConfig.h
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
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
//#define MICROHAL_USE_SERIAL_PORT1_INTERRUPT			//available settings are MICROHAL_USE_SERIAL_PORTx_POLLING
//						 MICROHAL_USE_SERIAL_PORTx_INTERRUPT
//						 MICROHAL_USE_SERIAL_PORTx_DMA
#define MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE 128

#define MICROHAL_USE_SERIAL_PORT2_INTERRUPT
#define MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE 128

#define MICROHAL_USE_SERIAL_PORT3_INTERRUPT
#define MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE 128

//#define MICROHAL_USE_SERIAL_PORT4_INTERRUPT
#define MICROHAL_SERIAL_PORT4_TX_BUFFER_SIZE 1024
#define MICROHAL_SERIAL_PORT4_RX_BUFFER_SIZE 128
//***********************************************************************************************//
//                                        I2C configurations                                     //
//***********************************************************************************************//
#define MICROHAL_USE_I2C1_INTERRUPT  // available settings are MICROHAL_USE_I2Cx_POLLING
//						 MICROHAL_USE_I2Cx_INTERRUPT
//						 MICROHAL_USE_I2Cx_DMA
#define MICROHAL_I2C1_DMA_RX_STREAM 0  // possible streams are 0 and 5
#define MICROHAL_I2C1_DMA_TX_STREAM 6  // possible streams are 6 and 7

#define MICROHAL_USE_I2C2_POLLING
#define MICROHAL_I2C2_DMA_RX_STREAM 3  // possible streams are 2 and 3
// tx stream can be connected only to stream 7

//#define MICROHAL_USE_I2C3_POLLING
// I2C 3 DMA can be connected only to stream 2 and 4

//***********************************************************************************************//
//                                        SPI configurations                                     //
//***********************************************************************************************//
//#define MICROHAL_USE_SPI1_INTERRUPT  // available settings are MICROHAL_USE_SPIx_POLLING
//						 MICROHAL_USE_SPIx_INTERRUPT
//						 MICROHAL_USE_SPIx_DMA
#define MICROHAL_SPI1_DMA_RX_STREAM 2  // possible streams are 0 and 2 this options are valid only when MICROHAL_USE_SPIx_DMA is defined
#define MICROHAL_SPI1_DMA_TX_STREAM 3  // possible streams are 3 and 5

//#define MICROHAL_USE_SPI2_DMA
// SPI 2 DMA can be connected only to stream 3 and 4

//#define MICROHAL_USE_SPI3_DMA
#define MICROHAL_SPI3_DMA_RX_STREAM 2  // possible streams are 0 and 2
#define MICROHAL_SPI3_DMA_TX_STREAM 5  // possible streams are 5 and 7

//#define MICROHAL_USE_SPI4_DMA
#define MICROHAL_SPI4_DMA_RX_STREAM 0  // possible streams are 0 and 3
#define MICROHAL_SPI4_DMA_TX_STREAM 1  // possible streams are 1 and 4

//#define MICROHAL_USE_SPI5_DMA
#define MICROHAL_SPI5_DMA_RX_STREAM 3  // possible streams are 3 and 5
#define MICROHAL_SPI5_DMA_TX_STREAM 4  // possible streams are 4 and 6

//#define MICROHAL_USE_SPI6_DMA
// SPI 6 DMA can be connected only to stream 5 and 6

#endif /* MICROHALPORTCONFIG_H_ */
