#ifndef _MICROHAL_PERIPHERAL_REGISTER_RTC
#define _MICROHAL_PERIPHERAL_REGISTER_RTC

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100

namespace microhal {
namespace registers {
/**
 *
 */
struct RTC {
    // RTC Control Register High
    union CRH {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SECIE; /*!< Second interrupt Enable */
            microhal::Bitfield<uint32_t, 1, 1> ALRIE; /*!< Alarm interrupt Enable */
            microhal::Bitfield<uint32_t, 2, 1> OWIE;  /*!< Overflow interrupt Enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CRH &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CRH &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CRH &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CRH operator&(uint32_t value) const {
            CRH tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CRH operator|(uint32_t value) const {
            CRH tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CRH, AccessType::ReadOnly>;
        friend class VolatileRegister<CRH, AccessType::WriteOnly>;
        friend class VolatileRegister<CRH, AccessType::ReadWrite>;
    };

    // RTC Control Register Low
    union CRL {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SECF;  /*!< Second Flag */
            microhal::Bitfield<uint32_t, 1, 1> ALRF;  /*!< Alarm Flag */
            microhal::Bitfield<uint32_t, 2, 1> OWF;   /*!< Overflow Flag */
            microhal::Bitfield<uint32_t, 3, 1> RSF;   /*!< Registers Synchronized Flag */
            microhal::Bitfield<uint32_t, 4, 1> CNF;   /*!< Configuration Flag */
            microhal::Bitfield<uint32_t, 5, 1> RTOFF; /*!< RTC operation OFF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CRL &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CRL &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CRL &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CRL operator&(uint32_t value) const {
            CRL tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CRL operator|(uint32_t value) const {
            CRL tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CRL, AccessType::ReadOnly>;
        friend class VolatileRegister<CRL, AccessType::WriteOnly>;
        friend class VolatileRegister<CRL, AccessType::ReadWrite>;
    };

    // RTC Prescaler Load Register High
    union PRLH {
        union {
            microhal::Bitfield<uint32_t, 0, 4> PRL; /*!< RTC Prescaler Load Register High */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PRLH &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PRLH &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PRLH &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PRLH operator&(uint32_t value) const {
            PRLH tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PRLH operator|(uint32_t value) const {
            PRLH tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PRLH, AccessType::ReadOnly>;
        friend class VolatileRegister<PRLH, AccessType::WriteOnly>;
        friend class VolatileRegister<PRLH, AccessType::ReadWrite>;
    };

    // RTC Prescaler Load Register Low
    union PRLL {
        union {
            microhal::Bitfield<uint32_t, 0, 16> PRL; /*!< RTC Prescaler Divider Register Low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PRLL &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PRLL &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PRLL &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PRLL operator&(uint32_t value) const {
            PRLL tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PRLL operator|(uint32_t value) const {
            PRLL tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PRLL, AccessType::ReadOnly>;
        friend class VolatileRegister<PRLL, AccessType::WriteOnly>;
        friend class VolatileRegister<PRLL, AccessType::ReadWrite>;
    };

    // RTC Prescaler Divider Register High
    union DIVH {
        union {
            microhal::Bitfield<uint32_t, 0, 4> DIV; /*!< RTC prescaler divider register high */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DIVH &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DIVH &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DIVH &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DIVH operator&(uint32_t value) const {
            DIVH tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DIVH operator|(uint32_t value) const {
            DIVH tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DIVH, AccessType::ReadOnly>;
        friend class VolatileRegister<DIVH, AccessType::WriteOnly>;
        friend class VolatileRegister<DIVH, AccessType::ReadWrite>;
    };

    // RTC Prescaler Divider Register Low
    union DIVL {
        union {
            microhal::Bitfield<uint32_t, 0, 16> DIV; /*!< RTC prescaler divider register Low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DIVL &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DIVL &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DIVL &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DIVL operator&(uint32_t value) const {
            DIVL tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DIVL operator|(uint32_t value) const {
            DIVL tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DIVL, AccessType::ReadOnly>;
        friend class VolatileRegister<DIVL, AccessType::WriteOnly>;
        friend class VolatileRegister<DIVL, AccessType::ReadWrite>;
    };

    // RTC Counter Register High
    union CNTH {
        union {
            microhal::Bitfield<uint32_t, 0, 16> CNT; /*!< RTC counter register high */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CNTH &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CNTH &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CNTH &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CNTH operator&(uint32_t value) const {
            CNTH tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CNTH operator|(uint32_t value) const {
            CNTH tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CNTH, AccessType::ReadOnly>;
        friend class VolatileRegister<CNTH, AccessType::WriteOnly>;
        friend class VolatileRegister<CNTH, AccessType::ReadWrite>;
    };

    // RTC Counter Register Low
    union CNTL {
        union {
            microhal::Bitfield<uint32_t, 0, 16> CNT; /*!< RTC counter register Low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CNTL &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CNTL &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CNTL &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CNTL operator&(uint32_t value) const {
            CNTL tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CNTL operator|(uint32_t value) const {
            CNTL tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CNTL, AccessType::ReadOnly>;
        friend class VolatileRegister<CNTL, AccessType::WriteOnly>;
        friend class VolatileRegister<CNTL, AccessType::ReadWrite>;
    };

    // RTC Alarm Register High
    union ALRH {
        union {
            microhal::Bitfield<uint32_t, 0, 16> ALR; /*!< RTC alarm register high */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ALRH &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ALRH &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ALRH &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ALRH operator&(uint32_t value) const {
            ALRH tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ALRH operator|(uint32_t value) const {
            ALRH tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ALRH, AccessType::ReadOnly>;
        friend class VolatileRegister<ALRH, AccessType::WriteOnly>;
        friend class VolatileRegister<ALRH, AccessType::ReadWrite>;
    };

    // RTC Alarm Register Low
    union ALRL {
        union {
            microhal::Bitfield<uint32_t, 0, 16> ALR; /*!< RTC alarm register low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ALRL &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ALRL &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ALRL &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ALRL operator&(uint32_t value) const {
            ALRL tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ALRL operator|(uint32_t value) const {
            ALRL tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ALRL, AccessType::ReadOnly>;
        friend class VolatileRegister<ALRL, AccessType::WriteOnly>;
        friend class VolatileRegister<ALRL, AccessType::ReadWrite>;
    };

    VolatileRegister<CRH, AccessType::ReadWrite> crh;   /*!< RTC Control Register High	Address offset: 0x0 */
    VolatileRegister<CRL, AccessType::ReadWrite> crl;   /*!< RTC Control Register Low	Address offset: 0x4 */
    VolatileRegister<PRLH, AccessType::ReadWrite> prlh; /*!< RTC Prescaler Load Register High	Address offset: 0x8 */
    VolatileRegister<PRLL, AccessType::ReadWrite> prll; /*!< RTC Prescaler Load Register Low	Address offset: 0xC */
    VolatileRegister<DIVH, AccessType::ReadWrite> divh; /*!< RTC Prescaler Divider Register High	Address offset: 0x10 */
    VolatileRegister<DIVL, AccessType::ReadWrite> divl; /*!< RTC Prescaler Divider Register Low	Address offset: 0x14 */
    VolatileRegister<CNTH, AccessType::ReadWrite> cnth; /*!< RTC Counter Register High	Address offset: 0x18 */
    VolatileRegister<CNTL, AccessType::ReadWrite> cntl; /*!< RTC Counter Register Low	Address offset: 0x1C */
    VolatileRegister<ALRH, AccessType::ReadWrite> alrh; /*!< RTC Alarm Register High	Address offset: 0x20 */
    VolatileRegister<ALRL, AccessType::ReadWrite> alrl; /*!< RTC Alarm Register Low	Address offset: 0x24 */
};

#if defined(_MICROHAL_RTC_BASE_ADDRESS)
[[maybe_unused]] static RTC *const rtc = reinterpret_cast<RTC *>(_MICROHAL_RTC_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_RTC
