/* ************************************************************************************************
 * INCLUDES
 */
#include "serialPort_linux.h"
#include <filesystem>
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;
namespace fs = std::filesystem;

namespace microhal {
namespace linux {

/* ************************************************************************************************
 * FUNCTIONS
 */
std::vector<std::string> SerialPort::getSerialPortList() {
    std::vector<std::string> result;
    std::string begin("/dev/tty");
    for (auto &p : fs::directory_iterator("/dev")) {
        auto pathString = std::string(p.path());
        if (pathString.rfind(begin, 0) == 0) {
            result.push_back(std::move(pathString));
        }
    }
    return result;
}

int SerialPort::open(OpenMode mode) noexcept {
    int openParam = O_NONBLOCK;
    switch (mode) {
        case WriteOnly:
            openParam |= O_WRONLY;
            break;
        case ReadOnly:
            openParam |= O_RDONLY;
            break;
        case ReadWrite:
            openParam |= O_RDWR;
            break;
        case NotOpen:
            if (isOpen()) close();
            return true;
    }

    if (isOpen() == false) {
        tty_fd = ::open(portName.c_str(), openParam);
        if (tty_fd > 0 && isatty(tty_fd)) {
            if (tcgetattr(tty_fd, &tio) == 0) {
                tio.c_cflag &= ~CRTSCTS;        // Disable RTS/CTS hardware flow control
                tio.c_cflag |= CREAD | CLOCAL;  // see termios.h for more information

                tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);
                tio.c_iflag &= ~(IXON | IXOFF | IXANY);                                       // Turn off s/w flow ctrl
                tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);  // Disable any special handling of received bytes

                tio.c_oflag &= ~(OPOST | ONLCR);

                tio.c_cc[VMIN] = 0;
                tio.c_cc[VTIME] = 1;
                return tcsetattr(tty_fd, TCSANOW, &tio) == 0;
            } else {
                diagChannel << MICROHAL_ERROR << "Unable to get current serial port configuration, errno value: " << strerror(errno) << endl;
            }
        } else {
            diagChannel << MICROHAL_ERROR << "Unable to open Linux serial port, errno value: " << strerror(errno) << endl;
        }
    }
    return false;
}

ssize_t SerialPort::read(char *buffer, size_t length, std::chrono::milliseconds timeout) noexcept {
    if (timeout == timeout.zero()) {
        // no timeout
        tio.c_cc[VMIN] = 0;
        tio.c_cc[VTIME] = 0;
        tcsetattr(tty_fd, TCSANOW, &tio);
    } else {
        tio.c_cc[VMIN] = 0;
        uint8_t termiosTimeout = (timeout.count() / 100) > 255 ? 255 : (timeout.count() / 100);
        tio.c_cc[VTIME] = (termiosTimeout == 0 && timeout.count() > 50) ? 1 : termiosTimeout;
        tcsetattr(tty_fd, TCSANOW, &tio);
    }
    ssize_t len = ::read(tty_fd, buffer, length);
    if (len >= 0)
        return len;
    else {
        diagChannel << Error << "Unable to read data from Linux serial port, errno value: " << strerror(errno) << endl;
        return 0;
    }
}

ssize_t SerialPort::write(const char *data, size_t length) noexcept {
    ssize_t len = ::write(tty_fd, data, length);
    if (len >= 0)
        return len;
    else {
        diagChannel << Error << "Unable to write data into Linux serial port, errno value: " << strerror(errno) << endl;
        return 0;
    }
}

bool SerialPort::clear(SerialPort::Direction dir) noexcept {
    int flags = 0;
    switch (dir) {
        case SerialPort::Direction::Input:
            flags |= TCIFLUSH;
            break;
        case SerialPort::Direction::Output:
            flags |= TCOFLUSH;
            break;
        case SerialPort::Direction::AllDirections:
            flags |= TCIOFLUSH;
            break;
    }
    if (::tcflush(tty_fd, flags) != 0) {
        diagChannel << MICROHAL_ERROR << "Unable to clear Linux serial port buffer, errno value: " << strerror(errno) << endl;
        return false;
    }
    return true;
}
/**
 * @brief This function set new baudrate.
 *
 * @param baudRate to set
 * @param dir
 *
 * @return
 */
bool SerialPort::setBaudRate(uint32_t baudRate) noexcept {
    int baud;
    // convert baudrate
    switch (baudRate) {
        case 50:
            baud = B50;
            break;
        case 75:
            baud = B75;
            break;
        case 110:
            baud = B110;
            break;
        case 134:
            baud = B134;
            break;
        case 150:
            baud = B150;
            break;
        case 200:
            baud = B200;
            break;
        case 300:
            baud = B300;
            break;
        case 600:
            baud = B600;
            break;
        case 1200:
            baud = B1200;
            break;
        case 1800:
            baud = B1800;
            break;
        case 2400:
            baud = B2400;
            break;
        case 4800:
            baud = B4800;
            break;
        case 9600:
            baud = B9600;
            break;
        case 19200:
            baud = B19200;
            break;
        case 38400:
            baud = B38400;
            break;
        case 57600:
            baud = B57600;
            break;
#ifdef B76800
        case 76800:
            baud = B76800;
            break;
#endif
        case 115200:
            baud = B115200;
            break;
        case 230400:
            baud = B230400;
            break;
        case 460800:
            baud = B460800;
            break;
        case 500000:
            baud = B500000;
            break;
        case 576000:
            baud = B576000;
            break;
        case 921600:
            baud = B921600;
            break;
        case 1000000:
            baud = B1000000;
            break;
        case 1152000:
            baud = B1152000;
            break;
        case 1500000:
            baud = B1500000;
            break;
        case 2000000:
            baud = B2000000;
            break;
        case 2500000:
            baud = B2500000;
            break;
        case 3000000:
            baud = B3000000;
            break;
        case 3500000:
            baud = B3500000;
            break;
        case 4000000:
            baud = B4000000;
            break;
        default:  // unsupported baudrate
            return false;
    }
    // set baudrate
    cfsetospeed(&tio, baud);
    cfsetispeed(&tio, baud);

    tcsetattr(tty_fd, TCSANOW, &tio);

    return true;
}
/**
 *
 * @param dir
 * @return
 */
uint32_t SerialPort::getBaudRate() const noexcept {
    auto baud = cfgetispeed(&tio);
    if (baud != cfgetospeed(&tio)) {
        return 0;
    }

    // convert baud to uint32_t
    switch (baud) {
        case B50:
            return 50;
        case B75:
            return 75;
        case B110:
            return 110;
        case B134:
            return 134;
        case B150:
            return 150;
        case B200:
            return 200;
        case B300:
            return 300;
        case B600:
            return 600;
        case B1200:
            return 1200;
        case B1800:
            return 1800;
        case B2400:
            return 2400;
        case B4800:
            return 4800;
        case B9600:
            return 9600;
        case B19200:
            return 19200;
        case B38400:
            return 38400;
        case B57600:
            return 57600;
#ifdef B76800
        case B76800:
            return 76800;
#endif
        case B115200:
            return 115200;
        default:
            return 0;
    }
}
/**
 *
 * @param parity
 * @return
 */
bool SerialPort::setParity(SerialPort::Parity parity) noexcept {
    switch (parity) {
        case SerialPort::NoParity:
            tio.c_cflag &= ~PARENB;
            break;
        case SerialPort::EvenParity:
            tio.c_cflag |= PARENB;
            tio.c_cflag &= ~PARODD;
            break;
        case SerialPort::OddParity:
            tio.c_cflag |= PARENB | PARODD;
            break;
        default:
            return false;
    }
    return tcsetattr(tty_fd, TCSANOW, &tio) == 0;
}
/**
 *
 * @param stopBits
 * @return
 */
bool SerialPort::setStopBits(SerialPort::StopBits stopBits) noexcept {
    switch (stopBits) {
        case OneStop:
            tio.c_cflag &= ~CSTOPB;
            break;
        case TwoStop:
            tio.c_cflag |= CSTOPB;
            break;
        default:
            return false;
    }
    return tcsetattr(tty_fd, TCSANOW, &tio) == 0;
}
/**
 *
 * @param dataBits
 * @return
 */
bool SerialPort::setDataBits(SerialPort::DataBits dataBits) noexcept {
    // clear actual settings
    tio.c_cflag &= ~(CS5 | CS6 | CS7 | CS8);
    // apply new settings
    switch (dataBits) {
        case Data5:
            tio.c_cflag |= CS5;
            break;
        case Data6:
            tio.c_cflag |= CS6;
            break;
        case Data7:
            tio.c_cflag |= CS7;
            break;
        case Data8:
            tio.c_cflag |= CS8;
            break;
        default:
            return false;
    }

    return tcsetattr(tty_fd, TCSANOW, &tio) == 0;
}

}  // namespace linux
}  // namespace microhal
