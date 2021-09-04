#ifndef _MICROHAL_PERIPHERAL_REGISTER_CAN
#define _MICROHAL_PERIPHERAL_REGISTER_CAN

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include <array>
#include "registersBaseAddressDefinitions.h"

#undef CAN

// Supported MCU version 1: STM32F302, STM32F303, STM32F3x4, STM32F301, STM32F3x8, STM32F373, STM32F0x2, STM32F0x8, STM32F0x1, STM32F101
// STM32F7x9, STM32F765, STM32F7x8, STM32F745, STM32F750, STM32F7x7, STM32F7x6, STM32F407, STM32F429, STM32F469, STM32F413, STM32F412,
// STM32F427, STM32F405, STM32F446, STM32F217, STM32F215

// Supported MCU version 2: STM32L4x1, STM32L4x2, STM32F7x3, STM32F7x2, STM32F730

// Supported MCU version 3: STM32F107

// Supported MCU version 4: STM32L4x3, STM32L4x5

// Supported MCU version 5: STM32F102, STM32F103

#if defined(STM32F334x8)
#define STM32F3x4
#endif

#if defined(STM32F302) || defined(STM32F303) || defined(STM32F3x4) || defined(STM32F301) || defined(STM32F3x8) || defined(STM32F373) || \
    defined(STM32F0x2) || defined(STM32F0x8) || defined(STM32F0x1) || defined(STM32F101) || defined(STM32F7x9) || defined(STM32F765) || \
    defined(STM32F7x8) || defined(STM32F745) || defined(STM32F750) || defined(STM32F7x7) || defined(STM32F7x6) || defined(STM32F407) || \
    defined(STM32F429) || defined(STM32F469) || defined(STM32F413) || defined(STM32F412) || defined(STM32F427) || defined(STM32F405) || \
    defined(STM32F446) || defined(STM32F217) || defined(STM32F215)
#define _MICROHAL_REGISTER_STM_CAN_VERSION 1
#endif

#if defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32F7x3) || defined(STM32F7x2) || defined(STM32F730)
#define _MICROHAL_REGISTER_STM_CAN_VERSION 2
#endif

#if defined(_MICROHAL_STM32F1XX_STM32F107xx)
#define _MICROHAL_REGISTER_STM_CAN_VERSION 3
#endif

#if defined(STM32L4x3) || defined(STM32L4x5)
#define _MICROHAL_REGISTER_STM_CAN_VERSION 4
#endif

#if defined(_MICROHAL_STM32F1XX_STM32F102xx) || defined(_MICROHAL_STM32F1XX_STM32F103xx)
#define _MICROHAL_REGISTER_STM_CAN_VERSION 5
#endif

#if _MICROHAL_REGISTER_STM_CAN_VERSION == 1
#define _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
#define _MICROHAL_REGISTERS_STM_CAN_FAxR_HAS_FACT14_TO_FACT27
#define _MICROHAL_REGISTERS_STM_CAN_FFAxR_HAS_FFA14_TO_FFA27
#define _MICROHAL_REGISTERS_STM_CAN_FSxR_HAS_FSC14_TO_FSC27
#define _MICROHAL_REGISTERS_STM_CAN_FMxR_HAS_FMB14_TO_FMT27
#define _MICROHAL_REGISTERS_CAN_STM_HAS_FILTERS
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FMR
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FILTER_BANK14_TO_FILTER_BANK_27
#endif

#if _MICROHAL_REGISTER_STM_CAN_VERSION == 2
#define _MICROHAL_REGISTERS_CAN_STM_HAS_FILTERS
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FMR
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FILTER_BANK14_TO_FILTER_BANK_27
#endif

#if _MICROHAL_REGISTER_STM_CAN_VERSION == 3
#define _MICROHAL_REGISTERS_CAN_STM_HAS_FILTERS
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FMR
#define _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FILTER_BANK14_TO_FILTER_BANK_27
#endif

#if _MICROHAL_REGISTER_STM_CAN_VERSION == 4
#define _MICROHAL_REGISTERS_CAN_STM_HAS_FILTERS
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FILTER_BANK14_TO_FILTER_BANK_27
#endif

#if _MICROHAL_REGISTER_STM_CAN_VERSION == 5
#define _MICROHAL_REGISTERS_STM_CAN_FAxR_HAS_FACT14_TO_FACT27
#define _MICROHAL_REGISTERS_STM_CAN_FFAxR_HAS_FFA14_TO_FFA27
#define _MICROHAL_REGISTERS_STM_CAN_FSxR_HAS_FSC14_TO_FSC27
#define _MICROHAL_REGISTERS_STM_CAN_FMxR_HAS_FMB14_TO_FMT27
#define _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FMR
#define _MICROHAL_REGISTERS_CAN_STM_HAS_FILTERS
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct CAN {
    // master control register
    union MCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> INRQ;   /*!< INRQ */
            microhal::Bitfield<uint32_t, 1, 1> SLEEP;  /*!< SLEEP */
            microhal::Bitfield<uint32_t, 2, 1> TXFP;   /*!< TXFP */
            microhal::Bitfield<uint32_t, 3, 1> RFLM;   /*!< RFLM */
            microhal::Bitfield<uint32_t, 4, 1> NART;   /*!< NART */
            microhal::Bitfield<uint32_t, 5, 1> AWUM;   /*!< AWUM */
            microhal::Bitfield<uint32_t, 6, 1> ABOM;   /*!< ABOM */
            microhal::Bitfield<uint32_t, 7, 1> TTCM;   /*!< TTCM */
            microhal::Bitfield<uint32_t, 15, 1> RESET; /*!< RESET */
            microhal::Bitfield<uint32_t, 16, 1> DBF;   /*!< DBF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MCR operator&(uint32_t value) const {
            MCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MCR operator|(uint32_t value) const {
            MCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MCR, AccessType::ReadWrite>;
    };

    // master status register
    union MSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> INAK;  /*!< INAK */
            microhal::Bitfield<uint32_t, 1, 1> SLAK;  /*!< SLAK */
            microhal::Bitfield<uint32_t, 2, 1> ERRI;  /*!< ERRI */
            microhal::Bitfield<uint32_t, 3, 1> WKUI;  /*!< WKUI */
            microhal::Bitfield<uint32_t, 4, 1> SLAKI; /*!< SLAKI */
            microhal::Bitfield<uint32_t, 8, 1> TXM;   /*!< TXM */
            microhal::Bitfield<uint32_t, 9, 1> RXM;   /*!< RXM */
            microhal::Bitfield<uint32_t, 10, 1> SAMP; /*!< SAMP */
            microhal::Bitfield<uint32_t, 11, 1> RX;   /*!< RX */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MSR operator&(uint32_t value) const {
            MSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MSR operator|(uint32_t value) const {
            MSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MSR, AccessType::ReadOnly>;
        friend class VolatileRegister<MSR, AccessType::WriteOnly>;
        friend class VolatileRegister<MSR, AccessType::ReadWrite>;
    };

    // transmit status register
    union TSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> RQCP0;  /*!< RQCP0 */
            microhal::Bitfield<uint32_t, 1, 1> TXOK0;  /*!< TXOK0 */
            microhal::Bitfield<uint32_t, 2, 1> ALST0;  /*!< ALST0 */
            microhal::Bitfield<uint32_t, 3, 1> TERR0;  /*!< TERR0 */
            microhal::Bitfield<uint32_t, 7, 1> ABRQ0;  /*!< ABRQ0 */
            microhal::Bitfield<uint32_t, 8, 1> RQCP1;  /*!< RQCP1 */
            microhal::Bitfield<uint32_t, 9, 1> TXOK1;  /*!< TXOK1 */
            microhal::Bitfield<uint32_t, 10, 1> ALST1; /*!< ALST1 */
            microhal::Bitfield<uint32_t, 11, 1> TERR1; /*!< TERR1 */
            microhal::Bitfield<uint32_t, 15, 1> ABRQ1; /*!< ABRQ1 */
            microhal::Bitfield<uint32_t, 16, 1> RQCP2; /*!< RQCP2 */
            microhal::Bitfield<uint32_t, 17, 1> TXOK2; /*!< TXOK2 */
            microhal::Bitfield<uint32_t, 18, 1> ALST2; /*!< ALST2 */
            microhal::Bitfield<uint32_t, 19, 1> TERR2; /*!< TERR2 */
            microhal::Bitfield<uint32_t, 23, 1> ABRQ2; /*!< ABRQ2 */
            microhal::Bitfield<uint32_t, 24, 2> CODE;  /*!< CODE */
            microhal::Bitfield<uint32_t, 26, 1> TME0;  /*!< Lowest priority flag for mailbox 0 */
            microhal::Bitfield<uint32_t, 27, 1> TME1;  /*!< Lowest priority flag for mailbox 1 */
            microhal::Bitfield<uint32_t, 28, 1> TME2;  /*!< Lowest priority flag for mailbox 2 */
            microhal::Bitfield<uint32_t, 29, 1> LOW0;  /*!< Lowest priority flag for mailbox 0 */
            microhal::Bitfield<uint32_t, 30, 1> LOW1;  /*!< Lowest priority flag for mailbox 1 */
            microhal::Bitfield<uint32_t, 31, 1> LOW2;  /*!< Lowest priority flag for mailbox 2 */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TSR operator&(uint32_t value) const {
            TSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TSR operator|(uint32_t value) const {
            TSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TSR, AccessType::ReadOnly>;
        friend class VolatileRegister<TSR, AccessType::WriteOnly>;
        friend class VolatileRegister<TSR, AccessType::ReadWrite>;
    };

    // receive FIFO x register
    union RFxR {
        union {
            microhal::Bitfield<uint32_t, 0, 2> FMP; /*!< FIFO message pending. These bits indicate how many messages are pending in the receive FIFO.
                                                       FMP is increased each time the hardware stores a new message in to the FIF. FMP is decreased
                                                       each time the software releases the output mailbox by setting the RFOM bit */
            microhal::Bitfield<uint32_t, 3, 1>
                FULL; /*!< FIFO full. Set by hardware when three messages are stored in the FIF. This bit is cleared by software. */
            microhal::Bitfield<uint32_t, 4, 1> FOVR; /*!< FIFO overrun. This bit is set by hardware when a new message has been received and passed
                                                        the filter while the FIF was full. This bit is cleared by software.   */
            microhal::Bitfield<uint32_t, 5, 1> RFOM; /*!< Release FIFO output mailbox. Set by software to release the output mailbox of the FIFO. The
                         output mailbox can only be released when at least one message is pending in the FIFO. Setting this bit when the FIFO is empty
                         has no effect. If at least two messages are pending in the FIFO, the software has to release the output mailbox to access the
                         next message.Cleared by hardware when the output mailbox has been released. */
        };

        uint32_t mesagesCount() const { return FMP; }
        void releaseMessage() { RFOM = 1; }
        bool isFull() const { return FULL; }

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RFxR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RFxR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RFxR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RFxR operator&(uint32_t value) const {
            RFxR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RFxR operator|(uint32_t value) const {
            RFxR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RFxR, AccessType::ReadOnly>;
        friend class VolatileRegister<RFxR, AccessType::WriteOnly>;
        friend class VolatileRegister<RFxR, AccessType::ReadWrite>;
    };

    // interrupt enable register
    union IER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TMEIE;  /*!< Transmit mailbox empty interrupt enable */
            microhal::Bitfield<uint32_t, 1, 1> FMPIE0; /*!< FIFO 0 message pending interrupt enable */
            microhal::Bitfield<uint32_t, 2, 1> FFIE0;  /*!< FIFO 0 full interrupt enable */
            microhal::Bitfield<uint32_t, 3, 1> FOVIE0; /*!< FIFO 0 overrun interrupt enable */
            microhal::Bitfield<uint32_t, 4, 1> FMPIE1; /*!< FIFO 1 message pending interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> FFIE1;  /*!< FIFO 1 full interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> FOVIE1; /*!< FIFO 1 overrun interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> EWGIE;  /*!< Error warning interrupt enable */
            microhal::Bitfield<uint32_t, 9, 1> EPVIE;  /*!< Error passive interrupt enable */
            microhal::Bitfield<uint32_t, 10, 1> BOFIE; /*!< Bus-off interrupt enable */
            microhal::Bitfield<uint32_t, 11, 1> LECIE; /*!< Last error code interrupt enable */
            microhal::Bitfield<uint32_t, 15, 1> ERRIE; /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 16, 1> WKUIE; /*!< Wakeup interrupt enable */
            microhal::Bitfield<uint32_t, 17, 1> SLKIE; /*!< Sleep interrupt enable */
        };

        enum class Interrupt : uint32_t {
            Sleep = 1 << 17,
            Wakeup = 1 << 16,
            Error = 1 << 15,
            LastErrorCode = 1 << 11,
            BussOff = 1 << 10,
            ErrorPasive = 1 << 9,
            ErrorWarning = 1 << 8,
            FIFO1_Overrun = 1 << 6,
            FIFO1_Full = 1 << 5,
            FIFO1_MessagePending = 1 << 4,
            FIFO0_Overrun = 1 << 3,
            FIFO0_Full = 1 << 2,
            FIFO0_MessagePending = 1 << 1,
            TransmitMailboxEmpty = 1 << 0
        };
        void enableInterrupt(Interrupt interrupt) { raw |= static_cast<uint32_t>(interrupt); }
        void disableInterrupt(Interrupt interrupt) { raw &= ~static_cast<uint32_t>(interrupt); }

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IER operator&(uint32_t value) const {
            IER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IER operator|(uint32_t value) const {
            IER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IER, AccessType::ReadOnly>;
        friend class VolatileRegister<IER, AccessType::WriteOnly>;
        friend class VolatileRegister<IER, AccessType::ReadWrite>;
    };

    // error status register
    union ESR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EWGF; /*!< EWGF */
            microhal::Bitfield<uint32_t, 1, 1> EPVF; /*!< EPVF */
            microhal::Bitfield<uint32_t, 2, 1> BOFF; /*!< BOFF */
            microhal::Bitfield<uint32_t, 4, 3> LEC;  /*!< LEC */
            microhal::Bitfield<uint32_t, 16, 8> TEC; /*!< TEC */
            microhal::Bitfield<uint32_t, 24, 8> REC; /*!< REC */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ESR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ESR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ESR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ESR operator&(uint32_t value) const {
            ESR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ESR operator|(uint32_t value) const {
            ESR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ESR, AccessType::ReadOnly>;
        friend class VolatileRegister<ESR, AccessType::WriteOnly>;
        friend class VolatileRegister<ESR, AccessType::ReadWrite>;
    };

    // bit timing register
    union BTR {
        union {
            microhal::Bitfield<uint32_t, 0, 10> BRP;  /*!< BRP */
            microhal::Bitfield<uint32_t, 16, 4> TS1;  /*!< TS1 */
            microhal::Bitfield<uint32_t, 20, 3> TS2;  /*!< TS2 */
            microhal::Bitfield<uint32_t, 24, 2> SJW;  /*!< SJW */
            microhal::Bitfield<uint32_t, 30, 1> LBKM; /*!< LBKM */
            microhal::Bitfield<uint32_t, 31, 1> SILM; /*!< SILM */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        BTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        BTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        BTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        BTR operator&(uint32_t value) const {
            BTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        BTR operator|(uint32_t value) const {
            BTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<BTR, AccessType::ReadOnly>;
        friend class VolatileRegister<BTR, AccessType::WriteOnly>;
        friend class VolatileRegister<BTR, AccessType::ReadWrite>;
    };

    struct TxMailBox {
        // TX mailbox identifier register
        union TIxR {
            union {
                microhal::Bitfield<uint32_t, 0, 1> TXRQ;   /*!< TXRQ */
                microhal::Bitfield<uint32_t, 1, 1> RTR;    /*!< RTR */
                microhal::Bitfield<uint32_t, 2, 1> IDE;    /*!< IDE */
                microhal::Bitfield<uint32_t, 3, 29> EXID;  /*!< EXID */
                microhal::Bitfield<uint32_t, 21, 11> STID; /*!< STID */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            TIxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            TIxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            TIxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            TIxR operator&(uint32_t value) const {
                TIxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            TIxR operator|(uint32_t value) const {
                TIxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<TIxR, AccessType::ReadOnly>;
            friend class VolatileRegister<TIxR, AccessType::WriteOnly>;
            friend class VolatileRegister<TIxR, AccessType::ReadWrite>;
        };

        // mailbox data length control and time stamp register
        union TDTxR {
            union {
                microhal::Bitfield<uint32_t, 0, 4> DLC;    /*!< DLC */
                microhal::Bitfield<uint32_t, 8, 1> TGT;    /*!< TGT */
                microhal::Bitfield<uint32_t, 16, 16> TIME; /*!< TIME */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            TDTxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            TDTxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            TDTxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            TDTxR operator&(uint32_t value) const {
                TDTxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            TDTxR operator|(uint32_t value) const {
                TDTxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<TDTxR, AccessType::ReadOnly>;
            friend class VolatileRegister<TDTxR, AccessType::WriteOnly>;
            friend class VolatileRegister<TDTxR, AccessType::ReadWrite>;
        };

        // mailbox data low register
        union TDLxR {
            union {
                microhal::Bitfield<uint32_t, 0, 8> DATA0;  /*!< DATA0 */
                microhal::Bitfield<uint32_t, 8, 8> DATA1;  /*!< DATA1 */
                microhal::Bitfield<uint32_t, 16, 8> DATA2; /*!< DATA2 */
                microhal::Bitfield<uint32_t, 24, 8> DATA3; /*!< DATA3 */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            TDLxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            TDLxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            TDLxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            TDLxR operator&(uint32_t value) const {
                TDLxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            TDLxR operator|(uint32_t value) const {
                TDLxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<TDLxR, AccessType::ReadOnly>;
            friend class VolatileRegister<TDLxR, AccessType::WriteOnly>;
            friend class VolatileRegister<TDLxR, AccessType::ReadWrite>;
        };

        // mailbox data high register
        union TDHxR {
            union {
                microhal::Bitfield<uint32_t, 0, 8> DATA4;  /*!< DATA4 */
                microhal::Bitfield<uint32_t, 8, 8> DATA5;  /*!< DATA5 */
                microhal::Bitfield<uint32_t, 16, 8> DATA6; /*!< DATA6 */
                microhal::Bitfield<uint32_t, 24, 8> DATA7; /*!< DATA7 */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            TDHxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            TDHxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            TDHxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            TDHxR operator&(uint32_t value) const {
                TDHxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            TDHxR operator|(uint32_t value) const {
                TDHxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<TDHxR, AccessType::ReadOnly>;
            friend class VolatileRegister<TDHxR, AccessType::WriteOnly>;
            friend class VolatileRegister<TDHxR, AccessType::ReadWrite>;
        };

        VolatileRegister<TIxR, AccessType::ReadWrite> tir;   /*!< TX mailbox identifier register */
        VolatileRegister<TDTxR, AccessType::ReadWrite> tdtr; /*!< mailbox data length control and time stamp register */
        VolatileRegister<TDLxR, AccessType::ReadWrite> tdlr; /*!< mailbox data low register */
        VolatileRegister<TDHxR, AccessType::ReadWrite> tdhr; /*!< mailbox data high register */
    };

    struct FIFOMailBox {
        // receive FIFO mailbox identifier register
        union RIxR {
            union {
                microhal::Bitfield<uint32_t, 1, 1> RTR;    /*!< RTR */
                microhal::Bitfield<uint32_t, 2, 1> IDE;    /*!< IDE */
                microhal::Bitfield<uint32_t, 3, 18> EXID;  /*!< EXID */
                microhal::Bitfield<uint32_t, 21, 11> STID; /*!< STID */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            RIxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            RIxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            RIxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            RIxR operator&(uint32_t value) const {
                RIxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            RIxR operator|(uint32_t value) const {
                RIxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<RIxR, AccessType::ReadOnly>;
            friend class VolatileRegister<RIxR, AccessType::WriteOnly>;
            friend class VolatileRegister<RIxR, AccessType::ReadWrite>;
        };

        // receive FIFO mailbox data length control and time stamp register
        union RDTxR {
            union {
                microhal::Bitfield<uint32_t, 0, 4> DLC;    /*!< DLC */
                microhal::Bitfield<uint32_t, 8, 8> FMI;    /*!< FMI */
                microhal::Bitfield<uint32_t, 16, 16> TIME; /*!< TIME */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            RDTxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            RDTxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            RDTxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            RDTxR operator&(uint32_t value) const {
                RDTxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            RDTxR operator|(uint32_t value) const {
                RDTxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<RDTxR, AccessType::ReadOnly>;
            friend class VolatileRegister<RDTxR, AccessType::WriteOnly>;
            friend class VolatileRegister<RDTxR, AccessType::ReadWrite>;
        };

        // receive FIFO mailbox data low register
        union RDLxR {
            union {
                microhal::Bitfield<uint32_t, 0, 8> DATA0;  /*!< DATA0 */
                microhal::Bitfield<uint32_t, 8, 8> DATA1;  /*!< DATA1 */
                microhal::Bitfield<uint32_t, 16, 8> DATA2; /*!< DATA2 */
                microhal::Bitfield<uint32_t, 24, 8> DATA3; /*!< DATA3 */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            RDLxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            RDLxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            RDLxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            RDLxR operator&(uint32_t value) const {
                RDLxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            RDLxR operator|(uint32_t value) const {
                RDLxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<RDLxR, AccessType::ReadOnly>;
            friend class VolatileRegister<RDLxR, AccessType::WriteOnly>;
            friend class VolatileRegister<RDLxR, AccessType::ReadWrite>;
        };

        // receive FIFO mailbox data high register
        union RDHxR {
            union {
                microhal::Bitfield<uint32_t, 0, 8> DATA4;  /*!< DATA4 */
                microhal::Bitfield<uint32_t, 8, 8> DATA5;  /*!< DATA5 */
                microhal::Bitfield<uint32_t, 16, 8> DATA6; /*!< DATA6 */
                microhal::Bitfield<uint32_t, 24, 8> DATA7; /*!< DATA7 */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            RDHxR &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            RDHxR &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            RDHxR &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            RDHxR operator&(uint32_t value) const {
                RDHxR tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            RDHxR operator|(uint32_t value) const {
                RDHxR tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<RDHxR, AccessType::ReadOnly>;
            friend class VolatileRegister<RDHxR, AccessType::WriteOnly>;
            friend class VolatileRegister<RDHxR, AccessType::ReadWrite>;
        };

        VolatileRegister<RIxR, AccessType::ReadWrite> rir;
        VolatileRegister<RDTxR, AccessType::ReadWrite> rdtr;
        VolatileRegister<RDLxR, AccessType::ReadWrite> rdlr;
        VolatileRegister<RDHxR, AccessType::ReadWrite> rdhr;
    };

    VolatileRegister<MCR, AccessType::ReadWrite> mcr;   /*!< master control register	Address offset: 0x0 */
    VolatileRegister<MSR, AccessType::ReadWrite> msr;   /*!< master status register	Address offset: 0x4 */
    VolatileRegister<TSR, AccessType::ReadWrite> tsr;   /*!< transmit status register	Address offset: 0x8 */
    VolatileRegister<RFxR, AccessType::ReadWrite> rf0r; /*!< receive FIFO 0 register	Address offset: 0xC */
    VolatileRegister<RFxR, AccessType::ReadWrite> rf1r; /*!< receive FIFO 1 register	Address offset: 0x10 */
    VolatileRegister<IER, AccessType::ReadWrite> ier;   /*!< interrupt enable register	Address offset: 0x14 */
    VolatileRegister<ESR, AccessType::ReadWrite> esr;   /*!< error status register	Address offset: 0x18 */
    VolatileRegister<BTR, AccessType::ReadWrite> btr;   /*!< bit timing register	Address offset: 0x1C */
    uint32_t reserved0[88];                             /*!< Reserved register	Address offset: 0x20 */
    TxMailBox txMailBox[3];
    FIFOMailBox fifoMailBox[2];
};

constexpr CAN::IER::Interrupt operator|(CAN::IER::Interrupt a, CAN::IER::Interrupt b) {
    return static_cast<CAN::IER::Interrupt>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

#if defined(_MICROHAL_CAN1_BASE_ADDRESS)
[[maybe_unused]] static CAN *const can1 = reinterpret_cast<CAN *>(_MICROHAL_CAN1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_CAN2_BASE_ADDRESS)
[[maybe_unused]] static CAN *const can2 = reinterpret_cast<CAN *>(_MICROHAL_CAN2_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_CAN
