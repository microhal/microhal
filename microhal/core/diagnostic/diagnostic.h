/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic component
 *
 * @authors    Pawel Okas
 * created on: 4-12-2015
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

#ifndef _MICROHAL_DIAGNOSTIC_H_
#define _MICROHAL_DIAGNOSTIC_H_
/* ************************************************************************************************
 * INCLUDE
 */
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "diagnostic_base.h"
#include "diagnostic_headerGenerator.h"
#include "diagnostics_conversion.h"
#include "microhalConfig.h"

// check if needed macros are defined
#ifndef MICROHAL_DIAGNOSTIC_LOG_LEVEL
#error MICROHAL_DIAGNOSTIC_LOG_LEVEL macro undefined. You must define MICROHAL_DIAGNOSTIC_LOG_LEVEL macro to one of LogLevel enum values. \
    All logs witch less priority than specyfied in MICROHAL_DIAGNOSTIC_LOG_LEVEL will be removed from code.
#endif

#undef ERROR  // todo remove

// define log headers
#define MICROHAL_EMERGENCY MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Emergency)
#define MICROHAL_ALERT MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Alert)
#define MICROHAL_CRITICAL MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Critical)
#define MICROHAL_ERROR MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Error)
#define MICROHAL_WARNING MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Warning)
#define MICROHAL_NOTICE MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Notice)
#define MICROHAL_INFORMATIONAL MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Informational)
#define MICROHAL_DEBUG MICROHAL_DIAGNOSTIC_HEADER_GENERATOR(microhal::diagnostic::LogLevel::Debug)

namespace microhal {
namespace diagnostic {

// create type for line end. This simplify overload operator << in Diagnostics class
typedef enum { endl } LineEnd;

// create type for unlock. This simplify overload operator << in Diagnostics class
typedef enum { unlock } UnlockType;

// create type for lock.
typedef enum { lock } LockType;

template <LogLevel level>
class LogLevels {
 public:
    explicit constexpr LogLevels() {}
};

// lock / unlock
// constexpr static LockUnlock<Diagnostic::Lock> lock{};
// constexpr static LockUnlock<Diagnostic::Unlock> unlock{};
// log levels
constexpr static LogLevels<LogLevel::Disabled> Disabled{};
constexpr static LogLevels<LogLevel::Emergency> Emergency{};
constexpr static LogLevels<LogLevel::Alert> Alert{};
constexpr static LogLevels<LogLevel::Critical> Critical{};
constexpr static LogLevels<LogLevel::Error> Error{};
constexpr static LogLevels<LogLevel::Warning> Warning{};
constexpr static LogLevels<LogLevel::Notice> Notice{};
constexpr static LogLevels<LogLevel::Informational> Informational{};
constexpr static LogLevels<LogLevel::Debug> Debug{};

static constexpr LogLevel compileTimeLogLevel = LogLevel::MICROHAL_DIAGNOSTIC_LOG_LEVEL;

// previous definition of lof level channel class, to this class all messages will be print.
template <LogLevel compileTimeLogLevel, bool B>
class LogLevelChannel;
/**
 *
 */
template <LogLevel compileTimeLogLevel>
class Diagnostic : public Diagnostic_base {
 public:
    //----------------------------------------- constructors --------------------------------------
    explicit constexpr Diagnostic(const char *header,
                                  HeaderDisplayMode mode = EnableTimestamp | EnableFileName | EnableLevelName | EnableErrorCode) noexcept
        : Diagnostic_base(header, mode, compileTimeLogLevel) {}
    explicit constexpr Diagnostic(const char *header, IODevice &dev,
                                  HeaderDisplayMode mode = EnableTimestamp | EnableFileName | EnableLevelName | EnableErrorCode) noexcept
        : Diagnostic_base(header, dev, mode, compileTimeLogLevel) {}
    //------------------------------------------ functions ----------------------------------------
    template <class _Rep, class _Period>
    inline bool tryLock(const std::chrono::duration<_Rep, _Period> &__rtime) {
        // return locked = ioDevice->mutex.try_lock_for(__rtime);
        return ioDevice->mutex.try_lock_for(__rtime);
    }
    //------------------------------------------ operators ----------------------------------------
    inline Diagnostic<compileTimeLogLevel> &operator<<(const UnlockType) __attribute__((always_inline)) {
        ioDevice->mutex.unlock();
        return *this;
    }

    inline Diagnostic<compileTimeLogLevel> &operator<<(const LockType) __attribute__((always_inline)) {
        // lazy evaluation of mutex locking. We will lock mutex only when this will be needed.
        // because user can change 'log level' at runtime we can suppose that sometimes message between 'lock' and 'unlock' commands won't be shown
        // on screen, so mutex locking and unlocking will be waste of MCU resources.
        // needLock = true;
        ioDevice->mutex.lock();
        return *this;
    }

    /** This function set log level for this diagnostic channel. If set as Warning then Notice, Informational
     *  and Debug will be ignored.
     *
     * @param level - maximum level for this diagnostic channel
     */
    template <LogLevel level>
    void setLogLevel(LogLevels<level>) {
        setLogLevel(level);
    }

    void setLogLevel(LogLevel level) {
        if (level <= compileTimeLogLevel) {
            logLevel = level;
        } else {
            logLevel = compileTimeLogLevel;

            ioDevice->write("\n\r---> Unable to set log level to: ");
            ioDevice->write(levelName[static_cast<unsigned int>(level)]);
            ioDevice->write(", because this value has lower priority than compile time set: ");
            ioDevice->write(levelName[static_cast<unsigned int>(compileTimeLogLevel)]);
            ioDevice->write(". <---\n\r");
        }
    }
    // This friend operator will be called when user write: "DiagnosticObject << LogingLevel ..." for example "diagChannel << Debug ...". This
    // operator create and return
    // log level channel object.
    template <LogLevel compileLogLevel, LogLevel level>
    friend constexpr LogLevelChannel<compileLogLevel, compileLogLevel >= level> operator<<(Diagnostic<compileLogLevel> &diagnostic,
                                                                                           const LogLevels<level>);

    // This operator is called when user write for example "diagChannel << WARNING ..." where WARNING is predefined macro.
    template <LogLevel compileLogLevel, LogLevel level>
    friend constexpr LogLevelChannel<compileLogLevel, compileLogLevel >= level> operator<<(Diagnostic<compileLogLevel> &diagnostic,
                                                                                           const LogLevelHeader<level> &header);

 private:
    template <LogLevel compileLogLevel, bool B>
    friend class LogLevelChannel;
};

// for disabled diagchannel lets overload our template. Let redefine all functions and live them empty so linker should be able to delete all unused
// text from
// binary file.
template <>
class Diagnostic<LogLevel::Disabled> {
 public:
    explicit constexpr Diagnostic(LogLevel level __attribute__((unused)) = LogLevel::Debug) noexcept {}
    explicit constexpr Diagnostic(const char *header __attribute__((unused)), IODevice &dev __attribute__((unused)),
                                  LogLevel level __attribute__((unused)) = LogLevel::Debug) noexcept {}

    template <class _Rep, class _Period>
    constexpr bool tryLock(const std::chrono::duration<_Rep, _Period> &__rtime) const noexcept {
        return true;
    }
    // lets provide generic operator << so we wouldn't have to overload this operator for every type,
    template <typename T>
    constexpr const Diagnostic<LogLevel::Disabled> &operator<<(const T t __attribute__((unused))) const noexcept {
        return *this;
    }

    void setOutputDevice(IODevice &) const noexcept __attribute__((always_inline)) {}

    void autoInsertSpaces(bool) const noexcept __attribute__((always_inline)) {}

    void setLogLevel(LogLevel) const noexcept __attribute__((always_inline)) {}
};

/* **************************************************************************************************************************************************
 * LogLevelChannel class
 */
template <LogLevel compileTimeLogLevel, bool B>
class LogLevelChannel {
 public:
    //------------------------------------------ operators ------------------------------------
    auto &operator<<(UnlockType unlock) {
        diagnostic << unlock;
        return *this;
    }

    template <LogLevel level>
    constexpr auto &operator<<(const LogLevels<level> &l) {
        return diagnostic << l;
    }

    template <LogLevel level>
    constexpr auto &operator<<(const LogLevelHeader<level> &header) {
        return diagnostic << header;
    }

    auto &operator<<(LineEnd) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.endl();
            }
        }
        return *this;
    }

    LogLevelChannel &operator<<(const char *c) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.write(c);
            }
        }
        return *this;
    }

    //    LogLevelChannel &operator<<(const std::string &string) {
    //        if constexpr (B) {
    //            if (diagnostic.logLevel >= this->logLevel) {
    //                diagnostic.writeText(string.c_str(), string.length());
    //            }
    //        }
    //        return *this;
    //    }

    template <size_t len>
    LogLevelChannel &operator<<(const char (&c)[len]) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.writeText(c, len - 1);
            }
        }
        return *this;
    }

    LogLevelChannel &operator<<(uint8_t i) { return operator<<(static_cast<uint32_t>(i)); }

    LogLevelChannel &operator<<(uint16_t i) { return operator<<(static_cast<uint32_t>(i)); }
#if !defined LINUX_PORT && !defined WINDOWS_PORT
    LogLevelChannel &operator<<(unsigned int i) { return operator<<(static_cast<uint32_t>(i)); }
#endif
    LogLevelChannel &operator<<(uint32_t i) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.write(i, 10);
            }
        }
        return *this;
    }

    LogLevelChannel &operator<<(uint64_t i) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.write(i, 10);
            }
        }
        return *this;
    }

    LogLevelChannel &operator<<(int8_t i) { return operator<<(static_cast<int32_t>(i)); }

    LogLevelChannel &operator<<(int16_t i) { return operator<<(static_cast<int32_t>(i)); }

    LogLevelChannel &operator<<(int32_t i) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.write(i, 10);
            }
        }
        return *this;
    }

    LogLevelChannel &operator<<(int64_t i) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.write(i, 10);
            }
        }
        return *this;
    }

    template <unsigned int base>
    LogLevelChannel &operator<<(const Converter<base> &converter) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                if (converter.dataPtr == nullptr) {
                    diagnostic.write(converter.data, base);
                } else {
                    switch (converter.data) {
                        case 1:
                            diagnostic.write(static_cast<const uint8_t *>(converter.dataPtr), converter.length, base);
                            break;
                        case 2:
                            diagnostic.write(static_cast<const uint16_t *>(converter.dataPtr), converter.length, base);
                            break;
                        case 4:
                            diagnostic.write(static_cast<const uint32_t *>(converter.dataPtr), converter.length, base);
                            break;
                    }
                }
            }
        }
        return *this;
    }

    LogLevelChannel &operator<<(double d) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.write(d);
            }
        }
        return *this;
    }

    LogLevelChannel &operator<<(bool b) {
        if constexpr (B) {
            if (diagnostic.logLevel >= this->logLevel) {
                diagnostic.write(b);
            }
        }
        return *this;
    }

 private:
    const LogLevel logLevel;
    class Diagnostic<compileTimeLogLevel> &diagnostic;

    //----------------------------------------- constructors ----------------------------------
    // because this constructor is private object of this class can be created by two friend operator
    explicit constexpr LogLevelChannel(LogLevel logLevel, Diagnostic<compileTimeLogLevel> &diagnostic) noexcept
        : logLevel(logLevel), diagnostic(diagnostic) {}

    //----------------------------------------- friends ----------------------------------
    friend Diagnostic<compileTimeLogLevel>;

    // This friend operator will be called when user write: "DiagnosticObject << LogingLevel ..." for example "diagChannel << Debug ...".
    // This operator create and return log level channel object.
    template <LogLevel compileLogLevel, LogLevel level>
    friend constexpr LogLevelChannel<compileLogLevel, compileLogLevel >= level> operator<<(Diagnostic<compileLogLevel> &diagnostic,
                                                                                           const LogLevels<level>);

    // This operator is called when user write for example "diagChannel << WARNING ..." where WARNING is predefined macro.
    template <LogLevel compileLogLevel, LogLevel level>
    friend constexpr LogLevelChannel<compileLogLevel, compileLogLevel >= level> operator<<(Diagnostic<compileLogLevel> &diagnostic,
                                                                                           const LogLevelHeader<level> &header);
};
/* **************************************************************************************************************************************************
 * out of class operators
 */
template <LogLevel compileTimeLogLevel, LogLevel level>
constexpr LogLevelChannel<compileTimeLogLevel, compileTimeLogLevel >= level> operator<<(Diagnostic<compileTimeLogLevel> &diagnostic,
                                                                                        const LogLevels<level>) {
    return LogLevelChannel<compileTimeLogLevel, compileTimeLogLevel >= level>(level, diagnostic);
}

template <LogLevel compileTimeLogLevel, LogLevel level>
constexpr LogLevelChannel<compileTimeLogLevel, compileTimeLogLevel >= level> operator<<(Diagnostic<compileTimeLogLevel> &diagnostic,
                                                                                        const LogLevelHeader<level> &header) {
    if constexpr (compileTimeLogLevel >= level) {
        if (diagnostic.logLevel >= level) {
            diagnostic.printHeader(header, level);
        }
    }
    return LogLevelChannel<compileTimeLogLevel, compileTimeLogLevel >= level>(level, diagnostic);
}
/* **************************************************************************************************************************************************
 * EXTERN OBJECTS
 */
extern Diagnostic<compileTimeLogLevel> diagChannel;

}  // namespace diagnostic
}  // namespace microhal

#endif  // _MICROHAL_DIAGNOSTIC_H_
