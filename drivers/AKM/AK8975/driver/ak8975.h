/* =========================================================================================== *//**
 @file        AK8975.h
 @authors     Pawel� Okas
 @version     $Id$
 @package
 @brief       AK9875 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Interface for 3-axis magnetometer.

 *//* ============================================================================================ */

#ifndef MAGNETOMETERAK8975_H_
#define MAGNETOMETERAK8975_H_

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "units/magnetic.h"
#include "math/vector.h"
/**
 * \addtogroup Devices
 * @{
 * @class AK8975
 * @}
 */
class MPU9150;
class AK8975: public microhal::I2CDevice {
    friend MPU9150;
public:
    enum Registers : uint8_t {
        WIA = 0x00,
        INFO = 0x01,
        ST1 = 0x02,
        HXL = 0x03,
        HXH = 0x04,
        HYL = 0x05,
        HYH = 0x06,
        HZL = 0x07,
        HZH = 0x08,
        ST2 = 0x09,
        //Write/read Register
        CNTL = 0x0A,
        RSV = 0x0B, ///<RSV register is reserved. Do not use this register.
        ASTC = 0x0C,
        TS1 = 0x0D, ///< SHIPMENT TEST, DO NOT USE
        TS2 = 0x0E, ///< SHIPMENT TEST, DO NOT USE
        I2CDIS = 0x0F,
        //Read-only Register
        ASAX = 0x10,
        ASAY = 0x11,
        ASAZ = 0x12
    };
    enum RegistersFlags : uint8_t {
        ST1_DRDY = 0x01, I2CDIS_I2CDIS = 0x01, ASTC_SELF = 0x40
    };
    /**
     * @brief This enum describes possible I2C address
     */
    enum PossibleI2CAddress {
        I2C_ADDRESS_0 = 0x18, //!< I2C_ADDRESS_0 this is default address for MPU9150 sensor
        I2C_ADDRESS_1 = 0x1A, //!< I2C_ADDRESS_1
        I2C_ADDRESS_2 = 0x1C, //!< I2C_ADDRESS_2
        I2C_ADDRESS_3 = 0x1E //!< I2C_ADDRESS_3
    };
    /**
     * Possible operating mode.
     */
    enum Mode : uint8_t {
        POWER_DOWN = (uint8_t) 0x00,        //!< POWER_DOWN
        SINGLE_MEASUREMENT = (uint8_t) 0x01,        //!< SINGLE_MEASUREMENT
        SELF_TEST = (uint8_t) 0x08,         //!< SELF_TEST
        FUSE_ROM_ACCESS = (uint8_t) 0x0F,   //!< FUSE_ROM_ACCESS
        UNKNOWN_MODE              //!< UNKNOWN_MODE
    };

    typedef enum {
         RANGE_1229uT,
         UNKNOWN_RANGE
    } FullScaleRange;

    using MagneticFieldVector = microhal::Vector<microhal::Magnetic, 3>;
    //------------------------------------------ functions ----------------------------------------
    AK8975(microhal::I2C &i2c, uint8_t addr);
   // virtual ~AK8975();
    //inline functions
    inline bool getDeviceID(uint8_t &id);
    inline bool getInfo(uint8_t &info);
    inline bool getMode(Mode &mode);
    inline bool reset();
    inline bool disableI2C();

    bool isDataReady();
    bool calibrate();
    bool setMode(Mode mode);

    bool setSelfTest();

    FullScaleRange getRange(void);
    bool setRange(FullScaleRange range);

    /** This function return actual sampling frequency of magnetometer.
     * @return Actual sampling frequency in Hz.
     */
    uint32_t getSamplingFrequency() {
        return 0;
    }
private:
    MagneticFieldVector magneticField;
    static constexpr float cSensitivity = 3.331977217;
    float scallingFactor[3];

    inline void update(uint8_t *buffer);
    bool readAdjustment(uint8_t &x, uint8_t &y, uint8_t &z);
};
/* ************************************************************************************************
 * INLINE FUNCTIONS
 */
/**This function read device ID of AKM.
 *
 * @param[out] id - reference where id will be written
 * @retval true if device id was read.
 * @retval false if an error occurred.
 */
bool AK8975::getDeviceID(uint8_t &id) {
    return readRegister(WIA, id);
}
/**This function read info register
 *
 * @param[out] info - read value
 * @retval true if device info was read.
 * @retval false if an error occurred.
 */
bool AK8975::getInfo(uint8_t &info) {
    return readRegister(INFO, info);
}
/**This function get operation mode of AKM magnetometer.
 *
 * @param[out] mode -
 * @retval true if mode was read successful.
 * @retval false if an error occurred.
 */
bool AK8975::getMode(Mode &mode) {
    return readRegister(CNTL, (uint8_t&) mode);
}
/** @brief This function reset magnetometer.
 *
 * @retval true if mode was read successful.
 * @retval false if an error occurred.
 */
bool AK8975::reset() {
    return setMode(POWER_DOWN);
}
/** @brief This function disable I2C operation.
 *
 * @return
 */
bool AK8975::disableI2C() {
    /* INFO FROM DATASHEET:
     * This register disables I2C bus interface. I2C bus interface is enabled in default.
     * To disable I2C bus interface,write “00011011” to I2CDIS register. Then I2CDIS bit turns
     * to “1” and I2C bus interface is disabled. Once I2CDIS is turned to “1” and I2C bus interface
     * is disabled, re-setting I2CDIST to “0” is prohibited. To enable I2C bus interface, reset
     * AK8975/C by turning VDD or VID to OFF (0V) once.
     */
    return writeRegister(I2CDIS, static_cast<uint8_t>(0b00011011));
}
/**
 *
 * @retval true if magnetic field strength was completed successfully.
 * @retval false if an error occurred.
 */
void AK8975::update(uint8_t *ptr) {
    int16_t *buffer = (int16_t*) ptr;
    //storage data in vector
//    magneticField.set((float) buffer[0] * scallingFactor[0], (float) buffer[1] * scallingFactor[1],
//            (float) buffer[2] * scallingFactor[2]);

    magneticField[0] = static_cast<float>(buffer[0]) * scallingFactor[0];
    magneticField[1] = static_cast<float>(buffer[1]) * scallingFactor[1];
    magneticField[2] = static_cast<float>(buffer[2]) * scallingFactor[2];
}
#endif /* MAGNETOMETERMPU9150_H_ */
