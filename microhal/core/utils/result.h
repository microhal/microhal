/*
 * result.h
 *
 *  Created on: Feb 10, 2021
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_CORE_UTILS_RESULT_H_
#define SRC_THIRD_PARTY_MICROHAL_CORE_UTILS_RESULT_H_

#ifdef _MICROHAL_RESULT_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"
#endif

namespace microhal {

template <typename Value, typename Error, Error noError>
class Result {
 public:
    constexpr Result(Error error) : m_error(error) {}
    constexpr Result(const Value &value) : m_value(value), m_error(noError) {}
    constexpr Result(Error error, const Value &value) : m_value(value), m_error(error) {}

    void value(const Value &value) {
        m_value = value;
        m_error = noError;
    }
    Value &value() { return m_value; }
    const Value &value() const { return m_value; }

    Value &value_or(Value or_value) {
        if (has_value())
            return m_value;
        else
            return or_value;
    }

    void error(Error error) { m_error = error; }
    Error error() { return m_error; }

    bool has_value() const { return m_error == noError; }
    operator bool() const { return has_value(); }

    Value &operator->() { return value(); }
    Value &operator*() { return value(); }

 private:
    Value m_value;
    Error m_error;
};

}  // namespace microhal

#ifdef _MICROHAL_RESULT_USE_DIAGNOSTIC
template <microhal::diagnostic::LogLevel level, bool B, typename Value, typename Error, Error noError>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel,
                                                                  microhal::Result<Value, Error, noError> result) {
    if (result.has_value()) {
        return logChannel << result.value();
    } else {
        return logChannel << result.error();
    }
}
#endif

#endif /* SRC_THIRD_PARTY_MICROHAL_CORE_UTILS_RESULT_H_ */
