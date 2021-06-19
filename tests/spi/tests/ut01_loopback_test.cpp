/**
 * @file
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      loopback SPI test file
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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
#include <span>
#include "CppUTest/TestHarness.h"
#include "bsp.h"

//#define FULL_TEST

using namespace microhal;

const constexpr size_t bufferSize = 1024;

constexpr std::array<uint8_t, bufferSize> bufferInit() {
    std::array<uint8_t, bufferSize> tmp;

    for (size_t i = 0; i < tmp.size(); i++)
        tmp[i] = i;

    return tmp;
}

const constexpr std::array<uint8_t, bufferSize> txBuff = bufferInit();
const std::span<const uint8_t> txBuffSpan(txBuff);
std::array<uint8_t, bufferSize> rxBuff{};
const std::span<const uint8_t> rxBuffSpan(rxBuff);

static const std::array<size_t, 9> quickTestLengths = {1, 2, 4, 5, 255, 256, 511, 512, 1024};

TEST_GROUP(SPILoopbackTest){//
                            //
                            void setup(){bsp::spi1.enable();
}

void teardown() {
    bsp::spi1.disable();
}
}
;

TEST(SPILoopbackTest, TestReadWrite) {
    /* Testing 'readWrite' method of SPI class
     * this test assume that MISO and MOSI are connected together
     */
#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto txData = txBuffSpan.subspan(0, dataLength);
        auto rxData = rxBuffSpan.subspan(0, dataLength);
        rxBuff.fill(0);
        CHECK(bsp::spi1.writeRead(rxBuff.data(), txData.data(), txData.size()) == SPI::Error::None);
        MEMCMP_EQUAL(txData.data(), rxData.data(), txData.size());
    }
}

TEST(SPILoopbackTest, TestRead) {
    /* Testing 'read' method of SPI class
     * this test assume that MISO and MOSI are connected together
     */
#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto rxData = rxBuffSpan.subspan(0, dataLength);
        rxBuff.fill(0);
        CHECK(bsp::spi1.read(rxBuff.data(), rxData.size(), 0xA3) == SPI::Error::None);
        CHECK(std::all_of(rxData.begin(), rxData.end(), [](uint8_t val) { return val == 0xA3; }));
    }
}

TEST(SPILoopbackTest, TestWrite) {
    /* Testing 'write' method of SPI class
     * in this test we can't tell if sent data are correct, but we are testing result returned from 'write' method
     */
#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto txData = txBuffSpan.subspan(0, dataLength);
        CHECK(bsp::spi1.write(txData.data(), txData.size(), false) == SPI::Error::None);
    }

#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto txData = txBuffSpan.subspan(0, dataLength);
        CHECK(bsp::spi1.write(txData.data(), txData.size(), true) == SPI::Error::None);
    }
}

TEST(SPILoopbackTest, TestFunctionOrder) {
    /* Testing if sequential method call don't break SPI functionality
     * this test assume that MISO and MOSI are connected together
     */

    // test write and writeRead
#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto txData = txBuffSpan.subspan(0, dataLength);
        auto rxData = rxBuffSpan.subspan(0, dataLength);
        rxBuff.fill(0);
        CHECK(bsp::spi1.write(txData.data(), txData.size(), false) == SPI::Error::None);
        CHECK(bsp::spi1.writeRead(rxBuff.data(), txData.data(), txData.size()) == SPI::Error::None);
        MEMCMP_EQUAL(txData.data(), rxData.data(), txData.size());
    }

    // test read and writeRead
#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto txData = txBuffSpan.subspan(0, dataLength);
        auto rxData = rxBuffSpan.subspan(0, dataLength);
        rxBuff.fill(0);
        CHECK(bsp::spi1.read(rxBuff.data(), rxData.size(), 0x5A) == SPI::Error::None);
        CHECK(std::all_of(rxData.begin(), rxData.end(), [](uint8_t val) { return val == 0x5A; }));
        CHECK(bsp::spi1.writeRead(rxBuff.data(), txData.data(), txData.size()) == SPI::Error::None);
        MEMCMP_EQUAL(txData.data(), rxData.data(), txData.size());
    }

    // test write and read
#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto txData = txBuffSpan.subspan(0, dataLength);
        auto rxData = rxBuffSpan.subspan(0, dataLength);
        rxBuff.fill(0);
        CHECK(bsp::spi1.write(txData.data(), txData.size(), false) == SPI::Error::None);
        CHECK(bsp::spi1.read(rxBuff.data(), rxData.size(), 0xA3) == SPI::Error::None);
        CHECK(std::all_of(rxData.begin(), rxData.end(), [](uint8_t val) { return val == 0xA3; }));
    }

    // test writeRead and read
#ifdef FULL_TEST
    for (size_t i = 0; i < txBuffSpan.size(); i++) {
        const size_t dataLength = i + 1;
#else
    for (size_t i = 0; i < quickTestLengths.size(); i++) {
        const size_t dataLength = quickTestLengths[i];
#endif
        auto txData = txBuffSpan.subspan(0, dataLength);
        auto rxData = rxBuffSpan.subspan(0, dataLength);
        rxBuff.fill(0);
        CHECK(bsp::spi1.writeRead(rxBuff.data(), txData.data(), txData.size()) == SPI::Error::None);
        MEMCMP_EQUAL(txData.data(), rxData.data(), txData.size());
        CHECK(bsp::spi1.read(rxBuff.data(), rxData.size(), 0x45) == SPI::Error::None);
        CHECK(std::all_of(rxData.begin(), rxData.end(), [](uint8_t val) { return val == 0x45; }));
    }
}
