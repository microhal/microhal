/*
 * adc_stm32f0xx.h
 *
 *  Created on: 12 cze 2014
 *      Author: Dell
 */

#ifndef ADC_STM32F4XX_H_
#define ADC_STM32F4XX_H_

#include "stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

class Adc {
public:
	/**
	 * @brief Possible ADC channals
	 */
	typedef enum {
		CHANNEL_0 = ((uint32_t) 0x00000001),
		CHANNEL_1 = ((uint32_t) 0x00000002),
		CHANNEL_2 = ((uint32_t) 0x00000004),
		CHANNEL_3 = ((uint32_t) 0x00000008),
		CHANNEL_4 = ((uint32_t) 0x00000040),
		CHANNEL_5 = ((uint32_t) 0x00000020),
		CHANNEL_6 = ((uint32_t) 0x00000040),
		CHANNEL_7 = ((uint32_t) 0x00000080),
		CHANNEL_8 = ((uint32_t) 0x00000100),
		CHANNEL_9 = ((uint32_t) 0x00000200),
		CHANNEL_10 = ((uint32_t) 0x00000400),
		CHANNEL_11 = ((uint32_t) 0x00000800),
		CHANNEL_12 = ((uint32_t) 0x00001000),
		CHANNEL_13 = ((uint32_t) 0x00002000),
		CHANNEL_14 = ((uint32_t) 0x00004000),
		CHANNEL_15 = ((uint32_t) 0x00010800),
		CHANNEL_16 = ((uint32_t) 0x00020000),
		CHANNEL_17 = ((uint32_t) 0x00040000),
		CHANNEL_18 = ((uint32_t) 0x00080000),
	} Channel;
	/**
	 * @brief Possible ADC resolution
	 */
	typedef enum {
		RESOLUTION_12BIT = 0x00,
		RESOLUTION_10BIT = 0x08,
		RESOLUTION_8BIT = 0x10,
		RESOLUTION_6BIT = 0x18,
	} Resolution;

	typedef enum{
		ADCCLK = 0x00000000,
		PCLK_2 = 0x40000000,
		PCLK_4 = 0x80000000
	}ClkMode;

	static Adc adc1;

	bool startConversion() {
//		uint32_t cr = adc.CR1;
//		if ((cr & (ADC_CR1_ADEN | ADC_CR1_ADDIS | ADC_CR1_ADSTART)) == ADC_CR1_ADEN) {
//			cr |= ADC_CR1_ADSTART;
//			adc.CR1 = cr;
//			return true;
//		}

		return false;
	}

	bool stopConversion() {
		//adc.CR1 |= ADC_CR1_ADSTP;
		return false;
	}

	bool enable() {
		//adc.CR1 |= ADC_CR1_ADEN;
		return false;
	}

	bool disable() {
		//adc.CR1 &= ~ADC_CR1_ADEN;
		return false;
	}

	bool setResolution(Resolution resolution) {
		uint32_t cr = adc.CR1;
		//clear actual resolution
		cr &= ~(RESOLUTION_12BIT | RESOLUTION_10BIT | RESOLUTION_8BIT
				| RESOLUTION_6BIT);
		//set new parameter
		cr |= resolution;
		//update CR register
		adc.CR1 = cr;
		return true;
	}

	bool selectChannel(Channel channel) {
		// todo
		channel = channel;
		//adc.CHSELR |= channel;
		return false;
	}

	bool deselectChannel(Channel channel) {
		//adc.CHSELR &= ~channel;
		return false;
	}

	bool readSamples(uint16_t &samples) {
		samples = adc.DR;
		return true;
	}

	bool waitForConversionEnd(uint32_t ms=10000) {
		while (ms--) { //todo timeout
//			if (adc.ISR & ADC_ISR_EOC) {
//				return true;
//			}
		}
		return false;
	}

	bool waitForADCready() {
		while (1) { //todo timeout
//			if (adc.ISR & ADC_ISR_ADRDY) {
//				return true;
//			}
		}
		return false;
	}

	bool setClockMode(ClkMode mode){
		//adc.CFGR2 = mode;	//fixme add condition check
		mode = mode;
		return true;
	}

	bool calibrate(){
		//adc.CR1 |= ADC_CR1_ADCAL;
		return true;
	}
private:
	ADC_TypeDef &adc;

	Adc(ADC_TypeDef &adc) :
			adc(adc) {
		//RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
		//ADC->CCR |= ADC_CCR_VREFEN;
	}
	//virtual ~ADC();
};

} // namespace stm32f4xx
} // namespace microhal

#endif // ADC_STM32F4XX_H_
