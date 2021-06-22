/*
 * volatileRegister.h
 *
 *  Created on: Oct 31, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_VOLATILEREGISTER_H_
#define _MICROHAL_PORTS_VOLATILEREGISTER_H_

#include <cstdint>
#include <type_traits>

namespace microhal {

enum AccessType { ReadOnly, WriteOnly, ReadWrite };

template <typename T, AccessType Access>
struct VolatileRegister {
    using underlyingType = T;
    void volatileStore(T value) { const_cast<volatile decltype(reg.raw) &>(reg.raw) = value.raw; }
    void volatileStore(decltype(T::raw) value) { const_cast<volatile decltype(reg.raw) &>(reg.raw) = value; }
    void volatileStore_8bit(decltype(T::raw) value) { reinterpret_cast<volatile uint8_t &>(reg.raw) = value; }

    T volatileLoad() const {
        T tmp;
        tmp.raw = const_cast<volatile decltype(reg.raw) &>(reg.raw);
        return tmp;
    }

    T volatileLoad_8bit() const {
        const void *ptr = &reg.raw;
        volatile uint8_t *volatilePtr = reinterpret_cast<volatile uint8_t *>(const_cast<void *>(ptr));
        T tmp;
        tmp.raw = *volatilePtr;
        return tmp;
    }

 private:
    T reg;
};

template <typename T>
struct VolatileRegister<T, WriteOnly> {
    using underlyingType = T;
    void volatileStore(T value) { const_cast<volatile uint32_t &>(reg) = value; }

 private:
    T reg;
};

template <typename T>
struct VolatileRegister<T, ReadOnly> {
    using underlyingType = T;
    T volatileLoad() const {
        T tmp;
        tmp.raw = const_cast<volatile uint32_t &>(reg.raw);
        return tmp;
    }

 private:
    T reg;
};

template <>
struct VolatileRegister<uint32_t, ReadWrite> {
    using underlyingType = uint32_t;
    void volatileStore(uint32_t value) { const_cast<volatile uint32_t &>(reg) = value; }

    uint32_t volatileLoad() const { return const_cast<volatile uint32_t &>(reg); }

 private:
    uint32_t reg;
};

}  // namespace microhal

#endif /* _MICROHAL_PORTS_VOLATILEREGISTER_H_ */
