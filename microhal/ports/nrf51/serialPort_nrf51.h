/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 05-12-2017
 * last modification: 05-12-2017
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

#ifndef _MICROHAL_SERIALPORT_NRF51_H_
#define _MICROHAL_SERIALPORT_NRF51_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "device/nrf.h"
#include "interfaces/serialPort_interface.h"

namespace microhal {
namespace nrf51 {

/* **************************************************************************************************************************************************
 * CLASS
 */
class SerialPort : public microhal::SerialPort {
 public:
#if (defined MICROHAL_USE_SERIAL_PORT1_POLLING) || (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT1_DMA)
    static SerialPort &Serial1;
#endif

    bool open(OpenMode mode) noexcept override = 0;
    bool isOpen(void) const noexcept final { return uart.ENABLE == UART_ENABLE_ENABLE_Enabled; }
    void close() noexcept final { uart.ENABLE = UART_ENABLE_ENABLE_Disabled; }

    bool setBaudRate(uint32_t baudrate) noexcept final {
        auto baudToRegValue = [](uint32_t baudrate) -> uint32_t {
            switch (baudrate) {
                case Baud1200:
                    return 0x0004F000;  // 1200 baud (actual rate: 1205)
                case Baud2400:
                    return 0x0009D000;  // 2400 baud (actual rate: 2396)
                case Baud4800:
                    return 0x0013B000;  // 4800 baud (actual rate: 4808)
                case Baud9600:
                    return 0x00275000;  // 9600 baud (actual rate: 9598)
                case 14400:
                    return 0x003AF000;  // 14400 baud (actual rate: 14401)
                case Baud19200:
                    return 0x004EA000;  // 19200 baud (actual rate: 19208)
                case 28800:
                    return 0x0075C000;  // 28800 baud (actual rate: 28777)
                case Baud38400:
                    return 0x009D0000;  // 38400 baud (actual rate: 38369)
                case Baud57600:
                    return 0x00EB0000;  // 57600 baud (actual rate: 57554)
                case 76800:
                    return 0x013A9000;  // 76800 baud (actual rate: 76923)
                case Baud115200:
                    return 0x01D60000;  // 115200 baud (actual rate: 115108)
                case Baud230400:
                    return 0x03B00000;  // 230400 baud (actual rate: 231884)
                case 250000:
                    return 0x04000000;  // 250000 baud
                case Baud460800:
                    return 0x07400000;  // 460800 baud (actual rate: 457143)
                case Baud921600:
                    return 0x0F000000;  // 921600 baud (actual rate: 941176)
                case Baud1M:
                    return 0x10000000;  // 1Mega baud
                default:
                    return 0;  // unsupported baud
            }
        };

        auto value = baudToRegValue(baudrate);
        if (value) {
            uart.BAUDRATE = value;
            return true;
        }
        return false;
    }
    uint32_t getBaudRate() const noexcept final {
        auto regValueToBaud = [&]() -> uint32_t {
            switch (uart.BAUDRATE) {
                case 0x0004F000:
                    return 1200;  // 1200 baud (actual rate: 1205)
                case 0x0009D000:
                    return 2400;  // 2400 baud (actual rate: 2396)
                case 0x0013B000:
                    return 4800;  // 4800 baud (actual rate: 4808)
                case 0x00275000:
                    return 9600;  // 9600 baud (actual rate: 9598)
                case 0x003AF000:
                    return 14400;  // 14400 baud (actual rate: 14401)
                case 0x004EA000:
                    return 19200;  // 19200 baud (actual rate: 19208)
                case 0x0075C000:
                    return 28800;  // 28800 baud (actual rate: 28777)
                case 0x009D0000:
                    return 38400;  // 38400 baud (actual rate: 38369)
                case 0x00EB0000:
                    return 57600;  // 57600 baud (actual rate: 57554)
                case 0x013A9000:
                    return 76800;  // 76800 baud (actual rate: 76923)
                case 0x01D60000:
                    return 115200;  // 115200 baud (actual rate: 115108)
                case 0x03B00000:
                    return 230400;  // 230400 baud (actual rate: 231884)
                case 0x04000000:
                    return 250000;  // 250000 baud
                case 0x07400000:
                    return 460800;  // 460800 baud (actual rate: 457143)
                case 0x0F000000:
                    return 921600;  // 921600 baud (actual rate: 941176)
                case 0x10000000:
                    return 1000000;  // 1Mega baud
                default:
                    std::terminate();  // unsupported value
            }
        };

        return regValueToBaud();
    }
    bool setParity(SerialPort::Parity parity) noexcept final {
        switch (parity) {
            case Parity::NoParity:
                uart.CONFIG &= ~(UART_CONFIG_PARITY_Included << UART_CONFIG_PARITY_Pos);
                break;
            case Parity::EvenParity:
                uart.CONFIG |= UART_CONFIG_PARITY_Included << UART_CONFIG_PARITY_Pos;
                break;
            default:
                return false;
        }
        return true;
    }
    bool setStopBits(SerialPort::StopBits stopBits) noexcept final {
        if (stopBits == StopBits::OneStop) return true;
        return false;
    }
    bool setDataBits(SerialPort::DataBits dataBits) noexcept final {
        if (dataBits == DataBits::Data8) return true;
        return false;
    }

 protected:
    //------------------------------------------- variables -----------------------------------------//
    NRF_UART_Type &uart;
//------------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SerialPort(NRF_UART_Type &uart) noexcept : uart(uart) {
    }

    void enableInterrupt(uint32_t priority) {
        switch (reinterpret_cast<uint32_t>(&uart)) {
            case reinterpret_cast<uint32_t>(NRF_UART0):
                NVIC_EnableIRQ(UART0_IRQn);
                NVIC_SetPriority(UART0_IRQn, priority);
                break;
            default:
                std::terminate();  // critical error
        }
    }
};

}  // namespace nrf51
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_NRF51_H_
