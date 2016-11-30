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
#include "microhal.h"
#include "microhal_bsp.h"

#include "mpl115a1.h"
#define N 100
double Pressure;
double sum=0;

using namespace microhal;
using namespace diagnostic;
// create barometer sensor object
mpl115a1 sensor(kamodbar_SPI,kamodbar_CS);  	 	//constructor with initial arguments defined in microhal_bsp.h:
													//static microhal::SPI &kamodbar_SPI = microhal::stm32f4xx::SPI::spi1;
													//constexpr microhal::GPIO::IOPin kamodbar_CS(microhal::stm32f4xx::GPIO::Port::PortE, 3);


int main(void) {
	debugPort.open(SerialPort::ReadWrite);
    // show example information
    debugPort.write("\n\r----- mpl115a1 Demo -----\n\r");
    // set default diagChannel output device.
    diagChannel.setOutputDevice(debugPort);
    if(0!=sensor.mpl115a1ReadCoeffs())return 1;
    while(1)
    {
    	for(int i=0;i<N;i++)
        {
    		if(0!=sensor.mpl115a1ReadPressureAndTempADC())return 1;// Could be error signal instead return 1
    		sensor.mpl115a1CalculatePressure(& Pressure);
            sum+=Pressure;
        }
        sum=sum/N;
        diagChannel << lock << DEBUG << " Pressure  "<< sum<< endl <<unlock;
    }


        /*diagChannel << lock << DEBUG << " Coeffs[0]  "<<sensor.mpl115a1Coeffs[0]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[1]  "<<sensor.mpl115a1Coeffs[1]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[2]  "<<sensor.mpl115a1Coeffs[2]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[3]  "<<sensor.mpl115a1Coeffs[3]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[4]  "<<sensor.mpl115a1Coeffs[4]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[5]  "<<sensor.mpl115a1Coeffs[5]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[6]  "<<sensor.mpl115a1Coeffs[6]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[7]  "<<sensor.mpl115a1Coeffs[7]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[8]  "<<sensor.mpl115a1Coeffs[8]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[9]  "<<sensor.mpl115a1Coeffs[9]<< endl <<unlock;
        */

       /* diagChannel << lock << DEBUG << " Coeffs[10] "<<sensor.mpl115a1Coeffs[10]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Coeffs[11] "<<sensor.mpl115a1Coeffs[11]<< endl <<unlock;*/
       // diagChannel << lock << DEBUG << " Temperature[0] "<<sensor.mpl115a1Temperature[0] << endl <<unlock;
       /* diagChannel << lock << DEBUG << " Temperature[1] "<<sensor.mpl115a1Temperature[1]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Pressure[0] "<<sensor.mpl115a1Pressure[0]<< endl <<unlock;
        diagChannel << lock << DEBUG << " Pressure[1] "<<sensor.mpl115a1Pressure[1]<< endl <<unlock;*/




    while (1) {
        {;}
    }

    return 0;
}
