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

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_PINFUNCTION_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_PINFUNCTION_H_

#include <utility>
#include "ports/stmCommon/stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

enum class PinFunction {
    ADC_Channel0,
    ADC_Channel1,
    ADC_Channel2,
    ADC_Channel3,
    ADC_Channel4,
    ADC_Channel5,
    ADC_Channel6,
    ADC_Channel7,
    ADC_Channel8,
    ADC_Channel9,
    ADC_Channel10,
    ADC_Channel11,
    ADC_Channel12,
    ADC_Channel13,
    ADC_Channel14,
    ADC_Channel15,
    ADC_Channel16,
    ADC_Channel17,
    ADC_Channel18,
    CAN_RX,
    CAN_TX,
    DAC_OUT1,
    DAC_OUT2,
    I2C_SDA,
    I2C_SCL,
    I2C_SMBA,
    I2S_CK,
    I2S_CKCEC,
    I2S_CKIN,
    I2S_CKLP,
    I2S_SD,
    I2S_MCK,
    I2S_WS,
    I2S_SDLP,
    SerialPort_CTS,
    SerialPort_RXD,
    SerialPort_RTS,
    SerialPort_TXD,
    SerialPort_CK,
    SPI_MISO,
    SPI_MOSI,
    SPI_SCK,
    SPI_NSS,
    Timer_BKIN,
    Timer_BKIN2,
    Timer_CH1,
    Timer_CH2,
    Timer_CH3,
    Timer_CH4,
    Timer_CH1N,
    Timer_CH2N,
    Timer_CH3N,
    Timer_CH1_ETR,
    Timer_ETR,
    Timer_ETRU,
    Timer_IN1,
    Timer_IN2,
    Timer_OUT,
    Timer_CHA1,
    Timer_CHA2,
    Timer_CHA3,
    Timer_CHA4,
    Timer_CHB1,
    Timer_CHB2,
    Timer_CHB3,
    Timer_CHB4,
    Timer_CHC1,
    Timer_CHC2,
    Timer_CHD1,
    Timer_CHD2,
    Timer_CHE1,
    Timer_CHE2,
    Timer_EEV1,
    Timer_EEV2,
    Timer_EEV3,
    Timer_EEV4,
    Timer_EEV5,
    Timer_EEV6,
    Timer_EEV7,
    Timer_EEV8,
    Timer_EEV9,
    Timer_EEV10,
    Timer_FLT1,
    Timer_FLT2,
    Timer_FLT3,
    Timer_FLT4,
    Timer_FLT5,
    Timer_SCIN,
    Timer_SCOUT
};

template <typename Enum>
constexpr auto to_underlying(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

enum class ADCChannel {
    Channel0 = to_underlying(PinFunction::ADC_Channel0),
    Channel1 = to_underlying(PinFunction::ADC_Channel1),
    Channel2 = to_underlying(PinFunction::ADC_Channel2),
    Channel3 = to_underlying(PinFunction::ADC_Channel3),
    Channel4 = to_underlying(PinFunction::ADC_Channel4),
    Channel5 = to_underlying(PinFunction::ADC_Channel5),
    Channel6 = to_underlying(PinFunction::ADC_Channel6),
    Channel7 = to_underlying(PinFunction::ADC_Channel7),
    Channel8 = to_underlying(PinFunction::ADC_Channel8),
    Channel9 = to_underlying(PinFunction::ADC_Channel9),
    Channel10 = to_underlying(PinFunction::ADC_Channel10),
    Channel11 = to_underlying(PinFunction::ADC_Channel11),
    Channel12 = to_underlying(PinFunction::ADC_Channel12),
    Channel13 = to_underlying(PinFunction::ADC_Channel13),
    Channel14 = to_underlying(PinFunction::ADC_Channel14),
    Channel15 = to_underlying(PinFunction::ADC_Channel15),
    Channel16 = to_underlying(PinFunction::ADC_Channel16),
    Channel17 = to_underlying(PinFunction::ADC_Channel17),
    Channel18 = to_underlying(PinFunction::ADC_Channel18)
};

enum class DACChannel { Channel1 = to_underlying(PinFunction::DAC_OUT1), Channel2 = to_underlying(PinFunction::DAC_OUT2) };

enum class TimerChannel {

};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_PINFUNCTION_H_ */
