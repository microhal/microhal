/*
 * spi_stm32f4xx.h
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_SPI_STMCOMMON_H_
#define _MICROHAL_SPI_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include <ports/stmCommon/driverConfiguration_stmCommon.h>
#if _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#include "spi_preprocessor_macros.h"
#if ENABLED_ANY_SPI(POLLING) || ENABLED_ANY_SPI(INTERRUPT) || ENABLED_ANY_SPI(DMA)  // Check if driver is enabled in microhal port config
/* ************************************************************************************************
 * INCLUDES
 */
#include <cmath>
#include <limits>
#include "../IOPin.h"
#include "../clockManager/spiClock.h"
#include "../registers/spiRegisters_v1.h"
#include "../stmCommonDefines.h"
#include "interfaces/spi_interface.h"
#include _MICROHAL_INCLUDE_PORT_GPIO
#include _MICROHAL_INCLUDE_PORT_DEVICE

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
class SPIDevice;
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
extern "C" {
void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void SPI3_IRQHandler(void);
void SPI4_IRQHandler(void);
void SPI5_IRQHandler(void);
void SPI6_IRQHandler(void);
}

namespace spiDetail {
enum class Interrupt { TransmitterEmpty = 0b01, ReceiverNotEmpty = 0b10 };

constexpr Interrupt operator|(Interrupt a, Interrupt b) {
    uint32_t tmp = static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
    return static_cast<Interrupt>(tmp);
}

constexpr Interrupt operator&(Interrupt a, Interrupt b) {
    uint32_t tmp = static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
    return static_cast<Interrupt>(tmp);
}
}  // namespace spiDetail

using spiDetail::operator&;
using spiDetail::operator|;
/* ************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup STM Common
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
        Prescaler2 = 0x00,  //!< Prescaler2
        Prescaler4,         //!< Prescaler4
        Prescaler8,         //!< Prescaler8
        Prescaler16,        //!< Prescaler16
        Prescaler32,        //!< Prescaler32
        Prescaler64,        //!< Prescaler64
        Prescaler128,       //!< Prescaler128
        Prescaler256,       //!< Prescaler256
    } Prescaler;
    //---------------------------------------- functions ----------------------------------------//
    bool setMode(Mode mode) final {
        const uint32_t modeFlags[] = {0b00, 0b01, 0b10, 0b11};

        if (isEnabled() == false) {
            auto cr1 = spi.cr1.volatileLoad();
            cr1.mode = modeFlags[mode];
            spi.cr1.volatileStore(cr1);
            return true;
        }
        return false;
    }

    void init(Mode mode, Prescaler prescaler) {
        const uint32_t modeFlags[] = {0b00, 0b01, 0b10, 0b11};
        registers::SPI::CR1 cr1 = {};
        cr1.MSTR.set();
        cr1.SSM.set();
        cr1.SSI.set();
        cr1.mode = modeFlags[mode];
        cr1.BR = prescaler;
        spi.cr1.volatileStore(cr1);
#if defined(_MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH) || defined(_MICROHAL_REGISTERS_SPI_CR2_HAS_DS)
        auto cr2 = spi.cr2.volatileLoad();
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
        cr2.setDataSize(8);
#endif
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
        cr2.setFIFOThreshold(registers::SPI::CR2::RxFIFOThreshold::Quater);
#endif
        spi.cr2.volatileStore(cr2);
#endif
    }

    void prescaler(Prescaler prescaler) {
        auto cr1 = spi.cr1.volatileLoad();
        cr1.BR = prescaler;
        spi.cr1.volatileStore(cr1);
    }

    Prescaler prescaler() const { return static_cast<Prescaler>(spi.cr1.volatileLoad().BR.get()); }

    bool getMISOstate() final { return misoPin.get(); }

    bool isEnabled() { return spi.cr1.volatileLoad().SPE; }

    void enable() final {
        auto cr1 = spi.cr1.volatileLoad();
        cr1.SPE.set();
        spi.cr1.volatileStore(cr1);
    }
    void disable() final {
        auto cr1 = spi.cr1.volatileLoad();
        cr1.SPE.clear();
        spi.cr1.volatileStore(cr1);
    }

    uint32_t dataSize([[maybe_unused]] uint32_t size) {
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
        if (size < 4 || size > 16) return dataSize();
        auto cr2 = spi.cr2.volatileLoad();
        cr2.setDataSize(size);
        spi.cr2.volatileStore(cr2);
        return size;
#else
        if (size != 8 && size != 16) return dataSize();
        auto cr1 = spi.cr1.volatileLoad();
        cr1.DFF = size == 8 ? 0 : 1;
        spi.cr1.volatileStore(cr1);
        return size;
#endif
    }

    uint32_t dataSize() const {
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
        return spi.cr2.volatileLoad().getDataSize();
#else
        return spi.cr1.volatileLoad().DFF == 0 ? 8 : 16;
#endif
    }
    /**@brief Set SPI CLK frequency.
     *
     * @param speed - CLK frequency in Hz
     * @return speed value that was set, this is closest value that was possible to set with current bus speed.
     */
    uint32_t speed(uint32_t speed) final {
        auto freq = ClockManager::SPIFrequency(getNumber());
        auto requiredPrescaler = freq / speed;
        prescaler(findClosestPrescaler(requiredPrescaler));

        return this->speed();
    }

    uint32_t speed() const final {
        const uint16_t prescalerValues[] = {2, 4, 8, 16, 32, 64, 128, 256};
        return ClockManager::SPIFrequency(getNumber()) / prescalerValues[static_cast<uint32_t>(prescaler())];
    }

 protected:
    using Interrupt = spiDetail::Interrupt;
    //---------------------------------------- variables ----------------------------------------//
    registers::SPI &spi;
    microhal::_MICROHAL_ACTIVE_PORT_NAMESPACE::GPIO misoPin;
//--------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SPI(registers::SPI &spi, _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin misoPin)
        : spi(spi), misoPin(misoPin) {
    }

    void busyWait() {
        while (!(spi.sr.volatileLoad().TXE)) {
        }
        while (spi.sr.volatileLoad().BSY) {
        }
    }

    Prescaler findClosestPrescaler(uint32_t prescaler);

    void enableGlobalInterrupt(uint32_t priority) {
        NVIC_EnableIRQ(irq());
        NVIC_SetPriority(irq(), priority);
    }

    uint8_t getNumber() const {
        if (&spi == registers::spi1) return 1;
        if (&spi == registers::spi2) return 2;
#ifdef _MICROHAL_SPI3_BASE_ADDRESS
        if (&spi == registers::spi3) return 3;
#endif
        std::terminate();
    }

    IRQn_Type irq() {
        if (&spi == registers::spi1) return SPI1_IRQn;
#if defined(SPI2)
        else if (&spi == registers::spi2)
            return SPI2_IRQn;
#endif
#if defined(SPI3)
        else if (&spi == registers::spi3)
            return SPI3_IRQn;
#endif
#if defined(SPI4)
        else if (&spi == registers::spi4)
            return SPI4_IRQn;
#endif
#if defined(SPI5)
        else if (&spi == registers::spi5)
            return SPI5_IRQn;
#endif
#if defined(SPI6)
        else if (&spi == registers::spi6)
            return SPI6_IRQn;
#endif
        std::terminate();
    }

    void clearErrorFlags(Error error) {
        if ((error & Error::Overrun) == Error::Overrun) {
            spi.dr.volatileLoad();
            // spi.sr.volatileLoad();
        }
        auto sr = spi.sr.volatileLoad();
        if ((error & Error::Crc) == Error::Crc) {
            sr.CRCERR.clear();
            spi.sr.volatileStore(sr);
        }
    }

    static SPI::Error errorCheck(registers::SPI::SR SRregisterValue) {
        SPI::Error error = Error::None;

        // Master Mode fault event
        if (SRregisterValue.MODF) {
            error = Error::MasterModeFault;
        }
        // Overrun error
        if (SRregisterValue.OVR) {
            error = Error::Overrun;
        }
        // CRC error flag
        if (SRregisterValue.CRCERR) {
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

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SPI_POLLING || ENABLED_ANY_SPI_INTERRUPT || ENABLED_ANY_SPI_DMA
#endif  //_MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1
#endif  // _MICROHAL_SPI_STMCOMMON_H_
