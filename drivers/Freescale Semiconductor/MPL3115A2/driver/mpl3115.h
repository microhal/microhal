/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 18-12-2014
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

#ifndef MPL3115_H_
#define MPL3115_H_

#include <optional>
#include <utility>

#include "I2CDevice/I2CDevice.h"
#include "microhal.h"
#include "units/pressure.h"
#include "units/temperature.h"

class MPL3115 : protected microhal::I2CDevice {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;
    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    static constexpr auto STATUS = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x00>{});
    static constexpr auto OUT_P_MSB = microhal::makeRegister<microhal::uint24_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0x01>{});
    // OUT_P_CSB
    // OUT_P_LSB

    static constexpr auto OUT_T_MSB = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0x04>{});
    // OUT_T_LSB
    static constexpr auto DR_STATUS = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x06>{});
    static constexpr auto OUT_P_DELTA_MSB = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x07>{});
    static constexpr auto OUT_P_DELTA_CSB = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x08>{});
    static constexpr auto OUT_P_DELTA_LSB = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x09>{});

    static constexpr auto OUT_T_DELTA_MSB = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0x0A>{});
    // OUT_T_DELTA_LSB
    static constexpr auto WHO_AM_I = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x0C>{});
    static constexpr auto F_STATUS = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x0D>{});
    static constexpr auto F_DATA = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x0E>{});
    static constexpr auto F_SETUP = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0F>{});
    static constexpr auto TIME_DLY = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x10>{});
    static constexpr auto SYSMOD = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x11>{});
    static constexpr auto INT_SOURCE = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x12>{});
    static constexpr auto PT_DATA_CFG = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x13>{});
    static constexpr auto BAR_IN_MSB = microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x14>{});
    // BAR_IN_LSB
    static constexpr auto P_TGT_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x16>{});
    // P_TGT_LSB
    static constexpr auto T_TGT = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x18>{});
    static constexpr auto P_WND_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x19>{});
    // P_WND_LSB
    static constexpr auto T_WND = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1B>{});
    static constexpr auto P_MIN_MSB = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1C>{});
    static constexpr auto P_MIN_CSB = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1D>{});
    static constexpr auto P_MIN_LSB = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1E>{});
    static constexpr auto T_MIN_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x1F>{});
    // T_MIN_LSB
    static constexpr auto P_MAX_MSB = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x21>{});
    static constexpr auto P_MAX_CSB = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x22>{});
    static constexpr auto P_MAX_LSB = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x23>{});
    static constexpr auto T_MAX_MSB = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x24>{});
    // T_MAX_LSB
    static constexpr auto CTRL_REG1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x26>{});
    static constexpr auto CTRL_REG2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x27>{});
    static constexpr auto CTRL_REG3 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x28>{});
    static constexpr auto CTRL_REG4 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x29>{});
    static constexpr auto CTRL_REG5 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2A>{});
    static constexpr auto OFF_P = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2B>{});
    static constexpr auto OFF_T = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2C>{});
    static constexpr auto OFF_H = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2D>{});

 public:
    /**
     *  Value of fixed registers.
     */
    enum ConstRegisterValues : uint8_t {
        WHO_AM_I_VALUE = 0xC4,  //!< ID VALUE
    };

    enum class Mode : uint8_t { Standby = 0x00, Active = 0x01 };

    enum class SensingMode : uint8_t { Barometer = 0b0000'0000, Altimeter = 0b1000'0000 };

    enum class InterruptSource : uint8_t {
        DataReady,
        FIFO,
        AltitudePressureAlert,
        TemperatureAlert,
        AltitudePresureThreshold,
        TemperatureThreshold,
        DeltaTemperature,
        DeltaPressure
    };

    enum OutputSampleRate : uint8_t {
        OSR_6ms = 0b0000'0000,
        OSR_10ms = 0b0000'1000,
        OSR_18ms = 0b0001'0000,
        OSR_34ms = 0b0001'1000,
        OSR_66ms = 0b0010'0000,
        OSR_130ms = 0b0010'1000,
        OSR_258ms = 0b0011'0000,
        OSR_512ms = 0b0011'1000
    };

    enum class Event { OnNewTemperature = 0x01, OnNewPressureOrAltitude = 0x02, DataChange = 0x04 };

    constexpr explicit MPL3115(microhal::I2C &i2c) noexcept : I2CDevice(i2c, 0xC0) {}
    ~MPL3115() {}

    bool init() {
        if (auto who = whoAmI()) {
            if (*who == WHO_AM_I_VALUE) return true;
        }
        return false;
    }

    std::optional<uint8_t> whoAmI() noexcept {
        std::optional<uint8_t> id;
        uint8_t whoAmI;
        if (readRegister(WHO_AM_I, whoAmI) == Error::None) {
            id = whoAmI;
        }

        return id;
    }

    std::optional<Mode> readMode() {
        std::optional<Mode> mode;
        uint8_t sysmode;
        if (readRegister(SYSMOD, sysmode) == Error::None) {
            mode = static_cast<Mode>(sysmode);
        }
        return mode;
    }

    std::optional<InterruptSource> readInterruptSource() {
        std::optional<InterruptSource> intSource;
        uint8_t source;
        if (readRegister(INT_SOURCE, source) == Error::None) {
            intSource = static_cast<InterruptSource>(source);
        }
        return intSource;
    }

    Error setOSR(OutputSampleRate osr) { return modifyBitsInRegister(CTRL_REG1, osr, 0x0C); }

    Error setSensingMode(SensingMode mode) { return modifyBitsInRegister(CTRL_REG1, static_cast<uint8_t>(mode), static_cast<uint8_t>(0b1000'0000)); }

    Error setMode(Mode mode) { return setBitsInRegister(CTRL_REG1, static_cast<uint8_t>(mode)); }

    Error enableEvent(Event event) { return setBitsInRegister(PT_DATA_CFG, static_cast<uint8_t>(event)); }

    bool isPressureOrAltitudeReady() {
        uint8_t status;
        if (readRegister(STATUS, status) == Error::None) {
            if (status & 0x08) {
                return true;
            }
        }
        return false;
    }

    auto read() {
        using Pressure = microhal::Pressure<float>;
        using Temperature = microhal::Temperature;
        using DataType = std::pair<Pressure, Temperature>;

        std::optional<DataType> data;
        int16_t temp;
        microhal::uint24_t pressure;
        std::tuple<microhal::uint24_t, int16_t> regsVal;
        if (readMultipleRegisters(regsVal, OUT_P_MSB, OUT_T_MSB) == Error::None) {
            data = std::make_pair(microhal::Pressure(static_cast<float>(pressure) / 64.0f),
                                  microhal::Temperature::fromCelcius(static_cast<float>(temp) / 256.0f));
        }
        return data;
    }
    //	template <microhal::diagnostic::LogLevel level>
    //	void registerDump(microhal::diagnostic::Diagnostic<level> &log) {
    //		uint8_t regVal;
    //
    //		log << microhal::diagnostic::Warning <<	"----------- MPL3110 register dump -----------" <<  microhal::diagnostic::endl;
    //
    //		for (uint8_t reg = STATUS; reg <= OFF_H; reg++) {
    //			log << microhal::diagnostic::Warning << microhal::diagnostic::toHex(reg) << " ";
    //
    //			if (readRegister(reg, regVal)) {
    //				log << microhal::diagnostic::Warning << regVal << microhal::diagnostic::endl;
    //			} else {
    //				log << microhal::diagnostic::Warning << "Error while reading." << microhal::diagnostic::endl;
    //			}
    //		}
    //
    //		log << microhal::diagnostic::Warning <<	"----------- End of register dump -----------" <<  microhal::diagnostic::endl;
    //	}
};

constexpr MPL3115::Event operator|(MPL3115::Event a, MPL3115::Event b) {
    return static_cast<MPL3115::Event>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

#endif /* MPL3115_H_ */
