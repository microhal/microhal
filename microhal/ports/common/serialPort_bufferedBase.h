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

#ifndef _MICROHAL_SERIALPORT_COMMON_BUFFERED_H_
#define _MICROHAL_SERIALPORT_COMMON_BUFFERED_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <chrono>

#include "buffers/cyclicBuffer.h"
#include "interfaces/serialPort_interface.h"
#include "microhal_semaphore.h"

#if defined(VENDOR_STMICROELECTRONICS)
#include "ports/stmCommon/stmCommonDefines.h"

#include "ports/stmCommon/serialPort_v1/serialPort_stmCommon.h"

#endif

namespace microhal {
namespace common {

namespace detail {
template <typename T>
class has_clearImpl {
 private:
    template <typename C>
    static constexpr auto test(C *c) -> typename std::is_same<bool, decltype(c->clearImpl(SerialPort::Direction::AllDirections))>::type {
        return {};
    }
    template <typename C>
    static constexpr std::false_type test(...) {
        return {};
    }

 public:
    static constexpr bool value = test<T>(nullptr);
};

}  // namespace detail
/* ************************************************************************************************
 * CLASS
 */
template <class Derived>
class SerialPort_BufferedBase : public _MICROHAL_ACTIVE_PORT_NAMESPACE::SerialPort {
 public:
    SerialPort_BufferedBase(registers::USART &usart, char *const rxData, size_t rxDataSize, char *const txData, size_t txDataSize) noexcept
        : SerialPort(usart), rxBuffer(rxData, rxDataSize), txBuffer(txData, txDataSize), txFinish(), rxSemaphore() {}
    //--------------------------------------------- functions ---------------------------------------//
    /**
     *
     * @param c
     * @return
     */
    int putChar(char c) noexcept final {
        atomicSectionBegin();
        if (txBuffer.append(c)) {
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
    int getChar(char &c) noexcept final {
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
    ssize_t write(const char *data, size_t length) noexcept final {
        atomicSectionBegin();
        size_t writeByte = txBuffer.write(data, length);
        atomicSectionEnd();
        if (writeByte != 0) startTransmission();

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
    ssize_t read(char *data, size_t length, std::chrono::milliseconds timeout) final {
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
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX) || defined(MCU_TYPE_STM32G0XX)
        if (txBuffer.isNotEmpty() || !(usart.isr.volatileLoad().TXE)) {
#else
        if (txBuffer.isNotEmpty() || !(usart.sr.volatileLoad().TXE)) {
#endif
            txWait = true;
            if (txFinish.wait(timeout) == false) {
                txWait = false;
                return false;
            }
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
        if constexpr (detail::has_clearImpl<Derived>::value) {
            return static_cast<Derived *>(this)->clearImpl(dir);
        } else {
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
    }

    size_t inputQueueSize() const noexcept final { return rxBuffer.getSize(); }
    size_t outputQueueSize() const noexcept final { return txBuffer.getSize(); }
    ssize_t availableBytes() const noexcept override { return rxBuffer.getLength(); }

 protected:
    CyclicBuffer<char> rxBuffer;
    CyclicBuffer<char> txBuffer;
    mutable bool volatile txWait = false;
    mutable microhal::os::Semaphore txFinish;
    microhal::os::Semaphore rxSemaphore;
    volatile size_t waitForBytes = 0;

    void startTransmission() { static_cast<Derived *>(this)->startTransmission_impl(); }

    void updateRxBuffer() { static_cast<Derived *>(this)->updateRxBuffer_impl(); }

    void configureRxWait(size_t bytesToWait) { static_cast<Derived *>(this)->configureRxWait_impl(bytesToWait); }

    void atomicSectionBegin() const { __disable_irq(); }
    void atomicSectionEnd() const { __enable_irq(); }
};

}  // namespace common
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_COMMON_BUFFERED_STM32F4XX_H_
