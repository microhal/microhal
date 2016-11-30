/**
* @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Driver for mpl115a1 barometr

 * @authors    Marcin Urbanik
 * created on: 18-01-2016
 * last modification: <DD-MM-YYYY>

 * @copyright Copyright (c) 2015, microHAL
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 * conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _MICROHAL_MPL115A1_H_
#define _MICROHAL_MPL115A1_H_

#include "microhal.h"
#include "SPIDevice/SPIDevice.h"
#include <thread>

class mpl115a1: private microhal::SPIDevice {
    /**
     * @brief MPL115A2_SPI registers address
     */

	enum Register {
		Padc_MSB = 0x00,		//(0x00 << 1),  		//!<10-bit Pressure ADC output value MSB
		Padc_LSB = 0x01,		//(0x01 << 1),  		//!<10-bit Pressure ADC output value LSB
		Tadc_MSB = 0x02, 		//(0x02 << 1),  		//!<10-bit Temperature ADC output value MSB
		Tadc_LSB = 0x03,  		//(0x03 << 1),			//!<10-bit Temperature ADC output value LSB
		a0_MSB = 0x04,  		//(0x04 << 1),			//!<a0 coefficient MSB
		a0_LSB = 0x05,  		//(0x05 << 1),			//!<a0 coefficient LSB
		b1_MSB = 0x06,  		//(0x06 << 1),			//!<b1 coefficient MSB
		b1_LSB = 0x07,  		//(0x07 << 1),			//!<b1 coefficient LSB
		b2_MSB = 0x08,  		//(0x08 << 1),			//!<b2 coefficient MSB
		b2_LSB = 0x09,  		//(0x09 << 1),			//!<b2 coefficient LSB
		c12_MSB = 0x0A,  		//(0x0A << 1),			//!<c12 coefficient MSB
		c12_LSB = 0x0B,  		//(0x0B << 1),			//!<c12 coefficient LSB
		//from 0x0C to 0x11 Reserved - value= 0x00
		CONVERT = 0x12			//(0x12 << 1),			//!< Start Pressure and Temperature Conversion
	};
	int16_t sia0, sib1, sib2, sic12, sic11, sic22;      // Helpful variables for Calculate Pressure
   
 public:
 
	uint8_t mpl115a1Temperature[2];
	uint8_t mpl115a1Pressure[2];
	uint8_t mpl115a1Coeffs[12];
	void mpl115a1CalculatePressure(double *CalcPress);	//Calculate Pressure
	bool mpl115a1ReadPressureAndTempADC(void);			//Read Pressure
	bool mpl115a1ReadCoeffs(void);						//Read calibrated coefficients

	 /**
     *  \brief Constructor for MPL115A2_SPI driver
     *  
     *  \param [in] spi SPI device
     *  \param [in] CEpin CE pin
     *  
     *  \details Object is constructed with default settings. Device is inactive.
     */
	mpl115a1(microhal::SPI &spi, const microhal::GPIO::IOPin CEpin) :
        SPIDevice(spi, CEpin) {
    }

     /**
     *  \brief Function initializes MPL115A2_SPI driver.
     *  
     *  \return True if device is present at provided SPI interface,
     *  	 is active and respond with proper identification value. False otherwise.
     *  
     */ 

    enum Commands {
           READ = 0x80 ,
           WRITE = 0x00
       };
    
};
#endif  // _MICROHAL_MPL115A1_H_
