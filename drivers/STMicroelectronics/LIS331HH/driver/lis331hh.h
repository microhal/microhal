/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for LIS331HH

 @authors    Kubaszek Mateusz
 created on: 19.01.2017
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

 */ /* ==========================================================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       */

#ifndef LIS331HH_H_
#define LIS331HH_H_

#include <cstdint>
#include <experimental/optional>

#include "I2CDevice.h"
#include "math/vector.h"
#include "microhal.h"
#include "units/acceleration.h"

class LIS331HH : protected microhal::I2CDevice {
 public:
    using Acceleration = microhal::Acceleration;

    // constructor

    /**
    * @brief This enum describes possible I2C address
    */
    typedef enum : microhal::I2C::DeviceAddress {
        I2CaddressLowSA0 = 0b0011000 << 1,  /// SA0 = 0
        I2CaddressLowSA1 = 0b0011001 << 1   /// SA0 = 1
    } Address;

    /**
     * @brief Constructor of LIS331HH driver
     *
     * @param i2c I2C device where LIS331HH is connected
     * @param address LIS331HH I2C address @ref Address
     */
    LIS331HH(microhal::I2C &i2c, Address address) : I2CDevice(i2c, address), sensitivityMultiplyer(6.0 / static_cast<float>(INT16_MAX)) {}

    enum class Axis : uint8_t { X = 0x01, Y = 0x02, Z = 0x04, none = 0, all = X | Y | Z };
    enum class PowerMode : uint8_t {
        powerDown = 0,
        normalMode = 1,
        lowPower_05Hz = 2,
        lowPower_1Hz = 3,
        lowPower_2Hz = 4,
        lowPower_5Hz = 5,
        lowPower_10Hz = 6
    };
    enum class DataRate : uint8_t { normalMode_50Hz = 0, normalMode_100Hz = 1, normalMode_400Hz = 2, normalMode_1000Hz = 3 };

    enum class Sensitivity : uint8_t { sensitivity6g = 0, sensitivity12g = 1, sensitivity24g = 3 };

    bool init(Axis axisEnable, PowerMode powerMode, Sensitivity sensitivity, DataRate dataRate = DataRate::normalMode_50Hz);

    bool setAxis(Axis axis);
    bool setSpeed(PowerMode powerMode, DataRate dataRate);
    bool setSensitivity(Sensitivity sensitivity);

    bool dataAvailable(Axis &axis);
    bool dataOverrwritten(Axis &axis);

    std::experimental::optional<Acceleration> getAcceleration(Axis axis);
    bool getAcceleration(Acceleration &xAcceleration, Acceleration &yAcceleration, Acceleration &zAcceleration);

    enum class InterruptSource : uint8_t { lowValueOnly, highValueOnly, highAndLowValue };
    enum class InterruptConfiguration : uint8_t {
        ORcombinationOfEvents = 0,
        sixDirectionMovementRecognition = 1,
        ANDcombinationOfEvents = 2,
        sixDirectionPositionRecognition = 3
    };

    bool interruptIsAcitive(Axis &axis);
    bool interruptRead(Axis axis, InterruptSource &interruptSource);

    bool interruptConfigure(InterruptConfiguration interruptConfiguration);
    bool interruptConfigureSource(Axis axis, InterruptSource interruptSource);

    bool test(void);

 private:
    float sensitivityMultiplyer;

    // registers definitions
    static constexpr auto CTRL_REG1 = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x20>{});
    static constexpr auto CTRL_REG2 = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x21>{});
    static constexpr auto CTRL_REG3 = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x22>{});
    static constexpr auto CTRL_REG4 = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x23>{});
    static constexpr auto REFERENCE = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x26>{});
    static constexpr auto STATUS_REG = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x27>{});
    static constexpr auto OUT_X_L = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x28>{});
    static constexpr auto OUT_X_H = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x29>{});
    static constexpr auto OUT_X =
        microhal::makeRegister<int16_t, microhal::Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x28>{});
    static constexpr auto OUT_Y_L = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x2A>{});
    static constexpr auto OUT_Y_H = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x2B>{});
    static constexpr auto OUT_Y =
        microhal::makeRegister<int16_t, microhal::Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x2A>{});
    static constexpr auto OUT_Z_L = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x2C>{});
    static constexpr auto OUT_Z_H = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x2D>{});
    static constexpr auto OUT_Z =
        microhal::makeRegister<int16_t, microhal::Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x2C>{});
    static constexpr auto INT1_CFG = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x30>{});
    static constexpr auto INT1_SOURCE = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x31>{});
    static constexpr auto INT1_THS = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x32>{});
    static constexpr auto INT1_DURATION = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x33>{});
    static constexpr auto INT2_CFG = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x34>{});
    static constexpr auto INT2_SOURCE = microhal::makeRegister<uint8_t, microhal::Access::ReadOnly>(microhal::Address<uint8_t, 0x35>{});
    static constexpr auto INT2_THS = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x36>{});
    static constexpr auto INT2_DURATION = microhal::makeRegister<uint8_t, microhal::Access::ReadWrite>(microhal::Address<uint8_t, 0x37>{});

    // CTRL_REG1 configurations
    typedef enum : uint8_t { zen = 0x04, yen = 0x02, xen = 0x01 } CTRL_REG1_Flags;
    typedef enum : uint8_t { PM_mask = 0xe0, DR_mask = 0x18, axis_mask = 0x07 } CTRL_REG1_Masks;
    typedef enum : uint8_t { PM_offset = 5, DR_offset = 3 } CTRL_REG1_Offsets;

    // CTRL_REG2 configurations
    typedef enum : uint8_t {
        boot = 0x80,
        FDS = 0x10,
        HPen2 = 0x80,
        HPen1 = 0x40,
    } CTRL_REG2_Flags;
    typedef enum : uint8_t { HPM_mask = 0x60, HPCF_mask = 0x03 } CTRL_REG2_Masks;
    typedef enum : uint8_t { HPM_offset = 5, HPCF_offset = 0 } CTRL_REG2_Offsets;

    // CTRL_REG3 configurations
    typedef enum : uint8_t { IAH = 0x80, PP_OD = 0x40, LIR2 = 0x20, LIR1 = 0x02 } CTRL_REG3_Flags;
    typedef enum : uint8_t { I2_CFG_mask = 0x18, I1_CFG_mask = 0x03 } CTRL_REG3_Masks;
    typedef enum : uint8_t { I2_CFG_offset = 3, I1_CFG_offset = 0 } CTRL_REG3_Offset;
    typedef enum : uint8_t { onlyOneSource = 0, orSource = 1, dataReady = 2, bootRunning = 3 } CTRL_REG3_Interrupt;

    // CTRL_REG4 configurations
    typedef enum : uint8_t { BDU = 0x80, BLE = 0x40, STsign = 0x08, ST = 0x02, SIM = 0x01 } CTRL_REG4_Flags;
    typedef enum : uint8_t { FS_mask = 0x30 } CTRL_REG4_Masks;
    typedef enum : uint8_t { FS_offset = 4 } CTRL_REG4_Offset;

    // CTRL_REG5 configurations
    typedef enum : uint8_t { turnOn1 = 0x02, turnOn2 = 0x01 } CTRL_REG5_Flags;

    // STATUS_REG
    typedef enum : uint8_t {
        ZYXOR = 0x80,
        ZOR = 0x40,
        YOR = 0x20,
        XOR = 0x10,
        ZYXDA = 0x08,
        ZDA = 0x04,
        YDA = 0x02,
        XDA = 0x01,
    } STATUS_REG_Flags;

    // INTERRUPTS
    typedef enum : uint8_t { AOI = 0x80, _6D = 0x40, ZHIE = 0x20, ZLIE = 0x10, YHIE = 0x08, YLIE = 0x04, XHIE = 0x02, XLIE = 0x01 } INTX_CFG_Flags;
    typedef enum : uint8_t { IA = 0x40, ZH = 0x20, ZL = 0x10, YH = 0x08, YL = 0x04, XH = 0x02, XL = 0x01 } INTX_SRC_Flags;
};  // class LIS331HH

constexpr LIS331HH::Axis operator|(const LIS331HH::Axis x, const LIS331HH::Axis y) {
    return static_cast<LIS331HH::Axis>(static_cast<uint8_t>(x) | static_cast<uint8_t>(y));
}

constexpr LIS331HH::Axis operator|=(LIS331HH::Axis &x, LIS331HH::Axis y) {
    x = static_cast<LIS331HH::Axis>(static_cast<uint8_t>(x) | static_cast<uint8_t>(y));
    return x;
}

constexpr LIS331HH::Axis operator&(const LIS331HH::Axis x, const LIS331HH::Axis y) {
    return static_cast<LIS331HH::Axis>(static_cast<uint8_t>(x) & static_cast<uint8_t>(y));
}

constexpr LIS331HH::Axis operator&=(LIS331HH::Axis &x, LIS331HH::Axis y) {
    x = static_cast<LIS331HH::Axis>(static_cast<uint8_t>(x) & static_cast<uint8_t>(y));
    return x;
}

//
// class MAG3110 : protected microhal::I2CDevice {
//  using Endianness = microhal::Endianness;
//  using Access = microhal::Access;
//  using Error = microhal::I2C::Error;
//
//  typedef enum : uint8_t {
//    CTRL_REG2_RAW = 0x20,
//    CTRL_REG2_AUTO_MRST_EN = 0x80
//  } CTRL_REG2_Flags;
//
//  typedef enum : uint8_t {
//    CTRL_REG1_AC = 0x01,
//  } CTRL_REG1_Flags;
//
//  static constexpr auto DR_STATUS =
//      microhal::makeRegister<uint8_t, Access::ReadWrite>(
//          microhal::Address<uint8_t, 0x00>{});
//  static constexpr auto OUT_X =
//      microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Big>(
//          microhal::Address<uint8_t, 0x01>{});
//  static constexpr auto OUT_Y =
//      microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Big>(
//          microhal::Address<uint8_t, 0x03>{});
//  static constexpr auto OUT_Z =
//      microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Big>(
//          microhal::Address<uint8_t, 0x05>{});
//  static constexpr auto WHO_AM_I =
//      microhal::makeRegister<uint8_t, Access::ReadWrite>(
//          microhal::Address<uint8_t, 0x07>{});
//  static constexpr auto SYSMOD =
//      microhal::makeRegister<uint8_t, Access::ReadWrite>(
//          microhal::Address<uint8_t, 0x08>{});
//  static constexpr auto OFF_X =
//      microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Big>(
//          microhal::Address<uint8_t, 0x09>{});
//  static constexpr auto OFF_Y =
//      microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Big>(
//          microhal::Address<uint8_t, 0x0B>{});
//  static constexpr auto OFF_Z =
//      microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Big>(
//          microhal::Address<uint8_t, 0x0D>{});
//  static constexpr auto DIE_TEMP =
//      microhal::makeRegister<int8_t, Access::ReadWrite>(
//          microhal::Address<uint8_t, 0x0F>{});
//  static constexpr auto CTRL_REG1 =
//      microhal::makeRegister<uint8_t, Access::ReadWrite>(
//          microhal::Address<uint8_t, 0x10>{});
//  static constexpr auto CTRL_REG2 =
//      microhal::makeRegister<uint8_t, Access::ReadWrite>(
//          microhal::Address<uint8_t, 0x11>{});
//
// public:
//  /**
//  * @brief This enum describes possible I2C address
//  */
//  typedef enum : microhal::I2C::DeviceAddress {
//    I2C_ADDRESS_0 = 0x0E << 1,  /// for MAG3110FCR1 device
//    I2C_ADDRESS_1 = 0x0F << 1,  /// for FXMS3110CDR1 device
//  } PossibleI2CAddress;
//  /**
//  *  Value of fixed registers.
//  */
//  enum ConstRegisterValues : uint8_t {
//    WHO_AM_I_VALUE = 0xC4,  //!< ID VALUE
//  };
//  /**
//  * Possible operating mode.
//  */
//  enum class Mode {
//    Standby = 0x00,          //!< STANDBY
//    ActiveRAW = 0x01,        //!< ACTIVE
//    ActiveCorrected = 0x02,  //!< ACTIVE
//  };
//
//  enum class Axis {
//    x = 0x01,
//    y = 0x02,
//    z = 0x04,
//    xy = x | y,       //!< XY
//    xz = x | z,       //!< XZ
//    yz = y | z,       //!< YZ
//    xyz = x | y | z,  //!< XYZ
//    Unknown
//  };
//
//  typedef enum : uint8_t {
//    ODR_80Hz_OSR_16 = 0x00,
//    ODR_40Hz_OSR_32 = 0x08,
//    ODR_20Hz_OSR_64,
//    ODR_10Hz_OSR_128,
//
//    ODR_40Hz_OSR_16,
//    ODR_20Hz_OSR_32,
//    ODR_10Hz_OSR_64,
//    ODR_5Hz_OSR_128,
//
//    ODR_20Hz_OSR_16,
//    ODR_10Hz_OSR_32,
//    ODR_5Hz_OSR_64,
//    ODR_2_5Hz_OSR_128,
//
//    ODR_10Hz_OSR_16,
//    ODR_5Hz_OSR_32,
//    ODR_2_5Hz_OSR_64,
//    ODR_1_25Hz_OSR_128,
//
//    ODR_5Hz_OSR_16,
//    ODR_2_5Hz_OSR_32,
//    ODR_1_25Hz_OSR_64,
//    ODR_0_63Hz_OSR_128,
//
//    ODR_2_5Hz_OSR_16,
//    ODR_1_25Hz_OSR_32,
//    ODR_0_63Hz_OSR_64,
//    ODR_0_31Hz_OSR_128,
//
//    ODR_1_25Hz_OSR_16,
//    ODR_0_63Hz_OSR_32,
//    ODR_0_31Hz_OSR_64,
//    ODR_0_16Hz_OSR_128,
//
//    ODR_0_63Hz_OSR_16,
//    ODR_0_31Hz_OSR_32,
//    ODR_0_16Hz_OSR_64,
//    ODR_0_8Hz_OSR_128,
//  } OutputDataRate_OverSamplingRate;
//
// public:
//  /**
//   * @brief Measured acceleration is returned as 3D vector of @ref Magnetic
//   */
//  using MagneticVector = microhal::Vector<microhal::Magnetic, 3>;
//
//  /**
//   * @brief Constructor of MAG3110 driver
//   *
//   * @param i2c I2C device where MAG3110 is connected
//   * @param address MAG3110 I2C address @ref PossibleI2CAddress
//   */
//  MAG3110(microhal::I2C& i2c, PossibleI2CAddress address)
//      : I2CDevice(i2c, address) {}
//  /**
//   * @brief Function initializes MAG3110 driver.
//   * @details It tries to communicate with chip and get its ID, which is
//   * compared expected value.
//   * @return true - if device is present, it is responding and its ID is
//   proper.
//   * false otherwise.
//   */
//  bool init(void);
//  /**
//   * @brief Set correction offset for magnetic field reading from X ax.
//   *
//   * @param offset
//   * @return true if setting was successful, false otherwise
//   */
//  bool setXCorrection(int16_t offset) {
//    return write(OFF_X, offset) == Error::NoError;
//  }
//  /**
//   * @brief Set correction offset for magnetic field reading from Y ax.
//   *
//   * @param offset
//   * @return true if setting was successful, false otherwise
//   */
//  bool setYCorrection(int16_t offset) {
//    return write(OFF_Y, offset) == Error::NoError;
//  }
//  /**
//   * @brief Set correction offset for magnetic field reading from Z ax.
//   *
//   *
//   * @param offset
//   * @return true if setting was successful, false otherwise
//   */
//  bool setZCorrection(int16_t offset) {
//    return write(OFF_Z, offset) == Error::NoError;
//  }
//  /**
//   * @brief Set correction offset for magnetic field reading all axis.
//   *
//   * @param x X ax offset
//   * @param y Y ax offset
//   * @param z Z ax offset
//   * @return true if setting was successful, false otherwise
//   */
//  bool setCorrection(int16_t x, int16_t y, int16_t z);
//  /**
//   * @brief Get X ax correction offset
//   *
//   * @param offset value of offset will be stored here
//   * @return true if read was successful, false otherwise
//   */
//  std::experimental::optional<int16_t> getXCorrection() {
//    std::experimental::optional<int16_t> out;
//    int16_t tmp;
//    if (read(OFF_X, tmp) == Error::NoError) {
//      out = tmp;
//    }
//    return out;
//  }
//  /**
//   * @brief Get Y ax correction offset
//   *
//   * @param offset value of offset will be stored here
//   * @return true if read was successful, false otherwise
//   */
//  std::experimental::optional<int16_t> getYCorrection() {
//    std::experimental::optional<int16_t> out;
//    int16_t tmp;
//    if (read(OFF_Y, tmp) == Error::NoError) {
//      out = tmp;
//    }
//    return out;
//  }
//  /**
//   * @brief Get Z ax correction offset
//   *
//   * @param offset value of offset will be stored here
//   * @return true if read was successful, false otherwise
//   */
//  std::experimental::optional<int16_t> getZCorrection() {
//    std::experimental::optional<int16_t> out;
//    int16_t tmp;
//    if (read(OFF_Z, tmp) == Error::NoError) {
//      out = tmp;
//    }
//    return out;
//  }
//  /**
//   * @brief Get all axis correction offset at once
//   *
//   * @param x X ax offset
//   * @param y Y ax offset
//   * @param z Z ax offset
//   * @return true if read was successful, false otherwise
//   */
//  bool getCorrection(int16_t* x, int16_t* y, int16_t* z);
//
//  std::experimental::optional<microhal::Temperature> temperature() {
//    std::experimental::optional<microhal::Temperature> temperature;
//    int8_t tmp;
//    if (read(DIE_TEMP, tmp) == Error::NoError) {
//      temperature = microhal::Temperature::fromCelcius(tmp);
//    }
//    return temperature;
//  }
//
//  bool setMode(Mode mode);
//  /**
//   * @return Actual working mode or UNKNOWN_MODE if an error occurred
//   */
//  std::experimental::optional<Mode> getMode() {
//    std::experimental::optional<Mode> mode;
//    uint8_t tmp;
//    if (read(SYSMOD, tmp) == Error::NoError) {
//      mode = static_cast<Mode>(tmp);
//    }
//    return mode;
//  }
//
//  bool setODR_OSR(OutputDataRate_OverSamplingRate odr_osr);
//
//  std::experimental::optional<MagneticVector> getMagnetic();
//
//  void convertToMagnetic(MagneticVector* mag, int16_t x, int16_t y, int16_t z)
//  {
//    float scale = 1.0;
//    (*mag)[0] = microhal::Magnetic(static_cast<float>(x) * scale);
//    (*mag)[1] = microhal::Magnetic(static_cast<float>(y) * scale);
//    (*mag)[2] = microhal::Magnetic(static_cast<float>(z) * scale);
//  }
//
//  std::experimental::optional<int16_t> getX(void) {
//    std::experimental::optional<int16_t> out;
//    int16_t tmp;
//    if (read(OUT_X, tmp) == Error::NoError) {
//      out = tmp;
//    }
//    return out;
//  }
//
//  std::experimental::optional<int16_t> getY(void) {
//    std::experimental::optional<int16_t> out;
//    int16_t tmp;
//    if (read(OUT_Y, tmp) == Error::NoError) {
//      out = tmp;
//    }
//    return out;
//  }
//
//  std::experimental::optional<int16_t> getZ(void) {
//    std::experimental::optional<int16_t> out;
//    int16_t tmp;
//    if (read(OUT_Z, tmp) == Error::NoError) {
//      out = tmp;
//    }
//    return out;
//  }
//};

#endif /* LIS331HH_H_ */
