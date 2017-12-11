/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 06-12-2017
 * last modification: 06-12-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_IOMANAGER_NRF51_H_
#define _MICROHAL_IOMANAGER_NRF51_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "gpio_nrf51.h"
#include "nrf.h"
#include "serialPort.h"

namespace microhal {
typedef enum {
    Rxd,
    Txd,
    Rts,
    Cts,
} SerialPinType;

namespace nrf51 {

class IOManager {
 public:
    IOManager() = delete;

    template <int serial, SerialPinType serialType, nrf51::GPIO::Port port, nrf51::GPIO::Pin pinNr>
    static void routeSerial(nrf51::GPIO::PullType pull = nrf51::GPIO::NoPull, nrf51::GPIO::OutputType type = nrf51::GPIO::PushPull) {
        // assert for Serial1
        static_assert(pinNr <= 31, "NRF51 has only 32 pins, numbered from 0 to 31");

        // switch (serial) {
        //  case 1:
        if (serialType == SerialPinType::Rxd) {
            nrf51::GPIO::setDirection(port, pinNr, nrf51::GPIO::Direction::Input);
            NRF_UART0->PSELRXD = pinNr;
        }
        if (serialType == SerialPinType::Txd) {
            nrf51::GPIO::setDirection(port, pinNr, nrf51::GPIO::Direction::Output);
            NRF_UART0->PSELTXD = pinNr;
        }
        if (serialType == SerialPinType::Rts) {
            NRF_UART0->PSELRTS = pinNr;
        }
        if (serialType == SerialPinType::Cts) {
            NRF_UART0->PSELCTS = pinNr;
        }
        //    break;
        //}
    }
};

}  // namespace nrf51
}  // namespace microhal

#endif  // _MICROHAL_IOMANAGER_NRF51_H_
