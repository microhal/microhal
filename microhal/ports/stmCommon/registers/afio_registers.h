#ifndef _MICROHAL_PERIPHERAL_REGISTER_AFIO
#define _MICROHAL_PERIPHERAL_REGISTER_AFIO

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include <array>
#include "registersBaseAddressDefinitions.h"

// Supported MCU: STM32F102, STM32F103, STM32F101, STM32F107, STM32F100
namespace microhal {
namespace registers {
/**
 *
 */
struct AFIO {
    // Event Control Register (AFIO_EVCR)
    union EVCR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> PIN;  /*!< Pin selection */
            microhal::Bitfield<uint32_t, 4, 3> PORT; /*!< Port selection */
            microhal::Bitfield<uint32_t, 7, 1> EVOE; /*!< Event Output Enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        EVCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        EVCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        EVCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        EVCR operator&(uint32_t value) const {
            EVCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        EVCR operator|(uint32_t value) const {
            EVCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<EVCR, AccessType::ReadOnly>;
        friend class VolatileRegister<EVCR, AccessType::WriteOnly>;
        friend class VolatileRegister<EVCR, AccessType::ReadWrite>;
    };

    // AF remap and debug I/O configuration register (AFIO_MAPR)
    union MAPR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SPI1_REMAP;          /*!< SPI1 remapping */
            microhal::Bitfield<uint32_t, 1, 1> I2C1_REMAP;          /*!< I2C1 remapping */
            microhal::Bitfield<uint32_t, 2, 1> USART1_REMAP;        /*!< USART1 remapping */
            microhal::Bitfield<uint32_t, 3, 1> USART2_REMAP;        /*!< USART2 remapping */
            microhal::Bitfield<uint32_t, 4, 2> USART3_REMAP;        /*!< USART3 remapping */
            microhal::Bitfield<uint32_t, 6, 2> TIM1_REMAP;          /*!< TIM1 remapping */
            microhal::Bitfield<uint32_t, 8, 2> TIM2_REMAP;          /*!< TIM2 remapping */
            microhal::Bitfield<uint32_t, 10, 2> TIM3_REMAP;         /*!< TIM3 remapping */
            microhal::Bitfield<uint32_t, 12, 1> TIM4_REMAP;         /*!< TIM4 remapping */
            microhal::Bitfield<uint32_t, 13, 2> CAN1_REMAP;         /*!< CAN1 remapping */
            microhal::Bitfield<uint32_t, 15, 1> PD01_REMAP;         /*!< Port D0/Port D1 mapping on OSCIN/OSCOUT */
            microhal::Bitfield<uint32_t, 16, 1> TIM5CH4_IREMAP;     /*!< Set and cleared by software */
            microhal::Bitfield<uint32_t, 17, 1> ADC1_ETRGINJ_REMAP; /*!< ADC 1 External trigger injected conversion remapping */
            microhal::Bitfield<uint32_t, 18, 1> ADC1_ETRGREG_REMAP; /*!< ADC 1 external trigger regular conversion remapping */
            microhal::Bitfield<uint32_t, 19, 1> ADC2_ETRGINJ_REMAP; /*!< ADC 2 external trigger injected conversion remapping */
            microhal::Bitfield<uint32_t, 20, 1> ADC2_ETRGREG_REMAP; /*!< ADC 2 external trigger regular conversion remapping */
            microhal::Bitfield<uint32_t, 21, 1> ETH_REMAP;          /*!< Ethernet MAC I/O remapping */
            microhal::Bitfield<uint32_t, 22, 1> CAN2_REMAP;         /*!< CAN2 I/O remapping */
            microhal::Bitfield<uint32_t, 23, 1> MII_RMII_SEL;       /*!< MII or RMII selection */
            microhal::Bitfield<uint32_t, 24, 3> SWJ_CFG;            /*!< Serial wire JTAG configuration */
            microhal::Bitfield<uint32_t, 28, 1> SPI3_REMAP;         /*!< SPI3/I2S3 remapping */
            microhal::Bitfield<uint32_t, 29, 1> TIM2ITR1_IREMAP;    /*!< TIM2 internal trigger 1 remapping */
            microhal::Bitfield<uint32_t, 30, 1> PTP_PPS_REMAP;      /*!< Ethernet PTP PPS remapping */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MAPR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MAPR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MAPR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MAPR operator&(uint32_t value) const {
            MAPR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MAPR operator|(uint32_t value) const {
            MAPR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MAPR, AccessType::ReadOnly>;
        friend class VolatileRegister<MAPR, AccessType::WriteOnly>;
        friend class VolatileRegister<MAPR, AccessType::ReadWrite>;
    };

    // External interrupt configuration register 1 (AFIO_EXTICR)
    union EXTICR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> EXTI0;  /*!< EXTI0 configuration */
            microhal::Bitfield<uint32_t, 4, 4> EXTI1;  /*!< EXTI1 configuration */
            microhal::Bitfield<uint32_t, 8, 4> EXTI2;  /*!< EXTI2 configuration */
            microhal::Bitfield<uint32_t, 12, 4> EXTI3; /*!< EXTI3 configuration */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        EXTICR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        EXTICR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        EXTICR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        EXTICR operator&(uint32_t value) const {
            EXTICR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        EXTICR operator|(uint32_t value) const {
            EXTICR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<EXTICR, AccessType::ReadOnly>;
        friend class VolatileRegister<EXTICR, AccessType::WriteOnly>;
        friend class VolatileRegister<EXTICR, AccessType::ReadWrite>;
    };

    // AF remap and debug I/O configuration register
    union MAPR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TIM15_REMAP;          /*!< TIM15 remapping */
            microhal::Bitfield<uint32_t, 1, 1> TIM16_REMAP;          /*!< TIM16 remapping */
            microhal::Bitfield<uint32_t, 2, 1> TIM17_REMAP;          /*!< TIM17 remapping */
            microhal::Bitfield<uint32_t, 3, 1> CEC_REMAP;            /*!< CEC remapping */
            microhal::Bitfield<uint32_t, 4, 1> TIM1_DMA_REMAP;       /*!< TIM1 DMA remapping */
            microhal::Bitfield<uint32_t, 5, 1> TIM9_REMAP;           /*!< TIM9 remapping */
            microhal::Bitfield<uint32_t, 6, 1> TIM10_REMAP;          /*!< TIM10 remapping */
            microhal::Bitfield<uint32_t, 7, 1> TIM11_REMAP;          /*!< TIM11 remapping */
            microhal::Bitfield<uint32_t, 8, 1> TIM13_REMAP;          /*!< TIM13 remapping */
            microhal::Bitfield<uint32_t, 9, 1> TIM14_REMAP;          /*!< TIM14 remapping */
            microhal::Bitfield<uint32_t, 10, 1> FSMC_NADV;           /*!< NADV connect/disconnect */
            microhal::Bitfield<uint32_t, 11, 1> TIM67_DAC_DMA_REMAP; /*!< TIM67_DAC DMA remapping */
            microhal::Bitfield<uint32_t, 12, 1> TIM12_REMAP;         /*!< TIM12 remapping */
            microhal::Bitfield<uint32_t, 13, 1> MISC_REMAP;          /*!< Miscellaneous features remapping */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MAPR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MAPR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MAPR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MAPR2 operator&(uint32_t value) const {
            MAPR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MAPR2 operator|(uint32_t value) const {
            MAPR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MAPR2, AccessType::ReadOnly>;
        friend class VolatileRegister<MAPR2, AccessType::WriteOnly>;
        friend class VolatileRegister<MAPR2, AccessType::ReadWrite>;
    };

    VolatileRegister<EVCR, AccessType::ReadWrite> evcr;        /*!< Event Control Register (AFIO_EVCR)	Address offset: 0x0 */
    VolatileRegister<MAPR, AccessType::ReadWrite> mapr;        /*!< AF remap and debug I/O configuration register (AFIO_MAPR)	Address offset: 0x4 */
    VolatileRegister<EXTICR, AccessType::ReadWrite> exticr[4]; /*!< External interrupt configuration register 1 (AFIO_EXTICR1)	Address offset: 0x8 */
    uint32_t reserved0;                                        /*!< Reserved register	Address offset: 0x18 */
    VolatileRegister<MAPR2, AccessType::ReadWrite> mapr2;      /*!< AF remap and debug I/O configuration register	Address offset: 0x1C */
};

#if defined(_MICROHAL_AFIO_BASE_ADDRESS)
[[maybe_unused]] inline AFIO *const afio1 = reinterpret_cast<AFIO *>(_MICROHAL_AFIO_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_AFIO
