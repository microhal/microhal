/* =========================================================================================== */ /**
 @file        ds2782.h
 @authors     Michal Karczewski
 @version     v 0.1
 @package
 @brief       DS2782 driver
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details     Implementation of the DS2782

 */ /* ============================================================================================ */

#ifndef DS2786_H_
#define DS2786_H_

/******************************************************************************
 * INCLUDES
 */
#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"

/** @addtogroup Devices
 *  @{
 *  @class DS2782
 *  @}
 *
 * @brief DS2782 stand-alone ocv-based fuel gauge 2-wire interface
 */
class DS2782 : protected microhal::I2CDevice {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;
    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    // enum Registers : uint8_t {
    static constexpr auto STATUS = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x01>{});
    static constexpr auto RAAC_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x02>{});
    //		static constexpr auto RAAC_LSB
    static constexpr auto RSAC_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x04>{});
    //	static constexpr auto RSAC_LSB
    static constexpr auto RARC = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x05>{});
    static constexpr auto RSRC = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x06>{});
    static constexpr auto IAVG_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x07>{});
    // static constexpr auto IAVG_LSB
    static constexpr auto TEMP_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x09>{});
    //		static constexpr auto TEMP_LSB
    static constexpr auto VOLT_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x0B>{});
    //	static constexpr auto VOLT_LSB
    static constexpr auto CURRENT_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x0D>{});
    //			static constexpr auto CURRENT_LSB
    static constexpr auto ACR_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x0F>{});
    ///		static constexpr auto ACR_LSB
    static constexpr auto ACRL_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x11>{});
    //			static constexpr auto ACRL_LSB
    static constexpr auto AS = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x12>{});
    static constexpr auto SFR = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x13>{});
    static constexpr auto FULL_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x14>{});
    //		static constexpr auto FULL_LSB
    static constexpr auto AE_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x16>{});
    //			static constexpr auto AE_LSB
    static constexpr auto SE_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x18>{});
    //			static constexpr auto SE_LSB
    static constexpr auto EEPROM_REGISTER = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1F>{});
    static constexpr auto USER_EEPROM_0 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_3 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_4 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_5 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_6 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_7 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_8 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_9 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_10 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_11 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_12 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_13 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_14 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto USER_EEPROM_15 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_0 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_3 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_4 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_5 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_6 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_7 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_8 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_9 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_10 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_11 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_12 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_13 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_14 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto ADD_USER_EEPROM_15 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_CONTROL = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x60>{});
    static constexpr auto EEPROM_AB = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_AC_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x00>{});
    //		static constexpr auto EEPROM_AC_LSB
    static constexpr auto EEPROM_VCHG = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_IMIN = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_VAE = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_IAE = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_ACTIVE_EMPTY = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_RSNSP = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_FULL_40_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x00>{});
    //	static constexpr auto    EEPROM_FULL_40_LSB
    static constexpr auto EEPROM_FULL_3040 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_FULL_2030 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_FULL_1020 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_FULL_0010 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_AE_3040 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_AE_2030 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_AE_1020 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_AE_0010 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_SE_3040 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_SE_2030 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_SE_1020 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_SE_0010 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_RSGAIN_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x00>{});
    //	static constexpr auto EEPROM_RSGAIN_LSB
    static constexpr auto EEPROM_RSTC = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto EEPROM_FRSGAIN_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x00>{});
    //		static constexpr auto EEPROM_FRSGAIN_LSB
    static constexpr auto EEPROM_2WIRE_ADDRESS = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x7E>{});
    static constexpr auto UNIQUE_ID_0 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0xF0>{});
    static constexpr auto UNIQUE_ID_1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto UNIQUE_ID_2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto UNIQUE_ID_3 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto UNIQUE_ID_4 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto UNIQUE_ID_5 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto UNIQUE_ID_6 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto UNIQUE_ID_7 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto FUNCTION_COMMAND_REGISTER = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0xFE>{});
    //};

    enum StatusBits : uint8_t { CHGTF = 0x80, AEF = 0x40, SEF = 0x20, LEARNF = 0x10, UVF = 0x04, PORF = 0x02 };

    enum ControlBits : uint8_t {
        UVEN = 0x40,
        PMOD = 0x20,
    };

    enum SpecialFeatureRegister : uint8_t { SAWE = 0x02, PIOSC = 0x01 };

    enum EEPROMRegister : uint8_t { EEC = 0x40, LOCK = 0x20, BL1 = 0x02, BL0 = 0x01 };

    enum FunctionCommand : uint8_t {
        COPY_DATA_0 = 0x42,
        COPY_DATA_1 = 0x44,
        RECALL_DATA_0 = 0xB2,
        RECALL_DATA_1 = 0xB4,
        LOCK_0 = 0x63,
        LOCK_1 = 0x66
    };

 public:
    typedef enum : uint8_t { DS2782_ADDR = 0x68 } PossibleI2CAddress;

    constexpr DS2782(microhal::I2C& i2c, PossibleI2CAddress address) : I2CDevice(i2c, address) {}

    /**
     *@brief Read eeprom control register.
     *
     *@param [OUT] data - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readEEPROMControl(uint8_t* data) {
        const bool status = readRegister(EEPROM_CONTROL, *data) == Error::None;
        return status;
    }

    /**
     *@brief Check under voltage SLEEP enable bit on eeprom control register.
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool checkUnderVoltageSleep() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_CONTROL, data);
        status = data & UVEN;
        return status;
    }

    /**
     *@brief clear under voltage SLEEP enable bit on eeprom control register.
     *
     *@retval   true if clearing bit in register is successful
     *@retval   false if an error occurred
     */

    bool clearUnderVoltageSleep() { return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN)) == Error::None; }

    /**
     *@brief Set  under voltage SLEEP enable bit on eeprom control bit on config register.
     *
     *@retval   true if seting bit in register is successful
     *@retval   false if an error occurred
     */

    bool enableUnderVoltageSleep() { return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN)) == Error::None; }

    /**
     *@brief Check power mode enable bit on eeprom control register.
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool checkPowerMode() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_CONTROL, data);
        status = data & PMOD;
        return status;
    }

    /**
     *@brief clear power mode enable bit on eeprom control register.
     *
     *@retval   true if clearing bit in register is successful
     *@retval   false if an error occurred
     */

    bool clearPowerMode() { return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD)) == Error::None; }

    /**
     *@brief Set power mode enable bit on eeprom control bit on config register.
     *
     *@retval   true if seting bit in register is successful
     *@retval   false if an error occurred
     */

    bool enablePowerMode() { return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD)) == Error::None; }

    /**
     *@brief Read voltage registers.
     *
     *@param [OUT] voltageValue - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readVoltage(uint16_t* voltageValue) {
        const bool status = readRegister(VOLT_MSB, *voltageValue) == Error::None;
        return status;
    }

    /**
     *@brief Read temperature registers.
     *
     *@param [OUT] temperatureValue - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readTemperature(uint16_t* temperatureValue) {
        const bool status = readRegister(TEMP_MSB, *temperatureValue) == Error::None;
        return status;
    }

    /**
     *@brief Read current registers.
     *
     *@param [OUT] currentValue - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readCurrent(uint16_t* currentValue) {
        const bool status = readRegister(CURRENT_MSB, *currentValue) == Error::None;
        return status;
    }

    /**
     *@brief Read average current registers.
     *
     *@param [OUT] averageColtageValue - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readAverageCurrent(uint16_t* averageColtageValue) {
        const bool status = readRegister(IAVG_MSB, *averageColtageValue) == Error::None;
        return status;
    }

    /**
     *@brief Read current offset correction registers.
     *
     *@param [OUT] currentOffsetCorrection - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readCurrentOffsetCorrection(uint16_t* currentOffsetCorrection) {
        const bool status = readRegister(ACR_MSB, *currentOffsetCorrection) == Error::None;
        return status;
    }

    /**
     *@brief Read current measurement calibration register.
     *
     *@param [OUT] currentCalibrationRegister - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readCurrentCalibrationCalibrationRegister(uint16_t* currentCalibrationCalibrationRegister) {
        const bool status = readRegister(EEPROM_RSGAIN_MSB, *currentCalibrationCalibrationRegister) == Error::None;
        return status;
    }

    /**
     *@brief Write current measurement calibration register.
     *
     *@param [IN] data - variable to register (array of two 8 bit values)
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeCurrentCalibrationCalibrationRegister(uint16_t data) {
        const bool status = writeRegister(EEPROM_RSGAIN_MSB, data) == Error::None;
        return status;
    }

    /**
     *@brief Read current measurement calibration factory value.
     *
     *@param [OUT] currentCalibrationRegisterFactoryValue - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readCurrentOffsetCorrectionFactoryValue(uint16_t* currentCalibrationRegisterFactoryValue) {
        const bool status = readRegister(ACR_MSB, *currentCalibrationRegisterFactoryValue) == Error::None;
        return status;
    }

    /**
     *@brief Write sense resistor temperature compensation.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeSenseResistorTemperatureCompensation(uint8_t data) {
        const bool status = writeRegister(EEPROM_RSTC, data) == Error::None;
        return status;
    }

    /**
     *@brief Read sense resistor temperature compensation.
     *
     *@param [OUT] senseResistorTemperatureCompensation - variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readSenseResistorTemperatureCompensation(uint8_t* senseResistorTemperatureCompensation) {
        const bool status = readRegister(EEPROM_RSTC, *senseResistorTemperatureCompensation) == Error::None;
        return status;
    }

    /**
     *@brief Write current accumulation register.
     *
     *@param [IN] data - variable to register (array of two 8 bit values)
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeCurrentAccumulation(uint16_t* data) {
        const bool status = writeRegister(ACR_MSB, *data) == Error::None;
        return status;
    }

    /**
     *@brief Read current accumulation register.
     *
     *@param [OUT] senseResistorTemperatureCompensation - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readCurrentAccumulation(uint16_t* currentAcculumation) {
        const bool status = readRegister(ACR_MSB, *currentAcculumation) == Error::None;
        return status;
    }

    /**
     *@brief Read current accumulation fractional results register.
     *
     *@param [OUT] currentAcculumationFractionalResults - array of two values. Variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readCurrentAccumulationFractionalResults(uint16_t* currentAcculumationFractionalResults) {
        const bool status = readRegister(ACRL_MSB, *currentAcculumationFractionalResults) == Error::None;
        return status;
    }

    /**
     *@brief Read accumulation bias register.
     *
     *@param [OUT] accumulationBias - variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readAccumulationBiasRegister(uint8_t* accumulationBias) {
        const bool status = readRegister(EEPROM_AB, *accumulationBias) == Error::None;
        return status;
    }

    /**
     *@brief Read accumulation bias register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeAccumulationBiasRegister(uint8_t data) {
        const bool status = writeRegister(EEPROM_AB, data) == Error::None;
        return status;
    }

    /**
     *@brief Read sense resistor prime register.
     *
     *@param [OUT] senseResistorPrime - variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readSenseResistorPrimeRegister(uint8_t* senseResistorPrime) {
        const bool status = readRegister(EEPROM_RSNSP, *senseResistorPrime) == Error::None;
        return status;
    }

    /**
     *@brief Write sense resistor prime register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeSenseResistorPrimeRegister(uint8_t data) {
        const bool status = writeRegister(EEPROM_RSNSP, data) == Error::None;
        return status;
    }

    /**
     *@brief Read charge voltage register.
     *
     *@param [OUT] chargeVoltage - variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readChargeVoltageRegister(uint8_t* chargeVoltage) {
        const bool status = readRegister(EEPROM_VCHG, *chargeVoltage) == Error::None;
        return status;
    }

    /**
     *@brief Write charge voltage register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeChargeVoltageRegister(uint8_t data) {
        const bool status = writeRegister(EEPROM_VCHG, data) == Error::None;
        return status;
    }

    /**
     *@brief Read minimum charge current register.
     *
     *@param [OUT] minimumChargeCurrent - variable for registers
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readMinimumChargeCurrentRegister(uint8_t* minimumChargeCurrent) {
        const bool status = readRegister(EEPROM_IMIN, *minimumChargeCurrent) == Error::None;
        return status;
    }

    /**
     *@brief Write charge voltage register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeMinimumChargeCurrentRegister(uint8_t data) {
        const bool status = writeRegister(EEPROM_IMIN, data) == Error::None;
        return status;
    }

    /**
     *@brief Read active empty voltage register.
     *
     *@param [OUT] activeEmptyVoltage - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readActiveEmptyVoltage(uint8_t* activeEmptyVoltage) {
        const bool status = readRegister(EEPROM_VAE, *activeEmptyVoltage) == Error::None;
        return status;
    }

    /**
     *@brief Write active empty voltage register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeActiveEmptyVoltage(uint8_t data) {
        const bool status = writeRegister(EEPROM_VAE, data) == Error::None;
        return status;
    }

    /**
     *@brief Read active empty current register.
     *
     *@param [OUT] activeChargeCurrent - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readActiveEmptyCurrent(uint8_t* activeChargeCurrent) {
        const bool status = readRegister(EEPROM_IAE, *activeChargeCurrent) == Error::None;
        return status;
    }

    /**
     *@brief Write active empty current register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeActiveEmptyCurrent(uint8_t data) {
        const bool status = writeRegister(EEPROM_IAE, data) == Error::None;
        return status;
    }

    /**
     *@brief Read aging capacity register.
     *
     *@param [OUT] agingCapacity - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readAgingCapacity(uint16_t* agingCapacity) {
        const bool status = readRegister(EEPROM_AC_MSB, *agingCapacity) == Error::None;
        return status;
    }

    /**
     *@brief Write aging capacity register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeAgingCapacity(uint8_t* data) {
        const bool status = writeRegister(EEPROM_AC_MSB, *data) == Error::None;
        return status;
    }

    /**
     *@brief Read age scalar register.
     *
     *@param [OUT] ageScalar - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readAgeScalar(uint8_t* ageScalar) {
        const bool status = readRegister(AS, *ageScalar) == Error::None;
        return status;
    }

    /**
     *@brief Write age scalar register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeAgeScalar(uint8_t data) {
        const bool status = writeRegister(AS, data) == Error::None;
        return status;
    }

    /**
     *@brief Read remaining active absolute capacity register.
     *
     *@param [OUT] remainingActiveAbsoluteCapacity - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readRemainingActiveAbsoluteCapacityRegister(uint16_t* remainingActiveAbsoluteCapacity) {
        const bool status = readRegister(RAAC_MSB, *remainingActiveAbsoluteCapacity) == Error::None;
        return status;
    }

    /**
     *@brief Read remaining standby absolute capacity register.
     *
     *@param [OUT] remainingStandbyAbsoluteCapacity - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readRemainingStandbyAbsoluteCapacityRegister(uint16_t* remainingStandbyAbsoluteCapacity) {
        const bool status = readRegister(RSAC_MSB, *remainingStandbyAbsoluteCapacity) == Error::None;
        return status;
    }

    /**
     *@brief Read remaining active relative capacity register.
     *
     *@param [OUT] remainingActiveRelativeCapacity - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readRemainingActiveRelativeCapacityRegister(uint8_t* remainingActiveRelativeCapacity) {
        const bool status = readRegister(RARC, *remainingActiveRelativeCapacity) == Error::None;
        return status;
    }

    /**
     *@brief Read remaining standby relative capacity register.
     *
     *@param [OUT] remainingStandbyRelativeCapacity - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readRemainingStandbyRelativeCapacityRegister(uint8_t* remainingStandbyRelativeCapacity) {
        const bool status = readRegister(RSRC, *remainingStandbyRelativeCapacity) == Error::None;
        return status;
    }

    /**
     *@brief Read status register.
     *
     *@param [OUT] data - variable for register
     *
     *@retval   true if reading register is successful
     *@retval   false if an error occurred
     */

    bool readStatus(uint8_t* data) {
        const bool status = readRegister(STATUS, *data) == Error::None;
        return status;
    }

    /**
     *@brief Check charge termination flag.
     *
     *@retval   true if ( VOLT > VCHG ) AND ( 0 < IAVG < IMIN ) continuously for a period between two IAVG register updates ( 28s to 56s ).
     *@retval   false if RARC < 90%
     */

    bool checkChargeTerminationFlag() {
        uint8_t data;
        bool status;
        readRegister(STATUS, data);
        status = data & CHGTF;
        return status;
    }

    /**
     *@brief Check active empty flag.
     *
     *@retval   true if VOLT < VAE
     *@retval   false if RARC > 5%
     */

    bool checkActiveEmptyFlag() {
        uint8_t data;
        bool status;
        readRegister(STATUS, data);
        status = data & AEF;
        return status;
    }

    /**
     *@brief Check standby empty flag.
     *
     *@retval   true if RSRC < 10%
     *@retval   false if RSRC > 15%
     */

    bool checkStandbyEmptyFlag() {
        uint8_t data;
        bool status;
        readRegister(STATUS, data);
        status = data & SEF;
        return status;
    }

    /**
     *@brief Check learn flag.
     *
     *@retval   true if ( VOLT falls from above VAE to below VAE ) AND ( CURRENT > IAE )
     *@retval   false if ( CHGTF = 1 ) OR ( CURRENT < +100μV ) OR ( ACR = 0 **) OR ( ACR written or recalled from EEPROM) OR ( SLEEP Entered )
     */

    bool checkLearnFlag() {
        uint8_t data;
        bool status;
        readRegister(STATUS, data);
        status = data & LEARNF;
        return status;
    }

    /**
     *@brief Check under voltage flag.
     *
     *@retval   true if VOLT < VSLEEP
     *@retval   false if user set
     */

    bool checkUnderVoltageFlag() {
        uint8_t data;
        bool status;
        readRegister(STATUS, data);
        status = data & UVF;
        return status;
    }

    /**
     *@brief Set 0 to under voltage flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool resetUnderVoltageFlag() { return clearBitsInRegister(STATUS, static_cast<uint8_t>(UVF)) == Error::None; }

    /**
     *@brief Check power on reset flag.
     *
     *@retval   true if Power-Up by hardware.
     *@retval   false if user set
     */

    bool checkPowerOnResetFlag() {
        uint8_t data;
        bool status;
        readRegister(STATUS, data);
        status = data & PORF;
        return status;
    }

    /**
     *@brief Set 0 to power on reset flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool resetPowerOnResetFlag() { return clearBitsInRegister(STATUS, static_cast<uint8_t>(PORF)) == Error::None; }

    /**
     *@brief Check under voltage sleep flag.
     *
     *@retval   true if enable transition.
     *@retval   false if disabled transition.
     */

    bool checkUnderVoltageSleepFlag() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_CONTROL, data);
        status = data & UVEN;
        return status;
    }

    /**
     *@brief Set 0 to under voltage sleep flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool resetUnderVoltageSleepFlag() { return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN)) == Error::None; }

    /**
     *@brief Set 1 to under voltage sleep flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool setUnderVoltageSleepFlag() { return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN)) == Error::None; }

    /**
     *@brief Check power mode flag.
     *
     *@retval   true if transition is enabled.
     *@retval   false if transition is disabled.
     */

    bool checkPowerModeFlag() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_CONTROL, data);
        status = data & PMOD;
        return status;
    }

    /**
     *@brief Set 0 to power mode flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool resetPowerModeFlag() { return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD)) == Error::None; }

    /**
     *@brief Set 1 to power mode flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool setPowerModeFlag() { return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD)) == Error::None; }

    /**
     *@brief Check slave address write flag.
     *
     *@retval   true if Slave Address Write Enable is enabled to write .
     *@retval   false if Slave Address Write Enable is disabled to write.
     */

    bool checkSlaveAddressWriteFlag() {
        uint8_t data;
        bool status;
        readRegister(SFR, data);
        status = data & SAWE;
        return status;
    }

    /**
     *@brief Set 0 to Slave Address Write Enable flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool resetSlaveAddressWriteFlag() { return clearBitsInRegister(SFR, static_cast<uint8_t>(SAWE)) == Error::None; }

    /**
     *@brief Set 1 to Slave Address Write Enable flag.
     *
     *@retval   true if seting bit in register is successful
     *@retval   false if an error occurred
     */

    bool setSlaveAddressWriteFlag() { return setBitsInRegister(SFR, static_cast<uint8_t>(SAWE)) == Error::None; }

    /**
     *@brief Check PIO Sense and Control flag.
     *
     *@retval   true if PIO pin ≥ Vih.
     *@retval   false if PIO pin ≤ Vil.
     */

    bool checkPIOSenseControlFlag() {
        uint8_t data;
        bool status;
        readRegister(SFR, data);
        status = data & PIOSC;
        return status;
    }

    /**
     *@brief Set 0 to PIO Sense and Control flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool resetPIOSenseControlFlag() { return clearBitsInRegister(SFR, static_cast<uint8_t>(PIOSC)) == Error::None; }

    /**
     *@brief Set 1 to PIO Sense and Control flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool setPIOSenseControlFlag() { return setBitsInRegister(SFR, static_cast<uint8_t>(PIOSC)) == Error::None; }

    /**
     *@brief Check EEPROM Copy flag.
     *
     *@retval   true if Copy Data command executed.
     *@retval   false if Copy Data command completes.
     */

    bool checkEEPROMCopyFlag() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_REGISTER, data);
        status = data & EEC;
        return status;
    }

    /**
     *@brief Check EEPROM Lock flag.
     *
     *@retval   true if Lock command enabled.
     *@retval   false if Lock command disabled.
     */

    bool checkEEPROMLockFlag() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_REGISTER, data);
        status = data & LOCK;
        return status;
    }

    /**
     *@brief Set 1 to EEPROM Lock flag.
     *
     *@retval   true if setting bit in register is successful
     *@retval   false if an error occurred
     */

    bool setEEPROMLockFlag() { return setBitsInRegister(EEPROM_REGISTER, static_cast<uint8_t>(LOCK)) == Error::None; }

    /**
     *@brief Check EEPROM Block 1 Lock flag.
     *
     *@retval   true if Lock command enabled.
     *@retval   false if Lock command disabled.
     */

    bool checkEEPROMBlock1LockFlag() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_REGISTER, data);
        status = data & BL1;
        return status;
    }

    /**
     *@brief Check EEPROM Block 0 Lock flag.
     *
     *@retval   true if Lock command enabled.
     *@retval   false if Lock command disabled.
     */

    bool checkEEPROMBlock0LockFlag() {
        uint8_t data;
        bool status;
        readRegister(EEPROM_REGISTER, data);
        status = data & BL0;
        return status;
    }

    /**
     *@brief Write slave address register.
     *
     *@param [IN] data - variable to register
     *
     *@retval   true if writing to register is successful
     *@retval   false if an error occurred
     */

    bool writeSlaveAddress(uint8_t data) {
        const bool status = writeRegister(EEPROM_2WIRE_ADDRESS, data) == Error::None;
        return status;
    }
};

#endif
