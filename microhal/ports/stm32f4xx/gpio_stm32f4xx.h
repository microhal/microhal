/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      GPIO port driver
 *
 * @authors    Michal Karwatowski, Pawel Okas
 * created on: 17-01-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014 - 2018, Pawel Okas
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

#ifndef _MICROHAL_GPIO_STM32F4XX_H_
#define _MICROHAL_GPIO_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "ports/stmCommon/gpio_v1/gpio_stmCommon.h"

namespace microhal {
namespace stm32f4xx {
namespace gpio_detail {
enum class AlternateFunction : uint8_t {
    Timer_1_2 = 1,
    Timer_3_4_5 = 2,
    Serial = 0x07,  //!< Serial
    Serial_4_5_6 = 0x08,
    SPI = 0x05,    //!< Alternate function for SPI 1 and 2
    SPI_3 = 0x06,  //!< Alternate function for SPI 3
    I2C = 0x04,    //!< I2C
    CAN1_2_TIM12_13_14 = 0x09,
    USB = 0x0A
};
}

using GPIO = GPIOCommon<gpio_detail::AlternateFunction>;
}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STM32F4XX_H_
