#ifndef _MICROHAL_PERIPHERAL_REGISTER_CANFILTER
#define _MICROHAL_PERIPHERAL_REGISTER_CANFILTER

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
struct CANFilter {
#ifdef _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FMR
    // filter master register
    union FMR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> FINIT; /*!< Filter init mode */
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
            microhal::Bitfield<uint32_t, 8, 6> CAN2SB; /*!< CAN2 start bank */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FMR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FMR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FMR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FMR operator&(uint32_t value) const {
            FMR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FMR operator|(uint32_t value) const {
            FMR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FMR, AccessType::ReadOnly>;
        friend class VolatileRegister<FMR, AccessType::WriteOnly>;
        friend class VolatileRegister<FMR, AccessType::ReadWrite>;
    };

    // filter mode register
    union FM1R {
        union {
            microhal::Bitfield<uint32_t, 0, 1> FBM0;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 1, 1> FBM1;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 2, 1> FBM2;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 3, 1> FBM3;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 4, 1> FBM4;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 5, 1> FBM5;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 6, 1> FBM6;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 7, 1> FBM7;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 8, 1> FBM8;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 9, 1> FBM9;   /*!< Filter mode */
            microhal::Bitfield<uint32_t, 10, 1> FBM10; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 11, 1> FBM11; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 12, 1> FBM12; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 13, 1> FBM13; /*!< Filter mode */
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMxR_HAS_FMB14_TO_FMT27
            microhal::Bitfield<uint32_t, 14, 1> FBM14; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 15, 1> FBM15; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 16, 1> FBM16; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 17, 1> FBM17; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 18, 1> FBM18; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 19, 1> FBM19; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 20, 1> FBM20; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 21, 1> FBM21; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 22, 1> FBM22; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 23, 1> FBM23; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 24, 1> FBM24; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 25, 1> FBM25; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 26, 1> FBM26; /*!< Filter mode */
            microhal::Bitfield<uint32_t, 27, 1> FBM27; /*!< Filter mode */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FM1R &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FM1R &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FM1R &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FM1R operator&(uint32_t value) const {
            FM1R tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FM1R operator|(uint32_t value) const {
            FM1R tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FM1R, AccessType::ReadOnly>;
        friend class VolatileRegister<FM1R, AccessType::WriteOnly>;
        friend class VolatileRegister<FM1R, AccessType::ReadWrite>;
    };

    // filter scale register
    union FS1R {
        union {
            microhal::Bitfield<uint32_t, 0, 1> FSC0;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 1, 1> FSC1;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 2, 1> FSC2;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 3, 1> FSC3;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 4, 1> FSC4;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 5, 1> FSC5;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 6, 1> FSC6;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 7, 1> FSC7;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 8, 1> FSC8;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 9, 1> FSC9;   /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 10, 1> FSC10; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 11, 1> FSC11; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 12, 1> FSC12; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 13, 1> FSC13; /*!< Filter scale configuration */
#ifdef _MICROHAL_REGISTERS_STM_CAN_FSxR_HAS_FSC14_TO_FSC27
            microhal::Bitfield<uint32_t, 14, 1> FSC14; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 15, 1> FSC15; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 16, 1> FSC16; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 17, 1> FSC17; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 18, 1> FSC18; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 19, 1> FSC19; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 20, 1> FSC20; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 21, 1> FSC21; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 22, 1> FSC22; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 23, 1> FSC23; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 24, 1> FSC24; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 25, 1> FSC25; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 26, 1> FSC26; /*!< Filter scale configuration */
            microhal::Bitfield<uint32_t, 27, 1> FSC27; /*!< Filter scale configuration */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FS1R &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FS1R &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FS1R &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FS1R operator&(uint32_t value) const {
            FS1R tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FS1R operator|(uint32_t value) const {
            FS1R tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FS1R, AccessType::ReadOnly>;
        friend class VolatileRegister<FS1R, AccessType::WriteOnly>;
        friend class VolatileRegister<FS1R, AccessType::ReadWrite>;
    };

    // filter FIFO assignment register
    union FFA1R {
        union {
            microhal::Bitfield<uint32_t, 0, 1> FFA0;   /*!< Filter FIFO assignment for filter 0 */
            microhal::Bitfield<uint32_t, 1, 1> FFA1;   /*!< Filter FIFO assignment for filter 1 */
            microhal::Bitfield<uint32_t, 2, 1> FFA2;   /*!< Filter FIFO assignment for filter 2 */
            microhal::Bitfield<uint32_t, 3, 1> FFA3;   /*!< Filter FIFO assignment for filter 3 */
            microhal::Bitfield<uint32_t, 4, 1> FFA4;   /*!< Filter FIFO assignment for filter 4 */
            microhal::Bitfield<uint32_t, 5, 1> FFA5;   /*!< Filter FIFO assignment for filter 5 */
            microhal::Bitfield<uint32_t, 6, 1> FFA6;   /*!< Filter FIFO assignment for filter 6 */
            microhal::Bitfield<uint32_t, 7, 1> FFA7;   /*!< Filter FIFO assignment for filter 7 */
            microhal::Bitfield<uint32_t, 8, 1> FFA8;   /*!< Filter FIFO assignment for filter 8 */
            microhal::Bitfield<uint32_t, 9, 1> FFA9;   /*!< Filter FIFO assignment for filter 9 */
            microhal::Bitfield<uint32_t, 10, 1> FFA10; /*!< Filter FIFO assignment for filter 10 */
            microhal::Bitfield<uint32_t, 11, 1> FFA11; /*!< Filter FIFO assignment for filter 11 */
            microhal::Bitfield<uint32_t, 12, 1> FFA12; /*!< Filter FIFO assignment for filter 12 */
            microhal::Bitfield<uint32_t, 13, 1> FFA13; /*!< Filter FIFO assignment for filter 13 */
#ifdef _MICROHAL_REGISTERS_STM_CAN_FFAxR_HAS_FFA14_TO_FFA27
            microhal::Bitfield<uint32_t, 14, 1> FFA14; /*!< Filter FIFO assignment for filter 14 */
            microhal::Bitfield<uint32_t, 15, 1> FFA15; /*!< Filter FIFO assignment for filter 15 */
            microhal::Bitfield<uint32_t, 16, 1> FFA16; /*!< Filter FIFO assignment for filter 16 */
            microhal::Bitfield<uint32_t, 17, 1> FFA17; /*!< Filter FIFO assignment for filter 17 */
            microhal::Bitfield<uint32_t, 18, 1> FFA18; /*!< Filter FIFO assignment for filter 18 */
            microhal::Bitfield<uint32_t, 19, 1> FFA19; /*!< Filter FIFO assignment for filter 19 */
            microhal::Bitfield<uint32_t, 20, 1> FFA20; /*!< Filter FIFO assignment for filter 20 */
            microhal::Bitfield<uint32_t, 21, 1> FFA21; /*!< Filter FIFO assignment for filter 21 */
            microhal::Bitfield<uint32_t, 22, 1> FFA22; /*!< Filter FIFO assignment for filter 22 */
            microhal::Bitfield<uint32_t, 23, 1> FFA23; /*!< Filter FIFO assignment for filter 23 */
            microhal::Bitfield<uint32_t, 24, 1> FFA24; /*!< Filter FIFO assignment for filter 24 */
            microhal::Bitfield<uint32_t, 25, 1> FFA25; /*!< Filter FIFO assignment for filter 25 */
            microhal::Bitfield<uint32_t, 26, 1> FFA26; /*!< Filter FIFO assignment for filter 26 */
            microhal::Bitfield<uint32_t, 27, 1> FFA27; /*!< Filter FIFO assignment for filter 27 */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FFA1R &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FFA1R &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FFA1R &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FFA1R operator&(uint32_t value) const {
            FFA1R tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FFA1R operator|(uint32_t value) const {
            FFA1R tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FFA1R, AccessType::ReadOnly>;
        friend class VolatileRegister<FFA1R, AccessType::WriteOnly>;
        friend class VolatileRegister<FFA1R, AccessType::ReadWrite>;
    };

    // CAN filter activation register
    union FA1R {
        union {
            microhal::Bitfield<uint32_t, 0, 1> FACT0;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 1, 1> FACT1;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 2, 1> FACT2;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 3, 1> FACT3;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 4, 1> FACT4;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 5, 1> FACT5;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 6, 1> FACT6;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 7, 1> FACT7;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 8, 1> FACT8;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 9, 1> FACT9;   /*!< Filter active */
            microhal::Bitfield<uint32_t, 10, 1> FACT10; /*!< Filter active */
            microhal::Bitfield<uint32_t, 11, 1> FACT11; /*!< Filter active */
            microhal::Bitfield<uint32_t, 12, 1> FACT12; /*!< Filter active */
            microhal::Bitfield<uint32_t, 13, 1> FACT13; /*!< Filter active */
#ifdef _MICROHAL_REGISTERS_STM_CAN_FAxR_HAS_FACT14_TO_FACT27
            microhal::Bitfield<uint32_t, 14, 1> FACT14; /*!< Filter active */
            microhal::Bitfield<uint32_t, 15, 1> FACT15; /*!< Filter active */
            microhal::Bitfield<uint32_t, 16, 1> FACT16; /*!< Filter active */
            microhal::Bitfield<uint32_t, 17, 1> FACT17; /*!< Filter active */
            microhal::Bitfield<uint32_t, 18, 1> FACT18; /*!< Filter active */
            microhal::Bitfield<uint32_t, 19, 1> FACT19; /*!< Filter active */
            microhal::Bitfield<uint32_t, 20, 1> FACT20; /*!< Filter active */
            microhal::Bitfield<uint32_t, 21, 1> FACT21; /*!< Filter active */
            microhal::Bitfield<uint32_t, 22, 1> FACT22; /*!< Filter active */
            microhal::Bitfield<uint32_t, 23, 1> FACT23; /*!< Filter active */
            microhal::Bitfield<uint32_t, 24, 1> FACT24; /*!< Filter active */
            microhal::Bitfield<uint32_t, 25, 1> FACT25; /*!< Filter active */
            microhal::Bitfield<uint32_t, 26, 1> FACT26; /*!< Filter active */
            microhal::Bitfield<uint32_t, 27, 1> FACT27; /*!< Filter active */
#endif
        };

        void activateFilter(uint_fast8_t filterNumber) { raw |= 1 << filterNumber; }
        void deactivateFilter(uint_fast8_t filterNumber) { raw &= ~(1 << filterNumber); }
        bool isFilterActive(uint_fast8_t filterNumber) { return raw & (1 << filterNumber); }

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FA1R &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FA1R &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FA1R &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FA1R operator&(uint32_t value) const {
            FA1R tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FA1R operator|(uint32_t value) const {
            FA1R tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FA1R, AccessType::ReadOnly>;
        friend class VolatileRegister<FA1R, AccessType::WriteOnly>;
        friend class VolatileRegister<FA1R, AccessType::ReadWrite>;
    };
#endif
    struct FilterRegister {
        // Filter bank x register 1
        union FxRx {
            union {
                microhal::Bitfield<uint32_t, 0, 1> FB0;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 1, 1> FB1;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 2, 1> FB2;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 3, 1> FB3;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 4, 1> FB4;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 5, 1> FB5;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 6, 1> FB6;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 7, 1> FB7;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 8, 1> FB8;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 9, 1> FB9;   /*!< Filter bits */
                microhal::Bitfield<uint32_t, 10, 1> FB10; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 11, 1> FB11; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 12, 1> FB12; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 13, 1> FB13; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 14, 1> FB14; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 15, 1> FB15; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 16, 1> FB16; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 17, 1> FB17; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 18, 1> FB18; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 19, 1> FB19; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 20, 1> FB20; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 21, 1> FB21; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 22, 1> FB22; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 23, 1> FB23; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 24, 1> FB24; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 25, 1> FB25; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 26, 1> FB26; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 27, 1> FB27; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 28, 1> FB28; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 29, 1> FB29; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 30, 1> FB30; /*!< Filter bits */
                microhal::Bitfield<uint32_t, 31, 1> FB31; /*!< Filter bits */
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            FxRx &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            FxRx &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            FxRx &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            FxRx operator&(uint32_t value) const {
                FxRx tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            FxRx operator|(uint32_t value) const {
                FxRx tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<FxRx, AccessType::ReadOnly>;
            friend class VolatileRegister<FxRx, AccessType::WriteOnly>;
            friend class VolatileRegister<FxRx, AccessType::ReadWrite>;
        };

        union ID32 {
            union {
                microhal::Bitfield<uint32_t, 0, 1> zero;
                microhal::Bitfield<uint32_t, 1, 1> rtr;
                microhal::Bitfield<uint32_t, 2, 1> ide;
                microhal::Bitfield<uint32_t, 21, 11> stid;
                microhal::Bitfield<uint32_t, 3, 29> exid;
            };

            operator uint32_t() const { return raw; }
            operator bool() const { return raw; }

            ID32 &operator=(uint32_t value) {
                raw = value;
                return *this;
            }
            ID32 &operator|=(uint32_t value) {
                raw |= value;
                return *this;
            }
            ID32 &operator&=(uint32_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint32_t value) const { return raw == value; }
            bool operator!=(uint32_t value) const { return raw != value; }
            bool operator>(uint32_t value) const { return raw > value; }
            bool operator<(uint32_t value) const { return raw < value; }
            bool operator>=(uint32_t value) const { return raw >= value; }
            bool operator<=(uint32_t value) const { return raw <= value; }
            ID32 operator&(uint32_t value) const {
                ID32 tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            ID32 operator|(uint32_t value) const {
                ID32 tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint32_t raw;
            friend class VolatileRegister<ID32, AccessType::ReadOnly>;
            friend class VolatileRegister<ID32, AccessType::WriteOnly>;
            friend class VolatileRegister<ID32, AccessType::ReadWrite>;
        };

        union ID16 {
            union {
                microhal::Bitfield<uint16_t, 0, 3> exid_15_17;
                microhal::Bitfield<uint16_t, 3, 1> ide;
                microhal::Bitfield<uint16_t, 4, 1> rtr;
                microhal::Bitfield<uint16_t, 5, 11> stid;
            };

            void setExtendedId(uint32_t id) {
                exid_15_17 = id >> 15;
                stid = id >> 19;
            }

            uint32_t getExtendedId() const {
                uint32_t id = stid << 19;
                id |= exid_15_17 << 15;
                return id;
            }

            operator uint32_t() const { return raw; }

            ID16 &operator=(uint16_t value) {
                raw = value;
                return *this;
            }
            ID16 &operator|=(uint16_t value) {
                raw |= value;
                return *this;
            }
            ID16 &operator&=(uint16_t value) {
                raw &= value;
                return *this;
            }

            bool operator==(uint16_t value) const { return raw == value; }
            bool operator==(ID16 value) const { return raw == value.raw; }
            bool operator!=(uint16_t value) const { return raw != value; }
            bool operator>(uint16_t value) const { return raw > value; }
            bool operator<(uint16_t value) const { return raw < value; }
            bool operator>=(uint16_t value) const { return raw >= value; }
            bool operator<=(uint16_t value) const { return raw <= value; }
            ID16 operator&(uint16_t value) const {
                ID16 tmp;
                tmp.raw = raw & value;
                return tmp;
            }
            ID16 operator|(uint16_t value) const {
                ID16 tmp;
                tmp.raw = raw | value;
                return tmp;
            }

         private:
            uint16_t raw;
            friend class VolatileRegister<ID16, AccessType::ReadOnly>;
            friend class VolatileRegister<ID16, AccessType::WriteOnly>;
            friend class VolatileRegister<ID16, AccessType::ReadWrite>;
        };

        union IdentifierMask16Bit {
            struct {
                ID16 id;
                ID16 mask;
            };

         private:
            uint32_t raw;
            friend class VolatileRegister<IdentifierMask16Bit, AccessType::ReadOnly>;
            friend class VolatileRegister<IdentifierMask16Bit, AccessType::WriteOnly>;
            friend class VolatileRegister<IdentifierMask16Bit, AccessType::ReadWrite>;
        };

        union IdentifierList16Bit {
            struct {
                ID16 id0;
                ID16 id1;
            };

         private:
            uint32_t raw;
            friend class VolatileRegister<IdentifierList16Bit, AccessType::ReadOnly>;
            friend class VolatileRegister<IdentifierList16Bit, AccessType::WriteOnly>;
            friend class VolatileRegister<IdentifierList16Bit, AccessType::ReadWrite>;
        };

        union {
            struct {
                VolatileRegister<FxRx, AccessType::ReadWrite> fr[2];
            } raw;
            struct {
                VolatileRegister<ID32, AccessType::ReadWrite> id;
                VolatileRegister<ID32, AccessType::ReadWrite> mask;
            } identifierMask;
            VolatileRegister<ID32, AccessType::ReadWrite> identifierList[2];
            VolatileRegister<IdentifierMask16Bit, AccessType::ReadWrite> identifierMask16bit[2];
            VolatileRegister<IdentifierList16Bit, AccessType::ReadWrite> identifierList16bit[2];
        };
    };
    static_assert(sizeof(FilterRegister) == 2 * sizeof(uint32_t), "");

#ifdef _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FMR
    VolatileRegister<FMR, AccessType::ReadWrite> fmr;     /*!< filter master register	Address offset: 0x200 */
    VolatileRegister<FM1R, AccessType::ReadWrite> fm1r;   /*!< filter mode register	Address offset: 0x204 */
    uint32_t reserved2;                                   /*!< Reserved register	Address offset: 0x208 */
    VolatileRegister<FS1R, AccessType::ReadWrite> fs1r;   /*!< filter scale register	Address offset: 0x20C */
    uint32_t reserved3;                                   /*!< Reserved register	Address offset: 0x210 */
    VolatileRegister<FFA1R, AccessType::ReadWrite> ffa1r; /*!< filter FIFO assignment register	Address offset: 0x214 */
    uint32_t reserved4;                                   /*!< Reserved register	Address offset: 0x218 */
    VolatileRegister<FA1R, AccessType::ReadWrite> fa1r;   /*!< CAN filter activation register	Address offset: 0x21C */
    uint32_t reserved5[8];                                /*!< Reserved register	Address offset: 0x220 */
#else
    uint32_t reserved1[28]; /*!< Reserved register	Address offset: 0x1d0 */
#endif
#ifdef _MICROHAL_REGISTERS_STM_CAN_FILTERS_HAS_FILTER_BANK14_TO_FILTER_BANK_27
    std::array<FilterRegister, 28> filterRegister;
#else
    std::array<FilterRegister, 14> filterRegister;
#endif
};

#if defined(_MICROHAL_CAN_COMMON_FILTERS_BASE_ADDRESS)
[[maybe_unused]] static CANFilter *const canFilter = reinterpret_cast<CANFilter *>(_MICROHAL_CAN_COMMON_FILTERS_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_CANFILTER
