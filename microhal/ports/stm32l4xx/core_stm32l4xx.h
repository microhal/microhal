/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 11-09-2016
 * last modification: 11-09-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

#ifndef STM32L4XX_CORE_STM32L4XX_H_
#define STM32L4XX_CORE_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "device/stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class Core {
	static uint32_t core_CLK;
	static uint32_t APB1_CLK;
	static uint32_t APB2_CLK;
	static void flash_latency(uint32_t frequency);
	Core(void) = delete;
public:
	typedef enum : uint32_t{
		MSI = 0x00, HSI = 0x01, HSE = 0x02, PLLCLK = 0x03
	} SYSCLKsource;

	enum class PLLsource : uint8_t{
		MSI = 0x00, HSI = 0x01, HSE = 0x02
	};

//	static bool pll_start(uint32_t crystal, uint32_t frequency);
	static bool pll_start(uint32_t frequency, PLLsource source);
	static void fpu_enable(void);

	static uint32_t getCore_CLK(void) {
		return core_CLK;
	}
	static uint32_t getAPB1_CLK(void) {
		return APB1_CLK;
	}
	static uint32_t getAPB2_CLK(void) {
		return APB2_CLK;
	}
	static uint32_t getAPB_CLK(uint8_t APBnr) {
		switch (APBnr) {
		case 1:
			return getAPB1_CLK();
		case 2:
			return getAPB2_CLK();
		default:
			return 0;
		}
	}

	/**
	 * @brief This function return usart clock
	 *
	 * @param usart device pointer
	 * @return
	 */
	static uint32_t getUSARTclk(USART_TypeDef *usart) {
		switch (reinterpret_cast<uint32_t>(usart)) {
		case reinterpret_cast<uint32_t>(USART1):
			return getAPB2frequency();
		case reinterpret_cast<uint32_t>(USART2):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(USART3):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(UART4):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(UART5):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(LPUART1):
			return getAPB2frequency();
		default:
			return 0;
		}
	}

	/**
	 * @brief This function return SPI clock
	 *
	 * @param spi device pointer
	 * @return
	 */
	static uint32_t getSPIclk(SPI_TypeDef *spi) {
		switch (reinterpret_cast<uint32_t>(spi)) {
		case reinterpret_cast<uint32_t>(SPI1):
			return getAPB2frequency();
		case reinterpret_cast<uint32_t>(SPI2):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(SPI3):
			return getAPB1frequency();
		default:
			return 0;
		}
	}

//	static uint32_t getTimerClk(TIM_TypeDef *tim){
//		switch (reinterpret_cast<uint32_t>(tim)) {
//			case reinterpret_cast<uint32_t>(TIM1):	return getAPB2frequency();
//
//			case reinterpret_cast<uint32_t>(TIM2):	return getAPB1frequency();
//			case reinterpret_cast<uint32_t>(TIM3):	return getAPB1frequency();
//			case reinterpret_cast<uint32_t>(TIM4):	return getAPB1frequency();
//			case reinterpret_cast<uint32_t>(TIM5):	return getAPB1frequency();
//			case reinterpret_cast<uint32_t>(TIM6):	return getAPB1frequency();
//			case reinterpret_cast<uint32_t>(TIM7):	return getAPB1frequency();
//
//			case reinterpret_cast<uint32_t>(TIM8):	return getAPB2frequency();
//			case reinterpret_cast<uint32_t>(TIM9):	return getAPB2frequency();
//			case reinterpret_cast<uint32_t>(TIM10):	return getAPB2frequency();
//			case reinterpret_cast<uint32_t>(TIM11):	return getAPB2frequency();
//
//			case reinterpret_cast<uint32_t>(TIM12):	return getAPB1frequency();
//			case reinterpret_cast<uint32_t>(TIM13):	return getAPB1frequency();
//			case reinterpret_cast<uint32_t>(TIM14):	return getAPB1frequency();
//			default:								return 0;
//		}
//	}

	/**
	 * @brief This function return I2C clock
	 *
	 * @param i2c device pointer
	 * @return
	 */
	static uint32_t getI2Cclk(I2C_TypeDef *i2c) {
		switch (reinterpret_cast<uint32_t>(i2c)) {
		case reinterpret_cast<uint32_t>(I2C1):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(I2C2):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(I2C3):
			return getAPB1frequency();
		default:
			return 0;
		}
	}

	static bool setSYSCLKsource(SYSCLKsource source, uint32_t frequency = 0) {
		uint32_t cfgr = RCC->CFGR;
		//clear actual value
		cfgr &= ~RCC_CFGR_SW;
		//set new value
		cfgr |= source;
		//write new value to register
		RCC->CFGR = cfgr;

		if(source == MSI)
		{
			uint32_t msiRange = 0;
			switch(frequency)
			{
			case 100000:
				msiRange = 0;
				break;
			case 200000:
				msiRange = 1;
				break;
			case 400000:
				msiRange = 2;
				break;
			case 800000:
				msiRange = 3;
				break;
			case 1000000:
				msiRange = 4;
				break;
			case 2000000:
				msiRange = 5;
				break;
			case 4000000:
				msiRange = 6;
				break;
			case 8000000:
				msiRange = 7;
				break;
			case 16000000:
				msiRange = 8;
				break;
			case 24000000:
				msiRange = 9;
				break;
			case 32000000:
				msiRange = 10;
				break;
			case 48000000:
				msiRange = 11;
				break;
			}

			RCC->CR &=~ RCC_CR_MSIRANGE_Msk;
			RCC->CR |= msiRange << RCC_CR_MSIRANGE_Pos;
			RCC->CR |= RCC_CR_MSIRGSEL;

			while(!(RCC->CR & RCC_CR_MSIRDY))
			{}
		}

		//wait for clock switch
		while ((RCC->CFGR & RCC_CFGR_SWS) != (source << 2)) {
			; //todo timeout
		}

		volatile uint32_t i = RCC->CR;

		return true;
	}
private:
	static uint32_t getSYSCLKfrequency() {
		uint32_t freq = 0;
		switch (RCC->CFGR & RCC_CFGR_SWS) {
		case 0:
			freq = getMSIfrequency(); //TODO
			break;
		case RCC_CFGR_SWS_0:
			freq = getHSIfrequency();
			break;
		case RCC_CFGR_SWS_1:
			freq = getHSEfrequency();
			break;
		case (RCC_CFGR_SWS_0 | RCC_CFGR_SWS_1):
			freq = getPLLfrequency();
			break;
		}
		return freq;
	}

	static uint32_t getAPB1frequency();

	static uint32_t getAPB2frequency();

	static uint32_t getAHBfrequency();
	/**
	 * @brief This function return MSI frequency.
	 *
	 * @return MSI frequency in [Hz].
	 */
	static uint32_t getMSIfrequency() {
		uint32_t freq = 0;

		switch ((RCC->CR & RCC_CR_MSIRANGE) >> RCC_CR_MSIRANGE_Pos)
		{
		case 0:
			freq = 100000;
			break;
		case 1:
			freq = 200000;
			break;
		case 2:
			freq = 400000;
			break;
		case 3:
			freq = 800000;
			break;
		case 4:
			freq = 1000000;
			break;
		case 5:
			freq = 2000000;
			break;
		case 6:
			freq = 4000000;
			break;
		case 7:
			freq = 8000000;
			break;
		case 8:
			freq = 16000000;
			break;
		case 9:
			freq = 24000000;
			break;
		case 10:
			freq = 32000000;
			break;
		case 11:
			freq = 48000000;
			break;
		}
		return freq;
	}
	/**
	 * @brief This function return HSI frequency.
	 *
	 * @return HSI frequency in [Hz].
	 */
	static uint32_t getHSIfrequency() {
		return 16000000;
	}
	/**
	 * @brief This function return HSE frequency. This value is defined under HSE_FREQUENCY
	 *
	 * @return HSE frequency in [Hz]
	 */
	static uint32_t getHSEfrequency() {
		return 8000000;
	}
	/**
	 * @brief This function return PLL frequency.
	 *
	 * @return PLL frequency in [Hz]
	 */
	static uint32_t getPLLfrequency(){
		uint32_t freq = 0;
		switch((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC_HSE_Msk) >> RCC_PLLCFGR_PLLSRC_Pos)
		{
		case 1:
			freq = getMSIfrequency();
			break;
		case 2:
			freq = getHSIfrequency();
			break;
		case 3:
			freq = getHSEfrequency();
			break;
		}
		freq /= getPLLR();
		freq *= getPLLN();
		freq /= getPLLM();

		return freq;
	}
	/**
	 * @brief This function return PLL R prescaler value.
	 *
	 * @return PLL divider
	 */
	static uint32_t getPLLR(){
		uint32_t presc = 2;
		switch((RCC->PLLCFGR & RCC_PLLCFGR_PLLR_Msk) >> RCC_PLLCFGR_PLLREN_Pos)
		{
		case 0:
			presc = 2;
			break;
		case 1:
			presc = 4;
			break;
		case 2:
			presc = 6;
			break;
		case 3:
			presc = 8;
			break;
		}
		return presc;
	}
	/**
	 * @brief This function return PLL N multiplier value.
	 *
	 * @return PLL multiplier
	 */
	static uint32_t getPLLN()
	{
		return ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN_Msk) >> RCC_PLLCFGR_PLLN_Pos);
	}
	/**
	 * @brief This function return PLL M prescaler value.
	 *
	 * @return PLL divider
	 */
	static uint32_t getPLLM(){
		uint32_t presc = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM_Msk) >> RCC_PLLCFGR_PLLM_Pos);
		return ++presc;
	}

};

/*------------------------------------------------------------------------*//**
 * \brief Enables FPU
 * \details Enables FPU in Cortex-M4 for both privileged and user mode. This is
 * done by enabling CP10 and CP11 coprocessors in CPACR register (possible only
 * when in privileged mode).
 *//*-------------------------------------------------------------------------*/

inline void Core::fpu_enable(void) {
//#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); // set CP10 and CP11 Full Access
//#warning "fpu dziala"
//#endif
}

} // namespace stm32l4xx
} // namespace microhal

#endif  // STM32L4XX_CORE_STM32L4XX_H_
