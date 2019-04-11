/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "at24mac.h"
#include "bsp.h"
#include "diagnostic/diagnostic.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

using namespace microhal;
using namespace diagnostic;

AT24MAC mac(bsp::i2c, 0xA0);

doctest::String toString(const AT24MAC::Error& error) {
    auto string = AT24MAC::toString(error);
    return {string.data(), string.size()};
}

TEST_CASE("Read MAC") {
    uint64_t eui = 0;
    // Check if we can read eui
    CHECK(mac.readEUI(eui) == AT24MAC::Error::None);

    const uint64_t AtmelMACMask = 0xFCC2'3D00'0000'0000;
    CHECK((eui & 0xFFFF'FF00'0000'0000) == AtmelMACMask);
}

TEST_CASE("Read serial number") {
    AT24MAC::SerialNumber serial;
    CHECK(mac.readSerialNumber(serial) == AT24MAC::Error::None);
}

TEST_CASE("Memory read") {
    uint8_t data[256];
    CHECK(mac.read(0x00, data) == AT24MAC::Error::None);

    uint8_t byte;
    CHECK(mac.readByte(0x00, byte) == AT24MAC::Error::None);
}

TEST_CASE("Memory write") {
    CHECK(mac.writeByte(0x00, 'a') == AT24MAC::Error::None);
    mac.writeWait();
    uint8_t pageData[16] = {0};
    CHECK(mac.writePage(0x00, pageData) == AT24MAC::Error::None);
    mac.writeWait();

    // try to write page but not from page begin
    CHECK(mac.writePage(0x01, pageData) == AT24MAC::Error::Addres);

    // try write to may data so page may overflow
    uint8_t overflowPage[17] = {0};
    CHECK(mac.writePage(0x01, overflowPage) == AT24MAC::Error::DataOverflow);
}

TEST_CASE("Memory single byte write read") {
    uint8_t write = 'a';
    CHECK(mac.writeByte(0x00, write) == AT24MAC::Error::None);
    mac.writeWait();
    uint8_t read;
    CHECK(mac.readByte(0x00, read) == AT24MAC::Error::None);

    CHECK(write == read);
}

TEST_CASE("Memory page write read") {
    uint8_t write[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    CHECK(mac.writePage(0x00, write) == AT24MAC::Error::None);
    mac.writeWait();
    mac.writeWait();
    std::array<uint8_t, 16> read = {0};
    CHECK(mac.read(0x00, read) == AT24MAC::Error::None);

    std::array<uint8_t, 16> write_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    CHECK(write_data == read);

    if (write_data != read) {
        bsp::debugPort.write((char*)read.data(), read.size());
        uint8_t data[256];
        if (mac.read(0x00, data) == AT24MAC::Error::None) {
            bsp::debugPort.write((char*)data, 256);
        }
    }
}

int main(int argc, char* const argv[]) {
    int result = -1;

    if (bsp::init()) {
        bsp::debugPort.write("\n\r------------------- AT24MAC Demo -------------------------\n\r");
        diagChannel.setOutputDevice(bsp::debugPort);

        diagChannel << lock << MICROHAL_INFORMATIONAL << "Starting unit tests." << endl << unlock;

        doctest::Context context(argc, argv);
        result = context.run();
        if (context.shouldExit()) {  // important - query flags (and --exit) rely on the user doing this
            bsp::deinit();
            return result;  // propagate the result of the tests
        }

    } else {
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to open communication ports." << endl << unlock;
    }

    return result;
}
