/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 31-12-2016
 * last modification: 31-12-2016
 *
 * @copyright Copyright (c) 2016, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_ONEWIRE_H_
#define _MICROHAL_ONEWIRE_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "diagnostic/diagnostic.h"
#include "serialPort.h"

// NOTICE: This driver use SerialPort do communicate with 1-Wire device. This driver was written based on:
// https://www.maximintegrated.com/en/app-notes/index.mvp/id/214

namespace microhal {
/* **************************************************************************************************************************************************
 * CLASS
 */
class OneWire {
 public:
    // using Rom = uint64_t;
    class Rom {
     public:
        constexpr Rom(uint64_t rom) noexcept : rom(rom) {}
        constexpr Rom() noexcept : rom(0) {}

        constexpr bool operator==(Rom rom) const noexcept { return rom == this->rom; }
        uint8_t familyCode() const { return rom & 0xFF; }

        constexpr uint64_t getRaw() const { return rom; }
        Rom &operator=(const Rom &rhs) {
            rom = rhs.rom;
            return *this;
        }

     private:
        uint64_t rom;

        constexpr void operator|=(uint64_t data) noexcept { rom |= data; }

        friend OneWire;
    };

    enum class Command : uint8_t {
        MatchROM = 0x55,
        SearchROM = 0xF0,
        SkipROM = 0xCC,
        AlarmSearch = 0xEC,
        ReadRom = 0x33,
    };

    constexpr OneWire(SerialPort &serial) noexcept : serial(serial) {}

    bool write(Command command) noexcept { return write(static_cast<uint8_t>(command)); }

    bool write(uint8_t data) noexcept {
        uint8_t tmp[8];
        for (uint8_t bitPos = 0; bitPos < 8; bitPos++) {
            if (data & (1 << (bitPos))) {
                tmp[bitPos] = 0xFF;
            } else {
                tmp[bitPos] = 0x00;
            }
        }
        if (serial.write(reinterpret_cast<char *>(tmp), sizeof(tmp)) == sizeof(tmp)) {
            return true;
        }
        return false;
    }

    bool write(const uint8_t *data, size_t size) noexcept {
        for (size_t i = 0; i < size; i++) {
            if (write(data[i]) == false) return false;
        }
        return true;
    }

    bool read(uint8_t *data, size_t length) const noexcept;

    bool sendResetPulse() const;

    bool searchRom(Rom *deviceRom);

    bool readRom(Rom *deviceRom) {
        if (sendResetPulse()) {
            write(Command::ReadRom);
            uint64_t tmp;
            read(reinterpret_cast<uint8_t *>(&tmp), sizeof(tmp));
            // tmp = convertEndianness();
            // check CRC
            *deviceRom = Rom(tmp);

            return true;
        }
        return false;
    }

 private:
    SerialPort &serial;

    bool writeBit(uint8_t bit);
    bool readBit(uint8_t &bit, uint8_t &complementaryBit);
};

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel,
                                                                  const OneWire::Rom &rom) {
    return logChannel << microhal::diagnostic::toHex(rom.getRaw());
}

}  // namespace microhal

#endif  // _MICROHAL_ONEWIRE_H_
