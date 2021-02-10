#ifndef _MICROHAL_PERIPHERAL_REGISTER_RCC
#define _MICROHAL_PERIPHERAL_REGISTER_RCC

#include <cstdint>
#include "ports/stmCommon/registers/registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

#define _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
#define _MICROHAL_REGISTERS_RCC_HAS_APBENR
#define _MICROHAL_REGISTERS_RCC_HAS_APBENR1
#define _MICROHAL_REGISTERS_RCC_HAS_APBENR2
#define _MICROHAL_REGISTERS_RCC_HAS_IOPENR
#define _MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC1EN
#define _MICROHAL_REGISTERS_RCC_BDCR_RTCSEL_HAS_HSE_DIV_32

// Supported MCU: STM32G071, STM32G070, STM32G081, STM32G071, STM32G070, STM32G081
namespace microhal {
namespace registers {
/**
 *
 */
struct RCC {
    // Clock control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 8, 1> HSION;    /*!< HSI16 clock enable */
            microhal::Bitfield<uint32_t, 9, 1> HSIKERON; /*!< HSI16 always enable for peripheral kernels */
            microhal::Bitfield<uint32_t, 10, 1> HSIRDY;  /*!< HSI16 clock ready flag */
            microhal::Bitfield<uint32_t, 11, 3> HSIDIV;  /*!< HSI16 clock division factor */
            microhal::Bitfield<uint32_t, 16, 1> HSEON;   /*!< HSE clock enable */
            microhal::Bitfield<uint32_t, 17, 1> HSERDY;  /*!< HSE clock ready flag */
            microhal::Bitfield<uint32_t, 18, 1> HSEBYP;  /*!< HSE crystal oscillator bypass */
            microhal::Bitfield<uint32_t, 19, 1> CSSON;   /*!< Clock security system enable */
            microhal::Bitfield<uint32_t, 24, 1> PLLON;   /*!< PLL enable */
            microhal::Bitfield<uint32_t, 25, 1> PLLRDY;  /*!< PLL clock ready flag */
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

    // Internal clock sources calibration register
    union ICSCR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> HSICAL;  /*!< HSI16 clock calibration */
            microhal::Bitfield<uint32_t, 8, 7> HSITRIM; /*!< HSI16 clock trimming */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ICSCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ICSCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ICSCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ICSCR operator&(uint32_t value) const {
            ICSCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ICSCR operator|(uint32_t value) const {
            ICSCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ICSCR, AccessType::ReadOnly>;
        friend class VolatileRegister<ICSCR, AccessType::WriteOnly>;
        friend class VolatileRegister<ICSCR, AccessType::ReadWrite>;
    };

    // Clock configuration register
    union CFGR {
        union {
            microhal::Bitfield<uint32_t, 0, 3> SW;      /*!< System clock switch */
            microhal::Bitfield<uint32_t, 3, 3> SWS;     /*!< System clock switch status */
            microhal::Bitfield<uint32_t, 8, 4> HPRE;    /*!< AHB prescaler */
            microhal::Bitfield<uint32_t, 12, 3> PPRE1;  /*!< APB prescaler */
            microhal::Bitfield<uint32_t, 24, 3> MCOSEL; /*!< Microcontroller clock output */
            microhal::Bitfield<uint32_t, 28, 3> MCOPRE; /*!< Microcontroller clock output prescaler */
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

    // PLL configuration register
    union PLLCFGR {
        union {
            microhal::Bitfield<uint32_t, 0, 2> PLLSRC;  /*!< PLL input clock source */
            microhal::Bitfield<uint32_t, 4, 3> PLLM;    /*!< Division factor M of the PLL input clock divider */
            microhal::Bitfield<uint32_t, 8, 7> PLLN;    /*!< PLL frequency multiplication factor N */
            microhal::Bitfield<uint32_t, 16, 1> PLLPEN; /*!< PLLPCLK clock output enable */
            microhal::Bitfield<uint32_t, 17, 5> PLLP;   /*!< PLL VCO division factor P for PLLPCLK clock output */
            microhal::Bitfield<uint32_t, 24, 1> PLLQEN; /*!< PLLQCLK clock output enable */
            microhal::Bitfield<uint32_t, 25, 3> PLLQ;   /*!< PLL VCO division factor Q for PLLQCLK clock output */
            microhal::Bitfield<uint32_t, 28, 1> PLLREN; /*!< PLLRCLK clock output enable */
            microhal::Bitfield<uint32_t, 29, 3> PLLR;   /*!< PLL VCO division factor R for PLLRCLK clock output */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PLLCFGR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PLLCFGR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PLLCFGR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PLLCFGR operator&(uint32_t value) const {
            PLLCFGR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PLLCFGR operator|(uint32_t value) const {
            PLLCFGR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PLLCFGR, AccessType::ReadOnly>;
        friend class VolatileRegister<PLLCFGR, AccessType::WriteOnly>;
        friend class VolatileRegister<PLLCFGR, AccessType::ReadWrite>;
    };

    // Clock interrupt enable register
    union CIER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSIRDYIE;    /*!< LSI ready interrupt enable */
            microhal::Bitfield<uint32_t, 1, 1> LSERDYIE;    /*!< LSE ready interrupt enable */
            microhal::Bitfield<uint32_t, 3, 1> HSIRDYIE;    /*!< HSI ready interrupt enable */
            microhal::Bitfield<uint32_t, 4, 1> HSERDYIE;    /*!< HSE ready interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> PLLSYSRDYIE; /*!< PLL ready interrupt enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CIER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CIER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CIER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CIER operator&(uint32_t value) const {
            CIER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CIER operator|(uint32_t value) const {
            CIER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CIER, AccessType::ReadOnly>;
        friend class VolatileRegister<CIER, AccessType::WriteOnly>;
        friend class VolatileRegister<CIER, AccessType::ReadWrite>;
    };

    // Clock interrupt flag register
    union CIFR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSIRDYF;    /*!< LSI ready interrupt flag */
            microhal::Bitfield<uint32_t, 1, 1> LSERDYF;    /*!< LSE ready interrupt flag */
            microhal::Bitfield<uint32_t, 3, 1> HSIRDYF;    /*!< HSI ready interrupt flag */
            microhal::Bitfield<uint32_t, 4, 1> HSERDYF;    /*!< HSE ready interrupt flag */
            microhal::Bitfield<uint32_t, 5, 1> PLLSYSRDYF; /*!< PLL ready interrupt flag */
            microhal::Bitfield<uint32_t, 8, 1> CSSF;       /*!< Clock security system interrupt flag */
            microhal::Bitfield<uint32_t, 9, 1> LSECSSF;    /*!< LSE Clock security system interrupt flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CIFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CIFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CIFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CIFR operator&(uint32_t value) const {
            CIFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CIFR operator|(uint32_t value) const {
            CIFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CIFR, AccessType::ReadOnly>;
        friend class VolatileRegister<CIFR, AccessType::WriteOnly>;
        friend class VolatileRegister<CIFR, AccessType::ReadWrite>;
    };

    // Clock interrupt clear register
    union CICR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSIRDYC;    /*!< LSI ready interrupt clear */
            microhal::Bitfield<uint32_t, 1, 1> LSERDYC;    /*!< LSE ready interrupt clear */
            microhal::Bitfield<uint32_t, 3, 1> HSIRDYC;    /*!< HSI ready interrupt clear */
            microhal::Bitfield<uint32_t, 4, 1> HSERDYC;    /*!< HSE ready interrupt clear */
            microhal::Bitfield<uint32_t, 5, 1> PLLSYSRDYC; /*!< PLL ready interrupt clear */
            microhal::Bitfield<uint32_t, 8, 1> CSSC;       /*!< Clock security system interrupt clear */
            microhal::Bitfield<uint32_t, 9, 1> LSECSSC;    /*!< LSE Clock security system interrupt clear */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CICR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CICR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CICR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CICR operator&(uint32_t value) const {
            CICR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CICR operator|(uint32_t value) const {
            CICR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CICR, AccessType::ReadOnly>;
        friend class VolatileRegister<CICR, AccessType::WriteOnly>;
        friend class VolatileRegister<CICR, AccessType::ReadWrite>;
    };

    // AHB peripheral reset register
    union AHBRSTR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> DMARST;   /*!< DMA1 reset */
            microhal::Bitfield<uint32_t, 8, 1> FLASHRST; /*!< FLITF reset */
            microhal::Bitfield<uint32_t, 12, 1> CRCRST;  /*!< CRC reset */
            microhal::Bitfield<uint32_t, 16, 1> AESRST;  /*!< AES hardware accelerator reset */
            microhal::Bitfield<uint32_t, 18, 1> RNGRST;  /*!< Random number generator reset */
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

    // GPIO reset register
    union IOPRSTR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> IOPARST; /*!< I/O port A reset */
            microhal::Bitfield<uint32_t, 1, 1> IOPBRST; /*!< I/O port B reset */
            microhal::Bitfield<uint32_t, 2, 1> IOPCRST; /*!< I/O port C reset */
            microhal::Bitfield<uint32_t, 3, 1> IOPDRST; /*!< I/O port D reset */
            microhal::Bitfield<uint32_t, 5, 1> IOPFRST; /*!< I/O port F reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IOPRSTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IOPRSTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IOPRSTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IOPRSTR operator&(uint32_t value) const {
            IOPRSTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IOPRSTR operator|(uint32_t value) const {
            IOPRSTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IOPRSTR, AccessType::ReadOnly>;
        friend class VolatileRegister<IOPRSTR, AccessType::WriteOnly>;
        friend class VolatileRegister<IOPRSTR, AccessType::ReadWrite>;
    };

    // APB peripheral reset register 1
    union APBRSTR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TIM2RST;     /*!< TIM2 timer reset */
            microhal::Bitfield<uint32_t, 1, 1> TIM3RST;     /*!< TIM3 timer reset */
            microhal::Bitfield<uint32_t, 4, 1> TIM6RST;     /*!< TIM6 timer reset */
            microhal::Bitfield<uint32_t, 5, 1> TIM7RST;     /*!< TIM7 timer reset */
            microhal::Bitfield<uint32_t, 14, 1> SPI2RST;    /*!< SPI2 reset */
            microhal::Bitfield<uint32_t, 17, 1> USART2RST;  /*!< USART2 reset */
            microhal::Bitfield<uint32_t, 18, 1> USART3RST;  /*!< USART3 reset */
            microhal::Bitfield<uint32_t, 19, 1> USART4RST;  /*!< USART4 reset */
            microhal::Bitfield<uint32_t, 20, 1> LPUART1RST; /*!< LPUART1 reset */
            microhal::Bitfield<uint32_t, 21, 1> I2C1RST;    /*!< I2C1 reset */
            microhal::Bitfield<uint32_t, 22, 1> I2C2RST;    /*!< I2C2 reset */
            microhal::Bitfield<uint32_t, 24, 1> CECRST;     /*!< HDMI CEC reset */
            microhal::Bitfield<uint32_t, 25, 1> UCPD1RST;   /*!< UCPD1 reset */
            microhal::Bitfield<uint32_t, 26, 1> UCPD2RST;   /*!< UCPD2 reset */
            microhal::Bitfield<uint32_t, 27, 1> DBGRST;     /*!< Debug support reset */
            microhal::Bitfield<uint32_t, 28, 1> PWRRST;     /*!< Power interface reset */
            microhal::Bitfield<uint32_t, 29, 1> DAC1RST;    /*!< DAC1 interface reset */
            microhal::Bitfield<uint32_t, 30, 1> LPTIM2RST;  /*!< Low Power Timer 2 reset */
            microhal::Bitfield<uint32_t, 31, 1> LPTIM1RST;  /*!< Low Power Timer 1 reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APBRSTR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APBRSTR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APBRSTR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APBRSTR1 operator&(uint32_t value) const {
            APBRSTR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APBRSTR1 operator|(uint32_t value) const {
            APBRSTR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APBRSTR1, AccessType::ReadOnly>;
        friend class VolatileRegister<APBRSTR1, AccessType::WriteOnly>;
        friend class VolatileRegister<APBRSTR1, AccessType::ReadWrite>;
    };

    // APB peripheral reset register 2
    union APBRSTR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SYSCFGRST;  /*!< SYSCFG, COMP and VREFBUF reset */
            microhal::Bitfield<uint32_t, 11, 1> TIM1RST;   /*!< TIM1 timer reset */
            microhal::Bitfield<uint32_t, 12, 1> SPI1RST;   /*!< SPI1 reset */
            microhal::Bitfield<uint32_t, 14, 1> USART1RST; /*!< USART1 reset */
            microhal::Bitfield<uint32_t, 15, 1> TIM14RST;  /*!< TIM14 timer reset */
            microhal::Bitfield<uint32_t, 16, 1> TIM15RST;  /*!< TIM15 timer reset */
            microhal::Bitfield<uint32_t, 17, 1> TIM16RST;  /*!< TIM16 timer reset */
            microhal::Bitfield<uint32_t, 18, 1> TIM17RST;  /*!< TIM17 timer reset */
            microhal::Bitfield<uint32_t, 20, 1> ADCRST;    /*!< ADC reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APBRSTR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APBRSTR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APBRSTR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APBRSTR2 operator&(uint32_t value) const {
            APBRSTR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APBRSTR2 operator|(uint32_t value) const {
            APBRSTR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APBRSTR2, AccessType::ReadOnly>;
        friend class VolatileRegister<APBRSTR2, AccessType::WriteOnly>;
        friend class VolatileRegister<APBRSTR2, AccessType::ReadWrite>;
    };

    // GPIO clock enable register
    union IOPENR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> IOPAEN; /*!< I/O port A clock enable */
            microhal::Bitfield<uint32_t, 1, 1> IOPBEN; /*!< I/O port B clock enable */
            microhal::Bitfield<uint32_t, 2, 1> IOPCEN; /*!< I/O port C clock enable */
            microhal::Bitfield<uint32_t, 3, 1> IOPDEN; /*!< I/O port D clock enable */
            microhal::Bitfield<uint32_t, 4, 1> IOPEEN; /*!< I/O port D clock enable */
            microhal::Bitfield<uint32_t, 5, 1> IOPFEN; /*!< I/O port F clock enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IOPENR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IOPENR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IOPENR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IOPENR operator&(uint32_t value) const {
            IOPENR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IOPENR operator|(uint32_t value) const {
            IOPENR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IOPENR, AccessType::ReadOnly>;
        friend class VolatileRegister<IOPENR, AccessType::WriteOnly>;
        friend class VolatileRegister<IOPENR, AccessType::ReadWrite>;
    };

    // AHB peripheral clock enable register
    union AHBENR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> DMAEN;   /*!< DMA clock enable */
            microhal::Bitfield<uint32_t, 8, 1> FLASHEN; /*!< Flash memory interface clock enable */
            microhal::Bitfield<uint32_t, 12, 1> CRCEN;  /*!< CRC clock enable */
            microhal::Bitfield<uint32_t, 16, 1> AESEN;  /*!< AES hardware accelerator */
            microhal::Bitfield<uint32_t, 18, 1> RNGEN;  /*!< Random number generator clock enable */
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

    // APB peripheral clock enable register 1
    union APBENR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TIM2EN;     /*!< TIM2 timer clock enable */
            microhal::Bitfield<uint32_t, 1, 1> TIM3EN;     /*!< TIM3 timer clock enable */
            microhal::Bitfield<uint32_t, 4, 1> TIM6EN;     /*!< TIM6 timer clock enable */
            microhal::Bitfield<uint32_t, 5, 1> TIM7EN;     /*!< TIM7 timer clock enable */
            microhal::Bitfield<uint32_t, 10, 1> RTCAPBEN;  /*!< RTC APB clock enable */
            microhal::Bitfield<uint32_t, 11, 1> WWDGEN;    /*!< WWDG clock enable */
            microhal::Bitfield<uint32_t, 14, 1> SPI2EN;    /*!< SPI2 clock enable */
            microhal::Bitfield<uint32_t, 17, 1> USART2EN;  /*!< USART2 clock enable */
            microhal::Bitfield<uint32_t, 18, 1> USART3EN;  /*!< USART3 clock enable */
            microhal::Bitfield<uint32_t, 19, 1> USART4EN;  /*!< USART4 clock enable */
            microhal::Bitfield<uint32_t, 20, 1> LPUART1EN; /*!< LPUART1 clock enable */
            microhal::Bitfield<uint32_t, 21, 1> I2C1EN;    /*!< I2C1 clock enable */
            microhal::Bitfield<uint32_t, 22, 1> I2C2EN;    /*!< I2C2 clock enable */
            microhal::Bitfield<uint32_t, 24, 1> CECEN;     /*!< HDMI CEC clock enable */
            microhal::Bitfield<uint32_t, 25, 1> UCPD1EN;   /*!< UCPD1 clock enable */
            microhal::Bitfield<uint32_t, 26, 1> UCPD2EN;   /*!< UCPD2 clock enable */
            microhal::Bitfield<uint32_t, 27, 1> DBGEN;     /*!< Debug support clock enable */
            microhal::Bitfield<uint32_t, 28, 1> PWREN;     /*!< Power interface clock enable */
            microhal::Bitfield<uint32_t, 29, 1> DAC1EN;    /*!< DAC1 interface clock enable */
            microhal::Bitfield<uint32_t, 30, 1> LPTIM2EN;  /*!< LPTIM2 clock enable */
            microhal::Bitfield<uint32_t, 31, 1> LPTIM1EN;  /*!< LPTIM1 clock enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APBENR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APBENR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APBENR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APBENR1 operator&(uint32_t value) const {
            APBENR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APBENR1 operator|(uint32_t value) const {
            APBENR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APBENR1, AccessType::ReadOnly>;
        friend class VolatileRegister<APBENR1, AccessType::WriteOnly>;
        friend class VolatileRegister<APBENR1, AccessType::ReadWrite>;
    };

    // APB peripheral clock enable register 2
    union APBENR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SYSCFGEN;  /*!< SYSCFG, COMP and VREFBUF clock enable */
            microhal::Bitfield<uint32_t, 11, 1> TIM1EN;   /*!< TIM1 timer clock enable */
            microhal::Bitfield<uint32_t, 12, 1> SPI1EN;   /*!< SPI1 clock enable */
            microhal::Bitfield<uint32_t, 14, 1> USART1EN; /*!< USART1 clock enable */
            microhal::Bitfield<uint32_t, 15, 1> TIM14EN;  /*!< TIM14 timer clock enable */
            microhal::Bitfield<uint32_t, 16, 1> TIM15EN;  /*!< TIM15 timer clock enable */
            microhal::Bitfield<uint32_t, 17, 1> TIM16EN;  /*!< TIM16 timer clock enable */
            microhal::Bitfield<uint32_t, 18, 1> TIM17EN;  /*!< TIM16 timer clock enable */
            microhal::Bitfield<uint32_t, 20, 1> ADC1EN;   /*!< ADC clock enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APBENR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APBENR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APBENR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APBENR2 operator&(uint32_t value) const {
            APBENR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APBENR2 operator|(uint32_t value) const {
            APBENR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APBENR2, AccessType::ReadOnly>;
        friend class VolatileRegister<APBENR2, AccessType::WriteOnly>;
        friend class VolatileRegister<APBENR2, AccessType::ReadWrite>;
    };

    // GPIO in Sleep mode clock enable register
    union IOPSMENR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> IOPASMEN; /*!< I/O port A clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 1, 1> IOPBSMEN; /*!< I/O port B clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 2, 1> IOPCSMEN; /*!< I/O port C clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 3, 1> IOPDSMEN; /*!< I/O port D clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 5, 1> IOPFSMEN; /*!< I/O port F clock enable during Sleep mode */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IOPSMENR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IOPSMENR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IOPSMENR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IOPSMENR operator&(uint32_t value) const {
            IOPSMENR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IOPSMENR operator|(uint32_t value) const {
            IOPSMENR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IOPSMENR, AccessType::ReadOnly>;
        friend class VolatileRegister<IOPSMENR, AccessType::WriteOnly>;
        friend class VolatileRegister<IOPSMENR, AccessType::ReadWrite>;
    };

    // AHB peripheral clock enable in Sleep mode register
    union AHBSMENR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> DMASMEN;   /*!< DMA clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 8, 1> FLASHSMEN; /*!< Flash memory interface clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 9, 1> SRAMSMEN;  /*!< SRAM clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 12, 1> CRCSMEN;  /*!< CRC clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 16, 1> AESSMEN;  /*!< AES hardware accelerator clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 18, 1> RNGSMEN;  /*!< Random number generator clock enable during Sleep mode */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        AHBSMENR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        AHBSMENR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        AHBSMENR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        AHBSMENR operator&(uint32_t value) const {
            AHBSMENR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        AHBSMENR operator|(uint32_t value) const {
            AHBSMENR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<AHBSMENR, AccessType::ReadOnly>;
        friend class VolatileRegister<AHBSMENR, AccessType::WriteOnly>;
        friend class VolatileRegister<AHBSMENR, AccessType::ReadWrite>;
    };

    // APB peripheral clock enable in Sleep mode register 1
    union APBSMENR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TIM2SMEN;     /*!< TIM2 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 1, 1> TIM3SMEN;     /*!< TIM3 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 4, 1> TIM6SMEN;     /*!< TIM6 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 5, 1> TIM7SMEN;     /*!< TIM7 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 10, 1> RTCAPBSMEN;  /*!< RTC APB clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 11, 1> WWDGSMEN;    /*!< WWDG clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 14, 1> SPI2SMEN;    /*!< SPI2 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 17, 1> USART2SMEN;  /*!< USART2 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 18, 1> USART3SMEN;  /*!< USART3 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 19, 1> USART4SMEN;  /*!< USART4 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 20, 1> LPUART1SMEN; /*!< LPUART1 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 21, 1> I2C1SMEN;    /*!< I2C1 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 22, 1> I2C2SMEN;    /*!< I2C2 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 24, 1> CECSMEN;     /*!< HDMI CEC clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 25, 1> UCPD1SMEN;   /*!< UCPD1 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 26, 1> UCPD2SMEN;   /*!< UCPD2 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 27, 1> DBGSMEN;     /*!< Debug support clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 28, 1> PWRSMEN;     /*!< Power interface clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 29, 1> DAC1SMEN;    /*!< DAC1 interface clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 30, 1> LPTIM2SMEN;  /*!< Low Power Timer 2 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 31, 1> LPTIM1SMEN;  /*!< Low Power Timer 1 clock enable during Sleep mode */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APBSMENR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APBSMENR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APBSMENR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APBSMENR1 operator&(uint32_t value) const {
            APBSMENR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APBSMENR1 operator|(uint32_t value) const {
            APBSMENR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APBSMENR1, AccessType::ReadOnly>;
        friend class VolatileRegister<APBSMENR1, AccessType::WriteOnly>;
        friend class VolatileRegister<APBSMENR1, AccessType::ReadWrite>;
    };

    // APB peripheral clock enable in Sleep mode register 2
    union APBSMENR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SYSCFGSMEN;  /*!< SYSCFG, COMP and VREFBUF clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 11, 1> TIM1SMEN;   /*!< TIM1 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 12, 1> SPI1SMEN;   /*!< SPI1 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 14, 1> USART1SMEN; /*!< USART1 clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 15, 1> TIM14SMEN;  /*!< TIM14 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 16, 1> TIM15SMEN;  /*!< TIM15 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 17, 1> TIM16SMEN;  /*!< TIM16 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 18, 1> TIM17SMEN;  /*!< TIM16 timer clock enable during Sleep mode */
            microhal::Bitfield<uint32_t, 20, 1> ADCSMEN;    /*!< ADC clock enable during Sleep mode */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        APBSMENR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        APBSMENR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        APBSMENR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        APBSMENR2 operator&(uint32_t value) const {
            APBSMENR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        APBSMENR2 operator|(uint32_t value) const {
            APBSMENR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<APBSMENR2, AccessType::ReadOnly>;
        friend class VolatileRegister<APBSMENR2, AccessType::WriteOnly>;
        friend class VolatileRegister<APBSMENR2, AccessType::ReadWrite>;
    };

    // Peripherals independent clock configuration register
    union CCIPR {
        union {
            microhal::Bitfield<uint32_t, 0, 2> USART1SEL;   /*!< USART1 clock source selection */
            microhal::Bitfield<uint32_t, 2, 2> USART2SEL;   /*!< USART2 clock source selection */
            microhal::Bitfield<uint32_t, 4, 2> USART3SEL;   /*!< USART2 clock source selection */
            microhal::Bitfield<uint32_t, 6, 1> CECSEL;      /*!< HDMI CEC clock source selection */
            microhal::Bitfield<uint32_t, 10, 2> LPUART1SEL; /*!< LPUART1 clock source selection */
            microhal::Bitfield<uint32_t, 12, 2> I2C1SEL;    /*!< I2C1 clock source selection */
            microhal::Bitfield<uint32_t, 14, 2> I2S2SEL;    /*!< I2S1 clock source selection */
            microhal::Bitfield<uint32_t, 18, 2> LPTIM1SEL;  /*!< LPTIM1 clock source selection */
            microhal::Bitfield<uint32_t, 20, 2> LPTIM2SEL;  /*!< LPTIM2 clock source selection */
            microhal::Bitfield<uint32_t, 22, 1> TIM1SEL;    /*!< TIM1 clock source selection */
            microhal::Bitfield<uint32_t, 24, 1> TIM15SEL;   /*!< TIM15 clock source selection */
            microhal::Bitfield<uint32_t, 26, 2> RNGSEL;     /*!< RNG clock source selection */
            microhal::Bitfield<uint32_t, 28, 2> RNGDIV;     /*!< Division factor of RNG clock divider */
            microhal::Bitfield<uint32_t, 30, 2> ADCSEL;     /*!< ADCs clock source selection */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CCIPR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CCIPR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CCIPR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CCIPR operator&(uint32_t value) const {
            CCIPR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CCIPR operator|(uint32_t value) const {
            CCIPR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CCIPR, AccessType::ReadOnly>;
        friend class VolatileRegister<CCIPR, AccessType::WriteOnly>;
        friend class VolatileRegister<CCIPR, AccessType::ReadWrite>;
    };

    // RTC domain control register
    union BDCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSEON;    /*!< LSE oscillator enable */
            microhal::Bitfield<uint32_t, 1, 1> LSERDY;   /*!< LSE oscillator ready */
            microhal::Bitfield<uint32_t, 2, 1> LSEBYP;   /*!< LSE oscillator bypass */
            microhal::Bitfield<uint32_t, 3, 2> LSEDRV;   /*!< LSE oscillator drive capability */
            microhal::Bitfield<uint32_t, 5, 1> LSECSSON; /*!< CSS on LSE enable */
            microhal::Bitfield<uint32_t, 6, 1> LSECSSD;  /*!< CSS on LSE failure Detection */
            microhal::Bitfield<uint32_t, 8, 2> RTCSEL;   /*!< RTC clock source selection */
            microhal::Bitfield<uint32_t, 15, 1> RTCEN;   /*!< RTC clock enable */
            microhal::Bitfield<uint32_t, 16, 1> BDRST;   /*!< RTC domain software reset */
            microhal::Bitfield<uint32_t, 24, 1> LSCOEN;  /*!< Low-speed clock output (LSCO) enable */
            microhal::Bitfield<uint32_t, 25, 1> LSCOSEL; /*!< Low-speed clock output selection */
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

    // Control/status register
    union CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LSION;     /*!< LSI oscillator enable */
            microhal::Bitfield<uint32_t, 1, 1> LSIRDY;    /*!< LSI oscillator ready */
            microhal::Bitfield<uint32_t, 23, 1> RMVF;     /*!< Remove reset flags */
            microhal::Bitfield<uint32_t, 25, 1> OBLRSTF;  /*!< Option byte loader reset flag */
            microhal::Bitfield<uint32_t, 26, 1> PINRSTF;  /*!< Pin reset flag */
            microhal::Bitfield<uint32_t, 27, 1> PWRRSTF;  /*!< BOR or POR/PDR flag */
            microhal::Bitfield<uint32_t, 28, 1> SFTRSTF;  /*!< Software reset flag */
            microhal::Bitfield<uint32_t, 29, 1> IWDGRSTF; /*!< Independent window watchdog reset flag */
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

    VolatileRegister<CR, AccessType::ReadWrite> cr;             /*!< Clock control register	Address offset: 0x0 */
    VolatileRegister<ICSCR, AccessType::ReadWrite> icscr;       /*!< Internal clock sources calibration register	Address offset: 0x4 */
    VolatileRegister<CFGR, AccessType::ReadWrite> cfgr;         /*!< Clock configuration register	Address offset: 0x8 */
    VolatileRegister<PLLCFGR, AccessType::ReadWrite> pllcfgr;   /*!< PLL configuration register	Address offset: 0xC */
    uint32_t reserved0[2];                                      /*!< Reserved register	Address offset: 0x10 */
    VolatileRegister<CIER, AccessType::ReadWrite> cier;         /*!< Clock interrupt enable register	Address offset: 0x18 */
    VolatileRegister<CIFR, AccessType::ReadWrite> cifr;         /*!< Clock interrupt flag register	Address offset: 0x1C */
    VolatileRegister<CICR, AccessType::ReadWrite> cicr;         /*!< Clock interrupt clear register	Address offset: 0x20 */
    VolatileRegister<IOPRSTR, AccessType::ReadWrite> ioprstr;   /*!< GPIO reset register	Address offset: 0x24 */
    VolatileRegister<AHBRSTR, AccessType::ReadWrite> ahbrstr;   /*!< AHB peripheral reset register	Address offset: 0x28 */
    VolatileRegister<APBRSTR1, AccessType::ReadWrite> apbrstr1; /*!< APB peripheral reset register 1	Address offset: 0x2C */
    VolatileRegister<APBRSTR2, AccessType::ReadWrite> apbrstr2; /*!< APB peripheral reset register 2	Address offset: 0x30 */
    VolatileRegister<IOPENR, AccessType::ReadWrite> iopenr;     /*!< GPIO clock enable register	Address offset: 0x34 */
    VolatileRegister<AHBENR, AccessType::ReadWrite> ahbenr;     /*!< AHB peripheral clock enable register	Address offset: 0x38 */
    union {
        VolatileRegister<APBENR1, AccessType::ReadWrite> apbenr1; /*!< APB peripheral clock enable register 1	Address offset: 0x3C */
        VolatileRegister<APBENR1, AccessType::ReadWrite> apb1enr; /*!< APB peripheral clock enable register 1	Address offset: 0x3C */
    };
    union {
        VolatileRegister<APBENR2, AccessType::ReadWrite> apbenr2; /*!< APB peripheral clock enable register 2	Address offset: 0x40 */
        VolatileRegister<APBENR2, AccessType::ReadWrite> apb2enr; /*!< APB peripheral clock enable register 2	Address offset: 0x40 */
    };
    VolatileRegister<IOPSMENR, AccessType::ReadWrite> iopsmenr;   /*!< GPIO in Sleep mode clock enable register	Address offset: 0x44 */
    VolatileRegister<AHBSMENR, AccessType::ReadWrite> ahbsmenr;   /*!< AHB peripheral clock enable in Sleep mode register	Address offset: 0x48 */
    VolatileRegister<APBSMENR1, AccessType::ReadWrite> apbsmenr1; /*!< APB peripheral clock enable in Sleep mode register 1 Address offset: 0x4C */
    VolatileRegister<APBSMENR2, AccessType::ReadWrite> apbsmenr2; /*!< APB peripheral clock enable in Sleep mode register 2 Address offset: 0x50 */
    VolatileRegister<CCIPR, AccessType::ReadWrite> ccipr;         /*!< Peripherals independent clock configuration register Address offset: 0x54 */
    uint32_t reserved4;                                           /*!< Reserved register	Address offset: 0x58 */
    VolatileRegister<BDCR, AccessType::ReadWrite> bdcr;           /*!< RTC domain control register	Address offset: 0x5C */
    VolatileRegister<CSR, AccessType::ReadWrite> csr;             /*!< Control/status register	Address offset: 0x60 */
};

[[maybe_unused]] inline RCC *const rcc = reinterpret_cast<RCC *>(_MICROHAL_RCC_BASE_ADDRESS);

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_RCC
