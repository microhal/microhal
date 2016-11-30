/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 14-10-2014
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

#ifndef MAG3110_H_
#define MAG3110_H_

#include <cstdint>
#include <experimental/optional>

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "units/magnetic.h"
#include "units/temperature.h"
#include "math/vector.h"


class MAG3110 : protected microhal::I2CDevice {
	using Endianness = microhal::Endianness;
	using RegisterAccess = microhal::RegisterAccess;

	typedef enum  : uint8_t {
        CTRL_REG2_RAW = 0x20,
        CTRL_REG2_AUTO_MRST_EN = 0x80
    } CTRL_REG2_Flags;

    typedef enum  : uint8_t {
    	 CTRL_REG1_AC = 0x01,
    } CTRL_REG1_Flags;

	static constexpr auto DR_STATUS = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x00>();
	static constexpr auto OUT_X = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int16_t, Endianness::BigEndian>().address<uint8_t, 0x01>();
	static constexpr auto OUT_Y = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int16_t, Endianness::BigEndian>().address<uint8_t, 0x03>();
	static constexpr auto OUT_Z = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int16_t, Endianness::BigEndian>().address<uint8_t, 0x05>();
	static constexpr auto WHO_AM_I = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x07>();
	static constexpr auto SYSMOD = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x08>();
	static constexpr auto OFF_X = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int16_t, Endianness::BigEndian>().address<uint8_t, 0x09>();
	static constexpr auto OFF_Y = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int16_t, Endianness::BigEndian>().address<uint8_t, 0x0B>();
	static constexpr auto OFF_Z = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int16_t, Endianness::BigEndian>().address<uint8_t, 0x0D>();
	static constexpr auto DIE_TEMP = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int8_t>().address<uint8_t, 0x0F>();
	static constexpr auto CTRL_REG1 = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x10>();
	static constexpr auto CTRL_REG2 = microhal::createDeviceRegister<RegisterAccess::ReadWrite, CTRL_REG2_Flags>().address<uint8_t, 0x11>();

 public:
    /**
    * @brief This enum describes possible I2C address
    */
    typedef enum : microhal::I2C::DeviceAddress {
        I2C_ADDRESS_0 = 0x0E << 1,  /// for MAG3110FCR1 device
        I2C_ADDRESS_1 = 0x0F << 1,  /// for FXMS3110CDR1 device
    } PossibleI2CAddress;
    /**
    *  Value of fixed registers.
    */
    enum ConstRegisterValues : uint8_t {
        WHO_AM_I_VALUE = 0xC4,                         //!< ID VALUE
    };
    /**
    * Possible operating mode.
    */
    typedef enum {
        STANDBY = 0x00,        //!< STANDBY
        ACTIVE_RAW =  0x01,        //!< ACTIVE
        ACTIVE_CORRECTED =  0x02,        //!< ACTIVE
    } Mode;

    typedef enum {
        X = 0x01,
        Y = 0x02,
        Z = 0x04,
        XY = X | Y,  //!< XY
        XZ = X | Z,  //!< XZ
        YZ = Y | Z,  //!< YZ
        XYZ = X | Y | Z,  //!< XYZ
        AxisUnknown
    } Axis;

    typedef enum : uint8_t {
        ODR_80Hz_OSR_16 = 0x00,
        ODR_40Hz_OSR_32 = 0x08,
        ODR_20Hz_OSR_64,
        ODR_10Hz_OSR_128,

        ODR_40Hz_OSR_16,
        ODR_20Hz_OSR_32,
        ODR_10Hz_OSR_64,
        ODR_5Hz_OSR_128,

        ODR_20Hz_OSR_16,
        ODR_10Hz_OSR_32,
        ODR_5Hz_OSR_64,
        ODR_2_5Hz_OSR_128,

        ODR_10Hz_OSR_16,
        ODR_5Hz_OSR_32,
        ODR_2_5Hz_OSR_64,
        ODR_1_25Hz_OSR_128,

        ODR_5Hz_OSR_16,
        ODR_2_5Hz_OSR_32,
        ODR_1_25Hz_OSR_64,
        ODR_0_63Hz_OSR_128,

        ODR_2_5Hz_OSR_16,
        ODR_1_25Hz_OSR_32,
        ODR_0_63Hz_OSR_64,
        ODR_0_31Hz_OSR_128,

        ODR_1_25Hz_OSR_16,
        ODR_0_63Hz_OSR_32,
        ODR_0_31Hz_OSR_64,
        ODR_0_16Hz_OSR_128,

        ODR_0_63Hz_OSR_16,
        ODR_0_31Hz_OSR_32,
        ODR_0_16Hz_OSR_64,
        ODR_0_8Hz_OSR_128,
    } OutputDataRate_OverSamplingRate;

 public:
    /**
     * @brief Measured acceleration is returned as 3D vector of @ref Magnetic
     */
    using MagneticVector = microhal::Vector<microhal::Magnetic, 3>;

	/**
	 * @brief Constructor of MAG3110 driver
	 * 
	 * @param i2c I2C device where MAG3110 is connected
	 * @param address MAG3110 I2C address @ref PossibleI2CAddress
	 */
    MAG3110(microhal::I2C &i2c, PossibleI2CAddress address) :
        I2CDevice(i2c, address) {
    }

    ~MAG3110() {
    }
	/**
	 * @brief Function initializes MAG3110 driver.
	 * @details It tries to communicate with chip and get its ID, which is compared expected value.
	 * @return true - if device is present, it is responding and its ID is proper. false otherwise. 
	 */
    bool init(void);
	/**
	 * @brief Set correction offest for magnetic field reading from X ax.
	 * 
	 * @param offset 
	 * @return true if setting was successful, false otherwise
	 */
    bool setXCorrection(int16_t offset) {
        return writeRegister(OFF_X, offset);
    }
	/**
	 * @brief Set correction offest for magnetic field reading from Y ax.
	 * 
	 * @param offset 
	 * @return true if setting was successful, false otherwise
	 */
    bool setYCorrection(int16_t offset) {
        return writeRegister(OFF_Y, offset);
    }
 	/**
	 * @brief Set correction offset for magnetic field reading from Z ax.
	 * 
 	 * 
	 * @param offset 
	 * @return true if setting was successful, false otherwise
	 */
    bool setZCorrection(int16_t offset) {
        return writeRegister(OFF_Z, offset);
    }
	/**
	 * @brief Set correction offset for magnetic field reading all axis.
	 * 
	 * @param x X ax offset
	 * @param y Y ax offset
	 * @param z Z ax offset
	 * @return true if setting was successful, false otherwise
	 */
    bool setCorrection(int16_t x, int16_t y, int16_t z);
	/**
	 * @brief Get X ax correction offset
	 * 
	 * @param offset value of offset will be stored here
	 * @return true if read was successful, false otherwise
	 */
    std::experimental::optional<int16_t> getXCorrection() {
        return readRegister(OFF_X);
    }
	/**
	 * @brief Get Y ax correction offset
	 * 
	 * @param offset value of offset will be stored here
	 * @return true if read was successful, false otherwise
	 */
    std::experimental::optional<int16_t> getYCorrection() {
        return readRegister(OFF_Y);
    }
	/**
	 * @brief Get Z ax correction offset
	 * 
	 * @param offset value of offset will be stored here
	 * @return true if read was successful, false otherwise
	 */
    std::experimental::optional<int16_t> getZCorrection() {
        return readRegister(OFF_Z);
    }
	/**
	 * @brief Get all axis correction offset at once
	 * 
	 * @param x X ax offset
	 * @param y Y ax offset
	 * @param z Z ax offset
	 * @return true if read was successful, false otherwise
	 */
    bool getCorrection(int16_t* x, int16_t* y, int16_t* z);


    std::experimental::optional<microhal::Temperature> getDieTemperature() {
    	std::experimental::optional<microhal::Temperature> temperature;
    	if (auto temp = readRegister(DIE_TEMP)) {
    		temperature = microhal::Temperature::fromCelcius(static_cast<float>(*temp));
        }
        return temperature;
    }

    bool setMode(Mode mode);
	/**
	 * @return Actual working mode or UNKNOWN_MODE if an error occurred
	 */
    std::experimental::optional<Mode> getMode() {
    	std::experimental::optional<Mode> mode;
    	if (auto sysmod = readRegister(SYSMOD)) {
            mode = static_cast<Mode>(*sysmod);
        }
        return mode;
    }

    bool setODR_OSR(OutputDataRate_OverSamplingRate odr_osr);

    std::experimental::optional <MagneticVector> getMagnetic();

    void convertToMagnetic(MagneticVector* mag, int16_t x, int16_t y, int16_t z) {
        float scale = 1.0;
        (*mag)[0] = microhal::Magnetic(static_cast<float>(x) * scale);
        (*mag)[1] = microhal::Magnetic(static_cast<float>(y) * scale);
        (*mag)[2] = microhal::Magnetic(static_cast<float>(z) * scale);
    }

    std::experimental::optional<int16_t> getX(void) {
        return readRegister(OUT_X);
    }

    std::experimental::optional<int16_t> getY(void) {
        return readRegister(OUT_Y);
    }

    std::experimental::optional<int16_t> getZ(void) {
        return readRegister(OUT_Z);
    }
};


#endif /* MAG3110_H_ */
