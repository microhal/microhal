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
    void volatileStore(T value) { const_cast<volatile decltype(reg.raw) &>(reg.raw) = value.raw; }

    T volatileLoad() const {
        T tmp;
        tmp.raw = const_cast<volatile uint32_t &>(reg.raw);
        return tmp;
    }

 private:
    T reg;
};

template <typename T>
struct VolatileRegister<T, WriteOnly> {
    void volatileStore(T value) { const_cast<volatile uint32_t &>(reg.raw) = value.raw; }

 private:
    T reg;
};

template <typename T>
struct VolatileRegister<T, ReadOnly> {
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
