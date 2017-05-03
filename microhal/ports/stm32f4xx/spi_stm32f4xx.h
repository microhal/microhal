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
        PRESCALER_2 = 0x00,                                          //!< PRESCALER_2
        PRESCALER_4 = SPI_CR1_BR_0,                                  //!< PRESCALER_4
        PRESCALER_8 = SPI_CR1_BR_1,                                  //!< PRESCALER_8
        PRESCALER_16 = SPI_CR1_BR_1 | SPI_CR1_BR_0,                  //!< PRESCALER_16
        PRESCALER_32 = SPI_CR1_BR_2,                                 //!< PRESCALER_32
        PRESCALER_64 = SPI_CR1_BR_2 | SPI_CR1_BR_0,                  //!< PRESCALER_64
        PRESCALER_128 = SPI_CR1_BR_2 | SPI_CR1_BR_1,                 //!< PRESCALER_128
        PRESCALER_256 = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0,  //!< PRESCALER_256
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

    bool getMISOstate() final { return microhal::stm32f4xx::GPIO::get(misoPort, misoPin); }

    bool isEnabled() { return spi.CR1 & SPI_CR1_SPE; }

    void enable() final { spi.CR1 |= SPI_CR1_SPE; }
    void disable() final { spi.CR1 &= ~SPI_CR1_SPE; }
    uint32_t speed(uint32_t speed) final {
        // TODO
        while (1)
            ;
        return speed;
    }

    uint32_t frequency() const {
        const uint16_t prescalerValues[] = {2, 4, 8, 16, 32, 64, 128, 256};
        return ClockManager::SPIFrequency(spi) / prescalerValues[static_cast<uint32_t>(prescaler()) >> 3];
    }

 protected:
    //---------------------------------------- variables ----------------------------------------//
    SPI_TypeDef &spi;
    microhal::stm32f4xx::GPIO::Pin misoPin;
    microhal::stm32f4xx::GPIO::Port misoPort;
//--------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SPI(SPI_TypeDef &spi, stm32f4xx::GPIO::IOPin misoPin)
        : spi(spi), misoPin(misoPin.pin), misoPort(misoPin.port) {
    }
    void busyWait() {
        while (!(spi.SR & SPI_SR_TXE)) {
        }
        while (spi.SR & SPI_SR_BSY) {
        }
    }

    void enableInterrupt(uint32_t priority) {
    	NVIC_EnableIRQ(irq());
        NVIC_SetPriority(irq(), priority);
    }

    IRQn_Type irq() {
        if (&spi == SPI1) return SPI1_IRQn;
#if defined(SPI2)
        else if (&spi == SPI2) return SPI2_IRQn;
#endif
#if defined(SPI3)
        else if (&spi == SPI3) return SPI3_IRQn;
#endif
#if defined(SPI4)
        else if (&spi == SPI4) return SPI4_IRQn;
#endif
#if defined(SPI5)
        else if (&spi == SPI5) return SPI5_IRQn;
#endif
#if defined(SPI6)
        else if (&spi == SPI6) return SPI6_IRQn;
#endif
        std::terminate();
    }


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
    //----------------------------------------- friends -----------------------------------------//
    friend microhal::SPIDevice;
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // SPI_STM32F4XX_H_
