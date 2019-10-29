/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Paweł Okas
 * created on: 07-01-2017
 * last modification: 07-01-2017
 *
 * @copyright Copyright (c) 2017, Paweł Okas
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "oneWire.h"

namespace microhal {
using namespace diagnostic;

bool OneWire::read(uint8_t *data, size_t length) const noexcept {
    if (serial.waitForWriteFinish(std::chrono::milliseconds{100})) {
        serial.clear(SerialPort::Direction::Input);
        const uint8_t write[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        static_assert(sizeof(write) == 8, "Internal microhal error.");
        for (size_t byte = 0; byte < length; byte++) {
            serial.write(reinterpret_cast<const char *>(write), sizeof(write));
            uint8_t read[8];
            serial.read(reinterpret_cast<char *>(read), sizeof(read), std::chrono::milliseconds{100});
            uint8_t tmp = 0;
            for (uint8_t bitPos = 0; bitPos < 8; bitPos++) {
                if (read[bitPos] == 0xFF) {
                    tmp |= 1 << (bitPos);
                }
            }
            data[byte] = tmp;
        }
        return true;
    }
    return false;
}

bool OneWire::searchRom(Rom *deviceRom) {
    if (sendResetPulse()) {
        if (write(Command::SearchROM)) {
            Rom devRom = 0;
            Rom lastFound = 0;
            uint_fast8_t lastTransition = 0xFF;
            uint_fast8_t transitionTmp = 0xFF;
            bool firstDeviceSearch = true;
            for (uint_fast8_t position = 0; position < 64; position++) {
                uint8_t bit, complementaryBit;
                if (readBit(bit, complementaryBit)) {
                    if (bit == 1 && complementaryBit == 1) {
                        return false;  // no device on bus, this may happen when someone will disconnect device from bus
                    }

                    diagChannel << lock << Debug << "Found bit: " << bit << " complementary: " << complementaryBit << endl << unlock;

                    if (bit != complementaryBit) {
                        writeBit(bit);
                        if (bit) devRom |= uint64_t{1} << position;
                    } else {
                        if (firstDeviceSearch) {
                            // always go to 0
                            transitionTmp = position;
                            writeBit(0);
                        } else {
                            if (lastTransition > position) {
                                transitionTmp = position;
                                if (lastFound.getRaw() & uint64_t{1} << position) {
                                    writeBit(1);
                                    devRom |= 1 << position;
                                } else {
                                    writeBit(0);
                                }
                            } else if (lastTransition == position) {
                                writeBit(1);
                                devRom |= uint64_t{1} << position;
                            } else {
                                transitionTmp = position;
                                writeBit(0);
                            }
                        }
                    }
                }
            }
            firstDeviceSearch = false;
            lastTransition = transitionTmp;
            lastFound = devRom;
            *deviceRom = devRom;
            return true;
        }
    }
    return false;
}

bool OneWire::writeBit(uint8_t bit) {
    unsigned char write[] = {0};
    if (bit) write[0] = 0xFF;
    if (serial.putChar(write[0])) {
        char tmp;
        return serial.read(&tmp, sizeof(tmp), std::chrono::milliseconds{1});
    }
    return false;
}

bool OneWire::readBit(uint8_t &bit, uint8_t &complementaryBit) {
    if (serial.waitForWriteFinish(std::chrono::milliseconds{100})) {
        serial.clear(SerialPort::Direction::Input);
        const uint8_t write[] = {0xFF, 0xFF};
        static_assert(sizeof(write) == 2, "");
        serial.write(reinterpret_cast<const char *>(write), sizeof(write));
        uint8_t read[2];
        serial.read(reinterpret_cast<char *>(read), sizeof(read), std::chrono::milliseconds{10});
        diagChannel << lock << Debug << "bit: " << read[0] << " complementary: " << read[1] << endl << unlock;
        bit = read[0] == 0xFF ? 1 : 0;
        complementaryBit = read[1] == 0xFF ? 1 : 0;
        return true;
    }
    return false;
}

bool OneWire::sendResetPulse() const {
    if (serial.waitForWriteFinish(std::chrono::milliseconds::max())) {
        serial.clear(SerialPort::Direction::Input);
        serial.setBaudRate(SerialPort::Baud9600);
        if (serial.putChar(0xF0)) {
            char response;
            if (serial.read(&response, 1, std::chrono::milliseconds{3})) {
                if (static_cast<uint8_t>(response) != 0xF0U) {
                    serial.setBaudRate(SerialPort::Baud115200);
                    return true;
                } else {
                    diagChannel << lock << MICROHAL_ERROR << "1-wire device not found" << unlock;
                }
            } else {
                diagChannel << lock << MICROHAL_ERROR << "Unable to read data from serial port." << unlock;
            }
        }
    }
    return false;
}

}  // namespace microhal
