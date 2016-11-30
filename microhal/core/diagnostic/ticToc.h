/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic component
 *
 * @authors    Pawel Okas
 * created on: 24-10-2014
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

#ifndef _MICROHAL_TICTOC_H_
#define _MICROHAL_TICTOC_H_

#include <chrono>
#include <cstdint>

#include "diagnostic.h"

namespace microhal {
namespace diagnostic {

class TicToc {
 public:
    constexpr TicToc() noexcept : startTime(std::chrono::high_resolution_clock::time_point::min()),
            endTime(std::chrono::high_resolution_clock::time_point::min()),
            callibrateFactor(std::chrono::high_resolution_clock::duration::zero()) {
        callibrate();
    }

    void tic() noexcept {
        startTime = getActualTime();
    }
    void toc() noexcept {
        endTime = getActualTime();
    }

    void callibrate() noexcept {
        tic();
        toc();

        callibrateFactor = endTime - startTime;
    }

    std::chrono::microseconds getDuration() const noexcept {
        return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime - callibrateFactor);
    }

 private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    std::chrono::high_resolution_clock::duration callibrateFactor;

    inline static std::chrono::high_resolution_clock::time_point getActualTime() noexcept {
        return std::chrono::high_resolution_clock::now();
    }
};

template <LogLevel level, bool B>
inline LogLevelChannel<level, B> operator<< (LogLevelChannel<level, B> log, const TicToc &time) {
    log << time.getDuration().count() << "us";

    return log;
}

}  // namespace diagnostic
}  // namespace microhal

#endif // _MICROHAL_TICTOC_H_
