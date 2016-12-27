/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 27-12-2016
 * last modification: 27-12-2016
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

#ifndef _MICROHAL_BSP_H_
#define _MICROHAL_BSP_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "microhal.h"

constexpr microhal::GPIO::IOPin led3_pin(microhal::nrf51::GPIO::Port::P0, 13);
constexpr microhal::GPIO::IOPin led4_pin(microhal::nrf51::GPIO::Port::P0, 12);
constexpr microhal::GPIO::IOPin led5_pin(microhal::nrf51::GPIO::Port::P0, 14);
constexpr microhal::GPIO::IOPin led6_pin(microhal::nrf51::GPIO::Port::P0, 15);

constexpr microhal::GPIO::IOPin button_pin(microhal::nrf51::GPIO::Port::P0, 0);

constexpr microhal::GPIO::IOPin greenLed_pin = led4_pin;
constexpr microhal::GPIO::IOPin redLed_pin = led3_pin;

#endif  // _MICROHAL_BSP_H_
