/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    pawel
 * created on: 01-01-2016
 * last modification: 01-01-2016
 *
 * @copyright Copyright (c) 2017, microHAL
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

#ifndef _MICROHAL_WS2812_H_
#define _MICROHAL_WS2812_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "microhal.h"
/* **************************************************************************************************************************************************
 * CLASS
 */
template <size_t chainLen>
class WS2812B {
 public:
	struct Pixel {
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	WS2812B(microhal::SPI &spi) : spi(spi) {
	}

	void redraw() {
		fillPixelBuff();
		spi.write(pixelBuff, sizeof(pixelBuff), true);
	}

	Pixel &operator[](size_t i) {
		if (i >= chainLen) std::terminate();
		return pixel[i];
	}
	const Pixel &operator[](size_t i) const {
		if (i >= chainLen) std::terminate();
		return pixel[i];
	}
 private:
	microhal::SPI &spi;
	uint8_t pixelBuff[chainLen * 24];
	Pixel pixel[chainLen];

	void fillPixelBuff() {
		for (size_t i=0; i<chainLen; i++) {
			setPixelTab(pixel[i], &pixelBuff[3*8*i]);
		}
	}

	void setPixelTab(Pixel pixel, uint8_t *tab) {
		setColorTab(pixel.g, tab);
		setColorTab(pixel.r, tab + 8);
		setColorTab(pixel.b, tab + 16);
	}

	void setColorTab(uint8_t color, uint8_t *tab) {
		for (uint8_t bit = 0; bit < 8; bit++) {
			if (color & (1 << (7-bit))) {
				tab[bit] = 0b11111000;
			} else {
				tab[bit] = 0b11100000;
			}
		}
	}
};

#endif  // _MICROHAL_WS2812_H_
