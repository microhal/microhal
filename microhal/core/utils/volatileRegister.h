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

    T volatileLoad() const {
        T tmp;
        tmp.raw = const_cast<volatile decltype(reg.raw) &>(reg.raw);
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

}  // namespace microhal

#endif /* _MICROHAL_PORTS_VOLATILEREGISTER_H_ */
