/*
 * AD7903.h
 *
 *  Created on: 11 cze 2014
 *      Author: Dell
 */

#ifndef AD7903_H_
#define AD7903_H_

#include "microhal.h"

class AD7903 {
public:
	AD7903(microhal::SPI &spi, microhal::GPIO::IOPin cnvPin) :
			spi(spi), cnv(cnvPin.port, cnvPin.pin, microhal::GPIO::OUTPUT) {
		cnv.reset();
	}

	void convert() {
		cnv.set();
	}
	bool readSamples(int16_t &adc1, int16_t &adc2) {
		bool status = false;

		spi.lock();

		if (waitForConversionEnd() == true) {

//			spi.read((uint16_t &)adc1);
			//		spi.read((uint16_t &)adc2);

			//adc1 = adc1 >> 8 | adc1 << 8;
			//adc2 = adc2 >> 8 | adc2 << 8;
			//status = true;

			if (spi.read((uint16_t&) adc1) == microhal::SPI::NO_ERROR) {
				if (spi.read((uint16_t&) adc2) == microhal::SPI::NO_ERROR) {
					adc1 = adc1 >> 8 | adc1 << 8;
					adc2 = adc2 >> 8 | adc2 << 8;
					status = true;
				}
			}
		}

		cnv.reset();
		spi.unlock();

		return status;
	}

protected:
	microhal::GPIO cnv;

	microhal::SPI &spi;

	bool waitForConversionEnd() {
		volatile uint8_t ns = 250;
		while (spi.getMISOstate() == false) {
			if (--ns == 0)
				return false;
		}

		return true;
	}
};

#endif /* AD7903_H_ */
