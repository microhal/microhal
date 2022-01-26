/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      serialPort port include file
 *
 * @authors    Pawel Okas
 * created on: 9-04-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015-2017, Pawel Okas
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

#ifndef _MICROHAL_SERIALPORT_PORT_H_
#define _MICROHAL_SERIALPORT_PORT_H_
/* ************************************************************************************************
 * INCLUDES
 */
#if defined(VENDOR_ATMEL)
#include "Atmel/serialPort_atmel.h"
#elif defined(VENDOR_NXP)

#elif defined(VENDOR_NORDIC)
#include "Nordic/serialPort_nordic.h"
#elif defined(VENDOR_RENESAS)
#include "Renesas/serialPort_renesas.h"
#elif defined(VENDOR_SILICON_LABS)
#include "SiliconLabs/serialPort_siliconLabs.h"
#elif defined(VENDOR_STMICROELECTRONICS)
#include "STMicroelectronics/serialPort_stm.h"
#elif defined(VENDOR_TEXAS_INSTRUMENTS)
#include "TexasInstruments/serialPort_ti.h"
#elif defined(LINUX_PORT)
#include "ports/linux/generic/serialPort_linux.h"
#elif defined(WINDOWS_PORT)
#include "ports/windows/General/serialPort_windows.h"
#else
#error "Microcontroller vendor undefined."
#endif

#endif  // _MICROHAL_SERIALPORT_PORT_H_
