/* =========================================================================================== *//**
 @file        mpu9150.h
 @authors     Pawel Okas
 @version     $Id$
 @package     HALFRED device port
 @brief       MPU9150 driver
 @hardware    none
 @copyright   $Copyright$
 @details

 *//* ============================================================================================ */

#ifndef MPU9150_H_
#define MPU9150_H_

//#define MPU9150_USE_DMP   //todo implement dmp features
/* ************************************************************************************************
 * INCLUDES
 */
#include "ak8975.h"
#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "gyroscopeMPU9150.h"
#include "accelerometerMPU9150.h"
#include "thermometerMPU9150.h"
#ifdef MPU9150_USE_DMP
#include "src/mpu9150/DMP/dmpMPU9150.h"
#endif

/**
 * \addtogroup Devices
 * @{
 *      @addtogroup MPU9150
 *      @{
 *          @class MPU9150
 *      @}
 * @}
 *
 * \brief   The MPU9150 class provides functions to control sensor
 */
class MPU9150: public microhal::I2CDevice {
    /** \brief MPU9150 registers map */
    enum Registers : uint8_t {
        ACCEL_OFFS = 0x06, PROD_ID = 0x0C,

        SELF_TEST_X = 0x0D,       //!< SELF_TEST_X
        SELF_TEST_Y = 0x0E,       //!< SELF_TEST_Y
        SELF_TEST_Z = 0x0F,       //!< SELF_TEST_Z
        SELF_TEST_A = 0x10,       //!< SELF_TEST_A
        SMPLRT_DIV = 0x19,        //!< SMPLRT_DIV
        CONFIG = 0x1A,            //!< CONFIG
        GYRO_CONFIG = 0x1B,       //!< GYRO_CONFIG
        ACCEL_CONFIG = 0x1C,      //!< ACCEL_CONFIG
        ACCEL_CONFIG2 = 0x1D,            //!< FF_THR
        LP_ACCEL_ODR = 0x1E,            //!< FF_DUR
        MOTION_THR = 0x1F,           //!< MOT_THR
        MOTION_DUR = 0x20,           //!< MOT_DUR
        ZRMOT_THR = 0x21,         //!< ZRMOT_THR
        ZRMOT_DUR = 0x22,         //!< ZRMOT_DUR
        FIFO_EN = 0x23,           //!< FIFO_EN
        I2C_MST_CTRL = 0x24,      //!< I2C_MST_CTRL
        I2C_SLV0_ADDR = 0x25,     //!< I2C_SLV0_ADDR
        I2C_SLV0_REG = 0x26,      //!< I2C_SLV0_REG
        I2C_SLV0_CTRL = 0x27,     //!< I2C_SLV0_CTRL
        I2C_SLV1_ADDR = 0x28,     //!< I2C_SLV1_ADDR
        I2C_SLV1_REG = 0x29,      //!< I2C_SLV1_REG
        I2C_SLV1_CTRL = 0x2A,     //!< I2C_SLV1_CTRL
        I2C_SLV2_ADDR = 0x2B,     //!< I2C_SLV2_ADDR
        I2C_SLV2_REG = 0x2C,      //!< I2C_SLV2_REG
        I2C_SLV2_CTRL = 0x2D,     //!< I2C_SLV2_CTRL
        I2C_SLV3_ADDR = 0x2E,     //!< I2C_SLV3_ADDR
        I2C_SLV3_REG = 0x2F,      //!< I2C_SLV3_REG
        I2C_SLV3_CTRL = 0x30,     //!< I2C_SLV3_CTRL
        I2C_SLV4_ADDR = 0x31,     //!< I2C_SLV4_ADDR
        I2C_SLV4_REG = 0x32,      //!< I2C_SLV4_REG
        I2C_SLV4_DO = 0x33,       //!< I2C_SLV4_DO
        I2C_SLV4_CTRL = 0x34,     //!< I2C_SLV4_CTRL
        I2C_SLV4_DI = 0x35,       //!< I2C_SLV4_DI
        I2C_MST_STATUS = 0x36,    //!< I2C_MST_STATUS
        INT_PIN_CFG = 0x37,       //!< INT_PIN_CFG
        INT_ENABLE = 0x38,        //!< INT_ENABLE
        DMP_INT_STATUS = 0x39,
        INT_STATUS = 0x3A,        //!< INT_STATUS
        ACCEL_XOUT_H = 0x3B,      //!< ACCEL_XOUT_H
        ACCEL_XOUT_L = 0x3C,      //!< ACCEL_XOUT_L
        ACCEL_YOUT_H = 0x3D,      //!< ACCEL_YOUT_H
        ACCEL_YOUT_L = 0x3E,      //!< ACCEL_YOUT_L
        ACCEL_ZOUT_H = 0x3F,      //!< ACCEL_ZOUT_H
        ACCEL_ZOUT_L = 0x40,      //!< ACCEL_ZOUT_L
        TEMP_OUT_H = 0x41,        //!< TEMP_OUT_H
        TEMP_OUT_L = 0x42,        //!< TEMP_OUT_L
        GYRO_XOUT_H = 0x43,       //!< GYRO_XOUT_H
        GYRO_XOUT_L = 0x44,       //!< GYRO_XOUT_L
        GYRO_YOUT_H = 0x45,       //!< GYRO_YOUT_H
        GYRO_YOUT_L = 0x46,       //!< GYRO_YOUT_L
        GYRO_ZOUT_H = 0x47,       //!< GYRO_ZOUT_H
        GYRO_ZOUT_L = 0x48,       //!< GYRO_ZOUT_L
        EXT_SENS_DATA_00 = 0x49,  //!< EXT_SENS_DATA_00
        EXT_SENS_DATA_01 = 0x4A,  //!< EXT_SENS_DATA_01
        EXT_SENS_DATA_02 = 0x4B,  //!< EXT_SENS_DATA_02
        EXT_SENS_DATA_03 = 0x4C,  //!< EXT_SENS_DATA_03
        EXT_SENS_DATA_04 = 0x4D,  //!< EXT_SENS_DATA_04
        EXT_SENS_DATA_05 = 0x4E,  //!< EXT_SENS_DATA_05
        EXT_SENS_DATA_06 = 0x4F,  //!< EXT_SENS_DATA_06
        EXT_SENS_DATA_07 = 0x50,  //!< EXT_SENS_DATA_07
        EXT_SENS_DATA_08 = 0x51,  //!< EXT_SENS_DATA_08
        EXT_SENS_DATA_09 = 0x52,  //!< EXT_SENS_DATA_09
        EXT_SENS_DATA_10 = 0x53,  //!< EXT_SENS_DATA_10
        EXT_SENS_DATA_11 = 0x54,  //!< EXT_SENS_DATA_11
        EXT_SENS_DATA_12 = 0x55,  //!< EXT_SENS_DATA_12
        EXT_SENS_DATA_13 = 0x56,  //!< EXT_SENS_DATA_13
        EXT_SENS_DATA_14 = 0x57,  //!< EXT_SENS_DATA_14
        EXT_SENS_DATA_15 = 0x58,  //!< EXT_SENS_DATA_15
        EXT_SENS_DATA_16 = 0x59,  //!< EXT_SENS_DATA_16
        EXT_SENS_DATA_17 = 0x5A,  //!< EXT_SENS_DATA_17
        EXT_SENS_DATA_18 = 0x5B,  //!< EXT_SENS_DATA_18
        EXT_SENS_DATA_19 = 0x5C,  //!< EXT_SENS_DATA_19
        EXT_SENS_DATA_20 = 0x5D,  //!< EXT_SENS_DATA_20
        EXT_SENS_DATA_21 = 0x5E,  //!< EXT_SENS_DATA_21
        EXT_SENS_DATA_22 = 0x5F,  //!< EXT_SENS_DATA_22
        EXT_SENS_DATA_23 = 0x60,  //!< EXT_SENS_DATA_23
        MOT_DETECT_STATUS = 0x61, //!< MOT_DETECT_STATUS
        I2C_SLV0_DO = 0x63,       //!< I2C_SLV0_DO
        I2C_SLV1_DO = 0x64,       //!< I2C_SLV1_DO
        I2C_SLV2_DO = 0x65,       //!< I2C_SLV2_DO
        I2C_SLV3_DO = 0x66,       //!< I2C_SLV3_DO
        I2C_MST_DELAY_CTRL = 0x67,       //!< I2C_MST_DELAY_CTRL
        SIGNAL_PATH_RESET = 0x68, //!< SIGNAL_PATH_RESET
        MOT_DETECT_CTRL = 0x69,   //!< MOT_DETECT_CTRL
        USER_CTRL = 0x6A,         //!< USER_CTRL
        PWR_MGMT_1 = 0x6B,        //!< PWR_MGMT_1
        PWR_MGMT_2 = 0x6C,        //!< PWR_MGMT_2
        //dmp reg
        BANK_SEL = 0x6D,
        MEM_START_ADR = 0x6E,
        MEM_R_W = 0x6F,
        PRGM_START_H = 0x70,
        PRGM_START_L = 0x71,
        //end dmp reg
        FIFO_COUNTH = 0x72,       //!< FIFO_COUNTH
        FIFO_COUNTL = 0x73,       //!< FIFO_COUNTL
        FIFO_R_W = 0x74,          //!< FIFO_R_W
        WHO_AM_I = 0x75,          //!< WHO_AM_I

    };

    /**
     * \brief MPU9150 general Flags
     */
    enum Flags {
        TEMP_RESET = 0x01,             ///< thermometer reset flag
        ACCEL_RESET = 0x02,            ///< accelerometer reset flag
        GYRO_RESET = 0x04,             ///< gyroscope reset flag
        INT_PIN_CFG_I2C_BYPASS_EN = 0x02,      ///< I2C_BYPASS_EN_FLAG

        PWR_MGMT_1_RESET = 0x80,
        PWR_MGMT_1_CLK_PLL = 0x01,

        USER_CTRL_I2C_MST_EN = 0x20,
        USER_CTRL_FIFO_EN = 0x40,

        I2C_SLVx_CTRL_I2C_SLVx_EN = 0x80,
        I2C_SLVx_CTRL_I2C_SLVx_BYTE_SW = 0x40,

        I2C_SLVx_ADDR_I2C_SLVx_RW = 0x80
    };

    /**
     * \brief MPU9150 interrupts flag
     * */
    enum Interrupts {
        DATA_READY_INT = 0x01,  ///< new data ready interrupt flag
        FIFO_OVERFLOW_INT = 0x10, ///< FIFO overflow interrupt flag
        I2C_MST_INT = 0x08, ///< I2C master interrupt flag
        DMP_INT = 0x02 ///< DMP data ready interrupt flag
    };

    /**
     * \brief MPU9150 Hardware constans
     */
    enum Hardware {
        BANK_SIZE = 256, MAX_FIFO = 1024, REGISTER_COUNT = 128, MAX_COMPASS_SAMPLE_RATE = 100
    };
    friend AccelerometerMPU9150;
    friend GyroscopeMPU9150;
    friend ThermometerMPU9150;
#ifdef MPU9150_USE_DMP
    friend DMP_MPU9150;
#endif
public:
    enum PossibleI2CAddress {
        DEFAULT_ADDRESS = 0xD0
    };
    /**This enum contains possible values of Digital Low Pass Filter (DLPF).
     *
     *
     * <pre>
     * |------------|----------------------------------|----------------------------------|
     * |            |         ACCELEROMETER            |            GYROSCOPE             |
     * | DLPF_CFG_x | Bandwidth | Delay  | Sample Rate | Bandwidth | Delay  | Sample Rate |
     * |------------+-----------+--------+-------------+-----------+----------------------|
     * | 0          | 260Hz     | 0ms    | 1kHz        | 256Hz     | 0.98ms | 8kHz        |
     * | 1          | 184Hz     | 2.0ms  | 1kHz        | 188Hz     | 1.9ms  | 1kHz        |
     * | 2          | 94Hz      | 3.0ms  | 1kHz        | 98Hz      | 2.8ms  | 1kHz        |
     * | 3          | 44Hz      | 4.9ms  | 1kHz        | 42Hz      | 4.8ms  | 1kHz        |
     * | 4          | 21Hz      | 8.5ms  | 1kHz        | 20Hz      | 8.3ms  | 1kHz        |
     * | 5          | 10Hz      | 13.8ms | 1kHz        | 10Hz      | 13.4ms | 1kHz        |
     * | 6          | 5Hz       | 19.0ms | 1kHz        | 5Hz       | 18.6ms | 1kHz        |
     * | 7          |         -- Reserved --           |   -- Reserved --   | 8kHz    |
     * |------------|----------------------------------|----------------------------------|
     * </pre>
     *
     */
    typedef enum {
        DLPF_CFG_0 = 0x00, //!< DLPF_CFG_0
        DLPF_CFG_1 = 0x01, //!< DLPF_CFG_1
        DLPF_CFG_2 = 0x02, //!< DLPF_CFG_2
        DLPF_CFG_3 = 0x03, //!< DLPF_CFG_3
        DLPF_CFG_4 = 0x04, //!< DLPF_CFG_4
        DLPF_CFG_5 = 0x05, //!< DLPF_CFG_5
        DLPF_CFG_6 = 0x06, //!< DLPF_CFG_6
        DLPF_CFG_7 = 0x07, //!< DLPF_CFG_7
        UNKNOWN
    } DLPF_CFG;
    /**
     * \brief FIFO enabled bit of individual sensors
     */
    typedef enum {
        SLV0_FIFO_EN = (uint8_t) 0x01,
        SLV1_FIFO_EN = (uint8_t) 0x02,
        SLV2_FIFO_EN = (uint8_t) 0x04,
        ACCEL_FIFO_EN = (uint8_t) 0x08, //!< ACCEL_FIFO_EN
        GYRO_FIFO_EN = (uint8_t) 0x70, //!< GYRO_FIFO_EN
        TEMP_FIFO_EN = (uint8_t) 0x80,  //!< TEMP_FIFO_EN
        ALL_SENSOR_FIFO_EN = ACCEL_FIFO_EN | GYRO_FIFO_EN | TEMP_FIFO_EN
    } FifoData;

    AK8975 magnetometer; ///< instance of magnetometer
    AccelerometerMPU9150 accelerometer; ///< instance of accelerometer
    GyroscopeMPU9150 gyroscope; ///< instance of gyroscope
    ThermometerMPU9150 thermometer; ///< instance of thermometer
#ifdef MPU9150_USE_DMP
    DMP_MPU9150 dmp;
#endif
    MPU9150(microhal::I2C &i2c, uint8_t address = DEFAULT_ADDRESS);
    //virtual ~MPU9150();

    //inline functions
    inline bool interruptEnable(Interrupts interrupt);
    inline bool interruptDisable(Interrupts interrupt);
    inline bool fifoEnableStream(FifoData fifo);
    inline bool fifoDisableStream(FifoData fifo);
    inline bool fifoEnable();
    inline bool fifoDisable();
    inline bool setBypasMode(bool state);
    inline bool getBypasMode();
    inline bool setI2CMaster(bool enable);

    bool init(microhal::SerialPort &port);

    bool lpAccelMode(uint16_t rate);
    bool setIntLatched(bool enable);

    uint16_t getNumberOfSamplesInFIFO(void);
    bool updateFromFIFO(void);
    bool updateFromRaw(void);

    bool resetFIFO();

    uint16_t setSamplingFrequency(uint16_t freq);
    uint16_t getSamplingFrequency();

    bool setDLPF(DLPF_CFG dlpf);
    DLPF_CFG getDLPF_CFG(void);

    bool isNewDataAvailable(void);

    bool setSensors(uint8_t sensors);

    bool setupCompass(bool setupForDMP = false);
    bool setCompassSampleRate(uint16_t rate);
#ifdef MPU9150_USE_DMP
    bool setDMPstate(bool state);
    bool isNewDataFromDMP();
#endif
private:
    uint8_t streamsInFifo = 0;
    uint16_t fifoLength = 0;

    void interruptFunction(void);

    void updateFifoLength() {
        fifoLength = 0;
        if (streamsInFifo & ACCEL_FIFO_EN) {
            fifoLength += 6;
        }
        if (streamsInFifo & TEMP_FIFO_EN) {
            fifoLength += 2;
        }
        if (streamsInFifo & GYRO_FIFO_EN) {
            fifoLength += 6;
        }
        if (streamsInFifo & SLV0_FIFO_EN) {
            fifoLength += 6;
        }
    }

    bool writeMem(uint16_t memAddr, const uint8_t *data, uint16_t length);
    bool readMem(uint16_t memAddr, uint8_t *data, uint16_t length);
    bool loadFirmware(uint16_t startAddr, const uint8_t *firmware, uint16_t length);
    bool readFifoStream(uint8_t *data, uint16_t length);

    // static void slot(void *obj){
    //   static_cast<MPU9150*>(obj)->interruptFunction();
    //}
};
/* ************************************************************************************************
 * INLINE FUNCTIONS
 */
/**
 * \brief This function enable interrupt.
 * @param interrupt - Interrupts source to enable.
 * @retval true if interrupt was enabled.
 * @retval false if an error occurred.
 */
bool MPU9150::interruptEnable(Interrupts interrupt) {
    return setBitsInRegister(INT_ENABLE, interrupt);
}
/**
 * \brief This function disable interrupt.
 * @param interrupt - Interrupts source to enable.
 * @retval true if interrupt was disabled.
 * @retval false if an error occurred.
 */
bool MPU9150::interruptDisable(Interrupts interrupt) {
    return clearBitsInRegister(INT_ENABLE, interrupt);
}
/**
 * This function enable sending data to FIFO from selected device
 * @param fifo Enum describing FIFO enabled bit of device
 * @return Return true if completed successfully.
 */
bool MPU9150::fifoEnableStream(FifoData fifo) {
    streamsInFifo |= fifo;
    updateFifoLength();
    return setBitsInRegister(FIFO_EN, fifo);
}
/**
 * This function disable sending data to FIFO form selected device.
 * @param fifo Enum describing FIFO enabled bit of device
 * @return Return true if completed successfully.
 */
bool MPU9150::fifoDisableStream(FifoData fifo) {
    streamsInFifo &= ~fifo;
    updateFifoLength();
    return clearBitsInRegister(FIFO_EN, fifo);
}
/**
 * \brief This function set fifo enabled.
 * @return Return true if completed successfully.
 */
bool MPU9150::fifoEnable() {
    return setBitsInRegister(USER_CTRL, USER_CTRL_FIFO_EN);
}
/**
 * \brief This function set fifo disabled
 * @return Return true if completed successfully.
 */
bool MPU9150::fifoDisable() {
    return clearBitsInRegister(USER_CTRL, USER_CTRL_FIFO_EN);
}
/** \brief This function enable I2C bypas mode
 *
 * @param state
 * @return
 */
bool MPU9150::setBypasMode(bool state) {
    if (state == true) {
        return setBitsInRegister(INT_PIN_CFG, INT_PIN_CFG_I2C_BYPASS_EN);
    } else {
        return clearBitsInRegister(INT_PIN_CFG, INT_PIN_CFG_I2C_BYPASS_EN);
    }
}
/**
 *
 * @return
 */
bool MPU9150::getBypasMode() {
    uint8_t pwrMgt;
    if (readRegister(INT_PIN_CFG, pwrMgt) == false) {
        return false;
    }
    return (pwrMgt & INT_PIN_CFG_I2C_BYPASS_EN);
}
/**
 *
 * @param enable
 * @return
 */
bool MPU9150::setI2CMaster(bool enable) {
    if (enable == true) {
        return setBitsInRegister(USER_CTRL, USER_CTRL_I2C_MST_EN);
    } else {
        return clearBitsInRegister(USER_CTRL, USER_CTRL_I2C_MST_EN);
    }
}

/**
 * @}
 */

#endif /* MPU9150_H_ */
