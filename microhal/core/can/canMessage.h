/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_CANMESSAGE_H_
#define _MICROHAL_CANMESSAGE_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

#include "gsl/span"

namespace microhal {
namespace can {

class Message {
 public:
    using span = gsl::span<uint8_t>;

    static constexpr size_t maxDataLength = 8;

    class ID {
     public:
        bool isStandard() const { return ide == 0; }
        bool isExtended() const { return ide; }
        uint32_t getID() const { return id; }

     protected:
        uint32_t id : 29;
        uint32_t ide : 1;

        ID(uint32_t id, uint32_t ide) : id(id), ide(ide) {}
    };
    static_assert(sizeof(ID) == sizeof(uint32_t), "Microhal internal error, sizeof CAN::Message::ID incorrect. Check compiler options.");

    class StandardID : public ID {
     public:
        StandardID(uint32_t id) : ID(id, 0) {}
    };
    class ExtendedID : public ID {
     public:
        ExtendedID(uint32_t id) : ID(id, 1) {}
    };

    struct {
        uint32_t ide : 2 = 2;
        uint32_t remote : 1;
        uint32_t stid_exid : 29;
    } id;

    Message() : size(0) {}
    /**
     * param stdid Standard Identifier 11 bits
     */
    void setStandardID(uint32_t stdid) {
        id.ide = 0;
        id.stid_exid = stdid;
    }
    /**
     * param extid Extended Identifier 29 bits
     */
    void setExtendedID(uint32_t extid) {
        id.ide = 1;
        id.stid_exid = extid;
    }

    bool setRemoteRequest(size_t requestedSize) {
        if (requestedSize > 8) return false;
        size = requestedSize;
        id.remote = 1;
        return true;
    }

    bool setDataFrame(span data) {
        if (data.size_bytes() > 8) return false;
        size = data.size_bytes();
        id.remote = 0;
        std::copy_n(std::begin(data), data.size_bytes(), this->data);
        return true;
    }
    uint32_t getID() const { return id.stid_exid; }
    gsl::span<const uint8_t> getData() const {
        gsl::span<const uint8_t> d(data, size);
        return d;
    }
    bool isStandardID() const { return id.ide == 0; }
    bool isExtendedID() const { return id.ide == 1; }
    bool isRemoteFrame() const { return id.remote; }
    bool isDataFrame() const { return !id.remote; }

    bool operator==(const Message &b) const { return id.stid_exid == b.id.stid_exid && id.ide == b.id.ide && dataEqual(b); }

 private:
    uint_fast8_t size;
    uint8_t data[8];

    bool dataEqual(const Message &b) const {
        if (size != b.size) return false;
        for (size_t i = 0; i < size; i++) {
            if (data[i] != b.data[i]) return false;
        }
        return true;
    }
};

}  // namespace can
}  // namespace microhal

#endif  // _MICROHAL_CANMESSAGE_H_
