/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      SD Card driver test file
 *
 * @authors    Pawel Okas
 * created on: 17-09-2016
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

#include "CppUTest/TestHarness.h"
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "sd.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

TEST_GROUP(SDCardRWTest){};

TEST(SDCardRWTest, WriteAndReadSinglePageFromSDCard) {
    Sd sdCard(bsp::sdCard::spi, bsp::sdCard::cs);
    CHECK(sdCard.init());
    static uint8_t dataToWrite[512];
    uint8_t i = 0;
    // fill data with numbers
    for (auto &a : dataToWrite) {
        a = i++;
    }

    {
        // WHEN("We write some data to first page.") {
        const uint8_t blockNumber = 1;
        CHECK(sdCard.writeBlock(dataToWrite, blockNumber) == Sd::Error::None);
        // THEN("When we will be reading the page it contains previously written data.") {
        uint8_t readBuffer[sizeof(dataToWrite)];
        std::fill(std::begin(readBuffer), std::end(readBuffer), 0);
        CHECK(sdCard.readBlock(readBuffer, blockNumber) == Sd::Error::None);
        MEMCMP_EQUAL(dataToWrite, readBuffer, sizeof(dataToWrite));
    }
    {
        // We want to write data to last page
        const uint32_t blockNumber = sdCard.getLastPageNumber();
        CHECK(sdCard.writeBlock(dataToWrite, blockNumber) == Sd::Error::None);
        // When we will be reading the page it contains previously written data.
        uint8_t readBuffer[sizeof(dataToWrite)];
        CHECK(sdCard.readBlock(readBuffer, blockNumber) == Sd::Error::None);
        MEMCMP_EQUAL(dataToWrite, readBuffer, sizeof(dataToWrite));
    }
    {
        // We try to write data to page that don't exist
        // We get capacity error
        const uint32_t blockNumber = sdCard.getLastPageNumber() + 1;
        // CAPTURE(blockNumber);
        CHECK(sdCard.writeBlock(dataToWrite, blockNumber) == Sd::Error::OutOfRange);
    }
    {
        // WHEN("We try to read data from page that don't exist.")
        // THEN("We get capacity error") {
        uint8_t readBuffer[sizeof(dataToWrite)];
        std::fill(std::begin(readBuffer), std::end(readBuffer), 0);
        const uint32_t blockNumber = sdCard.getLastPageNumber() + 1;
        // CAPTURE(blockNumber);
        CHECK(sdCard.readBlock(readBuffer, blockNumber) == Sd::Error::OutOfRange);
    }
}
