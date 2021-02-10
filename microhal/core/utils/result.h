/*
 * result.h
 *
 *  Created on: Feb 10, 2021
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_CORE_UTILS_RESULT_H_
#define SRC_THIRD_PARTY_MICROHAL_CORE_UTILS_RESULT_H_

namespace microhal {

template <typename Value, typename Error, Error noError>
class Result {
 public:
    constexpr Result(Error error) : m_error(error) {}
    constexpr Result(const Value &value) : m_value(value), m_error(noError) {}

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

    bool has_value() const { return m_error == noError; }
    operator bool() const { return has_value(); }

    Value &operator->() { return value(); }
    Value &operator*() { return value(); }

 private:
    Value m_value;
    Error m_error;
};

}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_CORE_UTILS_RESULT_H_ */
