/*
 * portConfigurationAssert_stm32f4xx.cpp
 *
 *  Created on: Oct 28, 2019
 *      Author: pokas
 */

#include "microhalPortConfig_stm32f3xx.h"

#if defined(STM32F334x8)
// ********************* Serial Port 1
// ---- POLLING and INTERRUPT
#if defined(MICROHAL_USE_SERIAL_PORT1_POLLING) || defined(MICROHAL_USE_SERIAL_PORT1_INTERRUPT)
#endif
// ---- INTERRUPT_DMA
#if defined(MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA)
#endif
// ---- DMA
#if defined(MICROHAL_USE_SERIAL_PORT1_DMA)
#ifndef MICROHAL_SERIAL_PORT1_DMA_TX_STREAM
#error MICROHAL_SERIAL_PORT1_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f3xx.h
#elif MICROHAL_SERIAL_PORT1_DMA_TX_STREAM != 4  // Serial port 1 TX stream can be connected only to DMA1 Channel 4.
#error MICROHAL_SERIAL_PORT1_DMA_TX_STREAM have to be equal 4, please check your microhalPortConfig_stm32f3xx.h
#endif
#ifndef MICROHAL_SERIAL_PORT1_DMA_RX_STREAM
#error MICROHAL_SERIAL_PORT1_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f3xx.h
#elif MICROHAL_SERIAL_PORT1_DMA_RX_STREAM != 5  // Serial port 1 RX stream can be connected only to DMA1 Channel 5.
#error MICROHAL_SERIAL_PORT1_DMA_RX_STREAM have to be equal 5, please check your microhalPortConfig_stm32f3xx.h
#endif
#endif
// ********************* Serial Port 2
// ---- POLLING and INTERRUPT
#if defined(MICROHAL_USE_SERIAL_PORT2_POLLING) || defined(MICROHAL_USE_SERIAL_PORT2_INTERRUPT)
#endif
// ---- INTERRUPT_DMA
#if defined(MICROHAL_USE_SERIAL_PORT2_INTERRUPT_DMA)
#endif
// ---- DMA
#if defined(MICROHAL_USE_SERIAL_PORT2_DMA)
#ifndef MICROHAL_SERIAL_PORT2_DMA_TX_STREAM
#error MICROHAL_SERIAL_PORT2_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f3xx.h
#elif MICROHAL_SERIAL_PORT2_DMA_TX_STREAM != 7  // Serial port 2 TX stream can be connected only to DMA1 Channel 7.
#error MICROHAL_SERIAL_PORT2_DMA_TX_STREAM have to be equal 7, please check your microhalPortConfig_stm32f3xx.h
#endif
#ifndef MICROHAL_SERIAL_PORT2_DMA_RX_STREAM
#error MICROHAL_SERIAL_PORT2_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f3xx.h
#elif MICROHAL_SERIAL_PORT2_DMA_RX_STREAM != 6  // Serial port 2 RX stream can be connected only to DMA1 Channel 6.
#error MICROHAL_SERIAL_PORT2_DMA_RX_STREAM have to be equal 6, please check your microhalPortConfig_stm32f3xx.h
#endif
#endif
// ********************* Serial Port 3
// ---- POLLING and INTERRUPT
#if defined(MICROHAL_USE_SERIAL_PORT3_POLLING) || defined(MICROHAL_USE_SERIAL_PORT3_INTERRUPT)
#endif
// ---- INTERRUPT_DMA
#if defined(MICROHAL_USE_SERIAL_PORT3_INTERRUPT_DMA)
#endif
// ---- DMA
#if defined(MICROHAL_USE_SERIAL_PORT3_DMA)
#ifndef MICROHAL_SERIAL_PORT3_DMA_TX_STREAM
#error MICROHAL_SERIAL_PORT3_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f3xx.h
#elif MICROHAL_SERIAL_PORT3_DMA_TX_STREAM != 2  // Serial port 3 TX stream can be connected only to DMA1 Channel 2.
#error MICROHAL_SERIAL_PORT3_DMA_TX_STREAM have to be equal 2, please check your microhalPortConfig_stm32f3xx.h
#endif
#ifndef MICROHAL_SERIAL_PORT3_DMA_RX_STREAM
#error MICROHAL_SERIAL_PORT3_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f3xx.h
#elif MICROHAL_SERIAL_PORT3_DMA_RX_STREAM != 3  // Serial port 3 RX stream can be connected only to DMA1 Channel 3.
#error MICROHAL_SERIAL_PORT3_DMA_RX_STREAM have to be equal 3, please check your microhalPortConfig_stm32f3xx.h
#endif
#endif

#else
#warning Port configuration assertions not available for selected MCU, please double check your settings inside: microhalPortConfig_stm32f4xx.h
#endif
