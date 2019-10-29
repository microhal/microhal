/*
 * portConfigurationAssert_stm32f407xx.cpp
 *
 *  Created on: Oct 29, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_STM32F4XX_PORTCONFIGURATIONASSERT_STM32F407XX_H_
#define _MICROHAL_STM32F4XX_PORTCONFIGURATIONASSERT_STM32F407XX_H_

// ********************* Serial Port 1
// ---- POLLING and INTERRUPT
#if defined(MICROHAL_USE_SERIAL_PORT1_POLLING) || defined(MICROHAL_USE_SERIAL_PORT1_INTERRUPT)
#endif
// ---- INTERRUPT_DMA
#if defined(MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA)
#endif
// ---- DMA
#if defined(MICROHAL_USE_SERIAL_PORT1_DMA)
// clang-format off
    #ifndef MICROHAL_SERIAL_PORT1_DMA_TX_STREAM
        #error MICROHAL_SERIAL_PORT1_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT1_DMA_TX_STREAM != 7  // Serial port 1 TX stream can be connected only to DMA2 Channel 7.
        #error MICROHAL_SERIAL_PORT1_DMA_TX_STREAM have to be equal 7, please check your microhalPortConfig_stm32f4xx.h
    #endif
    #ifndef MICROHAL_SERIAL_PORT1_DMA_RX_STREAM
        #error MICROHAL_SERIAL_PORT1_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT1_DMA_RX_STREAM != 2 && MICROHAL_SERIAL_PORT1_DMA_RX_STREAM != 5  // Serial port 1 RX stream can be connected to DMA2 Channel 2 or DMA2 Channel 5.
        #error MICROHAL_SERIAL_PORT1_DMA_TX_STREAM have to be equal 2 or 5, please check your microhalPortConfig_stm32f4xx.h
    #endif
// clang-format on
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
// clang-format off
    #ifndef MICROHAL_SERIAL_PORT2_DMA_TX_STREAM
        #error MICROHAL_SERIAL_PORT2_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT2_DMA_TX_STREAM != 6  // Serial port 2 TX stream can be connected only to DMA1 Channel 6.
        #error MICROHAL_SERIAL_PORT2_DMA_TX_STREAM have to be equal 6, please check your microhalPortConfig_stm32f4xx.h
    #endif
    #ifndef MICROHAL_SERIAL_PORT2_DMA_RX_STREAM
        #error MICROHAL_SERIAL_PORT2_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT2_DMA_RX_STREAM != 5  // Serial port 2 RX stream can be connected to DMA1 Channel 5.
        #error MICROHAL_SERIAL_PORT2_DMA_TX_STREAM have to be equal 5 or 7, please check your microhalPortConfig_stm32f4xx.h
    #endif
// clang-format on
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
// clang-format off
    #ifndef MICROHAL_SERIAL_PORT3_DMA_TX_STREAM
        #error MICROHAL_SERIAL_PORT3_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT3_DMA_TX_STREAM != 3 && MICROHAL_SERIAL_PORT3_DMA_TX_STREAM != 4 // Serial port 3 TX stream can be connected only to DMA1 Channel 3 or DMA1 Channel 4.
        #error MICROHAL_SERIAL_PORT3_DMA_TX_STREAM have to be equal 3 or 4, please check your microhalPortConfig_stm32f4xx.h
    #endif
    #ifndef MICROHAL_SERIAL_PORT3_DMA_RX_STREAM
        #error MICROHAL_SERIAL_PORT3_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT3_DMA_RX_STREAM != 1 // Serial port 3 RX stream can be connected to DMA1 Channel 1.
        #error MICROHAL_SERIAL_PORT3_DMA_TX_STREAM have to be equal 1, please check your microhalPortConfig_stm32f4xx.h
    #endif
// clang-format on
#endif
// ********************* Serial Port 4
// ---- POLLING and INTERRUPT
#if defined(MICROHAL_USE_SERIAL_PORT4_POLLING) || defined(MICROHAL_USE_SERIAL_PORT4_INTERRUPT)
#endif
// ---- INTERRUPT_DMA
#if defined(MICROHAL_USE_SERIAL_PORT4_INTERRUPT_DMA)
#endif
// ---- DMA
#if defined(MICROHAL_USE_SERIAL_PORT4_DMA)
// clang-format off
    #ifndef MICROHAL_SERIAL_PORT4_DMA_TX_STREAM
        #error MICROHAL_SERIAL_PORT4_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT4_DMA_TX_STREAM != 4 // Serial port 4 TX stream can be connected only to DMA1 Channel 4.
        #error MICROHAL_SERIAL_PORT4_DMA_TX_STREAM have to be equal 4, please check your microhalPortConfig_stm32f4xx.h
    #endif
    #ifndef MICROHAL_SERIAL_PORT4_DMA_RX_STREAM
        #error MICROHAL_SERIAL_PORT4_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT4_DMA_RX_STREAM != 2 // Serial port 4 RX stream can be connected to DMA1 Channel 2.
        #error MICROHAL_SERIAL_PORT4_DMA_TX_STREAM have to be equal 2, please check your microhalPortConfig_stm32f4xx.h
    #endif
// clang-format on
#endif
// ********************* Serial Port 5
// ---- POLLING and INTERRUPT
#if defined(MICROHAL_USE_SERIAL_PORT5_POLLING) || defined(MICROHAL_USE_SERIAL_PORT5_INTERRUPT)
#endif
// ---- INTERRUPT_DMA
#if defined(MICROHAL_USE_SERIAL_PORT5_INTERRUPT_DMA)
#endif
// ---- DMA
#if defined(MICROHAL_USE_SERIAL_PORT5_DMA)
// clang-format off
    #ifndef MICROHAL_SERIAL_PORT5_DMA_TX_STREAM
        #error MICROHAL_SERIAL_PORT5_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT5_DMA_TX_STREAM != 7 // Serial port 5 TX stream can be connected only to DMA1 Channel 7.
        #error MICROHAL_SERIAL_PORT5_DMA_TX_STREAM have to be equal 7, please check your microhalPortConfig_stm32f4xx.h
    #endif
    #ifndef MICROHAL_SERIAL_PORT5_DMA_RX_STREAM
        #error MICROHAL_SERIAL_PORT5_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT5_DMA_RX_STREAM != 0 // Serial port 5 RX stream can be connected to DMA1 Channel 0.
        #error MICROHAL_SERIAL_PORT5_DMA_TX_STREAM have to be equal 0, please check your microhalPortConfig_stm32f4xx.h
    #endif
// clang-format on
#endif
// ********************* Serial Port 6
// ---- POLLING and INTERRUPT
#if defined(MICROHAL_USE_SERIAL_PORT6_POLLING) || defined(MICROHAL_USE_SERIAL_PORT6_INTERRUPT)
#endif
// ---- INTERRUPT_DMA
#if defined(MICROHAL_USE_SERIAL_PORT6_INTERRUPT_DMA)
#endif
// ---- DMA
#if defined(MICROHAL_USE_SERIAL_PORT6_DMA)
// clang-format off
    #ifndef MICROHAL_SERIAL_PORT6_DMA_TX_STREAM
        #error MICROHAL_SERIAL_PORT6_DMA_TX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT6_DMA_TX_STREAM != 6 && MICROHAL_SERIAL_PORT6_DMA_TX_STREAM != 7 // Serial port 6 TX stream can be connected to DMA2 Channel 6 or DMA2 Channel 7.
        #error MICROHAL_SERIAL_PORT6_DMA_TX_STREAM have to be equal 6 or 7, please check your microhalPortConfig_stm32f4xx.h
    #endif
    #ifndef MICROHAL_SERIAL_PORT6_DMA_RX_STREAM
        #error MICROHAL_SERIAL_PORT6_DMA_RX_STREAM have to be defined inside microhalPortConfig_stm32f4xx.h
    #elif MICROHAL_SERIAL_PORT6_DMA_RX_STREAM != 1 && MICROHAL_SERIAL_PORT2_DMA_RX_STREAM != 2  // Serial port 6 RX stream can be connected to DMA2 Channel 1 or DMA1 Channel 2.
        #error MICROHAL_SERIAL_PORT6_DMA_TX_STREAM have to be equal 1 or 2, please check your microhalPortConfig_stm32f4xx.h
    #endif
// clang-format on
#endif
// ********************* Serial Port 7
// ---- POLLING, INTERRUPT, INTERRUPT_DMA, DMA
#if defined(MICROHAL_USE_SERIAL_PORT7_POLLING) || defined(MICROHAL_USE_SERIAL_PORT7_INTERRUPT) || \
    defined(MICROHAL_USE_SERIAL_PORT7_INTERRUPT_DMA) || defined(MICROHAL_USE_SERIAL_PORT7_DMA)
#error Enabled Serial Port 7 by defining MICROHAL_USE_SERIAL_PORT7_xxx but STM32F411xE do not have Serial port 7 hardware. Please disable Serial Port 7
#endif
// ********************* Serial Port 8
// ---- POLLING, INTERRUPT, INTERRUPT_DMA, DMA
#if defined(MICROHAL_USE_SERIAL_PORT8_POLLING) || defined(MICROHAL_USE_SERIAL_PORT8_INTERRUPT) || \
    defined(MICROHAL_USE_SERIAL_PORT8_INTERRUPT_DMA) || defined(MICROHAL_USE_SERIAL_PORT8_DMA)
#error Enabled Serial Port 8 by defining MICROHAL_USE_SERIAL_PORT8_xxx but STM32F411xE do not have Serial port 8 hardware. Please disable Serial Port 8
#endif

#endif /* _MICROHAL_STM32F4XX_PORTCONFIGURATIONASSERT_STM32F407XX_H_ */
