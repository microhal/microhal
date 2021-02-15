/*
 * fixedPoint_test.cpp
 *
 *  Created on: Sep 29, 2019
 *      Author: pokas
 */

#include <cmath>
#include <thread>
#include "diagnostic/diagnostic.h"
#include "doctest.h"
#include "math/fixedPoint.h"

using namespace std::literals;
using namespace microhal;
using namespace microhal::diagnostic;
using namespace microhal::math;
using namespace std::string_view_literals;

using Q2_13 = FixedPoint<int16_t, 2, 13>;
using Q2_29 = FixedPoint<int32_t, 3, 28>;
using Q21_10 = FixedPoint<int32_t, 21, 10>;
using Q3_4 = FixedPoint<int8_t, 3, 4>;

namespace microhal {
namespace math {
doctest::String toString(const Q21_10& data) {
    std::array<char, 30> buffer;
    auto str = data.toString(buffer);
    return {str.data(), str.size()};
}
}  // namespace math
}  // namespace microhal

TEST_CASE("FixedPoint float construction") {
    Q21_10 zero = Q21_10(0.0f);
    CHECK(zero.getIntegerPart() == 0);
    CHECK(zero.getFractionalPart() == 0);
    CHECK(zero.getFractionalPart<uint8_t>() == 0);
    CHECK(zero.getFractionalPart<uint16_t>() == 0);

    Q21_10 one = Q21_10(1.0f);
    CHECK(one.getIntegerPart() == 1);
    CHECK(one.getFractionalPart() == 0);
    CHECK(one.getFractionalPart<uint8_t>() == 0);
    CHECK(one.getFractionalPart<uint16_t>() == 0);

    Q21_10 minusOne = Q21_10(-1.0f);
    CHECK(minusOne.getIntegerPart() == -1);
    CHECK(minusOne.getFractionalPart() == 0);
    CHECK(minusOne.getFractionalPart<uint8_t>() == 0);
    CHECK(minusOne.getFractionalPart<uint16_t>() == 0);

    Q21_10 half = Q21_10(0.5f);
    CHECK(half.getIntegerPart() == 0);
    CHECK(half.getFractionalPart() == 0x8000'0000);
    CHECK(half.getFractionalPart<uint8_t>() == 0b1000'0000);
    CHECK(half.getFractionalPart<uint16_t>() == 0b1000'0000'0000'0000);

    CHECK(minusOne != half);

    Q21_10 minusHalf = Q21_10(-0.5f);
    CHECK(minusHalf.getIntegerPart() == 0);
    CHECK(minusHalf.getFractionalPart() == 0x8000'0000);
    CHECK(minusHalf.getFractionalPart<uint8_t>() == 0b1000'0000);
    CHECK(minusHalf.getFractionalPart<uint16_t>() == 0b1000'0000'0000'0000);

    Q21_10 quater = Q21_10(0.25f);
    CHECK(quater.getIntegerPart() == 0);
    CHECK(quater.getFractionalPart() == 0x4000'0000);
    CHECK(quater.getFractionalPart<uint8_t>() == 0b0100'0000);
    CHECK(quater.getFractionalPart<uint16_t>() == 0b0100'0000'0000'0000);

    Q21_10 minusQuater = Q21_10(-0.25f);
    CHECK(minusQuater.getIntegerPart() == 0);
    CHECK(minusQuater.getFractionalPart() == 0x4000'0000);
    CHECK(minusQuater.getFractionalPart<uint8_t>() == 0b0100'0000);
    CHECK(minusQuater.getFractionalPart<uint16_t>() == 0b0100'0000'0000'0000);

    Q21_10 oneAndHalf = Q21_10(1.5f);
    CHECK(oneAndHalf.getIntegerPart() == 1);
    CHECK(oneAndHalf.getFractionalPart() == 0x8000'0000);
    CHECK(oneAndHalf.getFractionalPart<uint8_t>() == 0b1000'0000);
    CHECK(oneAndHalf.getFractionalPart<uint16_t>() == 0b1000'0000'0000'0000);

    Q21_10 minusOneAndHalf = Q21_10(-1.5f);
    CHECK(minusOneAndHalf.getIntegerPart() == -1);
    CHECK(minusOneAndHalf.getFractionalPart() == 0x8000'0000);
    CHECK(minusOneAndHalf.getFractionalPart<uint8_t>() == 0b1000'0000);
    CHECK(minusOneAndHalf.getFractionalPart<uint16_t>() == 0b1000'0000'0000'0000);

    Q21_10 minusTwo = Q21_10(-2.0f);
    CHECK(minusTwo.getIntegerPart() == -2);
    CHECK(minusTwo.getFractionalPart() == 0x0000'0000);
    CHECK(minusTwo.getFractionalPart<uint8_t>() == 0b0000'0000);
    CHECK(minusTwo.getFractionalPart<uint16_t>() == 0b0000'0000'0000'0000);

    Q21_10 minusTwoAndHalf = Q21_10(-2.5f);
    CHECK(minusTwoAndHalf.getIntegerPart() == -2);
    CHECK(minusTwoAndHalf.getFractionalPart() == 0x8000'0000);
    CHECK(minusTwoAndHalf.getFractionalPart<uint8_t>() == 0b1000'0000);
    CHECK(minusTwoAndHalf.getFractionalPart<uint16_t>() == 0b1000'0000'0000'0000);

    Q21_10 minusTwoAndQuater = Q21_10(-2.25f);
    CHECK(minusTwoAndQuater.getIntegerPart() == -2);
    CHECK(minusTwoAndQuater.getFractionalPart() == 0x4000'0000);
    CHECK(minusTwoAndQuater.getFractionalPart<uint8_t>() == 0b0100'0000);
    CHECK(minusTwoAndQuater.getFractionalPart<uint16_t>() == 0b0100'0000'0000'0000);

    Q21_10 minusFourAndQuater = Q21_10(-4.25f);
    CHECK(minusFourAndQuater.getIntegerPart() == -4);
    CHECK(minusFourAndQuater.getFractionalPart() == 0x4000'0000);
    CHECK(minusFourAndQuater.getFractionalPart<uint8_t>() == 0b0100'0000);
    CHECK(minusFourAndQuater.getFractionalPart<uint16_t>() == 0b0100'0000'0000'0000);
}

TEST_CASE("FixedPoint addition test") {
    Q21_10 zero = Q21_10(0.0f);
    Q21_10 one = Q21_10(1.0f);
    Q21_10 two = Q21_10(2.0f);
    Q21_10 minusOne = Q21_10(-1.0f);
    Q21_10 minusTwo = Q21_10(-2.0f);

    CHECK(zero + zero == zero);
    CHECK(one + one == two);
    auto tmp = one;
    tmp += one;
    CHECK(tmp == two);
    CHECK(minusOne + minusOne == minusTwo);
    CHECK(minusOne + one == zero);
    CHECK(minusOne + zero == minusOne);
    CHECK(one + zero == one);

    Q21_10 half = Q21_10(0.5f);
    Q21_10 minusHalf = Q21_10(-0.5f);

    CHECK(half + half == one);
    CHECK(half + minusHalf == zero);
    CHECK(half + zero == half);
}

TEST_CASE("FixedPoint subtraction test") {
    Q21_10 zero = Q21_10(0.0f);
    Q21_10 one = Q21_10(1.0f);
    Q21_10 minusOne = Q21_10(-1.0f);
    Q21_10 minusTwo = Q21_10(-2.0f);

    CHECK(zero - zero == zero);
    CHECK(one - one == zero);
    auto tmp = one;
    tmp -= one;
    CHECK(tmp == zero);
    CHECK(minusOne - minusOne == zero);
    CHECK(minusOne - one == minusTwo);
    CHECK(minusOne - zero == minusOne);
    CHECK(one - zero == one);

    Q21_10 half = Q21_10(0.5f);
    Q21_10 minusHalf = Q21_10(-0.5f);

    CHECK(half - half == zero);
    CHECK(half - minusHalf == one);
    CHECK(half - zero == half);
}

TEST_CASE("FixedPoint multiply test") {
    Q21_10 zero = Q21_10(0.0f);
    Q21_10 one = Q21_10(1.0f);
    Q21_10 minusOne = Q21_10(-1.0f);

    CHECK(zero * zero == zero);
    CHECK(one * one == one);
    CHECK(minusOne * minusOne == one);
    CHECK(minusOne * one == minusOne);
    CHECK(minusOne * zero == zero);
    CHECK(one * zero == zero);

    Q21_10 half = Q21_10(0.5f);
    Q21_10 minusHalf = Q21_10(-0.5f);
    Q21_10 quater = Q21_10(0.25f);

    CHECK(half * one == half);
    CHECK(half * minusOne == minusHalf);
    CHECK(half * half == quater);
}

TEST_CASE("FixedPoint operator >= test") {
    Q21_10 zero = Q21_10(0.0f);
    Q21_10 one = Q21_10(1.0f);
    Q21_10 minusOne = Q21_10(-1.0f);
    Q21_10 half = Q21_10(0.5f);
    Q21_10 minusHalf = Q21_10(-0.5f);

    CHECK(zero >= zero);
    CHECK(one >= one);
    CHECK(minusOne >= minusOne);
    CHECK(half >= half);
    CHECK(minusHalf >= minusHalf);

    CHECK_FALSE(minusOne >= one);
    CHECK(one >= minusOne);
    CHECK_FALSE(minusOne >= zero);
    CHECK(one >= zero);

    {
        CHECK(zero >= Q2_13(0.0f));
        CHECK(one >= Q2_13(1.0f));
        CHECK(minusOne >= Q2_13(-1.0f));
        CHECK(half >= Q2_13(0.5f));
        CHECK(minusHalf >= Q2_13(-0.5f));

        CHECK_FALSE(zero >= Q2_13(1.0f));
        CHECK_FALSE(one >= Q2_13(2.5f));
        CHECK_FALSE(minusOne >= Q2_13(0.0f));
        CHECK_FALSE(half >= Q2_13(0.75f));
        CHECK_FALSE(minusHalf >= Q2_13(0.0f));
    }
}

TEST_CASE("FixedPoint abs test") {
    Q21_10 zero = Q21_10(0.0f);
    Q21_10 one = Q21_10(1.0f);
    Q21_10 minusOne = Q21_10(-1.0f);

    CHECK(abs(zero) == zero);
    CHECK(abs(one) == one);
    CHECK(abs(minusOne) == one);
    CHECK(abs(minusOne) == abs(one));

    Q21_10 half = Q21_10(0.5f);
    Q21_10 minusHalf = Q21_10(-0.5f);

    CHECK(abs(minusHalf) == half);
}

TEST_CASE("FixedPoint cast test") {
    using Q10_10 = FixedPoint<int32_t, 10, 10>;
    using UQ10_10 = FixedPoint<uint32_t, 10, 10>;
    using Q10_8 = FixedPoint<int32_t, 10, 8>;
    using UQ10_8 = FixedPoint<uint32_t, 10, 8>;

    {
        Q21_10 zero1 = Q10_10(0.0f);
        Q21_10 zero2 = UQ10_10(0.0f);
        Q21_10 zero3 = Q10_8(0.0f);
        Q21_10 zero4 = UQ10_8(0.0f);
        CHECK(zero1 == Q21_10(0.0f));
        CHECK(zero2 == Q21_10(0.0f));
        CHECK(zero3 == Q21_10(0.0f));
        CHECK(zero4 == Q21_10(0.0f));

        Q21_10 one1 = Q10_10(1.0f);
        Q21_10 one2 = UQ10_10(1.0f);
        Q21_10 one3 = Q10_8(1.0f);
        Q21_10 one4 = UQ10_8(1.0f);
        CHECK(one1 == Q21_10(1.0f));
        CHECK(one2 == Q21_10(1.0f));
        CHECK(one3 == Q21_10(1.0f));
        CHECK(one4 == Q21_10(1.0f));

        Q21_10 half1 = Q10_10(0.5f);
        Q21_10 half2 = UQ10_10(0.5f);
        Q21_10 half3 = Q10_8(0.5f);
        Q21_10 half4 = UQ10_8(0.5f);
        CHECK(half1 == Q21_10(0.5f));
        CHECK(half2 == Q21_10(0.5f));
        CHECK(half3 == Q21_10(0.5f));
        CHECK(half4 == Q21_10(0.5f));

        Q21_10 oneAndHalf1 = Q10_10(1.5f);
        Q21_10 oneAndHalf2 = UQ10_10(1.5f);
        Q21_10 oneAndHalf3 = Q10_8(1.5f);
        Q21_10 oneAndHalf4 = UQ10_8(1.5f);
        CHECK(oneAndHalf1 == Q21_10(1.5f));
        CHECK(oneAndHalf2 == Q21_10(1.5f));
        CHECK(oneAndHalf3 == Q21_10(1.5f));
        CHECK(oneAndHalf4 == Q21_10(1.5f));

        Q21_10 minusOne1 = Q10_10(-1.0f);
        Q21_10 minusOne3 = Q10_8(-1.0f);
        CHECK(minusOne1 == Q21_10(-1.0f));
        CHECK(minusOne3 == Q21_10(-1.0f));

        Q21_10 minusHalf1 = Q10_10(-0.5f);
        Q21_10 minusHalf3 = Q10_8(-0.5f);
        CHECK(minusHalf1 == Q21_10(-0.5f));
        CHECK(minusHalf3 == Q21_10(-0.5f));

        Q21_10 minusOneAndHalf1 = Q10_10(-1.5f);
        Q21_10 minusOneAndHalf3 = Q10_8(-1.5f);
        CHECK(minusOneAndHalf1 == Q21_10(-1.5f));
        CHECK(minusOneAndHalf3 == Q21_10(-1.5f));
    }

    {
        Q21_10 zero1 = FixedPoint_cast<Q21_10>(Q10_10(0.0f));
        Q21_10 zero2 = FixedPoint_cast<Q21_10>(UQ10_10(0.0f));
        Q21_10 zero3 = FixedPoint_cast<Q21_10>(Q10_8(0.0f));
        Q21_10 zero4 = FixedPoint_cast<Q21_10>(UQ10_8(0.0f));
        CHECK(zero1 == Q21_10(0.0f));
        CHECK(zero2 == Q21_10(0.0f));
        CHECK(zero3 == Q21_10(0.0f));
        CHECK(zero4 == Q21_10(0.0f));

        Q21_10 one1 = FixedPoint_cast<Q21_10>(Q10_10(1.0f));
        Q21_10 one2 = FixedPoint_cast<Q21_10>(UQ10_10(1.0f));
        Q21_10 one3 = FixedPoint_cast<Q21_10>(Q10_8(1.0f));
        Q21_10 one4 = FixedPoint_cast<Q21_10>(UQ10_8(1.0f));
        CHECK(one1 == Q21_10(1.0f));
        CHECK(one2 == Q21_10(1.0f));
        CHECK(one3 == Q21_10(1.0f));
        CHECK(one4 == Q21_10(1.0f));

        Q21_10 half1 = FixedPoint_cast<Q21_10>(Q10_10(0.5f));
        Q21_10 half2 = FixedPoint_cast<Q21_10>(UQ10_10(0.5f));
        Q21_10 half3 = FixedPoint_cast<Q21_10>(Q10_8(0.5f));
        Q21_10 half4 = FixedPoint_cast<Q21_10>(UQ10_8(0.5f));
        CHECK(half1 == Q21_10(0.5f));
        CHECK(half2 == Q21_10(0.5f));
        CHECK(half3 == Q21_10(0.5f));
        CHECK(half4 == Q21_10(0.5f));

        Q21_10 oneAndHalf1 = FixedPoint_cast<Q21_10>(Q10_10(1.5f));
        Q21_10 oneAndHalf2 = FixedPoint_cast<Q21_10>(UQ10_10(1.5f));
        Q21_10 oneAndHalf3 = FixedPoint_cast<Q21_10>(Q10_8(1.5f));
        Q21_10 oneAndHalf4 = FixedPoint_cast<Q21_10>(UQ10_8(1.5f));
        CHECK(oneAndHalf1 == Q21_10(1.5f));
        CHECK(oneAndHalf2 == Q21_10(1.5f));
        CHECK(oneAndHalf3 == Q21_10(1.5f));
        CHECK(oneAndHalf4 == Q21_10(1.5f));

        Q21_10 minusOne1 = FixedPoint_cast<Q21_10>(Q10_10(-1.0f));
        Q21_10 minusOne3 = FixedPoint_cast<Q21_10>(Q10_8(-1.0f));
        CHECK(minusOne1 == Q21_10(-1.0f));
        CHECK(minusOne3 == Q21_10(-1.0f));

        Q21_10 minusHalf1 = FixedPoint_cast<Q21_10>(Q10_10(-0.5f));
        Q21_10 minusHalf3 = FixedPoint_cast<Q21_10>(Q10_8(-0.5f));
        CHECK(minusHalf1 == Q21_10(-0.5f));
        CHECK(minusHalf3 == Q21_10(-0.5f));

        Q21_10 minusOneAndHalf1 = FixedPoint_cast<Q21_10>(Q10_10(-1.5f));
        Q21_10 minusOneAndHalf3 = FixedPoint_cast<Q21_10>(Q10_8(-1.5f));
        CHECK(minusOneAndHalf1 == Q21_10(-1.5f));
        CHECK(minusOneAndHalf3 == Q21_10(-1.5f));
    }

    {
        Q10_8 zero1 = FixedPoint_cast<Q10_8>(Q21_10(0.0f));
        Q10_8 zero2 = FixedPoint_cast<Q10_8>(UQ10_10(0.0f));
        CHECK(zero1 == Q10_8(0.0f));
        CHECK(zero2 == Q10_8(0.0f));

        Q10_8 one1 = FixedPoint_cast<Q10_8>(Q21_10(1.0f));
        Q10_8 one2 = FixedPoint_cast<Q10_8>(UQ10_10(1.0f));
        CHECK(one1 == Q10_8(1.0f));
        CHECK(one2 == Q10_8(1.0f));

        Q10_8 half1 = FixedPoint_cast<Q10_8>(Q21_10(0.5f));
        Q10_8 half2 = FixedPoint_cast<Q10_8>(UQ10_10(0.5f));
        CHECK(half1 == Q10_8(0.5f));
        CHECK(half2 == Q10_8(0.5f));

        Q10_8 oneAndHalf1 = FixedPoint_cast<Q10_8>(Q21_10(1.5f));
        Q10_8 oneAndHalf2 = FixedPoint_cast<Q10_8>(UQ10_10(1.5f));
        CHECK(oneAndHalf1 == Q10_8(1.5f));
        CHECK(oneAndHalf2 == Q10_8(1.5f));

        Q10_8 minusOne1 = FixedPoint_cast<Q10_8>(Q21_10(-1.0f));
        CHECK(minusOne1 == Q10_8(-1.0f));

        Q10_8 minusHalf1 = FixedPoint_cast<Q10_8>(Q21_10(-0.5f));
        CHECK(minusHalf1 == Q10_8(-0.5f));

        Q10_8 minusOneAndHalf1 = FixedPoint_cast<Q10_8>(Q21_10(-1.5f));
        CHECK(minusOneAndHalf1 == Q10_8(-1.5f));
    }

    {
        using Q3_4 = FixedPoint<int8_t, 3, 4>;
        using UQ3_5 = FixedPoint<uint8_t, 3, 5>;

        using Q3_12 = FixedPoint<uint16_t, 3, 12>;
        using UQ3_13 = FixedPoint<uint16_t, 3, 13>;

        using Q3_28 = FixedPoint<uint32_t, 3, 28>;
        using UQ3_29 = FixedPoint<uint32_t, 3, 29>;

        Q3_4 seven = FixedPoint_cast<Q3_4>(UQ3_5(7.0f));
        CHECK(seven == Q3_4(7.0f));

        Q3_12 seven2 = FixedPoint_cast<Q3_12>(UQ3_13(7.0f));
        CHECK(seven2 == Q3_12(7.0f));

        Q3_28 seven3 = FixedPoint_cast<Q3_28>(UQ3_29(7.0f));
        CHECK(seven3 == Q3_28(7.0f));
    }
}

TEST_CASE("FixedPoint toString test") {
    Q21_10 zero = Q21_10(0.0f);
    Q21_10 one = Q21_10(1.0f);
    Q21_10 minusOne = Q21_10(-1.0f);
    Q21_10 half = Q21_10(0.5f);
    Q21_10 minusHalf = Q21_10(-0.5f);
    Q21_10 quater = Q21_10(0.25f);
    Q21_10 minusQuater = Q21_10(-0.25f);
    Q21_10 smalestQuant = Q21_10::fromNative(1);
    Q21_10 minusSmalestQuant = Q21_10::fromNative(-1);
    Q21_10 oneNative = Q21_10::fromNative(0b100'0000'0000);
    Q21_10 halfNative = Q21_10::fromNative(0b10'0000'0000);

    std::array<char, 30> buffer;
    CHECK(zero.toString(buffer) == "0.0000000000000000"sv);
    CHECK(one.toString(buffer) == "1.0000000000000000"sv);
    CHECK(minusOne.toString(buffer) == "-1.0000000000000000"sv);

    CHECK(half.toString(buffer) == "0.5000000000000000"sv);
    CHECK(minusHalf.toString(buffer) == "-0.5000000000000000"sv);
    CHECK(quater.toString(buffer) == "0.2500000000000000"sv);
    CHECK(minusQuater.toString(buffer) == "-0.2500000000000000"sv);

    CHECK(smalestQuant.toString(buffer) == "0.0009765625000000"sv);
    CHECK(minusSmalestQuant.toString(buffer) == "-0.0009765625000000"sv);

    CHECK(oneNative.toString(buffer) == "1.0000000000000000"sv);
    CHECK(halfNative.toString(buffer) == "0.5000000000000000"sv);
}

TEST_CASE("FixedPoint two parameter construction, from integer and fractional part") {
    Q21_10 zero = Q21_10(0.0f);
    Q21_10 zeroTwoParam = Q21_10(0, 0);

    CHECK(zero == zeroTwoParam);
    CHECK(zeroTwoParam.getIntegerPart() == 0);
    CHECK(zeroTwoParam.getFractionalPart() == 0);

    Q21_10 one = Q21_10(1, 0);
    CHECK(one == Q21_10(1.0f));
    CHECK(one.getIntegerPart() == 1);
    CHECK(one.getFractionalPart() == 0);

    Q21_10 minusOne = Q21_10(-1, 0);
    CHECK(minusOne == Q21_10(-1.0f));
    CHECK(minusOne.getIntegerPart() == -1);
    CHECK(minusOne.getFractionalPart() == 0);

    Q21_10 half = Q21_10(0, 0x8000'0000);
    CHECK(half == Q21_10(0.5f));
    CHECK(half.getIntegerPart() == 0);
    CHECK(half.getFractionalPart() == 0x8000'0000);

    Q21_10 minusHalf = Q21_10(Q21_10::NegativeFractional, 0x8000'0000);
    CHECK(minusHalf == Q21_10(-0.5f));
    CHECK(minusHalf.getIntegerPart() == 0);
    CHECK(minusHalf.getFractionalPart() == 0x8000'0000);

    Q21_10 oneAndHalf = Q21_10(1, 0x8000'0000);
    CHECK(oneAndHalf == Q21_10(1.5f));
    CHECK(oneAndHalf.getIntegerPart() == 1);
    CHECK(oneAndHalf.getFractionalPart() == 0x8000'0000);

    Q21_10 minusOneAndHalf = Q21_10(-1, 0x8000'0000);
    CHECK(minusOneAndHalf == Q21_10(-1.5f));
    CHECK(minusOneAndHalf.getIntegerPart() == -1);
    CHECK(minusOneAndHalf.getFractionalPart() == 0x8000'0000);
}

void showNativeRepresentation() {
    Q3_4 half(0.5f);
    Q3_4 minusHalf(-0.5f);
    Q3_4 minusOne(-1.0f);
    Q3_4 minusOneAndHalf(-1.5f);
    Q3_4 minusQuater(-0.25f);
    Q3_4 minusOneAnd75(-1.75f);
    Q3_4 minusOneAndQuater(-1.25f);
    Q3_4 minusTwo(-2.0f);
    Q3_4 minusTwoAndHalf(-2.5f);
    Q3_4 a(1.5f);
    Q3_4 b(2.0f);
    Q3_4 mul = a * b;
    diagChannel << lock << Debug << "Q3_4 value 0.5: " << half << ", binaryRepresentation: " << toBin((uint8_t)half.getNative()) << endl << unlock;
    diagChannel << lock << Debug << "Q3_4 value -1.5: " << minusOneAndHalf
                << ", binaryRepresentation: " << toBin((uint8_t)minusOneAndHalf.getNative()) << endl
                << unlock;
    diagChannel << lock << Debug << "Q3_4 value -1.75: " << minusOneAnd75 << ", binaryRepresentation: " << toBin((uint8_t)minusOneAnd75.getNative())
                << endl
                << unlock;
    diagChannel << lock << Debug << "Q3_4 value -0.25: " << minusQuater << ", binaryRepresentation: " << toBin((uint8_t)minusQuater.getNative())
                << endl
                << unlock;
    diagChannel << lock << Debug << "Q3_4 value -1.25: " << minusOneAndQuater
                << ", binaryRepresentation: " << toBin((uint8_t)minusOneAndQuater.getNative()) << endl
                << unlock;
    diagChannel << lock << Debug << "Q3_4 value -0.5: " << minusHalf << ", binaryRepresentation: " << toBin((uint8_t)minusHalf.getNative()) << endl
                << unlock;
    diagChannel << lock << Debug << "Q3_4 value -1.0: " << minusOne << ", binaryRepresentation: " << toBin((uint8_t)minusOne.getNative()) << endl
                << unlock;
    diagChannel << lock << Debug << "Q3_4 value 1.5: " << a << ", binaryRepresentation: " << toBin((uint8_t)a.getNative()) << endl << unlock;
    diagChannel << lock << Debug << "Q3_4 value 2.0: " << b << ", binaryRepresentation: " << toBin((uint8_t)b.getNative()) << endl << unlock;
    diagChannel << lock << Debug << "Q3_4 value 1.5 * 2.0: " << mul << ", binaryRepresentation: " << toBin((uint8_t)mul.getNative()) << endl
                << unlock;

    diagChannel << lock << Debug << "Q3_4 value -2.0: " << minusTwo << ", binaryRepresentation: " << toBin((uint8_t)minusTwo.getNative()) << endl
                << unlock;
    diagChannel << lock << Debug << "Q3_4 value -2.5: " << minusTwoAndHalf
                << ", binaryRepresentation: " << toBin((uint8_t)minusTwoAndHalf.getNative()) << endl
                << unlock;
}

void showValues() {
    diagChannel << lock << Debug << "Q1_14 1.0 " << Q1_14{1.0f} << endl << unlock;
    diagChannel << lock << Debug << "Q1_14 0.5 " << Q1_14{0.5f} << endl << unlock;
    diagChannel << lock << Debug << "Q1_14 0.25 " << Q1_14{0.25f} << endl << unlock;
    diagChannel << lock << Debug << "Q1_14 0.125 " << Q1_14{0.125f} << endl << unlock;
    diagChannel << lock << Debug << "Q1_14 0.0625 " << Q1_14{0.0625f} << endl << unlock;
    diagChannel << lock << Debug << "Q1_14 0.03125 " << Q1_14{0.03125f} << endl << unlock;
    diagChannel << lock << Debug << "Q1_14 0.015625 " << Q1_14{0.015625f} << endl << unlock;
}

void showOperationValues() {
    Q1_14 a(1.0f);
    Q1_14 b(1.25f);
    Q1_14 c(0.125f);

    diagChannel << Debug << "a 1.0: " << a << endl;
    diagChannel << Debug << "b 1.25: " << b << endl;
    diagChannel << Debug << "c 0.125: " << c << endl;
    diagChannel << Debug << "b + c: " << b + c << endl;
    diagChannel << Debug << "b - c: " << b - c << endl;
    diagChannel << Debug << "a * c: " << a * c << endl;
    diagChannel << Debug << "c / b: " << c / b << endl;
}

void fixedPoint_test() {
    Q1_14 a(1.0f);
    Q1_14 b(1.25f);

    diagChannel << Debug << endl << "----------------------------------------" << endl;
    showNativeRepresentation();
    diagChannel << Debug << endl << "------------------" << endl;
    showValues();
    diagChannel << Debug << endl << "------------------" << endl;
    showOperationValues();

    diagChannel << Debug << "Q1_14 min value " << Q1_14::min() << ", max value: " << Q1_14::max() << endl;
    diagChannel << Debug << "Q2_13 min value " << Q2_13::min() << ", max value: " << Q2_13::max() << endl;
    diagChannel << Debug << "Q2_29 min value " << Q2_29::min() << ", max value: " << Q2_29::max() << endl;

    diagChannel << Debug << "saturate add: 1.5 + 1.8: " << Q1_14(1.5f) + Q1_14(1.8f) << endl;
    diagChannel << Debug << "saturate add: -1.5 - 1.8: " << Q1_14(-1.5f) + Q1_14(-1.8f) << endl;
    diagChannel << Debug << "saturate div: 1.0 / 0.1: " << Q1_14(1.0f) / Q1_14(0.1f) << endl;
    diagChannel << Debug << "1.0 * -0.1: " << Q1_14(1.0f) / Q1_14(-1.0f) << endl;
    diagChannel << Debug << "-1.0 * -0.1: " << Q1_14(-1.0f) / Q1_14(-1.0f) << endl;
    diagChannel << Debug << "1.5 * 2: " << Q2_29(1.5f) * Q2_29(2.0f) << endl;
}
