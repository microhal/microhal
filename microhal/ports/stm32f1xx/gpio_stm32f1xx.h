/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2019, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_PORTS_GPIO_STM32F1XX_H_
#define _MICROHAL_PORTS_GPIO_STM32F1XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "ports/stmCommon/gpio_v2/gpio_stmCommon.h"
/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace stm32f1xx {

namespace gpio_detail {
typedef enum : uint8_t {
    AF2 = 0x02,     //! TIM1, TIM3, TIM15, TIM16
    AF4 = 0x04,     //!< I2C, TIM1
    Serial = 0x07,  //!< Serial
    AF5 = 0x05,
    AF6 = 0x06,
    SPI_2 = 0x05,
    SPI_3 = 0x06,
    // I2C = 0x04,  //!< I2C
    CAN_TIM1_TIM15 = 0x09,
    AF10 = 0x0A,
    AF11 = 0x0B
} AlternateFunction;
}

using GPIO = GPIOCommon<gpio_detail::AlternateFunction>;

}  // namespace stm32f1xx
}  // namespace microhal

#endif  // _MICROHAL_PORTS_GPIO_STM32F1XX_H_
