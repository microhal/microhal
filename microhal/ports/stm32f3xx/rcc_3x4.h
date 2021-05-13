#ifndef _MICROHAL_PERIPHERAL_REGISTER_RCC
#define _MICROHAL_PERIPHERAL_REGISTER_RCC

#include "ports/stmCommon/registers/registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

#undef RCC

#define _MICROHAL_REGISTERS_RCC_BDCR_RTCSEL_HAS_HSE_DIV_128
#define _MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC12EN
#define _MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC34EN
#define _MICROHAL_REGISTERS_RCC_CFGR_HAS_PPRE2

// Supported MCU: STM32F3x4
namespace microhal {
namespace registers {
/**
 *
 */
struct RCC {
    // Clock control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> HSION;   /*!< Internal High Speed clock enable */
            microhal::Bitfield<uint32_t, 1, 1> HSIRDY;  /*!< Internal High Speed clock ready flag */
            microhal::Bitfield<uint32_t, 3, 5> HSITRIM; /*!< Internal High Speed clock trimming */
            microhal::Bitfield<uint32_t, 8, 8> HSICAL;  /*!< Internal High Speed clock Calibration */
            microhal::Bitfield<uint32_t, 16, 1> HSEON;  /*!< External High Speed clock enable */
            microhal::Bitfield<uint32_t, 17, 1> HSERDY; /*!< External High Speed clock ready flag */
            microhal::Bitfield<uint32_t, 18, 1> HSEBYP; /*!< External High Speed clock Bypass */
            microhal::Bitfield<uint32_t, 19, 1> CSSON;  /*!< Clock Security System enable */
            microhal::Bitfield<uint32_t, 24, 1> PLLON;  /*!< PLL enable */
            microhal::Bitfield<uint32_t, 25, 1> PLLRDY; /*!< PLL clock ready flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR operator&(uint32_t value) const {
            CR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR operator|(uint32_t value) const {
            CR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR, AccessType::ReadOnly>;
        friend class VolatileRegister<CR, AccessType::WriteOnly>;
        friend class VolatileRegister<CR, AccessType::ReadWrite>;
    };

    // Clock configuration register (RCC_CFGR)
    union CFGR {
        union {
            microhal::Bitfield<uint32_t, 0, 2> SW;        /*!< System clock Switch */
            microhal::Bitfield<uint32_t, 2, 2> SWS;       /*!< System Clock Switch Status */
            microhal::Bitfield<uint32_t, 4, 4> HPRE;      /*!< AHB prescaler */
            microhal::Bitfield<uint32_t, 8, 3> PPRE1;     /*!< APB Low speed prescaler (APB1) */
            microhal::Bitfield<uint32_t, 11, 3> PPRE2;    /*!< APB high speed prescaler (APB2) */
            microhal::Bitfield<uint32_t, 16, 1> PLLSRC;   /*!< PLL entry clock source */
            microhal::Bitfield<uint32_t, 17, 1> PLLXTPRE; /*!< HSE divider for PLL entry */
            microhal::Bitfield<uint32_t, 18, 4> PLLMUL;   /*!< PLL Multiplication Factor */
            microhal::Bitfield<uint32_t, 22, 1> USBPRES;  /*!< USB prescaler */
            microhal::Bitfield<uint32_t, 23, 1> I2SSRC;   /*!< I2S external clock source selection */
            microhal::Bitfield<uint32_t, 24, 3> MCO;      /*!< Microcontroller clock output */
            microhal::Bitfield<uint32_t, 28, 1> MCOF;     /*!< Microcontroller Clock Output Flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFGR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFGR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFGR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFGR operator&(uint32_t value) const {
            CFGR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFGR operator|(uint32_t value) const {
            CFGR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFGR, AccessType::ReadOnly>;
        friend class VolatileRegister<CFGR, AccessType::WriteOnly>;
        friend class VolatileRegister<CFGR, AccessType::ReadWrite>;
    };

    // Clock interrupt register (RCC_CIR)
    union CIR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSIRDYF;   /*!< LSI Ready Interrupt flag */
            microhal::Bitfield<uint32_t, 1, 1> LSERDYF;   /*!< LSE Ready Interrupt flag */
            microhal::Bitfield<uint32_t, 2, 1> HSIRDYF;   /*!< HSI Ready Interrupt flag */
            microhal::Bitfield<uint32_t, 3, 1> HSERDYF;   /*!< HSE Ready Interrupt flag */
            microhal::Bitfield<uint32_t, 4, 1> PLLRDYF;   /*!< PLL Ready Interrupt flag */
            microhal::Bitfield<uint32_t, 7, 1> CSSF;      /*!< Clock Security System Interrupt flag */
            microhal::Bitfield<uint32_t, 8, 1> LSIRDYIE;  /*!< LSI Ready Interrupt Enable */
            microhal::Bitfield<uint32_t, 9, 1> LSERDYIE;  /*!< LSE Ready Interrupt Enable */
            microhal::Bitfield<uint32_t, 10, 1> HSIRDYIE; /*!< HSI Ready Interrupt Enable */
            microhal::Bitfield<uint32_t, 11, 1> HSERDYIE; /*!< HSE Ready Interrupt Enable */
            microhal::Bitfield<uint32_t, 12, 1> PLLRDYIE; /*!< PLL Ready Interrupt Enable */
            microhal::Bitfield<uint32_t, 16, 1> LSIRDYC;  /*!< LSI Ready Interrupt Clear */
            microhal::Bitfield<uint32_t, 17, 1> LSERDYC;  /*!< LSE Ready Interrupt Clear */
            microhal::Bitfield<uint32_t, 18, 1> HSIRDYC;  /*!< HSI Ready Interrupt Clear */
            microhal::Bitfield<uint32_t, 19, 1> HSERDYC;  /*!< HSE Ready Interrupt Clear */
            microhal::Bitfield<uint32_t, 20, 1> PLLRDYC;  /*!< PLL Ready Interrupt Clear */
            microhal::Bitfield<uint32_t, 23, 1> CSSC;     /*!< Clock security system interrupt clear */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CIR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CIR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CIR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CIR operator&(uint32_t value) const {
            CIR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CIR operator|(uint32_t value) const {
            CIR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CIR, AccessType::ReadOnly>;
        friend class VolatileRegister<CIR, AccessType::WriteOnly>;
        friend class VolatileRegister<CIR, AccessType::ReadWrite>;
    };

    // APB2 peripheral reset register (RCC_APB2RSTR)
    union APB2RSTR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SYSCFGRST;  /*!< SYSCFG and COMP reset */
            microhal::Bitfield<uint32_t, 11, 1> TIM1RST;   /*!< TIM1 timer reset */
            microhal::Bitfield<uint32_t, 12, 1> SPI1RST;   /*!< SPI 1 reset */
            microhal::Bitfield<uint32_t, 13, 1> TIM8RST;   /*!< TIM8 timer reset */
            microhal::Bitfield<uint32_t, 14, 1> USART1RST; /*!< USART1 reset */
            microhal::Bitfield<uint32_t, 16, 1> TIM15RST;  /*!< TIM15 timer reset */
            microhal::Bitfield<uint32_t, 17, 1> TIM16RST;  /*!< TIM16 timer reset */
            microhal::Bitfield<uint32_t, 18, 1> TIM17RST;  /*!< TIM17 timer reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APB2RSTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APB2RSTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APB2RSTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APB2RSTR operator&(uint32_t value) const {
            APB2RSTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APB2RSTR operator|(uint32_t value) const {
            APB2RSTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APB2RSTR, AccessType::ReadOnly>;
        friend class VolatileRegister<APB2RSTR, AccessType::WriteOnly>;
        friend class VolatileRegister<APB2RSTR, AccessType::ReadWrite>;
    };

    // APB1 peripheral reset register (RCC_APB1RSTR)
    union APB1RSTR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TIM2RST;    /*!< Timer 2 reset */
            microhal::Bitfield<uint32_t, 1, 1> TIM3RST;    /*!< Timer 3 reset */
            microhal::Bitfield<uint32_t, 2, 1> TIM4RST;    /*!< Timer 14 reset */
            microhal::Bitfield<uint32_t, 4, 1> TIM6RST;    /*!< Timer 6 reset */
            microhal::Bitfield<uint32_t, 5, 1> TIM7RST;    /*!< Timer 7 reset */
            microhal::Bitfield<uint32_t, 11, 1> WWDGRST;   /*!< Window watchdog reset */
            microhal::Bitfield<uint32_t, 14, 1> SPI2RST;   /*!< SPI2 reset */
            microhal::Bitfield<uint32_t, 15, 1> SPI3RST;   /*!< SPI3 reset */
            microhal::Bitfield<uint32_t, 17, 1> USART2RST; /*!< USART 2 reset */
            microhal::Bitfield<uint32_t, 18, 1> USART3RST; /*!< USART3 reset */
            microhal::Bitfield<uint32_t, 19, 1> UART4RST;  /*!< UART 4 reset */
            microhal::Bitfield<uint32_t, 20, 1> UART5RST;  /*!< UART 5 reset */
            microhal::Bitfield<uint32_t, 21, 1> I2C1RST;   /*!< I2C1 reset */
            microhal::Bitfield<uint32_t, 22, 1> I2C2RST;   /*!< I2C2 reset */
            microhal::Bitfield<uint32_t, 23, 1> USBRST;    /*!< USB reset */
            microhal::Bitfield<uint32_t, 25, 1> CANRST;    /*!< CAN reset */
            microhal::Bitfield<uint32_t, 26, 1> DAC2RST;   /*!< DAC 2 interface reset */
            microhal::Bitfield<uint32_t, 28, 1> PWRRST;    /*!< Power interface reset */
            microhal::Bitfield<uint32_t, 29, 1> DAC1RST;   /*!< DAC 1 interface reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APB1RSTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APB1RSTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APB1RSTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APB1RSTR operator&(uint32_t value) const {
            APB1RSTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APB1RSTR operator|(uint32_t value) const {
            APB1RSTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APB1RSTR, AccessType::ReadOnly>;
        friend class VolatileRegister<APB1RSTR, AccessType::WriteOnly>;
        friend class VolatileRegister<APB1RSTR, AccessType::ReadWrite>;
    };

    // AHB Peripheral Clock enable register (RCC_AHBENR)
    union AHBENR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> DMA1EN;   /*!< DMA1 clock enable */
            microhal::Bitfield<uint32_t, 1, 1> DMA2EN;   /*!< DMA2 clock enable */
            microhal::Bitfield<uint32_t, 2, 1> SRAMEN;   /*!< SRAM interface clock enable */
            microhal::Bitfield<uint32_t, 4, 1> FLITFEN;  /*!< FLITF clock enable */
            microhal::Bitfield<uint32_t, 6, 1> CRCEN;    /*!< CRC clock enable */
            microhal::Bitfield<uint32_t, 17, 1> IOPAEN;  /*!< I/O port A clock enable */
            microhal::Bitfield<uint32_t, 18, 1> IOPBEN;  /*!< I/O port B clock enable */
            microhal::Bitfield<uint32_t, 19, 1> IOPCEN;  /*!< I/O port C clock enable */
            microhal::Bitfield<uint32_t, 20, 1> IOPDEN;  /*!< I/O port D clock enable */
            microhal::Bitfield<uint32_t, 21, 1> IOPEEN;  /*!< I/O port E clock enable */
            microhal::Bitfield<uint32_t, 22, 1> IOPFEN;  /*!< I/O port F clock enable */
            microhal::Bitfield<uint32_t, 24, 1> TSCEN;   /*!< Touch sensing controller clock enable */
            microhal::Bitfield<uint32_t, 28, 1> ADC12EN; /*!< ADC1 and ADC2 clock enable */
            microhal::Bitfield<uint32_t, 29, 1> ADC34EN; /*!< ADC3 and ADC4 clock enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        AHBENR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        AHBENR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        AHBENR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        AHBENR operator&(uint32_t value) const {
            AHBENR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        AHBENR operator|(uint32_t value) const {
            AHBENR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<AHBENR, AccessType::ReadOnly>;
        friend class VolatileRegister<AHBENR, AccessType::WriteOnly>;
        friend class VolatileRegister<AHBENR, AccessType::ReadWrite>;
    };

    // APB2 peripheral clock enable register (RCC_APB2ENR)
    union APB2ENR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SYSCFGEN;  /*!< SYSCFG clock enable */
            microhal::Bitfield<uint32_t, 11, 1> TIM1EN;   /*!< TIM1 Timer clock enable */
            microhal::Bitfield<uint32_t, 12, 1> SPI1EN;   /*!< SPI 1 clock enable */
            microhal::Bitfield<uint32_t, 13, 1> TIM8EN;   /*!< TIM8 Timer clock enable */
            microhal::Bitfield<uint32_t, 14, 1> USART1EN; /*!< USART1 clock enable */
            microhal::Bitfield<uint32_t, 16, 1> TIM15EN;  /*!< TIM15 timer clock enable */
            microhal::Bitfield<uint32_t, 17, 1> TIM16EN;  /*!< TIM16 timer clock enable */
            microhal::Bitfield<uint32_t, 18, 1> TIM17EN;  /*!< TIM17 timer clock enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APB2ENR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APB2ENR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APB2ENR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APB2ENR operator&(uint32_t value) const {
            APB2ENR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APB2ENR operator|(uint32_t value) const {
            APB2ENR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APB2ENR, AccessType::ReadOnly>;
        friend class VolatileRegister<APB2ENR, AccessType::WriteOnly>;
        friend class VolatileRegister<APB2ENR, AccessType::ReadWrite>;
    };

    // APB1 peripheral clock enable register (RCC_APB1ENR)
    union APB1ENR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TIM2EN;    /*!< Timer 2 clock enable */
            microhal::Bitfield<uint32_t, 1, 1> TIM3EN;    /*!< Timer 3 clock enable */
            microhal::Bitfield<uint32_t, 2, 1> TIM4EN;    /*!< Timer 4 clock enable */
            microhal::Bitfield<uint32_t, 4, 1> TIM6EN;    /*!< Timer 6 clock enable */
            microhal::Bitfield<uint32_t, 5, 1> TIM7EN;    /*!< Timer 7 clock enable */
            microhal::Bitfield<uint32_t, 11, 1> WWDGEN;   /*!< Window watchdog clock enable */
            microhal::Bitfield<uint32_t, 14, 1> SPI2EN;   /*!< SPI 2 clock enable */
            microhal::Bitfield<uint32_t, 15, 1> SPI3EN;   /*!< SPI 3 clock enable */
            microhal::Bitfield<uint32_t, 17, 1> USART2EN; /*!< USART 2 clock enable */
            microhal::Bitfield<uint32_t, 18, 1> USART3EN; /*!< USART 3 clock enable */
            microhal::Bitfield<uint32_t, 21, 1> I2C1EN;   /*!< I2C 1 clock enable */
            microhal::Bitfield<uint32_t, 22, 1> I2C2EN;   /*!< I2C 2 clock enable */
            microhal::Bitfield<uint32_t, 23, 1> USBEN;    /*!< USB clock enable */
            microhal::Bitfield<uint32_t, 25, 1> CANEN;    /*!< CAN clock enable */
            microhal::Bitfield<uint32_t, 26, 1> DAC2EN;   /*!< DAC 2 interface clock enable */
            microhal::Bitfield<uint32_t, 28, 1> PWREN;    /*!< Power interface clock enable */
            microhal::Bitfield<uint32_t, 29, 1> DAC1EN;   /*!< DAC 1 interface clock enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APB1ENR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APB1ENR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APB1ENR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APB1ENR operator&(uint32_t value) const {
            APB1ENR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APB1ENR operator|(uint32_t value) const {
            APB1ENR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APB1ENR, AccessType::ReadOnly>;
        friend class VolatileRegister<APB1ENR, AccessType::WriteOnly>;
        friend class VolatileRegister<APB1ENR, AccessType::ReadWrite>;
    };

    // Backup domain control register (RCC_BDCR)
    union BDCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSEON;  /*!< External Low Speed oscillator enable */
            microhal::Bitfield<uint32_t, 1, 1> LSERDY; /*!< External Low Speed oscillator ready */
            microhal::Bitfield<uint32_t, 2, 1> LSEBYP; /*!< External Low Speed oscillator bypass */
            microhal::Bitfield<uint32_t, 3, 2> LSEDRV; /*!< LSE oscillator drive capability */
            microhal::Bitfield<uint32_t, 8, 2> RTCSEL; /*!< RTC clock source selection */
            microhal::Bitfield<uint32_t, 15, 1> RTCEN; /*!< RTC clock enable */
            microhal::Bitfield<uint32_t, 16, 1> BDRST; /*!< Backup domain software reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        BDCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        BDCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        BDCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        BDCR operator&(uint32_t value) const {
            BDCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        BDCR operator|(uint32_t value) const {
            BDCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<BDCR, AccessType::ReadOnly>;
        friend class VolatileRegister<BDCR, AccessType::WriteOnly>;
        friend class VolatileRegister<BDCR, AccessType::ReadWrite>;
    };

    // Control/status register (RCC_CSR)
    union CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSION;     /*!< Internal low speed oscillator enable */
            microhal::Bitfield<uint32_t, 1, 1> LSIRDY;    /*!< Internal low speed oscillator ready */
            microhal::Bitfield<uint32_t, 24, 1> RMVF;     /*!< Remove reset flag */
            microhal::Bitfield<uint32_t, 25, 1> OBLRSTF;  /*!< Option byte loader reset flag */
            microhal::Bitfield<uint32_t, 26, 1> PINRSTF;  /*!< PIN reset flag */
            microhal::Bitfield<uint32_t, 27, 1> PORRSTF;  /*!< POR/PDR reset flag */
            microhal::Bitfield<uint32_t, 28, 1> SFTRSTF;  /*!< Software reset flag */
            microhal::Bitfield<uint32_t, 29, 1> IWDGRSTF; /*!< Independent watchdog reset flag */
            microhal::Bitfield<uint32_t, 30, 1> WWDGRSTF; /*!< Window watchdog reset flag */
            microhal::Bitfield<uint32_t, 31, 1> LPWRRSTF; /*!< Low-power reset flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CSR operator&(uint32_t value) const {
            CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CSR operator|(uint32_t value) const {
            CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<CSR, AccessType::ReadWrite>;
    };

    // AHB peripheral reset register
    union AHBRSTR {
        union {
            microhal::Bitfield<uint32_t, 17, 1> IOPARST;  /*!< I/O port A reset */
            microhal::Bitfield<uint32_t, 18, 1> IOPBRST;  /*!< I/O port B reset */
            microhal::Bitfield<uint32_t, 19, 1> IOPCRST;  /*!< I/O port C reset */
            microhal::Bitfield<uint32_t, 20, 1> IOPDRST;  /*!< I/O port D reset */
            microhal::Bitfield<uint32_t, 21, 1> IOPERST;  /*!< I/O port E reset */
            microhal::Bitfield<uint32_t, 22, 1> IOPFRST;  /*!< I/O port F reset */
            microhal::Bitfield<uint32_t, 24, 1> TSCRST;   /*!< Touch sensing controller reset */
            microhal::Bitfield<uint32_t, 28, 1> ADC12RST; /*!< ADC1 and ADC2 reset */
            microhal::Bitfield<uint32_t, 29, 1> ADC34RST; /*!< ADC3 and ADC4 reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        AHBRSTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        AHBRSTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        AHBRSTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        AHBRSTR operator&(uint32_t value) const {
            AHBRSTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        AHBRSTR operator|(uint32_t value) const {
            AHBRSTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<AHBRSTR, AccessType::ReadOnly>;
        friend class VolatileRegister<AHBRSTR, AccessType::WriteOnly>;
        friend class VolatileRegister<AHBRSTR, AccessType::ReadWrite>;
    };

    // Clock configuration register 2
    union CFGR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 4> PREDIV;    /*!< PREDIV division factor */
            microhal::Bitfield<uint32_t, 4, 5> ADC12PRES; /*!< ADC1 and ADC2 prescaler */
            microhal::Bitfield<uint32_t, 9, 5> ADC34PRES; /*!< ADC3 and ADC4 prescaler */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFGR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFGR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFGR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFGR2 operator&(uint32_t value) const {
            CFGR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFGR2 operator|(uint32_t value) const {
            CFGR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFGR2, AccessType::ReadOnly>;
        friend class VolatileRegister<CFGR2, AccessType::WriteOnly>;
        friend class VolatileRegister<CFGR2, AccessType::ReadWrite>;
    };

    // Clock configuration register 3
    union CFGR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 2> USART1SW; /*!< USART1 clock source selection */
            microhal::Bitfield<uint32_t, 4, 1> I2C1SW;   /*!< I2C1 clock source selection */
#ifndef STM32F334x8
            microhal::Bitfield<uint32_t, 5, 1> I2C2SW; /*!< I2C2 clock source selection */
#endif
            microhal::Bitfield<uint32_t, 8, 1> TIM1SW; /*!< Timer1 clock source selection */
#ifndef STM32F334x8
            microhal::Bitfield<uint32_t, 9, 1> TIM8SW; /*!< Timer8 clock source selection */
#endif
            microhal::Bitfield<uint32_t, 12, 1> HRTIM1SW; /*!< High Resolution Timer1 clock source selection */
#ifndef STM32F334x8
            microhal::Bitfield<uint32_t, 16, 2> USART2SW; /*!< USART2 clock source selection */
            microhal::Bitfield<uint32_t, 18, 2> USART3SW; /*!< USART3 clock source selection */
            microhal::Bitfield<uint32_t, 20, 2> UART4SW;  /*!< UART4 clock source selection */
            microhal::Bitfield<uint32_t, 22, 2> UART5SW;  /*!< UART5 clock source selection */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFGR3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFGR3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFGR3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFGR3 operator&(uint32_t value) const {
            CFGR3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFGR3 operator|(uint32_t value) const {
            CFGR3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFGR3, AccessType::ReadOnly>;
        friend class VolatileRegister<CFGR3, AccessType::WriteOnly>;
        friend class VolatileRegister<CFGR3, AccessType::ReadWrite>;
    };

    VolatileRegister<CR, AccessType::ReadWrite> cr;             /*!< Clock control register	Address offset: 0x0 */
    VolatileRegister<CFGR, AccessType::ReadWrite> cfgr;         /*!< Clock configuration register (RCC_CFGR)	Address offset: 0x4 */
    VolatileRegister<CIR, AccessType::ReadWrite> cir;           /*!< Clock interrupt register (RCC_CIR)	Address offset: 0x8 */
    VolatileRegister<APB2RSTR, AccessType::ReadWrite> apb2rstr; /*!< APB2 peripheral reset register (RCC_APB2RSTR)	Address offset: 0xC */
    VolatileRegister<APB1RSTR, AccessType::ReadWrite> apb1rstr; /*!< APB1 peripheral reset register (RCC_APB1RSTR)	Address offset: 0x10 */
    VolatileRegister<AHBENR, AccessType::ReadWrite> ahb1enr;    /*!< AHB Peripheral Clock enable register (RCC_AHBENR)	Address offset: 0x14 */
    VolatileRegister<APB2ENR, AccessType::ReadWrite> apb2enr;   /*!< APB2 peripheral clock enable register (RCC_APB2ENR)	Address offset: 0x18 */
    VolatileRegister<APB1ENR, AccessType::ReadWrite> apb1enr;   /*!< APB1 peripheral clock enable register (RCC_APB1ENR)	Address offset: 0x1C */
    VolatileRegister<BDCR, AccessType::ReadWrite> bdcr;         /*!< Backup domain control register (RCC_BDCR)	Address offset: 0x20 */
    VolatileRegister<CSR, AccessType::ReadWrite> csr;           /*!< Control/status register (RCC_CSR)	Address offset: 0x24 */
    VolatileRegister<AHBRSTR, AccessType::ReadWrite> ahbrstr;   /*!< AHB peripheral reset register	Address offset: 0x28 */
    VolatileRegister<CFGR2, AccessType::ReadWrite> cfgr2;       /*!< Clock configuration register 2	Address offset: 0x2C */
    VolatileRegister<CFGR3, AccessType::ReadWrite> cfgr3;       /*!< Clock configuration register 3	Address offset: 0x30 */
};

[[maybe_unused]] static RCC *const rcc = reinterpret_cast<RCC *>(_MICROHAL_RCC_BASE_ADDRESS);

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_RCC
