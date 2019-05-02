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
#include "catch.hpp"
#include "diagnostic/diagnostic.h"
#include "sd.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

SCENARIO("Detect SD cart type and read all card parameters.", "[initialize]") {
    {
        uint8_t data[5] = {0b0100'0000, 0x00, 0x00, 0x00, 0x00};
        CHECK(microhal::CRC7::calculate(data, sizeof(data), 0) == 0b1001010);
    }
    {
        uint8_t data[5] = {0b0101'0001, 0x00, 0x00, 0x00, 0x00};
        CHECK(microhal::CRC7::calculate(data, sizeof(data), 0) == 0b0101010);
    }
    {
        uint8_t data[5] = {0b0001'0001, 0x00, 0x00, 0x09, 0x00};
        CHECK(microhal::CRC7::calculate(data, sizeof(data), 0) == 0b0110011);
    }

    {
        std::array<uint8_t, 512> data;
        data.fill(0xFF);
        uint16_t crc_calculated = CRC16::calculate(data.data(), data.size(), 0);
        CHECK(crc_calculated == 0x7FA1);
    }
    GIVEN("Unknown SD card") {
        Sd sdCard(bsp::sdCard::spi, bsp::sdCard::cs);
        WHEN("Card was initialized successfully.") {
            REQUIRE(sdCard.init());
            THEN("We can check card type") {
                REQUIRE(sdCard.getCardType() != Sd::CardType::Unknown);
                AND_THEN("We can read card capacity") {
                    REQUIRE(sdCard.readCSD());
                    REQUIRE(sdCard.getCardCapacity() > (uint64_t)0);
                    CAPTURE(sdCard.getCardCapacity());
                }
            }
        }
    }
}
