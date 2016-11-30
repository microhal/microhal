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
#include "gpio_stm32f4xx.h"
#include "interfaces/spi_interface.h"
#include "microhalPortConfig_stm32f4xx.h"
#include "stm32f4xx.h"

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
#if (defined MICROHAL_USE_SPI4_INTERRUPT) || (defined MICROHAL_USE_SPI4_POLLING) || (defined MICROHAL_USE_SPI4_DMA)
    static SPI &spi4;
#endif
#if (defined MICROHAL_USE_SPI5_INTERRUPT) || (defined MICROHAL_USE_SPI5_POLLING) || (defined MICROHAL_USE_SPI5_DMA)
    static SPI &spi5;
#endif
#if (defined MICROHAL_USE_SPI6_INTERRUPT) || (defined MICROHAL_USE_SPI6_POLLING) || (defined MICROHAL_USE_SPI6_DMA)
    static SPI &spi6;
#endif
    //---------------------------------------- functions
    //----------------------------------------//
    bool setMode(Mode mode) override final {
        const uint32_t modeFlags[] = {0x00, SPI_CR1_CPHA, SPI_CR1_CPOL, SPI_CR1_CPHA | SPI_CR1_CPOL};

        if (isEnabled() == false) {
            spi.CR1 = (spi.CR1 & ~(SPI_CR1_CPHA | SPI_CR1_CPOL)) | modeFlags[mode];
            return true;
        }
        return false;
    }

    void init(Mode mode, Prescaler prescaler, bool noLock = false) {
        const uint32_t modeFlags[] = {0x00, SPI_CR1_CPHA, SPI_CR1_CPOL, SPI_CR1_CPHA | SPI_CR1_CPOL};
        spi.CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | modeFlags[mode] | prescaler;
        this->noLock = noLock;
    }

    void setPrescaler(Prescaler prescaler) {
    	spi.CR1 = (spi.CR1 & ~(SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)) | prescaler;
    }

    bool getMISOstate() override final { return microhal::stm32f4xx::GPIO::get(misoPort, misoPin); }

    bool isEnabled() { return spi.CR1 & SPI_CR1_SPE; }

    void enable() override final { spi.CR1 |= SPI_CR1_SPE; }
    void disable() override final { spi.CR1 &= ~SPI_CR1_SPE; }
    uint32_t speed(uint32_t speed) final {}

 protected:
    //---------------------------------------- variables
    //----------------------------------------//
    SPI_TypeDef &spi;
    microhal::stm32f4xx::GPIO::Pin misoPin;
    microhal::stm32f4xx::GPIO::Port misoPort;
//--------------------------------------- constructors
//--------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SPI(SPI_TypeDef &spi, stm32f4xx::GPIO::IOPin misoPin)
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

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // SPI_STM32F4XX_H_
