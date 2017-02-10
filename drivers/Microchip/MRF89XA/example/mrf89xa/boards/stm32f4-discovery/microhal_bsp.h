/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      hardware definitions

 @authors    Pawel
 created on: 11-08-2015
 last modification: <DD-MM-YYYY>

 Copyright (c) 2014, microHAL
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

extern microhal::SerialPort &debugPort;

extern microhal::SerialPort &communicationPort;

// first RF module pins configuration
extern microhal::SPI &rfModule1_SPI;
extern microhal::GPIO::IOPin rfModule1_csDat;
extern microhal::GPIO::IOPin rfModule1_csCon;
extern microhal::GPIO::IOPin rfModule1_IRQ0;
extern microhal::GPIO::IOPin rfModule1_IRQ1;
extern microhal::GPIO::IOPin rfModule1_RESET;

// second RF module pins configuration
extern microhal::SPI &rfModule2_SPI;
extern microhal::GPIO::IOPin rfModule2_csDat;
extern microhal::GPIO::IOPin rfModule2_csCon;
extern microhal::GPIO::IOPin rfModule2_IRQ0;
extern microhal::GPIO::IOPin rfModule2_IRQ1;
extern microhal::GPIO::IOPin rfModule2_RESET;

extern microhal::GPIO::IOPin Led3;
extern microhal::GPIO::IOPin Led4;
extern microhal::GPIO::IOPin Led5;
extern microhal::GPIO::IOPin Led6;

extern microhal::GPIO::IOPin Sw1;

extern microhal::GPIO::IOPin GreenLed;
extern microhal::GPIO::IOPin RedLed;

#endif /* STM32F4DISCOVERY_H_ */
