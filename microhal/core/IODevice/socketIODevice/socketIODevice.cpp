/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#include "socketIODevice.h"
#include <unistd.h>
#include <cerrno>
#include "sys/socket.h"

#ifdef MICROHAL_SOCKETIODEVICE_USE_DIAGNOSTIC
#include <string_view>
#include "diagnostic/diagnostic.h"

using namespace std::literals;
using namespace microhal::diagnostic;
#endif

namespace microhal {

SocketIODevice::SocketIODevice(int &&socket) : sockfd(socket) {}

SocketIODevice::~SocketIODevice() {
    readThread.request_stop();
    readThread.join();
    ::close(sockfd);
}

int SocketIODevice::open(OpenMode mode) noexcept {
    if (!isOpen()) {
        if (mode == OpenMode::ReadOnly || mode == OpenMode::ReadWrite) {
            readThread = std::jthread(&SocketIODevice::readThreadFunction, this);
        }
        this->mode = mode;
    }
    return false;
}

void SocketIODevice::close() noexcept {
    readThread.request_stop();
    mode = OpenMode::NotOpen;
}

ssize_t SocketIODevice::read(char *buffer, size_t length) noexcept {
    if (mode == OpenMode::ReadOnly || mode == OpenMode::ReadWrite) {
        return receiveBuffer.read(buffer, length);
    }
    return 0;
}

ssize_t SocketIODevice::write(const char *data, size_t length) noexcept {
    auto status = send(sockfd, data, length, 0);
    if (status < 0) {
#ifdef MICROHAL_SOCKETIODEVICE_USE_DIAGNOSTIC
        const auto error = errno;
        diagChannel << lock << MICROHAL_ERROR << "Socket send error, function returned: " << status << "  errno: " << error << " -> "
                    << strerror(error) << endl
                    << unlock;
#endif
        mode = OpenMode::NotOpen;
    }
    return status;
}

void SocketIODevice::readThreadFunction() {
    auto stoken = readThread.get_stop_token();
    while (!stoken.stop_requested()) {
        auto emptySpan = receiveBuffer.getEmptyContinousPart();
        const auto status = recv(sockfd, emptySpan.data(), emptySpan.size(), 0);
        if (status > 0) {
            std::lock_guard<std::mutex> g(receiveBufferMutex);
            receiveBuffer.updateWritePointer(status);
        } else {
#ifdef MICROHAL_SOCKETIODEVICE_USE_DIAGNOSTIC
            const auto error = errno;
            diagChannel << lock << MICROHAL_ERROR << "Socket recv error, function returned: " << status << "  errno: " << error << " -> "
                        << strerror(error) << endl
                        << unlock;
#endif
            mode = OpenMode::NotOpen;
            break;
        }
    }
}

}  // namespace microhal
