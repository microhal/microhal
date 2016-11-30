///**
// * @license    BSD 3-Clause
// * @copyright  microHAL
// * @version    $Id$
// * @brief      LIS2DH12 device driver
// *
// * @authors    Pawel Okas
// * created on: 7-02-2016
// * last modification: <DD-MM-YYYY>
// *
// * @copyright Copyright (c) 2016, microHAL
// * All rights reserved.
// *
// * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// *
// *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
// *        documentation and/or other materials provided with the distribution.
// *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
// *        software without specific prior written permission.
// *
// * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
// * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// */
//
//#ifndef _LIS2DH12_H_
//#define _LIS2DH12_H_
//
//#include <stdint.h>
//
//#include "microhal.h"
//#include "I2CDevice/I2CDevice.h"
//#include "units/temperature.h"
//#include "units/acceleration.h"
//#include "math/vector.h"
//
//class LIS2DH12 : private microhal::I2CDevice {
// public:
//    using Acceleration = microhal::Vector<microhal::Acceleration, 3>;
//
//    typedef enum : uint8_t {
//        X = 0x01,
//        Y = 0x02,
//        Z = 0x04,
//        XY = X | Y,  //!< XY
//        XZ = X | Z,  //!< XZ
//        YZ = Y | Z,  //!< YZ
//        XYZ = X | Y | Z,  //!< XYZ
//        AxisUnknown
//    } Axis;
//
//    enum class Mode {
//        Normal = 0x00,
//        LowPower = 0x08,
//        PowerDown
//    };
//
//    typedef enum : uint8_t {
//        ODR_1Hz = 0x10,
//        ODR_10Hz,
//        ODR_25Hz,
//        ODR_50Hz,
//        ODR_100Hz,
//        ODR_200Hz,
//        ODR_400Hz,
//        ODR_1620Hz,
//        ODR_1344Hz,
//        ODR_5376Hz,
//    } ODR;
//
//    enum class FullScale : uint8_t {
//    	Range_2g = 0x00,
//    	Range_4g = 0x10,
//		Range_8g = 0x20,
//		Range_16g = 0x30
//    };
//    /**
//    * @brief This enum describes possible I2C address
//    */
//    typedef enum : microhal::I2C::DeviceAddress {
//        I2C_ADDRESS_0 = 0b00110000,  /// for MAG3110FCR1 device
//        I2C_ADDRESS_1 = 0b00110010,  /// for FXMS3110CDR1 device
//    } PossibleI2CAddress;
//
// private:
//    using Endianness = microhal::Endianness;
//    template <typename AddressType, AddressType address, Endianness... endianness>
//    using RegisterAddress = microhal::RegisterAddress<AddressType, address, endianness...>;
//    template <typename AddressType, typename DataType, Endianness... DataEndianness>
//    using DeviceRegister = microhal::DeviceRegister<AddressType, DataType, DataEndianness...>;
//    /**
//    *  Value of fixed registers.
//    */
//    enum ConstRegisterValues {
//        WHO_AM_I_VALUE = 0x33,                         //!< ID VALUE
//    };
//
//    enum class STATUS_REG_AUX_Flag {
//        TOR = 0b01000000,
//        TDA = 0b00000100
//    };
//
//    enum class TMP_CFG_REG_Flag : uint8_t {
//        TEMP_EN0 = 0b01000000,
//        TEMP_EN1 = 0b10000000
//    };
//
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x07>, const uint8_t> STATUS_REG_AUX = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x0C>, const int16_t, Endianness::BigEndian> OUT_TEMP = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x0E>, const int16_t, Endianness::BigEndian> INT_COUNTER_REG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x0F>, const uint8_t> WHO_AM_I = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x1F>, TMP_CFG_REG_Flag> TEMP_CFG_REG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x20>, uint8_t> CTRL_REG1 = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x21>, uint8_t> CTRL_REG2 = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x22>, uint8_t> CTRL_REG3 = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x23>, uint8_t> CTRL_REG4 = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x24>, uint8_t> CTRL_REG5 = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x25>, uint8_t> CTRL_REG6 = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x26>, uint8_t> REFERENCE_DATACAPTURE = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x27>, const uint8_t> STATUS_REG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x28>, const int16_t, Endianness::BigEndian> OUT_X = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x2A>, const int16_t, Endianness::BigEndian> OUT_Y = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x2C>, const int16_t, Endianness::BigEndian> OUT_Z = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x2E>, uint8_t> FIFO_CTRL_REG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x2F>, const uint8_t> FIFO_SRC_REG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x30>, uint8_t> INT1_CFG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x31>, const uint8_t> INT1_SRC = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x32>, uint8_t> INT1_THS = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x33>, uint8_t> INT1_DURATION = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x34>, uint8_t> INT2_CFG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x35>, const uint8_t> INT2_SRC = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x36>, uint8_t> INT2_THS = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x37>, uint8_t> INT2_DURATION = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x38>, uint8_t> CLICK_CFG = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x39>, uint8_t> CLICK_SRC = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x3A>, uint8_t> CLICK_THS = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x3B>, uint8_t> TIME_LIMIT = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x3C>, uint8_t> TIME_LATENCY = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x3D>, uint8_t> TIME_WINDOW = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x3E>, uint8_t> Act_THS = {};
//    static constexpr DeviceRegister<RegisterAddress<uint8_t, 0x3F>, uint8_t> Act_DUR = {};
//
// public:
//    LIS2DH12(microhal::I2C &i2c, PossibleI2CAddress address) : I2CDevice(i2c, address) {
//    }
//    virtual ~LIS2DH12();
//
//    bool present() {
//        uint8_t who_am_i;
//        if (readRegister(WHO_AM_I, who_am_i)) {
//            return who_am_i == WHO_AM_I_VALUE;
//        }
//        return false;
//    }
//
//    bool readTemperature(microhal::Temperature& temperature) {
//         int16_t temp;
//         bool status = readRegister(OUT_TEMP, temp);
//         temperature.setCelsius(static_cast<int32_t>(temp));
//         return status;
//    }
//
//    bool isNewTemperatureAvailable() {
//        uint8_t status;
//        readRegister(STATUS_REG_AUX, status);
//        return status & static_cast<uint8_t>(STATUS_REG_AUX_Flag::TDA);
//    }
//
//    bool temperatureSensorEnable() {
//        return writeRegister(TEMP_CFG_REG, static_cast<TMP_CFG_REG_Flag>(
//        		static_cast<uint8_t>(TMP_CFG_REG_Flag::TEMP_EN1) | static_cast<uint8_t>(TMP_CFG_REG_Flag::TEMP_EN0)));
//    }
//
//    bool temperatureSensorDisable() {
//        return writeRegister(TEMP_CFG_REG, static_cast<TMP_CFG_REG_Flag>(0x00));
//    }
//
//    bool enableAxis(Axis axis) {
//        return setBitsInRegister(CTRL_REG1, static_cast<uint8_t>(axis));
//    }
//
//    bool disableAxis(Axis axis) {
//        return clearBitsInRegister(CTRL_REG1, static_cast<uint8_t>(axis));
//    }
//
//    bool setMode(Mode mode) {
//        return modifyBitsInRegister(CTRL_REG1, static_cast<uint8_t>(mode), static_cast<uint8_t>(Mode::LowPower));
//    }
//
//    Mode getMode() {
//        uint8_t ctrl;
//        readRegister(CTRL_REG1, ctrl);
//        return static_cast<Mode>(ctrl & static_cast<uint8_t>(Mode::LowPower));
//    }
//
//    bool setODR(ODR odr) {
//    	return writeRegister(CTRL_REG1, static_cast<uint8_t>(odr));
//    }
//
//    bool readAcceleration(Acceleration &acceleration) {
//    	int16_t accel[3];
//    	bool status = readRegisterMultipleData(OUT_X.getAddress(), gsl::span<int16_t>{accel});
//
//    	float scale = 1.0;
//    	acceleration[0] = microhal::Acceleration (static_cast<float>(accel[0]) * scale);
//    	return status;
//    }
//
//    bool setFullScale(FullScale scale) {
//
//    }
//
// private:
//    ODR odr;
//};
//
//#endif  // _LIS2DH12_H_
