/**
 * @file
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      board support package for nucleo-g071rb board
 *
 * @authors    Pawel Okas
 * created on: 02-02-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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

#ifndef NUCLEO_G071RB_H_
#define NUCLEO_G071RB_H_

#include "microhal.h"

namespace bsp {

static microhal::SerialPort &debugPort = microhal::stm32g0xx::SerialPort::Serial2;

namespace moduleA {
// connected into nucleo expander CON1A
extern microhal::stm32g0xx::SPI_interrupt &spi;                              // = microhal::stm32f4xx::SPI::spi1;
constexpr microhal::IOPin IRQ0(microhal::stm32g0xx::GPIO::Port::PortB, 13);  // CON1A GPIO4
constexpr microhal::IOPin IRQ1(microhal::stm32g0xx::GPIO::Port::PortC, 10);  // CON1A GPIO2
extern microhal::GPIO &csDat;
extern microhal::GPIO &csCon;
extern microhal::GPIO &RESET;
}  // namespace moduleA

namespace moduleB {
// connected into nucleo expander CON2A
extern microhal::stm32g0xx::SPI_interrupt &spi;                             // = microhal::stm32f4xx::SPI::spi3;
constexpr microhal::IOPin IRQ0(microhal::stm32g0xx::GPIO::Port::PortC, 6);  // CON2A GPIO4
constexpr microhal::IOPin IRQ1(microhal::stm32g0xx::GPIO::Port::PortD, 6);  // CON2A GPIO2
extern microhal::GPIO &csDat;
extern microhal::GPIO &csCon;
extern microhal::GPIO &RESET;
}  // namespace moduleB

void init();

}  // namespace bsp

#endif  // NUCLEO_G071RB_H_
