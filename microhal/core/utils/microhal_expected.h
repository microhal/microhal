/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 06-09-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_UTILS_MICROHAL_EXPECTED_H_
#define _MICROHAL_UTILS_MICROHAL_EXPECTED_H_

#include <type_traits>

#ifdef MICROHAL_EXPECTED_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"
#endif

namespace microhal {

namespace expected_detail {

template <bool, typename T>
struct TypeOrUnderlyingTypeImplementation;

template <typename T>
struct TypeOrUnderlyingTypeImplementation<false, T> {
    using Type = T;
};

template <typename T>
struct TypeOrUnderlyingTypeImplementation<true, T> {
    using Type = std::underlying_type_t<T>;
};

template <typename T>
struct TypeOrUnderlyingType {
    using Type = TypeOrUnderlyingTypeImplementation<std::is_enum_v<T>, T>::Type;
};

void handle_value_access_error();
void handle_error_access_error();

}  // namespace expected_detail

template <typename T>
requires std::is_signed_v<typename expected_detail::TypeOrUnderlyingType<T>::Type>  // type have to be able to hold negative values
    class UnexpectedNegativeValue {
 public:
    using UnexpectedType = expected_detail::TypeOrUnderlyingType<T>::Type;
};

template <typename ExpectedType, typename UnexpectedType>
class Expected {
    bool has_value() const { return m_value < 0; }

    const Expected value() const { return m_value; }

 private:
    UnexpectedType m_value = 0;
};

template <typename ExpectedType, typename UnexpectedType>
class Expected<ExpectedType, UnexpectedNegativeValue<UnexpectedType>> final {
    static_assert(sizeof(UnexpectedType) >= sizeof(ExpectedType),
                  "Unexpected type size to small. Unexpected type have to be able to hold expected value.");

    using DataType = UnexpectedNegativeValue<UnexpectedType>::UnexpectedType;

 public:
    template <typename U = UnexpectedType>
    constexpr Expected(ExpectedType value) : m_value(static_cast<DataType>(value)) {}

    constexpr Expected(UnexpectedType error) : m_value(static_cast<DataType>(error)) {}

    [[nodiscard]] constexpr bool has_value() const noexcept { return m_value >= 0; }

    [[nodiscard]] const ExpectedType value() const {
        if (!has_value()) {
            // error, accessing expected value when Expected class is holding unexpected (error) value
            expected_detail::handle_value_access_error();
        }
        return static_cast<ExpectedType>(m_value);
    }

    [[nodiscard]] const ExpectedType value_or(ExpectedType or_value) const noexcept {
        if (!has_value()) return or_value;
        return static_cast<ExpectedType>(m_value);
    }

    [[nodiscard]] const ExpectedType value_unsafe() const { return static_cast<ExpectedType>(m_value); }

    [[nodiscard]] const UnexpectedType error() const {
        if (has_value()) {
            // error, accessing unexpected (error) value when Expected class is holding expected value
            expected_detail::handle_error_access_error();
        }
        return static_cast<UnexpectedType>(m_value);
    }

    [[nodiscard]] constexpr operator bool() const noexcept { return has_value(); }

    [[nodiscard]] constexpr ExpectedType operator->() const { return static_cast<ExpectedType>(m_value); }
    [[nodiscard]] constexpr ExpectedType operator*() const { return static_cast<ExpectedType>(m_value); }

 private:
    DataType m_value = 0;
};

}  // namespace microhal

#ifdef MICROHAL_EXPECTED_USE_DIAGNOSTIC
template <microhal::diagnostic::LogLevel level, bool B, typename ExpectedType, typename UnexpectedType>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel,
                                                                  microhal::Expected<ExpectedType, UnexpectedType> result) {
    if (result.has_value()) {
        return logChannel << result.value();
    } else {
        return logChannel << result.error();
    }
}
#endif

#endif /* _MICROHAL_UTILS_MICROHAL_EXPECTED_H_ */
