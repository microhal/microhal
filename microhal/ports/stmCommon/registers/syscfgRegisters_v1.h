#ifndef _MICROHAL_PERIPHERAL_REGISTER_SYSCFG
#define _MICROHAL_PERIPHERAL_REGISTER_SYSCFG

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F407, STM32F405, STM32F217, STM32F215
// Supported MCU version 2: STM32F411, STM32F401
// Supported MCU version 3: STM32F427
// Supported MCU version 4: STM32F7x9, STM32F765, STM32F7x8, STM32F745, STM32F750, STM32F7x7, STM32F7x6, STM32F429, STM32F469, STM32F446
namespace microhal {
namespace registers {
/**
 *
 */
struct SYSCFG {
    // memory remap register
    union MEMRM {
        union {
            //#if not version 4
            // microhal::Bitfield<uint32_t, 0, 2> MEM_MODE; /*!< MEM_MODE */
            //#else
            microhal::Bitfield<uint32_t, 0, 3> MEM_MODE; /*!< Memory mapping selection */
            microhal::Bitfield<uint32_t, 8, 1> FB_MODE;  /*!< Flash bank mode selection */
            microhal::Bitfield<uint32_t, 10, 2> SWP_FMC; /*!< FMC memory mapping swap */
                                                         //#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MEMRM &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MEMRM &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MEMRM &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MEMRM operator&(uint32_t value) const {
            MEMRM tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MEMRM operator|(uint32_t value) const {
            MEMRM tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MEMRM, AccessType::ReadOnly>;
        friend class VolatileRegister<MEMRM, AccessType::WriteOnly>;
        friend class VolatileRegister<MEMRM, AccessType::ReadWrite>;
    };

    // peripheral mode configuration register
    union PMC {
        union {
            //#if version 2 || version 3 || version 4
            microhal::Bitfield<uint32_t, 16, 1> ADC1DC2;      /*!< ADC1DC2 */
                                                              //#endif
                                                              //#if version 3 || version 4
            microhal::Bitfield<uint32_t, 17, 1> ADC2DC2;      /*!< ADC2DC2 */
            microhal::Bitfield<uint32_t, 18, 1> ADC3DC2;      /*!< ADC3DC2 */
                                                              //#endif
            microhal::Bitfield<uint32_t, 23, 1> MII_RMII_SEL; /*!< Ethernet PHY interface selection */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PMC &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PMC &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PMC &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PMC operator&(uint32_t value) const {
            PMC tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PMC operator|(uint32_t value) const {
            PMC tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PMC, AccessType::ReadOnly>;
        friend class VolatileRegister<PMC, AccessType::WriteOnly>;
        friend class VolatileRegister<PMC, AccessType::ReadWrite>;
    };

    // external interrupt configuration register 1
    union EXTICR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> EXTI0;  /*!< EXTI x configuration (x = 0 to 3) */
            microhal::Bitfield<uint32_t, 4, 4> EXTI1;  /*!< EXTI x configuration (x = 0 to 3) */
            microhal::Bitfield<uint32_t, 8, 4> EXTI2;  /*!< EXTI x configuration (x = 0 to 3) */
            microhal::Bitfield<uint32_t, 12, 4> EXTI3; /*!< EXTI x configuration (x = 0 to 3) */
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

    // Compensation cell control register
    union CMPCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CMP_PD; /*!< Compensation cell power-down */
            microhal::Bitfield<uint32_t, 8, 1> READY;  /*!< READY */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CMPCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CMPCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CMPCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CMPCR operator&(uint32_t value) const {
            CMPCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CMPCR operator|(uint32_t value) const {
            CMPCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CMPCR, AccessType::ReadOnly>;
        friend class VolatileRegister<CMPCR, AccessType::WriteOnly>;
        friend class VolatileRegister<CMPCR, AccessType::ReadWrite>;
    };

    VolatileRegister<MEMRM, AccessType::ReadWrite> memrm;      /*!< memory remap register	Address offset: 0x0 */
    VolatileRegister<PMC, AccessType::ReadWrite> pmc;          /*!< peripheral mode configuration register	Address offset: 0x4 */
    VolatileRegister<EXTICR, AccessType::ReadWrite> exticr[4]; /*!< external interrupt configuration registers Address offset: 0x8 */
    uint32_t reserved0[2];                                     /*!< Reserved register	Address offset: 0x18 */
    VolatileRegister<CMPCR, AccessType::ReadWrite> cmpcr;      /*!< Compensation cell control register	Address offset: 0x20 */
};

#if defined(_MICROHAL_SYSCFG_BASE_ADDRESS)
[[maybe_unused]] inline SYSCFG *const syscfg = reinterpret_cast<SYSCFG *>(_MICROHAL_SYSCFG_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_SYSCFG
