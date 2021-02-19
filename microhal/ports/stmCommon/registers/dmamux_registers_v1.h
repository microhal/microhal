#ifndef _MICROHAL_PERIPHERAL_REGISTER_DMAMUX
#define _MICROHAL_PERIPHERAL_REGISTER_DMAMUX

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU: STM32G071, STM32G070, STM32G081, STM32G071, STM32G070, STM32G081
namespace microhal {
namespace registers {
/**
 *
 */
struct DMAMUX {
    // DMAMux - DMA request line multiplexer channel x control register
    union CxCR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> DMAREQ_ID; /*!< Input DMA request line selected */
            microhal::Bitfield<uint32_t, 8, 1> SOIE;      /*!< Interrupt enable at synchronization event overrun */
            microhal::Bitfield<uint32_t, 9, 1> EGE;       /*!< Event generation enable/disable */
            microhal::Bitfield<uint32_t, 16, 1> SE;       /*!< Synchronous operating mode enable/disable */
            microhal::Bitfield<uint32_t, 17, 2>
                SPOL; /*!< Synchronization event type selector Defines the synchronization event on the selected synchronization input: */
            microhal::Bitfield<uint32_t, 19, 5>
                NBREQ; /*!< Number of DMA requests to forward Defines the number of DMA requests forwarded before output event is generated. In
                          synchronous mode, it also defines the number of DMA requests to forward after a synchronization event, then stop forwarding.
                          The actual number of DMA requests forwarded is NBREQ+1. Note: This field can only be written when both SE and EGE bits are
                          reset. */
            microhal::Bitfield<uint32_t, 24, 5> SYNC_ID; /*!< Synchronization input selected */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CxCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CxCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CxCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CxCR operator&(uint32_t value) const {
            CxCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CxCR operator|(uint32_t value) const {
            CxCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CxCR, AccessType::ReadOnly>;
        friend class VolatileRegister<CxCR, AccessType::WriteOnly>;
        friend class VolatileRegister<CxCR, AccessType::ReadWrite>;
    };

    // DMAMux - DMA request generator channel x control register
    union DMAMUX_RGxCR {
        union {
            microhal::Bitfield<uint32_t, 0, 5> SIG_ID; /*!< DMA request trigger input selected */
            microhal::Bitfield<uint32_t, 8, 1> OIE;    /*!< Interrupt enable at trigger event overrun */
            microhal::Bitfield<uint32_t, 16, 1> GE;    /*!< DMA request generator channel enable/disable */
            microhal::Bitfield<uint32_t, 17, 2>
                GPOL; /*!< DMA request generator trigger event type selection Defines the trigger event on the selected DMA request trigger input */
            microhal::Bitfield<uint32_t, 19, 5> GNBREQ; /*!< Number of DMA requests to generate Defines the number of DMA requests generated after a
                                                           trigger event, then stop generating. The actual number of generated DMA requests is
                                                           GNBREQ+1. Note: This field can only be written when GE bit is reset. */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_RGxCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_RGxCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_RGxCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_RGxCR operator&(uint32_t value) const {
            DMAMUX_RGxCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_RGxCR operator|(uint32_t value) const {
            DMAMUX_RGxCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_RGxCR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_RGxCR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_RGxCR, AccessType::ReadWrite>;
    };

    // DMAMux - DMA request generator status register
    union DMAMUX_RGSR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> OF; /*!< Trigger event overrun flag The flag is set when a trigger event occurs on DMA request
                                                      generator channel x, while the DMA request generator counter value is lower than GNBREQ. The
                                                      flag is cleared by writing 1 to the corresponding COFx bit in DMAMUX_RGCFR register. */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_RGSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_RGSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_RGSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_RGSR operator&(uint32_t value) const {
            DMAMUX_RGSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_RGSR operator|(uint32_t value) const {
            DMAMUX_RGSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_RGSR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_RGSR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_RGSR, AccessType::ReadWrite>;
    };

    // DMAMux - DMA request generator clear flag register
    union DMAMUX_RGCFR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> COF; /*!< Clear trigger event overrun flag Upon setting, this bit clears the corresponding overrun flag
                                                       OFx in the DMAMUX_RGCSR register. */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_RGCFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_RGCFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_RGCFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_RGCFR operator&(uint32_t value) const {
            DMAMUX_RGCFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_RGCFR operator|(uint32_t value) const {
            DMAMUX_RGCFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_RGCFR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_RGCFR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_RGCFR, AccessType::ReadWrite>;
    };

    // DMAMUX request line multiplexer interrupt channel status register
    union DMAMUX_CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 7> SOF; /*!< Synchronization overrun event flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_CSR operator&(uint32_t value) const {
            DMAMUX_CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_CSR operator|(uint32_t value) const {
            DMAMUX_CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_CSR, AccessType::ReadWrite>;
    };

    // DMAMUX request line multiplexer interrupt clear flag register
    union DMAMUX_CFR {
        union {
            microhal::Bitfield<uint32_t, 0, 7> CSOF; /*!< Clear synchronization overrun event flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_CFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_CFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_CFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_CFR operator&(uint32_t value) const {
            DMAMUX_CFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_CFR operator|(uint32_t value) const {
            DMAMUX_CFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_CFR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_CFR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_CFR, AccessType::ReadWrite>;
    };

    // DMAMUX size identification register
    union DMAMUX_SIDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> SID; /*!< Size identification */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_SIDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_SIDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_SIDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_SIDR operator&(uint32_t value) const {
            DMAMUX_SIDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_SIDR operator|(uint32_t value) const {
            DMAMUX_SIDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_SIDR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_SIDR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_SIDR, AccessType::ReadWrite>;
    };

    // DMAMUX IP identification register
    union DMAMUX_IPIDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> ID; /*!< IP identification */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_IPIDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_IPIDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_IPIDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_IPIDR operator&(uint32_t value) const {
            DMAMUX_IPIDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_IPIDR operator|(uint32_t value) const {
            DMAMUX_IPIDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_IPIDR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_IPIDR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_IPIDR, AccessType::ReadWrite>;
    };

    // DMAMUX version register
    union DMAMUX_VERR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> MINREV; /*!< Minor IP revision */
            microhal::Bitfield<uint32_t, 4, 4> MAJREV; /*!< Major IP revision */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_VERR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_VERR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_VERR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_VERR operator&(uint32_t value) const {
            DMAMUX_VERR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_VERR operator|(uint32_t value) const {
            DMAMUX_VERR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_VERR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_VERR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_VERR, AccessType::ReadWrite>;
    };

    // DMAMUX hardware configuration 1 register
    union DMAMUX_HWCFGR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 8> NUM_DMA_STREAMS;    /*!< number of DMA request line multiplexer (output) channels */
            microhal::Bitfield<uint32_t, 8, 8> NUM_DMA_PERIPH_REQ; /*!< number of DMA request lines from peripherals */
            microhal::Bitfield<uint32_t, 16, 8> NUM_DMA_TRIG;      /*!< number of synchronization inputs */
            microhal::Bitfield<uint32_t, 24, 8> NUM_DMA_REQGEN;    /*!< number of DMA request generator channels */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_HWCFGR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_HWCFGR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_HWCFGR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_HWCFGR1 operator&(uint32_t value) const {
            DMAMUX_HWCFGR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_HWCFGR1 operator|(uint32_t value) const {
            DMAMUX_HWCFGR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_HWCFGR1, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_HWCFGR1, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_HWCFGR1, AccessType::ReadWrite>;
    };

    // DMAMUX hardware configuration 2 register
    union DMAMUX_HWCFGR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 8> NUM_DMA_EXT_REQ; /*!< Number of DMA request trigger inputs */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMUX_HWCFGR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMUX_HWCFGR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMUX_HWCFGR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMUX_HWCFGR2 operator&(uint32_t value) const {
            DMAMUX_HWCFGR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMUX_HWCFGR2 operator|(uint32_t value) const {
            DMAMUX_HWCFGR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMUX_HWCFGR2, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMUX_HWCFGR2, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMUX_HWCFGR2, AccessType::ReadWrite>;
    };

    VolatileRegister<CxCR, AccessType::ReadWrite>
        dmamux_c0cr; /*!< DMAMux - DMA request line multiplexer channel x control register	Address offset: 0x0 */
    VolatileRegister<CxCR, AccessType::ReadWrite>
        dmamux_c1cr; /*!< DMAMux - DMA request line multiplexer channel x control register	Address offset: 0x4 */
    VolatileRegister<CxCR, AccessType::ReadWrite>
        dmamux_c2cr; /*!< DMAMux - DMA request line multiplexer channel x control register	Address offset: 0x8 */
    VolatileRegister<CxCR, AccessType::ReadWrite>
        dmamux_c3cr; /*!< DMAMux - DMA request line multiplexer channel x control register	Address offset: 0xC */
    VolatileRegister<CxCR, AccessType::ReadWrite>
        dmamux_c4cr; /*!< DMAMux - DMA request line multiplexer channel x control register	Address offset: 0x10 */
    VolatileRegister<CxCR, AccessType::ReadWrite>
        dmamux_c5cr; /*!< DMAMux - DMA request line multiplexer channel x control register	Address offset: 0x14 */
    VolatileRegister<CxCR, AccessType::ReadWrite>
        dmamux_c6cr;        /*!< DMAMux - DMA request line multiplexer channel x control register	Address offset: 0x18 */
    uint32_t reserved0[57]; /*!< Reserved register	Address offset: 0x1c */
    VolatileRegister<DMAMUX_CSR, AccessType::ReadWrite>
        dmamux_csr; /*!< DMAMUX request line multiplexer interrupt channel status register	Address offset: 0x80 */
    VolatileRegister<DMAMUX_CFR, AccessType::ReadWrite>
        dmamux_cfr;          /*!< DMAMUX request line multiplexer interrupt clear flag register	Address offset: 0x84 */
    uint32_t reserved3[221]; /*!< Reserved register	Address offset: 0x88 */
    VolatileRegister<DMAMUX_RGxCR, AccessType::ReadWrite>
        dmamux_rg0cr; /*!< DMAMux - DMA request generator channel x control register	Address offset: 0x100 */
    VolatileRegister<DMAMUX_RGxCR, AccessType::ReadWrite>
        dmamux_rg1cr; /*!< DMAMux - DMA request generator channel x control register	Address offset: 0x104 */
    VolatileRegister<DMAMUX_RGxCR, AccessType::ReadWrite>
        dmamux_rg2cr; /*!< DMAMux - DMA request generator channel x control register	Address offset: 0x108 */
    VolatileRegister<DMAMUX_RGxCR, AccessType::ReadWrite>
        dmamux_rg3cr;       /*!< DMAMux - DMA request generator channel x control register	Address offset: 0x10C */
    uint32_t reserved1[12]; /*!< Reserved register	Address offset: 0x110 */
    VolatileRegister<DMAMUX_RGSR, AccessType::ReadWrite> dmamux_rgsr; /*!< DMAMux - DMA request generator status register	Address offset: 0x140
                                                                       */
    VolatileRegister<DMAMUX_RGCFR, AccessType::ReadWrite>
        dmamux_rgcfr;       /*!< DMAMux - DMA request generator clear flag register	Address offset: 0x144 */
    uint32_t reserved2[50]; /*!< Reserved register	Address offset: 0x148 */
    VolatileRegister<DMAMUX_HWCFGR2, AccessType::ReadWrite> dmamux_hwcfgr2; /*!< DMAMUX hardware configuration 2 register	Address offset: 0x3EC
                                                                             */
    VolatileRegister<DMAMUX_HWCFGR1, AccessType::ReadWrite> dmamux_hwcfgr1; /*!< DMAMUX hardware configuration 1 register	Address offset: 0x3F0
                                                                             */
    VolatileRegister<DMAMUX_VERR, AccessType::ReadWrite> dmamux_verr;       /*!< DMAMUX version register	Address offset: 0x3F4 */
    VolatileRegister<DMAMUX_IPIDR, AccessType::ReadWrite> dmamux_ipidr;     /*!< DMAMUX IP identification register	Address offset: 0x3F8 */
    VolatileRegister<DMAMUX_SIDR, AccessType::ReadWrite> dmamux_sidr;       /*!< DMAMUX size identification register	Address offset: 0x3FC */
};

#if defined(_MICROHAL_DMAMUX_BASE_ADDRESS)
[[maybe_unused]] inline DMAMUX *const dmamux1 = reinterpret_cast<DMAMUX *>(_MICROHAL_DMAMUX_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_DMAMUX
