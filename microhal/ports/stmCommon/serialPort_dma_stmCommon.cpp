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

#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
#define _MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(TX_SIZE, RX_SIZE, TX_DMA, RX_DMA, u, d)                                                \
    static char txBufferData_##u[TX_SIZE];                                                                                            \
    static char rxBufferData_##u[RX_SIZE];                                                                                            \
    SerialPort_Dma SerialPort_Dma::Serial##u(*registers::usart##u, rxBufferData_##u, txBufferData_##u, sizeof(rxBufferData_##u),      \
                                             sizeof(txBufferData_##u), DMA::dma1->stream[TX_DMA - 1], DMA::dma1->stream[RX_DMA - 1]); \
    SerialPort &SerialPort::Serial##u = SerialPort_Dma::Serial##u;
#else
#define _MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(TX_SIZE, RX_SIZE, TX_DMA, RX_DMA, u, d)                                           \
    static char txBufferData_##u[TX_SIZE];                                                                                       \
    static char rxBufferData_##u[RX_SIZE];                                                                                       \
    SerialPort_Dma SerialPort_Dma::Serial##u(*registers::usart##u, rxBufferData_##u, txBufferData_##u, sizeof(rxBufferData_##u), \
                                             sizeof(txBufferData_##u), *DMA::d, DMA::d->stream[TX_DMA], DMA::d->stream[RX_DMA]); \
    SerialPort &SerialPort::Serial##u = SerialPort_Dma::Serial##u;
#endif

#ifdef MICROHAL_USE_SERIAL_PORT1_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT1_DMA_TX_STREAM, MICROHAL_SERIAL_PORT1_DMA_RX_STREAM, 1, dma2)
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT2_DMA_TX_STREAM, MICROHAL_SERIAL_PORT2_DMA_RX_STREAM, 2, dma1)
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT3_DMA_TX_STREAM, MICROHAL_SERIAL_PORT3_DMA_RX_STREAM, 3, dma1)
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT4_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT4_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT4_DMA_TX_STREAM, MICROHAL_SERIAL_PORT4_DMA_RX_STREAM, 4, dma1)
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT5_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT5_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT5_DMA_TX_STREAM, MICROHAL_SERIAL_PORT5_DMA_RX_STREAM, 5, dma1)
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT6_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT6_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT6_DMA_TX_STREAM, MICROHAL_SERIAL_PORT6_DMA_RX_STREAM, 6, dma2)
#endif
#ifdef MICROHAL_USE_SERIAL_PORT7_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT7_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT7_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT7_DMA_TX_STREAM, MICROHAL_SERIAL_PORT7_DMA_RX_STREAM, 7, dma1)
#endif
#ifdef MICROHAL_USE_SERIAL_PORT8_DMA
_MICROHAL_SERIALPORT_DMA_OBJECT_CREATE(MICROHAL_SERIAL_PORT8_TX_BUFFER_SIZE, MICROHAL_SERIAL_PORT8_RX_BUFFER_SIZE,
                                       MICROHAL_SERIAL_PORT8_DMA_TX_STREAM, MICROHAL_SERIAL_PORT8_DMA_RX_STREAM, 8, dma1)
#endif

#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
SerialPort_Dma::SerialPort_Dma(registers::USART &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize,
                               DMA::Channel &txStream, DMA::Channel &rxStream)
    : SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize),
      txStream(txStream),
      rxStream(rxStream)
#else
SerialPort_Dma::SerialPort_Dma(microhal::registers::USART &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize,
                               DMA::DMA &dma, DMA::Stream &txStream, DMA::Stream &rxStream)
    : SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize),
      dma(dma),
      txStream(txStream),
      rxStream(rxStream)
#endif
{
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableUSART(number(), ClockManager::PowerMode::Normal);
#else
    ClockManager::enableUSART(number());
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
    txStream.setPeripheralAddress(&usart.tdr);
#else
    txStream.init(dma.channel(txStream, &usart), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&usart.dr);
#endif
    txStream.enableInterrupt(Interrupt::TransferComplete);
    // rx
    rxStream.deinit();
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
    rxStream.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                  DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
    rxStream.setPeripheralAddress(&usart.rdr);
#else
    rxStream.init(dma.channel(rxStream, &usart), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::PerToMem);
    rxStream.setPeripheralAddress(&usart.dr);
#endif
    rxStream.enableInterrupt(Interrupt::TransferComplete);

    dma.clearInterruptFlag(txStream, Interrupt::TransferComplete);
    dma.clearInterruptFlag(rxStream, Interrupt::TransferComplete);
    dma.enableInterrupt(txStream, 6);
    dma.enableInterrupt(rxStream, 6);
}

bool SerialPort_Dma::open(OpenMode mode) noexcept {
    if (isOpen() || (mode > 0x03)) return false;
    auto cr1 = usart.cr1.volatileLoad();
    cr1 |= (mode << 2);
    cr1.UE.set();
    usart.cr1.volatileStore(cr1);

    auto cr3 = usart.cr3.volatileLoad();
    cr3.DMAT.set();
    cr3.DMAR.set();
    usart.cr3.volatileStore(cr3);
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

bool SerialPort_Dma::clearImpl(Direction dir) noexcept {
    if ((dir == SerialPort::Input) || (dir == SerialPort::AllDirections)) {
        rxStream.disable();
        rxBuffer.flush();
        prepareRxDmaTransfer();
    }
    if ((dir == SerialPort::Output) || (dir == SerialPort::AllDirections)) {
        txStream.disable();
        txBuffer.flush();
        transferInProgress = 0;
        txWait = false;
    }
    return true;
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE

//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
inline void serialPort_interruptFunction(registers::USART *const usart, SerialPort_Dma &serial) {
    (void)serial;
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
    uint16_t sr = (uint32_t)usart->isr.volatileLoad();
#else
    uint16_t sr = (uint32_t)usart->sr.volatileLoad();
#endif
    (void)sr;
}

void DMA_rx_function(SerialPort_Dma &serial, uint32_t DMA_Stream_NDTR) {
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

void DMA_tx_function(SerialPort_Dma &serial) {
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
    serialPort_interruptFunction(registers::usart1, SerialPort_Dma::Serial1);
}
DMA2_RX_STREAM_IRQHANDLER(Serial1, MICROHAL_SERIAL_PORT1_DMA_RX_STREAM)
DMA2_TX_STREAM_IRQHANDLER(Serial1, MICROHAL_SERIAL_PORT1_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 2
#ifdef MICROHAL_USE_SERIAL_PORT2_DMA
void USART2_IRQHandler(void) {
    serialPort_interruptFunction(registers::usart2, SerialPort_Dma::Serial2);
}
DMA1_RX_STREAM_IRQHANDLER(Serial2, MICROHAL_SERIAL_PORT2_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial2, MICROHAL_SERIAL_PORT2_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 3
#ifdef MICROHAL_USE_SERIAL_PORT3_DMA
void USART3_IRQHandler(void) {
    serialPort_interruptFunction(registers::usart3, SerialPort_Dma::Serial3);
}
DMA1_RX_STREAM_IRQHANDLER(Serial3, MICROHAL_SERIAL_PORT3_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial3, MICROHAL_SERIAL_PORT3_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 4
#ifdef MICROHAL_USE_SERIAL_PORT4_DMA
void UART4_IRQHandler(void) {
    serialPort_interruptFunction(registers::uart4, SerialPort_Dma::Serial4);
}
DMA1_RX_STREAM_IRQHANDLER(Serial4, MICROHAL_SERIAL_PORT4_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial4, MICROHAL_SERIAL_PORT4_DMA_TX_STREAM)
}
#endif
// ---------------------------- serial port 5
#ifdef MICROHAL_USE_SERIAL_PORT5_DMA
void UART5_IRQHandler(void) {
    serialPort_interruptFunction(registers::uart5, SerialPort_Dma::Serial5);
}
DMA1_RX_STREAM_IRQHANDLER(Serial5, MICROHAL_SERIAL_PORT5_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial5, MICROHAL_SERIAL_PORT5_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 6
#ifdef MICROHAL_USE_SERIAL_PORT6_DMA
void USART6_IRQHandler(void) {
    serialPort_interruptFunction(registers::usart6, SerialPort_Dma::Serial6);
}
DMA2_RX_STREAM_IRQHANDLER(Serial6, MICROHAL_SERIAL_PORT6_DMA_RX_STREAM)
DMA2_TX_STREAM_IRQHANDLER(Serial6, MICROHAL_SERIAL_PORT6_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 7
#ifdef MICROHAL_USE_SERIAL_PORT7_DMA
void UART7_IRQHandler(void) {
    serialPort_interruptFunction(registers::usart7, SerialPort_Dma::Serial7);
}
DMA1_RX_STREAM_IRQHANDLER(Serial7, MICROHAL_SERIAL_PORT7_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial7, MICROHAL_SERIAL_PORT7_DMA_TX_STREAM)
#endif
// ---------------------------- serial port 8
#ifdef MICROHAL_USE_SERIAL_PORT8_DMA
void UART8_IRQHandler(void) {
    serialPort_interruptFunction(registers::uart8, SerialPort_Dma::Serial8);
}
DMA1_RX_STREAM_IRQHANDLER(Serial8, MICROHAL_SERIAL_PORT8_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(Serial8, MICROHAL_SERIAL_PORT8_DMA_TX_STREAM)
#endif

}  // namespace microhal
}  // namespace microhal
