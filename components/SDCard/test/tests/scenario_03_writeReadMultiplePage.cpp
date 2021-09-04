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

#include <algorithm>
#include <array>
#include "CppUTest/TestHarness.h"
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "sd.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

TEST_GROUP(SDCardRWMultiplePagesTest){};

TEST(SDCardRWMultiplePagesTest, WriteAndReadMultiplePageFromSDCard) {
    // GIVEN("Initialized SD card") {
    Sd sdCard(bsp::sdCard::spi, bsp::sdCard::cs);
    CHECK(sdCard.init());
    constexpr uint32_t blockCount = 2;
    static uint8_t dataToWrite[blockCount * 512];
    uint8_t i = 0;
    // fill data with numbers
    for (auto &a : dataToWrite) {
        a = i++;
    }
    //        WHEN("We write some data starting from first page.") {
    //            const uint8_t blockNumber = 1;
    //            REQUIRE(sdCard.writeMultipleBlock(dataToWrite, blockNumber, blockCount) == Sd::Error::None);
    //
    //            THEN("When we will be reading pages they contains previously written data.") {
    //                auto readBuffer = std::make_unique<std::array<uint8_t, sizeof(dataToWrite)> >();
    //                readBuffer->fill(0);
    //                REQUIRE(sdCard.readMultipleBlock(readBuffer->data(), blockNumber, blockCount) == Sd::Error::None);
    //                REQUIRE(memcmp(dataToWrite, readBuffer->data(), sizeof(dataToWrite)) == 0);
    //            }
    //        }

    {
        // WHEN("We want to write data that ends on last page")
        const uint32_t blockNumber = sdCard.getLastPageNumber() - blockCount;
        CHECK(sdCard.writeMultipleBlock(dataToWrite, blockNumber, blockCount) == Sd::Error::None);
        // THEN("When we will be reading the page it contains previously written data.") {
        auto readBuffer = std::make_unique<std::array<uint8_t, sizeof(dataToWrite)> >();
        std::fill(readBuffer->begin(), readBuffer->end(), 0);
        //  CAPTURE(blockCount);
        CHECK(sdCard.readMultipleBlock(readBuffer->data(), blockNumber, blockCount) == Sd::Error::None);
        MEMCMP_EQUAL(dataToWrite, readBuffer->data(), sizeof(dataToWrite));
        //}
    }
    {
        // WHEN("We try to write data to page that don't exist")
        // THEN("We get capacity error") {
        const uint32_t blockNumber = sdCard.getLastPageNumber() + 1;
        // CAPTURE(blockNumber);
        CHECK(sdCard.writeMultipleBlock(dataToWrite, blockNumber, blockCount) == Sd::Error::OutOfRange);
    }
    {
        // WHEN("We try to read data from page that don't exist.") {
        //  THEN("We get capacity error") {
        auto readBuffer = std::make_unique<std::array<uint8_t, sizeof(dataToWrite)> >();
        readBuffer->fill(0);
        const uint32_t blockNumber = sdCard.getLastPageNumber() + 1;
        // CAPTURE(blockNumber);
        CHECK(sdCard.readMultipleBlock(readBuffer->data(), blockNumber, blockCount) == Sd::Error::OutOfRange);
        //}
    }
    {
        // WHEN("We try to write data to page that cross out of space boundary.") {
        //   THEN("We get capacity error") {
        const uint32_t blockNumber = sdCard.getLastPageNumber();
        //  CAPTURE(blockNumber);
        CHECK((sdCard.writeMultipleBlock(dataToWrite, blockNumber, blockCount) & Sd::Error::DataWriteError) == Sd::Error::DataWriteError);
        // }
    }
    {
        // WHEN("We try to read data from page that cross out of space boundary.") {
        //  THEN("We get capacity error") {
        auto readBuffer = std::make_unique<std::array<uint8_t, sizeof(dataToWrite)> >();
        readBuffer->fill(0);
        const uint32_t blockNumber = sdCard.getLastPageNumber();
        //    CAPTURE(blockNumber);
        CHECK(sdCard.readMultipleBlock(readBuffer->data(), blockNumber, blockCount) == Sd::Error::OutOfRange);
        //}
    }
}
