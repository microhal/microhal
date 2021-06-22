/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      SD Card driver test file
 *
 * @authors    Pawel Okas
 * created on: 10-09-2016
 * last modification: <DD-MM-YYYY>
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

#include <bsp.h>
#include "CppUTest/TestHarness.h"
#include "diagnostic/diagnostic.h"
#include "sd.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

TEST_GROUP(SDCardTest){};

TEST(SDCardTest, TestSDCardCRC) {
    {
        uint8_t data[5] = {0b0100'0000, 0x00, 0x00, 0x00, 0x00};
        CHECK_EQUAL(0b1001010, microhal::CRC7<>::calculate(data, sizeof(data)));
    }
    {
        uint8_t data[5] = {0b0101'0001, 0x00, 0x00, 0x00, 0x00};
        CHECK_EQUAL(0b0101010, microhal::CRC7<>::calculate(data, sizeof(data)));
    }
    {
        uint8_t data[5] = {0b0001'0001, 0x00, 0x00, 0x09, 0x00};
        CHECK_EQUAL(0b0110011, microhal::CRC7<>::calculate(data, sizeof(data)));
    }
    {
        std::array<uint8_t, 512> data;
        data.fill(0xFF);
        uint16_t crc_calculated = CRC16_XMODEM<>::calculate(data.data(), data.size());
        CHECK_EQUAL(0x7FA1, crc_calculated);
    }
}

TEST(SDCardTest, DetectSDcardTypeAndReadAllCardParameters) {
    Sd sdCard(bsp::sdCard::spi, bsp::sdCard::cs);

    CHECK(sdCard.init());

    CHECK(sdCard.getCardType() != Sd::CardType::Unknown);

    CHECK(sdCard.readCSD().has_value());
    CHECK(sdCard.getCardCapacity() > (uint64_t)0);
    // CAPTURE(sdCard.getCardCapacity());
}
