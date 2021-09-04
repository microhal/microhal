/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      board support package for stm32f4Discovery board

 @authors    Pawel Okas
 created on: 16-04-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 	 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 	 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 	 	in the documentation and/or other materials provided with the distribution.
 	 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 	 	from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */ /* ==========================================================================================================================
                                                                                                                                                                                                                                                                         */

#ifndef STM32F4DISCOVERY_H_
#define STM32F4DISCOVERY_H_

#include "microhal.h"

namespace bsp {
namespace con1 {
namespace a {
constexpr microhal::IOPin io1(microhal::IOPin::PortE, 5);
constexpr microhal::IOPin io2(microhal::IOPin::PortC, 15);
constexpr microhal::IOPin io3(microhal::IOPin::PortC, 13);
constexpr microhal::IOPin io4(microhal::IOPin::PortE, 6);
constexpr microhal::IOPin io5(microhal::IOPin::PortE, 4);
constexpr microhal::IOPin io6(microhal::IOPin::PortE, 2);
constexpr microhal::IOPin ss(microhal::IOPin::PortC, 14);
}  // namespace a
namespace b {
constexpr microhal::IOPin io1(microhal::IOPin::PortC, 5);
constexpr microhal::IOPin io2(microhal::IOPin::PortB, 1);
constexpr microhal::IOPin io3(microhal::IOPin::PortE, 9);
constexpr microhal::IOPin io4(microhal::IOPin::PortE, 11);
constexpr microhal::IOPin io5(microhal::IOPin::PortE, 4);
constexpr microhal::IOPin ss(microhal::IOPin::PortE, 7);
}  // namespace b
}  // namespace con1

static microhal::SerialPort &debugPort = microhal::stm32f4xx::SerialPort::Serial3;

namespace sdCard {
static microhal::SPI &spi = microhal::stm32f4xx::SPI::spi3;
extern microhal::GPIO &cs;
}  // namespace sdCard

}  // namespace bsp

#endif /* STM32F4DISCOVERY_H_ */
