/* ************************************************************************************************
 * INCLUDES
 */
#include "core_stm32f4xx.h"
#include "hdr/hdr_rcc.h"

namespace microhal {
namespace stm32f4xx {

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
bool Core::pll_start(uint32_t crystal, uint32_t frequency) {
    uint32_t div, mul, div_core, vco_input_frequency, vco_output_frequency, frequency_core;
    uint32_t best_div = 0, best_mul = 0, best_div_core = 0, best_frequency_core = 0;

    RCC_CR_HSEON_bb = 1;					// enable HSE clock
    flash_latency(frequency);	// configure Flash latency for desired frequency

    for (div = 2; div <= 63; div++)			// PLLM in [2; 63]
            {
        vco_input_frequency = crystal / div;

        if ((vco_input_frequency < 1000000ul) || (vco_input_frequency > 2000000))// skip invalid settings
            continue;

        for (mul = 64; mul <= 432; mul++)	// PLLN in [64; 432]
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

    RCC->PLLCFGR = (best_div << RCC_PLLCFGR_PLLM_bit) | (best_mul << RCC_PLLCFGR_PLLN_bit)
            | ((best_div_core / 2 - 1) << RCC_PLLCFGR_PLLP_bit) | RCC_PLLCFGR_PLLQ_DIV9
            | RCC_PLLCFGR_PLLSRC_HSE;	// configure PLL factors, always divide USB clock by 9

    RCC->CFGR = RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_HPRE_DIV1;	// AHB - no prescaler, APB1 - divide by 4, APB2 - divide by 2

    while (!RCC_CR_HSERDY_bb)
        ;				// wait for stable clock

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
} // namespace stm32f4xx
} // namespace microhal
