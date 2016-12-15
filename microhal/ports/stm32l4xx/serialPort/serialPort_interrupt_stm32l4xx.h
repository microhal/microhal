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

#ifndef SERIALPORT_INTERRUPT_STM32L4XX_H_
#define SERIALPORT_INTERRUPT_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../serialPort_stm32l4xx.h"
#include "buffers/cyclicBuffer.h"

namespace microhal {
namespace stm32l4xx {
/* ************************************************************************************************
 * EXTERN DECLARATION
 */
extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void LPUART1_IRQHandler(void);
}
class SerialPort_interrupt: public stm32l4xx::SerialPort {
public:
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
    static SerialPort_interrupt Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
    static SerialPort_interrupt Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
    static SerialPort_interrupt Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT
    static SerialPort_interrupt Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT
    static SerialPort_interrupt Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT
    static SerialPort_interrupt Serial6;
#endif
//--------------------------------------------- functions ---------------------------------------//
    bool open(OpenMode mode) noexcept;
    /**
     *
     * @param c
     * @return
     */
    bool putChar(char c) noexcept final {
    	__disable_irq();
        if (txBuffer.append(c)){
        	startSending();
        	__enable_irq();
            return true;
        }
        __enable_irq();
        return false;
    }
    /**
     *
     * @return
     */
    bool getChar(char &c) noexcept final {
        if (rxBuffer.isNotEmpty()) {
            c = rxBuffer.get();
            return true;
        } else {
            return false;
        }
    }
    /**
     *
     * @param[in] msec
     * @return
     */
    bool waitForWriteFinish(std::chrono::milliseconds timeout) const noexcept final {
        //todo timeout
        while (1){
        	__disable_irq();
        	if(	txBuffer.isEmpty() ){
        		__enable_irq();
        		return true;
        	}
        	__enable_irq();
        	timeout--;
        }
        __enable_irq();
        return false;
    }
    /**@brief This function write data to serial port buffer.
     *
     * @param[in] data - pointer to buffer where data are stored.
     * @param[in] maxSize - number of bytes to copy.
     * @return number of bytes that was copy to buffer.
     */
    size_t write(const char *data, size_t length) noexcept final {
    	size_t writeByte = 0;

    	if(length > 0){
    		__disable_irq();
			for (; writeByte < length; writeByte++) {
				if (txBuffer.append(*data++) == false) break;
			}
			if(writeByte != 0) startSending();
			__enable_irq();
    	}
        return writeByte;
    }
    /**@brief This function copy read data from serial buffer to data pointer.
     *        If in buffer are less bytes than maxSize then function copy all bytes storage in buffer
     *        and return bytes count.
     *
     * @param[out] data - pointer to buffer where read data will be copy
     * @param[in] maxSize - size of data to copy.
     * @return number of bytes stored in data buffer.
     */
    size_t read(char *data, size_t length, std::chrono::milliseconds timeout) noexcept final {
        size_t len = rxBuffer.getLength();

        if (len == 0) return 0;

        if (len > length) {
            len = length;
        } else {
            length = len;
        }

        while (length--) {
            *data++ = rxBuffer.get();
        }

        return len;
    }
    bool clear(Direction dir) noexcept final;
    /**
     *
     * @return
     */
    size_t inputQueueSize() const noexcept final {
        return rxBuffer.getSize();
    }
    /**
     *
     * @return
     */
    size_t outputQueueSize() const noexcept final {
        return txBuffer.getSize();
    }
    /**
     *
     * @return
     */
    size_t availableBytes() const noexcept final {
        return rxBuffer.getLength();
    }
private:
//------------------------------------------- variables -----------------------------------------//
    CyclicBuffer<char> rxBuffer;
    CyclicBuffer<char> txBuffer;
//------------------------------------------- constructors --------------------------------------//
    inline SerialPort_interrupt(USART_TypeDef &usart, char * const rxData, char * const txData, size_t rxDataSize, size_t txDataSize);

    //virtual ~SerialPort_interrupt(){
    //}
//--------------------------------------------- functions ---------------------------------------//
    void startSending() {
        usart.CR1 |= USART_CR1_TXEIE;
    }
//------------------------------------------- friends -------------------------------------------//
    friend inline void __SerialPort_USART_interruptFunction(USART_TypeDef * const usart,
            SerialPort_interrupt &serialObject) __attribute__((always_inline));

    friend void USART1_IRQHandler(void);
    friend void USART2_IRQHandler(void);
    friend void USART3_IRQHandler(void);
    friend void USART4_IRQHandler(void);
    friend void USART5_IRQHandler(void);
    friend void LPUART1_IRQHandler(void);
}
;
//*************************************************************************************************
// INLINE FUNCTIONS
//*****************
SerialPort_interrupt::SerialPort_interrupt(USART_TypeDef &usart, char * const rxData, char * const txData, size_t rxDataSize, size_t txDataSize) :
        stm32l4xx::SerialPort(usart), rxBuffer(rxData, rxDataSize), txBuffer(txData, txDataSize) {
    uint32_t rccEnableFlag;

    switch (reinterpret_cast<uint32_t>(&usart)) {
    case reinterpret_cast<uint32_t>(USART1):
        rccEnableFlag = RCC_APB2ENR_USART1EN;
        NVIC_EnableIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, 0);
        break;
    case reinterpret_cast<uint32_t>(USART2):
        rccEnableFlag = RCC_APB1ENR1_USART2EN;
        NVIC_EnableIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, 0);
        break;
    case reinterpret_cast<uint32_t>(USART3):
        rccEnableFlag = RCC_APB1ENR1_USART3EN;
        NVIC_EnableIRQ (USART3_IRQn);
        NVIC_SetPriority(USART3_IRQn, 0);
        break;
    case reinterpret_cast<uint32_t>(UART4):
        rccEnableFlag = RCC_APB1ENR1_UART4EN;
        NVIC_EnableIRQ (UART4_IRQn);
        NVIC_SetPriority(UART4_IRQn, 0);
        break;
    case reinterpret_cast<uint32_t>(UART5):
        rccEnableFlag = RCC_APB1ENR1_UART5EN;
        NVIC_EnableIRQ (UART5_IRQn);
        NVIC_SetPriority(UART5_IRQn, 0);
        break;
    case reinterpret_cast<uint32_t>(LPUART1):
        rccEnableFlag = RCC_APB1ENR2_LPUART1EN;
        NVIC_EnableIRQ (LPUART1_IRQn);
        NVIC_SetPriority(LPUART1_IRQn, 0);
        break;
    }
    if (reinterpret_cast<uint32_t>(&usart) == reinterpret_cast<uint32_t>(USART1))
        RCC->APB2ENR |= rccEnableFlag;
    else if(reinterpret_cast<uint32_t>(&usart) == reinterpret_cast<uint32_t>(LPUART1))
    	RCC->APB1ENR2 |= rccEnableFlag;
    else
        RCC->APB1ENR1 |= rccEnableFlag;

}
} // namespace stm32l4xx
} // namespace microhal


/* **************************************************************************************************************************************************
 * CLASS
 */


#endif  // SERIALPORT_INTERRUPT_STM32L4XX_H_
