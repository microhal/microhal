/*
 * nvic.h
 *
 *  Created on: Nov 13, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STM32F1XX_NVIC_H_
#define SRC_MICROHAL_PORTS_STM32F1XX_NVIC_H_

//#define __CM3_REV                 0x0001U  /*!< Core revision r0p1                            */
#define __NVIC_PRIO_BITS 4U       /*!< STM32F303xC devices use 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig 0U /*!< Set to 1 if different SysTick Config is used */
// XS#define __FPU_PRESENT             1U       /*!< STM32F303xC devices provide an FPU */

/*!< Interrupt Number Definition */
typedef enum {
    /******  Cortex-M0 Processor Exceptions Numbers **************************************************************/
    NonMaskableInt_IRQn = -14, /*!< 2 Non Maskable Interrupt                                        */
    HardFault_IRQn = -13,      /*!< 3 Cortex-M0 Hard Fault Interrupt                                */
    SVC_IRQn = -5,             /*!< 11 Cortex-M0 SV Call Interrupt                                  */
    PendSV_IRQn = -2,          /*!< 14 Cortex-M0 Pend SV Interrupt                                  */
    SysTick_IRQn = -1,         /*!< 15 Cortex-M0 System Tick Interrupt                              */

    /******  STM32F0 specific Interrupt Numbers ******************************************************************/
    WWDG_IRQn = 0,                 /*!< Window WatchDog Interrupt                               */
    RTC_IRQn = 2,                  /*!< RTC Interrupt through EXTI Lines 17, 19 and 20                  */
    FLASH_IRQn = 3,                /*!< FLASH global Interrupt                                          */
    RCC_IRQn = 4,                  /*!< RCC global Interrupt                                            */
    EXTI0_1_IRQn = 5,              /*!< EXTI Line 0 and 1 Interrupt                                     */
    EXTI2_3_IRQn = 6,              /*!< EXTI Line 2 and 3 Interrupt                                     */
    EXTI4_15_IRQn = 7,             /*!< EXTI Line 4 to 15 Interrupt                                     */
    DMA1_Channel1_IRQn = 9,        /*!< DMA1 Channel 1 Interrupt                                        */
    DMA1_Channel2_3_IRQn = 10,     /*!< DMA1 Channel 2 and Channel 3 Interrupt                          */
    DMA1_Channel4_5_IRQn = 11,     /*!< DMA1 Channel 4 and Channel 5 Interrupt                          */
    ADC1_2_IRQn = 18,              /*!< ADC1 Interrupt                                                  */
    TIM1_BRK_UP_TRG_COM_IRQn = 13, /*!< TIM1 Break, Update, Trigger and Commutation Interrupt           */
    TIM1_BRK_IRQn = 24,
    TIM1_UP_IRQn = 25,
    TIM1_TRG_COM_IRQn = 26,  // TIM1 Trigger and Commutation    interrupts
    TIM1_CC_IRQn = 27,       /*!< TIM1 Capture Compare Interrupt                                  */
    TIM2_IRQn = 28,          // TIM2 global interrupt
    TIM3_IRQn = 29,          /*!< TIM3 global Interrupt                                           */
    TIM4_IRQn = 30,
    TIM5_IRQn = 50,
    TIM6_IRQn = 54, /*!< TIM6 global Interrupt                                           */
    TIM7_IRQn = 55, /*!< TIM7 global Interrupt                                           */
    TIM8_BRK_IRQn = 43,
    TIM8_UP_IRQn = 44,
    TIM8_TRG_COM_IRQn = 45,
    TIM8_CC_IRQn = 46,
    I2C1_IRQn = 23,   /*!< I2C1 Event Interrupt & EXTI Line23 Interrupt (I2C1 wakeup)      */
    I2C2_IRQn = 24,   /*!< I2C2 Event Interrupt                                            */
    SPI1_IRQn = 35,   /*!< SPI1 global Interrupt                                           */
    SPI2_IRQn = 36,   /*!< SPI2 global Interrupt                                           */
    SPI3_IRQn = 51,   /*!< SPI3 global Interrupt                                           */
    USART1_IRQn = 37, /*!< USART1 global Interrupt & EXTI Line25 Interrupt (USART1 wakeup) */
    USART2_IRQn = 38, /*!< USART2 global Interrupt & EXTI Line26 Interrupt (USART2 wakeup) */
    USART3_IRQn = 39,
    UART4_IRQn = 52,
    UART5_IRQn = 53, /*!< USART3 to USART6 global Interrupt                               */
} IRQn_Type;

/**
 * @}
 */

#include "core_cm3.h" /* Cortex-M3 processor and core peripherals */

#endif /* SRC_MICROHAL_PORTS_STM32F1XX_NVIC_H_ */
