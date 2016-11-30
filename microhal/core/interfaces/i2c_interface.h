/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      I2C interface
 *
 * @authors    Pawel Okas
 * created on: 22-04-2014
 * last modification: 27-06-2016
 *
 * @copyright Copyright (c) 2014-2016, Pawel Okas
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

#ifndef _MICROHAL_I2C_INTERFACE_H_
#define _MICROHAL_I2C_INTERFACE_H_

#include <stdint.h>
#include <mutex>

namespace microhal {

class I2C {
 public:
    typedef enum { UnknownError = 0x00, NoError = 0x01, Timeout, BusError, AcknowledgeFailure, ArbitrationLost, OverrunError } Error;

    enum class Mode : uint_fast8_t {
        Standard = 0,  // 100kHz
        Fast,          // 400kHz
        FastModePlus,  // 1MHz
        HighSpeed      // 3.4 MHz
    };

    void lock() {
        if (noLock == false) {
            mutex.lock();
        }
    }
    void unlock() {
        if (noLock == false) {
            mutex.unlock();
        }
    }

    virtual bool setMode(Mode mode) = 0;

    virtual Error write(uint8_t deviceAddress, uint8_t data) = 0;
    virtual Error write(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) = 0;
    virtual Error write(uint8_t deviceAddress, uint8_t registerAddress, const void *data, size_t length) = 0;

    virtual Error read(uint8_t deviceAddress, uint8_t &data) = 0;
    virtual Error read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) = 0;
    virtual Error read(uint8_t deviceAddress, uint8_t registerAddress, void *data, size_t length) = 0;

    virtual ~I2C() {}

 protected:
    bool noLock = false;

#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        I2C()
        : mutex() {
    }

    /**
         * @brief This function return maximum rise time for specific I2C bus
     * mode.
         *
         * @param mode for with will be maximum rise time returned.
         *
         * @return maximum rise time in [ns].
         */
    uint32_t getMaxRiseTime(Mode mode) const {
        const uint32_t riseTime[] = {1000, 300, 160, 80};
        return riseTime[static_cast<uint_fast8_t>(mode)];
    }

    uint32_t getSCLfreq(Mode mode) const {
        const uint32_t freq[] = {100000, 400000, 1000000, 3400000};
        return freq[static_cast<uint_fast8_t>(mode)];
    }

 private:
    std::mutex mutex;

    friend class I2CDevice;
};

}  // namespace microhal

#endif  // _MICROHAL_I2C_INTERFACE_H_
