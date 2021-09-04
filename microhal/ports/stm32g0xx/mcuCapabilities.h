/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-02-2021
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

#ifndef _MICROHAL_PORTS_STM32G0XX_MCUCAPABILITIES_H_
#define _MICROHAL_PORTS_STM32G0XX_MCUCAPABILITIES_H_

#include <cstdint>
#include "mcuDefines.h"

namespace microhal {
namespace stm32g0xx {
namespace hardwareLimits {

#ifdef _MICROHAL_STM32G0XX_STM32G071xx

#define _MICROHAL_STM32G0XX_HAS_AES1 0

#define _MICROHAL_STM32G0XX_HAS_I2C1 1
#define _MICROHAL_STM32G0XX_HAS_I2C2 1

#define _MICROHAL_STM32G0XX_HAS_USART1 1
#define _MICROHAL_STM32G0XX_HAS_USART2 1
#define _MICROHAL_STM32G0XX_HAS_USART3 1
#define _MICROHAL_STM32G0XX_HAS_USART4 1
#define _MICROHAL_STM32G0XX_HAS_USART5 0
#define _MICROHAL_STM32G0XX_HAS_USART6 0

#define _MICROHAL_STM32G0XX_HAS_RTC1 1

constexpr const uint32_t spiMaxCLKFrequency = 32'000'000;  // in [Hz]
#define _MICROHAL_STM32G0XX_HAS_SPI1 1
#define _MICROHAL_STM32G0XX_HAS_SPI2 1

#endif  // _MICROHAL_STM32G0XX_STM32G071xx

#ifdef _MICROHAL_STM32G0XX_STM32G070xx

#define _MICROHAL_STM32G0XX_HAS_AES1 0

#define _MICROHAL_STM32G0XX_HAS_I2C1 1
#define _MICROHAL_STM32G0XX_HAS_I2C2 1

#define _MICROHAL_STM32G0XX_HAS_USART1 1
#define _MICROHAL_STM32G0XX_HAS_USART2 1
#define _MICROHAL_STM32G0XX_HAS_USART3 1
#define _MICROHAL_STM32G0XX_HAS_USART4 1
#define _MICROHAL_STM32G0XX_HAS_USART5 0
#define _MICROHAL_STM32G0XX_HAS_USART6 0

#define _MICROHAL_STM32G0XX_HAS_RTC1 1

constexpr const uint32_t spiMaxCLKFrequency = 32'000'000;  // in [Hz]
#define _MICROHAL_STM32G0XX_HAS_SPI1 1
#define _MICROHAL_STM32G0XX_HAS_SPI2 1

#endif  // _MICROHAL_STM32G0XX_STM32G070xx

}  // namespace hardwareLimits
}  // namespace stm32g0xx
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STM32G0XX_MCUCAPABILITIES_H_ */
