#ifndef _MICROHAL_PERIPHERAL_REGISTER_EXTI
#define _MICROHAL_PERIPHERAL_REGISTER_EXTI

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU variant 1: STM32G071, STM32G070, STM32G081, STM32G071, STM32G070, STM32G081
// Supported MCU variant 2: STM32G041, STM32G030, STM32G031, STM32G041, STM32G030, STM32G031

#if defined(STM32G071) || defined(STM32G070) || defined(STM32G081) || defined(STM32G071) || defined(STM32G070) || defined(STM32G081)
#define _MICROHAL_REGISTERS_EXTI_V2_VARIANT1
#endif

#if defined(STM32G041) || defined(STM32G030) || defined(STM32G031) || defined(STM32G041) || defined(STM32G030) || defined(STM32G031)
#define _MICROHAL_REGISTERS_EXTI_V2_VARIANT2
#endif

#ifdef _MICROHAL_REGISTERS_EXTI_V2_VARIANT1
#define _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
#define _MICROHAL_REGISTERS_EXTI_V2_HAS_IMR2
#define _MICROHAL_REGISTERS_EXTI_V2_HAS_EMR2
#define _MICROHAL_REGISTERS_EXTI_V2_HAS_HWCFGR
#define _MICROHAL_REGISTERS_EXTI_V2_HAS_VERR
#define _MICROHAL_REGISTERS_EXTI_V2_HAS_IPIDR
#define _MICROHAL_REGISTERS_EXTI_V2_HAS_SIDR
#endif

#ifdef _MICROHAL_REGISTERS_EXTI_V2_VARIANT2
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct EXTI {
    // EXTI rising trigger selection register
    union RTSR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TR0;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 1, 1> TR1;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 2, 1> TR2;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 3, 1> TR3;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 4, 1> TR4;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 5, 1> TR5;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 6, 1> TR6;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 7, 1> TR7;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 8, 1> TR8;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 9, 1> TR9;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 10, 1> TR10; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 11, 1> TR11; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 12, 1> TR12; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 13, 1> TR13; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 14, 1> TR14; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 15, 1> TR15; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 16, 1> TR16; /*!< Rising trigger event configuration bit of Configurable Event input */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
            microhal::Bitfield<uint32_t, 17, 1> TR17; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 18, 1> TR18; /*!< Rising trigger event configuration bit of Configurable Event input */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RTSR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RTSR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RTSR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RTSR1 operator&(uint32_t value) const {
            RTSR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RTSR1 operator|(uint32_t value) const {
            RTSR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RTSR1, AccessType::ReadOnly>;
        friend class VolatileRegister<RTSR1, AccessType::WriteOnly>;
        friend class VolatileRegister<RTSR1, AccessType::ReadWrite>;
    };

    // EXTI falling trigger selection register
    union FTSR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TR0;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 1, 1> TR1;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 2, 1> TR2;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 3, 1> TR3;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 4, 1> TR4;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 5, 1> TR5;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 6, 1> TR6;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 7, 1> TR7;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 8, 1> TR8;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 9, 1> TR9;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 10, 1> TR10; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 11, 1> TR11; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 12, 1> TR12; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 13, 1> TR13; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 14, 1> TR14; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 15, 1> TR15; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 16, 1> TR16; /*!< Rising trigger event configuration bit of Configurable Event input */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
            microhal::Bitfield<uint32_t, 17, 1> TR17; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 18, 1> TR18; /*!< Rising trigger event configuration bit of Configurable Event input */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FTSR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FTSR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FTSR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FTSR1 operator&(uint32_t value) const {
            FTSR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FTSR1 operator|(uint32_t value) const {
            FTSR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FTSR1, AccessType::ReadOnly>;
        friend class VolatileRegister<FTSR1, AccessType::WriteOnly>;
        friend class VolatileRegister<FTSR1, AccessType::ReadWrite>;
    };

    // EXTI software interrupt event register
    union SWIER1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SWI0;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 1, 1> SWI1;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 2, 1> SWI2;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 3, 1> SWI3;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 4, 1> SWI4;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 5, 1> SWI5;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 6, 1> SWI6;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 7, 1> SWI7;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 8, 1> SWI8;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 9, 1> SWI9;   /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 10, 1> SWI10; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 11, 1> SWI11; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 12, 1> SWI12; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 13, 1> SWI13; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 14, 1> SWI14; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 15, 1> SWI15; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 16, 1> SWI16; /*!< Rising trigger event configuration bit of Configurable Event input */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
            microhal::Bitfield<uint32_t, 17, 1> SWI17; /*!< Rising trigger event configuration bit of Configurable Event input */
            microhal::Bitfield<uint32_t, 18, 1> SWI18; /*!< Rising trigger event configuration bit of Configurable Event input */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SWIER1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SWIER1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SWIER1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SWIER1 operator&(uint32_t value) const {
            SWIER1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SWIER1 operator|(uint32_t value) const {
            SWIER1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SWIER1, AccessType::ReadOnly>;
        friend class VolatileRegister<SWIER1, AccessType::WriteOnly>;
        friend class VolatileRegister<SWIER1, AccessType::ReadWrite>;
    };

    // EXTI rising edge pending register
    union RPR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> RPIF0;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 1, 1> RPIF1;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 2, 1> RPIF2;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 3, 1> RPIF3;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 4, 1> RPIF4;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 5, 1> RPIF5;   /*!< configurable event inputs x rising edge Pending bit */
            microhal::Bitfield<uint32_t, 6, 1> RPIF6;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 7, 1> RPIF7;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 8, 1> RPIF8;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 9, 1> RPIF9;   /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 10, 1> RPIF10; /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 11, 1> RPIF11; /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 12, 1> RPIF12; /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 13, 1> RPIF13; /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 14, 1> RPIF14; /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 15, 1> RPIF15; /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 16, 1> RPIF16; /*!< configurable event inputs x rising edge Pending bit. */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
            microhal::Bitfield<uint32_t, 17, 1> RPIF17; /*!< configurable event inputs x rising edge Pending bit. */
            microhal::Bitfield<uint32_t, 18, 1> RPIF18; /*!< configurable event inputs x rising edge Pending bit. */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RPR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RPR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RPR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RPR1 operator&(uint32_t value) const {
            RPR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RPR1 operator|(uint32_t value) const {
            RPR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RPR1, AccessType::ReadOnly>;
        friend class VolatileRegister<RPR1, AccessType::WriteOnly>;
        friend class VolatileRegister<RPR1, AccessType::ReadWrite>;
    };

    // EXTI falling edge pending register
    union FPR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> FPIF0;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 1, 1> FPIF1;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 2, 1> FPIF2;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 3, 1> FPIF3;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 4, 1> FPIF4;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 5, 1> FPIF5;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 6, 1> FPIF6;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 7, 1> FPIF7;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 8, 1> FPIF8;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 9, 1> FPIF9;   /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 10, 1> FPIF10; /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 11, 1> FPIF11; /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 12, 1> FPIF12; /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 13, 1> FPIF13; /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 14, 1> FPIF14; /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 15, 1> FPIF15; /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 16, 1> FPIF16; /*!< configurable event inputs x falling edge pending bit. */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
            microhal::Bitfield<uint32_t, 17, 1> FPIF17; /*!< configurable event inputs x falling edge pending bit. */
            microhal::Bitfield<uint32_t, 18, 1> FPIF18; /*!< configurable event inputs x falling edge pending bit. */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FPR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FPR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FPR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FPR1 operator&(uint32_t value) const {
            FPR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FPR1 operator|(uint32_t value) const {
            FPR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FPR1, AccessType::ReadOnly>;
        friend class VolatileRegister<FPR1, AccessType::WriteOnly>;
        friend class VolatileRegister<FPR1, AccessType::ReadWrite>;
    };

    // EXTI external interrupt selection register
    union EXTICR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> EXTI0_7;    /*!< GPIO port selection */
            microhal::Bitfield<uint32_t, 8, 8> EXTI8_15;   /*!< GPIO port selection */
            microhal::Bitfield<uint32_t, 16, 8> EXTI16_23; /*!< GPIO port selection */
            microhal::Bitfield<uint32_t, 24, 8> EXTI24_31; /*!< GPIO port selection */
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

    // EXTI CPU wakeup with interrupt mask register
    union IMR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> IM0;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 1, 1> IM1;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 2, 1> IM2;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 3, 1> IM3;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 4, 1> IM4;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 5, 1> IM5;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 6, 1> IM6;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 7, 1> IM7;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 8, 1> IM8;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 9, 1> IM9;   /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 10, 1> IM10; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 11, 1> IM11; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 12, 1> IM12; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 13, 1> IM13; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 14, 1> IM14; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 15, 1> IM15; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 16, 1> IM16; /*!< CPU wakeup with interrupt mask on event input */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
            microhal::Bitfield<uint32_t, 17, 1> IM17; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 18, 1> IM18; /*!< CPU wakeup with interrupt mask on event input */
#endif
            microhal::Bitfield<uint32_t, 19, 1> IM19; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 20, 1> IM20; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 21, 1> IM21; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 22, 1> IM22; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 23, 1> IM23; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 24, 1> IM24; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 25, 1> IM25; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 26, 1> IM26; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 27, 1> IM27; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 28, 1> IM28; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 29, 1> IM29; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 30, 1> IM30; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 31, 1> IM31; /*!< CPU wakeup with interrupt mask on event input */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IMR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IMR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IMR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IMR1 operator&(uint32_t value) const {
            IMR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IMR1 operator|(uint32_t value) const {
            IMR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IMR1, AccessType::ReadOnly>;
        friend class VolatileRegister<IMR1, AccessType::WriteOnly>;
        friend class VolatileRegister<IMR1, AccessType::ReadWrite>;
    };

    // EXTI CPU wakeup with event mask register
    union EMR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EM0;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 1, 1> EM1;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 2, 1> EM2;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 3, 1> EM3;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 4, 1> EM4;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 5, 1> EM5;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 6, 1> EM6;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 7, 1> EM7;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 8, 1> EM8;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 9, 1> EM9;   /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 10, 1> EM10; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 11, 1> EM11; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 12, 1> EM12; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 13, 1> EM13; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 14, 1> EM14; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 15, 1> EM15; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 16, 1> EM16; /*!< CPU wakeup with event mask on event input */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_TR17_TR18
            microhal::Bitfield<uint32_t, 17, 1> EM17; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 18, 1> EM18; /*!< CPU wakeup with event mask on event input */
#endif
            microhal::Bitfield<uint32_t, 19, 1> EM19; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 21, 1> EM21; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 23, 1> EM23; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 25, 1> EM25; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 26, 1> EM26; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 27, 1> EM27; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 28, 1> EM28; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 29, 1> EM29; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 30, 1> EM30; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 31, 1> EM31; /*!< CPU wakeup with event mask on event input */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        EMR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        EMR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        EMR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        EMR1 operator&(uint32_t value) const {
            EMR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        EMR1 operator|(uint32_t value) const {
            EMR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<EMR1, AccessType::ReadOnly>;
        friend class VolatileRegister<EMR1, AccessType::WriteOnly>;
        friend class VolatileRegister<EMR1, AccessType::ReadWrite>;
    };

    // EXTI CPU wakeup with interrupt mask register
    union IMR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> IM32; /*!< CPU wakeup with interrupt mask on event input */
            microhal::Bitfield<uint32_t, 1, 1> IM33; /*!< CPU wakeup with interrupt mask on event input */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IMR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IMR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IMR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IMR2 operator&(uint32_t value) const {
            IMR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IMR2 operator|(uint32_t value) const {
            IMR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IMR2, AccessType::ReadOnly>;
        friend class VolatileRegister<IMR2, AccessType::WriteOnly>;
        friend class VolatileRegister<IMR2, AccessType::ReadWrite>;
    };

    // EXTI CPU wakeup with event mask register
    union EMR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EM32; /*!< CPU wakeup with event mask on event input */
            microhal::Bitfield<uint32_t, 1, 1> EM33; /*!< CPU wakeup with event mask on event input */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        EMR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        EMR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        EMR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        EMR2 operator&(uint32_t value) const {
            EMR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        EMR2 operator|(uint32_t value) const {
            EMR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<EMR2, AccessType::ReadOnly>;
        friend class VolatileRegister<EMR2, AccessType::WriteOnly>;
        friend class VolatileRegister<EMR2, AccessType::ReadWrite>;
    };

    // Hardware configuration registers
    union HWCFGR7 {
        union {
            microhal::Bitfield<uint32_t, 0, 32> CPUEVENT; /*!< HW configuration CPU event generation */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR7 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR7 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR7 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR7 operator&(uint32_t value) const {
            HWCFGR7 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR7 operator|(uint32_t value) const {
            HWCFGR7 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR7, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR7, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR7, AccessType::ReadWrite>;
    };

    // Hardware configuration registers
    union HWCFGR6 {
        union {
            microhal::Bitfield<uint32_t, 0, 32> CPUEVENT; /*!< HW configuration CPU event generation */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR6 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR6 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR6 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR6 operator&(uint32_t value) const {
            HWCFGR6 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR6 operator|(uint32_t value) const {
            HWCFGR6 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR6, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR6, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR6, AccessType::ReadWrite>;
    };

    // Hardware configuration registers
    union HWCFGR5 {
        union {
            microhal::Bitfield<uint32_t, 0, 32> CPUEVENT; /*!< HW configuration CPU event generation */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR5 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR5 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR5 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR5 operator&(uint32_t value) const {
            HWCFGR5 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR5 operator|(uint32_t value) const {
            HWCFGR5 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR5, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR5, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR5, AccessType::ReadWrite>;
    };

    // Hardware configuration registers
    union HWCFGR4 {
        union {
            microhal::Bitfield<uint32_t, 0, 32> EVENT_TRG; /*!< HW configuration event trigger type */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR4 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR4 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR4 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR4 operator&(uint32_t value) const {
            HWCFGR4 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR4 operator|(uint32_t value) const {
            HWCFGR4 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR4, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR4, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR4, AccessType::ReadWrite>;
    };

    // Hardware configuration registers
    union HWCFGR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 32> EVENT_TRG; /*!< HW configuration event trigger type */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR3 operator&(uint32_t value) const {
            HWCFGR3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR3 operator|(uint32_t value) const {
            HWCFGR3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR3, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR3, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR3, AccessType::ReadWrite>;
    };

    // Hardware configuration registers
    union HWCFGR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 32> EVENT_TRG; /*!< HW configuration event trigger type */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR2 operator&(uint32_t value) const {
            HWCFGR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR2 operator|(uint32_t value) const {
            HWCFGR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR2, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR2, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR2, AccessType::ReadWrite>;
    };

    // Hardware configuration registers
    union HWCFGR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 8> NBEVENTS;  /*!< configuration number of event */
            microhal::Bitfield<uint32_t, 8, 4> NBCPUS;    /*!< configuration number of CPUs */
            microhal::Bitfield<uint32_t, 12, 4> CPUEVTEN; /*!< HW configuration of CPU event output enable */
            microhal::Bitfield<uint32_t, 16, 8> NBIOPORT; /*!< HW configuration of number of IO ports */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR1 operator&(uint32_t value) const {
            HWCFGR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR1 operator|(uint32_t value) const {
            HWCFGR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR1, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR1, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR1, AccessType::ReadWrite>;
    };

    // AES version register
    union VERR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> MINREV; /*!< Minor revision */
            microhal::Bitfield<uint32_t, 4, 4> MAJREV; /*!< Major revision */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        VERR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        VERR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        VERR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        VERR operator&(uint32_t value) const {
            VERR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        VERR operator|(uint32_t value) const {
            VERR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<VERR, AccessType::ReadOnly>;
        friend class VolatileRegister<VERR, AccessType::WriteOnly>;
        friend class VolatileRegister<VERR, AccessType::ReadWrite>;
    };

    // AES identification register
    union IPIDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> ID; /*!< Identification code */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IPIDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IPIDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IPIDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IPIDR operator&(uint32_t value) const {
            IPIDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IPIDR operator|(uint32_t value) const {
            IPIDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IPIDR, AccessType::ReadOnly>;
        friend class VolatileRegister<IPIDR, AccessType::WriteOnly>;
        friend class VolatileRegister<IPIDR, AccessType::ReadWrite>;
    };

    // AES size ID register
    union SIDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> ID; /*!< Size Identification code */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SIDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SIDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SIDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SIDR operator&(uint32_t value) const {
            SIDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SIDR operator|(uint32_t value) const {
            SIDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SIDR, AccessType::ReadOnly>;
        friend class VolatileRegister<SIDR, AccessType::WriteOnly>;
        friend class VolatileRegister<SIDR, AccessType::ReadWrite>;
    };

    VolatileRegister<RTSR1, AccessType::ReadWrite> rtsr1;      /*!< EXTI rising trigger selection register	Address offset: 0x0 */
    VolatileRegister<FTSR1, AccessType::ReadWrite> ftsr1;      /*!< EXTI falling trigger selection register	Address offset: 0x4 */
    VolatileRegister<SWIER1, AccessType::ReadWrite> swier1;    /*!< EXTI software interrupt event register	Address offset: 0x8 */
    VolatileRegister<RPR1, AccessType::ReadWrite> rpr1;        /*!< EXTI rising edge pending register	Address offset: 0xC */
    VolatileRegister<FPR1, AccessType::ReadWrite> fpr1;        /*!< EXTI falling edge pending register	Address offset: 0x10 */
    uint32_t reserved0[19];                                    /*!< Reserved register	Address offset: 0x14 */
    VolatileRegister<EXTICR, AccessType::ReadWrite> exticr[4]; /*!< EXTI external interrupt selection register	Address offset: 0x60 */
    // VolatileRegister<EXTICR2, AccessType::ReadWrite> exticr2; /*!< EXTI external interrupt selection register	Address offset: 0x64 */
    // VolatileRegister<EXTICR3, AccessType::ReadWrite> exticr3; /*!< EXTI external interrupt selection register	Address offset: 0x68 */
    // VolatileRegister<EXTICR4, AccessType::ReadWrite> exticr4; /*!< EXTI external interrupt selection register	Address offset: 0x6C */
    uint32_t reserved1[4];                              /*!< Reserved register	Address offset: 0x70 */
    VolatileRegister<IMR1, AccessType::ReadWrite> imr1; /*!< EXTI CPU wakeup with interrupt mask register	Address offset: 0x80 */
    VolatileRegister<EMR1, AccessType::ReadWrite> emr1; /*!< EXTI CPU wakeup with event mask register	Address offset: 0x80 */
    uint32_t reserved3[2];                              /*!< Reserved register	Address offset: 0x84 */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_IMR2
    VolatileRegister<IMR2, AccessType::ReadWrite> imr2; /*!< EXTI CPU wakeup with interrupt mask register	Address offset: 0x90 */
#endif
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_EMR2
    VolatileRegister<EMR2, AccessType::ReadWrite> emr2; /*!< EXTI CPU wakeup with event mask register	Address offset: 0x94 */
#endif
    uint32_t reserved4[208]; /*!< Reserved register	Address offset: 0x98 */
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_HWCFGR
    VolatileRegister<HWCFGR7, AccessType::ReadWrite> hwcfgr7; /*!< Hardware configuration registers	Address offset: 0x3D8 */
    VolatileRegister<HWCFGR6, AccessType::ReadWrite> hwcfgr6; /*!< Hardware configuration registers	Address offset: 0x3DC */
    VolatileRegister<HWCFGR5, AccessType::ReadWrite> hwcfgr5; /*!< Hardware configuration registers	Address offset: 0x3E0 */
    VolatileRegister<HWCFGR4, AccessType::ReadWrite> hwcfgr4; /*!< Hardware configuration registers	Address offset: 0x3E4 */
    VolatileRegister<HWCFGR3, AccessType::ReadWrite> hwcfgr3; /*!< Hardware configuration registers	Address offset: 0x3E8 */
    VolatileRegister<HWCFGR2, AccessType::ReadWrite> hwcfgr2; /*!< Hardware configuration registers	Address offset: 0x3EC */
    VolatileRegister<HWCFGR1, AccessType::ReadWrite> hwcfgr1; /*!< Hardware configuration registers	Address offset: 0x3F0 */
#endif
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_VERR
    VolatileRegister<VERR, AccessType::ReadWrite> verr; /*!< AES version register	Address offset: 0x3F4 */
#endif
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_IPIDR
    VolatileRegister<IPIDR, AccessType::ReadWrite> ipidr; /*!< AES identification register	Address offset: 0x3F8 */
#endif
#ifdef _MICROHAL_REGISTERS_EXTI_V2_HAS_SIDR
    VolatileRegister<SIDR, AccessType::ReadWrite> sidr; /*!< AES size ID register	Address offset: 0x3FC */
#endif
};

#if defined(_MICROHAL_EXTI_BASE_ADDRESS)
[[maybe_unused]] inline EXTI *const exti1 = reinterpret_cast<EXTI *>(_MICROHAL_EXTI_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_EXTI
