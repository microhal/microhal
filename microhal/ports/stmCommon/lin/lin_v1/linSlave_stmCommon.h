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

#ifndef _MICROHAL_PORTS_STMCOMMON_LIN_V1_LINSLAVE_STMCOMMON_H_
#define _MICROHAL_PORTS_STMCOMMON_LIN_V1_LINSLAVE_STMCOMMON_H_
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
#include "../../clockManager/usartClock.h"
#include "../../stmCommonDefines.h"
#include "lin/linSlave.h"
#include "microhal_semaphore.h"
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX) || defined(MCU_TYPE_STM32G0XX)
#include "../../registers/usartRegisters_v2.h"
#else
#include "../../registers/usartRegisters_v1.h"
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART6_IRQHandler(void);
}

class LINSlave final : public lin::LINSlave {
    enum Mode : uint8_t { WaitForBreak, Transmitter, Receiver };

 public:
#if MICROHAL_USE_LIN1 == 1
    static LINSlave lin1;
#endif
#if MICROHAL_USE_LIN2 == 1
    static LINSlave lin2;
#endif
#if MICROHAL_USE_LIN6 == 1
    static LINSlave lin6;
#endif

    bool init(uint32_t baudRate, uint32_t interruptPriority);
    void deinit();

    void enable();
    bool isEnabled() const { return usart.cr1.volatileLoad().UE; }
    void disable();

    Error waitForBreak(std::chrono::milliseconds timeout);

 private:
    registers::USART &usart;
    microhal::os::Semaphore txDone{};
    Error status = Error::None;
    uint8_t mode = WaitForBreak;
    uint8_t rxBuffer[12];
    int8_t rxBufferIter = 0;
    int8_t waitForBytes = -1;
    uint8_t *txPtr = nullptr;
    uint8_t *txEndPtr = nullptr;

    LINSlave(registers::USART &usart) : usart(usart) {}
    LINSlave(const LINSlave &) = delete;
    LINSlave(LINSlave &&) = delete;
    LINSlave &operator=(const LINSlave &) = delete;

    Error write(std::span<uint8_t> data, std::chrono::milliseconds timeout) final;
    Error readHeader_to(lin::Header &header, std::chrono::milliseconds timeout) final;
    Error read_to(uint8_t *data, uint_fast8_t length, std::chrono::milliseconds timeout) final;

    void enableInterrupt(uint32_t priority);
    void disableInterrupt();
    void interrupt();

    friend void USART1_IRQHandler(void);
    friend void USART2_IRQHandler(void);
    friend void USART6_IRQHandler(void);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif
#endif /* _MICROHAL_PORTS_STMCOMMON_LIN_V1_LINSLAVE_STMCOMMON_H_ */
