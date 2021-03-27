/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 26-03-2021
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

#ifndef SRC_MICROHAL_PORTS_STM32G0XX_NVIC_H_
#define SRC_MICROHAL_PORTS_STM32G0XX_NVIC_H_

#define __CM0PLUS_REV 0U          /*!< Core Revision r0p0                            */
#define __MPU_PRESENT 1U          /*!< STM32G0xx  provides an MPU                    */
#define __VTOR_PRESENT 1U         /*!< Vector  Table  Register supported             */
#define __NVIC_PRIO_BITS 2U       /*!< STM32G0xx uses 2 Bits for the Priority Levels */
#define __Vendor_SysTickConfig 0U /*!< Set to 1 if different SysTick Config is used  */

// based on RM0454
typedef enum {
    /******  Cortex-M0+ Processor Exceptions Numbers ***************************************************************/
    NonMaskableInt_IRQn = -14, /*!< 2 Non Maskable Interrupt                                          */
    HardFault_IRQn = -13,      /*!< 3 Cortex-M Hard Fault Interrupt                                   */
    SVC_IRQn = -5,             /*!< 11 Cortex-M SV Call Interrupt                                     */
    PendSV_IRQn = -2,          /*!< 14 Cortex-M Pend SV Interrupt                                     */
    SysTick_IRQn = -1,         /*!< 15 Cortex-M System Tick Interrupt                                 */
                               /******  STM32G0xxxx specific Interrupt Numbers ****************************************************************/
    WWDG_IRQn = 0,             /*!< Window WatchDog Interrupt                                         */
    RTC_TAMP_IRQn = 2,         /*!< RTC interrupt through the EXTI line 19 & 21                       */
    FLASH_IRQn = 3,            /*!< FLASH global Interrupt                                            */
    RCC_IRQn = 4,              /*!< RCC global Interrupt                                              */
    EXTI0_1_IRQn = 5,          /*!< EXTI 0 and 1 Interrupts                                           */
    EXTI2_3_IRQn = 6,          /*!< EXTI Line 2 and 3 Interrupts                                      */
    EXTI4_15_IRQn = 7,         /*!< EXTI Line 4 to 15 Interrupts                                      */
    DMA1_Channel1_IRQn = 9,    /*!< DMA1 Channel 1 Interrupt                                          */
    DMA1_Channel2_3_IRQn = 10, /*!< DMA1 Channel 2 and Channel 3 Interrupts                           */
    DMA1_Ch4_7_DMAMUX1_DMA2_Ch1_5_IRQn = 11, /*!< DMA1 Channel 4 to Channel 7 and DMAMUX1 Overrun Interrupts        */
    ADC1_IRQn = 12,                          /*!< ADC1, COMP1 and COMP2 Interrupts (combined with EXTI 17 & 18)     */
    TIM1_BRK_UP_TRG_COM_IRQn = 13,           /*!< TIM1 Break, Update, Trigger and Commutation Interrupts            */
    TIM1_CC_IRQn = 14,                       /*!< TIM1 Capture Compare Interrupt                                    */
    TIM3_TIM4_IRQn = 16,                     /*!< TIM3 global Interrupt                                             */
    TIM6_IRQn = 17,                          /*!< TIM6, DAC and LPTIM1 global Interrupts                            */
    TIM7_IRQn = 18,                          /*!< TIM7 and LPTIM2 global Interrupt                                  */
    TIM14_IRQn = 19,                         /*!< TIM14 global Interrupt                                            */
    TIM15_IRQn = 20,                         /*!< TIM15 global Interrupt                                            */
    TIM16_IRQn = 21,                         /*!< TIM16 global Interrupt                                            */
    TIM17_IRQn = 22,                         /*!< TIM17 global Interrupt                                            */
    I2C1_IRQn = 23,                          /*!< I2C1 Interrupt  (combined with EXTI 23)                           */
    I2C2_I2C3_IRQn = 24,                     /*!< I2C2 Interrupt                                                    */
    SPI1_IRQn = 25,                          /*!< SPI1/I2S1 Interrupt                                               */
    SPI2_SPI3_IRQn = 26,                     /*!< SPI2 Interrupt                                                    */
    USART1_IRQn = 27,                        /*!< USART1 Interrupt                                                  */
    USART2_IRQn = 28,                        /*!< USART2 Interrupt                                                  */
    USART3_4_5_6_IRQn = 29,                  /*!< USART3, USART4 and LPUART1 globlal Interrupts (combined with EXTI 28) */
} IRQn_Type;

#define _MICROHAL_DMA_SHERED_IRQ

#include <core_cm0plus.h>

#endif /* SRC_MICROHAL_PORTS_STM32G0XX_NVIC_H_ */
