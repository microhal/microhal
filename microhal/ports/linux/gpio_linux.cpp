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
    char buf[100];
    auto fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd > 0) {
        sprintf(buf, "%" PRIuFAST16, pin.pin);
        write(fd, buf, strlen(buf));
        close(fd);

        init_file_path();
        setDirection(dir);
    } else {
        microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio/export file"
                                          << microhal::diagnostic::unlock;
        std::terminate();
    }
}

GPIO::~GPIO() {
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

GPIO::Error GPIO::set() noexcept {
    if (!value_file_path.empty()) {
        // char buf[100];
        //        sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/value", pin.pin);
        auto fd = ::open(value_file_path.c_str(), O_WRONLY);
        if (fd > 0) {
            // Set GPIO high status
            ::write(fd, "1", 1);
            ::close(fd);
            output_state_requested == State::High;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio file"
                                              << microhal::diagnostic::unlock;
            std::terminate();
        }
        return Error::None;
    }
    return Error::LinuxGPIOUnconfigured;
}

GPIO::Error GPIO::reset() noexcept {
    if (!value_file_path.empty()) {
        // char buf[100];
        // sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/value", pin.pin);
        auto fd = open(value_file_path.c_str(), O_WRONLY);
        if (fd > 0) {
            // Set GPIO low status
            write(fd, "0", 1);
            close(fd);
            output_state_requested == State::Low;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio file"
                                              << microhal::diagnostic::unlock;
            std::terminate();
        }
        return Error::None;
    }
    return Error::LinuxGPIOUnconfigured;
}

GPIO::PinStateReturnType GPIO::get() const noexcept {
    // char buf[100];
    // sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/value", pin.pin);
    if (!value_file_path.empty()) {
        auto fd = open(value_file_path.c_str(), O_RDONLY);
        if (fd > 0) {
            char value;
            read(fd, &value, 1);
            const State pinState = (value == '0') ? State::Low : State::High;

            close(fd);
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "Pin value " << value
                                              << microhal::diagnostic::unlock;
            return pinState;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "GPIO error, unable to open gpio file, GPIO number: " << pin.pin << microhal::diagnostic::unlock;
            std::terminate();
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
    // char buf[100];
    //  sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/direction", pin.pin);
    if (!value_file_path.empty()) {
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
