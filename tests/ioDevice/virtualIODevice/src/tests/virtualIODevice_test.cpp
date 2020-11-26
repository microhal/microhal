/*
 * virtualIODevice_test.cpp
 *
 *  Created on: Nov 19, 2020
 *      Author: pokas
 */

#include "IODevice/virtualIODevice.h"
#include "doctest.h"

using namespace microhal;

TEST_CASE("Testing microhal::VirtualIODevice class") {
    VirtualIODevice deviceA;
    VirtualIODevice deviceB;

    SUBCASE("Testing open functionality") {
        CHECK(deviceA.open(IODevice::OpenMode::ReadOnly));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());
        CHECK_FALSE(deviceA.open(IODevice::OpenMode::WriteOnly));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());
        CHECK_FALSE(deviceA.open(IODevice::OpenMode::ReadWrite));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());
        CHECK(deviceA.open(IODevice::OpenMode::NotOpen));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());

        REQUIRE(deviceA.connect(deviceB));
        CHECK(deviceA.open(IODevice::OpenMode::ReadOnly));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());
        CHECK(deviceA.open(IODevice::OpenMode::WriteOnly));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());
        CHECK(deviceA.open(IODevice::OpenMode::ReadWrite));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());
        CHECK(deviceA.open(IODevice::OpenMode::NotOpen));
        deviceA.close();
        REQUIRE(!deviceA.isOpen());
        REQUIRE(deviceA.disconnect(deviceB));
    }

    SUBCASE("Testing transmission functionality") {
        REQUIRE(deviceA.connect(deviceB));
        REQUIRE(deviceB.connect(deviceA));

        REQUIRE(deviceA.open(IODevice::OpenMode::ReadWrite));
        REQUIRE(deviceB.open(IODevice::OpenMode::ReadWrite));

        constexpr std::string_view str("Test string");
        CHECK(deviceA.write(str.data(), str.length()));
        CHECK(deviceA.availableBytes() == 0);
        CHECK(deviceB.availableBytes() == str.length());

        char buffer[20];
        static_assert(sizeof(buffer) > str.size(), "");
        REQUIRE(deviceB.read(buffer, str.length()) == str.length());
        std::string_view receivedStr(buffer, str.length());
        CHECK(str == receivedStr);
    }
}
