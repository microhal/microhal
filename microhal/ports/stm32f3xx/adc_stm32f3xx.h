/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-08-2018
 * last modification: 02-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_ADC_STM32F3XX_H_
#define _MICROHAL_ADC_STM32F3XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */

#include "device/stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class Adc final {
 public:
    /**
     * @brief Possible ADC channels
     */
    typedef enum : uint32_t {
        Channel0 = 0x00000001,
        Channel1 = 0x00000002,
        Channel2 = 0x00000004,
        Channel3 = 0x00000008,
        Channel4 = 0x00000040,
        Channel5 = 0x00000020,
        Channel6 = 0x00000040,
        Channel7 = 0x00000080,
        Channel8 = 0x00000100,
        Channel9 = 0x00000200,
        Channel10 = 0x00000400,
        Channel11 = 0x00000800,
        Channel12 = 0x00001000,
        Channel13 = 0x00002000,
        Channel14 = 0x00004000,
        Channel15 = 0x00010800,
        Channel16 = 0x00020000,
        Channel17 = 0x00040000,
        Channel18 = 0x00080000,
    } Channel;
    /**
     * @brief Possible ADC resolution
     */
    typedef enum {
        Resolution_12Bit = 0x00,
        Resolution_10Bit = ADC_CFGR_RES_0,
        Resolution_8Bit = ADC_CFGR_RES_1,
        Resolution_6Bit = ADC_CFGR_RES_0 | ADC_CFGR_RES_1,
    } Resolution;

    typedef enum {
        ADCCLK = 0x00,
        PCLK = ADC12_CCR_CKMODE_0,  //!< This option can be used only when AHB clock prescaler is set to 1
        PCLK_Half = ADC12_CCR_CKMODE_1,
        PCLK_Quater = ADC12_CCR_CKMODE_0 | ADC12_CCR_CKMODE_1
    } ClkMode;
    /**
     *
     * @retval true if conversion was started
     * @retval false when conversion start is impossible. This may be caused by disabled ADC or ADC conversion is ongoing or stop conversion request
     * is ongoing.
     */
    bool startConversion() {
        // Software is allowed to set ADSTART only when ADEN=1 and ADDIS=0 (ADC is enabled and there is no pending request to disable the ADC)
        uint32_t cr = adc.CR;
        if ((cr & (ADC_CR_ADEN | ADC_CR_ADDIS | ADC_CR_ADSTART)) == ADC_CR_ADEN) {
            cr |= ADC_CR_ADSTART;
            adc.CR = cr;
            return true;
        }
        return false;
    }
    /**
     * Stop ongoing conversion.
     */
    bool stopConversion() {
        // Setting ADSTP to �1� is only effective when ADSTART=1 and ADDIS=0 (ADC is enabled and may be converting and there is no pending request to
        // disable the ADC)
        uint32_t cr = adc.CR;
        if ((cr & (ADC_CR_ADSTART | ADC_CR_ADDIS)) == ADC_CR_ADSTART) {
            adc.CR |= ADC_CR_ADSTP;
            return true;
        }
        return false;
    }

    bool enable() {
        // Software is allowed to set ADEN only when all bits of ADC_CR registers are 0 (ADCAL=0, ADSTP=0, ADSTART=0, ADDIS=0 and ADEN=0)
        uint32_t cr = adc.CR;
        if ((cr & (ADC_CR_ADCAL | ADC_CR_JADSTART | ADC_CR_ADSTP | ADC_CR_ADSTART | ADC_CR_ADDIS | ADC_CR_ADEN)) == 0) {
            adc.CR |= ADC_CR_ADEN;
            return true;
        }
        return false;
    }

    bool disable() {
        // Setting ADDIS to �1� is only effective when ADEN=1 and ADSTART=0 (which ensures that no conversion is ongoing)
        uint32_t cr = adc.CR;
        if ((cr & (ADC_CR_ADEN | ADC_CR_ADSTART | ADC_CR_JADSTART)) == ADC_CR_ADEN) {
            adc.CR |= ADC_CR_ADDIS;
            return true;
        }
        return false;
    }

    bool isEnabled() { return adc.CR & ADC_CR_ADEN; }

    bool setResolution(Resolution resolution) {
        if (isEnabled() == false) {
            uint32_t cfgr = adc.CFGR;
            // clear actual resolution
            cfgr &= ~(Resolution_12Bit | Resolution_10Bit | Resolution_8Bit | Resolution_6Bit);
            // set new parameter
            cfgr |= resolution;
            // update CR register
            adc.CFGR = cfgr;
            return true;
        }
        return false;
    }
    /**
     *
     * @param sequenceLength
     * @param sequencePosition
     * @param channel number form 1 to 18
     * @return
     */
    bool configureSamplingSequence(uint16_t sequenceLength, uint16_t sequencePosition, Channel channel) {
        if ((sequenceLength == 0) || (sequenceLength > 16)) return false;
        if (channel == Channel::Channel0) return false;
        if (adc.CR & ADC_CR_ADSTART) return false;

        uint32_t sqr1 = adc.SQR1;
        sqr1 &= ~0b1111;
        sqr1 |= sequenceLength - 1;
        if (sequencePosition <= 4) {
            sqr1 &= ~(0b11111 << ((sequencePosition * 6) + 6));
            sqr1 |= (channel << ((sequencePosition * 6) + 6));
        } else if (sequencePosition <= 9) {
            uint32_t sqr = adc.SQR2;
            sqr &= ~(0b11111 << ((sequencePosition - 5) * 6));
            sqr |= (channel << ((sequencePosition - 5) * 6));
            adc.SQR2 = sqr;
        } else if (sequencePosition <= 14) {
            uint32_t sqr = adc.SQR3;
            sqr &= ~(0b11111 << ((sequencePosition - 10) * 6));
            sqr |= (channel << ((sequencePosition - 10) * 6));
            adc.SQR3 = sqr;
        } else if (sequencePosition <= 16) {
            uint32_t sqr = adc.SQR4;
            sqr &= ~(0b11111 << ((sequencePosition - 15) * 6));
            sqr |= (channel << ((sequencePosition - 15) * 6));
            adc.SQR4 = sqr;
        } else {
            return false;
        }

        adc.SQR1 = sqr1;
        return true;
    }

    uint16_t readSamples() { return adc.DR; }

    bool waitForConversionEnd(uint32_t ms = 10000) {
        while (ms--) {
            if (adc.ISR & ADC_ISR_EOC) {
                return true;
            }
        }
        return false;
    }

    bool waitForADCready(uint32_t ms = 10000) {
        while (ms--) {
            if (adc.ISR & ADC_ISR_ADRDY) {
                return true;
            }
        }
        return false;
    }

    bool setClockMode(ClkMode mode) {
        // Software is allowed to write these bits only when the ADC is disabled (ADCAL=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
        if (isEnabled() == false) {
            ADC12_COMMON->CCR |= mode;
            return true;
        }
        return false;
    }
    /**
     * @note Calibrate function can be called only when ADC is disabled.
     */
    bool calibrate() {
        // Software is allowed to set ADCAL only when the ADC is disabled (ADCAL=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
        uint32_t cr = adc.CR;
        if ((cr & (ADC_CR_ADCAL | ADC_CR_ADSTART | ADC_CR_ADSTP | ADC_CR_ADDIS | ADC_CR_ADEN)) == 0) {
            adc.CR |= ADC_CR_ADCAL;
            return true;
        }
        return false;
    }

 public:
    Adc(ADC_TypeDef &adc) : adc(adc) {
        RCC->AHBENR |= RCC_AHBENR_ADC12EN;
        RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_4 | RCC_CFGR2_ADCPRE12_3;
        enableVoltageRegulator();

        ADC12_COMMON->CCR |= ADC_CCR_VREFEN;
    }
    ~Adc() {
        stopConversion();
        disable();
        RCC->AHBENR &= ~RCC_AHBENR_ADC12EN;
        RCC->CFGR2 &= (~RCC_CFGR2_ADCPRE12_4 | RCC_CFGR2_ADCPRE12_3 | RCC_CFGR2_ADCPRE12_2 | RCC_CFGR2_ADCPRE12_1 | RCC_CFGR2_ADCPRE12_0);
    }

 private:
    ADC_TypeDef &adc;

    void enableVoltageRegulator() {
        adc.CR &= ~(0b11 << ADC_CR_ADVREGEN_Pos);
        adc.CR |= 0b01 << ADC_CR_ADVREGEN_Pos;

        // wait for adc voltage regulator enable
        while ((adc.CR & (0b11 << ADC_CR_ADVREGEN_Pos)) == 0) {
        }
    }

    void disableVoltageRegulator() {
        adc.CR &= ~(0b11 << ADC_CR_ADVREGEN_Pos);
        adc.CR |= 0b10 << ADC_CR_ADVREGEN_Pos;
    }
};
}  // namespace stm32f3xx
}  // namespace microhal
#endif  // _MICROHAL_ADC_STM32F3XX_H_
