/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      GPIO unit test file
 *
 * @authors    Pawel Okas
 * created on: 06-09-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#include <cstdint>
#include "CppUTest/TestHarness.h"
#include "bsp.h"

using namespace microhal;

TEST_GROUP(GPIOTest){

    void setup(){}

    void teardown(){}  //
};

TEST(GPIOTest, TestInputGPIO) {
    CHECK(bsp::gpio.configureAsInput(GPIO::PullType::PullUp) == GPIO::Error::None);
    {
        const auto direction = bsp::gpio.getDirection();
        CHECK(direction.has_value());
        CHECK(direction.value() == GPIO::Direction::Input);
    }
    {
        const auto pull = bsp::gpio.getPullType();
        CHECK(pull.has_value());
        CHECK(pull.value() == GPIO::PullType::PullUp);
    }
    {
        const auto outputType = bsp::gpio.getOutputType();
        CHECK(outputType.has_value() == false);
        CHECK(outputType.error() == GPIO::Error::GPIONotOutput);
    }
    CHECK(bsp::gpio.get());
}

TEST(GPIOTest, TestOutputGPIO) {
    CHECK(bsp::gpio.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull) == GPIO::Error::None);
    {
        const auto direction = bsp::gpio.getDirection();
        CHECK(direction.has_value());
        CHECK(direction.value() == GPIO::Direction::Output);
    }
    {
        const auto pull = bsp::gpio.getPullType();
        CHECK(pull.has_value());
        CHECK(pull.value() == GPIO::PullType::NoPull);
    }
    {
        const auto outputType = bsp::gpio.getOutputType();
        CHECK(outputType.has_value());
        CHECK(outputType.value() == GPIO::OutputType::PushPull);
    }

    CHECK(bsp::gpio.set() == GPIO::Error::None);
    {
        const auto outputState = bsp::gpio.getOutputState();
        CHECK(outputState.has_value());
        CHECK(outputState.value() == GPIO::State::High);
    }
    {
        const auto state = bsp::gpio.get();
        CHECK(state.has_value());
        CHECK(state.value() == GPIO::State::High);
    }
}
