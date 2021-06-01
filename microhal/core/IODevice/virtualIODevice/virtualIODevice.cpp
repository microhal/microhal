/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2020-2021, Pawel Okas
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

#include "virtualIODevice.h"

namespace microhal {

VirtualIODevice::VirtualIODevice() {}

VirtualIODevice::~VirtualIODevice() {}

bool VirtualIODevice::connect(VirtualIODevice &receiver) {
    if (device == nullptr) {
        device = &receiver;
        return true;
    }
    return false;
}
bool VirtualIODevice::disconnect(VirtualIODevice &receiver) {
    if (device == &receiver) {
        device = nullptr;
        return true;
    }
    return false;
}

int VirtualIODevice::open(OpenMode mode) noexcept {
    if (mode == OpenMode::NotOpen) {
        this->mode = mode;
        return true;
    }
    if (!isOpen()) {
        if (device || (mode == OpenMode::ReadOnly)) {
            this->mode = mode;
            return true;
        }
    }
    return false;
}

ssize_t VirtualIODevice::read(char *buffer, size_t length) noexcept {
    if (mode == OpenMode::ReadOnly || mode == OpenMode::ReadWrite) {
        return receiveBuffer.read(buffer, length);
    }
    return 0;
}

ssize_t VirtualIODevice::write(const char *data, size_t length) noexcept {
    if (mode == OpenMode::WriteOnly || mode == OpenMode::ReadWrite) {
        return device->receiveBuffer.write(data, length);
    }
    return 0;
}

} /* namespace microhal */
