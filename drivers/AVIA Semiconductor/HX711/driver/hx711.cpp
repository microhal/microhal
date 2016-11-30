/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      HX711 device driver
 *
 * @authors    Pawel Okas
 * created on: 7-02-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, microHAL
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

#include "hx711.h"
#include "diagnostic/diagnostic.h"

using namespace microhal;
using namespace std::literals::chrono_literals;

void HX711::read() {
    // requirements:
    // spi clk frequency have to be grater than 10kHz, and lower than 5MHz
    microhal::ExternalInterrupt::disable(miso);
    const uint8_t command[] = {0b0100'0000, 0b01010'0000, 0b0101'0100};
    uint8_t data[6];

    spi.readBuffer(data, sizeof(data), 0b01010101);
    spi.write(command[static_cast<uint8_t>(channel)], true);

    uint32_t out = 0;
    for (size_t i = 0, bit = 0; bit < 4; i += 2, bit++) {
         out |= (0x01 & (data[0] >> i)) << (bit + 20);
         out |= (0x01 & (data[1] >> i)) << (bit + 16);
         out |= (0x01 & (data[2] >> i)) << (bit + 12);
         out |= (0x01 & (data[3] >> i)) << (bit + 8);
         out |= (0x01 & (data[4] >> i)) << (bit + 4);
         out |= (0x01 & (data[5] >> i)) << (bit + 0);
    }
    // because output is in 2's complement we have to check the MSB(24th) bit and if is equal to one we need to set
    // to one all bytes between 32 and 24.
    if (out &(1UL << 23)) {
        out |=0xFF000000;
    }

    this->data = out;
    microhal::ExternalInterrupt::enable(miso);
}
