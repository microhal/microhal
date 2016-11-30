/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 21-02-2014
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

#ifndef SHT21_H_
#define SHT21_H_

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"

/**
 * \addtogroup Devices
 * @{
 * @class SHT21
 * @}
 */
class SHT21: public microhal::I2CDevice {
    /**
     * Registers flags
     */
    enum Flags {
        USER_REGISTER_ENABLE_ON_CHIP_HEATER = 0x04, //!< USER_REGISTER_ENABLE_ON_CHIP_HEATER
    };
    /**
     * Possible SHT21 command
     */
    enum PossibleCommands {
        TRIGGER_T_MEASURE_HOLD = 0xE3,    //!< TRIGGER_T_MEASURE_HOLD
        TRIGGER_RH_MEASURE_HOLD = 0xE5,   //!< TRIGGER_RH_MEASURE_HOLD
        TRIGGER_T_MEASURE_NO_HOLD = 0xF3, //!< TRIGGER_T_MEASURE_NO_HOLD
        TRIGGER_RH_MEASURE_NO_HOLD = 0xF5, //!< TRIGGER_RH_MEASURE_NO_HOLD
        WRITE_USER_REGISTER_CMD = 0xE6,   //!< WRITE_USER_REGISTER_CMD
        READ_USER_REGISTER_CMD = 0xE7,    //!< READ_USER_REGISTER_CMD
        SOFT_RESET_CMD = 0xFE             //!< SOFT_RESET_CMD
    };
    /**
     * Possible I2C address
     */
    enum PossibleI2CAddress {
        DEFAULT_ADDRESS = 0x80             //!< DEFAULT_ADDRESS
    };
public:
    /** @brief Possible temperature and humidity resolution
     *
     *
     *
     */
    typedef enum {
        RESOLUTION_12_14 = 0x00, //!< RESOLUTION_12_14
        RESOLUTION_8_12 = 0x01, //!< RESOLUTION_8_12
        RESOLUTION_10_13 = 0x80, //!< RESOLUTION_10_13
        RESOLUTION_11_11 = 0x81, //!< RESOLUTION_11_11
        UNKNOWN
    } Resolution;
    //---------------------------------------- constructors ---------------------------------------
    inline SHT21(microhal::I2C &i2c) :
                I2CDevice(i2c, DEFAULT_ADDRESS) {
    }
    //------------------------------------------ functions ----------------------------------------
    bool reset();

    bool startTemperatureConversion() {
    	return write(TRIGGER_T_MEASURE_HOLD);
    }

    bool readTemperature(float &temperature){
    	uint8_t temp;

    	if (read(temp) == true) {
    		temperature = (float) temp * 0.002681274;
    		temperature -= 46.85;

    		return true;
    	}
    	return false;
    }

    /** @brief This function read temperature in celsius degree from SHT21 device. I2C data buss is lock until measurement is doing.
     *
     * @param[out] temperature in celcius degree
     * @retval true if temperature read correctly.
     * @retval false if an error occurred.
     */
    bool getTemperature(float &temperature) {
        uint16_t temp;

        if (readRegister(TRIGGER_T_MEASURE_HOLD, temp, microhal::BigEndian) == true) {
            temperature = (float) temp * 0.002681274;
            temperature -= 46.85;

            return true;
        }
        return false;
    }
    /** This function read humidity from SHT21 device. I2C data buss is lock until measurement is doing.
     *
     * @param[out] humidity
     * @retval true if humidity read successful.
     * @retval false if an error occurred.
     */
    bool getHumidity(float &humidity) {
        uint16_t tmp;

        if (readRegister(TRIGGER_RH_MEASURE_HOLD, tmp, microhal::BigEndian) == true) {
            humidity = (float) tmp * 0.001907349;
            humidity -= 6;

            return true;
        }
        return false;
    }

    bool heaterEnable();
    bool heaterDisable();

    bool setResolution(Resolution resolution);
    Resolution getResolution();
};
#endif /* SHT21_H_ */
