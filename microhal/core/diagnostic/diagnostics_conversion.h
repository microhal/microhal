/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic component
 *
 * @authors    Pawel Okas
 * created on: 9-12-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015, microHAL
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

#ifndef _MICROHAL_DIAGNOSTICS_CONVERSION_H_
#define _MICROHAL_DIAGNOSTICS_CONVERSION_H_

#include <cstddef>
#include <cstdint>

namespace microhal {
namespace diagnostic {

template <unsigned int base>
class Converter {
 public:
    explicit constexpr Converter (uint32_t data) noexcept : data(data), length(0), dataPtr(nullptr) {
    }

    explicit constexpr Converter (const uint32_t *data, size_t size) noexcept : data(4), length(size/sizeof(uint32_t)), dataPtr(data) {
    }

    explicit constexpr Converter (const uint16_t *data, size_t size) noexcept : data(2), length(size/sizeof(uint16_t)), dataPtr(data) {
    }

    explicit constexpr Converter (const uint8_t *data, size_t size) noexcept : data(1), length(size/sizeof(uint8_t)), dataPtr(data) {
    }
 private:
    template<LogLevel compileTimeLogLevel, bool B>
    friend class LogLevelChannel;

    const uint32_t data;
    const size_t length;
    const void *dataPtr;
};

using toBin = Converter<2>;
using toOct = Converter<8>;
using toHex = Converter<16>;

} // namespace diagnostic
} // namespace microhal

#endif // _MICROHAL_DIAGNOSTICS_CONVERSION_H_
