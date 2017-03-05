
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
	static constexpr auto OUT_TEMP = microhal::makeRegister<uint16_t, Access::ReadOnly,Endianness::Little>(microhal::Address<uint8_t, 0x0C|SUB7>{});
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

  enum class CTRL_REG5_BITS : uint8_t {

    D4DINT2 = 0,
    LIR_INT2 = 1U << 1,
    D4D_INT1 = 1U << 2,
    LIR_INT1 = 1U << 3,
    FIFO_EN = 1U << 6,
    BOOT = 1U << 7
  };

  enum class CTRL_REG6_BITS : uint8_t {
    H_LACTIVE = 1UL << 1,
    P2_ACT = 1UL << 3,
    BOOT_I2 = 1UL << 4,
    I2_INT2 = 1UL << 5,
    I2_INT1 = 1UL << 6,
    I2_CLICKen = 1UL << 7
  };

  enum class INT1_CFG_BITS : uint8_t {
    XLIE_XDOWNE = 1,
    XHIE_XUPE = 1UL << 1,
    YLIE_YDOWNE = 1UL << 2,
    YHIE_YUPE = 1UL << 3,
    ZLIE_ZDOWNE = 1UL << 4,
    ZHIE_ZUPE = 1UL << 5,
    _6D = 1UL << 6,
    AIO = 1UL << 7
  };

  enum class CLICK_CFG_BITS : uint8_t {
    XS = 1,
    XD = 1UL << 1,
    YS = 1UL << 2,
    YD = 1UL << 3,
    ZS = 1UL << 4,
    ZD = 1UL << 5
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
    Mode10Hz = 2UL << 4,
    Mode25Hz = 3UL << 4,
    Mode50Hz = 4UL << 4,
    Mode100Hz = 5UL << 4,
    Mode200Hz = 6UL << 4,
    Mode400Hz = 7UL << 4,
    LowPower1_62kHz = 8UL << 4,
    HRNormal1_344khzLowpower5_376kHz = 9UL << 4
  };

  LIS2DH(microhal::I2C &i2c, PossibleI2CAddress address)
      : I2CDevice(i2c, address) {
    scale = Scale::Scale2G;
  }
  enum Range { range2g = 0, range4g = 1, range8g = 2, range16g = 3 };

  bool resetRegisters() {
    Error status = Error::NoError;
    // here all register in acc must be reseted to default values

    // manual clearing
    status = write(TEMP_CFG_REG, 0);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CTRL_REG1, 0x07);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CTRL_REG2, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CTRL_REG3, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CTRL_REG4, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CTRL_REG5, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CTRL_REG6, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(FIFO_CTRL_REG, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(INT1_CFG, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(INT1_THS, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(INT1_DURATION, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(INT2_CFG, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(INT2_THS, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(INT2_DURATION, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CLICK_CFG, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(CLICK_THS, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(TIME_LIMIT, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(TIME_LATENCY, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    status = write(TIME_WINDOW, 0x00);
    if (status != Error::NoError) {
      return false;
    }
    // at this moment i dont know how use it
    //    uint8_t temp = static_cast<uint8_t>(CTRL_REG5_BITS::BOOT);
    //    if (write(CTRL_REG5, temp) == Error::NoError) {
    //      return true;
    //    }

    return false;
  }
  bool init() {
    // Boot procedure is complete about 5 milliseconds just after powered up the
    // device.
    uint8_t temp;
    if (read(WHO_AM_I, temp) == Error::NoError) {
      if (temp != WHO_AM_I_BITS) {
        return false;
      }
    }
    return true;
  }
  // ContinuousUpdate must be enabled also
  bool temperatureSensorEnable() {
    // TEMP_EN bit of the TEMP_CFG_REG register to 1.
    // To retrieve the temperature sensor data BDU bit on CTRL_REG4 (23h) must
    // be set to ‘1’.
    // Both OUT_TEMP_H and OUT_TEMP_L registers must be read.
    uint8_t temp = static_cast<uint8_t>(TEMP_CFG_REG_BITS::TEMP_EN0) |
                   static_cast<uint8_t>(TEMP_CFG_REG_BITS::TEMP_EN1);
    if (bitsSet(TEMP_CFG_REG, temp) == Error::NoError) {
      return true;
    }
    return false;
  }
  bool temperatureSensorDisable() {
    if (bitsSet(TEMP_CFG_REG, 0) == Error::NoError) {
      return true;
    }
    return false;
  }
  std::experimental::optional<microhal::Temperature> getTemperatue() {
    std::experimental::optional<microhal::Temperature> temperature;

    uint8_t temp1, temp2;
    read(OUT_TEMP_L, temp1);
    read(OUT_TEMP_H, temp2);
    uint16_t temp = 0;
    int8_t *temp8 = reinterpret_cast<int8_t *>(&temp);
    if (read(OUT_TEMP, temp) == Error::NoError) {
      //      temperature = microhal::Temperature::fromCelcius(temp8[1]);
    }
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
  // CTRL_REG3 I1-CLICK
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
  bool ContinuousUpdate(bool enable) {
    uint8_t temp = static_cast<uint8_t>(CTRL_REG4_BITS::BDU);
    if (true == enable) {
      if (bitsClear(CTRL_REG4, temp) == Error::NoError) {
        return true;
      }
    } else {
      if (bitsSet(CTRL_REG4, temp) == Error::NoError) {
        return true;
      }
    }
    return false;
  }
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

  enum Scale : uint8_t {
    Scale2G = 0,
    Scale4G = 1UL << 4,
    Scale8G = 2UL << 4,
    Scale16G = 3UL << 4
  };

 private:
  Scale scale;

 public:
  bool setScale(Scale scale) {
    uint8_t temp = static_cast<uint8_t>(scale);
    this->scale = scale;
    if (write(CTRL_REG4, temp) == Error::NoError) {
      return true;
    }
    return false;
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

  //  enum class interruptSource : uint8_t {
  //    XL = 1,
  //    XH = 1UL << 1,
  //    YL = 1UL << 2,
  //    YH = 1UL << 3,
  //    ZL = 1UL << 4,
  //    ZH = 1UL << 5,
  //    IA = 1UL << 6
  //  };
  struct interruptSources {
    uint8_t XL : 1;
    uint8_t XH : 1;
    uint8_t YL : 1;
    uint8_t YH : 1;
    uint8_t ZL : 1;
    uint8_t ZH : 1;
    uint8_t IA : 1;
  };

  union interruptSource {
    uint8_t value;
    interruptSources sources;
  };

  bool checkInterrupt1Source(interruptSource &source) {
    uint8_t temp;
    if (read(INT1_SOURCE, temp) != Error::NoError) return false;
    source.value = temp;
    return true;
  }
  bool checkInterrupt2Source(uint8_t &source) {
    if (read(INT1_SOURCE, source) == Error::NoError) return true;
    return false;
  }

  enum class INT1Source : uint8_t {
    ORCombination = 0,
    Direction6D,
    AndCombination,
    Postiion6D,
    ZHighEvent,
    ZHighDirection,
    ZLowEvent,
    ZLowDirection,
    YHighEvent,
    YHighDirection,
    YLowEvent,
    YLowDirection,
    XHighEvent,
    XHighDirection,
    XLowEvent,
    XLowDirection

  };

  bool configureInterrupt1source(INT1Source cfg) {
    uint8_t clear = 0;
    uint8_t set = 0;

    switch (cfg) {
      case INT1Source::AndCombination:
        set = static_cast<uint8_t>(INT1_CFG_BITS::AIO);
        clear = static_cast<uint8_t>(INT1_CFG_BITS::_6D);
        break;

      case INT1Source::ZLowDirection:
      case INT1Source::ZLowEvent:
        set |= static_cast<uint8_t>(INT1_CFG_BITS::ZLIE_ZDOWNE);
        break;
      case INT1Source::YLowDirection:
      case INT1Source::YLowEvent:
        set |= static_cast<uint8_t>(INT1_CFG_BITS::YLIE_YDOWNE);
        break;

      case INT1Source::XLowEvent:
      case INT1Source::XLowDirection:
        set |= static_cast<uint8_t>(INT1_CFG_BITS::XLIE_XDOWNE);
        break;
      case INT1Source::ORCombination:
      default:  // OrCombination
        clear = INT1_CFG_BITS::AIO | INT1_CFG_BITS::_6D;
    }
    if (clear) {
      if (bitsClear(INT1_CFG, clear) != Error::NoError) {
        return false;
      }
    }
    if (set) {
      if (bitsSet(INT1_CFG, set) != Error::NoError) {
        return false;
      }
    }
    return true;
  }
  friend uint8_t operator|(LIS2DH::INT1_CFG_BITS a, LIS2DH::INT1_CFG_BITS b) {
    return static_cast<uint8_t>(a) | static_cast<uint8_t>(b);
  }

  enum class Interrupt1Sources {
    OVERRUN = 1UL << 1,
    WTM = 1UL << 2,
    DRDY2 = 1UL << 3,
    DRDY1 = 1UL << 4,
    AOI2 = 1UL << 5,
    AOI1 = 1UL << 6,
    Click = 1UL << 7

  };
  bool interrupt1EnableSource(Interrupt1Sources source) {
    uint8_t temp = static_cast<uint8_t>(source);

    if (bitsSet(CTRL_REG3, temp) == Error::NoError) {
      return true;
    }
  }
  bool interrupt1DisableSource(Interrupt1Sources source) {
    uint8_t temp = static_cast<uint8_t>(source);

    if (bitsSet(CTRL_REG3, temp) == Error::NoError) {
      return true;
    }
  }
  bool interrupt1DisableAll() {
    if (write(CTRL_REG3, 0) == Error::NoError) {
      return true;
    }
  }

  struct clickinterruptSources {
    uint8_t X : 1;     // X click-click detection
    uint8_t Y : 1;     // Y click-click detection
    uint8_t Z : 1;     // Z click-click detection
    uint8_t Sign : 1;  // Click-click-sign 0-positive,1-negative
    uint8_t Stap : 1;
    uint8_t DClick : 1;
    uint8_t IA : 1;  // one or more interrupts has been generated
  };

  union clickinterruptSource {
    uint8_t value;
    clickinterruptSources sources;
  };
  bool checkClickSource(clickinterruptSource &source) {
    uint8_t temp;
    if (read(CLICK_SRC, temp) != Error::NoError) return false;
    source.value = temp;
    return true;
  }
  // threshold - 1 LSB = full scale/128.
  // timelimit - 1 LSB = 1/ODR.
  // interrupt is kept high until click source is read
  bool setClick(uint8_t threshold, uint8_t timeLimit, uint8_t timeLatency,
                uint8_t timeWindow, bool xAxis, bool yAxis, bool zAxis,
                bool singleClick, bool doubleClick) {
    uint8_t temp = 0;

    // registers has only 7 bits
    if (threshold > 126) return false;
    if (timeLimit > 126) return false;

    if (write(CLICK_THS, threshold) != Error::NoError) {
      return false;
    }

    if (write(TIME_LIMIT, timeLimit) != Error::NoError) {
      return false;
    }

    if (write(TIME_LATENCY, timeLatency) != Error::NoError) {
      return false;
    }

    if (write(TIME_WINDOW, timeWindow) != Error::NoError) {
      return false;
    }

    if (true == singleClick) {
      if (true == xAxis) {
        temp = static_cast<uint8_t>(CLICK_CFG_BITS::XS);
      }
      if (true == yAxis) {
        temp |= static_cast<uint8_t>(CLICK_CFG_BITS::YS);
      }
      if (true == zAxis) {
        temp |= static_cast<uint8_t>(CLICK_CFG_BITS::ZS);
      }
    }

    if (true == doubleClick) {
      if (true == xAxis) {
        temp |= static_cast<uint8_t>(CLICK_CFG_BITS::XD);
      }
      if (true == yAxis) {
        temp |= static_cast<uint8_t>(CLICK_CFG_BITS::YD);
      }
      if (true == zAxis) {
        temp |= static_cast<uint8_t>(CLICK_CFG_BITS::ZD);
      }
    }

    if (bitsSet(CLICK_CFG, temp) != Error::NoError) {
      return false;
    }
    return true;
  }
  enum class interruptLevel { LOW = 0, HIGH };
  bool interruptSetLevel(interruptLevel level) {
    if (level == interruptLevel::LOW) {
      if (bitsSet(CTRL_REG6, static_cast<uint8_t>(CTRL_REG6_BITS::H_LACTIVE)) !=
          Error::NoError) {
        return false;
      }
    } else {
      if (bitsClear(CTRL_REG6,
                    static_cast<uint8_t>(CTRL_REG6_BITS::H_LACTIVE)) !=
          Error::NoError) {
        return false;
      }
    }
    return true;
  }
  bool interrupt1LatchRequest(bool enable) {
    if (enable) {
      if (bitsSet(CTRL_REG5, static_cast<uint8_t>(CTRL_REG5_BITS::LIR_INT1)) !=
          Error::NoError) {
        return false;
      }
    } else {
      if (bitsClear(CTRL_REG5,
                    static_cast<uint8_t>(CTRL_REG5_BITS::LIR_INT1)) !=
          Error::NoError) {
        return false;
      }
    }
    return true;
  }
  bool configureFreefall(uint8_t threshold, uint8_t eventDuration) {
    if (write(INT1_THS, threshold) != Error::NoError) {
      return false;
    }

    if (write(INT1_DURATION, eventDuration) != Error::NoError) {
      return false;
    }
    return true;
  }

  bool disableHighPassFilters() {
    if (write(CTRL_REG2, 0) != Error::NoError) {
      return false;
    }
    return true;
  }

  // used to debug
  void test() {
    uint8_t temp;
    read(CTRL_REG1, temp);
  }
};

#endif /* LIS2DH_H_ */
