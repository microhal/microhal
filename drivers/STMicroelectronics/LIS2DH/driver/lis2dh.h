
#ifndef LIS2DH_H_
#define LIS2DH_H_

#include <cstdint>
#include <experimental/optional>

#include "I2CDevice/I2CDevice.h"
#include "math/vector.h"
#include "microhal.h"
#include "units/acceleration.h"
#include "units/temperature.h"

class LIS2DH : protected microhal::I2CDevice {
  using Endianness = microhal::Endianness;
  using Access = microhal::Access;
  using Error = microhal::I2C::Error;

  // IF subaddress 7bit is set than multiple register read is allowed
  static constexpr uint8_t SUB7 = (1U << 7);

  // clang-format off
    static constexpr auto STAUS_REG_AUX = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x07>{});
	static constexpr auto OUT_TEMP_L = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x0C>{});
	static constexpr auto OUT_TEMP_H = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x0D>{});
	static constexpr auto OUT_TEMP = microhal::makeRegister<uint16_t, Access::ReadOnly,Endianness::Little>(microhal::Address<uint8_t, 0x0C>{});
	static constexpr auto INT_COUNTER_REG = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x0E>{});
	static constexpr auto WHO_AM_I = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x0F>{});
	static constexpr auto TEMP_CFG_REG = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x1F>{});
	static constexpr auto CTRL_REG1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x20>{});
	static constexpr auto CTRL_REG2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x21>{});
	static constexpr auto CTRL_REG3 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x22>{});
	static constexpr auto CTRL_REG4 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x23>{});
	static constexpr auto CTRL_REG5 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x24>{});
	static constexpr auto CTRL_REG6 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x25>{});
	static constexpr auto REFERENCE = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x26>{});
	static constexpr auto STATUS_REG2 = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x27>{});
	static constexpr auto OUT_X = microhal::makeRegister<int16_t, Access::ReadOnly,Endianness::Little>(microhal::Address<uint8_t, 0x28|SUB7>{});
	static constexpr auto OUT_Y = microhal::makeRegister<int16_t, Access::ReadOnly,Endianness::Little>(microhal::Address<uint8_t, 0x2a|SUB7>{});
	static constexpr auto OUT_Z = microhal::makeRegister<int16_t, Access::ReadOnly,Endianness::Little>(microhal::Address<uint8_t, 0x2c|SUB7>{});
	static constexpr auto FIFO_CTRL_REG = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x2e>{});
	static constexpr auto FIFO_SRC_REG = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x2f>{});
	static constexpr auto INT1_CFG = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x30>{});
	static constexpr auto INT1_SOURCE = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x31>{});
	static constexpr auto INT1_THS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x32>{});
	static constexpr auto INT1_DURATION = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x33>{});
	static constexpr auto INT2_CFG = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x34>{});
	static constexpr auto INT2_SOURCE = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x35>{});
	static constexpr auto INT2_THS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x36>{});
	static constexpr auto INT2_DURATION = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x37>{});
	static constexpr auto CLICK_CFG = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x38>{});
	static constexpr auto CLICK_SRC = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x39>{});
	static constexpr auto CLICK_THS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x3a>{});
	static constexpr auto TIME_LIMIT = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x3b>{});
	static constexpr auto TIME_LATENCY = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x3c>{});
	static constexpr auto TIME_WINDOW = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x3d>{});
	static constexpr auto Act_THS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x3e>{});
	static constexpr auto Act_DUR = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x3f>{});
  // clang-format on
  enum class STAUS_REG_AUX_BITS : uint8_t {
    TOR = (1U << 7),  // Temperature data overrun
    TDA = (1U << 3)   // Temperature new data available
  };
  static constexpr uint8_t WHO_AM_I_BITS = 0b00110011;
  enum class TEMP_CFG_REG_BITS : uint8_t {
    TEMP_EN0 = 1U << 6,
    TEMP_EN1 = 1U << 7
  };
  enum class CTRL_REG1_BITS : uint8_t {

    Xen = 1,
    Yen = 1U << 1,
    Zen = 1U << 2,
    LPen = 1U << 3,
    ODR0 = 1U << 4,
    ODR1 = 1U << 5,
    ODR2 = 1U << 6,
    ODR3 = 1U << 7
  };
  enum class Mode {
    normal = 0,
    lowPower = 1,
    highResolution = 2,
    notAllowed = 23
  };

  enum class CTRL_REG2_BITS : uint8_t {
    HPIS1 = 1,        // AOI high pass filter int 1
    HPIS2 = 1U << 1,  // int2
    HPCLICK = 1U << 2,
    FDS = 1U << 3,
    HPCF1 = 1U << 4,
    HPCF2 = 1U << 5,
    HPM0 = 1U << 6,
    HPM1 = 1U << 7
  };

  enum class HighPassFilterMode {
    NormalResetReading = 0,
    ReferenceSignal,
    Normal,
    Autoreset
  };

  enum class CTRL_REG3_BITS : uint8_t {
    I1_OVERRUN = 1U << 1,
    I1_WTM = 1U << 2,
    I1_DRDY2 = 1U << 3,
    I1_DRDY1 = 1U << 4,
    I1_AOI2 = 1U << 5,
    I1_AOI1 = 1U << 6,
    I1_CLICK = 1U << 7
  };

  enum class CTRL_REG4_BITS : uint8_t {
    SIM = 1,
    ST0 = 1U << 1,
    ST1 = 1U << 2,
    HR = 1U << 3,
    FS0 = 1U << 4,
    FS1 = 1U << 5,
    BLE = 1U << 6,
    BDU = 1U << 7
  };

 public:
  using AccelerationVector = microhal::Vector<microhal::Acceleration, 3>;

  enum PossibleI2CAddress : microhal::I2C::DeviceAddress {
    I2C_ADDRESS_0 = 0b0011000 << 1,  /// SDO/SA0 pin is '0'
    I2C_ADDRESS_1 = 0b0011001 << 1,  /// SDO/SA0 pin is '1'
  };

  enum class DataRate : uint8_t {
    PowerDown = 0,
    Mode1Hz = 1UL << 4,
    Mode10Hz,
    Mode25Hz,
    Mode50Hz,
    Mode100Hz,
    Mode200Hz,
    Mode400Hz,
    LowPower1_62kHz,
    HRNormal1_344khzLowpower5_376kHz
  };

  LIS2DH(microhal::I2C &i2c, PossibleI2CAddress address)
      : I2CDevice(i2c, address) {
    scale = Scale::Scale2G;
  }
  enum Range { range2g = 0, range4g = 1, range8g = 2, range16g = 3 };

  bool init() {
    // Boot procedure is complete about 5 milliseconds just after powered up the
    // device.
    uint8_t temp;
    if (read(WHO_AM_I, temp) == Error::NoError) {
      if (temp == WHO_AM_I_BITS) {
        return true;
      }

      // here all register in acc must be reseted to default values
    }
    return false;
  }
  // ContinuousUpdate must be enabled also
  void temperatureSensorEnable() {
    // TEMP_EN bit of the TEMP_CFG_REG register to 1.
    // To retrieve the temperature sensor data BDU bit on CTRL_REG4 (23h) must
    // be set to ‘1’.
    // Both OUT_TEMP_H and OUT_TEMP_L registers must be read.
  }
  void temperatureSensorDisable() {}
  std::experimental::optional<microhal::Temperature> getTemperatue() {
    std::experimental::optional<microhal::Temperature> temperature;
    uint16_t temp;

    //    if (read(OUT_TEMP | SUB7, temp) == Error::NoError) {
    //    }
    return temperature;
  }
  bool isTemperatureAvailable() {
    uint8_t temp = 0;
    if (read(STAUS_REG_AUX, temp) == Error::NoError) {
      if (temp & static_cast<uint8_t>(STAUS_REG_AUX_BITS::TDA)) {
        return true;
      }
    }
    return false;
  }
  bool isTemperatureOverrun() {
    uint8_t temp = 0;
    if (read(STAUS_REG_AUX, temp) == Error::NoError) {
      if (temp & static_cast<uint8_t>(STAUS_REG_AUX_BITS::TOR)) {
        return true;
      }
    }
    return false;
  }
  void setMeasurementRange() {}
  void enableOrientationReocgnition() {}
  void setSleepThreshold() {}
  bool isDataReady() { return false; }
  void enableFreeFallInterrupt() {}
  void enableWakeUpInterrupt() {}
  // Stream
  // The FIFO continues filling
  // until it’s full (32 slots of data for X, Y and Z). When full, the FIFO
  // discards the older data as
  // the new arrive.
  enum class FifoMode { fifo, stream, trigger, bypass, streamToFifo };
  void configureFifomode() {
    // FIFO_MODE bits into the FIFO_CTRL_REG (2E).
  }
  void enableFifoInterrupt() {
    //		/FIFO_empty or FIFO_Full events can be enabled to generate
    // dedicated
    //	interrupts on INT1/2 pin (configuration through FIFO_CFG_REG).
  }

  void watermarkInterruptEnable() {
    //(FIFO_WTMK_EN bit into FIFO_CTRL_REG (2E) in order to be
    // raised when the FIFO is filled to the level specified into the
    // FIFO_WTMK_LEVEL bits of
    // FIFO_CTRL_REG (2E). The FIFO continues filling until it is full (32 slots
    // of data for X, Y and
    // Z). When full, the FIFO stops collecting data from the input channels.
  }
  std::experimental::optional<AccelerationVector> getLastData() {
    std::array<int16_t, 3> data;
    std::experimental::optional<AccelerationVector> vector;
    if (readRegisters(data, OUT_X, OUT_Y, OUT_Z) == Error::NoError) {
      int16_t x = data[0];
      int16_t y = data[1];
      int16_t z = data[2];

      vector->operator[](0) = microhal::Acceleration(static_cast<float>(x));
      vector->operator[](1) = microhal::Acceleration(static_cast<float>(y));
      vector->operator[](2) = microhal::Acceleration(static_cast<float>(z));
    }
    return vector;
  }
  bool getLastData(AccelerationVector &vector) {
    std::array<int16_t, 3> data;
    if (readRegisters(data, OUT_X, OUT_Y, OUT_Z) != Error::NoError) {
      return false;
    }
    int16_t x = data[0];
    int16_t y = data[1];
    int16_t z = data[2];

    vector[0] = microhal::Acceleration(convertToAcceleration(x));
    vector[1] = microhal::Acceleration(convertToAcceleration(y));
    vector[2] = microhal::Acceleration(convertToAcceleration(z));
    return true;
  }
  bool getLastData(int16_t &x, int16_t &y, int16_t &z) {
    std::array<int16_t, 3> data;
    if (readRegisters(data, OUT_X, OUT_Y, OUT_Z) != Error::NoError) {
      return false;
    }
    x = data[0];
    y = data[1];
    z = data[2];
    return true;
  }
  bool enableAxis(bool x, bool y, bool z) {
    uint8_t temp = 0;
    if (true == x) {
      temp |= static_cast<uint8_t>(CTRL_REG1_BITS::Xen);
    }
    if (true == y) {
      temp |= static_cast<uint8_t>(CTRL_REG1_BITS::Yen);
    }
    if (true == z) {
      temp |= static_cast<uint8_t>(CTRL_REG1_BITS::Zen);
    }
    if (bitsSet(CTRL_REG1, temp) == Error::NoError) {
      return true;
    }
    return false;
  }
  void enableContinuousUpdate() {}
  void getFullFifo() {
    //	The reading address is automatically updated by the device and it rolls
    // back to 0x28 when
    //	register 0x2D is reached. In order to read all FIFO levels in a multiple
    // byte reading,192 bytes
    //(6 output registers by 32 levels) have to be read.
  }
  void getHalfFifo() {}
  bool getLastRawX(int16_t &x) {
    if (read(OUT_X, x) == Error::NoError) return true;
    return false;
  }

  bool getLastRawY(int16_t &y) {
    if (read(OUT_Y, y) == Error::NoError) return true;
    return false;
  }
  bool getLastRawZ(int16_t &z) {
    if (read(OUT_Z, z) == Error::NoError) return true;
    return false;
  }
  void highSpeedEndian(Endianness endian) {
    // check if in high resolution mode
  }
  enum class SelfTestMode { normal, test0, test1 };
  void enableSelfTest(SelfTestMode mode) { uint8_t temp = 0; }
  bool setDataRate(DataRate rate) {
    uint8_t temp = static_cast<uint8_t>(rate);
    if (bitsSet(CTRL_REG1, temp) == Error::NoError) {
      return true;
    }
  }
  void configureHighPassFilter() {
    // cut off selection
    // mode
    // interrupt enable ...
  }
  void enableClickinterrupt() {}

  enum Scale : uint8_t { Scale2G = 0, Scale4G = 1UL << 4, Scale8G, Scale16G };

 private:
  Scale scale;

 public:
  bool setScale(Scale scale) {
    uint8_t temp = static_cast<uint8_t>(scale);
    this->scale = scale;
    if (bitsSet(CTRL_REG4, temp) == Error::NoError) {
      return true;
    }
  }
  microhal::Acceleration convertToAcceleration(int16_t value) {
    float range = 0;
    switch (scale) {
      case Scale2G:
        range = 2;
        break;
      case Scale4G:
        range = 4;
        break;
      case Scale8G:
        range = 8;
        break;
      case Scale16G:
        range = 16;
        break;
    }
    float temp =
        range * static_cast<float>(value) / static_cast<float>(1 << 15);
    return microhal::Acceleration(temp);
  }
};

#endif /* LIS2DH_H_ */
