/*
 * spi_stm32f4xx.h
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */

#ifndef SPI_STM32F4XX_H_
#define SPI_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cmath>
#include <limits>
#include "IOPin.h"
#include "clockManager.h"
#include "device/stm32f4xx.h"
#include "gpio_stm32f4xx.h"
#include "interfaces/spi_interface.h"
#include "microhalPortConfig_stm32f4xx.h"

namespace microhal {
class SPIDevice;
namespace stm32f4xx {
extern "C" {
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void SPI3_IRQHandler(void);
void SPI4_IRQHandler(void);
void SPI5_IRQHandler(void);
void SPI6_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup stm32f4xx_port
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
#if (defined MICROHAL_USE_SPI4_INTERRUPT) || (defined MICROHAL_USE_SPI4_POLLING) || (defined MICROHAL_USE_SPI4_DMA)
    static SPI &spi4;
#endif
#if (defined MICROHAL_USE_SPI5_INTERRUPT) || (defined MICROHAL_USE_SPI5_POLLING) || (defined MICROHAL_USE_SPI5_DMA)
    static SPI &spi5;
#endif
#if (defined MICROHAL_USE_SPI6_INTERRUPT) || (defined MICROHAL_USE_SPI6_POLLING) || (defined MICROHAL_USE_SPI6_DMA)
    static SPI &spi6;
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
    }

    void prescaler(Prescaler prescaler) { spi.CR1 = (spi.CR1 & ~(SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)) | prescaler; }

    Prescaler prescaler() const { return static_cast<Prescaler>(spi.CR1 & (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)); }

    bool getMISOstate() final { return misoPin.get(); }

    bool isEnabled() { return spi.CR1 & SPI_CR1_SPE; }

    void enable() final { spi.CR1 |= SPI_CR1_SPE; }
    void disable() final { spi.CR1 &= ~SPI_CR1_SPE; }
    /**@brief Set SPI CLK frequency.
     *
     * @param speed - CLK frequency in Hz
     * @return speed value that was set, this is closest value that was possible to set with current bus speed.
     */
    uint32_t speed(uint32_t speed) final {
        auto freq = ClockManager::SPIFrequency(spi);
        auto requiredPrescaler = freq / speed;
        prescaler(findClosestPrescaler(requiredPrescaler));

        return this->speed();
    }

    uint32_t speed() const final {
        const uint16_t prescalerValues[] = {2, 4, 8, 16, 32, 64, 128, 256};
        return ClockManager::SPIFrequency(spi) / prescalerValues[static_cast<uint32_t>(prescaler()) >> 3];
    }

 protected:
    //---------------------------------------- variables ----------------------------------------//
    SPI_TypeDef &spi;
    // microhal::stm32f4xx::IOPin misoPin;
    microhal::stm32f4xx::GPIO misoPin;
//--------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SPI(SPI_TypeDef &spi, stm32f4xx::IOPin misoPin)
        : spi(spi), misoPin(misoPin) {
    }
    void busyWait() {
        while (!(spi.SR & SPI_SR_TXE)) {
        }
        while (spi.SR & SPI_SR_BSY) {
        }
    }

    Prescaler findClosestPrescaler(uint32_t prescaler) {
        const int32_t prescalerValues[] = {2, 4, 8, 16, 32, 64, 128, 256};
        uint32_t bestMatch = std::numeric_limits<uint32_t>::max();
        size_t bestPos = 0;
        for (size_t i = 0; i < sizeof(prescalerValues) / sizeof(prescalerValues[0]); i++) {
            uint32_t distance = std::abs(prescalerValues[i] - static_cast<int32_t>(prescaler));
            if (bestMatch > distance) {
                bestMatch = distance;
                bestPos = i;
            }
        }
        return static_cast<Prescaler>(bestPos << 3);
    }

    void enableInterrupt(uint32_t priority) {
        NVIC_EnableIRQ(irq());
        NVIC_SetPriority(irq(), priority);
    }

    IRQn_Type irq() {
        if (&spi == SPI1) return SPI1_IRQn;
#if defined(SPI2)
        else if (&spi == SPI2)
            return SPI2_IRQn;
#endif
#if defined(SPI3)
        else if (&spi == SPI3)
            return SPI3_IRQn;
#endif
#if defined(SPI4)
        else if (&spi == SPI4)
            return SPI4_IRQn;
#endif
#if defined(SPI5)
        else if (&spi == SPI5)
            return SPI5_IRQn;
#endif
#if defined(SPI6)
        else if (&spi == SPI6)
            return SPI6_IRQn;
#endif
        std::terminate();
    }

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
    //----------------------------------------- friends -----------------------------------------//
    friend microhal::SPIDevice;
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // SPI_STM32F4XX_H_
