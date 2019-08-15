/*
 * bitfield.h
 *
 *  Created on: Aug 15, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_BITFIELD_H_
#define _MICROHAL_PORTS_BITFIELD_H_

namespace microhal {
template <typename type, int...>
class myBitfield;

template <typename type, int begin, int end>
class myBitfield<type, begin, end> {
 public:
    myBitfield &operator=(type value) {
        uint32_t tmp = data;
        tmp &= ~bitfieldMask;
        tmp |= (value & valueMask) << begin;
        data = tmp;
        return *this;
    }

    myBitfield &operator|=(type value) {
        uint32_t tmp = data;
        tmp |= (value & valueMask) << begin;
        data = tmp;
        return *this;
    }

    myBitfield &operator&=(type value) {
        uint32_t tmp = data;
        tmp &= (value | ~valueMask) << begin;
        data = tmp;
        return *this;
    }

    operator type() { return get(); }

    void set() { data |= bitfieldMask; }
    void clear() { data &= ~bitfieldMask; }
    type get() { return (data >> begin) & valueMask; }

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

    enum { bitfieldMask = maskGen(begin, end), valueMask = maskGen(0, end - begin) };
};

template <typename type, int begin, int end, int begin2, int end2>
class myBitfield<type, begin, end, begin2, end2> {
 public:
    myBitfield &operator=(type value) {
        uint32_t tmp = data;
        tmp &= ~(bitfieldMask1 | bitfieldMask2);
        tmp |= (value & valueMask1) << begin;
        tmp |= (value & valueMask2) << (begin2 - end);
        data = tmp;
        return *this;
    }

    myBitfield &operator|=(type value) {
        uint32_t tmp = data;
        tmp |= (value & valueMask1) << begin;
        tmp |= (value & valueMask2) << (begin2 - end);
        data = tmp;
        return *this;
    }

    myBitfield &operator&=(type value) {
        uint32_t tmp = data;
        tmp &= (value | ~valueMask1) << begin;
        tmp &= (value | ~valueMask2) << (begin2 - end);
        data = tmp;
        return *this;
    }

    operator type() { return get(); }

    void set() { data |= bitfieldMask1 | bitfieldMask2; }
    void clear() { data &= ~(bitfieldMask1 | bitfieldMask2); }
    type get() {
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
