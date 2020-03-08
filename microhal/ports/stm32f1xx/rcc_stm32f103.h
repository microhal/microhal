#ifndef _MICROHAL_PERIPHERAL_REGISTER_RCC
#define _MICROHAL_PERIPHERAL_REGISTER_RCC

#include "ports/stmCommon/registers/registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F103

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
            microhal::Bitfield<uint32_t, 11, 3> PPRE2;    /*!< APB High speed prescaler (APB2) */
            microhal::Bitfield<uint32_t, 14, 2> ADCPRE;   /*!< ADC prescaler */
            microhal::Bitfield<uint32_t, 16, 1> PLLSRC;   /*!< PLL entry clock source */
            microhal::Bitfield<uint32_t, 17, 1> PLLXTPRE; /*!< HSE divider for PLL entry */
            microhal::Bitfield<uint32_t, 18, 4> PLLMUL;   /*!< PLL Multiplication Factor */
            microhal::Bitfield<uint32_t, 22, 1> OTGFSPRE; /*!< USB OTG FS prescaler */
            microhal::Bitfield<uint32_t, 24, 3> MCO;      /*!< Microcontroller clock output */
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
            microhal::Bitfield<uint32_t, 0, 1> AFIORST;    /*!< Alternate function I/O reset */
            microhal::Bitfield<uint32_t, 2, 1> IOPARST;    /*!< IO port A reset */
            microhal::Bitfield<uint32_t, 3, 1> IOPBRST;    /*!< IO port B reset */
            microhal::Bitfield<uint32_t, 4, 1> IOPCRST;    /*!< IO port C reset */
            microhal::Bitfield<uint32_t, 5, 1> IOPDRST;    /*!< IO port D reset */
            microhal::Bitfield<uint32_t, 6, 1> IOPERST;    /*!< IO port E reset */
            microhal::Bitfield<uint32_t, 7, 1> IOPFRST;    /*!< IO port F reset */
            microhal::Bitfield<uint32_t, 8, 1> IOPGRST;    /*!< IO port G reset */
            microhal::Bitfield<uint32_t, 9, 1> ADC1RST;    /*!< ADC 1 interface reset */
            microhal::Bitfield<uint32_t, 10, 1> ADC2RST;   /*!< ADC 2 interface reset */
            microhal::Bitfield<uint32_t, 11, 1> TIM1RST;   /*!< TIM1 timer reset */
            microhal::Bitfield<uint32_t, 12, 1> SPI1RST;   /*!< SPI 1 reset */
            microhal::Bitfield<uint32_t, 13, 1> TIM8RST;   /*!< TIM8 timer reset */
            microhal::Bitfield<uint32_t, 14, 1> USART1RST; /*!< USART1 reset */
            microhal::Bitfield<uint32_t, 15, 1> ADC3RST;   /*!< ADC 3 interface reset */
            microhal::Bitfield<uint32_t, 19, 1> TIM9RST;   /*!< TIM9 timer reset */
            microhal::Bitfield<uint32_t, 20, 1> TIM10RST;  /*!< TIM10 timer reset */
            microhal::Bitfield<uint32_t, 21, 1> TIM11RST;  /*!< TIM11 timer reset */
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
            microhal::Bitfield<uint32_t, 2, 1> TIM4RST;    /*!< Timer 4 reset */
            microhal::Bitfield<uint32_t, 3, 1> TIM5RST;    /*!< Timer 5 reset */
            microhal::Bitfield<uint32_t, 4, 1> TIM6RST;    /*!< Timer 6 reset */
            microhal::Bitfield<uint32_t, 5, 1> TIM7RST;    /*!< Timer 7 reset */
            microhal::Bitfield<uint32_t, 6, 1> TIM12RST;   /*!< Timer 12 reset */
            microhal::Bitfield<uint32_t, 7, 1> TIM13RST;   /*!< Timer 13 reset */
            microhal::Bitfield<uint32_t, 8, 1> TIM14RST;   /*!< Timer 14 reset */
            microhal::Bitfield<uint32_t, 11, 1> WWDGRST;   /*!< Window watchdog reset */
            microhal::Bitfield<uint32_t, 14, 1> SPI2RST;   /*!< SPI2 reset */
            microhal::Bitfield<uint32_t, 15, 1> SPI3RST;   /*!< SPI3 reset */
            microhal::Bitfield<uint32_t, 17, 1> USART2RST; /*!< USART 2 reset */
            microhal::Bitfield<uint32_t, 18, 1> USART3RST; /*!< USART 3 reset */
            microhal::Bitfield<uint32_t, 19, 1> UART4RST;  /*!< UART 4 reset */
            microhal::Bitfield<uint32_t, 20, 1> UART5RST;  /*!< UART 5 reset */
            microhal::Bitfield<uint32_t, 21, 1> I2C1RST;   /*!< I2C1 reset */
            microhal::Bitfield<uint32_t, 22, 1> I2C2RST;   /*!< I2C2 reset */
            microhal::Bitfield<uint32_t, 23, 1> USBRST;    /*!< USB reset */
            microhal::Bitfield<uint32_t, 25, 1> CANRST;    /*!< CAN reset */
            microhal::Bitfield<uint32_t, 27, 1> BKPRST;    /*!< Backup interface reset */
            microhal::Bitfield<uint32_t, 28, 1> PWRRST;    /*!< Power interface reset */
            microhal::Bitfield<uint32_t, 29, 1> DACRST;    /*!< DAC interface reset */
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
            microhal::Bitfield<uint32_t, 0, 1> DMA1EN;  /*!< DMA1 clock enable */
            microhal::Bitfield<uint32_t, 1, 1> DMA2EN;  /*!< DMA2 clock enable */
            microhal::Bitfield<uint32_t, 2, 1> SRAMEN;  /*!< SRAM interface clock enable */
            microhal::Bitfield<uint32_t, 4, 1> FLITFEN; /*!< FLITF clock enable */
            microhal::Bitfield<uint32_t, 6, 1> CRCEN;   /*!< CRC clock enable */
            microhal::Bitfield<uint32_t, 8, 1> FSMCEN;  /*!< FSMC clock enable */
            microhal::Bitfield<uint32_t, 10, 1> SDIOEN; /*!< SDIO clock enable */
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
#define _MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC1EN
#define _MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC2EN
#define _MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC3EN
        union {
            microhal::Bitfield<uint32_t, 0, 1> AFIOEN;    /*!< Alternate function I/O clock enable */
            microhal::Bitfield<uint32_t, 2, 1> IOPAEN;    /*!< I/O port A clock enable */
            microhal::Bitfield<uint32_t, 3, 1> IOPBEN;    /*!< I/O port B clock enable */
            microhal::Bitfield<uint32_t, 4, 1> IOPCEN;    /*!< I/O port C clock enable */
            microhal::Bitfield<uint32_t, 5, 1> IOPDEN;    /*!< I/O port D clock enable */
            microhal::Bitfield<uint32_t, 6, 1> IOPEEN;    /*!< I/O port E clock enable */
            microhal::Bitfield<uint32_t, 7, 1> IOPFEN;    /*!< I/O port F clock enable */
            microhal::Bitfield<uint32_t, 8, 1> IOPGEN;    /*!< I/O port G clock enable */
            microhal::Bitfield<uint32_t, 9, 1> ADC1EN;    /*!< ADC 1 interface clock enable */
            microhal::Bitfield<uint32_t, 10, 1> ADC2EN;   /*!< ADC 2 interface clock enable */
            microhal::Bitfield<uint32_t, 11, 1> TIM1EN;   /*!< TIM1 Timer clock enable */
            microhal::Bitfield<uint32_t, 12, 1> SPI1EN;   /*!< SPI 1 clock enable */
            microhal::Bitfield<uint32_t, 13, 1> TIM8EN;   /*!< TIM8 Timer clock enable */
            microhal::Bitfield<uint32_t, 14, 1> USART1EN; /*!< USART1 clock enable */
            microhal::Bitfield<uint32_t, 15, 1> ADC3EN;   /*!< ADC3 interface clock enable */
            microhal::Bitfield<uint32_t, 19, 1> TIM9EN;   /*!< TIM9 Timer clock enable */
            microhal::Bitfield<uint32_t, 20, 1> TIM10EN;  /*!< TIM10 Timer clock enable */
            microhal::Bitfield<uint32_t, 21, 1> TIM11EN;  /*!< TIM11 Timer clock enable */
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
            microhal::Bitfield<uint32_t, 3, 1> TIM5EN;    /*!< Timer 5 clock enable */
            microhal::Bitfield<uint32_t, 4, 1> TIM6EN;    /*!< Timer 6 clock enable */
            microhal::Bitfield<uint32_t, 5, 1> TIM7EN;    /*!< Timer 7 clock enable */
            microhal::Bitfield<uint32_t, 6, 1> TIM12EN;   /*!< Timer 12 clock enable */
            microhal::Bitfield<uint32_t, 7, 1> TIM13EN;   /*!< Timer 13 clock enable */
            microhal::Bitfield<uint32_t, 8, 1> TIM14EN;   /*!< Timer 14 clock enable */
            microhal::Bitfield<uint32_t, 11, 1> WWDGEN;   /*!< Window watchdog clock enable */
            microhal::Bitfield<uint32_t, 14, 1> SPI2EN;   /*!< SPI 2 clock enable */
            microhal::Bitfield<uint32_t, 15, 1> SPI3EN;   /*!< SPI 3 clock enable */
            microhal::Bitfield<uint32_t, 17, 1> USART2EN; /*!< USART 2 clock enable */
            microhal::Bitfield<uint32_t, 18, 1> USART3EN; /*!< USART 3 clock enable */
            microhal::Bitfield<uint32_t, 19, 1> UART4EN;  /*!< UART 4 clock enable */
            microhal::Bitfield<uint32_t, 20, 1> UART5EN;  /*!< UART 5 clock enable */
            microhal::Bitfield<uint32_t, 21, 1> I2C1EN;   /*!< I2C 1 clock enable */
            microhal::Bitfield<uint32_t, 22, 1> I2C2EN;   /*!< I2C 2 clock enable */
            microhal::Bitfield<uint32_t, 23, 1> USBEN;    /*!< USB clock enable */
            microhal::Bitfield<uint32_t, 25, 1> CANEN;    /*!< CAN clock enable */
            microhal::Bitfield<uint32_t, 27, 1> BKPEN;    /*!< Backup interface clock enable */
            microhal::Bitfield<uint32_t, 28, 1> PWREN;    /*!< Power interface clock enable */
            microhal::Bitfield<uint32_t, 29, 1> DACEN;    /*!< DAC interface clock enable */
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

    VolatileRegister<CR, AccessType::ReadWrite> cr;             /*!< Clock control register	Address offset: 0x0 */
    VolatileRegister<CFGR, AccessType::ReadWrite> cfgr;         /*!< Clock configuration register (RCC_CFGR)	Address offset: 0x4 */
    VolatileRegister<CIR, AccessType::ReadWrite> cir;           /*!< Clock interrupt register (RCC_CIR)	Address offset: 0x8 */
    VolatileRegister<APB2RSTR, AccessType::ReadWrite> apb2rstr; /*!< APB2 peripheral reset register (RCC_APB2RSTR)	Address offset: 0xC */
    VolatileRegister<APB1RSTR, AccessType::ReadWrite> apb1rstr; /*!< APB1 peripheral reset register (RCC_APB1RSTR)	Address offset: 0x10 */
    VolatileRegister<AHBENR, AccessType::ReadWrite> ahbenr;     /*!< AHB Peripheral Clock enable register (RCC_AHBENR)	Address offset: 0x14 */
    VolatileRegister<APB2ENR, AccessType::ReadWrite> apb2enr;   /*!< APB2 peripheral clock enable register (RCC_APB2ENR)	Address offset: 0x18 */
    VolatileRegister<APB1ENR, AccessType::ReadWrite> apb1enr;   /*!< APB1 peripheral clock enable register (RCC_APB1ENR)	Address offset: 0x1C */
    VolatileRegister<BDCR, AccessType::ReadWrite> bdcr;         /*!< Backup domain control register (RCC_BDCR)	Address offset: 0x20 */
    VolatileRegister<CSR, AccessType::ReadWrite> csr;           /*!< Control/status register (RCC_CSR)	Address offset: 0x24 */
};

[[maybe_unused]] static RCC *const rcc = reinterpret_cast<RCC *>(_MICROHAL_RCC_BASE_ADDRESS);

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_RCC
