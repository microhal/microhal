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

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_PERIPHERAL_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_PERIPHERAL_H_

#include "ports/stmCommon/registers/registersBaseAddressDefinitions.h"
#include "ports/stmCommon/stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

enum class Peripheral {
#ifdef _MICROHAL_ADC1_BASE_ADDRESS
    ADC1,
#endif
#ifdef _MICROHAL_ADC2_BASE_ADDRESS
    ADC2,
#endif
#if defined _MICROHAL_CAN1_BASE_ADDRESS || defined _MICROHAL_CAN_BASE_ADDRESS
    CAN1,
#endif
#if defined _MICROHAL_CAN2_BASE_ADDRESS
    CAN2,
#endif
#ifdef _MICROHAL_DAC1_BASE_ADDRESS
    DAC1,
#endif
#ifdef _MICROHAL_DAC2_BASE_ADDRESS
    DAC2,
#endif
#ifdef _MICROHAL_I2C1_BASE_ADDRESS
    I2C1,
#endif
#ifdef _MICROHAL_I2C2_BASE_ADDRESS
    I2C2,
#endif
#ifdef _MICROHAL_I2C3_BASE_ADDRESS
    I2C3,
#endif
#ifdef _MICROHAL_I2C4_BASE_ADDRESS
    I2C4,
#endif
    I2S,
    I2S1,
    I2S2,
    I2S2ext,
    I2S3,
    I2S3ext,
    I2S4,
    I2S5,
    LPTIM1,
    LPTIM2,
    SerialPort1,
    SerialPort2,
    SerialPort3,
    SerialPort4,
    SerialPort5,
    SerialPort6,
#ifdef _MICROHAL_SPI1_BASE_ADDRESS
    SPI1,
#endif
#ifdef _MICROHAL_SPI2_BASE_ADDRESS
    SPI2,
#endif
#ifdef _MICROHAL_SPI3_BASE_ADDRESS
    SPI3,
#endif
#ifdef _MICROHAL_SPI4_BASE_ADDRESS
    SPI4,
#endif
#ifdef _MICROHAL_SPI5_BASE_ADDRESS
    SPI5,
#endif
#ifdef _MICROHAL_TIM1_BASE_ADDRESS
    TIM1,
#endif
#ifdef _MICROHAL_TIM2_BASE_ADDRESS
    TIM2,
#endif
#ifdef _MICROHAL_TIM3_BASE_ADDRESS
    TIM3,
#endif
#ifdef _MICROHAL_TIM4_BASE_ADDRESS
    TIM4,
#endif
#ifdef _MICROHAL_TIM5_BASE_ADDRESS
    TIM5,
#endif
#ifdef _MICROHAL_TIM6_BASE_ADDRESS
    TIM6,
#endif
#ifdef _MICROHAL_TIM7_BASE_ADDRESS
    TIM7,
#endif
#ifdef _MICROHAL_TIM8_BASE_ADDRESS
    TIM8,
#endif
#ifdef _MICROHAL_TIM9_BASE_ADDRESS
    TIM9,
#endif
#ifdef _MICROHAL_TIM10_BASE_ADDRESS
    TIM10,
#endif
#ifdef _MICROHAL_TIM11_BASE_ADDRESS
    TIM11,
#endif
#ifdef _MICROHAL_TIM12_BASE_ADDRESS
    TIM12,
#endif
#ifdef _MICROHAL_TIM13_BASE_ADDRESS
    TIM13,
#endif
#ifdef _MICROHAL_TIM14_BASE_ADDRESS
    TIM14,
#endif
#ifdef _MICROHAL_TIM15_BASE_ADDRESS
    TIM15,
#endif
#ifdef _MICROHAL_TIM16_BASE_ADDRESS
    TIM16,
#endif
#ifdef _MICROHAL_TIM17_BASE_ADDRESS
    TIM17,
#endif
#ifdef _MICROHAL_HRTIM_MASTER_BASE_ADDRESS
    HRTIM1,
#endif
};

constexpr bool is_adc(Peripheral peripherial) {
    return true;
}

constexpr bool is_can(Peripheral peripherial) {
    return true;
}

constexpr bool is_dac(Peripheral peripherial) {
    return true;
}

constexpr bool is_i2c(Peripheral peripherial) {
    return true;
}

constexpr bool is_serial_port(Peripheral peripherial) {
    return true;
}

constexpr bool is_spi(Peripheral peripherial) {
    return true;
}

constexpr bool is_timer(Peripheral peripherial) {
    return true;
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_IOMANAGER_PERIPHERAL_H_ */
