/* =========================================================================================== *//**
 @file        mpu9150.cpp
 @authors     Pawel Okas
 @version     $Id$
 @package     MPU9150 driver
 @brief
 @hardware    none
 @copyright   $Copyright$
 @details

 Created on: 22-11-2013
 Last modification on: 22-11-2013
 *//* ============================================================================================ */

#include "mpu9150.h"
#include "gyroscopeMPU9150.h"
#include "accelerometerMPU9150.h"

#include <string.h>

enum lp_accel_rate_e {
    INV_LPA_1_25HZ, INV_LPA_5HZ, INV_LPA_20HZ, INV_LPA_40HZ
};

#define min(a,b) ((a<b)?a:b)

#define BIT_I2C_MST_VDDIO (0x80)
#define BIT_FIFO_EN (0x40)
#define BIT_DMP_EN (0x80)
#define BIT_FIFO_RST (0x04)
#define BIT_DMP_RST (0x08)
#define BIT_FIFO_OVERFLOW (0x10)
#define BIT_DATA_RDY_EN (0x01)
#define BIT_DMP_INT_EN (0x02)
#define BIT_MOT_INT_EN (0x40)
#define BITS_FSR (0x18)
#define BITS_LPF (0x07)
#define BITS_HPF (0x07)
#define BITS_CLK (0x07)
#define BIT_FIFO_SIZE_1024 (0x40)
#define BIT_FIFO_SIZE_2048 (0x80)
#define BIT_FIFO_SIZE_4096 (0xC0)
#define BIT_RESET (0x80)
#define BIT_SLEEP (0x40)
#define BIT_S0_DELAY_EN (0x01)
#define BIT_S2_DELAY_EN (0x04)
#define BITS_SLAVE_LENGTH (0x0F)
#define BIT_SLAVE_BYTE_SW (0x40)
#define BIT_SLAVE_GROUP (0x10)
#define BIT_SLAVE_EN (0x80)
#define BIT_I2C_READ (0x80)
#define BITS_I2C_MASTER_DLY (0x1F)
#define BIT_AUX_IF_EN (0x20)
#define BIT_ACTL (0x80)
#define BIT_LATCH_EN (0x20)
#define BIT_ANY_RD_CLR (0x10)
#define BIT_BYPASS_EN (0x02)
#define BITS_WOM_EN (0xC0)
#define BIT_LPA_CYCLE (0x20)
#define BIT_STBY_XA (0x20)
#define BIT_STBY_YA (0x10)
#define BIT_STBY_ZA (0x08)
#define BIT_STBY_XG (0x04)
#define BIT_STBY_YG (0x02)
#define BIT_STBY_ZG (0x01)
#define BIT_STBY_XYZA (BIT_STBY_XA | BIT_STBY_YA | BIT_STBY_ZA)
#define BIT_STBY_XYZG (BIT_STBY_XG | BIT_STBY_YG | BIT_STBY_ZG)

void delay_ms(uint32_t i) {
    volatile uint32_t j = 30000;
    while (i) {
        i--;
        while (j) {
            j--;
            j++;
            j--;
        }
    }
}
/**
 * \brief Default constructor for MPU9150 class. Initialize I2C device number and MPU9150 address.
 * @param i2cNr - I2C number
 * @param address - device address
 */
MPU9150::MPU9150(microhal::I2C &i2c, uint8_t address) :
        I2CDevice(i2c, address), accelerometer(this), gyroscope(this), magnetometer(i2c,
                AK8975::I2C_ADDRESS_0), thermometer(this), streamsInFifo(0)
#ifdef MPU9150_USE_DMP
, dmp(this)
#endif
{
    writeRegister(PWR_MGMT_1, static_cast<uint8_t>(0x00));
}
/**
 * Initialize MPU9150
 * @return Return true if successfull initialization.
 */
bool MPU9150::init(microhal::SerialPort &port) {
    uint8_t data[6], rev;

    // Reset device
    if (writeRegister(PWR_MGMT_1, static_cast<uint8_t>(PWR_MGMT_1_RESET)) == false) return false;
    delay_ms(100);

    // Wake up chip
    if (writeRegister(PWR_MGMT_1, static_cast<uint8_t>(0x00)) == false) return false;

    // Check product revision
    if (readRegisters(ACCEL_OFFS, data, 6) == false) return false;
    rev = ((data[5] & 0x01) << 2) | ((data[3] & 0x01) << 1) | (data[1] & 0x01);

    if (rev) {
        /* Congrats, these parts are better. */
        if (rev == 1) {
            //st.chip_cfg.accel_half = 1;
        } else if (rev == 2) {
            // st.chip_cfg.accel_half = 0;
        } else {
            port.write("Unsupported software product rev %d.\n"); //, rev);
            return false;
        }
    } else {
        if (readRegister(PROD_ID, data[0]) == false) return false;
        rev = data[0] & 0x0F;
        if (!rev) {
            port.write(
                    "Product ID read as 0 indicates device is either incompatible or an MPU3050.\n");
            return false;
        } else if (rev == 4) {
            port.write("Half sensitivity part found.\n");
            //st.chip_cfg.accel_half = 1;
        } else {
            //    st.chip_cfg.accel_half = 0;
        }
    }

    if (gyroscope.setRange(GyroscopeMPU9150::RANGE_2000) == false) return false;
    if (accelerometer.setRange(AccelerometerMPU9150::RANGE_2g) == false) return false;
    //if (setDLPF(42)) return -1;
    if (setSamplingFrequency(50) == 0) return false;
    //if (mpu_configure_fifo(0)) return -1;

    //if (int_param) reg_int_cb (int_param);

    if (setupCompass() == false) return false;
    if (setCompassSampleRate(10) == false) return false;

    return setSensors(0);
}
/**
 *
 * @param setupForDMP
 * @return
 */
bool MPU9150::setupCompass(bool setupForDMP) {
    //connect magnetometer to main I2C bus
    if (setBypasMode(true) == false) return false;

    //callibrate magnetometer
    if (magnetometer.calibrate() == false) return false;

    //disconnect bus
    if (setBypasMode(false) == false) return false;

    // Set up master mode, master clock, and ES bit.
    if (writeRegister(I2C_MST_CTRL, static_cast<uint8_t>(0x40)) == false) return false;

    // Slave 0 reads from AKM data registers.
    if (writeRegister(I2C_SLV0_ADDR, static_cast<uint8_t>(I2C_SLVx_ADDR_I2C_SLVx_RW | (magnetometer.getMyI2CAddress() >> 1))) == false)
        return false;

    // Compass reads start at this register.
    uint8_t addrTmp, lenTmp;

    if (setupForDMP == true) {
        addrTmp = AK8975::ST1;
        lenTmp = 8;
    } else {
        addrTmp = AK8975::HXL;
        lenTmp = 6;
    }
    if (writeRegister(I2C_SLV0_REG, addrTmp) == false) return false;

    // Enable slave 0, 8-byte reads.
    if (writeRegister(I2C_SLV0_CTRL, static_cast<uint8_t>(I2C_SLVx_CTRL_I2C_SLVx_EN | lenTmp)) == false) return false;

    // Slave 1 changes AKM measurement mode.
    if (writeRegister(I2C_SLV1_ADDR, static_cast<uint8_t>(magnetometer.getMyI2CAddress() >> 1)) == false) return false;

    // AKM measurement mode register.
    if (writeRegister(I2C_SLV1_REG, static_cast<uint8_t>(AK8975::CNTL)) == false) return false;

    // Enable slave 1, 1-byte writes.
    if (writeRegister(I2C_SLV1_CTRL, static_cast<uint8_t>(I2C_SLVx_CTRL_I2C_SLVx_EN | 1)) == false) return false;

    // Set slave 1 data.
    if (writeRegister(I2C_SLV1_DO, static_cast<uint8_t>(AK8975::SINGLE_MEASUREMENT)) == false) return false;

    // Trigger slave 0 and slave 1 actions at each sample.
    return writeRegister(I2C_MST_DELAY_CTRL, static_cast<uint8_t>(0x03));
}
/**
 *  @brief      Set compass sampling rate.
 *  The compass on the auxiliary I2C bus is read by the MPU hardware at a
 *  maximum of 100Hz. The actual rate can be set to a fraction of the gyro
 *  sampling rate.
 *
 *  \n WARNING: The new rate may be different than what was requested. Call
 *  gyro_get_compass_sample_rate to check the actual setting.
 *  @param[in]  rate    Desired compass sampling rate (Hz).
 *  @return     0 if successful.
 */
bool MPU9150::setCompassSampleRate(uint16_t rate) {
    //read sampling frequency
    uint8_t sampleRate = getSamplingFrequency();
    //if rate is grager than sampling frequency or grater than maximum sampling rate return false
    if ((rate == 0) || (rate > sampleRate) || (rate > MAX_COMPASS_SAMPLE_RATE)) return false;
    // calculate divider
    uint8_t div = sampleRate / rate - 1;
    //write divider to register
    return writeRegister(I2C_SLV4_CTRL, div);
}
/**
 *  @brief      Enter low-power accel-only mode.
 *  In low-power accel mode, the chip goes to sleep and only wakes up to sample
 *  the accelerometer at one of the following frequencies:
 *  \n MPU6050: 1.25Hz, 5Hz, 20Hz, 40Hz
 *  \n MPU6500: 1.25Hz, 2.5Hz, 5Hz, 10Hz, 20Hz, 40Hz, 80Hz, 160Hz, 320Hz, 640Hz
 *  \n If the requested rate is not one listed above, the device will be set to
 *  the next highest rate. Requesting a rate above the maximum supported
 *  frequency will result in an error.
 *  \n To select a fractional wake-up frequency, round down the value passed to
 *  @e rate.
 *  @param[in]  rate        Minimum sampling rate, or zero to disable LP
 *                          accel mode.
 *  @return     0 if successful.
 */
bool MPU9150::lpAccelMode(uint16_t rate) {
    uint8_t tmp[2];

    if (rate > 40) return false;

    if (!rate) {
        setIntLatched(false);
        tmp[0] = 0;
        tmp[1] = BIT_STBY_XYZG;
        if (writeRegisters(PWR_MGMT_1, tmp, sizeof(tmp)) == false) return false;
        //st.chip_cfg.lp_accel_mode = 0;
        return true;
    }
    /* For LP accel, we automatically configure the hardware to produce latched
     * interrupts. In LP accel mode, the hardware cycles into sleep mode before
     * it gets a chance to deassert the interrupt pin; therefore, we shift this
     * responsibility over to the MCU.
     *
     * Any register read will clear the interrupt.
     */
    setIntLatched(true);

    tmp[0] = BIT_LPA_CYCLE;
    if (rate == 1) {
        tmp[1] = INV_LPA_1_25HZ;
        setDLPF(DLPF_CFG_6);
    } else if (rate <= 5) {
        tmp[1] = INV_LPA_5HZ;
        setDLPF(DLPF_CFG_6);
    } else if (rate <= 20) {
        tmp[1] = INV_LPA_20HZ;
        setDLPF(DLPF_CFG_5);
    } else {
        tmp[1] = INV_LPA_40HZ;
        setDLPF(DLPF_CFG_4);
    }
    tmp[1] = (tmp[1] << 6) | BIT_STBY_XYZG;
    if (writeRegisters(PWR_MGMT_1, tmp, 2) == false) return false;

    //st.chip_cfg.sensors = INV_XYZ_ACCEL;
    //st.chip_cfg.clk_src = 0;
    //st.chip_cfg.lp_accel_mode = 1;
    //configure_fifo(0);

    return true;
}
/**
 *  @brief      Enable latched interrupts.
 *  Any MPU register will clear the interrupt.
 *  @param[in]  enable  1 to enable, 0 to disable.
 *  @return     0 if successful.
 */
bool MPU9150::setIntLatched(bool enable) {
    uint8_t tmp;

    if (readRegister(INT_PIN_CFG, tmp) == false) return false;

    if (enable == true) {
        tmp |= BIT_LATCH_EN | BIT_ANY_RD_CLR;
    } else {
        tmp &= ~(BIT_LATCH_EN | BIT_ANY_RD_CLR);
    }

    return writeRegister(INT_PIN_CFG, tmp);
}

/**
 *  @brief      Turn specific sensors on/off.
 *  @e sensors can contain a combination of the following flags:
 *  \n INV_X_GYRO, INV_Y_GYRO, INV_Z_GYRO
 *  \n INV_XYZ_GYRO
 *  \n INV_XYZ_ACCEL
 *  \n INV_XYZ_COMPASS
 *  @param[in]  sensors    Mask of sensors to wake.
 *  @return     0 if successful.
 */
bool MPU9150::setSensors(uint8_t sensors) {
//    uint8_t data;
//    uint8_t user_ctrl;
//
////if (sensors == st.chip_cfg.sensors)
////  return 0;
//    if (sensors & INV_XYZ_GYRO)
//        data = PWR_MGMT_1_BIT_CLK_PLL;
//    else if (sensors)
//        data = 0;
//    else
//        data = BIT_SLEEP;
//
//    if (writeRegister(PWR_MGMT_1, data) == false) return false;
//
//// st.chip_cfg.clk_src = data & ~BIT_SLEEP;
//
//    data = 0;
//    if (!(sensors & INV_X_GYRO)) data |= BIT_STBY_XG;
//    if (!(sensors & INV_Y_GYRO)) data |= BIT_STBY_YG;
//    if (!(sensors & INV_Z_GYRO)) data |= BIT_STBY_ZG;
//    if (!(sensors & INV_XYZ_ACCEL)) data |= BIT_STBY_XYZA;
//
//    if (writeRegister(PWR_MGMT_2, data) == false) return false;
//
//    if (sensors && (sensors != INV_XYZ_ACCEL)) {
//        /* Latched interrupts only used in LP accel mode. */
//        //set_int_latched(0);
//    }
//    if (readRegister(USER_CTRL, user_ctrl) == false) return false;
//    /* Handle AKM power management. */
//    if (sensors & INV_XYZ_COMPASS) {
//        data = hal::AK8975::SINGLE_MEASUREMENT;
//        user_ctrl |= BIT_AUX_IF_EN;
//    } else {
//        data = hal::AK8975::POWER_DOWN;
//        user_ctrl &= ~BIT_AUX_IF_EN;
//    }
//    if (writeRegister(I2C_SLV1_DO, data) == false) return false;
//    /* Enable/disable I2C master mode. */
//    if (writeRegister(USER_CTRL, user_ctrl) == false) return false;

    delay_ms(15);
    return true;
}
/**
 * \brief This function read data from MPU9150 and update data in accelerometer, gyroscope, and thermometer
 */
bool MPU9150::updateFromFIFO(void) {
    uint8_t buffer[20];
    uint_fast8_t index = 0;

    //read data from mpu
    if (readFifoStream(buffer, fifoLength) == true) {
        //update data from specify device
        if (streamsInFifo & ACCEL_FIFO_EN) {
            accelerometer.update(buffer);
            index = 6;
        }
        if (streamsInFifo & TEMP_FIFO_EN) {
            thermometer.update(&buffer[index]);
            index += 2;
        }
        if (streamsInFifo & GYRO_FIFO_EN) {
            gyroscope.update(&buffer[index]);
            index += 6;
        }
        if (streamsInFifo & SLV0_FIFO_EN) {
            magnetometer.update(&buffer[index]);
        }
        return true;
    }
    return false;
}
/**
 * \brief This function read data from MPU9150 and update data in accelerometer, gyroscope, and thermometer
 */
bool MPU9150::updateFromRaw(void) {
    if (isNewDataAvailable() == true) {
        uint8_t buffer[14];
        //read data from mpu
        if (readRegisters(ACCEL_XOUT_H, buffer, sizeof(buffer)) == true) {
            //update data from specify device
            accelerometer.update(buffer);
            thermometer.update(&buffer[6]);
            gyroscope.update(&buffer[8]);
            return true;
        }
    }
    return false;
}
/**
 *  @brief Get one unparsed packet from the FIFO.
 *  This function should be used if the packet is to be parsed elsewhere.
 *  @param[in]  length  Length of one FIFO packet.
 *  @param[in]  data    FIFO packet.
 *
 *  @retval true if fifo read successful.
 */
bool MPU9150::readFifoStream(uint8_t *data, uint16_t length) {
    if (getNumberOfSamplesInFIFO() < length) {
        return false;
    }

    return readRegisters(FIFO_R_W, data, length);
}

/**
 * \brief This function reset FIFO.
 *
 * Disable interrupts, clear FIFO buffer and enable interrupts.
 * @return Return true if completed successfully.
 */
bool MPU9150::resetFIFO() {
    uint8_t data[3];
//remember register settings
// if (readRegister(MPU9150::USER_CTRL, data[0]) == false) return false;
// if (readRegister(MPU9150::INT_ENABLE, data[1]) == false) return false;
    if (readRegister(MPU9150::FIFO_EN, data[2]) == false) return false;
//clear registers
    if (writeRegister(MPU9150::INT_ENABLE, static_cast<uint8_t>(0x00)) == false) return false;
    if (writeRegister(MPU9150::FIFO_EN, static_cast<uint8_t>(0x00)) == false) return false;
    if (writeRegister(MPU9150::USER_CTRL, static_cast<uint8_t>(0x00)) == false) return false;

//if dmp is on
#ifdef MPU9150_USE_DMP
    if (dmp.isOn() == true) {
        if (writeRegister(MPU9150::USER_CTRL, BIT_FIFO_RST | BIT_DMP_RST) == false) {
            return false;
        }
        delay_ms(50);
        if (writeRegister(MPU9150::USER_CTRL, BIT_DMP_EN | BIT_FIFO_EN | BIT_AUX_IF_EN) == false) {
            return false;
        }
        if (writeRegister(MPU9150::INT_ENABLE, BIT_DMP_INT_EN) == false) {
            return false;
        }
        if (clearRegister(MPU9150::FIFO_EN) == false) return false;
    } else
#endif
    {
        if (writeRegister(MPU9150::USER_CTRL, static_cast<uint8_t>(BIT_FIFO_RST)) == false) {
            return false;
        }

        if (writeRegister(MPU9150::USER_CTRL, static_cast<uint8_t>(BIT_FIFO_EN | BIT_AUX_IF_EN)) == false) {
            return false;
        }
        delay_ms(50);
        if (writeRegister(MPU9150::INT_ENABLE, static_cast<uint8_t>(BIT_DATA_RDY_EN)) == false) {
            return false;
        }
        if (writeRegister(MPU9150::FIFO_EN, data[2]) == false) {
            return false;
        }
    }
//delay_ms(50);
//if (writeRegister(MPU9150::USER_CTRL, data[0]) == false) return false;
//if (writeRegister(MPU9150::INT_ENABLE, data[1]) == false) return false;
//if (writeRegister(MPU9150::FIFO_EN, data[2]) == false) return false;

    return true;
}

/**
 * \brief This function sets sampling frequency
 * \param   freq sampling frequency in Hz to set.
 *
 * \return  actual sampling rate
 */
uint16_t MPU9150::setSamplingFrequency(uint16_t freq) {
    uint8_t smplrtDiv;
    uint16_t gyroscopeOutputRate;

    switch (getDLPF_CFG()) {
    case UNKNOWN:
        return 0;
    case DLPF_CFG_0:
    case DLPF_CFG_7:
        gyroscopeOutputRate = 8000;
        break;
    default:
        gyroscopeOutputRate = 1000;
    }

    if (freq > gyroscopeOutputRate) {   //maximum frequency is 8kHz
        freq = gyroscopeOutputRate;
    }
//calculate divider
    smplrtDiv = (gyroscopeOutputRate / freq) - 1;
//set sampling divider
    writeRegister(SMPLRT_DIV, smplrtDiv);
//return actual sampling frequently
    return gyroscopeOutputRate / (smplrtDiv + 1);
}

/**
 * \brief This function return actual sampling frequently.
 * @return actual sampling frequently in Hz.
 */
uint16_t MPU9150::getSamplingFrequency() {
    uint8_t smplrtDiv;

    if (readRegister(SMPLRT_DIV, smplrtDiv) == false) {
        return 0;
    }

    uint16_t gyroscopeOutputRate;

    //read gyroscope data rate
    switch (getDLPF_CFG()) {
    case UNKNOWN:   //if reading error
        return 0;   //return 0 -> indication of error
    case DLPF_CFG_0:
    case DLPF_CFG_7:
        gyroscopeOutputRate = 8000;
        break;
    default:
        gyroscopeOutputRate = 1000;
    }

    return gyroscopeOutputRate / (smplrtDiv + 1); //calculate actual sampling freq
}
/**
 * \brief This function return number of samples storage in FIFO buffer.
 * @return Number of samples in FIFO.
 */
uint16_t MPU9150::getNumberOfSamplesInFIFO(void) {
    uint8_t buff[2];
    uint16_t size;

    if (readRegisters(FIFO_COUNTH, buff, sizeof(buff)) == false) {
        return 0;
    }
    size = buff[0] << 8 | buff[1];

    return size;
}
/**\brief This function changing Digital Low Pass Filter Bandwidth.
 * Attention if DLPF is changed, the sampling frequently may be change. Please check actual sampling frequency when DLPF was change.
 *
 * @param dlpf low pas filter option to set
 * @retval true if new filter option was set.
 * @retval false if an error occurred.
 */
bool MPU9150::setDLPF(DLPF_CFG dlpf) {
    uint8_t config;
    if (dlpf == UNKNOWN) return false;
    if (readRegister(CONFIG, config) == false) {
        return false;
    }
    config = (config & ~0x07) | dlpf;
    return writeRegister(CONFIG, config);
}
/**
 * \brief This function return Digital Low Pass Filter Configuration
 * @return configuration
 */
MPU9150::DLPF_CFG MPU9150::getDLPF_CFG(void) {
    uint8_t config;
    if (readRegister(CONFIG, config) == false) {
        return UNKNOWN;
    }
    return (DLPF_CFG) (config & 0x07);
}
/**
 * \brief This function checks available samples.
 * @retval true if new data is available
 * @retval false if no new data available or an error occurred
 */
bool MPU9150::isNewDataAvailable(void) {
    uint8_t buffer;

    if (readRegister(INT_STATUS, buffer) == true) {

        if (buffer & DATA_READY_INT) {
            //return update();
            return true;
        }
    }
    return false;
}
/**
 *  @brief      Write to the DMP memory.
 *  This function prevents I2C writes past the bank boundaries.
 *  @param[in]  memAddr    Memory location (bank << 8 | start address)
 *  @param[in]  data        Bytes to write to memory.
 *  @param[in]  length      Number of bytes to write.
 *
 *  @retval    true if successful.
 *  @retval    false if an error occurred.
 */
bool MPU9150::writeMem(uint16_t memAddr, const uint8_t *data, uint16_t length) {
    uint8_t tmp[2];

//if empty pointer return false
    if (!data) return false;
//parse address
    tmp[0] = (uint8_t)(memAddr >> 8);
    tmp[1] = (uint8_t)(memAddr & 0xFF);

// Check bank boundaries.
    if (tmp[1] + length > BANK_SIZE) return false;

    if (writeRegisters(BANK_SEL, tmp, sizeof(tmp)) == false) return false;

    return writeRegisters(MEM_R_W, data, length);
}

/**
 *  @brief      Read from the DMP memory.
 *  This function prevents I2C reads past the bank boundaries.
 *  @param[in]  memAddr    Memory location (bank << 8 | start address)
 *  @param[out] data       Bytes read from memory.
 *  @param[in]  length     Number of bytes to read.
 *  @retval    true if successful.
 *  @retval    false if an error occurred.
 */
bool MPU9150::readMem(uint16_t memAddr, uint8_t *data, uint16_t length) {
    uint8_t tmp[2];

//if empty pointer return false
    if (!data) {
        return false;
    }
//parse address
    tmp[0] = (uint8_t)(memAddr >> 8);
    tmp[1] = (uint8_t)(memAddr & 0xFF);

// Check bank boundaries. //
    if (tmp[1] + length > BANK_SIZE) {
        return false;
    }

    if (writeRegisters(BANK_SEL, tmp, 2) == false) {
        return false;
    }
    return readRegisters(MEM_R_W, data, length);
}
/**
 * @brief Load and verify DMP image.
 *
 * @param[in] startAddr Starting address of DMP code memory.
 * @param[in] firmware DMP code.
 * @param[in] length Length of DMP image.
 *
 * @return true if successful.
 */
bool MPU9150::loadFirmware(uint16_t startAddr, const uint8_t *firmware, uint16_t length) {
    /* Must divide evenly into BANK_SIZE to avoid bank crossings. */
    constexpr uint8_t loadChunkSize = 16;
    static bool isLoaded = false;
    uint16_t thisWrite;
    uint8_t tmp[2];

// DMP should only be loaded once.
    if (isLoaded) {
        return false;
    }
//if firmware pointer is null
    if (firmware == NULL) {
        return false;
    }

    uint8_t progBuffer[loadChunkSize];
    for (uint16_t ii = 0; ii < length; ii += thisWrite) {
        thisWrite = min(loadChunkSize, length - ii);

        if (writeMem(ii, &firmware[ii], thisWrite) == false) {
#ifdef MPU_DEBUG
            Debug::debug() << "firmware write failed.\n\r";
#endif
            return false;
        }

        if (readMem(ii, progBuffer, thisWrite) == false) {
            return false;
        }

        if (memcmp(&firmware[ii], progBuffer, thisWrite) != 0) {
#ifdef MPU_DEBUG
            Debug::debug() << "Firmware compare failed addr: " << ii << "/n/r";
#endif
            return false;
        }
    }
    /* Set program start address. */
    tmp[0] = startAddr >> 8;
    tmp[1] = startAddr & 0xFF;
    if (writeRegisters(PRGM_START_H, tmp, 2) == false) {
        return false;
    }
    isLoaded = true;
#ifdef MPU_DEBUG
    Debug::debug() << "Firmware loaded\n\r";
#endif
    return true;
}

void MPU9150::interruptFunction(void) {
    uint8_t intStatus;
//read interrupt source
    if (readRegister(INT_STATUS, intStatus) == false) {
        //todo debug notice
        return;
    }
    if (intStatus & DATA_READY_INT) {
        //update();
        //todo emit signal
    }
    if (intStatus & FIFO_OVERFLOW_INT) {
        //todo emit signal
    }
    if (intStatus & I2C_MST_INT) {
        //todo emit signal
    }
}
#ifdef MPU9150_USE_DMP
/**
 * \brief This function checks if new part of data form DMP are available
 * @retval true if new data is available
 * @retval false if no new data available or an error occurred
 */
bool MPU9150::isNewDataFromDMP() {
    uint8_t buffer[2];

    if (readRegisters(INT_STATUS, buffer, 2) == false) {
        return false;
    }
    if (buffer[0] & DMP_INT) {
        return true;
    }
    return false;
}

bool MPU9150::setDMPstate(bool enabled) {
    static uint8_t fifoStat;
//todo
    if (dmp.isOn() == enabled) return true;

    if (enabled == true) {
        if (dmp.isImageLoaded() == false) return false;
        /* Disable data ready interrupt. */
        if (interruptDisable(DATA_READY_INT) == false) return false;
        /* Disable bypass mode. */
        if (setBypasMode(false) == false) return false;
        /* Keep constant sample rate, FIFO rate controlled by DMP. */
        setSamplingFrequency(dmp.getSampleRate());
        //remember fifo configuration
        if (readRegister(FIFO_EN, fifoStat) == false) return false;
        /* Remove FIFO elements. */
        if (clearRegister(FIFO_EN) == false) return false;
        /* Enable DMP interrupt. */
        if (interruptEnable(DMP_INT) == false) return false;
        if (dmp.on() == false) return false;
    } else {
        /* Disable DMP interrupt. */
        if (interruptDisable(DMP_INT) == false) return false;
        if (dmp.off() == false) return false;
        /* Restore FIFO settings. */
        if (writeRegister(FIFO_EN, fifoStat) == false) return false;
        /* Enable data ready interrupt. */
        if (interruptEnable(DATA_READY_INT) == false) return false;
    }
    return resetFIFO();
//return true;
}
#endif
