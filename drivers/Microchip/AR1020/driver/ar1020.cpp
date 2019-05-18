/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 20-04-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#include "ar1020.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;

AR1020::Position AR1020::readPosition() {
    std::array<uint8_t, 5> data;
    auto status = i2c.read(data);
    if (status != Error::None) {
        diagChannel << MICROHAL_ERROR << microhal::I2C::toString(status) << endl;
    }
    bool pressed = data[0] & 1;
    uint16_t x = data[1] | (data[2] << 7);
    uint16_t y = data[3] | (data[4] << 7);
    return {pressed, x, y};
}

bool AR1020::sendCMD(Command::Base &cmd, uint8_t *data, size_t dataSize) {
    if (i2cbase.writeRead(0x9A, cmd.getDataPtr(), cmd.size_bytes(), data, dataSize) == Error::None) {
        return true;
    }
    return false;
}

bool AR1020::sendCMD(Command::Base &cmd) {
    union {
        uint8_t rxBuff[sizeof(Command::Response)];
        Command::Response response;
    };
    if (sendCMD(cmd, rxBuff, sizeof(rxBuff))) {
        return response.command == cmd.command && response.response == Command::Response::Status::Success;
    }
    return false;
}

bool AR1020::getVersion(uint16_t &version) {
    disable();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    union {
        uint8_t rxBuffer[sizeof(Command::Response) + 3];
        struct {
            Command::Response cmdStatus;
            uint8_t version_hi;
            uint8_t version_lo;
            uint8_t version_type;
        } response;
    };

    Command::GetVersion cmd;
    auto status = sendCMD(cmd, rxBuffer, sizeof(rxBuffer));
    version = (response.version_hi << 8) | response.version_lo;
    enable();
    return status;
}

bool AR1020::calibrationStart() {
    Command::Calibrate cal;
    Command::Response response;
    return sendCMD(cal, (uint8_t *)&response, sizeof(response));
}

bool AR1020::eepromRead() {
    Command::EepromRead cmd(0x00, 10);
    std::array<uint8_t, sizeof(Command::Response) + 10> buff;
    sendCMD(cmd, buff.data(), buff.size());
    for (auto value : buff) {
        diagChannel << Debug << toHex(value) << endl;
    }
    return true;
}

bool AR1020::callibrationWaitForTouch() {
    Command::Response expectedResponse;
    expectedResponse.command = Command::CommandType::CalibrateMode;
    expectedResponse.header = 0x55;
    expectedResponse.dataSize = 2;
    expectedResponse.response = Command::Response::Status::Success;

    union {
        uint8_t rxBuff[sizeof(Command::Response)];
        Command::Response response;
    };

    i2c.read(rxBuff);

    return response == expectedResponse;
}

bool AR1020::callibrationStop() {
    // wait for EEPROM write
    std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    return true;
}
