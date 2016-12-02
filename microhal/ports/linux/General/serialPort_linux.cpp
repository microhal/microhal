/* ************************************************************************************************
 * INCLUDES
 */
#include "serialPort_linux.h"

namespace microhal {
namespace linux {

/* ************************************************************************************************
 * FUNCTIONS
 */
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
    //convert baudrate
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
    default:    //unsupported baudrate
        return false;
    }
    //set baudrate
//    switch (dir) {
//    case SerialPort::Input:
//        cfsetispeed(&tio, baud);
//        break;
//    case SerialPort::Output:
//        cfsetospeed(&tio, baud);
//        break;
//    case SerialPort::AllDirections:
        cfsetospeed(&tio, baud);
        cfsetispeed(&tio, baud);
//        break;
//    }
    tcsetattr(tty_fd, TCSANOW, &tio);

    return true;
}
/**
 *
 * @param dir
 * @return
 */
uint32_t SerialPort::getBaudRate() const noexcept {
    int baud;
    //get baudrate
//    switch (dir) {
//    case SerialPort::Input:
//        baud = cfgetispeed(&tio);
//        break;
//    case SerialPort::Output:
//        baud = cfgetospeed(&tio);
//        break;
//    case SerialPort::AllDirections:
        baud = cfgetispeed(&tio);
        if (baud != cfgetospeed(&tio)) {
            return 0;
        }
     //   break;
//    default:
//        return 0;
//    }
    //convert baud to uint32_t
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
    tcsetattr(tty_fd, TCSANOW, &tio);
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
    tcsetattr(tty_fd, TCSANOW, &tio);
}
/**
 *
 * @param dataBits
 * @return
 */
bool SerialPort::setDataBits(SerialPort::DataBits dataBits) noexcept {
    //clear actual settings
    tio.c_cflag &= ~(CS5 | CS6 | CS7 | CS8);
    //apply new settings
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
    tcsetattr(tty_fd, TCSANOW, &tio);
}
}
}
