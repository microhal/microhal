#ifndef _MICROHAL_PERIPHERAL_REGISTER_DMA
#define _MICROHAL_PERIPHERAL_REGISTER_DMA

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100, STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0, STM32F302, STM32F303, STM32F3x4,
//                STM32F301, STM32F3x8, STM32F373, STM32L100, STM32L162, STM32L151

namespace microhal {
namespace registers {
/**
 *
 */
struct DMA {
    // DMA interrupt status register (DMA_ISR)
    union ISR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> GIF1;   /*!< Channel 1 Global interrupt flag */
            microhal::Bitfield<uint32_t, 1, 1> TCIF1;  /*!< Channel 1 Transfer Complete flag */
            microhal::Bitfield<uint32_t, 2, 1> HTIF1;  /*!< Channel 1 Half Transfer Complete flag */
            microhal::Bitfield<uint32_t, 3, 1> TEIF1;  /*!< Channel 1 Transfer Error flag */
            microhal::Bitfield<uint32_t, 4, 1> GIF2;   /*!< Channel 2 Global interrupt flag */
            microhal::Bitfield<uint32_t, 5, 1> TCIF2;  /*!< Channel 2 Transfer Complete flag */
            microhal::Bitfield<uint32_t, 6, 1> HTIF2;  /*!< Channel 2 Half Transfer Complete flag */
            microhal::Bitfield<uint32_t, 7, 1> TEIF2;  /*!< Channel 2 Transfer Error flag */
            microhal::Bitfield<uint32_t, 8, 1> GIF3;   /*!< Channel 3 Global interrupt flag */
            microhal::Bitfield<uint32_t, 9, 1> TCIF3;  /*!< Channel 3 Transfer Complete flag */
            microhal::Bitfield<uint32_t, 10, 1> HTIF3; /*!< Channel 3 Half Transfer Complete flag */
            microhal::Bitfield<uint32_t, 11, 1> TEIF3; /*!< Channel 3 Transfer Error flag */
            microhal::Bitfield<uint32_t, 12, 1> GIF4;  /*!< Channel 4 Global interrupt flag */
            microhal::Bitfield<uint32_t, 13, 1> TCIF4; /*!< Channel 4 Transfer Complete flag */
            microhal::Bitfield<uint32_t, 14, 1> HTIF4; /*!< Channel 4 Half Transfer Complete flag */
            microhal::Bitfield<uint32_t, 15, 1> TEIF4; /*!< Channel 4 Transfer Error flag */
            microhal::Bitfield<uint32_t, 16, 1> GIF5;  /*!< Channel 5 Global interrupt flag */
            microhal::Bitfield<uint32_t, 17, 1> TCIF5; /*!< Channel 5 Transfer Complete flag */
            microhal::Bitfield<uint32_t, 18, 1> HTIF5; /*!< Channel 5 Half Transfer Complete flag */
            microhal::Bitfield<uint32_t, 19, 1> TEIF5; /*!< Channel 5 Transfer Error flag */
            microhal::Bitfield<uint32_t, 20, 1> GIF6;  /*!< Channel 6 Global interrupt flag */
            microhal::Bitfield<uint32_t, 21, 1> TCIF6; /*!< Channel 6 Transfer Complete flag */
            microhal::Bitfield<uint32_t, 22, 1> HTIF6; /*!< Channel 6 Half Transfer Complete flag */
            microhal::Bitfield<uint32_t, 23, 1> TEIF6; /*!< Channel 6 Transfer Error flag */
            microhal::Bitfield<uint32_t, 24, 1> GIF7;  /*!< Channel 7 Global interrupt flag */
            microhal::Bitfield<uint32_t, 25, 1> TCIF7; /*!< Channel 7 Transfer Complete flag */
            microhal::Bitfield<uint32_t, 26, 1> HTIF7; /*!< Channel 7 Half Transfer Complete flag */
            microhal::Bitfield<uint32_t, 27, 1> TEIF7; /*!< Channel 7 Transfer Error flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ISR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ISR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ISR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ISR operator&(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ISR operator|(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ISR, AccessType::ReadOnly>;
        friend class VolatileRegister<ISR, AccessType::WriteOnly>;
        friend class VolatileRegister<ISR, AccessType::ReadWrite>;
    };

    // DMA interrupt flag clear register (DMA_IFCR)
    union IFCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CGIF1;   /*!< Channel 1 Global interrupt clear */
            microhal::Bitfield<uint32_t, 1, 1> CTCIF1;  /*!< Channel 1 Transfer Complete clear */
            microhal::Bitfield<uint32_t, 2, 1> CHTIF1;  /*!< Channel 1 Half Transfer clear */
            microhal::Bitfield<uint32_t, 3, 1> CTEIF1;  /*!< Channel 1 Transfer Error clear */
            microhal::Bitfield<uint32_t, 4, 1> CGIF2;   /*!< Channel 2 Global interrupt clear */
            microhal::Bitfield<uint32_t, 5, 1> CTCIF2;  /*!< Channel 2 Transfer Complete clear */
            microhal::Bitfield<uint32_t, 6, 1> CHTIF2;  /*!< Channel 2 Half Transfer clear */
            microhal::Bitfield<uint32_t, 7, 1> CTEIF2;  /*!< Channel 2 Transfer Error clear */
            microhal::Bitfield<uint32_t, 8, 1> CGIF3;   /*!< Channel 3 Global interrupt clear */
            microhal::Bitfield<uint32_t, 9, 1> CTCIF3;  /*!< Channel 3 Transfer Complete clear */
            microhal::Bitfield<uint32_t, 10, 1> CHTIF3; /*!< Channel 3 Half Transfer clear */
            microhal::Bitfield<uint32_t, 11, 1> CTEIF3; /*!< Channel 3 Transfer Error clear */
            microhal::Bitfield<uint32_t, 12, 1> CGIF4;  /*!< Channel 4 Global interrupt clear */
            microhal::Bitfield<uint32_t, 13, 1> CTCIF4; /*!< Channel 4 Transfer Complete clear */
            microhal::Bitfield<uint32_t, 14, 1> CHTIF4; /*!< Channel 4 Half Transfer clear */
            microhal::Bitfield<uint32_t, 15, 1> CTEIF4; /*!< Channel 4 Transfer Error clear */
            microhal::Bitfield<uint32_t, 16, 1> CGIF5;  /*!< Channel 5 Global interrupt clear */
            microhal::Bitfield<uint32_t, 17, 1> CTCIF5; /*!< Channel 5 Transfer Complete clear */
            microhal::Bitfield<uint32_t, 18, 1> CHTIF5; /*!< Channel 5 Half Transfer clear */
            microhal::Bitfield<uint32_t, 19, 1> CTEIF5; /*!< Channel 5 Transfer Error clear */
            microhal::Bitfield<uint32_t, 20, 1> CGIF6;  /*!< Channel 6 Global interrupt clear */
            microhal::Bitfield<uint32_t, 21, 1> CTCIF6; /*!< Channel 6 Transfer Complete clear */
            microhal::Bitfield<uint32_t, 22, 1> CHTIF6; /*!< Channel 6 Half Transfer clear */
            microhal::Bitfield<uint32_t, 23, 1> CTEIF6; /*!< Channel 6 Transfer Error clear */
            microhal::Bitfield<uint32_t, 24, 1> CGIF7;  /*!< Channel 7 Global interrupt clear */
            microhal::Bitfield<uint32_t, 25, 1> CTCIF7; /*!< Channel 7 Transfer Complete clear */
            microhal::Bitfield<uint32_t, 26, 1> CHTIF7; /*!< Channel 7 Half Transfer clear */
            microhal::Bitfield<uint32_t, 27, 1> CTEIF7; /*!< Channel 7 Transfer Error clear */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IFCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IFCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IFCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IFCR operator&(uint32_t value) const {
            IFCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IFCR operator|(uint32_t value) const {
            IFCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IFCR, AccessType::ReadOnly>;
        friend class VolatileRegister<IFCR, AccessType::WriteOnly>;
        friend class VolatileRegister<IFCR, AccessType::ReadWrite>;
    };

    struct Channel {
        // DMA channel configuration register (DMA_CCR)
        union CCRx {
            union {
                microhal::Bitfield<uint32_t, 0, 1> EN;       /*!< Channel enable */
                microhal::Bitfield<uint32_t, 1, 1> TCIE;     /*!< Transfer complete interrupt enable */
                microhal::Bitfield<uint32_t, 2, 1> HTIE;     /*!< Half Transfer interrupt enable */
                microhal::Bitfield<uint32_t, 3, 1> TEIE;     /*!< Transfer error interrupt enable */
                microhal::Bitfield<uint32_t, 4, 1> DIR;      /*!< Data transfer direction */
                microhal::Bitfield<uint32_t, 5, 1> CIRC;     /*!< Circular mode */
                microhal::Bitfield<uint32_t, 6, 1> PINC;     /*!< Peripheral increment mode */
                microhal::Bitfield<uint32_t, 7, 1> MINC;     /*!< Memory increment mode */
                microhal::Bitfield<uint32_t, 8, 2> PSIZE;    /*!< Peripheral size */
                microhal::Bitfield<uint32_t, 10, 2> MSIZE;   /*!< Memory size */
                microhal::Bitfield<uint32_t, 12, 2> PL;      /*!< Channel Priority level */
                microhal::Bitfield<uint32_t, 14, 1> MEM2MEM; /*!< Memory to memory mode */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            CCRx &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            CCRx &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            CCRx &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            CCRx operator&(uint32_t value) const {
                CCRx tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            CCRx operator|(uint32_t value) const {
                CCRx tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<CCRx, AccessType::ReadOnly>;
            friend class VolatileRegister<CCRx, AccessType::WriteOnly>;
            friend class VolatileRegister<CCRx, AccessType::ReadWrite>;
        };

        // DMA channel x number of data register
        union CNDTRx {
            union {
                microhal::Bitfield<uint32_t, 0, 16> NDT; /*!< Number of data to transfer */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            CNDTRx &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            CNDTRx &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            CNDTRx &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            CNDTRx operator&(uint32_t value) const {
                CNDTRx tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            CNDTRx operator|(uint32_t value) const {
                CNDTRx tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<CNDTRx, AccessType::ReadOnly>;
            friend class VolatileRegister<CNDTRx, AccessType::WriteOnly>;
            friend class VolatileRegister<CNDTRx, AccessType::ReadWrite>;
        };

        VolatileRegister<CCRx, AccessType::ReadWrite> ccr;      /*!< DMA channel configuration register (DMA_CCR)	Address offset: 0x8 */
        VolatileRegister<CNDTRx, AccessType::ReadWrite> cndtr;  /*!< DMA channel x number of data register	Address offset: 0xC */
        VolatileRegister<uint32_t, AccessType::ReadWrite> cpar; /*!< DMA channel x peripheral address register	Address offset: 0x10 */
        VolatileRegister<uint32_t, AccessType::ReadWrite> cmar; /*!< DMA channel x memory address register	Address offset: 0x14 */
        uint32_t reserved;
    };

    VolatileRegister<ISR, AccessType::ReadWrite> isr;   /*!< DMA interrupt status register (DMA_ISR)	Address offset: 0x0 */
    VolatileRegister<IFCR, AccessType::ReadWrite> ifcr; /*!< DMA interrupt flag clear register (DMA_IFCR)	Address offset: 0x4 */
    // Channel channel[7];                                 /*!< Reserved register	Address offset: 0x18 */
};

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_DMA
