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

void showNativeRepresentation() {
    Q3_4 a(1.5f);
    Q3_4 b(2.0f);
    Q3_4 mul = a * b;
    diagChannel << lock << Debug << "Q3_4 value 1.5: " << a << ", binaryRepresentation: " << toBin((uint8_t)a.getNative()) << endl << unlock;
    diagChannel << lock << Debug << "Q3_4 value 2.0: " << b << ", binaryRepresentation: " << toBin((uint8_t)b.getNative()) << endl << unlock;
    diagChannel << lock << Debug << "Q3_4 value 1.5 * 2.0: " << mul << ", binaryRepresentation: " << toBin((uint8_t)mul.getNative()) << endl
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
