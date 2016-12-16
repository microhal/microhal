/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 20-07-2016
 * last modification: 20-07-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */

#include "gpio_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {

void GPIO::pinInitialize(const Port port, const uint_fast8_t pin,
                         const PinConfiguration config) {
  // enable clock
  switch (port) {
    case PortA:
      RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
      break;
    case PortB:
      RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
      break;
    case PortC:
      RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
      break;
    case PortD:
      RCC->AHBENR |= RCC_AHBENR_GPIODEN;
      break;
    case PortE:
      RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
      break;
    case PortF:
      RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
      break;
  }

  // set mode -> config.mode is split to 2 part 4MSB bit
  //      contain alternate function and 4LSB bit contain mode
  reinterpret_cast<volatile GPIO_TypeDef *>(port)->MODER |= (0x03 & config.mode)
                                                            << (pin * 2);
  // set type
  reinterpret_cast<volatile GPIO_TypeDef *>(port)->OTYPER |= config.type << pin;
  // set pullup
  reinterpret_cast<volatile GPIO_TypeDef *>(port)->PUPDR |= config.pull
                                                            << (pin * 2);
  // set speed
  reinterpret_cast<volatile GPIO_TypeDef *>(port)->OSPEEDR |= config.speed
                                                              << (pin * 2);

  reinterpret_cast<volatile GPIO_TypeDef *>(port)->AFR[pin / 8] |=
      ((0xF0 & config.mode) >> 4) << ((pin % 8) * 4);
}

}  // namespace stm32f3xx
}  // namespace microhal
