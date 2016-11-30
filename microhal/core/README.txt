BSD 3-Clause

Copyright (c) 2015, microHAL

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
       software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Aby projekt sie skompilowal nalezy zmodyfikowac plik gthr.h znajdujacy sie w: include/..sciezka_dla_danego_rdzenia../bits
Zmodyfikowany gthr.h znajduje sie w katalogu glownym projektu mircoHAL.

Instrukcja konfuguracji srodowiska znajduje sie w katalogu: ./info/microHal-srodowisko.odt

todo:
SerialPort:
	- funkcje read, write, putChar, getChar powinny zostac zabezpieczone mutexem(rekursywnym?) aby poszczegulne watki nie mieszaly odczytu/zapisu 

SPIDevice:
	funkcje typu writeBuffer(const uint32_t *buffer, uint16_t length, bool last) moze powinny przymowac parametr ByteOrder
	
I2Cslave - projekt klasy
SPIslave - projekt klasy
	
	
	
