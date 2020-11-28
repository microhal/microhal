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

        setDirection(dir);
    } else {
        microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio/export file"
                                          << microhal::diagnostic::unlock;
        std::terminate();
    }
}

GPIO::~GPIO() {
    setDirection(Direction::Input);
    auto fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd > 0) {
        char buf[100];
        sprintf(buf, "%" PRIuFAST16, pin.pin);
        write(fd, buf, strlen(buf));

        close(fd);
    }
}

bool GPIO::set() {
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/value", pin.pin);
    auto fd = ::open(buf, O_WRONLY);
    if (fd > 0) {
        // Set GPIO high status
        ::write(fd, "1", 1);
        ::close(fd);
    } else {
        microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio file"
                                          << microhal::diagnostic::unlock;
        std::terminate();
    }
    return true;
}

bool GPIO::reset() {
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/value", pin.pin);
    auto fd = open(buf, O_WRONLY);
    if (fd > 0) {
        // Set GPIO low status
        write(fd, "0", 1);
        close(fd);
    } else {
        microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio file"
                                          << microhal::diagnostic::unlock;
        std::terminate();
    }
    return true;
}

bool GPIO::get() const {
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/value", pin.pin);

    auto fd = open(buf, O_RDONLY);
    if (fd > 0) {
        char value;
        read(fd, &value, 1);
        bool pinState = (value == '0') ? 0 : 1;

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

void GPIO::setDirection(Direction direction) {
    char buf[100];
    sprintf(buf, "/sys/class/gpio/gpio%" PRIuFAST16 "/direction", pin.pin);
    auto fd = open(buf, O_WRONLY);
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

}  // namespace linux
}  // namespace microhal
