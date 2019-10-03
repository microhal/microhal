/*
 * iir.h
 *
 *  Created on: Sep 24, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_MATH_IIR_H_
#define _MICROHAL_MATH_IIR_H_

#include <algorithm>
#include <array>

namespace microhal {
namespace math {

namespace detail {
template <typename T>
class has_multiplyAccumulate {
    typedef char yes_type;
    typedef long no_type;
    template <typename U>
    static yes_type test(decltype(&U::multiplyAccumulate));
    template <typename U>
    static no_type test(...);

 public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
};
}  // namespace detail

/**
 * https://en.wikipedia.org/wiki/Digital_biquad_filter
 * @note 'a' filter coefficients have to be manually negated.
 */
enum filterType { DirectForm1, DirectForm2, TransposedForm1, TransposedForm2 };

template <typename T, filterType type, size_t length>
class IIR;

template <typename T, size_t filterOrder>
class IIR<T, DirectForm1, filterOrder> {
    static_assert(filterOrder > 0, "Filter order have to be at least 1");
    static_assert(filterOrder < 128, "Filter order have to be lower than 128");

 public:
    using aCoefficients = std::array<const T, filterOrder>;
    using bCoefficients = std::array<const T, filterOrder + 1>;

    IIR(const aCoefficients& a, const bCoefficients& b) : a(a), b(b) {
        inputSamples.fill(0.0f);
        outputSamples.fill(0.0f);
    }

    T filter(T newSample) {
        // inputSamples[0] contain z^-1 sample
        // inputSamples[1] contain z^-2 sample
        // outputSamples[0] contain z^-1 sample
        // outputSamples[1] contain z^-2 sample
        // a[0] contain -a1
        // a[1] contain -a2
        // b[0] contain b0
        // b[1] contain b1

        // IIR filtering
        T accumulator = b[0] * newSample;
        for (size_t i = 0; i < filterOrder; i++) {
            // if constexpr (detail::has_multiplyAccumulate<T>::value) {
            //    outputSamples[0].multiplyAccumulate(b[i] * inputSamples[i], outputSamples[0]);
            //    outputSamples[0].multiplySubstract(a[i] * outputSamples[i], outputSamples[0]);
            //} else {
            accumulator += b[i + 1] * inputSamples[i] + a[i] * outputSamples[i];
            //}
        }
        // move samples buffer
        for (int_fast8_t i = filterOrder - 1; i > 0; i--) {
            inputSamples[i] = inputSamples[i - 1];
            outputSamples[i] = outputSamples[i - 1];
        }
        inputSamples[0] = newSample;
        outputSamples[0] = accumulator;
        return accumulator;
    }

    auto debugInput() { return inputSamples; }
    auto debugOutput() { return outputSamples; }

 private:
    const aCoefficients& a;
    const bCoefficients& b;
    std::array<T, filterOrder> inputSamples = {};
    std::array<T, filterOrder> outputSamples = {};
};

template <typename T, size_t filterOrder>
class IIR<T, DirectForm2, filterOrder> {
    static_assert(filterOrder > 0, "Filter order have to be at least 1");
    static_assert(filterOrder < 128, "Filter order have to be lower than 128");

 public:
    using aCoefficients = std::array<const T, filterOrder>;
    using bCoefficients = std::array<const T, filterOrder + 1>;

    IIR(const aCoefficients& a, const bCoefficients& b) : a(a), b(b) {}

    T filter(T newSample) {
        // delay[0] contain z^-1 sample
        // delay[1] contain z^-2 sample
        // a[0] contain -a1
        // a[1] contain -a2
        // b[0] contain b0
        // b[1] contain b1

        T xAccumulator = a[filterOrder - 1] * delay[filterOrder - 1];
        T yAccumulator = b[filterOrder] * delay[filterOrder - 1];
        for (int_fast8_t i = filterOrder - 2; i >= 0; i--) {
            xAccumulator += a[i] * delay[i];
            yAccumulator += b[i + 1] * delay[i];
            delay[i + 1] = delay[i];
        }
        xAccumulator += newSample;
        yAccumulator += b[0] * xAccumulator;
        delay[0] = xAccumulator;
        return yAccumulator;
    }

 private:
    const aCoefficients& a;
    const bCoefficients& b;
    std::array<T, filterOrder> delay = {};
};

template <typename T, size_t filterOrder>
class IIR<T, TransposedForm1, filterOrder> {
    static_assert(filterOrder > 0, "Filter order have to be at least 1");

 public:
    using aCoefficients = std::array<const T, filterOrder>;
    using bCoefficients = std::array<const T, filterOrder + 1>;

    IIR(const aCoefficients& a, const bCoefficients& b) : a(a), b(b) {}

    T filter(T newSample) {
        // inputSamples[0] contain z^-1 sample
        // inputSamples[1] contain z^-2 sample
        // outputSamples[0] contain z^-1 sample
        // outputSamples[1] contain z^-2 sample
        // a[0] contain -a1
        // a[1] contain -a2
        // b[0] contain b0
        // b[1] contain b1
        T xAccumulator = inputSamples[0] + newSample;
        T output = b[0] * xAccumulator + outputSamples[0];

        // IIR filtering
        for (size_t i = 0; i < filterOrder - 1; i++) {
            inputSamples[i] = a[i] * xAccumulator + inputSamples[i + 1];
            outputSamples[i] = b[i + 1] * xAccumulator + outputSamples[i + 1];
        }
        inputSamples[filterOrder - 1] = a[filterOrder - 1] * xAccumulator;
        outputSamples[filterOrder - 1] = b[filterOrder] * xAccumulator;

        return output;
    }

 private:
    std::array<T, filterOrder> inputSamples = {};
    std::array<T, filterOrder> outputSamples = {};
    const aCoefficients& a;
    const bCoefficients& b;
};

template <typename T, size_t filterOrder>
class IIR<T, TransposedForm2, filterOrder> {
    static_assert(filterOrder > 0, "Filter order have to be at least 1");

 public:
    using aCoefficients = std::array<const T, filterOrder>;
    using bCoefficients = std::array<const T, filterOrder + 1>;

    IIR(const aCoefficients& a, const bCoefficients& b) : a(a), b(b) {}

    T filter(T newSample) {
        // delay[0] contain z^-1 sample
        // delay[1] contain z^-2 sample
        // a[0] contain -a1
        // a[1] contain -a2
        // b[0] contain b0
        // b[1] contain b1
        T output = newSample * b[0] + delay[0];

        for (size_t i = 0; i < filterOrder - 1; i++) {
            delay[i] = newSample * b[i + 1] + a[i] * output + delay[i + 1];
        }
        // calculation for latest sample
        delay[filterOrder - 1] = newSample * b[filterOrder] + a[filterOrder - 1] * output;
        return output;
    }

 private:
    const aCoefficients& a;
    const bCoefficients& b;
    std::array<T, filterOrder> delay = {};
};

}  // namespace math
}  // namespace microhal
#endif /* _MICROHAL_MATH_IIR_H_ */
