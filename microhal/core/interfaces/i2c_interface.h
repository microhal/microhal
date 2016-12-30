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

#include <cstdint>
#include <mutex>

namespace microhal {

class I2C {
 public:
	using DeviceAddress = uint8_t;
	using Speed = uint32_t;

    typedef enum { NoError = 0x00, Timeout, BusError, AcknowledgeFailure, ArbitrationLost, OverrunError, UnknownError = 0xFF } Error;

    enum class Mode : uint_fast8_t {
        Standard = 0,  // 100kHz
        Fast,          // 400kHz
        FastModePlus,  // 1MHz
        HighSpeed      // 3.4 MHz
    };

    void lock() noexcept {
        mutex.lock();
    }
    void unlock() noexcept {
        mutex.unlock();
    }

    virtual bool setMode(Mode mode) noexcept = 0; // todo delete

    virtual Speed speed(Speed speed) noexcept = 0;
    virtual Speed speed() noexcept = 0;
    virtual void busReset() noexcept = 0;

    Error write(uint8_t deviceAddress, uint8_t data) noexcept {
    	return write(deviceAddress, &data, sizeof(data));
    }
    virtual Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t length) noexcept = 0;
    virtual Error write(DeviceAddress deviceAddress,
        		        const uint8_t* data, size_t dataLength,
                        const uint8_t* dataB, size_t dataBLength) noexcept = 0;
    virtual Error read(DeviceAddress deviceAddress, uint8_t *data, size_t size) noexcept = 0;
    virtual Error read(DeviceAddress deviceAddress,
    		           uint8_t *data, size_t dataLength,
                       uint8_t *dataB, size_t dataBLength) noexcept = 0;
    virtual Error writeRead(DeviceAddress deviceAddress,
    		                const uint8_t* write, size_t writeLength,
							uint8_t *read, size_t readLength) noexcept = 0;

    virtual ~I2C() {}

 protected:
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
    uint32_t getMaxRiseTime(Mode mode) const noexcept {
        const uint32_t riseTime[] = {1000, 300, 160, 80};
        return riseTime[static_cast<uint_fast8_t>(mode)];
    }

    uint32_t getSCLfreq(Mode mode) const noexcept {
        const uint32_t freq[] = {100000, 400000, 1000000, 3400000};
        return freq[static_cast<uint_fast8_t>(mode)];
    }

 private:
    std::mutex mutex;

    friend class I2CDevice;
};

}  // namespace microhal

#endif  // _MICROHAL_I2C_INTERFACE_H_
