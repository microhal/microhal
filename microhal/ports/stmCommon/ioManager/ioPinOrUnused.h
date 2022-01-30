/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 27-01-2022
 *
 * @copyright Copyright (c) 2022, Pawel Okas
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

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_IOPINORUNUSED_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_IOPINORUNUSED_H_

#include "peripheral.h"
#include "pinFunction.h"
#include "ports/stmCommon/stmCommonDefines.h"
#include _MICROHAL_INCLUDE_PORT_GPIO

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

struct IOPinOrUnused {
    typedef enum { Unused } unused_type;
    constexpr IOPinOrUnused(unused_type) {}
    constexpr IOPinOrUnused(IOPin pin) : haveIOPin(true), port(pin.port), pin_number(pin.pin) {}

    constexpr auto getIOPin() const { return IOPin(port, pin_number); }

    const bool haveIOPin = false;
    const IOPin::Port port{};
    const uint8_t pin_number{};

    constexpr bool operator!=(unused_type) const { return haveIOPin; }
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_IOPINORUNUSED_H_ */
