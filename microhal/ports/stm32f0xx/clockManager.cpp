/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel
 * created on: 23-10-2018
 * last modification: 23-10-2018
 *
 * @copyright Copyright (c) 2018, microHAL
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "clockManager.h"

namespace microhal {
namespace stm32f0xx {

uint32_t ClockManager::USARTFrequency(uint8_t number) {
    UsartClockSource usartClockSource = USARTClockSource(number);

    switch (usartClockSource) {
        case PCLK:
            return APB1Frequency();
        case SYSCLK:
            return SYSCLK::frequency();
        case LSE:
            while (1)
                ;
            return 0;  // LSE::frequency();
            break;
        case HSI:
            return HSI::frequency();
    }
    std::terminate();
}

ClockManager::UsartClockSource ClockManager::USARTClockSource(uint8_t number) {
    uint32_t pos;
    if (number == 1)
        pos = RCC_CFGR3_USART1SW_Pos;
    else if (number == 2)
        // pos = RCC_CFGR3_USART2SW_Pos;
        return UsartClockSource::PCLK;
#if defined(USART3)
    else if (number == 3)
        // pos = RCC_CFGR3_USART3SW_Pos;
        return UsartClockSource::PCLK;
#endif
    else {
        std::terminate();
    }
    return static_cast<UsartClockSource>((RCC->CFGR3 >> pos) & 0b11);
}

ClockManager::Frequency ClockManager::APB1Frequency() {
    const uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE_Msk) >> RCC_CFGR_PPRE_Pos;
    if (ppre1 & 0b100) {
        const uint32_t div[] = {2, 4, 8, 16};
        return AHBFrequency() / div[ppre1 & 0b011];
    } else {
        return AHBFrequency();
    }
}

uint32_t ClockManager::AHBFrequency() noexcept {
    const uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;
    if (hpre & 0b1000) {
        const uint32_t div[] = {2, 4, 8, 16, 64, 128, 256, 512};
        return SYSCLK::frequency() / div[hpre & 0b0111];
    } else {
        return SYSCLK::frequency();
    }
}

}  // namespace stm32f0xx
}  // namespace microhal
