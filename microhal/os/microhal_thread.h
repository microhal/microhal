/*
 * thread.h
 *
 *  Created on: Dec 15, 2020
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_OS_MICROHAL_THREAD_H_
#define SRC_THIRD_PARTY_MICROHAL_OS_MICROHAL_THREAD_H_

#include <string_view>
#include <thread>

namespace microhal {
namespace os {

class thread : public std::thread {
 public:
    template <typename _Callable, typename... _Args>
    explicit thread(size_t stackSize, std::string_view name, int priotiry, _Callable&& __f, _Args&&... __args) : std::thread(__f, __args...) {}
};

}  // namespace os
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_OS_MICROHAL_THREAD_H_ */
