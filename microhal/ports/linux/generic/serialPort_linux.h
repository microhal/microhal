/*
 * serialPortlinux.h
 *
 *  Created on: 16 sty 2014
 *      Author: pawel
 */

#ifndef SERIALPORTLINUX_H_
#define SERIALPORTLINUX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "serialPort.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>  // needed for memset
#include <vector>

#include <sys/ioctl.h>

#if defined(linux)
#undef linux
#endif

namespace microhal {
namespace linux {
/* ************************************************************************************************
 * CLASS
 */
class SerialPort : public microhal::SerialPort {
 public:
    SerialPort(const microhal::linux::SerialPort &) = delete;
    void operator=(const microhal::linux::SerialPort &) = delete;

    static std::vector<std::string> getSerialPortList();

    std::string_view getPortName() const { return portName; }

    int open(OpenMode mode) noexcept final;
    /**
     * @brief This function check is serial port open. When port is open function
     * return true in other cases return false;
     *
     * @retval true if port is open
     * @retval false if port is close
     */
    int isOpen(void) const noexcept final { return (tty_fd != 0); }
    /**
     * @brief This function close serial port
     */
    void close() noexcept final {
        if (tty_fd) {
            if (::close(tty_fd) == 0) {
                tty_fd = 0;
            }
        }
    }
    /**
     *
     * @return
     */
    int getChar(char &c) noexcept final { return ::read(tty_fd, &c, 1); }
    /**
     *
     * @param c
     * @return
     */
    int putChar(char c) noexcept final { return ::write(tty_fd, &c, 1); }
    /**
     * @brief This function write data to transmit buffer and start sending it.
     *
     * @param[in] data - pointer to data buffer
     * @param[in] length - number of bytes to send
     *
     * @return number of bytes sent.
     */
    ssize_t write(const char *data, size_t length) noexcept final;
    /**
     * @brief This function read data from serial port.
     *
     * @param[out] buffer
     * @param[in] length
     *
     * @return number of read data.
     */
    ssize_t read(char *buffer, size_t length, std::chrono::milliseconds timeout) noexcept final;

    size_t inputQueueSize() const noexcept final {
        return 0;  // todo
    }
    size_t outputQueueSize() const noexcept final {
        return 0;  // todo
    }
    /**
     * @brief This function return available bytes in receive buffer.
     *
     * @return number of bytes in receive buffer.
     */
    ssize_t availableBytes(void) const noexcept final {
        int bytes;

        ioctl(tty_fd, FIONREAD, &bytes);

        return bytes;
    }

    bool waitForWriteFinish(std::chrono::milliseconds timeout) const noexcept final { return ::tcdrain(tty_fd) == 0; }

    bool clear(SerialPort::Direction dir = AllDirections) noexcept final;

    bool setBaudRate(uint32_t baudRate) noexcept final;
    uint32_t getBaudRate() const noexcept final;

    bool setParity(SerialPort::Parity parity) noexcept final;
    bool setStopBits(SerialPort::StopBits stopBits) noexcept final;
    bool setDataBits(SerialPort::DataBits dataBits) noexcept final;

    //------------------------------------------- constructors --------------------------------------//
    SerialPort(const char *name) : tty_fd(0), portName(name) { memset(&tio, 0, sizeof(tio)); }
    virtual ~SerialPort() {
        if (isOpen()) close();
    }

 private:
    //------------------------------------------- variables -----------------------------------------//
    struct termios tio {};
    int tty_fd = 0;
    const char *portName{};
};

}  // namespace linux
}  // namespace microhal

#endif  // SERIALPORTLINUX_H_
