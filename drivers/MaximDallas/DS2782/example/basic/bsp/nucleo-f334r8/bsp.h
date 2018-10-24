/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      board support package for nucleo-f334r8 board
 *
 * @authors    Pawel Okas
 * created on: 14-03-2017
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2017, Paweł Okas
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
#ifndef NUCLEO_F334R8_H_
#define NUCLEO_F334R8_H_

#include "microhal.h"

namespace bsp {

// microhal extension board for nucleo evaluation boards
namespace con1 {
namespace a {
constexpr microhal::IOPin scl(microhal::stm32f3xx::GPIO::Port::PortB, 8);
constexpr microhal::IOPin sda(microhal::stm32f3xx::GPIO::Port::PortB, 9);
constexpr microhal::IOPin mosi(microhal::stm32f3xx::GPIO::Port::PortA, 7);
constexpr microhal::IOPin miso(microhal::stm32f3xx::GPIO::Port::PortA, 6);
constexpr microhal::IOPin sck(microhal::stm32f3xx::GPIO::Port::PortA, 5);
constexpr microhal::IOPin ss(microhal::stm32f3xx::GPIO::Port::PortA, 15);
constexpr microhal::IOPin rxd(microhal::stm32f3xx::GPIO::Port::PortA, 10);
constexpr microhal::IOPin txd(microhal::stm32f3xx::GPIO::Port::PortB, 6);
constexpr microhal::IOPin io1(microhal::stm32f3xx::GPIO::Port::PortC, 11);
constexpr microhal::IOPin io2(microhal::stm32f3xx::GPIO::Port::PortC, 10);
constexpr microhal::IOPin io3(microhal::stm32f3xx::GPIO::Port::PortD, 2);
constexpr microhal::IOPin io4(microhal::stm32f3xx::GPIO::Port::PortB, 13);
// constexpr microhal::IOPin io5 (microhal::stm32f3xx::GPIO::Port::PortC, 15);
constexpr microhal::IOPin io6(microhal::stm32f3xx::GPIO::Port::PortC, 12);
}  // namespace a
namespace b {
constexpr microhal::IOPin scl(microhal::stm32f3xx::GPIO::Port::PortB, 10);
constexpr microhal::IOPin sda(microhal::stm32f3xx::GPIO::Port::PortB, 11);
constexpr microhal::IOPin mosi(microhal::stm32f3xx::GPIO::Port::PortB, 15);
constexpr microhal::IOPin miso(microhal::stm32f3xx::GPIO::Port::PortB, 14);
constexpr microhal::IOPin sck(microhal::stm32f3xx::GPIO::Port::PortC, 7);
constexpr microhal::IOPin ss(microhal::stm32f3xx::GPIO::Port::PortB, 12);
constexpr microhal::IOPin io1(microhal::stm32f3xx::GPIO::Port::PortC, 11);
constexpr microhal::IOPin io2(microhal::stm32f3xx::GPIO::Port::PortC, 10);
constexpr microhal::IOPin io3(microhal::stm32f3xx::GPIO::Port::PortD, 2);
constexpr microhal::IOPin io4(microhal::stm32f3xx::GPIO::Port::PortB, 13);
}  // namespace b
namespace c {
constexpr microhal::IOPin scl(microhal::stm32f3xx::GPIO::Port::PortA, 8);
constexpr microhal::IOPin sda(microhal::stm32f3xx::GPIO::Port::PortC, 9);
constexpr microhal::IOPin mosi(microhal::stm32f3xx::GPIO::Port::PortB, 5);
constexpr microhal::IOPin miso(microhal::stm32f3xx::GPIO::Port::PortB, 4);
constexpr microhal::IOPin sck(microhal::stm32f3xx::GPIO::Port::PortB, 3);
constexpr microhal::IOPin ss(microhal::stm32f3xx::GPIO::Port::PortC, 1);
constexpr microhal::IOPin io1(microhal::stm32f3xx::GPIO::Port::PortB, 0);
constexpr microhal::IOPin io2(microhal::stm32f3xx::GPIO::Port::PortC, 2);
constexpr microhal::IOPin io3(microhal::stm32f3xx::GPIO::Port::PortC, 3);
constexpr microhal::IOPin io4(microhal::stm32f3xx::GPIO::Port::PortA, 3);
}  // namespace c
}  // namespace con1
namespace con2 {
namespace a {
constexpr microhal::IOPin scl(microhal::stm32f3xx::GPIO::Port::PortB, 8);
constexpr microhal::IOPin sda(microhal::stm32f3xx::GPIO::Port::PortB, 9);
constexpr microhal::IOPin mosi(microhal::stm32f3xx::GPIO::Port::PortA, 7);
constexpr microhal::IOPin miso(microhal::stm32f3xx::GPIO::Port::PortA, 6);
constexpr microhal::IOPin sck(microhal::stm32f3xx::GPIO::Port::PortA, 5);
constexpr microhal::IOPin ss(microhal::stm32f3xx::GPIO::Port::PortB, 2);
constexpr microhal::IOPin rxd(microhal::stm32f3xx::GPIO::Port::PortA, 12);
constexpr microhal::IOPin txd(microhal::stm32f3xx::GPIO::Port::PortA, 11);
constexpr microhal::IOPin io1(microhal::stm32f3xx::GPIO::Port::PortC, 8);
constexpr microhal::IOPin io2(microhal::stm32f3xx::GPIO::Port::PortD, 8);
constexpr microhal::IOPin io3(microhal::stm32f3xx::GPIO::Port::PortB, 1);
constexpr microhal::IOPin io4(microhal::stm32f3xx::GPIO::Port::PortC, 6);
constexpr microhal::IOPin io5(microhal::stm32f3xx::GPIO::Port::PortC, 5);
constexpr microhal::IOPin io6(microhal::stm32f3xx::GPIO::Port::PortA, 9);
}  // namespace a
}  // namespace con2
constexpr microhal::IOPin Led2(microhal::stm32f3xx::GPIO::Port::PortF, 6);
constexpr microhal::IOPin Led3(microhal::stm32f3xx::GPIO::Port::PortF, 7);
constexpr microhal::IOPin Led4(microhal::stm32f3xx::GPIO::Port::PortA, 13);
constexpr microhal::IOPin Led5(microhal::stm32f3xx::GPIO::Port::PortA, 14);
constexpr microhal::IOPin Sw1(microhal::stm32f3xx::GPIO::Port::PortC, 13);
constexpr microhal::IOPin Sw2(microhal::stm32f3xx::GPIO::Port::PortC, 4);
// end of microhal extension board definition
namespace ds2782 {
static microhal::I2C &i2c = microhal::stm32f3xx::I2C::i2c1;
}
static microhal::SerialPort &debugPort = microhal::stm32f3xx::SerialPort::Serial2;
}  // namespace bsp

#endif  // NUCLEO_F334R8_H_
