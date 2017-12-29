/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 17-12-2016
 * last modification: 17-12-2016
 *
 * @copyright Copyright (c) 2016 - 2017, Pawel Okas
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

#ifndef _MICROHAL_HARDWARE_H_
#define _MICROHAL_HARDWARE_H_
/* ************************************************************************************************
 * INCLUDES
 */
#if defined(VENDOR_ATMEL)
#include "Atmel/hardware_atmel.h"
#elif defined(VENDOR_NORDIC)
#include "Nordic/hardware_nordic.h"
#elif defined(VENDOR_RENESAS)
#include "Renesas/hardware_renesas.h"
#elif defined(VENDOR_SILICON_LABS)
#include "SiliconLabs/hardware_siliconLabs.h"
#elif defined(VENDOR_STMICROELECTRONICS)
#include "STMicroelectronics/hardware_stm.h"
#elif defined(VENDOR_TEXAS_INSTRUMENTS)
#include "TexasInstruments/hardware_ti.h"
#elif defined(LINUX_PORT)
#include "ports/linux/hardware_linux.h"
#else
#error "Microcontroller vendor undefined."
#endif

#endif  // _MICROHAL_HARDWARE_H_
