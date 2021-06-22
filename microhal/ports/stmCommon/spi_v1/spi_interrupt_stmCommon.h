/*
 * spi_interrupt_STM32F4xx.h
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_SPI_INTERRUPT_STMCOMMON_H_
#define _MICROHAL_SPI_INTERRUPT_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include "spi_stmCommon.h"
#if _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SPI(INTERRUPT)                  // Check if driver is enabled in microhal port config
/* ************************************************************************************************
 * INCLUDES
 */
#include "microhal_semaphore.h"
#include _MICROHAL_INCLUDE_PORT_IOMANAGER

#if !defined(MICROHAL_USE_SPI1_INTERRUPT)
#define MICROHAL_USE_SPI1_INTERRUPT 0
#endif
#if !defined(MICROHAL_USE_SPI2_INTERRUPT)
#define MICROHAL_USE_SPI2_INTERRUPT 0
#endif
#if !defined(MICROHAL_USE_SPI3_INTERRUPT)
#define MICROHAL_USE_SPI3_INTERRUPT 0
#endif
#if !defined(MICROHAL_USE_SPI4_INTERRUPT)
#define MICROHAL_USE_SPI4_INTERRUPT 0
#endif
#if !defined(MICROHAL_USE_SPI5_INTERRUPT)
#define MICROHAL_USE_SPI5_INTERRUPT 0
#endif
#if !defined(MICROHAL_USE_SPI6_INTERRUPT)
#define MICROHAL_USE_SPI6_INTERRUPT 0
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 * CLASS
 */
class SPI_interrupt : public _MICROHAL_ACTIVE_PORT_NAMESPACE::SPI {
 public:
    template <int number, IOPin miso, IOPin mosi, IOPin sck>
    static SPI_interrupt &create(GPIO::OutputType mosiType = GPIO::OutputType::PushPull, GPIO::OutputType sckType = GPIO::OutputType::PushPull) {
        IOManager::routeSPI<number, MISO, miso>();
        IOManager::routeSPI<number, MOSI, mosi>(GPIO::PullType::NoPull, mosiType);
        IOManager::routeSPI<number, SCK, sck>(GPIO::PullType::NoPull, sckType);
        static_assert(IOManager::spiPinAssert(number, miso, mosi, sck), "Incorrect Pin configuration");

        if constexpr (number == 1) {
            static_assert(MICROHAL_USE_SPI1_INTERRUPT == 1 && number == 1,
                          "You have to define 'MICROHAL_USE_SPI1_INTERRUPT 1' in microhalPortConfig_xxx.h file.");
#if MICROHAL_USE_SPI1_INTERRUPT == 1
            static SPI_interrupt _spi1(*microhal::registers::spi1, miso);
            spi1 = &_spi1;
            return *spi1;
#endif
        }
        if constexpr (number == 2) {
            static_assert(MICROHAL_USE_SPI2_INTERRUPT == 1 && number == 2,
                          "You have to define 'MICROHAL_USE_SPI2_INTERRUPT 1' in microhalPortConfig_xxx.h file.");
#if MICROHAL_USE_SPI2_INTERRUPT == 1
            static SPI_interrupt _spi2(*microhal::registers::spi2, miso);
            spi2 = &_spi2;
            return *spi2;
#endif
        }
        if constexpr (number == 3) {
            static_assert(MICROHAL_USE_SPI3_INTERRUPT == 1 && number == 3,
                          "You have to define 'MICROHAL_USE_SPI3_INTERRUPT 1' in microhalPortConfig_xxx.h file.");
#if MICROHAL_USE_SPI3_INTERRUPT == 1
            static SPI_interrupt _spi3(*microhal::registers::spi3, miso);
            spi3 = &_spi3;
            return *spi3;
#endif
        }
        if constexpr (number == 4) {
            static_assert(MICROHAL_USE_SPI4_INTERRUPT == 1 && number == 4,
                          "You have to define 'MICROHAL_USE_SPI4_INTERRUPT 1' in microhalPortConfig_xxx.h file.");
#if MICROHAL_USE_SPI4_INTERRUPT == 1
            static SPI_interrupt _spi4(*microhal::registers::spi4, miso);
            spi4 = &_spi4;
            return *spi4;
#endif
        }
        if constexpr (number == 5) {
            static_assert(MICROHAL_USE_SPI5_INTERRUPT == 1 && number == 5,
                          "You have to define 'MICROHAL_USE_SPI5_INTERRUPT 1' in microhalPortConfig_xxx.h file.");
#if MICROHAL_USE_SPI5_INTERRUPT == 1
            static SPI_interrupt _spi5(*microhal::registers::spi5, miso);
            spi5 = &_spi5;
            return *spi5;
#endif
        }
        if constexpr (number == 6) {
            static_assert(MICROHAL_USE_SPI6_INTERRUPT == 1 && number == 6,
                          "You have to define 'MICROHAL_USE_SPI3_INTERRUPT 1' in microhalPortConfig_xxx.h file.");
#if MICROHAL_USE_SPI6_INTERRUPT == 1
            static SPI_interrupt _spi6(*microhal::registers::spi6, miso);
            spi6 = &_spi6;
            return *spi6;
#endif
        }
    }

    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(const void *data, size_t len, bool last) final;
    SPI::Error read(void *data, size_t len, uint8_t write = 0x00) final;
    SPI::Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final;

 private:
#if defined(MICROHAL_USE_SPI1_INTERRUPT) && MICROHAL_USE_SPI1_INTERRUPT == 1
    static SPI_interrupt *spi1;
#endif
#if defined(MICROHAL_USE_SPI2_INTERRUPT) && MICROHAL_USE_SPI2_INTERRUPT == 1
    static SPI_interrupt *spi2;
#endif
#if defined(MICROHAL_USE_SPI3_INTERRUPT) && MICROHAL_USE_SPI3_INTERRUPT == 1
    static SPI_interrupt *spi3;
#endif
#if defined(MICROHAL_USE_SPI4_INTERRUPT) && MICROHAL_USE_SPI4_INTERRUPT == 1
    static SPI_interrupt *spi4;
#endif
#if defined(MICROHAL_USE_SPI5_INTERRUPT) && MICROHAL_USE_SPI5_INTERRUPT == 1
    static SPI_interrupt *spi5;
#endif
#if defined(MICROHAL_USE_SPI6_INTERRUPT) && MICROHAL_USE_SPI6_INTERRUPT == 1
    static SPI_interrupt *spi6;
#endif
    //---------------------------------------- variables ----------------------------------------//
    uint8_t *readPtr = nullptr;
    uint8_t *readEnd = nullptr;
    const uint8_t *writePtr = nullptr;
    const uint8_t *writeEnd = nullptr;
    os::Semaphore semaphore;

    //--------------------------------------- constructors --------------------------------------//
    SPI_interrupt(registers::SPI &spi, _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin misoPin) : SPI(spi, misoPin), semaphore() {
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
        ClockManager::enableSPI(getNumber() + 1, ClockManager::PowerMode::Normal);
#else
        ClockManager::enableSPI(getNumber() + 1);
#endif
#if defined(HAL_RTOS_FreeRTOS)
        enableGlobalInterrupt(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#else
        enableGlobalInterrupt(0);
#endif
    }

    SPI_interrupt &operator=(const SPI_interrupt &);

    SPI_interrupt(const SPI_interrupt &);
    //---------------------------------------- functions ----------------------------------------//
    void clearRxFifo();
    void IRQfunction();
    //----------------------------------------- friends -----------------------------------------//
    friend void SPI1_IRQHandler(void);
    friend void SPI2_IRQHandler(void);
    friend void SPI3_IRQHandler(void);
    friend void SPI4_IRQHandler(void);
    friend void SPI5_IRQHandler(void);
    friend void SPI6_IRQHandler(void);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SPI(INTERRUPT)
#endif  // _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1
#endif  /* _MICROHAL_SPI_INTERRUPT_STMCOMMON_H_ */
