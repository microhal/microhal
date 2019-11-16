/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      board support package for nucleo-f411re board
 *
 * @authors    Pawel Okas
 * created on: 18-11-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Paweł Okas
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

#ifndef NUCLEO_F103RB_H_
#define NUCLEO_F103RB_H_

#include "microhal.h"
namespace bsp {
// microhal extension board for nucleo evaluation boards
namespace con1 {
namespace a {
constexpr microhal::IOPin scl(microhal::stm32f1xx::IOPin::PortB, 8);
constexpr microhal::IOPin sda(microhal::stm32f1xx::IOPin::PortB, 9);
constexpr microhal::IOPin mosi(microhal::stm32f1xx::IOPin::PortA, 7);
constexpr microhal::IOPin miso(microhal::stm32f1xx::IOPin::PortA, 6);
constexpr microhal::IOPin sck(microhal::stm32f1xx::IOPin::PortA, 5);
constexpr microhal::IOPin ss(microhal::stm32f1xx::IOPin::PortA, 15);
constexpr microhal::IOPin rxd(microhal::stm32f1xx::IOPin::PortA, 10);
constexpr microhal::IOPin txd(microhal::stm32f1xx::IOPin::PortB, 6);
constexpr microhal::IOPin io1(microhal::stm32f1xx::IOPin::PortC, 11);
constexpr microhal::IOPin io2(microhal::stm32f1xx::IOPin::PortC, 10);
constexpr microhal::IOPin io3(microhal::stm32f1xx::IOPin::PortD, 2);
constexpr microhal::IOPin io4(microhal::stm32f1xx::IOPin::PortB, 13);
// constexpr microhal::IOPin io5 (microhal::stm32f1xx::IOPin::PortC, 15);
constexpr microhal::IOPin io6(microhal::stm32f1xx::IOPin::PortC, 12);
}  // namespace a
namespace b {
constexpr microhal::IOPin scl(microhal::stm32f1xx::IOPin::PortB, 10);
constexpr microhal::IOPin sda(microhal::stm32f1xx::IOPin::PortB, 11);
constexpr microhal::IOPin mosi(microhal::stm32f1xx::IOPin::PortB, 15);
constexpr microhal::IOPin miso(microhal::stm32f1xx::IOPin::PortB, 14);
constexpr microhal::IOPin sck(microhal::stm32f1xx::IOPin::PortC, 7);
constexpr microhal::IOPin ss(microhal::stm32f1xx::IOPin::PortB, 12);
constexpr microhal::IOPin io1(microhal::stm32f1xx::IOPin::PortC, 11);
constexpr microhal::IOPin io2(microhal::stm32f1xx::IOPin::PortC, 10);
constexpr microhal::IOPin io3(microhal::stm32f1xx::IOPin::PortD, 2);
constexpr microhal::IOPin io4(microhal::stm32f1xx::IOPin::PortB, 13);
}  // namespace b
namespace c {
constexpr microhal::IOPin scl(microhal::stm32f1xx::IOPin::PortA, 8);
constexpr microhal::IOPin sda(microhal::stm32f1xx::IOPin::PortC, 9);
constexpr microhal::IOPin mosi(microhal::stm32f1xx::IOPin::PortB, 5);
constexpr microhal::IOPin miso(microhal::stm32f1xx::IOPin::PortB, 4);
constexpr microhal::IOPin sck(microhal::stm32f1xx::IOPin::PortB, 3);
constexpr microhal::IOPin ss(microhal::stm32f1xx::IOPin::PortC, 1);
constexpr microhal::IOPin io1(microhal::stm32f1xx::IOPin::PortB, 0);
constexpr microhal::IOPin io2(microhal::stm32f1xx::IOPin::PortC, 2);
constexpr microhal::IOPin io3(microhal::stm32f1xx::IOPin::PortC, 3);
constexpr microhal::IOPin io4(microhal::stm32f1xx::IOPin::PortA, 3);
}  // namespace c
}  // namespace con1
namespace con2 {
namespace a {
constexpr microhal::IOPin scl(microhal::stm32f1xx::IOPin::PortB, 8);
constexpr microhal::IOPin sda(microhal::stm32f1xx::IOPin::PortB, 9);
constexpr microhal::IOPin mosi(microhal::stm32f1xx::IOPin::PortA, 7);
constexpr microhal::IOPin miso(microhal::stm32f1xx::IOPin::PortA, 6);
constexpr microhal::IOPin sck(microhal::stm32f1xx::IOPin::PortA, 5);
constexpr microhal::IOPin ss(microhal::stm32f1xx::IOPin::PortB, 2);
constexpr microhal::IOPin rxd(microhal::stm32f1xx::IOPin::PortA, 12);
constexpr microhal::IOPin txd(microhal::stm32f1xx::IOPin::PortA, 11);
constexpr microhal::IOPin io1(microhal::stm32f1xx::IOPin::PortC, 8);
constexpr microhal::IOPin io2(microhal::stm32f1xx::IOPin::PortD, 8);
constexpr microhal::IOPin io3(microhal::stm32f1xx::IOPin::PortB, 1);
constexpr microhal::IOPin io4(microhal::stm32f1xx::IOPin::PortC, 6);
constexpr microhal::IOPin io5(microhal::stm32f1xx::IOPin::PortC, 5);
constexpr microhal::IOPin io6(microhal::stm32f1xx::IOPin::PortA, 9);
}  // namespace a
}  // namespace con2
// constexpr microhal::IOPin Led2(microhal::stm32f1xx::IOPin::PortF, 6);
// constexpr microhal::IOPin Led3(microhal::stm32f1xx::IOPin::PortF, 7);
constexpr microhal::IOPin Led4(microhal::stm32f1xx::IOPin::PortA, 13);
constexpr microhal::IOPin Led5(microhal::stm32f1xx::IOPin::PortA, 14);
constexpr microhal::IOPin Sw1(microhal::stm32f1xx::IOPin::PortC, 13);
constexpr microhal::IOPin Sw2(microhal::stm32f1xx::IOPin::PortC, 4);
// end of microhal extension board definition

namespace as5311 {
extern microhal::SPI &spi;
extern microhal::GPIO &ce;
}  // namespace as5311
static microhal::SerialPort &debugPort = microhal::stm32f1xx::SerialPort::Serial2;
}  // namespace bsp
#endif  // NUCLEO_F103RB_H_
