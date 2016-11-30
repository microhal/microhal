/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      STM32F4xx serial port common functions implementation.
 *
 * @authors    Pawel Okas
 * created on: 16-11-2016
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

#ifndef _MICROHAL_SERIALPORT_COMMON_BUFFERED_STM32F4XX_H_
#define _MICROHAL_SERIALPORT_COMMON_BUFFERED_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "../serialPort_stm32f4xx.h"

#include <chrono>

#include "buffers/cyclicBuffer.h"
#include "microhal_semaphore.h"


namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
template <class Derived>
class SerialPort_BufferedBase: public stm32f4xx::SerialPort {
public:
	SerialPort_BufferedBase(USART_TypeDef & usart, char * const rxData, size_t rxDataSize, char * const txData, size_t txDataSize) noexcept
		: SerialPort(usart), rxBuffer(rxData, rxDataSize), txBuffer(txData, txDataSize) {}
//--------------------------------------------- functions ---------------------------------------//
    /**
     *
     * @param c
     * @return
     */
    bool putChar(char c) noexcept final {
    	atomicSectionBegin();
        if (txBuffer.append(c)){
        	atomicSectionEnd();
        	startTransmission();
            return true;
        }
        atomicSectionEnd();
        return false;
    }
    /**
     *
     * @return
     */
    bool getChar(char &c) noexcept final {
        if (rxBuffer.isNotEmpty()) {
        	atomicSectionBegin();
            c = rxBuffer.get();
            atomicSectionEnd();
            return true;
        } else {
            return false;
        }
    }
    /**@brief This function write data to serial port buffer.
     *
     * @param[in] data - pointer to buffer where data are stored.
     * @param[in] maxSize - number of bytes to copy.
     * @return number of bytes that was copy to buffer.
     */
    size_t write(const char *data, size_t length) noexcept final {
    	atomicSectionBegin();
		size_t writeByte = txBuffer.write(data, length);
		atomicSectionEnd();
		if(writeByte != 0) startTransmission();

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
    size_t read(char *data, size_t length, std::chrono::milliseconds timeout) final {
    	updateRxBuffer();
        auto len = rxBuffer.read(data, length);
    	if (len < length && timeout != std::chrono::milliseconds::zero()) {
    		configureRxWait(length - len);
    		if (rxSemaphore.wait(timeout) == false) {
    			waitForBytes = 0;
    			updateRxBuffer();
    		}
    		len += rxBuffer.read(data + len, length - len);
    	}

        return len;
    }

    bool waitForWriteFinish(std::chrono::milliseconds timeout) const noexcept final {
    	if (txBuffer.isNotEmpty()) {
    		txWait = true;
    		return txFinish.wait(timeout);
    	}
    	return true;
    }
	/**@brief This function clear buffer specified in dir parameter. If dir == Input function will
	 *        flush rxBuffer, if dir == Output then txBuffer will be flushed.
	 *        If dir == AllDirections both buffers will be cleared.
	 *
	 * @param[in] dir - buffer direction to be cleared
	 * @retval true - if buffer was cleared successful
	 * @retval false - if an error occurred
	 */
	bool clear(Direction dir) noexcept final {
	    switch (dir) {
	    case SerialPort::Input:
	        rxBuffer.flush();
	        break;
	    case SerialPort::Output:
	        txBuffer.flush();
	        break;
	    case SerialPort::AllDirections:
	        rxBuffer.flush();
	        txBuffer.flush();
	        break;
	    default:
	        return false;
	    }
	    return true;
	}

    size_t inputQueueSize() const noexcept final { return rxBuffer.getSize(); }
    size_t outputQueueSize() const noexcept final { return txBuffer.getSize(); }
    size_t availableBytes() const noexcept override { return rxBuffer.getLength(); }
 protected:
	CyclicBuffer<char> rxBuffer;
    CyclicBuffer<char> txBuffer;
    mutable bool txWait = false;
    mutable microhal::os::Semaphore txFinish;
    microhal::os::Semaphore rxSemaphore;
    size_t waitForBytes = 0;

    void startTransmission() {
    	static_cast<Derived*>(this)->startTransmission_impl();
    }

    void updateRxBuffer() {
    	static_cast<Derived*>(this)->updateRxBuffer_impl();
    }

    void configureRxWait(size_t bytesToWait) {
    	static_cast<Derived*>(this)->configureRxWait_impl(bytesToWait);
    }

    void atomicSectionBegin() const { __disable_irq(); }
    void atomicSectionEnd() const { __enable_irq(); }
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_COMMON_BUFFERED_STM32F4XX_H_
