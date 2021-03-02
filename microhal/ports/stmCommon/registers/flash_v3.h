#ifndef _MICROHAL_PERIPHERAL_REGISTER_FLASH
#define _MICROHAL_PERIPHERAL_REGISTER_FLASH

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32G071, STM32G041, STM32G030, STM32G070, STM32G081, STM32G031, STM32G071, STM32G041, STM32G030, STM32G070, STM32G081, STM32G031
namespace microhal {
namespace registers {
/**
 *
 */
struct FLASH {
    // Access control register
    union ACR {
        union {
            microhal::Bitfield<uint32_t, 0, 3> LATENCY;   /*!< Latency */
            microhal::Bitfield<uint32_t, 8, 1> PRFTEN;    /*!< Prefetch enable */
            microhal::Bitfield<uint32_t, 9, 1> ICEN;      /*!< Instruction cache enable */
            microhal::Bitfield<uint32_t, 11, 1> ICRST;    /*!< Instruction cache reset */
            microhal::Bitfield<uint32_t, 16, 1> EMPTY;    /*!< Flash User area empty */
            microhal::Bitfield<uint32_t, 18, 1> DBG_SWEN; /*!< Debug access software enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ACR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ACR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ACR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ACR operator&(uint32_t value) const {
            ACR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ACR operator|(uint32_t value) const {
            ACR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ACR, AccessType::ReadOnly>;
        friend class VolatileRegister<ACR, AccessType::WriteOnly>;
        friend class VolatileRegister<ACR, AccessType::ReadWrite>;
    };

    // Flash key register
    union KEYR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> KEY; /*!< KEYR */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        KEYR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        KEYR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        KEYR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        KEYR operator&(uint32_t value) const {
            KEYR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        KEYR operator|(uint32_t value) const {
            KEYR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<KEYR, AccessType::ReadOnly>;
        friend class VolatileRegister<KEYR, AccessType::WriteOnly>;
        friend class VolatileRegister<KEYR, AccessType::ReadWrite>;
    };

    // Option byte key register
    union OPTKEYR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> OPTKEY; /*!< Option byte key */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OPTKEYR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OPTKEYR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OPTKEYR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OPTKEYR operator&(uint32_t value) const {
            OPTKEYR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OPTKEYR operator|(uint32_t value) const {
            OPTKEYR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OPTKEYR, AccessType::ReadOnly>;
        friend class VolatileRegister<OPTKEYR, AccessType::WriteOnly>;
        friend class VolatileRegister<OPTKEYR, AccessType::ReadWrite>;
    };

    // Status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EOP;      /*!< End of operation */
            microhal::Bitfield<uint32_t, 1, 1> OPERR;    /*!< Operation error */
            microhal::Bitfield<uint32_t, 3, 1> PROGERR;  /*!< Programming error */
            microhal::Bitfield<uint32_t, 4, 1> WRPERR;   /*!< Write protected error */
            microhal::Bitfield<uint32_t, 5, 1> PGAERR;   /*!< Programming alignment error */
            microhal::Bitfield<uint32_t, 6, 1> SIZERR;   /*!< Size error */
            microhal::Bitfield<uint32_t, 7, 1> PGSERR;   /*!< Programming sequence error */
            microhal::Bitfield<uint32_t, 8, 1> MISERR;   /*!< Fast programming data miss error */
            microhal::Bitfield<uint32_t, 9, 1> FASTERR;  /*!< Fast programming error */
            microhal::Bitfield<uint32_t, 14, 1> RDERR;   /*!< PCROP read error */
            microhal::Bitfield<uint32_t, 15, 1> OPTVERR; /*!< Option and Engineering bits loading validity error */
            microhal::Bitfield<uint32_t, 16, 1> BSY;     /*!< Busy */
            microhal::Bitfield<uint32_t, 18, 1> CFGBSY;  /*!< Programming or erase configuration busy. */
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

    // Flash control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PG;          /*!< Programming */
            microhal::Bitfield<uint32_t, 1, 1> PER;         /*!< Page erase */
            microhal::Bitfield<uint32_t, 2, 1> MER;         /*!< Mass erase */
            microhal::Bitfield<uint32_t, 3, 6> PNB;         /*!< Page number */
            microhal::Bitfield<uint32_t, 16, 1> STRT;       /*!< Start */
            microhal::Bitfield<uint32_t, 17, 1> OPTSTRT;    /*!< Options modification start */
            microhal::Bitfield<uint32_t, 18, 1> FSTPG;      /*!< Fast programming */
            microhal::Bitfield<uint32_t, 24, 1> EOPIE;      /*!< End of operation interrupt enable */
            microhal::Bitfield<uint32_t, 25, 1> ERRIE;      /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 26, 1> RDERRIE;    /*!< PCROP read error interrupt enable */
            microhal::Bitfield<uint32_t, 27, 1> OBL_LAUNCH; /*!< Force the option byte loading */
            microhal::Bitfield<uint32_t, 28, 1> SEC_PROT;   /*!< Securable memory area protection enable */
            microhal::Bitfield<uint32_t, 30, 1> OPTLOCK;    /*!< Options Lock */
            microhal::Bitfield<uint32_t, 31, 1> LOCK;       /*!< FLASH_CR Lock */
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

    // Flash ECC register
    union ECCR {
        union {
            microhal::Bitfield<uint32_t, 0, 14> ADDR_ECC; /*!< ECC fail address */
            microhal::Bitfield<uint32_t, 20, 1> SYSF_ECC; /*!< ECC fail for Corrected ECC Error or Double ECC Error in info block */
            microhal::Bitfield<uint32_t, 24, 1> ECCIE;    /*!< ECC correction interrupt enable */
            microhal::Bitfield<uint32_t, 30, 1> ECCC;     /*!< ECC correction */
            microhal::Bitfield<uint32_t, 31, 1> ECCD;     /*!< ECC detection */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ECCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ECCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ECCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ECCR operator&(uint32_t value) const {
            ECCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ECCR operator|(uint32_t value) const {
            ECCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ECCR, AccessType::ReadOnly>;
        friend class VolatileRegister<ECCR, AccessType::WriteOnly>;
        friend class VolatileRegister<ECCR, AccessType::ReadWrite>;
    };

    // Flash option register
    union OPTR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> RDP;               /*!< Read protection level */
            microhal::Bitfield<uint32_t, 8, 1> BOREN;             /*!< BOR reset Level */
            microhal::Bitfield<uint32_t, 9, 2> BORF_LEV;          /*!< These bits contain the VDD supply level threshold that activates the reset */
            microhal::Bitfield<uint32_t, 11, 2> BORR_LEV;         /*!< These bits contain the VDD supply level threshold that releases the reset. */
            microhal::Bitfield<uint32_t, 13, 1> nRST_STOP;        /*!< nRST_STOP */
            microhal::Bitfield<uint32_t, 14, 1> nRST_STDBY;       /*!< nRST_STDBY */
            microhal::Bitfield<uint32_t, 15, 1> nRSTS_HDW;        /*!< nRSTS_HDW */
            microhal::Bitfield<uint32_t, 16, 1> IDWG_SW;          /*!< Independent watchdog selection */
            microhal::Bitfield<uint32_t, 17, 1> IWDG_STOP;        /*!< Independent watchdog counter freeze in Stop mode */
            microhal::Bitfield<uint32_t, 18, 1> IWDG_STDBY;       /*!< Independent watchdog counter freeze in Standby mode */
            microhal::Bitfield<uint32_t, 19, 1> WWDG_SW;          /*!< Window watchdog selection */
            microhal::Bitfield<uint32_t, 22, 1> RAM_PARITY_CHECK; /*!< SRAM parity check control */
            microhal::Bitfield<uint32_t, 24, 1> nBOOT_SEL;        /*!< nBOOT_SEL */
            microhal::Bitfield<uint32_t, 25, 1> nBOOT1;           /*!< Boot configuration */
            microhal::Bitfield<uint32_t, 26, 1> nBOOT0;           /*!< nBOOT0 option bit */
            microhal::Bitfield<uint32_t, 27, 2> NRST_MODE;        /*!< NRST_MODE */
            microhal::Bitfield<uint32_t, 29, 1> IRHEN;            /*!< Internal reset holder enable bit */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OPTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OPTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OPTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OPTR operator&(uint32_t value) const {
            OPTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OPTR operator|(uint32_t value) const {
            OPTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OPTR, AccessType::ReadOnly>;
        friend class VolatileRegister<OPTR, AccessType::WriteOnly>;
        friend class VolatileRegister<OPTR, AccessType::ReadWrite>;
    };

    // Flash PCROP zone A Start address register
    union PCROP1ASR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> PCROP1A_STRT; /*!< PCROP1A area start offset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PCROP1ASR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PCROP1ASR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PCROP1ASR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PCROP1ASR operator&(uint32_t value) const {
            PCROP1ASR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PCROP1ASR operator|(uint32_t value) const {
            PCROP1ASR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PCROP1ASR, AccessType::ReadOnly>;
        friend class VolatileRegister<PCROP1ASR, AccessType::WriteOnly>;
        friend class VolatileRegister<PCROP1ASR, AccessType::ReadWrite>;
    };

    // Flash PCROP zone A End address register
    union PCROP1AER {
        union {
            microhal::Bitfield<uint32_t, 0, 8> PCROP1A_END; /*!< PCROP1A area end offset */
            microhal::Bitfield<uint32_t, 31, 1> PCROP_RDP;  /*!< PCROP area preserved when RDP level decreased */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PCROP1AER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PCROP1AER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PCROP1AER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PCROP1AER operator&(uint32_t value) const {
            PCROP1AER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PCROP1AER operator|(uint32_t value) const {
            PCROP1AER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PCROP1AER, AccessType::ReadOnly>;
        friend class VolatileRegister<PCROP1AER, AccessType::WriteOnly>;
        friend class VolatileRegister<PCROP1AER, AccessType::ReadWrite>;
    };

    // Flash WRP area A address register
    union WRP1AR {
        union {
            microhal::Bitfield<uint32_t, 0, 6> WRP1A_STRT; /*!< WRP area A start offset */
            microhal::Bitfield<uint32_t, 16, 6> WRP1A_END; /*!< WRP area A end offset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        WRP1AR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        WRP1AR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        WRP1AR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        WRP1AR operator&(uint32_t value) const {
            WRP1AR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        WRP1AR operator|(uint32_t value) const {
            WRP1AR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<WRP1AR, AccessType::ReadOnly>;
        friend class VolatileRegister<WRP1AR, AccessType::WriteOnly>;
        friend class VolatileRegister<WRP1AR, AccessType::ReadWrite>;
    };

    // Flash WRP area B address register
    union WRP1BR {
        union {
            microhal::Bitfield<uint32_t, 0, 6> WRP1B_STRT; /*!< WRP area B start offset */
            microhal::Bitfield<uint32_t, 16, 6> WRP1B_END; /*!< WRP area B end offset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        WRP1BR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        WRP1BR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        WRP1BR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        WRP1BR operator&(uint32_t value) const {
            WRP1BR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        WRP1BR operator|(uint32_t value) const {
            WRP1BR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<WRP1BR, AccessType::ReadOnly>;
        friend class VolatileRegister<WRP1BR, AccessType::WriteOnly>;
        friend class VolatileRegister<WRP1BR, AccessType::ReadWrite>;
    };

    // Flash PCROP zone B Start address register
    union PCROP1BSR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> PCROP1B_STRT; /*!< PCROP1B area start offset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PCROP1BSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PCROP1BSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PCROP1BSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PCROP1BSR operator&(uint32_t value) const {
            PCROP1BSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PCROP1BSR operator|(uint32_t value) const {
            PCROP1BSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PCROP1BSR, AccessType::ReadOnly>;
        friend class VolatileRegister<PCROP1BSR, AccessType::WriteOnly>;
        friend class VolatileRegister<PCROP1BSR, AccessType::ReadWrite>;
    };

    // Flash PCROP zone B End address register
    union PCROP1BER {
        union {
            microhal::Bitfield<uint32_t, 0, 8> PCROP1B_END; /*!< PCROP1B area end offset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PCROP1BER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PCROP1BER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PCROP1BER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PCROP1BER operator&(uint32_t value) const {
            PCROP1BER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PCROP1BER operator|(uint32_t value) const {
            PCROP1BER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PCROP1BER, AccessType::ReadOnly>;
        friend class VolatileRegister<PCROP1BER, AccessType::WriteOnly>;
        friend class VolatileRegister<PCROP1BER, AccessType::ReadWrite>;
    };

    // Flash Security register
    union SECR {
        union {
            microhal::Bitfield<uint32_t, 0, 7> SEC_SIZE;   /*!< Securable memory area size */
            microhal::Bitfield<uint32_t, 16, 1> BOOT_LOCK; /*!< used to force boot from user area */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SECR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SECR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SECR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SECR operator&(uint32_t value) const {
            SECR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SECR operator|(uint32_t value) const {
            SECR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SECR, AccessType::ReadOnly>;
        friend class VolatileRegister<SECR, AccessType::WriteOnly>;
        friend class VolatileRegister<SECR, AccessType::ReadWrite>;
    };

    VolatileRegister<ACR, AccessType::ReadWrite> acr;             /*!< Access control register	Address offset: 0x0 */
    uint32_t reserved0[1];                                        /*!< Reserved register	Address offset: 0x4 */
    VolatileRegister<KEYR, AccessType::ReadWrite> keyr;           /*!< Flash key register	Address offset: 0x8 */
    VolatileRegister<OPTKEYR, AccessType::ReadWrite> optkeyr;     /*!< Option byte key register	Address offset: 0xC */
    VolatileRegister<SR, AccessType::ReadWrite> sr;               /*!< Status register	Address offset: 0x10 */
    VolatileRegister<CR, AccessType::ReadWrite> cr;               /*!< Flash control register	Address offset: 0x14 */
    VolatileRegister<ECCR, AccessType::ReadWrite> eccr;           /*!< Flash ECC register	Address offset: 0x18 */
    uint32_t reserved1[1];                                        /*!< Reserved register	Address offset: 0x1c */
    VolatileRegister<OPTR, AccessType::ReadWrite> optr;           /*!< Flash option register	Address offset: 0x20 */
    VolatileRegister<PCROP1ASR, AccessType::ReadWrite> pcrop1asr; /*!< Flash PCROP zone A Start address register	Address offset: 0x24 */
    VolatileRegister<PCROP1AER, AccessType::ReadWrite> pcrop1aer; /*!< Flash PCROP zone A End address register	Address offset: 0x28 */
    VolatileRegister<WRP1AR, AccessType::ReadWrite> wrp1ar;       /*!< Flash WRP area A address register	Address offset: 0x2C */
    VolatileRegister<WRP1BR, AccessType::ReadWrite> wrp1br;       /*!< Flash WRP area B address register	Address offset: 0x30 */
    VolatileRegister<PCROP1BSR, AccessType::ReadWrite> pcrop1bsr; /*!< Flash PCROP zone B Start address register	Address offset: 0x34 */
    VolatileRegister<PCROP1BER, AccessType::ReadWrite> pcrop1ber; /*!< Flash PCROP zone B End address register	Address offset: 0x38 */
    uint32_t reserved2[17];                                       /*!< Reserved register	Address offset: 0x3c */
    VolatileRegister<SECR, AccessType::ReadWrite> secr;           /*!< Flash Security register	Address offset: 0x80 */
};

#if defined(_MICROHAL_FLASH_BASE_ADDRESS)
[[maybe_unused]] inline FLASH *const flash = reinterpret_cast<FLASH *>(_MICROHAL_FLASH_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_FLASH
