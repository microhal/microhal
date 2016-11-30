/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      SerialPort driver interface
 *
 * @authors    Pawel Okas
 * created on: 16-01-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2016, Paweł Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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

#ifndef _MICROHAL_SERIALPORT_INTERFACE_H_
#define _MICROHAL_SERIALPORT_INTERFACE_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <chrono>
#include <cstdint>
#include "IODevice.h"

namespace microhal {
/*!
 *  * @addtogroup HAL Interfaces
 * @{
 * @class SerialPort
 * @}
 *
 * \brief The SerialPort class provides functions to access serial ports
 */
/* ************************************************************************************************
 * CLASS
 */
class SerialPort : public microhal::IODevice {
 public:
    /*!
     * This enum describes the baud rate which the communication device operates
     * with. Note: only the most common standard baud rates are listed in this
     * enum.
     */
    enum BaudRate {
        Baud1200 = 1200,      ///<	1200 baud.
        Baud2400 = 2400,      ///<	2400 baud.
        Baud4800 = 4800,      ///<	4800 baud.
        Baud9600 = 9600,      ///<	9600 baud.
        Baud19200 = 19200,    ///<	19200 baud.
        Baud38400 = 38400,    ///<	38400 baud.
        Baud57600 = 57600,    ///<	57600 baud.
        Baud115200 = 115200,  ///<	115200 baud.
        Baud230400 = 230400,  ///<	230400 baud.
        Baud460800 = 460800,  ///<	460800 baud.
        Baud576000 = 576000,  ///<	576000 baud.
        Baud921600 = 921600,  ///<	921600 baud.
        Baud1M = 1000000,     ///<	1000000 baud.
        Baud2M = 2000000,     ///<  2000000 baud.
        UnknownBaud = -1      ///<	Unknown baud.
    };

    /*!
     * This enum describes the number of data bits used.
     */
    enum DataBits {
        Data5,           ///<	Five bits.
        Data6,           ///< 	Six bits.
        Data7,           ///< 	Seven bits.
        Data8,           ///<	Eight bits.
        Data9,           ///< 	Nine bits.
        UnknownDatabits  ///< 	Unknown number of bits.
    };

    /*!
     * This enum describes the possible directions of the data transmission.
     */
    enum Direction {
        Input = 0x01,                   ///< Input direction.
        Output = 0x02,                  ///< Output direction.
        AllDirections = Input | Output  ///< Simultaneously in two directions.
    };

    /*!
     * This enum describes the flow control used.
     */
    enum FlowControl {
        NoFlowControl,      ///< No flow control.
        HardwareControl,    ///< Hardware flow control (RTS/CTS).
        SoftwareControl,    ///< Software flow control (XON/XOFF).
        UnknownFlowControl  ///< Unknown flow control.
    };

    /*!
     * This enum describes the parity scheme used.
     */
    enum Parity {
        NoParity,      ///< No parity.
        EvenParity,    ///< Even parity.
        OddParity,     ///< Odd parity.
        SpaceParity,   ///< Space parity.
        MarkParity,    ///< Mark parity.
        UnknownParity  ///< Unknown parity.
    };

    /*!
     *  This enum describes the errors that may be occurred.
     */
    enum SerialPortError {
        NoError = 0,              ///< No error occurred.
        OpenError = 3,            ///< An error occurred while attempting to open an already opened device in this object.
        ParityError = 4,          ///< Parity error detected by the hardware while reading data.
        FramingError = 5,         ///< Framing error detected by the hardware while reading data.
        BreakConditionError = 6,  ///< Break condition detected by the hardware on the input line.
        WriteError = 7,           ///< An I/O error occurred while writing the data.
        ReadError = 8,            ///< An I/O error occurred while reading the data.
        UnknownError = 11         ///< An unidentified error occurred.
    };

    /*!
     * This enum describes the number of stop bits used.
     */
    enum StopBits {
        OneStop,         ///< 1 stop bit.
        OneAndHalfStop,  ///< 1.5 stop bits.
        TwoStop,         ///< 2 stop bits.
        UnknownStopBits  ///< Unknown number of stop bits/
    };
    //------------------------------------------ destructors ----------------------------------------//
    //virtual ~SerialPort() {}
    //------------------------------------------- functions -----------------------------------------//

    bool open(OpenMode mode, uint32_t baudRate, DataBits dataBits = Data8, StopBits stopBits = OneStop, Parity parity = NoParity) noexcept;
    // IODevice functions
    virtual bool open(OpenMode mode) noexcept = 0;
//    void close() noexcept override = 0;
//    bool isOpen() const noexcept override = 0;

    virtual size_t read(char *buffer, size_t length, std::chrono::milliseconds timeout) = 0;
    size_t read(char *buffer, size_t length) noexcept override {
        return read(buffer, length, std::chrono::milliseconds::zero());
    }
//    virtual size_t readLine(char *buffer, size_t maxLength) noexcept {
//        (void)buffer;
//        (void)maxLength;
//        // todo implement
//        return 0;
//    }

//    virtual bool getChar(char &c) noexcept = 0;
//    virtual bool putChar(char c) noexcept = 0;

//    virtual size_t write(const char *data, size_t length) noexcept = 0;
//    using IODevice::write;
    // end of IODevice functions

    virtual bool setBaudRate(uint32_t baudRate) noexcept = 0;
    virtual uint32_t getBaudRate() const noexcept = 0;
    virtual bool setParity(SerialPort::Parity parity) noexcept = 0;
    virtual bool setStopBits(SerialPort::StopBits stopBits) noexcept = 0;
    virtual bool setDataBits(SerialPort::DataBits dataBits) noexcept = 0;

    virtual size_t inputQueueSize() const noexcept = 0;
    virtual size_t outputQueueSize() const noexcept = 0;
    virtual size_t availableBytes() const noexcept = 0;

    virtual bool waitForWriteFinish(std::chrono::milliseconds timeout) const noexcept = 0;
    virtual bool clear(SerialPort::Direction dir = AllDirections) noexcept = 0;
};

}  // namespace microhal

#endif // _MICROHAL_SERIALPORT_INTERFACE_H_
