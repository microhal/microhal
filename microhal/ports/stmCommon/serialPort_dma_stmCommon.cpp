/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      STM32F4xx serial port driver implementation. Driver support receiving and transmitting using DMA.
 *
 * @authors    Pawel Okas
 * created on: 7-07-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

/* ************************************************************************************************
 * INCLUDES
 */
#include "serialPort_dma_stmCommon.h"
#include _MICROHAL_INCLUDE_PORT_clockManager

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
//***********************************************************************************************//
//                                   STATIC VARIABLES
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_DMA
static char txBufferData_1[MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE];
static char rxBufferData_1[MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE];
SerialPort_Dma SerialPort_Dma::Serial1(*USART1, rxBufferData_1, txBufferData_1, sizeof(rxBufferData_1), sizeof(txBufferData_1), *DMA::dma2,
                                       DMA::dma2->stream[7], DMA::dma2->stream[MICROHAL_SERIAL_PORT1_DMA_RX_STREAM]);
SerialPort &SerialPort::Serial1 = SerialPort_Dma::Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_DMA
static char txBufferData_2[MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE];
static char rxBufferData_2[MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE];
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
SerialPort_Dma SerialPort_Dma::Serial2(*USART2, rxBufferData_2, txBufferData_2, sizeof(rxBufferData_2), sizeof(txBufferData_2),
                                       DMA::dma1->stream[MICROHAL_SERIAL_PORT2_DMA_TX_STREAM - 1],
                                       DMA::dma1->stream[MICROHAL_SERIAL_PORT2_DMA_RX_STREAM - 1]);
#else
SerialPort_Dma SerialPort_Dma::Serial2(*USART2, rxBufferData_2, txBufferData_2, sizeof(rxBufferData_2), sizeof(txBufferData_2), *DMA::dma1,
                                       DMA::dma1->stream[6], DMA::dma1->stream[5]);
#endif
SerialPort &SerialPort::Serial2 = SerialPort_Dma::Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_DMA
static char txBufferData_3[MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE];
static char rxBufferData_3[MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE];
SerialPort_Dma SerialPort_Dma::Serial3(*USART3, rxBufferData_3, txBufferData_3, sizeof(rxBufferData_3), sizeof(txBufferData_3), *DMA::dma1,
                                       DMA::dma1->stream[MICROHAL_SERIAL_PORT3_DMA_TX_STREAM], DMA::dma1->stream[1]);
SerialPort &SerialPort::Serial3 = SerialPort_Dma::Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_DMA
static char txBufferData_4[MICROHAL_SERIAL_PORT4_TX_BUFFER_SIZE];
static char rxBufferData_4[MICROHAL_SERIAL_PORT4_RX_BUFFER_SIZE];
SerialPort_Dma SerialPort_Dma::Serial4(*UART4, rxBufferData_4, txBufferData_4, sizeof(rxBufferData_4), sizeof(txBufferData_4), *DMA::dma1,
                                       DMA::dma1->stream[4], DMA::dma1->stream[2]);
SerialPort &SerialPort::Serial4 = SerialPort_Dma::Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_DMA
static char txBufferData_5[MICROHAL_SERIAL_PORT5_TX_BUFFER_SIZE];
static char rxBufferData_5[MICROHAL_SERIAL_PORT5_RX_BUFFER_SIZE];
SerialPort_Dma SerialPort_Dma::Serial5(*UART5, rxBufferData_5, txBufferData_5, sizeof(rxBufferData_5), sizeof(txBufferData_5), *DMA::dma1,
                                       DMA::dma1->stream[7], DMA::dma1->stream[0]);
SerialPort &SerialPort::Serial5 = SerialPort_Dma::Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_DMA
static char txBufferData_6[MICROHAL_SERIAL_PORT6_TX_BUFFER_SIZE];
static char rxBufferData_6[MICROHAL_SERIAL_PORT6_RX_BUFFER_SIZE];
SerialPort_Dma SerialPort_Dma::Serial6(*USART6, rxBufferData_6, txBufferData_6, sizeof(rxBufferData_6), sizeof(txBufferData_6), *DMA::dma2,
                                       DMA::dma2->stream[MICROHAL_SERIAL_PORT6_DMA_TX_STREAM],
                                       DMA::dma2->stream[MICROHAL_SERIAL_PORT6_DMA_RX_STREAM]);
SerialPort &SerialPort::Serial6 = SerialPort_Dma::Serial6;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT7_DMA
static char txBufferData_7[MICROHAL_SERIAL_PORT7_TX_BUFFER_SIZE];
static char rxBufferData_7[MICROHAL_SERIAL_PORT7_RX_BUFFER_SIZE];
SerialPort_Dma SerialPort_Dma::Serial7(*UART7, rxBufferData_7, txBufferData_7, sizeof(rxBufferData_7), sizeof(txBufferData_7), *DMA::dma1,
                                       DMA::dma1->stream[1], DMA::dma1->stream[3]);
SerialPort &SerialPort::Serial7 = SerialPort_Dma::Serial7;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT8_DMA
static char txBufferData_8[MICROHAL_SERIAL_PORT8_TX_BUFFER_SIZE];
static char rxBufferData_8[MICROHAL_SERIAL_PORT8_RX_BUFFER_SIZE];
SerialPort_Dma SerialPort_Dma::Serial8(*UART8, rxBufferData_8, txBufferData_8, sizeof(rxBufferData_8), sizeof(txBufferData_8), *DMA::dma1,
                                       DMA::dma1->stream[0], DMA::dma1->stream[6]);
SerialPort &SerialPort::Serial8 = SerialPort_Dma::Serial8;
#endif

#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
SerialPort_Dma::SerialPort_Dma(USART_TypeDef &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize,
                               DMA::Channel &txStream, DMA::Channel &rxStream)
    : SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize),
      txStream(txStream),
      rxStream(rxStream)
#else
SerialPort_Dma::SerialPort_Dma(USART_TypeDef &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize, DMA::DMA &dma,
                               DMA::Stream &txStream, DMA::Stream &rxStream)
    : SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize),
      dma(dma),
      txStream(txStream),
      rxStream(rxStream)
#endif
{
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enable(usart, ClockManager::PowerMode::Normal);
#else
    ClockManager::enable(usart);
#endif
    enableInterrupt(0);
///////////////////////////////////
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
    using Interrupt = DMA::Channel::Interrupt;
    auto &dma = *DMA::dma1;
#else
    using Interrupt = DMA::Stream::Interrupt;
#endif
    dma.clockEnable();
    // tx
    txStream.deinit();
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
    txStream.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                  DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&usart.TDR);
#else
    txStream.init(dma.channel(txStream, &usart), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&usart.DR);
#endif
    txStream.enableInterrupt(Interrupt::TransferComplete);
    // rx
    rxStream.deinit();
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
    rxStream.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                  DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
    rxStream.setPeripheralAddress(&usart.RDR);
#else
    rxStream.init(dma.channel(rxStream, &usart), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::PerToMem);
    rxStream.setPeripheralAddress(&usart.DR);
#endif
    rxStream.enableInterrupt(Interrupt::TransferComplete);

    dma.clearInterruptFlag(txStream, Interrupt::TransferComplete);
    dma.clearInterruptFlag(rxStream, Interrupt::TransferComplete);
    dma.enableInterrupt(txStream, 6);
    dma.enableInterrupt(rxStream, 6);
}

bool SerialPort_Dma::open(OpenMode mode) noexcept {
    if (isOpen() || (mode > 0x03)) return false;
    usart.CR1 |= (mode << 2) | USART_CR1_UE;  // | USART_CR1_RXNEIE;
    usart.CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    prepareRxDmaTransfer();
    return true;
}

size_t SerialPort_Dma::prepareDmaTransfer() {
    const char *writePtr;
    size_t writeLen = txBuffer.getContinousPart(writePtr);
    if (writeLen) {
        transferInProgress = writeLen;
        // configure tx
        txStream.setNumberOfItemsToTransfer(writeLen);
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
        txStream.setMemoryAddress((void *)writePtr);
#else
        txStream.setMemoryBank0(writePtr);
#endif
        txStream.enable();
    }
    return writeLen;
}

void SerialPort_Dma::prepareRxDmaTransfer(size_t bytesToReceive) {
    const char *readPtr = nullptr;
    size_t readLen = rxBuffer.getEmptyContinousPart(readPtr);
    if (readLen > bytesToReceive) readLen = bytesToReceive;
    if (readLen) {
        rxTransferInProgress = readLen;
        // configure tx
        rxStream.setNumberOfItemsToTransfer(readLen);
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
        rxStream.setMemoryAddress((void *)readPtr);
#else
        rxStream.setMemoryBank0(readPtr);
#endif
        rxStream.enable();
    } else {
    }
}
//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
inline void serialPort_interruptFunction(USART_TypeDef *const usart, SerialPort_Dma &serial) {
    (void)serial;
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
    uint16_t sr = usart->ISR;
#else
    uint16_t sr = usart->SR;
#endif
    (void)sr;
}

inline void DMA_rx_function(SerialPort_Dma &serial, uint32_t DMA_Stream_NDTR) {
    // update rx buffer
    size_t receivedBytes = serial.rxTransferInProgress - DMA_Stream_NDTR;
    serial.rxBuffer.updateWritePointer(receivedBytes);

    if (serial.waitForBytes != 0) {
        // calculate number of bytes to received
        int32_t bytesForReceive = serial.waitForBytes - receivedBytes;
        serial.waitForBytes = bytesForReceive;
        if (bytesForReceive > 0) {
            serial.prepareRxDmaTransfer(bytesForReceive);
        } else {
            serial.prepareRxDmaTransfer();
            bool shouldYeld = serial.rxSemaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        }
    }
}

inline void DMA_tx_function(SerialPort_Dma &serial) {
    serial.txBuffer.updateReadPointer(serial.transferInProgress);
    serial.transferInProgress = 0;
    if (serial.prepareDmaTransfer() == 0 && serial.txWait) {
        serial.txWait = false;
        bool shouldYeld = serial.txFinish.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR(shouldYeld);
#else
        (void)shouldYeld;
#endif
    }
}

//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#define GLUE3(x, y, z) x##y##z
#define GLUE(x, y) x##y

#define ENABLE_IF_LESS_4(c, v) ENABLE_IF_L_##c(v)
#define ENABLE_IF_MORE_4(c, v) ENABLE_IF_M_##c(v)
#define ENABLE_IF_L_0(x) x
#define ENABLE_IF_L_1(x) x
#define ENABLE_IF_L_2(x) x
#define ENABLE_IF_L_3(x) x
#define ENABLE_IF_L_4(x)
#define ENABLE_IF_L_5(x)
#define ENABLE_IF_L_6(x)
#define ENABLE_IF_L_7(x)

#define ENABLE_IF_M_0(x)
#define ENABLE_IF_M_1(x)
#define ENABLE_IF_M_2(x)
#define ENABLE_IF_M_3(x)
#define ENABLE_IF_M_4(x) x
#define ENABLE_IF_M_5(x) x
#define ENABLE_IF_M_6(x) x
#define ENABLE_IF_M_7(x) x

#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
#define DMA1_TX_STREAM_IRQHANDLER(s, n)                                                                     \
    extern "C" void GLUE3(DMA1_Channel, n, _IRQHandler)(void) {                                             \
        DMA::dma1->clearInterruptFlag(DMA::dma1->stream[n - 1], DMA::Channel::Interrupt::TransferComplete); \
        DMA::dma1->stream[n - 1].disable();                                                                 \
        DMA_tx_function(SerialPort_Dma::s);                                                                 \
    }

#define DMA1_RX_STREAM_IRQHANDLER(s, n)                                                                     \
    extern "C" void GLUE3(DMA1_Channel, n, _IRQHandler)(void) {                                             \
        DMA::dma1->clearInterruptFlag(DMA::dma1->stream[n - 1], DMA::Channel::Interrupt::TransferComplete); \
        DMA::dma1->stream[n - 1].disable();                                                                 \
        DMA_rx_function(SerialPort_Dma::s, DMA::dma1->stream[n - 1].getNumberOfItemsInTransfer());          \
    }
#else
#define DMA1_TX_STREAM_IRQHANDLER(s, n)                              \
    extern "C" void GLUE3(DMA1_Stream, n, _IRQHandler)(void) {       \
        ENABLE_IF_LESS_4(n, DMA1->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);) \
        ENABLE_IF_MORE_4(n, DMA1->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);) \
        GLUE(DMA1_Stream, n)->CR &= ~DMA_SxCR_EN;                    \
        DMA_tx_function(SerialPort_Dma::s);                          \
    }

#define DMA1_RX_STREAM_IRQHANDLER(s, n)                                 \
    extern "C" void GLUE3(DMA1_Stream, n, _IRQHandler)(void) {          \
        ENABLE_IF_LESS_4(n, DMA1->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);)    \
        ENABLE_IF_MORE_4(n, DMA1->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);)    \
        GLUE(DMA1_Stream, n)->CR &= ~DMA_SxCR_EN;                       \
        DMA_rx_function(SerialPort_Dma::s, GLUE(DMA1_Stream, n)->NDTR); \
    }

#define DMA2_TX_STREAM_IRQHANDLER(s, n)                              \
    extern "C" void GLUE3(DMA2_Stream, n, _IRQHandler)(void) {       \
        ENABLE_IF_LESS_4(n, DMA2->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);) \
        ENABLE_IF_MORE_4(n, DMA2->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);) \
        GLUE(DMA2_Stream, n)->CR &= ~DMA_SxCR_EN;                    \
        DMA_tx_function(SerialPort_Dma::s);                          \
    }

#define DMA2_RX_STREAM_IRQHANDLER(s, n)                                 \
    extern "C" void GLUE3(DMA2_Stream, n, _IRQHandler)(void) {          \
        ENABLE_IF_LESS_4(n, DMA2->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);)    \
        ENABLE_IF_MORE_4(n, DMA2->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);)    \
        GLUE(DMA2_Stream, n)->CR &= ~DMA_SxCR_EN;                       \
        DMA_rx_function(SerialPort_Dma::s, GLUE(DMA2_Stream, n)->NDTR); \
    }
#endif
// ---------------------------- serial port 1
#ifdef MICROHAL_USE_SERIAL_PORT1_DMA
void USART1_IRQHandler(void) {
    serialPort_interruptFunction(USART1, SerialPort_Dma::Serial1);
}
DMA2_RX_STREAM_IRQHANDLER(Serial1, MICROHAL_SERIAL_PORT1_DMA_RX_STREAM)
DMA2_TX_STREAM_IRQHANDLER(Serial1, MICROHAL_SERIAL_PORT1_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 2
#ifdef MICROHAL_USE_SERIAL_PORT2_DMA
void USART2_IRQHandler(void) {
    serialPort_interruptFunction(USART2, SerialPort_Dma::Serial2);
}
DMA1_RX_STREAM_IRQHANDLER(Serial2, MICROHAL_SERIAL_PORT2_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial2, MICROHAL_SERIAL_PORT2_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 3
#ifdef MICROHAL_USE_SERIAL_PORT3_DMA
void USART3_IRQHandler(void) {
    serialPort_interruptFunction(USART3, SerialPort_Dma::Serial3);
}
DMA1_RX_STREAM_IRQHANDLER(Serial3, MICROHAL_SERIAL_PORT3_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial3, MICROHAL_SERIAL_PORT3_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 4
#ifdef MICROHAL_USE_SERIAL_PORT4_DMA
void UART4_IRQHandler(void) {
    serialPort_interruptFunction(UART4, SerialPort_Dma::Serial4);
}
DMA1_RX_STREAM_IRQHANDLER(Serial4, MICROHAL_SERIAL_PORT4_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial4, MICROHAL_SERIAL_PORT4_DMA_TX_STREAM)
}
#endif
// ---------------------------- serial port 5
#ifdef MICROHAL_USE_SERIAL_PORT5_DMA
void UART5_IRQHandler(void) {
    serialPort_interruptFunction(UART5, SerialPort_Dma::Serial5);
}
DMA1_RX_STREAM_IRQHANDLER(Serial5, MICROHAL_SERIAL_PORT5_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial5, MICROHAL_SERIAL_PORT5_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 6
#ifdef MICROHAL_USE_SERIAL_PORT6_DMA
void USART6_IRQHandler(void) {
    serialPort_interruptFunction(USART6, SerialPort_Dma::Serial6);
}
DMA2_RX_STREAM_IRQHANDLER(Serial6, MICROHAL_SERIAL_PORT6_DMA_RX_STREAM)
DMA2_TX_STREAM_IRQHANDLER(Serial6, MICROHAL_SERIAL_PORT6_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 7
#ifdef MICROHAL_USE_SERIAL_PORT7_DMA
void UART7_IRQHandler(void) {
    serialPort_interruptFunction(UART7, SerialPort_Dma::Serial7);
}
DMA1_RX_STREAM_IRQHANDLER(Serial7, MICROHAL_SERIAL_PORT7_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial7, MICROHAL_SERIAL_PORT7_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 8
#ifdef MICROHAL_USE_SERIAL_PORT8_DMA
void UART8_IRQHandler(void) {
    serialPort_interruptFunction(UART8, SerialPort_Dma::Serial8);
}
DMA1_RX_STREAM_IRQHANDLER(Serial8, MICROHAL_SERIAL_PORT8_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial8, MICROHAL_SERIAL_PORT8_DMA_TX_STREAM)
#endif

}  // namespace microhal
}  // namespace microhal
