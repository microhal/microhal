/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    pawel
 * created on: 01-01-2017
 * last modification: 18-02-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "bsp.h"
#include "ws2812.h"

using namespace microhal;

int main() {
	bsp::debugPort.write("\n\r------------------- WS2812B Demo -------------------------\n\r");

	constexpr int WS2812_chainLength = 8;
	WS2812B<WS2812_chainLength> ws(bsp::wsSpi);

	WS2812B<WS2812_chainLength>::Pixel colors[8];

	colors[0].r = 0x10;
	colors[0].g = 0x00;
	colors[0].b = 0x00;
	colors[1].r = 0x00;
	colors[1].g = 0x10;
	colors[1].b = 0x00;
	colors[2].r = 0x00;
	colors[2].g = 0x00;
	colors[2].b = 0x10;
	colors[3].r = 0x10;
	colors[3].g = 0x10;
	colors[3].b = 0x10;
	colors[4].r = 0x10;
	colors[4].g = 0x10;
	colors[4].b = 0x00;
	colors[5].r = 0x10;
	colors[5].g = 0x00;
	colors[5].b = 0x10;
	colors[6].r = 0x00;
	colors[6].g = 0x10;
	colors[6].b = 0x10;
	colors[7].r = 0x20;
	colors[7].g = 0x20;
	colors[7].b = 0x20;

	uint8_t offset = 0;
	while(1) {
		for (size_t i=0; i<8;i++) {
			ws[i] = colors[(i + offset)%8];
		}
		offset++;
		if(offset >= 8) offset = 0;

		ws.redraw();

		std::this_thread::sleep_for(std::chrono::milliseconds {200});
	}

	return 0;
}


