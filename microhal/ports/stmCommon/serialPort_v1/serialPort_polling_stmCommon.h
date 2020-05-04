/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 25-08-2016
 * last modification: 25-08-2016
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

#ifndef _MICROHAL_PORTS_SERIALPORT_POLLING_STMCOMMON_H_
#define _MICROHAL_PORTS_SERIALPORT_POLLING_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include "serialPort_stmCommon.h"
#if _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SERIAL_PORT(POLLING)                    // Check if driver is enabled in microhal port config
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "stmCommonDefines.h"
/* **************************************************************************************************************************************************
 * CLASS
 */

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class SerialPort_polling : public _MICROHAL_ACTIVE_PORT_NAMESPACE::SerialPort {
 public:
#ifdef MICROHAL_USE_SERIAL_PORT1_POLLING
    static SerialPort_polling Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_POLLING
    static SerialPort_polling Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_POLLING
    static SerialPort_polling Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_POLLING
    static SerialPort_polling Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_POLLING
    static SerialPort_polling Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_POLLING
    static SerialPort_polling Serial6;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT7_POLLING
    static SerialPort_polling Serial7;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT8_POLLING
    static SerialPort_polling Serial8;
#endif

    bool open(OpenMode mode) noexcept {
        if (isOpen() || (mode > 0x03)) return false;
        auto cr1 = usart.cr1.volatileLoad();
        cr1 |= (mode << 2);
        cr1.UE.set();
        usart.cr1.volatileStore(cr1);
        return true;
    }

    /**
     *
     * @param c
     * @return
     */
    bool putChar(char c) noexcept final {
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
        while (!(usart.isr.volatileLoad().TXE)) {
        }
        usart.tdr.volatileStore(c);
#else
        while (!(usart.sr.volatileLoad().TXE)) {
        }
        usart.dr.volatileStore((uint32_t)c);
#endif
        return true;
    }

    /**
     *
     * @return
     */
    bool getChar(char &c) noexcept final {
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
        while (!(usart.isr.volatileLoad().RXNE)) {
        }
        c = (uint32_t)usart.rdr.volatileLoad();
#else
        while (!(usart.sr.volatileLoad().RXNE)) {
        }
        c = usart.dr.volatileLoad().get();
#endif
        return true;
    }
    /**@brief This function write data to serial port buffer.
     *
     * @param[in] data - pointer to buffer where data are stored.
     * @param[in] maxSize - number of bytes to copy.
     * @return number of bytes that was copy to buffer.
     */
    size_t write(const char *data, size_t length) noexcept final {
        size_t writeByte = 0;

        for (; writeByte < length; writeByte++) {
            putChar(*data++);
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
        (void)timeout;
        size_t tmp = length;
        while (length--) {
            getChar(*data++);
        }
        return tmp;
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
        (void)dir;
        return true;
    }
    /**
     *
     * @return
     */
    size_t inputQueueSize() const noexcept final { return 1; }
    /**
     *
     * @return
     */
    size_t outputQueueSize() const noexcept final { return 1; }

    size_t availableBytes() const noexcept final {
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX)
        if ((usart.isr.volatileLoad().RXNE)) {
#else
        if (!(usart.sr.volatileLoad().RXNE)) {
#endif
            return 1;
        }
        return 0;
    }

    bool waitForWriteFinish(std::chrono::milliseconds timeout) const noexcept final {
        (void)timeout;
        return true;
    }

 private:
    SerialPort_polling(registers::USART &usart);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SERIAL_PORT(POLLING)
#endif  // _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1
#endif  // _MICROHAL_PORTS_SERIALPORT_POLLING_STMCOMMON_H_
