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
#include <type_traits>

namespace microhal {
namespace os {

class thread : public std::thread {
 public:
#ifdef LINUX_PORT
    template <typename _Callable, typename... _Args>
    explicit thread(size_t stackSize, const char* name, int priority, _Callable&& __f, _Args&&... __args)
        : std::thread(__f, __args...), m_name(name) {}
#else
    template <typename _Callable, typename... _Args, typename = std::_Require<__not_same<_Callable> > >
    explicit thread(size_t stackSize, const char* name, int priority, _Callable&& __f, _Args&&... __args) {
        static_assert(std::__is_invocable<typename std::decay<_Callable>::type, typename std::decay<_Args>::type...>::value,
                      "std::thread arguments must be invocable after conversion to rvalues");

        auto __depend = nullptr;
        _M_start_thread(stackSize, name, priority, _S_make_state(__make_invoker(std::forward<_Callable>(__f), std::forward<_Args>(__args)...)),
                        __depend);
    }
#endif

 private:
#ifdef LINUX_PORT
    std::string m_name;
#endif
};

}  // namespace os
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_OS_MICROHAL_THREAD_H_ */
