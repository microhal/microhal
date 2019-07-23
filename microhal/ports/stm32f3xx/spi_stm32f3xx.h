/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 31-08-2016
 * last modification: 31-08-2016
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

#ifndef _MICROHAL_SPI_STM32F3XX_H_
#define _MICROHAL_SPI_STM32F3XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "clockManager.h"
#include "device/stm32f3xx.h"
#include "gpio_stm32f3xx.h"
#include "interfaces/spi_interface.h"
#include "microhalPortConfig_stm32f3xx.h"

/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {

class SPIDevice;
namespace stm32f3xx {
extern "C" {
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void SPI3_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup stm32f3xx_port
 * @{
 * @class SPI
 * @}
 * \brief   This class implements SPI functions.
 */
class SPI : public microhal::SPI {
    friend class IOManager;

 public:
    //---------------------------------------- typedefs -----------------------------------------//
    /**
     *
     */
    typedef enum {
        Prescaler2 = 0x00,                                          //!< Prescaler2
        Prescaler4 = SPI_CR1_BR_0,                                  //!< Prescaler4
        Prescaler8 = SPI_CR1_BR_1,                                  //!< Prescaler8
        Prescaler16 = SPI_CR1_BR_1 | SPI_CR1_BR_0,                  //!< Prescaler16
        Prescaler32 = SPI_CR1_BR_2,                                 //!< Prescaler32
        Prescaler64 = SPI_CR1_BR_2 | SPI_CR1_BR_0,                  //!< Prescaler64
        Prescaler128 = SPI_CR1_BR_2 | SPI_CR1_BR_1,                 //!< Prescaler128
        Prescaler256 = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0,  //!< Prescaler256
    } Prescaler;

//------------------------------------- static reference --------------------------------------//
#if (defined MICROHAL_USE_SPI1_INTERRUPT) || (defined MICROHAL_USE_SPI1_POLLING) || (defined MICROHAL_USE_SPI1_DMA)
    static SPI &spi1;
#endif
#if (defined MICROHAL_USE_SPI2_INTERRUPT) || (defined MICROHAL_USE_SPI2_POLLING) || (defined MICROHAL_USE_SPI2_DMA)
    static SPI &spi2;
#endif
#if (defined MICROHAL_USE_SPI3_INTERRUPT) || (defined MICROHAL_USE_SPI3_POLLING) || (defined MICROHAL_USE_SPI3_DMA)
    static SPI &spi3;
#endif
    //---------------------------------------- functions ----------------------------------------//
    bool setMode(Mode mode) final {
        const uint32_t modeFlags[] = {0x00, SPI_CR1_CPHA, SPI_CR1_CPOL, SPI_CR1_CPHA | SPI_CR1_CPOL};

        if (isEnabled() == false) {
            spi.CR1 = (spi.CR1 & ~(SPI_CR1_CPHA | SPI_CR1_CPOL)) | modeFlags[mode];
            return true;
        }
        return false;
    }

    void init(Mode mode, Prescaler prescaler) {
        const uint32_t modeFlags[] = {0x00, SPI_CR1_CPHA, SPI_CR1_CPOL, SPI_CR1_CPHA | SPI_CR1_CPOL};
        spi.CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | modeFlags[mode] | prescaler;
        dataSize(8);
        spi.CR2 |= SPI_CR2_FRXTH;
    }

    void prescaler(Prescaler prescaler) { spi.CR1 = (spi.CR1 & ~(SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)) | prescaler; }

    Prescaler prescaler() const { return static_cast<Prescaler>(spi.CR1 & (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)); }

    bool getMISOstate() final { return misoPin.get(); }

    bool isEnabled() { return spi.CR1 & SPI_CR1_SPE; }

    void enable() final { spi.CR1 |= SPI_CR1_SPE; }
    void disable() final { spi.CR1 &= ~SPI_CR1_SPE; }

    uint32_t dataSize(uint32_t size) {
        if (size < 4 || size > 16) return 0;
        spi.CR2 = (spi.CR2 & ~SPI_CR2_DS_Msk) | ((size - 1) << SPI_CR2_DS_Pos);
        return size;
    }

    uint32_t dataSize() const { return ((spi.CR2 & SPI_CR2_DS_Msk) >> SPI_CR2_DS_Pos) + 1; }

    uint32_t speed(uint32_t speed) final {
        // TODO
        return speed;
    }

    uint32_t speed() const final {
        const uint16_t prescalerValues[] = {2, 4, 8, 16, 32, 64, 128, 256};
        return ClockManager::SPIFrequency(spi) / prescalerValues[static_cast<uint32_t>(prescaler()) >> 3];
    }

 protected:
    //---------------------------------------- variables ----------------------------------------//
    SPI_TypeDef &spi;
    microhal::stm32f3xx::GPIO misoPin;
//--------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SPI(SPI_TypeDef &spi, stm32f3xx::IOPin misoPin)
        : spi(spi), misoPin(misoPin) {
    }
    // virtual ~SPI() {
    //}

    static SPI::Error errorCheck(uint32_t SRregisterValue) {
        SPI::Error error = Error::None;

        // Master Mode fault event
        if (SRregisterValue & SPI_SR_MODF) {
            error = Error::MasterModeFault;
        }
        // Overrun error
        if (SRregisterValue & SPI_SR_OVR) {
            error = Error::Overrun;
        }
        // CRC error flag
        if (SRregisterValue & SPI_SR_CRCERR) {
            error = Error::Crc;
        }
        // TI frame format error
        // if (SRregisterValue & SPI_SR_FRE) {
        //
        // }
        return error;
    }

    void priority(uint32_t priority) {
        switch (reinterpret_cast<uint32_t>(&spi)) {
#if defined(SPI1)
            case reinterpret_cast<uint32_t>(SPI1_BASE):
                NVIC_EnableIRQ(SPI1_IRQn);
                NVIC_SetPriority(SPI1_IRQn, priority);
                break;
#endif
#if defined(SPI2)
            case reinterpret_cast<uint32_t>(SPI2_BASE):
                NVIC_EnableIRQ(SPI2_IRQn);
                NVIC_SetPriority(SPI2_IRQn, priority);
                break;
#endif
#if defined(SPI3)
            case reinterpret_cast<uint32_t>(SPI3_BASE):
                NVIC_EnableIRQ(SPI3_IRQn);
                NVIC_SetPriority(SPI3_IRQn, priority);
                break;
#endif
            default:
                std::terminate();
        }
    }

    void writeDR(uint8_t data) { *(volatile uint8_t *)&spi.DR = data; }

    uint8_t readDR() const { return *(uint8_t *)&spi.DR; }
    //----------------------------------------- friends -----------------------------------------//
    friend microhal::SPIDevice;
};

}  // namespace stm32f3xx
}  // namespace microhal
#endif  // _MICROHAL_SPI_STM32F3XX_H_
