/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef NUCLEO_F411RE_H_
#define NUCLEO_F411RE_H_

#include "i2c.h"
#include "microhal.h"

namespace bsp {
static microhal::SerialPort &debugPort = microhal::stm32f4xx::SerialPort::Serial2;
extern microhal::GPIO &lcd_rst;
extern microhal::GPIO &lcd_cs;
extern microhal::GPIO &lcd_rs;
extern microhal::GPIO &lcd_wr;
extern microhal::GPIO &lcd_rd;
extern microhal::GPIO &lcd_d0;
extern microhal::GPIO &lcd_d1;
extern microhal::GPIO &lcd_d2;
extern microhal::GPIO &lcd_d3;
extern microhal::GPIO &lcd_d4;
extern microhal::GPIO &lcd_d5;
extern microhal::GPIO &lcd_d6;
extern microhal::GPIO &lcd_d7;
bool init();
void deinit();
}  // namespace bsp
#endif  // NUCLEO_F411RE_H_
