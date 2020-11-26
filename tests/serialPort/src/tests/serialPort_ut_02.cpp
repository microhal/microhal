/*
 * serialPort_ut_02.cpp
 *
 *  Created on: Nov 26, 2020
 *      Author: pokas
 */

#include <string_view>
#include "bsp.h"
#include "doctest.h"

using namespace std::literals;
using namespace microhal;

TEST_CASE("FixedPoint addition test") {
    SUBCASE("Test isOpen") {
        CHECK_FALSE(serialPort1.isOpen());
        CHECK(serialPort1.open(SerialPort::ReadWrite));
        CHECK(serialPort1.isOpen());
        serialPort1.close();
        CHECK_FALSE(serialPort1.isOpen());
    }

    SUBCASE("Test transmit and receive") {
        CHECK(serialPort1.open(SerialPort::ReadWrite));
        CHECK(serialPort2.open(SerialPort::ReadWrite));
        REQUIRE(serialPort1.isOpen());
        REQUIRE(serialPort2.isOpen());
        serialPort1.clear();
        serialPort2.clear();

        CHECK(serialPort1.availableBytes() == 0);
        CHECK(serialPort2.availableBytes() == 0);

        std::string_view write = "Text that is testing write correctness.";
        CHECK(serialPort1.write(write) == write.length());
        CHECK(serialPort1.availableBytes() == 0);
        std::this_thread::sleep_for(10ms);
        CHECK(serialPort2.availableBytes() == write.length());
        char buffer[100];
        CHECK(serialPort2.read(buffer, write.length()) == write.length());
        CHECK(write.compare(buffer) == 0);

        std::array<const char, 5> data{0, 1, 0, 5, 126};
        CHECK(serialPort1.write(data.data(), data.size()) == data.size());
        CHECK(serialPort1.availableBytes() == 0);
        std::this_thread::sleep_for(10ms);
        CHECK(serialPort2.availableBytes() == data.size());
        CHECK(serialPort2.read(buffer, data.size()) == data.size());
        for (size_t i = 0; i < data.size(); i++) {
            CHECK(buffer[i] == data[i]);
        }
    }
}
