#ifndef _MICROHAL_PERIPHERAL_REGISTER_PWR
#define _MICROHAL_PERIPHERAL_REGISTER_PWR

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100

namespace microhal {
namespace registers {
/**
 *
 */
struct PWR {
    // Power control register (PWR_CR)
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LPDS; /*!< Low Power Deep Sleep */
            microhal::Bitfield<uint32_t, 1, 1> PDDS; /*!< Power Down Deep Sleep */
            microhal::Bitfield<uint32_t, 2, 1> CWUF; /*!< Clear Wake-up Flag */
            microhal::Bitfield<uint32_t, 3, 1> CSBF; /*!< Clear STANDBY Flag */
            microhal::Bitfield<uint32_t, 4, 1> PVDE; /*!< Power Voltage Detector Enable */
            microhal::Bitfield<uint32_t, 5, 3> PLS;  /*!< PVD Level Selection */
            microhal::Bitfield<uint32_t, 8, 1> DBP;  /*!< Disable Backup Domain write protection */
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

    // Power control register (PWR_CR)
    union CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> WUF;  /*!< Wake-Up Flag */
            microhal::Bitfield<uint32_t, 1, 1> SBF;  /*!< STANDBY Flag */
            microhal::Bitfield<uint32_t, 2, 1> PVDO; /*!< PVD Output */
            microhal::Bitfield<uint32_t, 8, 1> EWUP; /*!< Enable WKUP pin */
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

    VolatileRegister<CR, AccessType::ReadWrite> cr;   /*!< Power control register (PWR_CR)	Address offset: 0x0 */
    VolatileRegister<CSR, AccessType::ReadWrite> csr; /*!< Power control register (PWR_CR)	Address offset: 0x4 */
};

#if defined(_MICROHAL_PWR_BASE_ADDRESS)
[[maybe_unused]] static PWR *const pwr = reinterpret_cast<PWR *>(_MICROHAL_PWR_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_PWR
