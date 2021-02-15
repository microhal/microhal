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

template <typename dataType, size_t integerBits, size_t fractionalBits>
class FixedPoint;
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

template <size_t bytes>
struct ResultType {
    static_assert(bytes <= 32);
    using type = typename std::conditional<bytes <= 8, uint8_t, typename std::conditional<bytes <= 16, uint16_t, uint32_t>::type>::type;
};

constexpr size_t _bytesSum(size_t integerABits, size_t integerBBits, size_t fractionalABits, size_t fractionalBBits, bool isSigned) {
    size_t signedBits = 0;
    if (isSigned) signedBits = 1;
    return std::max(integerABits, integerBBits) + std::max(fractionalABits, fractionalBBits) + signedBits;
}

template <typename dataType, size_t integerBits, size_t fractionalBits, typename dataAType, size_t integerABits, size_t fractionalABits>
auto FixedPoint_cast_impl(FixedPoint<dataAType, integerABits, fractionalABits> a) {
    using ResultType = FixedPoint<dataType, integerBits, fractionalBits>;

    using TmpType = typename std::common_type<dataType, dataAType>::type;
    TmpType tmp = a.getNative();
    if constexpr (fractionalBits >= fractionalABits) {
        tmp <<= (fractionalBits - fractionalABits);
        return ResultType::fromNative(tmp);
    } else {
        tmp >>= (fractionalABits - fractionalBits);
        return ResultType::fromNative(tmp);
    }
}

template <typename DataType, typename FractionalType, size_t integerBits, size_t fractionalBits>
constexpr DataType makeData(DataType integerPart, FractionalType fractionalPart) {
    using UnsignedDataType = typename std::make_unsigned<DataType>::type;
    constexpr auto fractionalPartBits = std::numeric_limits<FractionalType>::digits;

    if constexpr (std::is_signed_v<DataType>) {
        if (integerPart == -1 && fractionalPart != 0) {
            DataType integer = UnsignedDataType(integerPart) << (fractionalBits + 1);
            UnsignedDataType fractional = UnsignedDataType(fractionalPart) >> (fractionalPartBits - fractionalBits);
            return integer | fractional;
        } else {
            DataType integer = UnsignedDataType(integerPart) << (fractionalBits);
            UnsignedDataType fractional = UnsignedDataType(fractionalPart) >> (fractionalPartBits - fractionalBits);
            return integer | fractional;
        }
    } else {
        DataType integer = UnsignedDataType(integerPart) << (fractionalBits);
        UnsignedDataType fractional = UnsignedDataType(fractionalPart) >> (fractionalPartBits - fractionalBits);
        return integer | fractional;
    }

}  // namespace fixedPointDetail

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
    using UnsignedDataType = typename std::make_unsigned<dataType>::type;
    constexpr explicit FixedPoint(dataType a) : data(a) {}

 public:
    typedef enum { NegativeFractional } NegativeFractionalType;
    using UnderlyingType = dataType;
    static constexpr const size_t fractionalBitsCount = fractionalBits;
    static constexpr const size_t integerBitsCount = integerBits;

    FixedPoint() = default;
    constexpr FixedPoint(float value) : data(value * (1 << fractionalBits)) {}
    explicit constexpr FixedPoint(dataType integerPart, UnsignedDataType fractionalPart)
        : data(fixedPointDetail::makeData<dataType, UnsignedDataType, integerBits, fractionalBits>(integerPart, fractionalPart)) {}
    explicit constexpr FixedPoint(NegativeFractionalType, UnsignedDataType fractionalPart)
        : data(fixedPointDetail::makeData<dataType, UnsignedDataType, integerBits, fractionalBits>(0, fractionalPart) * -1) {}

    template <typename dataAType, size_t integerABits, size_t fractionalABits>
    constexpr FixedPoint(FixedPoint<dataAType, integerABits, fractionalABits> a)
        : data(static_cast<dataType>(a.getNative()) << (fractionalBits - fractionalABits)) {
        static_assert(
            integerABits <= integerBits,
            "Error, you are trying to convert FixedPoint variable with higher integral bytes count into FixedPoint with smaller integral bytes "
            "count, "
            "this could cause lost of data by truncating integral part of your data. If you want to convert types use FixedPoint_cast instead.");
        static_assert(fractionalABits <= fractionalBits);
    }

    static constexpr FixedPoint min() noexcept { return std::numeric_limits<dataType>::min(); }
    static constexpr FixedPoint max() noexcept { return std::numeric_limits<dataType>::max(); }
    static constexpr FixedPoint zero() noexcept { return 0; }

    template <typename ReturnType = UnsignedDataType>
    ReturnType getFractionalPart() const {
        static_assert(std::is_signed_v<ReturnType> == false, "Retrun type has to be unsigned.");

        using UnsignedDataType = typename std::make_unsigned<dataType>::type;
        using Type = typename std::conditional<(sizeof(ReturnType) > sizeof(UnsignedDataType)), ReturnType, UnsignedDataType>::type;

        if constexpr (std::is_signed_v<dataType>) {
            Type udata = data;
            if (data < 0) {
                udata = data * -1;
            }

            if constexpr (std::numeric_limits<ReturnType>::digits >= fractionalBits) {
                return udata << (std::numeric_limits<ReturnType>::digits - fractionalBits);
            } else {
                return udata >> (fractionalBits - std::numeric_limits<ReturnType>::digits);
            }
        } else {
            Type udata = data;
            if constexpr (std::numeric_limits<ReturnType>::digits >= fractionalBits) {
                return udata << (std::numeric_limits<ReturnType>::digits - fractionalBits);
            } else {
                return udata >> (fractionalBits - std::numeric_limits<ReturnType>::digits);
            }
        }
    }

    dataType getIntegerPart() const {
        if constexpr (std::is_signed_v<dataType>) {
            if (data < 0) {
                UnsignedDataType tmp = ~(data - 1);
                tmp >>= fractionalBits;
                return dataType(~tmp) + 1;
                // UnsignedDataType tmp = data * -1;
                // tmp >>= fractionalBits;
                // return dataType(tmp) * -1;
            } else {
                return data >> fractionalBits;
            }
        } else {
            return data >> fractionalBits;
        }
    }

    FixedPoint operator+(FixedPoint b) const { return FixedPoint(addSaturate(data, b.data)); }
    FixedPoint operator-(const FixedPoint& b) const { return FixedPoint(substractSaturate(data, b.data)); }
    FixedPoint operator*(const FixedPoint& b) const { return FixedPoint(mulSaturate(data, b.data)); }
    FixedPoint operator/(const FixedPoint& b) const { return FixedPoint(divSaturate(data, b.data)); }
    bool operator==(const FixedPoint& b) const { return data == b.data; }
    template <typename DataBType, size_t integerBBits, size_t fractionalBBits>
    bool operator==(const FixedPoint<DataBType, integerBBits, fractionalBBits>& b) const {
        if constexpr (fractionalBBits >= fractionalBits) {
            DataBType tmp = b.getNative() >> (fractionalBBits - fractionalBits);
            return data == tmp;
        } else {
            return data == b.getNative();
        }
    }
    bool operator!=(const FixedPoint& b) const { return data != b.data; }
    bool operator>(const FixedPoint& b) const { return data > b.data; }
    bool operator<(const FixedPoint& b) const { return data < b.data; }

    bool operator>=(const FixedPoint& b) const { return data >= b.data; }
    template <typename DataBType, size_t integerBBits, size_t fractionalBBits>
    bool operator>=(const FixedPoint<DataBType, integerBBits, fractionalBBits>& b) const {
        if constexpr (fractionalBBits >= fractionalBits) {
            DataBType tmp = b.getNative() >> (fractionalBBits - fractionalBits);
            return data >= tmp;
        } else {
            return data >= b.getNative();
        }
    }
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

    double toDouble() const {
        double data = this->data;
        data /= 1 << fractionalBits;
        return data;
    }

    std::string_view toString(std::array<char, 30>& buffer) const {
        using UnsignedData = std::make_unsigned_t<dataType>;
        UnsignedData unsignedData = std::is_signed_v<dataType> ? data < 0 ? -data : data : data;
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

    static constexpr dataType integerMask() {
        using SignedDataType = typename std::make_signed<dataType>::type;
        SignedDataType mask = -1;
        UnsignedDataType tmp = mask;
        return tmp << (fractionalBits);
    }
};

template <typename dataAType, size_t integerABits, size_t fractionalABits, typename dataBType, size_t integerBBits, size_t fractionalBBits>
auto operator+(FixedPoint<dataAType, integerABits, fractionalABits> a, FixedPoint<dataBType, integerBBits, fractionalBBits> b) {
    using ResultUnderlyingType =
        typename std::conditional<std::is_signed<dataAType>::value | std::is_signed<dataBType>::value,
                                  typename std::make_signed<typename fixedPointDetail::ResultType<fixedPointDetail::_bytesSum(
                                      integerABits, integerBBits, fractionalABits, fractionalBBits, true)>::type>::type,
                                  typename fixedPointDetail::ResultType<fixedPointDetail::_bytesSum(integerABits, integerBBits, fractionalABits,
                                                                                                    fractionalBBits, false)>::type>::type;
    using ResultFixedPointType = FixedPoint<ResultUnderlyingType, std::max(integerABits, integerBBits), std::max(fractionalABits, fractionalBBits)>;

    ResultFixedPointType aResult(a);
    ResultFixedPointType bResult(b);
    return aResult + bResult;
}

template <typename dataAType, size_t integerABits, size_t fractionalABits, typename dataBType, size_t integerBBits, size_t fractionalBBits>
auto operator-(FixedPoint<dataAType, integerABits, fractionalABits> a, FixedPoint<dataBType, integerBBits, fractionalBBits> b) {
    using ResultUnderlyingType =
        typename std::conditional<std::is_signed<dataAType>::value | std::is_signed<dataBType>::value,
                                  typename std::make_signed<typename fixedPointDetail::ResultType<fixedPointDetail::_bytesSum(
                                      integerABits, integerBBits, fractionalABits, fractionalBBits, true)>::type>::type,
                                  typename fixedPointDetail::ResultType<fixedPointDetail::_bytesSum(integerABits, integerBBits, fractionalABits,
                                                                                                    fractionalBBits, false)>::type>::type;
    using ResultFixedPointType = FixedPoint<ResultUnderlyingType, std::max(integerABits, integerBBits), std::max(fractionalABits, fractionalBBits)>;

    ResultFixedPointType aResult(a);
    ResultFixedPointType bResult(b);
    return aResult - bResult;
}

template <typename dataAType, size_t integerABits, size_t fractionalABits>
FixedPoint<dataAType, integerABits, fractionalABits> abs(FixedPoint<dataAType, integerABits, fractionalABits> a) {
    using ReturnType = FixedPoint<dataAType, integerABits, fractionalABits>;
    if constexpr (std::is_signed_v<dataAType>) {
        constexpr const ReturnType minusOne(-1, 0);
        if (a < a.zero()) {
            return a * minusOne;
        } else {
            return a;
        }
    } else {
        return a;
    }
}

template <typename ResultType, typename dataAType, size_t integerABits, size_t fractionalABits>
ResultType FixedPoint_cast(FixedPoint<dataAType, integerABits, fractionalABits> a) {
    return fixedPointDetail::FixedPoint_cast_impl<typename ResultType::UnderlyingType, ResultType::integerBitsCount, ResultType::fractionalBitsCount>(
        a);
}

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
