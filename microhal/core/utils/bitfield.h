/*
 * bitfield.h
 *
 *  Created on: Aug 15, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_BITFIELD_H_
#define _MICROHAL_PORTS_BITFIELD_H_

#include <cstdint>

namespace microhal {

template <typename T>
auto volatileLoad(const T &reg) {
    T tmp = const_cast<volatile T &>(reg);
    return tmp;
}

template <typename T>
void volatileStore(const T &reg, T &addr) {
    const_cast<volatile T>(addr) = reg;
}

template <typename type, int...>
class Bitfield;

template <typename type, int begin, int size>
class Bitfield<type, begin, size> {
    static_assert(begin >= 0, "Template parameter 'begin' have to be bigger than 0.");
    static_assert(size >= 0, "Template parameter 'size' have to be bigger than 0.");
    static_assert((begin + size) <= (sizeof(type) * 8), "Bitfield is bigger than size of data type.");

 public:
    Bitfield &operator=(type value) {
        uint32_t tmp = data;
        tmp &= ~bitfieldMask;
        tmp |= (value & valueMask) << begin;
        data = tmp;
        return *this;
    }

    Bitfield &operator|=(type value) {
        uint32_t tmp = data;
        tmp |= (value & valueMask) << begin;
        data = tmp;
        return *this;
    }

    Bitfield &operator&=(type value) {
        uint32_t tmp = data;
        tmp &= (value | ~valueMask) << begin;
        data = tmp;
        return *this;
    }

    operator type() const { return get(); }

    void set() { data |= bitfieldMask; }
    void clear() { data &= ~bitfieldMask; }
    type get() const { return (data >> begin) & valueMask; }

 private:
    type data;

    static constexpr type maskGen(int maskBegin, int bitfieldSize) {
        type mask = 0;
        for (int i = maskBegin; i < (maskBegin + bitfieldSize); i++) {
            mask |= 1 << i;
        }
        return mask;
    }
    static_assert(maskGen(0, 1) == 0b01, "");
    static_assert(maskGen(0, 2) == 0b11, "");

    enum : type { bitfieldMask = maskGen(begin, size), valueMask = maskGen(0, size) };
};

template <typename type, int begin, int end, int begin2, int end2>
class Bitfield<type, begin, end, begin2, end2> {
 public:
    Bitfield &operator=(type value) volatile {
        uint32_t tmp = data;
        tmp &= ~(bitfieldMask1 | bitfieldMask2);
        tmp |= (value & valueMask1) << begin;
        tmp |= (value & valueMask2) << (begin2 - end);
        data = tmp;
        return *this;
    }

    Bitfield &operator|=(type value) volatile {
        uint32_t tmp = data;
        tmp |= (value & valueMask1) << begin;
        tmp |= (value & valueMask2) << (begin2 - end);
        data = tmp;
        return *this;
    }

    Bitfield &operator&=(type value) volatile {
        uint32_t tmp = data;
        tmp &= (value | ~valueMask1) << begin;
        tmp &= (value | ~valueMask2) << (begin2 - end);
        data = tmp;
        return *this;
    }

    operator type() volatile const { return get(); }

    void set() volatile { data |= bitfieldMask1 | bitfieldMask2; }
    void clear() volatile { data &= ~(bitfieldMask1 | bitfieldMask2); }
    type get() volatile const {
        type tmp = data;
        type value1 = (tmp & bitfieldMask1) >> begin;
        type value2 = (tmp & bitfieldMask2) >> (begin2 - end);
        return value1 | value2;
    }

 private:
    type data;

    static constexpr uint32_t maskGen(int maskBegin, int maskEnd) {
        uint32_t mask = 0;
        for (int i = maskBegin; i <= maskEnd; i++) {
            mask |= 1 << i;
        }
        return mask;
    }
    static_assert(maskGen(0, 0) == 0b01, "");
    static_assert(maskGen(0, 1) == 0b11, "");

    enum {
        bitfieldMask1 = maskGen(begin, end),
        valueMask1 = maskGen(0, end - begin),
        bitfieldMask2 = maskGen(begin2, end2),
        valueMask2 = maskGen(end - begin, end2 - begin2)
    };
};
}  // namespace microhal

#endif /* _MICROHAL_PORTS_BITFIELD_H_ */
