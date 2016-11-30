/** \file vectors.c
 * \brief STM32F4x vector table and __Default_Handler()
 * \details STM32F4x vector table and __Default_Handler()
 * \author Freddie Chopin, http://www.freddiechopin.info/
 * \date 2012-03-15
 */

/******************************************************************************
* chip: STM32F4x
* compiler: arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors) 4.6.2
* 	20110921 (release) [ARM/embedded-4_6-branch revision 182083]
******************************************************************************/

/*------------------------------------------------------------------------*//**
* \brief Default interrupt handler.
* \details Default interrupt handler, used for interrupts that don't have their
* own handler defined.
*//*-------------------------------------------------------------------------*/

static void __Default_Handler(void) __attribute__ ((interrupt));
static void __Default_Handler(void)
{
	asm volatile("BKPT #01");
	while (1);
}

static void __Default_NMI_Handler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_HardFault_Handler(void) {	asm volatile("BKPT #01");while (1); }
static void __Default_MemManage_Handler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_BusFault_Handler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_UsageFault_Handler(void) {asm volatile("BKPT #01"); while (1); }
static void __Default_SVC_Handler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_DebugMon_Handler(void) {asm volatile("BKPT #01"); while (1); }
static void __Default_PendSV_Handler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_SysTick_Handler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_WWDG_IRQHandler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_PVD_IRQHandler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_TAMP_STAMP_IRQHandler(void) { asm volatile("BKPT #01");while (1); }
static void __Default_RTC_WKUP_IRQHandler(void) {asm volatile("BKPT #01"); while (1); }
static void __Default_FLASH_IRQHandler(void) {asm volatile("BKPT #01"); while (1); }
static void __Default_RCC_IRQHandler(void) { while (1); }
static void __Default_EXTI0_IRQHandler(void) { while (1); }
static void __Default_EXTI1_IRQHandler(void) { while (1); }
static void __Default_EXTI2_IRQHandler(void) { while (1); }
static void __Default_EXTI3_IRQHandler(void) { while (1); }
static void __Default_EXTI4_IRQHandler(void) { while (1); }
static void __Default_DMA1_Stream0_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA1_Stream1_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA1_Stream2_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA1_Stream3_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA1_Stream4_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA1_Stream5_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA1_Stream6_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_ADC_IRQHandler(void) { while (1); }
static void __Default_CAN1_TX_IRQHandler(void) { while (1); }
static void __Default_CAN1_RX0_IRQHandler(void) { while (1); }
static void __Default_CAN1_RX1_IRQHandler(void) { while (1); }
static void __Default_CAN1_SCE_IRQHandler(void) { while (1); }
static void __Default_EXTI9_5_IRQHandler(void) { while (1); }

static void __Default_I2C1_EV_IRQHandler(void) { while (1); }
static void __Default_I2C1_ER_IRQHandler(void) { while (1); }
static void __Default_I2C2_EV_IRQHandler(void) { while (1); }
static void __Default_I2C2_ER_IRQHandler(void) { while (1); }
static void __Default_SPI1_IRQHandler(void) { while (1); }
static void __Default_SPI2_IRQHandler(void) { while (1); }
static void __Default_USART1_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_USART2_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_USART3_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_EXTI15_10_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_RTC_Alarm_IRQHandler(void) { while (1); }

static void __Default_DMA1_Stream7_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }

static void __Default_SPI3_IRQHandler(void) { while (1); }
static void __Default_UART4_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_UART5_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_TIM6_DAC_IRQHandler(void) { while (1); }
static void __Default_TIM7_IRQHandler(void) { while (1); }
static void __Default_DMA2_Stream0_IRQHandler(void) { while (1); }
static void __Default_DMA2_Stream1_IRQHandler(void) { while (1); }
static void __Default_DMA2_Stream2_IRQHandler(void) { while (1); }
static void __Default_DMA2_Stream3_IRQHandler(void) { while (1); }
static void __Default_DMA2_Stream4_IRQHandler(void) { while (1); }
//static void __Default_(void) { while (1); }
//static void __Default_(void) { while (1); }


static void __Default_DMA2_Stream5_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA2_Stream6_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_DMA2_Stream7_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }
static void __Default_USART6_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }

static void __Default_FPU_IRQHandler(void) { asm volatile("BKPT #01"); while (1); }

static void __Default_Reserved_Handler(void) {asm volatile("BKPT #01");  while (1); }

/*
+=============================================================================+
| assign all unhandled interrupts to the default handler
+=============================================================================+
*/

// Non-maskable interrupt (RCC clock security system)
void NMI_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_NMI_Handler")));

// All class of fault
void HardFault_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_HardFault_Handler")));

// Memory management
void MemManage_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_MemManage_Handler")));

// Pre-fetch fault, memory access fault
void BusFault_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_BusFault_Handler")));

// Undefined instruction or illegal state
void UsageFault_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_UsageFault_Handler")));

// Reserved 0x1C
void __Reserved_0x1C_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_Reserved_Handler")));

// Reserved 0x20
void __Reserved_0x20_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_Reserved_Handler")));

// Reserved 0x24
void __Reserved_0x24_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_Reserved_Handler")));

// Reserved 0x28
void __Reserved_0x28_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_Reserved_Handler")));

// System service call via SWI instruction
void SVC_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_SVC_Handler")));

// Debug monitor
void DebugMon_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_DebugMon_Handler")));

// Reserved 0x34
void __Reserved_0x34_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_Reserved_Handler")));

// Pendable request for system service
void PendSV_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_PendSV_Handler")));

// System tick timer
void SysTick_Handler(void) __attribute__ ((interrupt, weak, alias("__Default_SysTick_Handler")));

// Window WatchDog
void WWDG_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_WWDG_IRQHandler")));

// PVD through EXTI Line detection
void PVD_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_PVD_IRQHandler")));

// Tamper and TimeStamps through the EXTI line
void TAMP_STAMP_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_TAMP_STAMP_IRQHandler")));

// RTC Wakeup through the EXTI line
void RTC_WKUP_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_RTC_WKUP_IRQHandler")));

// FLASH
void FLASH_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_FLASH_IRQHandler")));

// RCC
void RCC_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_RCC_IRQHandler")));

// EXTI Line0
void EXTI0_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_EXTI0_IRQHandler")));

// EXTI Line1
void EXTI1_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_EXTI1_IRQHandler")));

// EXTI Line2
void EXTI2_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_EXTI2_IRQHandler")));

// EXTI Line3
void EXTI3_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_EXTI3_IRQHandler")));

// EXTI Line4
void EXTI4_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_EXTI4_IRQHandler")));

// DMA1 Stream 0
void DMA1_Stream0_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream0_IRQHandler")));

// DMA1 Stream 1
void DMA1_Stream1_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream1_IRQHandler")));

// DMA1 Stream 2
void DMA1_Stream2_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream2_IRQHandler")));

// DMA1 Stream 3
void DMA1_Stream3_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream3_IRQHandler")));

// DMA1 Stream 4
void DMA1_Stream4_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream4_IRQHandler")));

// DMA1 Stream 5
void DMA1_Stream5_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream5_IRQHandler")));

// DMA1 Stream 6
void DMA1_Stream6_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream6_IRQHandler")));

// ADC1, ADC2 and ADC3s
void ADC_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_ADC_IRQHandler")));

// CAN1 TX
void CAN1_TX_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_CAN1_TX_IRQHandler")));

// CAN1 RX0
void CAN1_RX0_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_CAN1_RX0_IRQHandler")));

// CAN1 RX1
void CAN1_RX1_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_CAN1_RX1_IRQHandler")));

// CAN1 SCE
void CAN1_SCE_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_CAN1_SCE_IRQHandler")));

// External Line[9:5]s
void EXTI9_5_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_EXTI9_5_IRQHandler")));

// TIM1 Break and TIM9
void TIM1_BRK_TIM9_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM1 Update and TIM10
void TIM1_UP_TIM10_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM1 Trigger and Commutation and TIM11
void TIM1_TRG_COM_TIM11_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM1 Capture Compare
void TIM1_CC_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM2
void TIM2_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM3
void TIM3_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM4
void TIM4_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// I2C1 Event
void I2C1_EV_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_I2C1_EV_IRQHandler")));

// I2C1 Error
void I2C1_ER_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_I2C1_ER_IRQHandler")));

// I2C2 Event
void I2C2_EV_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_I2C2_EV_IRQHandler")));

// I2C2 Error
void I2C2_ER_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_I2C2_ER_IRQHandler")));

// SPI1
void SPI1_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_SPI1_IRQHandler")));

// SPI2
void SPI2_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_SPI2_IRQHandler")));

// USART1
void USART1_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_USART1_IRQHandler")));

// USART2
void USART2_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_USART2_IRQHandler")));

// USART3
void USART3_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_USART3_IRQHandler")));

// External Line[15:10]s
void EXTI15_10_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_EXTI15_10_IRQHandler")));

// RTC Alarm (A and B) through EXTI Line
void RTC_Alarm_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_RTC_Alarm_IRQHandler")));

// USB OTG FS Wakeup through EXTI line
void OTG_FS_WKUP_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM8 Break and TIM12
void TIM8_BRK_TIM12_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM8 Update and TIM13
void TIM8_UP_TIM13_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM8 Trigger and Commutation and TIM14
void TIM8_TRG_COM_TIM14_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM8 Capture Compare
void TIM8_CC_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// DMA1 Stream7
void DMA1_Stream7_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA1_Stream7_IRQHandler")));

// FSMC
void FSMC_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// SDIO
void SDIO_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// TIM5
void TIM5_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// SPI3
void SPI3_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_SPI3_IRQHandler")));

// UART4
void UART4_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_UART4_IRQHandler")));

// UART5
void UART5_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_UART5_IRQHandler")));

// TIM6 and DAC1&2 underrun errors
void TIM6_DAC_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_TIM6_DAC_IRQHandler")));

// TIM7
void TIM7_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_TIM7_IRQHandler")));

// DMA2 Stream 0
void DMA2_Stream0_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream0_IRQHandler")));

// DMA2 Stream 1
void DMA2_Stream1_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream1_IRQHandler")));

// DMA2 Stream 2
void DMA2_Stream2_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream2_IRQHandler")));

// DMA2 Stream 3
void DMA2_Stream3_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream3_IRQHandler")));

// DMA2 Stream 4
void DMA2_Stream4_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream4_IRQHandler")));

// Ethernet
void ETH_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// Ethernet Wakeup through EXTI line
void ETH_WKUP_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// CAN2 TX
void CAN2_TX_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// CAN2 RX0
void CAN2_RX0_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// CAN2 RX1
void CAN2_RX1_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// CAN2 SCE
void CAN2_SCE_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// USB OTG FS
void OTG_FS_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// DMA2 Stream 5
void DMA2_Stream5_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream5_IRQHandler")));

// DMA2 Stream 6
void DMA2_Stream6_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream6_IRQHandler")));

// DMA2 Stream 7
void DMA2_Stream7_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_DMA2_Stream7_IRQHandler")));

// USART6
void USART6_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_USART6_IRQHandler")));

// I2C3 event
void I2C3_EV_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// I2C3 error
void I2C3_ER_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// USB OTG HS End Point 1 Out
void OTG_HS_EP1_OUT_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// USB OTG HS End Point 1 In
void OTG_HS_EP1_IN_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// USB OTG HS Wakeup through EXTI
void OTG_HS_WKUP_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// USB OTG HS
void OTG_HS_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// DCMI
void DCMI_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// CRYP crypto
void CRYP_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// Hash and Rng
void HASH_RNG_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_Handler")));

// FPU
void FPU_IRQHandler(void) __attribute__ ((interrupt, weak, alias("__Default_FPU_IRQHandler")));

/*
+=============================================================================+
| Vector table
+=============================================================================+
*/

extern const char __main_stack_end;			// imported main stack end (from linker script)

void Reset_Handler(void);					// import the address of Reset_Handler()

void (* const vectors[])(void) __attribute__ ((section(".vectors"))) = {
		(void (*)(void))&__main_stack_end,	// Main stack end address
		Reset_Handler,						// Reset
		NMI_Handler,						// Non-maskable interrupt (RCC clock security system)
		HardFault_Handler,					// All class of fault
		MemManage_Handler,					// Memory management
		BusFault_Handler,					// Pre-fetch fault, memory access fault
		UsageFault_Handler,					// Undefined instruction or illegal state
		__Reserved_0x1C_Handler,			// Reserved 0x1C
		__Reserved_0x20_Handler,			// Reserved 0x20
		__Reserved_0x24_Handler,			// Reserved 0x24
		__Reserved_0x28_Handler,			// Reserved 0x28
		SVC_Handler,						// System service call via SWI instruction
		DebugMon_Handler,					// Debug monitor
		__Reserved_0x34_Handler,			// Reserved 0x34
		PendSV_Handler,						// Pendable request for system service
		SysTick_Handler,					// System tick timer
		WWDG_IRQHandler,					// Window WatchDog
		PVD_IRQHandler,						// PVD through EXTI Line detection
		TAMP_STAMP_IRQHandler,				// Tamper and TimeStamps through the EXTI line
		RTC_WKUP_IRQHandler,				// RTC Wakeup through the EXTI line
		FLASH_IRQHandler,					// FLASH
		RCC_IRQHandler,						// RCC
		EXTI0_IRQHandler,					// EXTI Line0
		EXTI1_IRQHandler,					// EXTI Line1
		EXTI2_IRQHandler,					// EXTI Line2
		EXTI3_IRQHandler,					// EXTI Line3
		EXTI4_IRQHandler,					// EXTI Line4
		DMA1_Stream0_IRQHandler,			// DMA1 Stream 0
		DMA1_Stream1_IRQHandler,			// DMA1 Stream 1
		DMA1_Stream2_IRQHandler,			// DMA1 Stream 2
		DMA1_Stream3_IRQHandler,			// DMA1 Stream 3
		DMA1_Stream4_IRQHandler,			// DMA1 Stream 4
		DMA1_Stream5_IRQHandler,			// DMA1 Stream 5
		DMA1_Stream6_IRQHandler,			// DMA1 Stream 6
		ADC_IRQHandler,						// ADC1, ADC2 and ADC3s
		CAN1_TX_IRQHandler,					// CAN1 TX
		CAN1_RX0_IRQHandler,				// CAN1 RX0
		CAN1_RX1_IRQHandler,				// CAN1 RX1
		CAN1_SCE_IRQHandler,				// CAN1 SCE
		EXTI9_5_IRQHandler,					// External Line[9:5]s
		TIM1_BRK_TIM9_IRQHandler,			// TIM1 Break and TIM9
		TIM1_UP_TIM10_IRQHandler,			// TIM1 Update and TIM10
		TIM1_TRG_COM_TIM11_IRQHandler,		// TIM1 Trigger and Commutation and TIM11
		TIM1_CC_IRQHandler,					// TIM1 Capture Compare
		TIM2_IRQHandler,					// TIM2
		TIM3_IRQHandler,					// TIM3
		TIM4_IRQHandler,					// TIM4
		I2C1_EV_IRQHandler,					// I2C1 Event
		I2C1_ER_IRQHandler,					// I2C1 Error
		I2C2_EV_IRQHandler,					// I2C2 Event
		I2C2_ER_IRQHandler,					// I2C2 Error
		SPI1_IRQHandler,					// SPI1
		SPI2_IRQHandler,					// SPI2
		USART1_IRQHandler,					// USART1
		USART2_IRQHandler,					// USART2
		USART3_IRQHandler,					// USART3
		EXTI15_10_IRQHandler,				// External Line[15:10]s
		RTC_Alarm_IRQHandler,				// RTC Alarm (A and B) through EXTI Line
		OTG_FS_WKUP_IRQHandler,				// USB OTG FS Wakeup through EXTI line
		TIM8_BRK_TIM12_IRQHandler,			// TIM8 Break and TIM12
		TIM8_UP_TIM13_IRQHandler,			// TIM8 Update and TIM13
		TIM8_TRG_COM_TIM14_IRQHandler,		// TIM8 Trigger and Commutation and TIM14
		TIM8_CC_IRQHandler,					// TIM8 Capture Compare
		DMA1_Stream7_IRQHandler,			// DMA1 Stream7
		FSMC_IRQHandler,					// FSMC
		SDIO_IRQHandler,					// SDIO
		TIM5_IRQHandler,					// TIM5
		SPI3_IRQHandler,					// SPI3
		UART4_IRQHandler,					// UART4
		UART5_IRQHandler,					// UART5
		TIM6_DAC_IRQHandler,				// TIM6 and DAC1&2 underrun errors
		TIM7_IRQHandler,					// TIM7
		DMA2_Stream0_IRQHandler,			// DMA2 Stream 0
		DMA2_Stream1_IRQHandler,			// DMA2 Stream 1
		DMA2_Stream2_IRQHandler,			// DMA2 Stream 2
		DMA2_Stream3_IRQHandler,			// DMA2 Stream 3
		DMA2_Stream4_IRQHandler,			// DMA2 Stream 4
		ETH_IRQHandler,						// Ethernet
		ETH_WKUP_IRQHandler,				// Ethernet Wakeup through EXTI line
		CAN2_TX_IRQHandler,					// CAN2 TX
		CAN2_RX0_IRQHandler,				// CAN2 RX0
		CAN2_RX1_IRQHandler,				// CAN2 RX1
		CAN2_SCE_IRQHandler,				// CAN2 SCE
		OTG_FS_IRQHandler,					// USB OTG FS
		DMA2_Stream5_IRQHandler,			// DMA2 Stream 5
		DMA2_Stream6_IRQHandler,			// DMA2 Stream 6
		DMA2_Stream7_IRQHandler,			// DMA2 Stream 7
		USART6_IRQHandler,					// USART6
		I2C3_EV_IRQHandler,					// I2C3 event
		I2C3_ER_IRQHandler,					// I2C3 error
		OTG_HS_EP1_OUT_IRQHandler,			// USB OTG HS End Point 1 Out
		OTG_HS_EP1_IN_IRQHandler,			// USB OTG HS End Point 1 In
		OTG_HS_WKUP_IRQHandler,				// USB OTG HS Wakeup through EXTI
		OTG_HS_IRQHandler,					// USB OTG HS
		DCMI_IRQHandler,					// DCMI
		CRYP_IRQHandler,					// CRYP crypto
		HASH_RNG_IRQHandler,				// Hash and Rng
		FPU_IRQHandler,						// FPU
};

/******************************************************************************
* END OF FILE
******************************************************************************/
