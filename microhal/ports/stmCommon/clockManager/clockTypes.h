/*
 * clockTypes.h
 *
 *  Created on: Nov 17, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CLOCKTYPES_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CLOCKTYPES_H_

#include <cstdint>
#include <exception>
#include <type_traits>

namespace microhal {
namespace ClockManager {

using Frequency = uint32_t;
enum class PowerMode { Normal = 0b1, Sleep = 0b10 };

static constexpr bool isEnabled(PowerMode mode, PowerMode flag) {
    using enumType = std::underlying_type<PowerMode>::type;
    if (static_cast<enumType>(mode) & static_cast<enumType>(flag)) return true;
    return false;
}

namespace ClockManagerDetail {
//********************************************************************************************
#define CREATE_SET_CLEAR_FUNCTION(x)                                                        \
    template <typename T, typename = void>                                                  \
    struct hasMember##x : std::false_type {};                                               \
                                                                                            \
    template <typename T>                                                                   \
    struct hasMember##x<T, std::void_t<decltype(std::declval<T>().x)>> : std::true_type {}; \
                                                                                            \
    template <typename T>                                                                   \
    constexpr std::enable_if_t<hasMember##x<T>::value> set_##x##_ifExist(T& a) {            \
        a.x.set();                                                                          \
    }                                                                                       \
                                                                                            \
    template <typename T>                                                                   \
    constexpr std::enable_if_t<hasMember##x<T>::value> clear_##x##_ifExist(T& a) {          \
        a.x.clear();                                                                        \
    }                                                                                       \
                                                                                            \
    template <typename T>                                                                   \
    constexpr std::enable_if_t<hasMember##x<T>::value, uint32_t> get_##x##_ifExist(T& a) {  \
        return a.x;                                                                         \
    }                                                                                       \
                                                                                            \
    constexpr void set_##x##_ifExist(...) {}                                                \
    constexpr void clear_##x##_ifExist(...) {}                                              \
    constexpr uint32_t get_##x##_ifExist(...) {                                             \
        while (1) {                                                                         \
        }                                                                                   \
    }
//********************************************************************************************
}  // namespace ClockManagerDetail
}  // namespace ClockManager
}  // namespace microhal
#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CLOCKTYPES_H_ */
