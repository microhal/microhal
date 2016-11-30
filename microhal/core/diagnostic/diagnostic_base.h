/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic component
 *
 * @authors    Pawel Okas
 * created on: 12-03-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015, microHAL
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

#ifndef _MICROHAL_DIAGNOSTIC_BASE_H_
#define _MICROHAL_DIAGNOSTIC_BASE_H_

#include "../IODevice.h"
#include "../ioDeviceNull/IODeviceNull.h"
#include "../itoa.h"

namespace microhal {
namespace diagnostic {

/**
 *  @brief This enum specify possible debug levels.
 */
enum class LogLevel {
    Disabled = 0,
    Emergency = 1,      //!< Emergency
    Alert = 2,          //!< Alert
    Critical = 3,       //!< Critical
    Error = 4,          //!< Error
    Warning = 5,        //!< Warning
    Notice = 6,         //!< Notice
    Informational = 7,  //!< Informational
    Debug = 8,          //!< Debug
};

constexpr bool operator >= (LogLevel a, LogLevel b) {
    return static_cast<unsigned int>(a) >= static_cast<unsigned int>(b);
}

class LogLevelHeader_base {
 public:
    explicit constexpr LogLevelHeader_base(const char* levelFileLineInfo, const char *code) noexcept
        : fileLine(levelFileLineInfo), code(code) {
    }
 private:
    const char* fileLine;
    const char* code;

    friend class Diagnostic_base;
};

typedef enum : uint8_t {
    EnableTimestamp = 0x01,
    EnableFileName = 0x02,
    EnableLevelName = 0x04,
    EnableErrorCode = 0x08
} HeaderDisplayMode;

constexpr HeaderDisplayMode operator| (const HeaderDisplayMode &x1, const HeaderDisplayMode &x2) noexcept {
    return static_cast<HeaderDisplayMode>(static_cast<unsigned>(x1) | static_cast<unsigned>(x2));
}


/**
 * @addtogroup Diagnostics
 * @{
 * @class Diagnostics
 * @}
 * @brief This class provides basic log functions.
 */
/* ************************************************************************************************
 * CLASS
 */
class Diagnostic_base {
 public:
    //------------------------------------------ functions ----------------------------------------
    /** This function set diagnostic output device.
     *
     * @param device - reference to IODevice
     */
    inline void setOutputDevice(IODevice &device) __attribute__ ((always_inline)) {
        ioDevice = &device;
    }
    /** This function enable or disable automatic space insertion.
     *
     * @param insert - if true then space will be automatically insert after every '<<' operator
     */
    inline void autoInsertSpaces(bool insert) __attribute__ ((always_inline)) {
        spaces = insert;
    }

    inline void setHeaderDisplayMode(HeaderDisplayMode mode) __attribute__ ((always_inline)) {
        headerDisplayMode = mode;
    }

    template <class _Rep, class _Period>
    inline bool tryLock(const std::chrono::duration<_Rep, _Period>& __rtime) {
        return ioDevice->mutex.try_lock_for(__rtime);
    }

 protected:
    //------------------------------------------- variables ---------------------------------------
    static constexpr char endLine[3] = "\n\r";
    static const char *levelName[9];

    IODevice *ioDevice;
    const char *header;
    LogLevel logLevel;
    bool spaces;
    HeaderDisplayMode headerDisplayMode;

    //----------------------------------------- constructors --------------------------------------
    constexpr Diagnostic_base(const char * header, HeaderDisplayMode mode, LogLevel level) noexcept
                              : ioDevice(&nullIODevice), header(header), logLevel(level), spaces(false), headerDisplayMode(mode) {
    }
    constexpr Diagnostic_base(const char * header, IODevice &dev, HeaderDisplayMode mode, LogLevel level) noexcept
                              : ioDevice(&dev), header(header), logLevel(level), spaces(false) , headerDisplayMode(mode) {
    }


    //------------------------------------------ functions ----------------------------------------
    inline void putChar(char c) {
        while (ioDevice->putChar(c) != true) {}
    }

    inline void writeText(const char *txt, size_t len) {
        size_t written = ioDevice->write(txt, len);
        if (written != len) {
            size_t toWrite = len - written;
            do {
                written += ioDevice->write(txt + written, toWrite);
                toWrite = len - written;
            } while (toWrite);
        }
    }

    inline void insertSpace() {
        if (spaces == true) {
            putChar(' ');
        }
    }

    void write(const char *c) {
#ifdef MICROHAL_DIAGNOSTIC_TEXT_VISIBLE
        if (c != nullptr) {
            size_t len = strlen(c);
            writeText(c, len);
            insertSpace();
        }
#else
        (void)c;
#endif
    }

    void endl() {
        writeText("\n\r", 2);
    }

    void write(const uint8_t *data, size_t size, uint8_t radix);
    void write(const uint16_t *data, size_t size, uint8_t radix);
    void write(const uint32_t *data, size_t size, uint8_t radix);
    /** This function write to output device unsigned data.
     *
     * @param data
     * @param radix
     */
    void write(const uint32_t data, uint8_t radix) {
        char buffer[33];

        switch (radix) {
            case 2:
                ioDevice->write("0b");
                break;
            case 16:
                ioDevice->write("0x");
                break;
        }
        itoa(data, buffer, radix);
        size_t len = strlen(buffer);
        writeText(buffer, len);

        insertSpace();
    }

    void write(const uint64_t data, uint8_t radix) {
        char buffer[65];

        switch (radix) {
            case 2:
                ioDevice->write("0b");
                break;
            case 16:
                ioDevice->write("0x");
                break;
        }
        itoa(data, buffer, radix);
        size_t len = strlen(buffer);
        writeText(buffer, len);

        insertSpace();
    }
    /**
     *
     * @param data
     * @param radix
     */
    void write(const int32_t data, uint8_t radix) {
        char buffer[33];

        switch (radix) {
            case 2:
                ioDevice->write("0b");
                break;
            case 16:
                ioDevice->write("0x");
                break;
        }

        itoa(data, buffer, radix);
        size_t len = strlen(buffer);
        writeText(buffer, len);

        insertSpace();
    }

    void write(const int64_t data, uint8_t radix) {
        char buffer[65];

        switch (radix) {
            case 2:
                ioDevice->write("0b");
                break;
            case 16:
                ioDevice->write("0x");
                break;
        }

        itoa(data, buffer, radix);
        size_t len = strlen(buffer);
        writeText(buffer, len);

        insertSpace();
    }
    /**
     *
     * @param state
     */
    void write(bool state) {
        if (state) {
            writeText("true", 4);
        } else {
            writeText("false", 5);
        }
        insertSpace();
    }
    /**
     *
     * @param d
     */
    void write(double d) {
        char buffer[10];

        int len = snprintf(buffer, sizeof(buffer), "%f", d);
        writeText(buffer, len);
        insertSpace();
    }

    void printHeader(const LogLevelHeader_base &logHeader, const LogLevel level);
};

}  // namespace diagnostic
}  // namespace microhal

#endif  // _MICROHAL_DIAGNOSTIC_BASE_H_
