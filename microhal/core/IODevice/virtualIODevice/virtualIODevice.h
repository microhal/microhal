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

#ifndef _MICROHAL_VIRTUALIODEVICE_H_
#define _MICROHAL_VIRTUALIODEVICE_H_

#include "../IODevice.h"
#include "buffers/cyclicBuffer.h"

namespace microhal {

class VirtualIODevice : public IODevice {
 public:
    VirtualIODevice();
    virtual ~VirtualIODevice();

    VirtualIODevice(const VirtualIODevice &) = delete;
    void operator=(const VirtualIODevice &) = delete;

    bool connect(VirtualIODevice &receiver);
    bool disconnect(VirtualIODevice &receiver);

    int open(OpenMode mode) noexcept final;

    void close() noexcept final { mode = OpenMode::NotOpen; }

    int isOpen() const noexcept final { return mode != OpenMode::NotOpen; }

    ssize_t read(char *buffer, size_t length) noexcept final;

    ssize_t availableBytes() const noexcept final { return receiveBuffer.getLength(); }

    ssize_t write(const char *data, size_t length) noexcept final;

 private:
    OpenMode mode = OpenMode::NotOpen;
    VirtualIODevice *device = nullptr;
    CyclicBuffer_data<char, 100> receiveBuffer{};
};

} /* namespace microhal */

#endif /* _MICROHAL_VIRTUALIODEVICE_H_ */
