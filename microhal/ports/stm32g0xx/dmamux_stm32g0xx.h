/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      DMA MUX implementation for STM32G0xx
 *
 * @authors    Pawel Okas
 * created on: 10-06-2021
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

#ifndef _MICROHAL_PORTS_DMAMUX_STM32G0XX_H_
#define _MICROHAL_PORTS_DMAMUX_STM32G0XX_H_

#include "ports/stmCommon/dmamux/dmamux_v1/dmamux_stmCommon.h"

namespace microhal {
namespace stm32g0xx {
namespace DMA {

enum class DMAMUXRequest {  // based on table 52 in RM0444
    dmamux_req_gen0 = 1,
    dmamux_req_gen1,
    dmamux_req_gen2,
    dmamux_req_gen3,
    ADC,
    AES_IN,
    AES_OUT,
    DAC_Channel1,
    DAC_channel2,
    I2C1_RX,
    I2C1_TX,
    I2C2_RX,
    I2C2_TX,
    LPUART_RX,
    LPUART_TX,
    SPI1_RX,
    SPI1_TX,
    SPI2_RX,
    SPI2_TX,
    TIM1_CH1,
    TIM1_CH2,
    TIM1_CH3,
    TIM1_CH4,
    TIM1_TRIG_COM,
    TIM1_UP,
    TIM2_CH1,
    TIM2_CH2,
    TIM2_CH3,
    TIM2_CH4,
    TIM2_TRIG,
    TIM2_UP,
    TIM3_CH1,
    TIM3_CH2,
    TIM3_CH3,
    TIM3_CH4,
    TIM3_TRIG,
    TIM3_UP,
    TIM6_UP,
    TIM7_UP,
    TIM15_CH1,
    TIM15_CH2,
    TIM15_TRIG_COM,
    TIM15_UP,
    TIM16_CH1,
    TIM16_COM,
    TIM16_UP,
    TIM17_CH1,
    TIM17_COM,
    TIM17_UP,
    USART1_RX,
    USART1_TX,
    USART2_RX,
    USART2_TX,
    USART3_RX,
    USART3_TX,
    USART4_RX,
    USART4_TX,
    UCPD1_RX,
    UCPD1_TX,
    UCPD2_RX,
    UCPD2_TX,
    I2C3_RX,
    I2C3_TX,
    LPUART2_RX,
    LPUART2_TX,
    SPI3_RX,
    SPI3_TX,
    TIM4_CH1,
    TIM4_CH2,
    TIM4_CH3,
    TIM4_CH4,
    TIM4_TRIG,
    TIM4_UP,
    USART5_RX,
    USART5_TX,
    USART6_RX,
    USART6_TX
};

using DMAMUX = DMAMUXBase<DMAMUXRequest>;

static_assert(static_cast<size_t>(DMAMUXRequest::ADC) == 5);
static_assert(static_cast<size_t>(DMAMUXRequest::USART6_TX) == 77);

}  // namespace DMA
}  // namespace stm32g0xx
}  // namespace microhal

#endif /* _MICROHAL_PORTS_DMAMUX_STM32G0XX_H_ */
