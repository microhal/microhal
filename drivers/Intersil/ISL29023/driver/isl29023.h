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

#ifndef ISL29023_H_
#define ISL29023_H_

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"

/**
 * \addtogroup Devices
 * @{
 * @class ISL29023
 * @}
 */
class ISL29023 : public microhal::I2CDevice {
    /**
     * @brief ISL29023 registers.
     */
    enum Registers {
        COMMAND_1 = 0x00,       //!< Command 1 8-bit register
        COMMAND_2 = 0x01,      //!< Command 2 8-bit register
        DATA = 0x02,  //!< LSB first 16-bit data register
        INT_LT = 0x04,  //!< LSB first 16-bit lower interrupt threshold register
        INT_HT = 0x06,  //!< LSB first 16-bit upper interrupt threshold register
        TEST = 0x08
    };
public:
    /**
     * Possible data ranges
     */
    typedef enum {
        Range1 = 0x00,  //!< range_1
        Range2 = 0x01,  //!< range_2
        Range3 = 0x02,  //!< range_3
        Range4 = 0x03, //!< range_4
        RangeUnknown
    } FullScalleRange;
    /**
     *  Possible operating modes
     */
    typedef enum {
        Mode_PowerDown = 0x00,                        //!< mode_power_down
        Mode_AlsOnce = 0x20,
        Mode_IrOnce = 0x40,
        Mode_AlsContinuous = 0xa0,
        Mode_IrContinuous = 0xb0,
        Mode_Unknown
    } OperatimgModes;
    /**
     *
     */
    typedef enum {
        Resolution_16bit = 0x00, //!< RESOLUTION_16BIT
        Resolution_12bit = 0x04, //!< RESOLUTION_12BIT
        Resolution_8bit = 0x08, //!< RESOLUTION_8BIT
        Resolution_4bit = 0x0C, //!< RESOLUTION_4BIT
        Resolution_Unknown
    } Resolution;
    /**
     * @brief possible I2C slave address.
     */
    enum PossibleI2CAddress {
        Default_I2C_Address = 0x88,                         //!< DEFAULT_I2C_ADDRESS
    };

    float alfa;
//------------------------------------------ functions ----------------------------------------
    ISL29023(microhal::I2C &i2c) :
    	I2CDevice(i2c, Default_I2C_Address), alfa(0.015258789) {
    }

    bool reset();

    bool setMode(OperatimgModes mode);
    OperatimgModes getMode();

    bool setRange(FullScalleRange range);
    FullScalleRange getRange();

    bool setResolution(Resolution resolution);
    Resolution getResolution();

    inline bool setIntLowThreshold(uint16_t threshold);
    inline uint16_t getIntLowThreshold();

    inline bool setIntHighThreshold(uint16_t threshold);
    inline uint16_t getIntHighThreshold();

    inline bool getLux(float &lux);


private:
    bool calibrate();
};
//*************************************************************************************************
// INLINE FUNCTIONS
//*****************
/** @brief This function set low interrupt threshold.
 *
 * @param[in] threshold new low threshold to set.
 * @return
 */
bool ISL29023::setIntLowThreshold(uint16_t threshold) {
    return writeRegister(INT_LT, threshold, microhal::LittleEndian);
}
/**
 *
 * @return
 */
uint16_t ISL29023::getIntLowThreshold() {
    uint16_t threshold;

    if (readRegister(INT_LT, threshold, microhal::LittleEndian) == true) {
        return threshold;
    } else {
        return 0;
    }
}
/** @brief This function set high interrupt threshold.
 *
 * @param[in] threshold new high threshold to set.
 * @return
 */
bool ISL29023::setIntHighThreshold(uint16_t threshold) {
    return writeRegister(INT_HT, threshold, microhal::LittleEndian);
}
/**
 *
 * @return
 */
uint16_t ISL29023::getIntHighThreshold() {
    uint16_t threshold;

    if (readRegister(INT_HT, threshold, microhal::LittleEndian) == true) {
        return threshold;
    } else {
        return 0;
    }
}
/** @brief This function read actual light density.
 *
 * @param[out] lux - measured light density in lux
 * @return
 */
bool ISL29023::getLux(float &lux) {
    uint16_t data;

    if (readRegister(DATA, data, microhal::LittleEndian) == true) {
        lux = static_cast<float>(data) * alfa;
        return true;
    } else {
        return false;
    }
}
#endif /* ISL29023_H_ */
