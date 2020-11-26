/*
 * virtualIODevice.h
 *
 *  Created on: Nov 19, 2020
 *      Author: pokas
 */

#ifndef THIRD_PARTY_MICROHAL_CORE_IODEVICE_VIRTUALIODEVICE_H_
#define THIRD_PARTY_MICROHAL_CORE_IODEVICE_VIRTUALIODEVICE_H_

#include "IODevice.h"
#include "buffers/cyclicBuffer.h"

namespace microhal {

class VirtualIODevice : public IODevice {
 public:
    VirtualIODevice();
    virtual ~VirtualIODevice();

    bool connect(VirtualIODevice &receiver) {
        if (device == nullptr) {
            device = &receiver;
            return true;
        }
        return false;
    }
    bool disconnect(VirtualIODevice &receiver) {
        if (device == &receiver) {
            device = nullptr;
            return true;
        }
        return false;
    }

    bool open(OpenMode mode) noexcept final {
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

    void close() noexcept final { mode = OpenMode::NotOpen; }

    bool isOpen() const noexcept final { return mode != OpenMode::NotOpen; }

    size_t read(char *buffer, size_t length) noexcept final {
        if (mode == OpenMode::ReadOnly || mode == OpenMode::ReadWrite) {
            return receiveBuffer.read(buffer, length);
        }
        return 0;
    }

    size_t availableBytes() const noexcept final { return receiveBuffer.getLength(); }

    size_t write(const char *data, size_t length) noexcept final {
        if (mode == OpenMode::WriteOnly || mode == OpenMode::ReadWrite) {
            return device->receiveBuffer.write(data, length);
        }
        return 0;
    }

 private:
    OpenMode mode = OpenMode::NotOpen;
    VirtualIODevice *device = nullptr;
    CyclicBuffer_data<char, 100> receiveBuffer;
};

} /* namespace microhal */

#endif /* THIRD_PARTY_MICROHAL_CORE_IODEVICE_VIRTUALIODEVICE_H_ */
