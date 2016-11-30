/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 22-02-2014
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

#ifndef TMP006_H_
#define TMP006_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "units/temperature.h"
#include <cmath>

/**
 * \addtogroup Devices
 * @{
 * @class TMP006
 * @}
 */
class TMP006: public microhal::I2CDevice {
    /**
     *  Device flags
     */
    enum Flags {
        CONFIGURATION_RESET = (uint16_t) 0x8000,          //!< CONFIGURATION_RESET
        CONFIGURATION_MODE_POWER_DOWN = (uint16_t) 0x0000,        //!< CONFIGURATION_MODE_POWER_DOWN
        CONFIGURATION_MODE_ACTIVE = (uint16_t) 0x0700,    //!< CONFIGURATION_MODE_ACTIVE
        CONFIGURATION_DRDY_ENABLE = (uint16_t) 0x0080,    //!< CONFIGURATION_DRDY_ENABLE
        CONFIGURATION_DRDY_STATUS = (uint16_t) 0x0080,    //!< CONFIGURATION_DRDY_STATUS
    };
    /**
     * \brief TMP006 registers.
     */
    enum Registers {
        V_OBJECT = 0x00,       //!< V_OBJECT
        T_AMBIENT = 0x01,      //!< T_AMBIENT
        CONFIGURATION = 0x02,  //!< CONFIGURATION
        MANUFACTURER_ID = 0xFE,  //!< MANUFACTURER_ID
        DEVICE_ID = 0xFF       //!< DEVICE_ID
    };
public:
    /**
     * \brief Possible conversion rates.
     */
    typedef enum ConversionRates {
        ConversionRate_4Hz = 0x0000,   //!< conversion rate = 4Hz
        ConversionRate_2Hz = 0x0200,   //!< conversion rate = 2Hz
        ConversionRate_1Hz = 0x0400,   //!< conversion rate = 1Hz
        ConversionRate_0_5Hz = 0x0600, //!< conversion rate = 0.5Hz
        ConversionRate_0_25Hz = 0x0800, //!< conversion rate = 0.25Hz
        ConversionRateUnknown           //!< UNKNOWN
    } ConversionRates;
    /**
     *  Possible operating modes
     */
    typedef enum {
        ModePowerDown = CONFIGURATION_MODE_POWER_DOWN,                        //!< MODE_POWER_DOWN
        ModeActive = CONFIGURATION_MODE_ACTIVE,         //!< MODE_ACTIVE
        ModeUnknown
    } OperatimgModes;
    /**
     * \brief possible I2C slave address.
     */
    enum PossibleI2CAddress {
        I2C_ADDRESS_1 = 0x80,                         //!< I2C_ADDRESS_1
        I2C_ADDRESS_2 = 0x82,                         //!< I2C_ADDRESS_2
        I2C_ADDRESS_3 = 0x84,                         //!< I2C_ADDRESS_3
        I2C_ADDRESS_4 = 0x86,                         //!< I2C_ADDRESS_4
        I2C_ADDRESS_5 = 0x88,                         //!< I2C_ADDRESS_5
        I2C_ADDRESS_6 = 0x8A,                         //!< I2C_ADDRESS_6
        I2C_ADDRESS_7 = 0x8C,                         //!< I2C_ADDRESS_7
        I2C_ADDRESS_8 = 0x8E,                         //!< I2C_ADDRESS_8
    };
    /**
     *  Value of fixed registers.
     */
    enum ConstRegisterValues {
        MANUFACTURER_ID_VALUE = 0x5449,                         //!< MANUFACTURER_ID_VALUE
        DEVICE_ID_VALUE = 0x0067       //!< DEVICE_ID_VALUE
    };
    //------------------------------------------ functions ----------------------------------------
    inline bool reset();
    inline bool isNewDataAvailable(void);

    inline void setCallibrationFactor(float factor);
    inline float getCallibrationFactor();

    microhal::Temperature getAmbientTemperature() {
    	return ambientTemperature;
    }

    microhal::Temperature getObjectTemperature() {
    	return objectTemperature;
    }
    inline uint16_t getManufacturerID();
    inline uint16_t getDeviceID();

    inline bool enableDRDYpin();
    inline bool disableDRDYpin();

    inline bool setMode(OperatimgModes mode);

    inline bool setConversionRate(ConversionRates rate);
    inline ConversionRates getConversionRate();
    /**
     *
     * @param i2c
     * @param address
     */
    TMP006(microhal::I2C &i2c, uint8_t address) :
            I2CDevice(i2c, address), S0(0.00000000000006) {
    }
private:
    float S0;
    microhal::Temperature ambientTemperature;
    microhal::Temperature objectTemperature;
//------------------------------------------ functions ----------------------------------------
    bool update();
};
//*************************************************************************************************
// INLINE FUNCTIONS
//*****************
/**
 * @brief This function set reset bit in configuration register.
 * @return true if sensor was reset;
 */
bool TMP006::reset() {
    return setBitsInRegister(CONFIGURATION, (uint16_t) CONFIGURATION_RESET, microhal::BigEndian);
}
/**
 * @brief This function enable the DRDY pin.
 * @return
 */
bool TMP006::enableDRDYpin() {
    return setBitsInRegister(CONFIGURATION, (uint16_t) CONFIGURATION_DRDY_ENABLE, microhal::BigEndian);
}
/**
 * @brief This function disable the DRDY pin.
 * @return
 */
bool TMP006::disableDRDYpin() {
    return clearBitsInRegister(CONFIGURATION, (uint16_t) CONFIGURATION_DRDY_ENABLE, microhal::BigEndian);
}
/**
 * @brief This function read Manufacturer ID from TMP006 sensor.
 * @return Manufacturer ID
 */
uint16_t TMP006::getManufacturerID() {
    uint16_t id;

    if (readRegister(MANUFACTURER_ID, id, microhal::BigEndian) == true) {
        return id;
    } else {
        return 0;
    }
}
/**
 * @brief This function read Device ID from TMP006 sensor.
 * @return Device ID
 */
uint16_t TMP006::getDeviceID() {
    uint16_t id;

    if (readRegister(DEVICE_ID, id, microhal::BigEndian) == true) {
        return id;
    } else {
        return 0;
    }
}
/**
 * @brief This function read actual conversion rate.
 * @return actual Conversion Rate
 */
TMP006::ConversionRates TMP006::getConversionRate() {
    uint16_t conf;

    if (readRegister(CONFIGURATION, conf, microhal::BigEndian) == true) {
        //clear other bits than conversion rate
        conf &= ConversionRate_4Hz | ConversionRate_2Hz | ConversionRate_1Hz
                | ConversionRate_0_5Hz | ConversionRate_0_25Hz;

        return (ConversionRates) conf;
    } else {
        return ConversionRateUnknown;
    }
}
/**
 * @brief This function check is new data available
 * @retval true if new data is available
 * @retval false if no new data or an error occurred
 */
bool TMP006::isNewDataAvailable(void) {
    uint16_t conf;
//read configuration register
    if (readRegister(CONFIGURATION, conf, microhal::BigEndian) == true) {
        //check DRDY bit
        if (conf & CONFIGURATION_DRDY_STATUS) {
            //new data available
            update();
            return true;
        }
    }
    return false;
}
/**
 * @brief This function device operating mode
 * @param[in] mode - mode to set
 * @return
 */
bool TMP006::setMode(OperatimgModes mode) {
    if (mode == ModeUnknown) return false;

    uint16_t conf;
//read configuration register
    if (readRegister(CONFIGURATION, conf, microhal::BigEndian) == true) {
        //clear actual configuration
        conf &= ~(ModePowerDown | ModeActive);
        //apply new configuration
        conf |= mode;
        //write configuration
        return writeRegister(CONFIGURATION, conf, microhal::BigEndian);
    }
    return false;
}
/**
 * @brief This function set temperature conversion rate
 * @param[in] rate - rate to set
 * @return
 */
bool TMP006::setConversionRate(ConversionRates rate) {
    if (rate == ConversionRateUnknown) return false;

    uint16_t conf;
//read configuration register
    if (readRegister(CONFIGURATION, conf, microhal::BigEndian) == true) {
        //clear actual configuration
        conf &= ~(ConversionRate_4Hz | ConversionRate_2Hz | ConversionRate_1Hz
                | ConversionRate_0_5Hz | ConversionRate_0_25Hz);
        //apply new configuration
        conf |= rate;
        //write configuration
        return writeRegister(CONFIGURATION, conf, microhal::BigEndian);
    }
    return false;
}
/**
 * @brief This function set the callibration factor see TMP006 User's Guide chapter 6 (variable S0)
 * @param factor
 */
void TMP006::setCallibrationFactor(float factor) {
    S0 = factor;
}
/**
 *
 * @return
 */
float TMP006::getCallibrationFactor() {
    return S0;
}
#endif /* TMP006_H_ */
