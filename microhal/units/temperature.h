/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      temperature unit

 * @authors    Pawel
 * created on: 10-11-2015
 * last modification: <DD-MM-YYYY>

 * @copyright Copyright (c) 2015, microHAL
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 * conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_TEMPERATURE_H_
#define _MICROHAL_TEMPERATURE_H_

#include <cstdint>

namespace microhal {

class Temperature {
 public:
    explicit Temperature(const float temperatureInKelvin = 0.0) :
        temperature(temperatureInKelvin) { }
    explicit Temperature(const int32_t temperatureInKelvin) :
        temperature(static_cast <float>(temperatureInKelvin)) { }
    ~Temperature() {}

    void setFahrenheit(const int32_t temperatureInFahrenheit) {
        temperature = (((static_cast <float> (temperatureInFahrenheit)) - 32.00) / 1.8000) + 273.15;
    }
    void setFahrenheit(const float temperatureInFahrenheit) {
        temperature = ((temperatureInFahrenheit - 32.00) / 1.8000) + 273.15;
    }

    void setCelsius(const int32_t temperatureInCelsius) {
        temperature = (static_cast <float> (temperatureInCelsius)) + 273.15;
    }
    void setCelsius(const float temperatureInCelsius) {
        temperature = (temperatureInCelsius) + 273.15;
    }

    float getKelvin() const {
        return temperature;
    }

    float getCelsius() const {
        return temperature - 273.15;
    }

    float getFahrenheit() const {
        return (temperature - 273.15) * 1.8000 + 32.00;
    }

    Temperature operator+(const Temperature& temp) const {
        return Temperature {temperature + temp.temperature};
    }

    Temperature operator-(const Temperature& temp) const {
        return Temperature {temperature - temp.temperature};
    }

    Temperature operator*(const float mul) const {
        return Temperature {temperature * mul};
    }
    Temperature operator/(const float div) const {
        return Temperature {temperature / div};
    }

    Temperature& operator+=(const Temperature& temp) {
        temperature += temp.temperature;
        return *this;
    }

    Temperature& operator-=(const Temperature& temp) {
        temperature -= temp.temperature;
        return *this;
    }

    Temperature& operator*=(const float mul) {
        temperature *= mul;
        return *this;
    }

    Temperature& operator/=(const float div) {
        temperature /= div;
        return *this;
    }

 private:
  float temperature;  // in Kelvin degree
};

} /* namespace microhal */

#endif  // _MICROHAL_TEMPERATURE_H_
