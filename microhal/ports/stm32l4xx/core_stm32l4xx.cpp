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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "core_stm32l4xx.h"
#include "hdr/hdr_rcc.h"

namespace microhal {
namespace stm32l4xx {

uint32_t Core::core_CLK;
uint32_t Core::APB1_CLK;
uint32_t Core::APB2_CLK;

/*------------------------------------------------------------------------*//**
 * \brief Configures Flash latency.
 * \details Configures Flash latency (wait-states) which allows the chip to run
 * at higher speeds. The constants used for calculations are valid when chip's
 * voltage is in 2.7V to 3.6V range.
 *
 * \param [in] frequency defines the target frequency of the core
 *//*-------------------------------------------------------------------------*/
void Core::flash_latency(uint32_t frequency) {
    uint32_t wait_states;

    wait_states = frequency / 30000000ul; // calculate wait_states (30M is valid for 2.7V to 3.6V voltage range, use 24M for 2.4V to 2.7V, 18M for 2.1V to 2.4V or 16M for  1.8V to 2.1V)
    wait_states &= 7;						// trim to max allowed value - 7

    FLASH->ACR = wait_states;					// set wait_states, disable all caches and prefetch
    FLASH->ACR = FLASH_ACR_DCRST | FLASH_ACR_ICRST | wait_states;					// reset caches
    FLASH->ACR = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | wait_states;// enable caches and prefetch
}

/*------------------------------------------------------------------------*//**
 * \brief Starts the PLL
 * \details Configure and enable PLL to achieve some frequency with some crystal.
 * Before the speed change Flash latency is configured via flash_latency(). PLL
 * parameters are based on both function parameters. The PLL is set up,
 * started and connected. AHB clock ratio is set to 1:1 with core clock, APB1 -
 * 1:4 (max 42MHz), APB2 - 1:2 (max 84MHz). USB OTG, RNG and SDIO clock is set
 * to (VCO output / 9) (max 48MHz).
 *
 * \param [in] crystal is the frequency of the crystal resonator connected to the
 * STM32F407VG
 * \param [in] frequency is the desired target frequency after enabling the PLL
 *
 * \return real frequency that was set
 *//*-------------------------------------------------------------------------*/
bool Core::pll_start(uint32_t frequency, PLLsource source){
    uint32_t div, mul, div_core, vco_input_frequency, vco_output_frequency, frequency_core;
    uint32_t best_div = 0, best_mul = 0, best_div_core = 0, best_frequency_core = 0;
    uint32_t crystal = 0, aSource = 0;

    switch(source)
    {
    case PLLsource::MSI:
    	crystal = getMSIfrequency();
    	RCC->CR |= RCC_CR_MSION;
    	aSource = RCC_PLLCFGR_PLLSRC_MSI;
    	break;
    case PLLsource::HSI:
    	crystal = getHSIfrequency();
    	RCC->CR |= RCC_CR_HSION;
    	aSource = RCC_PLLCFGR_PLLSRC_HSI;
    	break;
    case PLLsource::HSE:
    	crystal = getHSEfrequency();
    	RCC->CR |= RCC_CR_HSEON;
    	aSource = RCC_PLLCFGR_PLLSRC_HSE;
    	break;
    }

//    RCC_CR_HSEON_bb = 1;					// enable HSE clock
    flash_latency(frequency);	// configure Flash latency for desired frequency

    for (div = 1; div <= 8; div++)			// PLLM in [1; 8]
            {
        vco_input_frequency = crystal / div;

        if ((vco_input_frequency < 1000000ul) || (vco_input_frequency > 2000000))// skip invalid settings
            continue;

        for (mul = 8; mul <= 86; mul++)	// PLLN in [8; 86 ]
                {
            vco_output_frequency = vco_input_frequency * mul;

            if ((vco_output_frequency < 64000000ul) || (vco_output_frequency > 432000000ul))// skip invalid settings
                continue;

            for (div_core = 2; div_core <= 8; div_core += 2)	// PLLP in {2, 4, 6, 8}
                    {
                frequency_core = vco_output_frequency / div_core;

                if (frequency_core > frequency)	// skip values over desired frequency
                    continue;

                if (frequency_core > best_frequency_core)// is this configuration better than previous one?
                        {
                    best_frequency_core = frequency_core;	// yes - save values
                    best_div = div;
                    best_mul = mul;
                    best_div_core = div_core;
                }
            }
        }
    }

    RCC->PLLCFGR = (best_div << RCC_PLLCFGR_PLLM_Pos) | (best_mul << RCC_PLLCFGR_PLLN_Pos)
            | ((best_div_core / 2 - 1) << RCC_PLLCFGR_PLLR_Pos) | RCC_PLLCFGR_PLLQ_DIV8
            | aSource;	// configure PLL factors, always divide USB clock by 9

    RCC->CFGR = RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_HPRE_DIV1;	// AHB - no prescaler, APB1 - divide by 4, APB2 - divide by 2

    switch(source)
    {
    case PLLsource::MSI:
    	while(!(RCC->CR & RCC_CR_MSIRDY))
    	;
		break;
    case PLLsource::HSI:
    	while(!(RCC->CR & RCC_CR_HSIRDY))
    	;
    	break;
    case PLLsource::HSE:
    	while(!(RCC->CR & RCC_CR_HSERDY))
    	;
    	break;
    }

    RCC_CR_PLLON_bb = 1;					// enable PLL
    while (!RCC_CR_PLLRDY_bb)
        ;				// wait for PLL lock

    RCC->CFGR |= RCC_CFGR_SW_PLL;			// change SYSCLK to PLL
    while (((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
        ;	// wait for switch

    core_CLK = best_frequency_core;
    APB1_CLK = best_frequency_core / 4;
    APB2_CLK = best_frequency_core / 2;

    //USARTstm::usartStm1.reBaud();
    //USARTstm::usartStm2.reBaud();
    //USARTstm::usartStm1.reBaud();

    return (best_frequency_core == frequency);
}

uint32_t Core::getAPB1frequency() {
	uint32_t cfgr = RCC->CFGR;
	uint32_t divider;

	cfgr &= RCC_CFGR_PPRE1;

	switch (cfgr) {
	case RCC_CFGR_PPRE1_2:
		divider = 2;
		break;
	case RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE1_0:
		divider = 4;
		break;
	case RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE1_1:
		divider = 8;
		break;
	case RCC_CFGR_PPRE1_2 | RCC_CFGR_PPRE1_1 | RCC_CFGR_PPRE1_0:
		divider = 16;
		break;
	default:
		divider = 1;
	}
	return getAHBfrequency() / divider;
}

uint32_t Core::getAPB2frequency() {
	uint32_t cfgr = RCC->CFGR;
	uint32_t divider;

	cfgr &= RCC_CFGR_PPRE2;

	switch (cfgr) {
	case RCC_CFGR_PPRE2_2:
		divider = 2;
		break;
	case RCC_CFGR_PPRE2_2 | RCC_CFGR_PPRE2_0:
		divider = 4;
		break;
	case RCC_CFGR_PPRE2_2 | RCC_CFGR_PPRE2_1:
		divider = 8;
		break;
	case RCC_CFGR_PPRE2_2 | RCC_CFGR_PPRE2_1 | RCC_CFGR_PPRE2_0:
		divider = 16;
		break;
	default:
		divider = 1;
	}
	return getAHBfrequency() / divider;
}

uint32_t Core::getAHBfrequency() {
	uint32_t cfgr = RCC->CFGR;
	uint32_t divider;

	cfgr &= RCC_CFGR_HPRE;

	switch (cfgr) {
	case RCC_CFGR_HPRE_3:
		divider = 2;
		break;
	case RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_0:
		divider = 4;
		break;
	case RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_1:
		divider = 8;
		break;
	case RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_0:
		divider = 16;
		break;
	case RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2:
		divider = 64;
		break;
	case RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_0:
		divider = 128;
		break;
	case RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_1:
		divider = 256;
		break;
	case RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_0:
		divider = 512;
		break;
	default:
		divider = 1;
	}
	return getSYSCLKfrequency() / divider;
}
////////////////////////////////////////////////////////////////////////////////
} // namespace stm32l4xx
} // namespace microhal




