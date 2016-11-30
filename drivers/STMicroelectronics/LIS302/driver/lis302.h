/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Driver for LIS302DL accelerometr

 * @authors    Pawel
 * created on: 10-11-2015
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
#ifndef _MICROHAL_LIS302_H_
#define _MICROHAL_LIS302_H_

#include "microhal.h"
#include "SPIDevice/SPIDevice.h"
#include "units/acceleration.h"
#include "math/vector.h"

class LIS302DL: private microhal::SPIDevice {
    /**
     * @brief LIS302DL registers address
     */
    enum Registers {
        WHO_AM_I = 0x0F,        //!< WHO_AM_I
        CTRL_REG1 = 0x20,       //!< CTRL_REG1
        CTRL_REG2 = 0x21,       //!< CTRL_REG2
        CTRL_REG3 = 0x22,       //!< CTRL_REG3
        HP_FILTER_RESET = 0x23,  //!< HP_FILTER_RESET
        STATUS_REG = 0x27,      //!< STATUS_REG
        OUTX = 0x29,            //!< OUTX
        OUTY = 0x2B,            //!< OUTY
        OUTZ = 0x2D,            //!< OUTZ
        // reserved 2E-2F
        FF_WU_CFG_1 = 0x30,     //!< FF_WU_CFG_1
        FF_WU_SRC_1 = 0x31,     //!< FF_WU_SRC_1
        FF_WU_THS_1 = 0x32,     //!< FF_WU_THS_1
        FF_WU_DURATION_1 = 0x33,     //!< FF_WU_DURATION_1
        FF_WU_CFG_2 = 0x34,     //!< FF_WU_CFG_2
        FF_WU_SRC_2 = 0x35,     //!< FF_WU_SRC_2
        FF_WU_THS_2 = 0x36,     //!< FF_WU_THS_2
        FF_WU_DURATION_2 = 0x37,     //!< FF_WU_DURATION_2
        CLICK_CFG = 0x38,       //!< CLICK_CFG
        CLICK_SRC = 0x39,       //!< CLICK_SRC
        // 0x3A unused
        CLICK_THSY_X = 0x3B,    //!< CLICK_THSY_X
        CLICK_THSZ = 0x3C,      //!< CLICK_THSZ
        CLICK_TIME_LIMIT = 0x3D,  //!< CLICK_TIME_LIMIT
        CLICK_LATENCY = 0x3E,    //!< CLICK_LATENCY
        CLICK_WINDOW = 0x3F     //!< CLICK_WINDOW
    };
    /**
     * @brief Status register flag - measure completion
     */
    enum RegisterFlags {
        STATUS_REG_ZYXOR = 0x80,     //!< STATUS_REG_ZYXOR
        STATUS_REG_ZOR = 0x40,  //!< STATUS_REG_ZOR
        STATUS_REG_YOR = 0x20,  //!< STATUS_REG_YOR
        STATUS_REG_XOR = 0x10,  //!< STATUS_REG_XOR
        STATUS_REG_ZYXDA = 0x08,  //!< STATUS_REG_ZYXDA
        STATUS_REG_ZDA = 0x04,  //!< STATUS_REG_ZDA
        STATUS_REG_YDA = 0x02,  //!< STATUS_REG_YDA
        STATUS_REG_XDA = 0x01,  //!< STATUS_REG_XDA
    };
    /**
     *  @brief Serial interface command
     */
    enum Commands {
        READ = 0x80     //!< READ
    };

 public:
    /**
     *  Value of fixed registers.
     */
    enum ConstRegisterValues {
        WHO_AM_I_VALUE = 0x3B,  //!< WHO_AM_I VALUE
    };

    /**
     * @brief Possible setting of Output Data Rate - measure frequency
     */
    typedef enum {
        ODR_100Hz = 0x00,  //!< ODR_100HZ
        ODR_400Hz = 0x80,  //!< ODR_400HZ
        ODR_Unknown      //!< ODR_UNKNOWN
    } Odr;
    /**
     * Possible operating modes
     */
    typedef enum {
        PowerDown = 0x00,      //!< POWER_DOWN
        Active = 0x40,     //!< ACTIVE
        ModeUnknown
    } Mode;
    /**
     * @brief Possible axis configuration
     */
    typedef enum {
        X = 0x01,          //!< X
        Y = 0x02,          //!< Y
        Z = 0x04,          //!< Z
        XY = X | Y,  //!< XY
        XZ = X | Z,  //!< XZ
        YZ = Y | Z,  //!< YZ
        XYZ = X | Y | Z,  //!< XYZ
        AxisUnknown
    } Axis;
    /**
     * @brief Avaliable measure ranges
     */
    typedef enum {
        Range_2g = 0x00,  // Range +- 2g
        Range_8g = 0x20,  // range +- 4g
        Range_Unknown
    } Range;

	 /**
     * @brief Measured acceleration is returned as 3D vector of @ref Acceleration
     */
    using AccelerationVector = microhal::Vector<microhal::Acceleration, 3>;

	 /**
     *  \brief Constructor for LIS302DL driver
     *  
     *  \param [in] spi SPI device
     *  \param [in] CEpin CE pin
     *  
     *  \details Object is constructed with default settings. Device is inactive.
     */
    LIS302DL(microhal::SPI &spi, const microhal::GPIO::IOPin CEpin) :
        SPIDevice(spi, CEpin), m_axis(AxisUnknown), m_range(Range_2g) {
    }

     /**
     *  \brief Function initializes LIS302DL driver.
     *  
     *  \return True if device is present at provided SPI interface,
     *  	 is active and respond with proper identification value. False otherwise.
     *  
     */ 
    bool init();

    /**
     *  \brief Function sets operation mode of device.
     *  
     *  \param [in] mode Mode of operation - @ref Mode .
     *  \return True if setting mode was successful. False otherwise.
     *  
     */
    bool setMode(Mode mode);

     /**
     *  \brief Get actual operation mode from device.
     *  
     *  \return Mode of operation - @ref Mode .
     *  	 If driver is unable to read mode, @ref ModeUnknown is returned.
     *  
     */
    Mode getMode();

    /**
     *  \brief Set measurement rate.
     *  
     *  \param [in] odr Output data rate - @ref Odr .
     *  \return True if setting ODR was successful. False otherwise.
     *  
     */	
    bool setODR(Odr odr);

    /**
     *  \brief Get measurement rate.
     *  
     *  \return Output data rate - @ref Odr .
     *  	 If driver is unable to read ODR, @ref ODR_Unknown is returned. 
     */	
    Odr getODR();

    /**
     *  \brief Enable measurement on particular axis.
     *  
     *  \param [in] axis Axis (its combination), where measurement will be performed - @ref Axis.
     *  \return True if setting Axis was successful. False otherwise.
     *  
     */	
    bool enableAxis(Axis axis);

    /**
     *  \brief Get currently enable measurement Axis.
     *  
     *  \return @ref Axis.
     *  	 If driver is unable to read Axis, @ref AxisUnknown is returned.  
     *  
     */	
    Axis getActiveAxis();

    /**
     *  \brief Set measurement range of device
     *  
     *  \param [in] range Range of measurements - @ref Range.
     *  \return True if setting Range was successful. False otherwise.
     *  
     */
    bool setRange(Range range);

    /**
     *  \brief Get current measurement range setting.
     *  
     *  \return @ref Range. 
     *  	 If driver is unable to read Range, @ref Range_Unknown is returned.  
     */	
    Range getRange();

    /**
     *  \brief Function reads acceleration measurement from device.
     *  
     *  \param [in] acc Reference to @ref AccelerationVector, where measurement will be stored.
     *  			If driver is unable to read measurements or they are not ready (for all configured axis),
     *  			result wont be writen.
     *  
     *  \return True if reading measurements was successful (all Axis values are obtained). False otherwise.
     *  
     *  \details Returned values of acceleration on each axis are express in g unit.
     *  		Conversion from read values to acceleration is performed according to current settings of range
     *  	    (using scale factors from datasheet).
     */	
    bool getAcceleration(AccelerationVector* acc);

 private:
    Axis m_axis;
    Range m_range;

    void convertToAcceleration(AccelerationVector* acc,
                               int8_t x, int8_t y, int8_t z);

    uint8_t getID();

    int8_t getX();
    int8_t getY();
    int8_t getZ();

    enum SENSIVITY_RANGE2 {
        SENSIVITY_RAGNE2_MULTIPLIER = 18,
        SENSIVITY_RAGNE2_DIVIDER = 1000
    };
    enum SENSIVITY_RANGE8 {
        SENSIVITY_RAGNE8_MULTIPLIER = 72,
        SENSIVITY_RAGNE8_DIVIDER = 1000
    };
};
#endif  // _MICROHAL_LIS302_H_
