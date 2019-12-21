/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 21-02-2014
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

#ifndef SHT21_H_
#define SHT21_H_

#include "I2CDevice/I2CDevice.h"
#include "microhal.h"

/**
 * \addtogroup Devices
 * @{
 * @class SHT21
 * @}
 */
class SHT21 : public microhal::I2CDevice {
 public:
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;

 private:
    /**
     * Registers flags
     */
    enum Flags {
        USER_REGISTER_ENABLE_ON_CHIP_HEATER = 0x04,  //!< USER_REGISTER_ENABLE_ON_CHIP_HEATER
    };
    /**
     * SHT21 registers
     */
    static constexpr auto TRIGGER_T_MEASURE_HOLD =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0xE3>{});  //!< TRIGGER_T_MEASURE_HOLD
    static constexpr auto TRIGGER_RH_MEASURE_HOLD =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0xE5>{});  //!< TRIGGER_RH_MEASURE_HOLD
    static constexpr auto TRIGGER_T_MEASURE_NO_HOLD =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0xF3>{});  //!< TRIGGER_T_MEASURE_NO_HOLD
    static constexpr auto TRIGGER_RH_MEASURE_NO_HOLD =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0xF5>{});  //!< TRIGGER_RH_MEASURE_NO_HOLD
    static constexpr auto WRITE_USER_REGISTER_CMD =
        microhal::makeRegister<uint8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0xE6>{});  //!< WRITE_USER_REGISTER_CMD
    static constexpr auto READ_USER_REGISTER_CMD =
        microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0xE7>{});  //!< READ_USER_REGISTER_CMD
    static constexpr auto SOFT_RESET_CMD = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0xFE>{});  //!< SOFT_RESET_CMD

    /**
     * Possible I2C address
     */
    enum PossibleI2CAddress {
        DEFAULT_ADDRESS = 0x80  //!< DEFAULT_ADDRESS
    };

 public:
    /** @brief Possible temperature and humidity resolution
     *
     */
    typedef enum {
        RESOLUTION_12_14 = 0x00,  //!< RESOLUTION_12_14
        RESOLUTION_8_12 = 0x01,   //!< RESOLUTION_8_12
        RESOLUTION_10_13 = 0x80,  //!< RESOLUTION_10_13
        RESOLUTION_11_11 = 0x81,  //!< RESOLUTION_11_11
        UNKNOWN
    } Resolution;
    //---------------------------------------- constructors ---------------------------------------
    inline SHT21(microhal::I2C &i2c) noexcept : I2CDevice(i2c, DEFAULT_ADDRESS) {}
    //------------------------------------------ functions ----------------------------------------
    bool reset() noexcept;

    Error startTemperatureConversion() noexcept { return write(TRIGGER_T_MEASURE_HOLD.getAddress()); }

    Error readTemperature(float &temperature) noexcept {
        uint8_t temp[1];

        auto status = read(temp);
        temperature = (float)temp[0] * 0.002681274;
        temperature -= 46.85;

        return status;
    }

    /** @brief This function read temperature in celsius degree from SHT21 device. I2C data buss is lock until measurement is doing.
     *
     * @param[out] temperature in celcius degree
     * @retval true if temperature read correctly.
     * @retval false if an error occurred.
     */
    Error getTemperature(float &temperature) noexcept {
        uint16_t temp;
        auto status = readRegister(TRIGGER_T_MEASURE_HOLD, temp);
        temperature = (float)temp * 0.002681274;
        temperature -= 46.85;
        return status;
    }
    /** This function read humidity from SHT21 device. I2C data buss is lock until measurement is doing.
     *
     * @param[out] humidity
     * @retval true if humidity read successful.
     * @retval false if an error occurred.
     */
    Error getHumidity(float &humidity) noexcept {
        uint16_t tmp;
        auto status = readRegister(TRIGGER_RH_MEASURE_HOLD, tmp);
        humidity = (float)tmp * 0.001907349;
        humidity -= 6;
        return status;
    }

    Error heaterEnable() noexcept;
    Error heaterDisable() noexcept;

    Error setResolution(Resolution resolution) noexcept;
    Resolution getResolution() noexcept;
};
#endif /* SHT21_H_ */
