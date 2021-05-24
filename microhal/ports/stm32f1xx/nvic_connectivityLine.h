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
    NonMaskableInt_IRQn = -14,   /*!< 2 Non Maskable Interrupt                                        */
    HardFault_IRQn = -13,        /*!< 3 Cortex-M0 Hard Fault Interrupt                                */
    MemoryManagement_IRQn = -12, /*!< 4 Cortex-M3 Memory Management Interrupt                         */
    BusFault_IRQn = -11,         /*!< 5 Cortex-M3 Bus Fault Interrupt                                 */
    UsageFault_IRQn = -10,       /*!< 6 Cortex-M3 Usage Fault Interrupt                               */
    SVC_IRQn = -5,               /*!< 11 Cortex-M0 SV Call Interrupt                                  */
    PendSV_IRQn = -2,            /*!< 14 Cortex-M0 Pend SV Interrupt                                  */
    SysTick_IRQn = -1,           /*!< 15 Cortex-M0 System Tick Interrupt                              */

    /******  STM32F0 specific Interrupt Numbers ******************************************************************/
    WWDG_IRQn = 0,           /*!< Window WatchDog Interrupt                            */
    PVD_IRQn = 1,            /*!< PVD through EXTI Line detection Interrupt            */
    TAMPER_IRQn = 2,         /*!< Tamper Interrupt                                     */
    RTC_IRQn = 3,            /*!< RTC global Interrupt                                 */
    FLASH_IRQn = 4,          /*!< FLASH global Interrupt                               */
    RCC_IRQn = 5,            /*!< RCC global Interrupt                                 */
    EXTI0_IRQn = 6,          /*!< EXTI Line0 Interrupt                                 */
    EXTI1_IRQn = 7,          /*!< EXTI Line1 Interrupt                                 */
    EXTI2_IRQn = 8,          /*!< EXTI Line2 Interrupt                                 */
    EXTI3_IRQn = 9,          /*!< EXTI Line3 Interrupt                                 */
    EXTI4_IRQn = 10,         /*!< EXTI Line4 Interrupt                                 */
    DMA1_Channel1_IRQn = 11, /*!< DMA1 Channel 1 global Interrupt                      */
    DMA1_Channel2_IRQn = 12, /*!< DMA1 Channel 2 global Interrupt                      */
    DMA1_Channel3_IRQn = 13, /*!< DMA1 Channel 3 global Interrupt                      */
    DMA1_Channel4_IRQn = 14, /*!< DMA1 Channel 4 global Interrupt                      */
    DMA1_Channel5_IRQn = 15, /*!< DMA1 Channel 5 global Interrupt                      */
    DMA1_Channel6_IRQn = 16, /*!< DMA1 Channel 6 global Interrupt                      */
    DMA1_Channel7_IRQn = 17, /*!< DMA1 Channel 7 global Interrupt                      */
    ADC1_2_IRQn = 18,        /*!< ADC1 and ADC2 global Interrupt                       */
    CAN1_TX_IRQn = 19,       /*!< USB Device High Priority or CAN1 TX Interrupts       */
    CAN1_RX0_IRQn = 20,      /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
    CAN1_RX1_IRQn = 21,      /*!< CAN1 RX1 Interrupt                                   */
    CAN1_SCE_IRQn = 22,      /*!< CAN1 SCE Interrupt                                   */
    EXTI9_5_IRQn = 23,       /*!< External Line[9:5] Interrupts                        */
    TIM1_BRK_IRQn = 24,      /*!< TIM1 Break Interrupt                                 */
    TIM1_UP_IRQn = 25,       /*!< TIM1 Update Interrupt                                */
    TIM1_TRG_COM_IRQn = 26,  /*!< TIM1 Trigger and Commutation Interrupt               */
    TIM1_CC_IRQn = 27,       /*!< TIM1 Capture Compare Interrupt                       */
    TIM2_IRQn = 28,          /*!< TIM2 global Interrupt                                */
    TIM3_IRQn = 29,          /*!< TIM3 global Interrupt                                */
    TIM4_IRQn = 30,          /*!< TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 31,       /*!< I2C1 Event Interrupt                                 */
    I2C1_ER_IRQn = 32,       /*!< I2C1 Error Interrupt                                 */
    I2C2_EV_IRQn = 33,       /*!< I2C2 Event Interrupt                                 */
    I2C2_ER_IRQn = 34,       /*!< I2C2 Error Interrupt                                 */
    SPI1_IRQn = 35,          /*!< SPI1 global Interrupt                                */
    SPI2_IRQn = 36,          /*!< SPI2 global Interrupt                                */
    USART1_IRQn = 37,        /*!< USART1 global Interrupt                              */
    USART2_IRQn = 38,        /*!< USART2 global Interrupt                              */
    USART3_IRQn = 39,        /*!< USART3 global Interrupt                              */
    EXTI15_10_IRQn = 40,     /*!< External Line[15:10] Interrupts                      */
    RTCAlarm_IRQn = 41,      /*!< RTC Alarm through EXTI Line Interrupt                */
    OTG_FS_WKUP_IRQn = 42,   /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
    TIM5_IRQn = 50,          /*!< TIM5 global Interrupt                                */
    SPI3_IRQn = 51,          /*!< SPI3 global Interrupt                                */
    UART4_IRQn = 52,         /*!< UART4 global Interrupt                               */
    UART5_IRQn = 53,         /*!< UART5 global Interrupt                               */
    TIM6_IRQn = 54,          /*!< TIM6 global Interrupt                                */
    TIM7_IRQn = 55,          /*!< TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn = 56, /*!< DMA2 Channel 1 global Interrupt                      */
    DMA2_Channel2_IRQn = 57, /*!< DMA2 Channel 2 global Interrupt                      */
    DMA2_Channel3_IRQn = 58, /*!< DMA2 Channel 3 global Interrupt                      */
    DMA2_Channel4_IRQn = 59, /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
    DMA2_Channel5_IRQn = 60, /*!< DMA2 Channel 5 global Interrupt        */
    ETH_IRQn = 61,
    ETH_WKUP_IRQn,
    CAN2_TX_IRQn = 63,
    CAN2_RX0_IRQn = 64,
    CAN2_RX1_IRQn = 65,
    CAN2_SCE_IRQn = 66,
    OTG_FS_IRQn
} IRQn_Type;

/**
 * @}
 */

#include "core_cm3.h" /* Cortex-M3 processor and core peripherals */

#endif /* SRC_MICROHAL_PORTS_STM32F1XX_NVIC_H_ */