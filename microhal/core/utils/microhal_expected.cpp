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

#include "microhal_expected.h"
#include <ports/manager/hardware.h>

using namespace microhal;

#ifdef MICROHAL_EXPECTED_USE_DIAGNOSTIC
using namespace diagnostic;
#endif

namespace microhal {
namespace expected_detail {

void handle_value_access_error() {
#ifdef MICROHAL_EXPECTED_USE_DIAGNOSTIC
    diagChannel << lock << MICROHAL_EMERGENCY << "Microhal expected, trying to access value when expected class is holding error." << unlock;
#endif

#ifdef MICROHAL_EXPECTED_BREAKPOINT_AT_ERROR
    hardware::breakpoint();
#endif

#ifdef MICROHAL_EXPECTED_TROW_EXCEPTION_AT_ERROR
#error unimplemented
    // todo implement
#endif
}

void handle_error_access_error() {
#ifdef MICROHAL_EXPECTED_USE_DIAGNOSTIC
    diagChannel << lock << MICROHAL_EMERGENCY << "Microhal expected, trying to access error when expected class is holding value." << unlock;
#endif

#ifdef MICROHAL_EXPECTED_BREAKPOINT_AT_ERROR
    hardware::breakpoint();
#endif

#ifdef MICROHAL_EXPECTED_TROW_EXCEPTION_AT_ERROR
#error unimplemented
    // todo implement
#endif
}

}  // namespace expected_detail
}  // namespace microhal
