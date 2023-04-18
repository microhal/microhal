/*
 * gpio_linux.cpp
 *
 *  Created on: Nov 13, 2020
 *      Author: pokas
 */

#include "gpio_linux.h"
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <cstdio>
#include "diagnostic/diagnostic.h"

namespace microhal {
namespace linux {

GPIO::GPIO(IOPin pin, Direction dir, PullType pull, OutputType type) : pin(pin) {
    auto fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd >= 0) {
        char buf[10];
        const auto len = sprintf(buf, "%" PRIuFAST16, pin.pin);
        write(fd, buf, len);
        close(fd);

        init_file_path();
        setDirection(dir);
        microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_INFORMATIONAL << "GPIO export: " << pin.pin
                                          << microhal::diagnostic::unlock;
    } else {
        microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio/export file"
                                          << microhal::diagnostic::unlock;
    }
}

GPIO::~GPIO() {
    if (pin.pin >= 0) {
        setDirection(Direction::Input);
        direction_file_path.clear();
        value_file_path.clear();

        auto fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd > 0) {
            char buf[100];
            sprintf(buf, "%" PRIuFAST16, pin.pin);
            write(fd, buf, strlen(buf));

            close(fd);
        }
    }
}

GPIO::Error GPIO::set() noexcept {
    if (!value_file_path.empty()) {
        auto fd = ::open(value_file_path.c_str(), O_WRONLY);
        if (fd > 0) {
            // Set GPIO to high state
            ::write(fd, "1", 1);
            ::close(fd);
            output_state_requested = State::High;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "GPIO error, unable to open GPIO file. File path: " << value_file_path.c_str()
                                              << microhal::diagnostic::unlock;
        }
        return Error::None;
    }
    return Error::LinuxGPIOUnconfigured;
}

GPIO::Error GPIO::reset() noexcept {
    if (!value_file_path.empty()) {
        auto fd = open(value_file_path.c_str(), O_WRONLY);
        if (fd > 0) {
            // Set GPIO to low state
            write(fd, "0", 1);
            close(fd);
            output_state_requested = State::Low;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "GPIO error, unable to open GPIO file. File path: " << value_file_path.c_str()
                                              << microhal::diagnostic::unlock;
        }
        return Error::None;
    }
    return Error::LinuxGPIOUnconfigured;
}

GPIO::PinStateReturnType GPIO::get() const noexcept {
    if (!value_file_path.empty()) {
        auto fd = open(value_file_path.c_str(), O_RDONLY);
        if (fd > 0) {
            char value;
            read(fd, &value, 1);
            close(fd);
            const State pinState = (value == '0') ? State::Low : State::High;
            return pinState;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "GPIO error, unable to open gpio file, GPIO number: " << pin.pin << microhal::diagnostic::unlock;
        }
    }
    return Error::LinuxGPIOUnconfigured;
}

GPIO::PinStateReturnType GPIO::getOutputState() const noexcept {
    return output_state_requested;
}

Expected<GPIO::OutputType, UnexpectedNegativeValue<GPIO::Error>> GPIO::getOutputType() const noexcept {}

Expected<GPIO::PullType, UnexpectedNegativeValue<GPIO::Error>> GPIO::getPullType() const noexcept {}

Expected<GPIO::Direction, UnexpectedNegativeValue<GPIO::Error>> GPIO::getDirection() const noexcept {}

void GPIO::setDirection(Direction direction) {
    if (!direction_file_path.empty()) {
        auto fd = open(direction_file_path.c_str(), O_WRONLY);
        if (fd > 0) {
            switch (direction) {
                case Direction::Output:
                    write(fd, "out", 3);
                    break;
                case Direction::Input:
                    write(fd, "in", 2);
                    break;
            }
            ::close(fd);
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "GPIO error, unable to open gpio file, GPIO number: " << pin.pin << microhal::diagnostic::unlock;
        }
    }
    // return Error::LinuxGPIOUnconfigured;
}

void GPIO::init_file_path() {
    char buf[50];
    sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/direction", pin.pin);
    direction_file_path = buf;

    sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/value", pin.pin);
    value_file_path = buf;
}

}  // namespace linux
}  // namespace microhal
