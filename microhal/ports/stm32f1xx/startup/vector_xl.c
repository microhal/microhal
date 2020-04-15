/**
 * @license    BSD 3-Clause
 *
 * @version    $Id$
 * @brief
 *
 * @authors Pawel Okas
 *
 * @copyright Copyright (c) 2020, Pawel Okas
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
#include <stdint.h>

static void __Default_Handler(void) __attribute__((interrupt));
static void __Default_Handler(void) {
    __asm volatile("BKPT #01");
    while (1)
        ;
}

static void __Default_HardFault_Handler(void) {
    __asm volatile("BKPT #01");
    while (1)
        ;
}

static void Reserved_Handler(void) {
    __asm volatile("BKPT #01");
    while (1)
        ;
}

void NMI_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void HardFault_Handler(void) __attribute__((interrupt, weak, alias("__Default_HardFault_Handler")));
void MemManage_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void BusFault_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void UsageFault_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SVC_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DebugMon_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void PendSV_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SysTick_Handler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void WWDG_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void PVD_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TAMP_STAMP_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void RTC_WKUP_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void FLASH_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void RCC_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void EXTI2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void EXTI3_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void EXTI4_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void ADC1_2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USB_HP_CAN_TX_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USB_LP_CAN_RX0_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void CAN_RX1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void CAN_SCE_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM1_BRK_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM1_UP_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM3_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM4_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void I2C2_EV_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void I2C2_ER_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SPI1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SPI2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USART1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USART2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USART3_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void RTC_Alarm_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void CEC_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM12_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM13_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM14_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM5_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SPI3_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM6_DAC1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM7_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA2_Channel1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA2_Channel2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA2_Channel3_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA2_Channel4_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void DMA2_Channel5_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SDADC1_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SDADC2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void SDADC3_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void COMP1_2_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USB_HP_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USB_LP_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void USBWakeUp_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void TIM19_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));
void FPU_IRQHandler(void) __attribute__((interrupt, weak, alias("__Default_Handler")));

/*
+=============================================================================+
| Vector table
+=============================================================================+
*/

extern const char __main_stack_end;  // imported main stack end (from linker script)

void Reset_Handler(void);  // import the address of Reset_Handler()

void (*const vectors[])(void) __attribute__((section(".vectors"), used)) = {
    (void (*)(void)) & __main_stack_end,  // Main stack end address
    Reset_Handler,                        // 1 Reset
    NMI_Handler,                          // 2 Non-maskable interrupt (RCC clock security system)
    HardFault_Handler,                    // 3 All class of fault
    MemManage_Handler,                    // 4 Memory management
    BusFault_Handler,                     // 5 Pre-fetch fault, memory access fault
    UsageFault_Handler,                   // 6 Undefined instruction or illegal state
    Reserved_Handler,                     // 7 Reserved
    Reserved_Handler,                     // 8 Reserved
    Reserved_Handler,                     // 9 Reserved
    Reserved_Handler,                     // 10 Reserved
    SVC_Handler,                          // 11 0x2C System service call via SWI instruction
    DebugMon_Handler,                     // 12 0x30 Debug monitor
    Reserved_Handler,                     // 13 0x34 Reserved
    PendSV_Handler,                       // 14 0x38 Pendable request for system service
    SysTick_Handler,                      // 15 0x3C System tick timer
    WWDG_IRQHandler,                      // 16 Window WatchDog
    PVD_IRQHandler,                       // 17 PVD through EXTI Line detection
    TAMP_STAMP_IRQHandler,                // 18 Tamper and TimeStamps through the EXTI line
    RTC_WKUP_IRQHandler,                  // 19 RTC Wakeup through the EXTI line
    FLASH_IRQHandler,                     // 20 FLASH
    RCC_IRQHandler,                       // 21 RCC
    EXTI0_IRQHandler,                     // 22 EXTI Line0
    EXTI1_IRQHandler,                     // 23 EXTI Line1
    EXTI2_IRQHandler,                     // 24 EXTI Line2
    EXTI3_IRQHandler,                     // 25 EXTI Line3
    EXTI4_IRQHandler,                     // 26 EXTI Line4
    DMA1_Channel1_IRQHandler,             // 27 DMA1 Channel 1
    DMA1_Channel2_IRQHandler,             // 28 DMA1 Channel 2
    DMA1_Channel3_IRQHandler,             // 29 DMA1 Channel 3
    DMA1_Channel4_IRQHandler,             // 30 DMA1 Channel 4
    DMA1_Channel5_IRQHandler,             // 31 DMA1 Channel 5
    DMA1_Channel6_IRQHandler,             // 32 DMA1 Channel 6
    DMA1_Channel7_IRQHandler,             // 33 DMA1 Channel 7
    ADC1_2_IRQHandler,                    // 34 ADC1, ADC2 and ADC3s
    USB_HP_CAN_TX_IRQHandler,             // 35 CAN1 TX
    USB_LP_CAN_RX0_IRQHandler,            // 36 CAN1 RX0
    CAN_RX1_IRQHandler,                   // CAN1 RX1
    CAN_SCE_IRQHandler,                   // CAN1 SCE
    EXTI9_5_IRQHandler,                   // External Line[9:5]s
    TIM1_BRK_IRQHandler,                  // TIM1 Break
    TIM1_UP_IRQHandler,                   // TIM1 Update
    TIM1_TRG_COM_IRQHandler,              // TIM1 Trigger and Commutation
    TIM1_CC_IRQHandler,                   // TIM1 Capture Compare
    TIM2_IRQHandler,                      // TIM2
    TIM3_IRQHandler,                      // TIM3
    TIM4_IRQHandler,                      // TIM4
    I2C1_EV_IRQHandler,                   // I2C1 Event
    I2C1_ER_IRQHandler,                   // I2C1 Error
    I2C2_EV_IRQHandler,                   // I2C2 Event
    I2C2_ER_IRQHandler,                   // I2C2 Error
    SPI1_IRQHandler,                      // SPI1
    SPI2_IRQHandler,                      // SPI2
    USART1_IRQHandler,                    // USART1
    USART2_IRQHandler,                    // USART2
    USART3_IRQHandler,                    // USART3
    EXTI15_10_IRQHandler,                 // External Line[15:10]s
    RTC_Alarm_IRQHandler,                 // RTC Alarm (A and B) through EXTI Line
    CEC_IRQHandler,
    TIM12_IRQHandler,
    TIM13_IRQHandler,
    TIM14_IRQHandler,
    0,
    0,
    0,
    0,
    TIM5_IRQHandler,
    SPI3_IRQHandler,
    0,
    0,
    TIM6_DAC1_IRQHandler,
    TIM7_IRQHandler,           // TIM7
    DMA2_Channel1_IRQHandler,  // DMA2 Channel 0
    DMA2_Channel2_IRQHandler,  // DMA2 Channel 1
    DMA2_Channel3_IRQHandler,  // DMA2 Channel 2
    DMA2_Channel4_IRQHandler,  // DMA2 Channel 3
    DMA2_Channel5_IRQHandler,  // DMA2 Channel 4
    SDADC1_IRQHandler,
    SDADC2_IRQHandler,
    SDADC3_IRQHandler,
    COMP1_2_IRQHandler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    USB_HP_IRQHandler,
    USB_LP_IRQHandler,
    USBWakeUp_IRQHandler,
    0,
    TIM19_IRQHandler,
    0,
    0,
    FPU_IRQHandler,  // FPU
};
