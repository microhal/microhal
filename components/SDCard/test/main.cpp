/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic example main file
 *
 * @authors    Pawel Okas
 * created on: 2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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

#include <bsp.h>
#include <cstring>

#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "sd.h"

#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_NOSTDOUT

// extern "C" int _EXFUN(gettimeofday, (struct timeval * __restrict __p, void* __restrict __tz));
#include "catch.hpp"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

#include <errno.h>
#include <sys/stat.h>

class CatchBuf : public std::stringbuf {
    char buff[300];

 public:
    CatchBuf() { this->setp(buff, &buff[298]); }
    virtual int sync() override {
        writeData();
        return 0;
    }

    int overflow(int c) override {
        if (c != traits_type::eof()) {
            *pptr() = c;
            pbump(1);
            writeData();
            return c;
        }

        return traits_type::eof();
    }

    void writeData() {
        size_t toWrite = pptr() - pbase();
        char* dataPtr = pbase();
        do {
            size_t written = bsp::debugPort.write(dataPtr, toWrite);
            dataPtr += written;
            toWrite -= written;
        } while (toWrite);
        pbump(-(pptr() - pbase()));
    }
};

namespace Catch {
CatchBuf buff;
std::ostream stream(&buff);

std::ostream& cout() {
    return stream;
}
std::ostream& cerr() {
    return stream;
}
}  // namespace Catch

extern "C" int _isatty(int fd) {
    struct stat buf;

    if (fstat(fd, &buf) < 0) {
        errno = EBADF;
        return 0;
    }
    if (S_ISCHR(buf.st_mode)) return 1;
    errno = ENOTTY;
    return 0;
}

int main() {
    bsp::init();
    bsp::debugPort.write("\n\r------------------- SD card test -------------------------\n\r");
    diagChannel.setOutputDevice(bsp::debugPort);
    // diagChannel.setLogLevel(Informational);
    // lets check if diagChannal is working
    diagChannel << lock << MICROHAL_EMERGENCY << "Information from diagnostic channel." << unlock;

    auto result = Catch::Session().run();

    while (1) {
        // diagChannel << lock << Emergency << "." << unlock;
        std::this_thread::sleep_for(5s);
    }

    return 0;
}
