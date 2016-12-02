/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      example for LSM330DL driver

 @authors    Pawel
 created on: 16-10-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
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

 *//* ========================================================================================================================== */

#include "microhal.h"
#include "diagnostic/diagnostic.h"
#include "lsm330dl.h"
#include "microhal_bsp.h"


using namespace microhal;
using namespace diagnostic;

int main(void) {
	debugPort.clear();

	debugPort.setDataBits(SerialPort::Data8);
	debugPort.setStopBits(SerialPort::OneStop);
	debugPort.setParity(SerialPort::NoParity);
	debugPort.open(SerialPort::ReadWrite);
	debugPort.setBaudRate(SerialPort::Baud115200);

	debugPort.write("\n\r------------------- LSM330DL Demo -------------------------\n\r");

    diagChannel.setOutputDevice(debugPort);

    lsm330::Accelerometer lsm(sensorI2C, lsm330::Accelerometer::I2C_ADDRESS_1);

    lsm330::Gyroscope gyro(sensorI2C, lsm330::Gyroscope::I2C_ADDRESS_1);


    lsm.registerDump(diagChannel);
    gyro.registerDump(diagChannel);

    diagChannel << MICROHAL_DEBUG << "Initializing device...";
    if(lsm.init(false, lsm330::Accelerometer::ODR_400Hz,
    		lsm330::Accelerometer::Axis::X | lsm330::Accelerometer::Axis::Y | lsm330::Accelerometer::Axis::Z)){
    	diagChannel << Debug << "OK";
    } else {
    	diagChannel << Debug << "Failed.";
    	diagChannel << MICROHAL_ERROR << "Unable to initialize LSM330 device.";
    }

    diagChannel << MICROHAL_DEBUG << "Setting accelerometer full scale range...";
    if(lsm.setFullScaleRange(lsm330::Accelerometer::FSR_16g)){
    	diagChannel << Debug << "OK";
    } else {
    	diagChannel << Debug << "Failed.";
    	diagChannel << MICROHAL_ERROR << "Unable to set LSM330 accelerometer full scale range.";
    }

    diagChannel << MICROHAL_DEBUG << "Initializing LSM330 gyroscope...";
    if(gyro.init(lsm330::Gyroscope::Mode::NormalOrSleep, lsm330::Gyroscope::ODR_400Hz_cutoff_50Hz,
    		lsm330::Gyroscope::Axis::X | lsm330::Gyroscope::Axis::Y | lsm330::Gyroscope::Axis::Z)){
		diagChannel << Debug << "OK";
	} else {
		diagChannel << MICROHAL_ERROR << "Unable to initialize LSM330 device.";
	}

    diagChannel << MICROHAL_DEBUG << "Setting gyroscope full scale range...";
    if(gyro.setFullScaleRange(lsm330::Gyroscope::FSR_2000dps)){
    	diagChannel << Debug << "OK";
    } else {
    	diagChannel << MICROHAL_ERROR << "Unable to set LSM330 gyroscope full scale range.";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds {4000});

    int16_t x, y, z;
	while (1) {
		if(lsm.isNewDataAvailable(lsm330::Accelerometer::Axis::X | lsm330::Accelerometer::Axis::Y | lsm330::Accelerometer::Axis::Z)){
		   if(lsm.readSamples(x, y, z) == true){
			   diagChannel << Debug << "X acceleration: " << x << endl << "Y acceleration: " << y << endl << "Z acceleration: " << z << endl;
		   } else {
			   diagChannel << MICROHAL_ERROR << "Can't read sample.";
			   lsm.registerDump(diagChannel);
		   }
		} else {
			diagChannel << MICROHAL_ERROR << "No new data.";
			lsm.registerDump(diagChannel);
		}

	   if(gyro.isNewDataAvailable(lsm330::Gyroscope::Axis::X | lsm330::Gyroscope::Axis::Y | lsm330::Gyroscope::Axis::Z)){
		   if(gyro.readSamples(x, y, z)){
			   diagChannel << Debug << "X gyro: " << x << endl << "Y gyro: " << y << endl << "Z gyro: " << z << endl;
			} else {
				diagChannel << MICROHAL_ERROR << "Can't read sample.";
				gyro.registerDump(diagChannel);
			}
		} else {
			diagChannel << Debug << "New sample unavailable.";
			gyro.registerDump(diagChannel);
		}

	   std::this_thread::sleep_for(std::chrono::milliseconds {1000});
	}
}
