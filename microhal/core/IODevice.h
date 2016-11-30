/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      IODevice
 *
 * @authors    Pawel
 * created on: 16-02-2014
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

#ifndef _MICROHAL_IODEVICE_H_
#define _MICROHAL_IODEVICE_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include <cstring>
#include <mutex>
#include <experimental/string_view>

namespace microhal {
/**
 * @addtogroup HAL Interfaces
 * @{
 * @class IODevice
 * @}
 *
 * \brief This class is basic interface for stream IO Devices like: serial port, files
 */
/* ************************************************************************************************
 * CLASS
 */
class IODevice {
 public:
	using string_view = std::experimental::string_view;
    /**
     *
     */
    typedef enum {
        NotOpen = 0x00,                     // The device is not open.
        ReadOnly = 0x01,                    // The device is open for reading.
        WriteOnly = 0x02,                   // The device is open for writing.
        ReadWrite = ReadOnly | WriteOnly,   // The device is open for reading and writing.
//        Append = 0x04,                      // The device is opened in append mode, so that all data is written to the end of the file.
//        Truncate = 0x08,                    // If possible, the device is truncated before it is opened. All earlier contents of the device are lost.
//        Text = 0x10,                        // When reading, the end-of-line terminators are translated to '\n'. When writing, the end-of-line
                                            //  terminators are translated to the local encoding, for example '\r\n' for Win32.
 //       Unbuffered = 0x20                   // Any buffer in the device is bypassed.
    } OpenMode;

    std::timed_mutex mutex;

#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr IODevice(): mutex() { }
#else
    IODevice(): mutex() { }
#endif

    virtual ~IODevice() { }
    /**
     *
     * @param mode
     * @retval
     */
    virtual bool open(OpenMode mode) noexcept = 0;
    /**
     *
     */
    virtual void close() noexcept  = 0;
    /**
     *
     * @return
     */
    virtual bool isOpen() const noexcept  = 0;

    /**
     *
     * @param[out] buffer
     * @param[in] length
     * @retval
     */
    virtual size_t read(char *buffer, size_t length) noexcept = 0;
    /**
     *
     */
    virtual size_t availableBytes() const noexcept = 0;
    /**
     *
     * @param[out] buffer
     * @param[in] maxLength
     * @return
     */
//    virtual size_t readLine(char *buffer, size_t maxLength) noexcept = 0;
    /**
     *
     * @param[out] buffer - pointer to buffer where data will be storage.
     * @return
     */
//    size_t readAll(char *buffer) noexcept {
//        return read(buffer, 0xFFFF);
//    }
    /**
     *
     * @return
     */
    virtual bool getChar(char &c) noexcept = 0;
    /**
     *
     * @param[in] c
     * @return
     */
    virtual bool putChar(char c) noexcept = 0;
    /**
     *
     * @param[in] data - pointer to data buffer
     * @param[in] length - length of data to write
     * @return number of bytes that was copy to buffer.
     */
    virtual size_t write(const char *data, size_t length) noexcept = 0;
    /**
     *
     * @param[in] data - pointer to text buffer.
     * @return number of bytes that was copy to buffer.
     */
    size_t write(const char *data) noexcept {
        if (data != nullptr) {
            size_t len = strlen(data);
            return write(data, len);
        } else {
            return 0;
        }
    }

    size_t write(string_view string) {
        return write(string.data(), string.length());
    }
};

} // namespace microhal

#endif // _MICROHAL_IODEVICE_H_
