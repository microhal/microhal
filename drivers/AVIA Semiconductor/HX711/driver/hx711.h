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

#ifndef _MICROHAL_HX711_H_
#define _MICROHAL_HX711_H_

#include "microhal.h"
#include "microhal_bsp.h"

#include <ExternalInterrupt.h>

class HX711 {
	const float standard_scale_coefficient =  -64.0*14.65f;
 public:
    enum class Channel {
      A_Gain_128 = 0,
      B_Gain_32,
      A_Gain_64
    };

    explicit HX711(microhal::SPI &spi, microhal::GPIO::IOPin miso) : spi(spi), miso(miso), offset(0), scale_coefficient(standard_scale_coefficient) {
        microhal::ExternalInterrupt::init();
        microhal::ExternalInterrupt::connect(falling_edgeSlot, *this, microhal::ExternalInterrupt::Trigger::OnFallingEdge, miso);
        microhal::ExternalInterrupt::enable(miso);
    }

    void reset() {
        // to reset device we need to pull up PSCK pin for at least 60us
        std::lock_guard<microhal::SPI> guard(spi);
        spi.write(static_cast<uint8_t>(0xFF), true);
        offset = 0;
        scale_coefficient = standard_scale_coefficient;
    }

    void setChannel(Channel ch) {
        channel = ch;
    }

    void read();

    int32_t getrawData() const noexcept {
        return data;
    }

    void tare() {
        offset = data;
    }

    float gettaredData() const noexcept {
        return static_cast<float>(data - offset);
    }

    float getscaledData() const noexcept {
        return static_cast<float>(data - offset) / scale_coefficient;
    }

    void scale(int32_t mass) {
        float temp = static_cast<float>(data - offset);
        scale_coefficient = temp /static_cast<float>( mass);
    }

 private:
    microhal::SPI &spi;
    microhal::GPIO::IOPin miso;
    microhal::Slot_0<HX711, &HX711::read> falling_edgeSlot;
    Channel channel = Channel::A_Gain_128;
    int32_t data = 0;
    int32_t offset;
    float scale_coefficient;
};

#endif  // _MICROHAL_HX711_H_
