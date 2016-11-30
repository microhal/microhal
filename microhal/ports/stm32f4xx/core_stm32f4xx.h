#ifndef CORE_STM32F4XX_H__
#define CORE_STM32F4XX_H__
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

class Core {
	static uint32_t core_CLK;
	static uint32_t APB1_CLK;
	static uint32_t APB2_CLK;
	static void flash_latency(uint32_t frequency);
	Core(void) = delete;
public:
	typedef enum : uint32_t{
		HSI = 0x00, HSE = 0x01, PLLCLK = 0x02
	} SYSCLKsource;

	static bool pll_start(uint32_t crystal, uint32_t frequency);
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
		case reinterpret_cast<uint32_t>(USART6):
			return getAPB2frequency();
		case reinterpret_cast<uint32_t>(UART7):
			return getAPB1frequency();
		case reinterpret_cast<uint32_t>(UART8):
			return getAPB1frequency();
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
		case reinterpret_cast<uint32_t>(SPI4):
			return getAPB2frequency();
		case reinterpret_cast<uint32_t>(SPI5):
			return getAPB2frequency();
		case reinterpret_cast<uint32_t>(SPI6):
			return getAPB2frequency();
		default:
			return 0;
		}
	}

	static uint32_t getTimerClk(TIM_TypeDef *tim){
		switch (reinterpret_cast<uint32_t>(tim)) {
			case reinterpret_cast<uint32_t>(TIM1):	return getAPB2frequency();

			case reinterpret_cast<uint32_t>(TIM2):	return getAPB1frequency();
			case reinterpret_cast<uint32_t>(TIM3):	return getAPB1frequency();
			case reinterpret_cast<uint32_t>(TIM4):	return getAPB1frequency();
			case reinterpret_cast<uint32_t>(TIM5):	return getAPB1frequency();
			case reinterpret_cast<uint32_t>(TIM6):	return getAPB1frequency();
			case reinterpret_cast<uint32_t>(TIM7):	return getAPB1frequency();

			case reinterpret_cast<uint32_t>(TIM8):	return getAPB2frequency();
			case reinterpret_cast<uint32_t>(TIM9):	return getAPB2frequency();
			case reinterpret_cast<uint32_t>(TIM10):	return getAPB2frequency();
			case reinterpret_cast<uint32_t>(TIM11):	return getAPB2frequency();

			case reinterpret_cast<uint32_t>(TIM12):	return getAPB1frequency();
			case reinterpret_cast<uint32_t>(TIM13):	return getAPB1frequency();
			case reinterpret_cast<uint32_t>(TIM14):	return getAPB1frequency();
			default:								return 0;
		}
	}

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

	static bool setSYSCLKsource(SYSCLKsource source) {
		uint32_t cfgr = RCC->CFGR;
		//clear actual value
		cfgr &= ~RCC_CFGR_SW;
		//set new value
		cfgr |= source;
		//write new value to register
		RCC->CFGR = cfgr;

		//wait for clock switch
		while ((RCC->CFGR & RCC_CFGR_SWS) != (source << 2)) {
			; //todo timeout
		}

		return true;
	}
private:
	static uint32_t getSYSCLKfrequency() {
		uint32_t freq = 0;
		switch (RCC->CFGR & RCC_CFGR_SWS) {
		case 0:
			freq = getHSIfrequency();
			break;
		case RCC_CFGR_SWS_0:
			freq = getHSEfrequency();
			break;
		case RCC_CFGR_SWS_1:
			//freq = getPLLfrequency();
			freq = 168000000;
			break;
		}
		return freq;
	}

	static uint32_t getAPB1frequency();

	static uint32_t getAPB2frequency();

	static uint32_t getAHBfrequency();
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

} // namespace stm32f4xx
} // namespace microhal

#endif // CORE_STM32F4XX_H__
