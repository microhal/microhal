#ifndef _MICROHAL_PERIPHERAL_REGISTER_RTC_V2
#define _MICROHAL_PERIPHERAL_REGISTER_RTC_V2

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32G474xx, STM32G484xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx
// Supported MCU: STM32G041, STM32G030, STM32G031, STM32G041, STM32G030, STM32G031

namespace microhal {
namespace registers {
/**
 *
 */
struct RTC {
    // time register
    union TR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> SU;   /*!< Second units in BCD format */
            microhal::Bitfield<uint32_t, 4, 3> ST;   /*!< Second tens in BCD format */
            microhal::Bitfield<uint32_t, 8, 4> MNU;  /*!< Minute units in BCD format */
            microhal::Bitfield<uint32_t, 12, 3> MNT; /*!< Minute tens in BCD format */
            microhal::Bitfield<uint32_t, 16, 4> HU;  /*!< Hour units in BCD format */
            microhal::Bitfield<uint32_t, 20, 2> HT;  /*!< Hour tens in BCD format */
            microhal::Bitfield<uint32_t, 22, 1> PM;  /*!< AM/PM notation */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TR operator&(uint32_t value) const {
            TR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TR operator|(uint32_t value) const {
            TR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TR, AccessType::ReadOnly>;
        friend class VolatileRegister<TR, AccessType::WriteOnly>;
        friend class VolatileRegister<TR, AccessType::ReadWrite>;
    };

    // date register
    union DR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> DU;   /*!< Date units in BCD format */
            microhal::Bitfield<uint32_t, 4, 2> DT;   /*!< Date tens in BCD format */
            microhal::Bitfield<uint32_t, 8, 4> MU;   /*!< Month units in BCD format */
            microhal::Bitfield<uint32_t, 12, 1> MT;  /*!< Month tens in BCD format */
            microhal::Bitfield<uint32_t, 13, 3> WDU; /*!< Week day units */
            microhal::Bitfield<uint32_t, 16, 4> YU;  /*!< Year units in BCD format */
            microhal::Bitfield<uint32_t, 20, 4> YT;  /*!< Year tens in BCD format */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DR operator&(uint32_t value) const {
            DR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DR operator|(uint32_t value) const {
            DR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DR, AccessType::ReadOnly>;
        friend class VolatileRegister<DR, AccessType::WriteOnly>;
        friend class VolatileRegister<DR, AccessType::ReadWrite>;
    };

    // sub second register
    union SSR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> SS; /*!< Sub second value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SSR operator&(uint32_t value) const {
            SSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SSR operator|(uint32_t value) const {
            SSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SSR, AccessType::ReadOnly>;
        friend class VolatileRegister<SSR, AccessType::WriteOnly>;
        friend class VolatileRegister<SSR, AccessType::ReadWrite>;
    };

    // initialization and status register
    union ICSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ALRAWF;   /*!< Alarm A write flag */
            microhal::Bitfield<uint32_t, 1, 1> ALRBWF;   /*!< Alarm B write flag */
            microhal::Bitfield<uint32_t, 2, 1> WUTWF;    /*!< Wakeup timer write flag */
            microhal::Bitfield<uint32_t, 3, 1> SHPF;     /*!< Shift operation pending */
            microhal::Bitfield<uint32_t, 4, 1> INITS;    /*!< Initialization status flag */
            microhal::Bitfield<uint32_t, 5, 1> RSF;      /*!< Registers synchronization flag */
            microhal::Bitfield<uint32_t, 6, 1> INITF;    /*!< Initialization flag */
            microhal::Bitfield<uint32_t, 7, 1> INIT;     /*!< Initialization mode */
            microhal::Bitfield<uint32_t, 16, 1> RECALPF; /*!< Recalibration pending Flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ICSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ICSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ICSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ICSR operator&(uint32_t value) const {
            ICSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ICSR operator|(uint32_t value) const {
            ICSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ICSR, AccessType::ReadOnly>;
        friend class VolatileRegister<ICSR, AccessType::WriteOnly>;
        friend class VolatileRegister<ICSR, AccessType::ReadWrite>;
    };

    // prescaler register
    union PRER {
        union {
            microhal::Bitfield<uint32_t, 0, 15> PREDIV_S; /*!< Synchronous prescaler factor */
            microhal::Bitfield<uint32_t, 16, 7> PREDIV_A; /*!< Asynchronous prescaler factor */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PRER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PRER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PRER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PRER operator&(uint32_t value) const {
            PRER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PRER operator|(uint32_t value) const {
            PRER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PRER, AccessType::ReadOnly>;
        friend class VolatileRegister<PRER, AccessType::WriteOnly>;
        friend class VolatileRegister<PRER, AccessType::ReadWrite>;
    };

    // wakeup timer register
    union WUTR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> WUT; /*!< Wakeup auto-reload value bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        WUTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        WUTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        WUTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        WUTR operator&(uint32_t value) const {
            WUTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        WUTR operator|(uint32_t value) const {
            WUTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<WUTR, AccessType::ReadOnly>;
        friend class VolatileRegister<WUTR, AccessType::WriteOnly>;
        friend class VolatileRegister<WUTR, AccessType::ReadWrite>;
    };

    // control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 3> WUCKSEL;        /*!< Wakeup clock selection */
            microhal::Bitfield<uint32_t, 3, 1> TSEDGE;         /*!< Time-stamp event active edge */
            microhal::Bitfield<uint32_t, 4, 1> REFCKON;        /*!< Reference clock detection enable (50 or 60 Hz) */
            microhal::Bitfield<uint32_t, 5, 1> BYPSHAD;        /*!< Bypass the shadow registers */
            microhal::Bitfield<uint32_t, 6, 1> FMT;            /*!< Hour format */
            microhal::Bitfield<uint32_t, 8, 1> ALRAE;          /*!< Alarm A enable */
            microhal::Bitfield<uint32_t, 9, 1> ALRBE;          /*!< Alarm B enable */
            microhal::Bitfield<uint32_t, 10, 1> WUTE;          /*!< Wakeup timer enable */
            microhal::Bitfield<uint32_t, 11, 1> TSE;           /*!< Time stamp enable */
            microhal::Bitfield<uint32_t, 12, 1> ALRAIE;        /*!< Alarm A interrupt enable */
            microhal::Bitfield<uint32_t, 13, 1> ALRBIE;        /*!< Alarm B interrupt enable */
            microhal::Bitfield<uint32_t, 14, 1> WUTIE;         /*!< Wakeup timer interrupt enable */
            microhal::Bitfield<uint32_t, 15, 1> TSIE;          /*!< Time-stamp interrupt enable */
            microhal::Bitfield<uint32_t, 16, 1> ADD1H;         /*!< Add 1 hour (summer time change) */
            microhal::Bitfield<uint32_t, 17, 1> SUB1H;         /*!< Subtract 1 hour (winter time change) */
            microhal::Bitfield<uint32_t, 18, 1> BKP;           /*!< Backup */
            microhal::Bitfield<uint32_t, 19, 1> COSEL;         /*!< Calibration output selection */
            microhal::Bitfield<uint32_t, 20, 1> POL;           /*!< Output polarity */
            microhal::Bitfield<uint32_t, 21, 2> OSEL;          /*!< Output selection */
            microhal::Bitfield<uint32_t, 23, 1> COE;           /*!< Calibration output enable */
            microhal::Bitfield<uint32_t, 24, 1> ITSE;          /*!< timestamp on internal event enable */
            microhal::Bitfield<uint32_t, 25, 1> TAMPTS;        /*!< TAMPTS */
            microhal::Bitfield<uint32_t, 26, 1> TAMPOE;        /*!< TAMPOE */
            microhal::Bitfield<uint32_t, 29, 1> TAMPALRM_PU;   /*!< TAMPALRM_PU */
            microhal::Bitfield<uint32_t, 30, 1> TAMPALRM_TYPE; /*!< TAMPALRM_TYPE */
            microhal::Bitfield<uint32_t, 31, 1> OUT2EN;        /*!< OUT2EN */
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

    // write protection register
    union WPR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> KEY; /*!< Write protection key */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        WPR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        WPR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        WPR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        WPR operator&(uint32_t value) const {
            WPR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        WPR operator|(uint32_t value) const {
            WPR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<WPR, AccessType::ReadOnly>;
        friend class VolatileRegister<WPR, AccessType::WriteOnly>;
        friend class VolatileRegister<WPR, AccessType::ReadWrite>;
    };

    // calibration register
    union CALR {
        union {
            microhal::Bitfield<uint32_t, 0, 9> CALM;    /*!< Calibration minus */
            microhal::Bitfield<uint32_t, 13, 1> CALW16; /*!< Use a 16-second calibration cycle period */
            microhal::Bitfield<uint32_t, 14, 1> CALW8;  /*!< Use an 8-second calibration cycle period */
            microhal::Bitfield<uint32_t, 15, 1> CALP;   /*!< Increase frequency of RTC by 488.5 ppm */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CALR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CALR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CALR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CALR operator&(uint32_t value) const {
            CALR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CALR operator|(uint32_t value) const {
            CALR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CALR, AccessType::ReadOnly>;
        friend class VolatileRegister<CALR, AccessType::WriteOnly>;
        friend class VolatileRegister<CALR, AccessType::ReadWrite>;
    };

    // shift control register
    union SHIFTR {
        union {
            microhal::Bitfield<uint32_t, 0, 15> SUBFS; /*!< Subtract a fraction of a second */
            microhal::Bitfield<uint32_t, 31, 1> ADD1S; /*!< Add one second */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SHIFTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SHIFTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SHIFTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SHIFTR operator&(uint32_t value) const {
            SHIFTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SHIFTR operator|(uint32_t value) const {
            SHIFTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SHIFTR, AccessType::ReadOnly>;
        friend class VolatileRegister<SHIFTR, AccessType::WriteOnly>;
        friend class VolatileRegister<SHIFTR, AccessType::ReadWrite>;
    };

    // time stamp time register
    union TSTR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> SU;   /*!< Second units in BCD format */
            microhal::Bitfield<uint32_t, 4, 3> ST;   /*!< Second tens in BCD format */
            microhal::Bitfield<uint32_t, 8, 4> MNU;  /*!< Minute units in BCD format */
            microhal::Bitfield<uint32_t, 12, 3> MNT; /*!< Minute tens in BCD format */
            microhal::Bitfield<uint32_t, 16, 4> HU;  /*!< Hour units in BCD format */
            microhal::Bitfield<uint32_t, 20, 2> HT;  /*!< Hour tens in BCD format */
            microhal::Bitfield<uint32_t, 22, 1> PM;  /*!< AM/PM notation */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TSTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TSTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TSTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TSTR operator&(uint32_t value) const {
            TSTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TSTR operator|(uint32_t value) const {
            TSTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TSTR, AccessType::ReadOnly>;
        friend class VolatileRegister<TSTR, AccessType::WriteOnly>;
        friend class VolatileRegister<TSTR, AccessType::ReadWrite>;
    };

    // time stamp date register
    union TSDR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> DU;   /*!< Date units in BCD format */
            microhal::Bitfield<uint32_t, 4, 2> DT;   /*!< Date tens in BCD format */
            microhal::Bitfield<uint32_t, 8, 4> MU;   /*!< Month units in BCD format */
            microhal::Bitfield<uint32_t, 12, 1> MT;  /*!< Month tens in BCD format */
            microhal::Bitfield<uint32_t, 13, 3> WDU; /*!< Week day units */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TSDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TSDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TSDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TSDR operator&(uint32_t value) const {
            TSDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TSDR operator|(uint32_t value) const {
            TSDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TSDR, AccessType::ReadOnly>;
        friend class VolatileRegister<TSDR, AccessType::WriteOnly>;
        friend class VolatileRegister<TSDR, AccessType::ReadWrite>;
    };

    // timestamp sub second register
    union TSSSR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> SS; /*!< Sub second value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TSSSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TSSSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TSSSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TSSSR operator&(uint32_t value) const {
            TSSSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TSSSR operator|(uint32_t value) const {
            TSSSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TSSSR, AccessType::ReadOnly>;
        friend class VolatileRegister<TSSSR, AccessType::WriteOnly>;
        friend class VolatileRegister<TSSSR, AccessType::ReadWrite>;
    };

    // alarm A register
    union ALRMAR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> SU;     /*!< Second units in BCD format */
            microhal::Bitfield<uint32_t, 4, 3> ST;     /*!< Second tens in BCD format */
            microhal::Bitfield<uint32_t, 7, 1> MSK1;   /*!< Alarm A seconds mask */
            microhal::Bitfield<uint32_t, 8, 4> MNU;    /*!< Minute units in BCD format */
            microhal::Bitfield<uint32_t, 12, 3> MNT;   /*!< Minute tens in BCD format */
            microhal::Bitfield<uint32_t, 15, 1> MSK2;  /*!< Alarm A minutes mask */
            microhal::Bitfield<uint32_t, 16, 4> HU;    /*!< Hour units in BCD format */
            microhal::Bitfield<uint32_t, 20, 2> HT;    /*!< Hour tens in BCD format */
            microhal::Bitfield<uint32_t, 22, 1> PM;    /*!< AM/PM notation */
            microhal::Bitfield<uint32_t, 23, 1> MSK3;  /*!< Alarm A hours mask */
            microhal::Bitfield<uint32_t, 24, 4> DU;    /*!< Date units or day in BCD format */
            microhal::Bitfield<uint32_t, 28, 2> DT;    /*!< Date tens in BCD format */
            microhal::Bitfield<uint32_t, 30, 1> WDSEL; /*!< Week day selection */
            microhal::Bitfield<uint32_t, 31, 1> MSK4;  /*!< Alarm A date mask */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ALRMAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ALRMAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ALRMAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ALRMAR operator&(uint32_t value) const {
            ALRMAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ALRMAR operator|(uint32_t value) const {
            ALRMAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ALRMAR, AccessType::ReadOnly>;
        friend class VolatileRegister<ALRMAR, AccessType::WriteOnly>;
        friend class VolatileRegister<ALRMAR, AccessType::ReadWrite>;
    };

    // alarm A sub second register
    union ALRMASSR {
        union {
            microhal::Bitfield<uint32_t, 0, 15> SS;     /*!< Sub seconds value */
            microhal::Bitfield<uint32_t, 24, 4> MASKSS; /*!< Mask the most-significant bits starting at this bit */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ALRMASSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ALRMASSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ALRMASSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ALRMASSR operator&(uint32_t value) const {
            ALRMASSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ALRMASSR operator|(uint32_t value) const {
            ALRMASSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ALRMASSR, AccessType::ReadOnly>;
        friend class VolatileRegister<ALRMASSR, AccessType::WriteOnly>;
        friend class VolatileRegister<ALRMASSR, AccessType::ReadWrite>;
    };

    // alarm B register
    union ALRMBR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> SU;     /*!< Second units in BCD format */
            microhal::Bitfield<uint32_t, 4, 3> ST;     /*!< Second tens in BCD format */
            microhal::Bitfield<uint32_t, 7, 1> MSK1;   /*!< Alarm B seconds mask */
            microhal::Bitfield<uint32_t, 8, 4> MNU;    /*!< Minute units in BCD format */
            microhal::Bitfield<uint32_t, 12, 3> MNT;   /*!< Minute tens in BCD format */
            microhal::Bitfield<uint32_t, 15, 1> MSK2;  /*!< Alarm B minutes mask */
            microhal::Bitfield<uint32_t, 16, 4> HU;    /*!< Hour units in BCD format */
            microhal::Bitfield<uint32_t, 20, 2> HT;    /*!< Hour tens in BCD format */
            microhal::Bitfield<uint32_t, 22, 1> PM;    /*!< AM/PM notation */
            microhal::Bitfield<uint32_t, 23, 1> MSK3;  /*!< Alarm B hours mask */
            microhal::Bitfield<uint32_t, 24, 4> DU;    /*!< Date units or day in BCD format */
            microhal::Bitfield<uint32_t, 28, 2> DT;    /*!< Date tens in BCD format */
            microhal::Bitfield<uint32_t, 30, 1> WDSEL; /*!< Week day selection */
            microhal::Bitfield<uint32_t, 31, 1> MSK4;  /*!< Alarm B date mask */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ALRMBR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ALRMBR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ALRMBR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ALRMBR operator&(uint32_t value) const {
            ALRMBR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ALRMBR operator|(uint32_t value) const {
            ALRMBR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ALRMBR, AccessType::ReadOnly>;
        friend class VolatileRegister<ALRMBR, AccessType::WriteOnly>;
        friend class VolatileRegister<ALRMBR, AccessType::ReadWrite>;
    };

    // alarm B sub second register
    union ALRMBSSR {
        union {
            microhal::Bitfield<uint32_t, 0, 15> SS;     /*!< Sub seconds value */
            microhal::Bitfield<uint32_t, 24, 4> MASKSS; /*!< Mask the most-significant bits starting at this bit */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ALRMBSSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ALRMBSSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ALRMBSSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ALRMBSSR operator&(uint32_t value) const {
            ALRMBSSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ALRMBSSR operator|(uint32_t value) const {
            ALRMBSSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ALRMBSSR, AccessType::ReadOnly>;
        friend class VolatileRegister<ALRMBSSR, AccessType::WriteOnly>;
        friend class VolatileRegister<ALRMBSSR, AccessType::ReadWrite>;
    };

    // status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ALRAF; /*!< ALRAF */
            microhal::Bitfield<uint32_t, 1, 1> ALRBF; /*!< ALRBF */
            microhal::Bitfield<uint32_t, 2, 1> WUTF;  /*!< WUTF */
            microhal::Bitfield<uint32_t, 3, 1> TSF;   /*!< TSF */
            microhal::Bitfield<uint32_t, 4, 1> TSOVF; /*!< TSOVF */
            microhal::Bitfield<uint32_t, 5, 1> ITSF;  /*!< ITSF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SR operator&(uint32_t value) const {
            SR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SR operator|(uint32_t value) const {
            SR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SR, AccessType::ReadOnly>;
        friend class VolatileRegister<SR, AccessType::WriteOnly>;
        friend class VolatileRegister<SR, AccessType::ReadWrite>;
    };

    // masked interrupt status register
    union MISR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ALRAMF; /*!< ALRAMF */
            microhal::Bitfield<uint32_t, 1, 1> ALRBMF; /*!< ALRBMF */
            microhal::Bitfield<uint32_t, 2, 1> WUTMF;  /*!< WUTMF */
            microhal::Bitfield<uint32_t, 3, 1> TSMF;   /*!< TSMF */
            microhal::Bitfield<uint32_t, 4, 1> TSOVMF; /*!< TSOVMF */
            microhal::Bitfield<uint32_t, 5, 1> ITSMF;  /*!< ITSMF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MISR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MISR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MISR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MISR operator&(uint32_t value) const {
            MISR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MISR operator|(uint32_t value) const {
            MISR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MISR, AccessType::ReadOnly>;
        friend class VolatileRegister<MISR, AccessType::WriteOnly>;
        friend class VolatileRegister<MISR, AccessType::ReadWrite>;
    };

    // status clear register
    union SCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CALRAF; /*!< CALRAF */
            microhal::Bitfield<uint32_t, 1, 1> CALRBF; /*!< CALRBF */
            microhal::Bitfield<uint32_t, 2, 1> CWUTF;  /*!< CWUTF */
            microhal::Bitfield<uint32_t, 3, 1> CTSF;   /*!< CTSF */
            microhal::Bitfield<uint32_t, 4, 1> CTSOVF; /*!< CTSOVF */
            microhal::Bitfield<uint32_t, 5, 1> CITSF;  /*!< CITSF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SCR operator&(uint32_t value) const {
            SCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SCR operator|(uint32_t value) const {
            SCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SCR, AccessType::ReadOnly>;
        friend class VolatileRegister<SCR, AccessType::WriteOnly>;
        friend class VolatileRegister<SCR, AccessType::ReadWrite>;
    };

    VolatileRegister<TR, AccessType::ReadWrite> tr;             /*!< time register	Address offset: 0x0 */
    VolatileRegister<DR, AccessType::ReadWrite> dr;             /*!< date register	Address offset: 0x4 */
    VolatileRegister<SSR, AccessType::ReadWrite> ssr;           /*!< sub second register	Address offset: 0x8 */
    VolatileRegister<ICSR, AccessType::ReadWrite> icsr;         /*!< initialization and status register	Address offset: 0xC */
    VolatileRegister<PRER, AccessType::ReadWrite> prer;         /*!< prescaler register	Address offset: 0x10 */
    VolatileRegister<WUTR, AccessType::ReadWrite> wutr;         /*!< wakeup timer register	Address offset: 0x14 */
    VolatileRegister<CR, AccessType::ReadWrite> cr;             /*!< control register	Address offset: 0x18 */
    uint32_t reserved0[2];                                      /*!< Reserved register	Address offset: 0x1c */
    VolatileRegister<WPR, AccessType::ReadWrite> wpr;           /*!< write protection register	Address offset: 0x24 */
    VolatileRegister<CALR, AccessType::ReadWrite> calr;         /*!< calibration register	Address offset: 0x28 */
    VolatileRegister<SHIFTR, AccessType::ReadWrite> shiftr;     /*!< shift control register	Address offset: 0x2C */
    VolatileRegister<TSTR, AccessType::ReadWrite> tstr;         /*!< time stamp time register	Address offset: 0x30 */
    VolatileRegister<TSDR, AccessType::ReadWrite> tsdr;         /*!< time stamp date register	Address offset: 0x34 */
    VolatileRegister<TSSSR, AccessType::ReadWrite> tsssr;       /*!< timestamp sub second register	Address offset: 0x38 */
    uint32_t reserved1;                                         /*!< Reserved register	Address offset: 0x3c */
    VolatileRegister<ALRMAR, AccessType::ReadWrite> alrmar;     /*!< alarm A register	Address offset: 0x40 */
    VolatileRegister<ALRMASSR, AccessType::ReadWrite> alrmassr; /*!< alarm A sub second register	Address offset: 0x44 */
    VolatileRegister<ALRMBR, AccessType::ReadWrite> alrmbr;     /*!< alarm B register	Address offset: 0x48 */
    VolatileRegister<ALRMBSSR, AccessType::ReadWrite> alrmbssr; /*!< alarm B sub second register	Address offset: 0x4C */
    VolatileRegister<SR, AccessType::ReadWrite> sr;             /*!< status register	Address offset: 0x50 */
    VolatileRegister<MISR, AccessType::ReadWrite> misr;         /*!< masked interrupt status register	Address offset: 0x54 */
    uint32_t reserved2;                                         /*!< Reserved register	Address offset: 0x58 */
    VolatileRegister<SCR, AccessType::ReadWrite> scr;           /*!< status clear register	Address offset: 0x5C */
};

#if defined(_MICROHAL_RTC_BASE_ADDRESS)
[[maybe_unused]] inline RTC *const rtc = reinterpret_cast<RTC *>(_MICROHAL_RTC_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_RTC_V2
