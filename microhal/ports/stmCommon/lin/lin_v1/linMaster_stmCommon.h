/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 13-03-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#ifndef _MICROHAL_PORTS_STMCOMMON_LIN_V1_LINMASTER_STMCOMMON_H_
#define _MICROHAL_PORTS_STMCOMMON_LIN_V1_LINMASTER_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include <ports/stmCommon/driverConfiguration_stmCommon.h>
#if _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU

/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "../../stmCommonDefines.h"  // required for _MICROHAL_ACTIVE_PORT_NAMESPACE and _MICROHAL_INCLUDE_PORT_CONFIG
#include "lin/linMaster.h"
#include "microhal_semaphore.h"
#include _MICROHAL_INCLUDE_PORT_CONFIG
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX) || defined(MCU_TYPE_STM32G0XX)
#include "../../registers/usartRegisters_v2.h"
#else
#include "../../registers/usartRegisters_v1.h"
#endif  // defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX) || defined(MCU_TYPE_STM32G0XX)

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART6_IRQHandler(void);
}

class LINMaster final : public lin::LINMaster {
    enum Mode { Idle = 0b000, WaitForBreak = 0b100, Transmitter = 0b001, Receiver = 0b010 };

 public:
#if MICROHAL_USE_LIN1 == 2
    static LINMaster lin1;
#endif
#if MICROHAL_USE_LIN2 == 2
    static LINMaster lin2;
#endif
#if MICROHAL_USE_LIN6 == 2
    static LINMaster lin6;
#endif

    bool init(uint32_t baudRate, uint32_t interruptPriority);
    void deinit();

    void enable();
    bool isEnabled() const { return usart.cr1.volatileLoad().UE; }
    void disable();

 private:
    registers::USART &usart;
    os::Semaphore semaphore{};
    Error status = Error::None;
    uint8_t mode = Idle;
    uint8_t rxBuffer[12];
    uint8_t rxBufferIter = 0;
    uint8_t waitForBytes = -1;
    uint8_t *txPtr = nullptr;
    uint8_t *txEndPtr = nullptr;

    LINMaster(registers::USART &usart) : usart(usart) {}
    LINMaster(const LINMaster &) = delete;
    LINMaster(LINMaster &&) = delete;
    LINMaster &operator=(const LINMaster &) = delete;

    void sendBreak();
    Error write(std::span<uint8_t> data, std::chrono::milliseconds timeout) final;
    Error request_impl(lin::Frame &frame, std::chrono::milliseconds timeout) final;

    void enableInterrupt(uint32_t priority);
    void disableInterrupt();
    void interrupt();

    friend void USART1_IRQHandler(void);
    friend void USART2_IRQHandler(void);
    friend void USART6_IRQHandler(void);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1
#endif  // _MICROHAL_PORTS_STMCOMMON_LIN_V1_LINMASTER_STMCOMMON_H_
