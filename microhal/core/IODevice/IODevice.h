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

#ifndef MICROHAL_IODEVICE_H_
#define MICROHAL_IODEVICE_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include <cstring>
#include <mutex>
#include <string_view>

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
    using string_view = std::string_view;
    /**
     *
     */
    typedef enum {
        NotOpen = 0x00,                    // The device is not open.
        ReadOnly = 0x01,                   // The device is open for reading.
        WriteOnly = 0x02,                  // The device is open for writing.
        ReadWrite = ReadOnly | WriteOnly,  // The device is open for reading and writing.
    } OpenMode;

    std::recursive_timed_mutex mutex = {};

#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr IODevice() : mutex() {}
#else
    IODevice() = default;
#endif

    virtual ~IODevice() = default;
    /**
     *
     * @param mode
     * @retval
     */
    virtual int open(OpenMode mode) noexcept = 0;
    /**
     *
     */
    virtual void close() noexcept = 0;
    /**
     *
     * @return
     */
    [[nodiscard]] virtual int isOpen() const noexcept = 0;
    /**
     *
     * @param[out] buffer
     * @param[in] length
     * @retval
     */
    virtual ssize_t read(char *buffer, size_t length) noexcept = 0;
    /**
     *
     */
    [[nodiscard]] virtual ssize_t availableBytes() const noexcept = 0;
    /**
     *
     * @return
     */
    virtual int getChar(char &c) noexcept { return read(&c, 1) == 1; }
    /**
     *
     * @param[in] c
     * @return
     */
    virtual int putChar(char c) noexcept { return write(&c, 1) == 1; }
    /**
     *
     * @param[in] data - pointer to data buffer
     * @param[in] length - length of data to write
     * @return number of bytes that was copy to buffer.
     */
    virtual ssize_t write(const char *data, size_t length) noexcept = 0;
    /**
     *
     * @param[in] data - pointer to text buffer.
     * @return number of bytes that was copy to buffer.
     */
    ssize_t write(const char *data) noexcept {
        if (data != nullptr) {
            size_t len = strlen(data);
            return write(data, len);
        } else {
            return 0;
        }
    }

    ssize_t write(string_view string) { return write(string.data(), string.length()); }
};

}  // namespace microhal

#endif  // MICROHAL_IODEVICE_H_
