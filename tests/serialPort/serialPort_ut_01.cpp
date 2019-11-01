/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      serial port test
 *
 * @authors    Pawel Okas
 * created on: 5-01-2016
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

#include "microhal.h"

#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "bsp.h"

using namespace microhal;
using SerialPortList = std::vector<SerialPort *>;

// -------------------------------------------------- tests --------------------------------------------------
void sendTest(std::string_view str, SerialPortList ports) {
    for (auto serial : ports) {
        serial->write(str);
    }
}

void echoTest(SerialPort &serial, SerialPortList ports) {
    while (1) {
        for (auto serial : ports) {
            if (serial->availableBytes() != 0) {
                char buffer[100];
                auto read = serial->read(buffer, sizeof(buffer));
                serial->write(buffer, read);
            }
        }
        if (serial.availableBytes() >= 3) {
            char buffer[10];
            serial.read(buffer, sizeof(buffer));
            std::string_view cmd(buffer);
            if (cmd == "end") break;
        }
    }
}

void setBaudrate(uint32_t baudrate, SerialPortList ports) {
    for (auto serial : ports) {
        serial->setBaudRate(baudrate);
    }
}

void setParity(SerialPort::Parity parity, SerialPortList ports) {
    for (auto serial : ports) {
        serial->setParity(parity);
    }
}

void setStopBits(SerialPort::StopBits stop, SerialPortList ports) {
    for (auto serial : ports) {
        serial->setStopBits(stop);
    }
}

void writeSerials(SerialPortList ports) {
    for (auto serial : ports) {
        serial->write("test string\n");
    }
}

//---------------------------------------------------------------- end of tests ------------------------------------------------------
auto findCommandBegin(std::string_view command) {
    return command.find('-');
}

std::string readCommand(SerialPort &serial) {
    char tmp[200] = {};
    size_t inBuffer = 0;
    while (1) {
        inBuffer += serial.read(tmp + inBuffer, sizeof(tmp) - inBuffer);
        if (inBuffer) {
            std::string_view command(tmp);
            if (command.find('\r') != std::string_view::npos) {
                if (command.find('-') != std::string_view::npos && command.find('-') < command.find('\r')) {
                    // we have command in buffer lets extract it
                    auto cmd = command.substr(command.find('-') + 1, command.find('\r') - 1);
                    return std::string(cmd);
                } else {
                    inBuffer -= command.find('\r') + 1;
                    std::copy(std::begin(tmp) + command.find('\r') + 1, std::end(tmp), std::begin(tmp));
                }
            }
        }
    }
}

void testController(SerialPort &serial, SerialPortList ports) {
    serial.write("Serial tests");
    while (1) {
        auto commandParameter = readCommand(serial);
        std::string_view commandAndParameter(commandParameter);
        auto command = commandAndParameter.find(' ') == std::string_view::npos ? commandAndParameter
                                                                               : commandAndParameter.substr(0, commandAndParameter.find(' '));
        auto parameter = commandAndParameter.substr(command.length(), commandAndParameter.length());

        parameter.remove_prefix(parameter.find_first_not_of(' ') != std::string_view::npos ? parameter.find_first_not_of(' ') : 0);

        if (command == "baudrate") {
            serial.write("Received baudrate command");

            char *end;
            auto i = strtol(std::string(parameter).c_str(), &end, 10);
            if (errno == ERANGE) {
                serial.write("range error, got ");
                errno = 0;
            } else {
                serial.write("\n Setting baudrate to: ");
                char buffer[50];
                itoa(i, buffer, 10);
                serial.write(buffer);
                setBaudrate(i, ports);
            }
        } else if (command == "test") {
            serial.write("Received test command");
            writeSerials(ports);
        } else if (command == "parity") {
            if (parameter == "none") {
                setParity(SerialPort::Parity::NoParity, ports);
                serial.write("Setting parity to: none");
            } else if (parameter == "even") {
                setParity(SerialPort::Parity::EvenParity, ports);
                serial.write("Setting parity to: even");
            } else if (parameter == "odd") {
                setParity(SerialPort::Parity::OddParity, ports);
                serial.write("Setting parity to: odd");
            } else {
                serial.write("Unsupported parity parameter: ");
                serial.write(parameter);
            }
        } else if (command == "stop") {
            if (parameter == "one") {
                setStopBits(SerialPort::StopBits::OneStop, ports);
                serial.write("Setting stop bits to: one");
            } else if (parameter == "onePointFive") {
                setStopBits(SerialPort::StopBits::OneAndHalfStop, ports);
                serial.write("Setting stop bits to: one and half");
            } else if (parameter == "two") {
                setStopBits(SerialPort::StopBits::TwoStop, ports);
                serial.write("Setting stop bits to: two");
            } else {
                serial.write("Unsupported parity parameter: ");
                serial.write(parameter);
            }
        }

        else if (command == "echo") {
            serial.write("Starting echo test.");
            echoTest(serial, ports);
        } else {
            serial.write("unsupported command: ");
            serial.write(command);
        }
    }
}

static void configureSerialPort(SerialPort &serial) {
    serial.open(SerialPort::ReadWrite);
    serial.setBaudRate(SerialPort::Baud115200);
    serial.setDataBits(SerialPort::Data8);
    serial.setStopBits(SerialPort::OneStop);
    serial.setParity(SerialPort::NoParity);
}

int main(int argc, char *argv[]) {
#if defined(LINUX)
    initialize(argc, argv);
#endif
    for (auto serial : serialPorts) {
        configureSerialPort(*serial);
    }

    SerialPortList allPorts = {
        &microhal::stm32f4xx::SerialPort::Serial1, &microhal::stm32f4xx::SerialPort::Serial2, &microhal::stm32f4xx::SerialPort::Serial4,
        &microhal::stm32f4xx::SerialPort::Serial3
        // microhal::stm32f4xx::SerialPort::Serial5,
        // microhal::stm32f4xx::SerialPort::Serial6
    };

    SerialPortList ports = {
        &microhal::stm32f4xx::SerialPort::Serial1, &microhal::stm32f4xx::SerialPort::Serial2,
        &microhal::stm32f4xx::SerialPort::Serial4  //,
                                                   // microhal::stm32f4xx::SerialPort::Serial5,
        // microhal::stm32f4xx::SerialPort::Serial6
    };

    sendTest("----------------------------- SerialPort Test -----------------------------\n", allPorts);
    sendTest("----------------------------- SerialPort Test -----------------------------\n", allPorts);
    //   echoTest();

    testController(*serialPorts[2], ports);

    return 0;
}
