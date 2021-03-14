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

#include "nvic.h"
#include <exception>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace nvic {

IRQn_Type USARTIrq(const registers::USART* usart) {
    if (usart == registers::usart1) return USART1_IRQn;
    if (usart == registers::usart2) return USART2_IRQn;
#if defined(_MICROHAL_USART3_BASE_ADDRESS)
#if defined(_MICROHAL_USART3_IRQ_COMBINED)
    if (usart == registers::usart3) return USART3_4_5_6_7_8_IRQn;
#else
    if (usart == registers::usart3) return USART3_IRQn;
#endif
#endif
#if defined(_MICROHAL_UART4_BASE_ADDRESS)
    if (usart == registers::uart4) return UART4_IRQn;
#endif
#if defined(_MICROHAL_UART5_BASE_ADDRESS)
    if (usart == registers::uart5) return UART5_IRQn;
#endif
#if defined(USART6) && defined(RCC_APB2ENR_USART6EN)
    if (usart == registers::usart6) return USART6_IRQn;
#endif
#if defined(UART7)
    if (usart == UART7) return UART7_IRQn;
#endif
#if defined(UART8)
    if (usart == UART8) return UART8_IRQn;
#endif
    std::terminate();
}

}  // namespace nvic
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
