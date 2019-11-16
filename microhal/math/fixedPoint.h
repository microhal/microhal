/*
 * fixedPoint.h
 *
 *  Created on: Sep 29, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_MATH_FIXEDPOINT_H_
#define _MICROHAL_MATH_FIXEDPOINT_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <limits>
#include <type_traits>
#include "diagnostic/diagnostic.h"

namespace microhal {
enum class FixedPointImplementation { Generic, CortexM4 };
constexpr FixedPointImplementation fixedPointImplementation = FixedPointImplementation::Generic;
namespace math {

/* ************************************************************************************************
 * detail implementation
 */
namespace fixedPointDetail {
// clang-format off
template <class> struct doubleSize;
template <> struct doubleSize<int8_t>   { using type = int16_t;  };
template <> struct doubleSize<uint8_t>  { using type = uint16_t; };
template <> struct doubleSize<int16_t>  { using type = int32_t;  };
template <> struct doubleSize<uint16_t> { using type = uint32_t; };
template <> struct doubleSize<int32_t>  { using type = int64_t;  };
template <> struct doubleSize<uint32_t> { using type = uint64_t; };
// clang-format on

constexpr std::array<uint64_t, 16> generateFractionalToDecimalConversionTable() {
    constexpr const size_t fractionaBitsCount = 16;
    uint64_t numerator = 1;
    std::array<uint64_t, fractionaBitsCount> table = {};
    for (size_t i = 0; i < fractionaBitsCount; i++) {
        numerator *= 10;
    }

    for (size_t i = 0; i < fractionaBitsCount; i++) {
        uint32_t denominator = 1 << (i + 1);
        table[i] = numerator / denominator;
    }
    return table;
}

class FixedPointBase {
 protected:
    static const constexpr auto toDecConverter = fixedPointDetail::generateFractionalToDecimalConversionTable();

    static uint64_t fracToDec(uint32_t fractional, size_t fractionalBitsCount) {
        uint64_t decimalBase = 0;
        for (size_t i = 0; i < std::min(fractionalBitsCount, size_t{toDecConverter.size()}); i++) {
            if (fractional & (1 << (fractionalBitsCount - 1))) {
                decimalBase += toDecConverter[i];
            }
            fractional = fractional << 1;
        }
        return decimalBase;
    }
};
}  // namespace fixedPointDetail

/*************************************************************************************************
 * @class FixedPoint
 * FixedPoint class provide fixed point arithmetic functions.
 */
template <typename dataType, size_t integerBits, size_t fractionalBits>
class FixedPoint : private fixedPointDetail::FixedPointBase {
    static_assert(std::is_floating_point_v<dataType> == false, "Underlying data type have to be integer.");
    static_assert(std::numeric_limits<dataType>::digits >= integerBits + fractionalBits, "dataType template parameter is to small.");
    static_assert(fractionalBits > 0, "fractionalBits template parameter have to be larger than 0.");

    using doubleSizeType = typename fixedPointDetail::doubleSize<dataType>::type;
    constexpr explicit FixedPoint(dataType a) : data(a) {}

 public:
    FixedPoint() = default;
    constexpr FixedPoint(float value) : data(value * (1 << fractionalBits)) {}
    constexpr FixedPoint(dataType integerPart, dataType fractionalPart) : data((integerPart << fractionalBits) | fractionalPart) {}

    static constexpr FixedPoint min() noexcept { return std::numeric_limits<dataType>::min(); }
    static constexpr FixedPoint max() noexcept { return std::numeric_limits<dataType>::max(); }
    static constexpr FixedPoint zero() noexcept { return 0; }

    FixedPoint operator+(FixedPoint b) const { return FixedPoint(addSaturate(data, b.data)); }
    FixedPoint operator-(const FixedPoint& b) const { return FixedPoint(substractSaturate(data, b.data)); }
    FixedPoint operator*(const FixedPoint& b) const { return FixedPoint(mulSaturate(data, b.data)); }
    FixedPoint operator/(const FixedPoint& b) const { return FixedPoint(divSaturate(data, b.data)); }
    bool operator==(const FixedPoint& b) const { return data == b.data; }
    bool operator>(const FixedPoint& b) const { return data > b.data; }
    bool operator<(const FixedPoint& b) const { return data < b.data; }

    bool operator>=(const FixedPoint& b) const { return data >= b.data; }
    bool operator<=(const FixedPoint& b) const { return data <= b.data; }

    FixedPoint& operator+=(const FixedPoint& b) {
        data = addSaturate(data, b.data);
        return *this;
    }
    FixedPoint& operator-=(const FixedPoint& b) {
        data = substractSaturate(data, b.data);
        return *this;
    }

    FixedPoint& operator*=(const FixedPoint& b) {
        data = mulSaturate(data, b.data);
        return *this;
    }

    FixedPoint& operator/=(const FixedPoint& b) {
        data = divSaturate(data, b.data);
        return *this;
    }

    float toFloat() const {
        float data = this->data;
        data /= 1 << fractionalBits;
        return data;
    }

    std::string_view toString(std::array<char, 30>& buffer) const {
        using UnsignedData = std::make_unsigned_t<dataType>;
        UnsignedData unsignedData = std::is_signed_v<dataType> ? std::abs(data) : data;
        auto begin = buffer.begin();
        if constexpr (std::is_signed_v<dataType>) {
            if (data < 0) {
                buffer[0] = '-';
                begin++;
            }
        }
        UnsignedData integer = unsignedData >> fractionalBits;
        auto result = std::to_chars(begin, buffer.end(), integer);
        *result.ptr++ = '.';
        // convert fractional part
        std::array<char, toDecConverter.size()> tmpBuffer;
        auto fractionalResult = std::to_chars(tmpBuffer.begin(), tmpBuffer.end(), fracToDec(unsignedData, fractionalBits));
        size_t fractionalLen = fractionalResult.ptr - tmpBuffer.begin();
        for (uint32_t i = 0; i < toDecConverter.size() - fractionalLen; i++) {
            *result.ptr++ = '0';
        }
        std::copy_n(tmpBuffer.begin(), fractionalLen, result.ptr);
        result.ptr += fractionalLen;

        return {buffer.data(), size_t(result.ptr - buffer.begin())};
    }

    dataType getNative() const { return data; }
    static FixedPoint<dataType, integerBits, fractionalBits> fromNative(dataType data) {
        FixedPoint<dataType, integerBits, fractionalBits> tmp;
        tmp.data = data;
        return tmp;
    }

 private:
    dataType data;

    dataType addSaturate(dataType a, dataType b) const {
        doubleSizeType sum = doubleSizeType(a) + b;
        return clamp(sum);
    }

    dataType substractSaturate(dataType a, dataType b) const {
        doubleSizeType diff = doubleSizeType{a} - b;
        return clamp(diff);
    }

    dataType mulSaturate(dataType a, dataType b) const {
        doubleSizeType mul = (doubleSizeType{a} * b);
        mul >>= fractionalBits;
        return clamp(mul);
    }

    dataType divSaturate(dataType a, dataType b) const {
        doubleSizeType tmp = doubleSizeType{a} << fractionalBits;
        if ((tmp >= 0 && b >= 0) || (tmp < 0 && b < 0)) {
            tmp += b >> 1;
        } else {
            tmp -= b >> 1;
        }
        doubleSizeType div = tmp / b;
        return clamp(div);
    }

    doubleSizeType clamp(doubleSizeType a) const {
        return std::clamp(a, doubleSizeType{std::numeric_limits<dataType>::min()}, doubleSizeType(std::numeric_limits<dataType>::max()));
    }
};

// template <typename dataAType, size_t integerABits, size_t fractionalABits, typename dataBType, size_t integerBBits, size_t fractionalBBits>
// FixedPoint<dataAType, integerABits, fractionalABits> operator+(FixedPoint<dataAType, integerABits, fractionalABits> a,
//                                                               FixedPoint<dataBType, integerBBits, fractionalBBits> b) {
//    return a.getNative() + b.getNative();
//}

template <diagnostic::LogLevel level, bool B, typename T, size_t I, size_t F>
inline diagnostic::LogLevelChannel<level, B> operator<<(diagnostic::LogLevelChannel<level, B> logChannel, math::FixedPoint<T, I, F> a) {
    std::array<char, 30> buffer;
    return logChannel << a.toString(buffer);
}

using Q1_14 = FixedPoint<int16_t, 1, 14>;
using Q1_30 = FixedPoint<int32_t, 1, 31>;
}  // namespace math
}  // namespace microhal

#endif /* _MICROHAL_MATH_FIXEDPOINT_H_ */
