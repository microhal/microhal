/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 15-02-2018
 * last modification: 15-02-2018
 *
 * @copyright Copyright (c) 2018, microHAL
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

#ifndef _MICROHAL_TYPES_H_
#define _MICROHAL_TYPES_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>

namespace microhal {
/* **************************************************************************************************************************************************
 * CLASS
 */
struct uint24_t {
    uint8_t a[3];

    uint24_t() = default;
    constexpr uint24_t(uint32_t t)
        : a{static_cast<uint8_t>((t >> 16) & 0xFFU), static_cast<uint8_t>((t >> 8) & 0xFFU), static_cast<uint8_t>(t & 0xFFU)} {}

    operator uint32_t() {
        uint32_t tmp = (*reinterpret_cast<uint32_t *>(a)) & 0x00FFFFFF;
        return tmp;
    }

 private:
    void byteswap() {
        uint8_t tmp = a[0];
        a[0] = a[2];
        a[2] = tmp;
    }
    template <typename T>
    friend constexpr T convertEndianness(T);
};

}  // namespace microhal
#endif  // _MICROHAL_TYPES_H_
