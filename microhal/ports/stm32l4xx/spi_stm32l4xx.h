/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 18-09-2016
 * last modification: 18-09-2016
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

#ifndef _MICROHAL_SPI_STM32L4XX_H_
#define _MICROHAL_SPI_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "gpio_stm32l4xx.h"
#include "interfaces/spi_interface.h"
#include "microhalPortConfig_stm32l4xx.h"
#include "device/stm32l4xx.h"

namespace microhal {
class SPIDevice;
namespace stm32l4xx {
extern "C" {
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void SPI3_IRQHandler(void);
}
/* **************************************************************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup stm32l4xx_port
 * @{
 * @class SPI
 * @}
 * \brief   This class implements SPI functions.
 */
class SPI : public microhal::SPI {
    friend class IOManager;

 public:
    //---------------------------------------- typedefs
    //-----------------------------------------//
    /**
     *
     */
    typedef enum {
        PRESCALER_2 = 0x00,                                          //!< PRESCALER_2
        PRESCALER_4 = SPI_CR1_BR_0,                                  //!< PRESCALER_4
        PRESCALER_8 = SPI_CR1_BR_1,                                  //!< PRESCALER_8
        PRESCALER_16 = SPI_CR1_BR_1 | SPI_CR1_BR_0,                  //!< PRESCALER_16
        PRESCALER_32 = SPI_CR1_BR_2,                                 //!< PRESCALER_32
        PRESCALER_64 = SPI_CR1_BR_2 | SPI_CR1_BR_0,                  //!< PRESCALER_64
        PRESCALER_128 = SPI_CR1_BR_2 | SPI_CR1_BR_1,                 //!< PRESCALER_128
        PRESCALER_256 = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0,  //!< PRESCALER_256
    } Prescaler;
//------------------------------------- static reference
//--------------------------------------//
#if (defined MICROHAL_USE_SPI1_INTERRUPT) || (defined MICROHAL_USE_SPI1_POLLING) || (defined MICROHAL_USE_SPI1_DMA)
    static SPI &spi1;
#endif
#if (defined MICROHAL_USE_SPI2_INTERRUPT) || (defined MICROHAL_USE_SPI2_POLLING) || (defined MICROHAL_USE_SPI2_DMA)
    static SPI &spi2;
#endif
#if (defined MICROHAL_USE_SPI3_INTERRUPT) || (defined MICROHAL_USE_SPI3_POLLING) || (defined MICROHAL_USE_SPI3_DMA)
    static SPI &spi3;
#endif
    //---------------------------------------- functions
    //----------------------------------------//
    bool setMode(Mode mode) override final {
        const uint32_t modeFlags[] = {0x00, SPI_CR1_CPHA, SPI_CR1_CPOL, SPI_CR1_CPHA | SPI_CR1_CPOL};

        if(isEnabled())
        	disable();
        spi.CR1 = (spi.CR1 & ~(SPI_CR1_CPHA | SPI_CR1_CPOL)) | modeFlags[mode];

        enable();
        return true;


//        if (isEnabled() == false) {
//            spi.CR1 = (spi.CR1 & ~(SPI_CR1_CPHA | SPI_CR1_CPOL)) | modeFlags[mode];
//            return true;
//        }
//        return false;
    }

    void init(Mode mode, Prescaler prescaler, bool noLock = false) {
        const uint32_t modeFlags[] = {0x00, SPI_CR1_CPHA, SPI_CR1_CPOL, SPI_CR1_CPHA | SPI_CR1_CPOL};
   //     spi.CR1 = /*SPI_CR1_MSTR |*/ /*SPI_CR1_SSM*/ | SPI_CR1_SSI | modeFlags[mode] | prescaler; // SPI_CR1_MSTR testing -> slave mode

        spi.CR1 = SPI_CR1_SSM | modeFlags[mode] | prescaler;
        spi.CR2 = SPI_CR2_FRXTH | SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;
//        spi.CR1 = 512;
//        spi.CR2 = 5888;

        this->noLock = noLock;
    }

    bool getMISOstate() override final { return microhal::stm32l4xx::GPIO::get(misoPort, misoPin); }

    bool isEnabled() { return spi.CR1 & SPI_CR1_SPE; }

    void enable() override final { spi.CR1 |= SPI_CR1_SPE; }
    void disable() override final { spi.CR1 &= ~SPI_CR1_SPE; }
    uint32_t speed(uint32_t speed) final {}

 protected:
    //---------------------------------------- variables
    //----------------------------------------//
    SPI_TypeDef &spi;
    microhal::stm32l4xx::GPIO::Pin misoPin;
    microhal::stm32l4xx::GPIO::Port misoPort;
//--------------------------------------- constructors
//--------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SPI(SPI_TypeDef &spi, stm32l4xx::GPIO::IOPin misoPin)
        : spi(spi), misoPin(misoPin.pin), misoPort(misoPin.port) {
    }
    // virtual ~SPI() {
    //}

    static SPI::Error errorCheck(uint32_t SRregisterValue) {
        SPI::Error error = NoError;

        // Master Mode fault event
        if (SRregisterValue & SPI_SR_MODF) {
            error = MasterModeFault;
        }
        // Overrun error
        if (SRregisterValue & SPI_SR_OVR) {
            error = OverrunError;
        }
        // CRC error flag
        if (SRregisterValue & SPI_SR_CRCERR) {
            error = CRCError;
        }
        // TI frame format error
        // if (SRregisterValue & SPI_SR_FRE) {
        //
        // }
        return error;
    }
    //----------------------------------------- friends
    //-----------------------------------------//
    friend microhal::SPIDevice;
};

}  // namespace stm32flxx
}  // namespace microhal
#endif  // _MICROHAL_SPI_STM32L4XX_H_
