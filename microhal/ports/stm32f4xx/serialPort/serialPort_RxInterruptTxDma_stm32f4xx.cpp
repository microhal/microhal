/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      STM32F4xx serial port driver implementation. Driver support receiving via interrupt and transmitting using DMA.
 *
 * @authors    Pawel Okas
 * created on: 7-07-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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
#include "serialPort_RxInterruptTxDma_stm32f4xx.h"
#include "../clockManager.h"

namespace microhal {
namespace stm32f4xx {
//***********************************************************************************************//
//                                   STATIC VARIABLES
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA
static char txBufferData_1[MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE];
static char rxBufferData_1[MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE];
SerialPort_RxInterruptTxDma SerialPort_RxInterruptTxDma::Serial1(*USART1, rxBufferData_1, txBufferData_1, sizeof(rxBufferData_1), sizeof(txBufferData_1), *DMA::dma2, DMA::dma2->stream[7]);
SerialPort &SerialPort::Serial1 = SerialPort_RxInterruptTxDma::Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT_DMA
static char txBufferData_2[MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE];
static char rxBufferData_2[MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE];
SerialPort_RxInterruptTxDma SerialPort_RxInterruptTxDma::Serial2(*USART2, rxBufferData_2, txBufferData_2, sizeof(rxBufferData_2), sizeof(txBufferData_2), *DMA::dma1, DMA::dma1->stream[6]);
SerialPort &SerialPort::Serial2 = SerialPort_RxInterruptTxDma::Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT_DMA
static char txBufferData_3[MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE];
static char rxBufferData_3[MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE];
SerialPort_RxInterruptTxDma SerialPort_RxInterruptTxDma::Serial3(*USART3, rxBufferData_3, txBufferData_3, sizeof(rxBufferData_3), sizeof(txBufferData_3), *DMA::dma1, DMA::dma1->stream[MICROHAL_SERIAL_PORT3_DMA_TX_STREAM]);
SerialPort &SerialPort::Serial3 = SerialPort_RxInterruptTxDma::Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT_DMA
static char txBufferData_4[MICROHAL_SERIAL_PORT4_TX_BUFFER_SIZE];
static char rxBufferData_4[MICROHAL_SERIAL_PORT4_RX_BUFFER_SIZE];
SerialPort_RxInterruptTxDma SerialPort_RxInterruptTxDma::Serial4(*UART4, rxBufferData_4, txBufferData_4, sizeof(rxBufferData_4), sizeof(txBufferData_4), *DMA::dma1, DMA::dma1->stream[4]);
SerialPort &SerialPort::Serial4 = SerialPort_RxInterruptTxDma::Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT_DMA
static char txBufferData_5[MICROHAL_SERIAL_PORT5_TX_BUFFER_SIZE];
static char rxBufferData_5[MICROHAL_SERIAL_PORT5_RX_BUFFER_SIZE];
SerialPort_RxInterruptTxDma SerialPort_RxInterruptTxDma::Serial5(*USART5, rxBufferData_5, txBufferData_5, sizeof(rxBufferData_5), sizeof(txBufferData_5), *DMA::dma1, DMA::dma1->stream[7]);
SerialPort &SerialPort::Serial5 = SerialPort_RxInterruptTxDma::Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT_DMA
static char txBufferData_6[MICROHAL_SERIAL_PORT6_TX_BUFFER_SIZE];
static char rxBufferData_6[MICROHAL_SERIAL_PORT6_RX_BUFFER_SIZE];
SerialPort_RxInterruptTxDma SerialPort_RxInterruptTxDma::Serial6(*USART6, rxBufferData_6, txBufferData_6, sizeof(rxBufferData_6), sizeof(txBufferData_6), *DMA::dma2, DMA::dma1->stream[MICROHAL_SERIAL_PORT6_DMA_TX_STREAM]);
SerialPort &SerialPort::Serial6 = SerialPort_RxInterruptTxDma::Serial6;
#endif

//
//static DMA::Stream::Channel getTxChannalNumber(USART_TypeDef &usart) {
//  switch (reinterpret_cast<uint32_t>(&usart)) {
//    //case reinterpret_cast<uint32_t>(USART1):
//      //return DMA::Stream::Channel::Channel3;
//    case reinterpret_cast<uint32_t>(USART2):
//    // intentionally lack of break
//    case reinterpret_cast<uint32_t>(USART3):
//#if MICROHAL_SERIAL_PORT3_DMA_TX_STREAM == 3
//      return DMA::Stream::Channel::Channel4;
//#elif MICROHAL_SERIAL_PORT3_DMA_TX_STREAM == 4
//    return DMA::Stream::Channel::Channel7;
//#endif
//    break;
////#ifdef USART4_IRQn
////    case reinterpret_cast<uint32_t>(USART4):
////      return DMA::Stream::Channel::Channel4;
////      return DMA::Stream::Channel::Channel5;
////      break;
////#endif
////#ifdef USART5_IRQn
////    case reinterpret_cast<uint32_t>(USART5):
////      return DMA::Stream::Channel::Channel2;
////      return DMA::Stream::Channel::Channel7;
////#endif
////#ifdef USART6_IRQn
////    case reinterpret_cast<uint32_t>(USART6):
////      return DMA::Stream::Channel::Channel1;
////#endif
//  }
//  while (1)
//    ;
//  return DMA::Stream::Channel::Channel0;
//}

SerialPort_RxInterruptTxDma::SerialPort_RxInterruptTxDma(USART_TypeDef &usart, char * const rxData, char * const txData, size_t rxDataSize, size_t txDataSize, DMA::DMA &dma, DMA::Stream & txStream) :
        SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize), dma(dma), txStream(txStream) {
	ClockManager::enable(usart);
	enableInterrupt(0);
///////////////////////////////////
    dma.clockEnable();
    // tx
    txStream.deinit();
  //  txStream.getUsartChanalNumber(usart);
    auto channel = dma.channel(txStream, usart);
    txStream.init(
    	channel, DMA::Stream::MemoryBurst::SingleTransfer,
        DMA::Stream::PeripheralBurst::SingleTransfer,
        DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte,
        DMA::Stream::MemoryIncrementMode::PointerIncremented,
        DMA::Stream::PeripheralIncrementMode::PointerFixed,
        DMA::Stream::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&usart.DR);
    txStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);

    dma.clearInterruptFlag(txStream, DMA::Stream::Interrupt::TransferComplete);
    dma.enableInterrupt(txStream, 6);
}

bool SerialPort_RxInterruptTxDma::open(OpenMode mode) noexcept {
    if (isOpen() || (mode > 0x03)) return false;
    usart.CR1 |= (mode << 2) | USART_CR1_UE | USART_CR1_RXNEIE;
    usart.CR3 |= USART_CR3_DMAT;
    return true;
}

size_t SerialPort_RxInterruptTxDma::prepareDmaTransfer() {
	const char *writePtr;
	size_t writeLen = txBuffer.getContinousPart(writePtr);
	if (writeLen) {
		transferInProgress = writeLen;
		// configure tx
		txStream.setNumberOfItemsToTransfer(writeLen);
		txStream.setMemoryBank0(writePtr);
		txStream.enable();
	}
	return writeLen;
}

void SerialPort_RxInterruptTxDma::onDMATxFinish() {
	txBuffer.updateReadPointer(transferInProgress);
	transferInProgress = 0;
	if (prepareDmaTransfer() == 0 && txWait) {
		txWait = false;
		bool shouldYeld = txFinish.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
		portYIELD_FROM_ISR( shouldYeld );
#else
		(void)shouldYeld;
#endif
	}
}

//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
void serialPort_interruptFunction(USART_TypeDef * const usart, SerialPort_RxInterruptTxDma &serial) {
    uint16_t sr = usart->SR;

    if (sr & USART_SR_RXNE) {
        char tmp = usart->DR;
        serial.rxBuffer.append(tmp);
        if (serial.waitForBytes != 0 && serial.rxBuffer.getLength() == serial.waitForBytes) {
        	serial.waitForBytes = 0;
        	bool shouldYeld = serial.rxSemaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
        	portYIELD_FROM_ISR( shouldYeld );
#else
        	(void)shouldYeld;
#endif
        }
    }
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
// ---------------------------- serial port 1
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA
void USART1_IRQHandler(void) {
    serialPort_interruptFunction(USART1, SerialPort_RxInterruptTxDma::Serial1);
}

void DMA2_Stream7_IRQHandler(void) {
  DMA2->HIFCR = DMA_HIFCR_CTCIF7;
  DMA2_Stream7->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial1.onDMATxFinish();
}
#endif
// ---------------------------- serial port 2
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT_DMA
void USART2_IRQHandler(void) {
    serialPort_interruptFunction(USART2, SerialPort_RxInterruptTxDma::Serial2);
}

void DMA1_Stream6_IRQHandler(void) {
  DMA1->HIFCR = DMA_HIFCR_CTCIF6;
  DMA1_Stream6->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial2.onDMATxFinish();
}
#endif
// ---------------------------- serial port 3
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT_DMA
void USART3_IRQHandler(void) {
    serialPort_interruptFunction(USART3, SerialPort_RxInterruptTxDma::Serial3);
}

#if MICROHAL_SERIAL_PORT3_DMA_TX_STREAM == 3
void DMA1_Stream3_IRQHandler(void) {
  DMA1->LIFCR = DMA_LIFCR_CTCIF3;
  DMA1_Stream3->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial3.onDMATxFinish();
}
#elif MICROHAL_SERIAL_PORT3_DMA_TX_STREAM == 4
void DMA1_Stream4_IRQHandler(void) {
  DMA1->HIFCR = DMA_HIFCR_CTCIF4;
  DMA1_Stream4->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial3.onDMATxFinish();
}
#endif
#endif
// ---------------------------- serial port 4
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT_DMA
void UART4_IRQHandler(void) {
    serialPort_interruptFunction(UART4, SerialPort_RxInterruptTxDma::Serial4);
}

void DMA1_Stream4_IRQHandler(void) {
  DMA1->HIFCR = DMA_HIFCR_CTCIF4;
  DMA1_Stream4->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial4.onDMATxFinish();
}
#endif
// ---------------------------- serial port 5
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT_DMA
void UART5_IRQHandler(void) {
    serialPort_interruptFunction(UART5, SerialPort_RxInterruptTxDma::Serial5);
}

void DMA1_Stream7_IRQHandler(void) {
  DMA1->HIFCR = DMA_HIFCR_CTCIF7;
  DMA1_Stream7->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial5.onDMATxFinish();
}
#endif
// ---------------------------- serial port 6
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT_DMA
void USART6_IRQHandler(void) {
    serialPort_interruptFunction(USART6, SerialPort_RxInterruptTxDma::Serial6);
}

#if MICROHAL_SERIAL_PORT6_DMA_TX_STREAM == 6
void DMA2_Stream6_IRQHandler(void) {
  DMA2->HIFCR = DMA_HIFCR_CTCIF6;
  DMA2_Stream6->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial6.onDMATxFinish();
}
#elif MICROHAL_SERIAL_PORT3_DMA_TX_STREAM == 7
void DMA1_Stream4_IRQHandler(void) {
  DMA2->HIFCR = DMA_HIFCR_CTCIF7;
  DMA2_Stream7->CR &= ~DMA_SxCR_EN;

  SerialPort_RxInterruptTxDma::Serial6.onDMATxFinish();
}
#endif
#endif

}  // namespace stm32f4xx
}  // namespace microhal
