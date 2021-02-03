/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-04-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_L86_H_
#define _MICROHAL_L86_H_

#include <cstdint>
#include <thread>
#include "gpio.h"
#include "gps.h"

namespace microhal {

class L86 : public GPS {
 public:
    L86(GPIO& reset, GPIO& forceOn, GPIO& antennaDetect, GPIO& pps) : resetPin(reset), forceOnPin(forceOn), antennaDetect(antennaDetect), pps(pps) {
        resetPin.configureAsOutput(GPIO::OutputType::OpenDrain, GPIO::PullType::NoPull);
        forceOnPin.configureAsOutput(GPIO::OutputType::OpenDrain, GPIO::PullType::NoPull);
        antennaDetect.configureAsInput(GPIO::PullType::NoPull);
        pps.configureAsInput(GPIO::PullType::NoPull);
    }

    void reset() {
        using namespace std::literals;

        resetPin.reset();
        std::this_thread::sleep_for(11ms);
        resetPin.set();
    }

    void forceOn(bool force) { forceOnPin = force; }

    bool isExternalAntenaActive() const { return antennaDetect.isReset(); }

 private:
    GPIO& resetPin;
    GPIO& forceOnPin;
    GPIO& antennaDetect;
    GPIO& pps;
};

}  // namespace microhal

#endif /* _MICROHAL_L86_H_ */
