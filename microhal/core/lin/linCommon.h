/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 13-03-2021
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

#ifndef SRC_THIRD_PARTY_MICROHAL_CORE_LIN_LINCOMMON_H_
#define SRC_THIRD_PARTY_MICROHAL_CORE_LIN_LINCOMMON_H_

#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"
#endif

#ifdef MICROHAL_LIN_USE_OSTREAM
#include <array>
#include <ostream>
#include <string_view>
#endif

namespace microhal {
namespace lin {
namespace detail {

enum class Error : uint8_t {
    None,
    Timeout,
    BusConflictDetected,
    BreakDetected,
    IncorrectSync,
    IncorrectHeaderParity,
    IncorrectChecksum,
    InputError,
    // ------- errors returned by transport layer
    Busy,
    BufferOverflow,
};

}
}  // namespace lin
}  // namespace microhal

#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B>& operator<<(microhal::diagnostic::LogLevelChannel<level, B>& logChannel,
                                                                   const microhal::lin::detail::Error error) {
    constexpr const std::array<std::string_view, 8> str = {
        "None", "Timeout", "BusConflictDetected", "BreakDetected", "IncorrectSync", "IncorrectHeaderParity", "IncorrectChecksum", "InputError"};
    return logChannel << str[static_cast<int>(error)];
}
#endif

#ifdef MICROHAL_LIN_USE_OSTREAM
inline std::ostream& operator<<(std::ostream& os, const microhal::lin::detail::Error error) {
    constexpr const std::array<std::string_view, 8> str = {
        "None", "Timeout", "BusConflictDetected", "BreakDetected", "IncorrectSync", "IncorrectHeaderParity", "IncorrectChecksum", "InputError"};
    os << str[static_cast<uint32_t>(error)];
    return os;
}
#endif

#endif /* SRC_THIRD_PARTY_MICROHAL_CORE_LIN_LINCOMMON_H_ */
