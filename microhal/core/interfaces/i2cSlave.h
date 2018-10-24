/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 06-08-2018
 * last modification: 06-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_I2CSLAVE_H_
#define _MICROHAL_I2CSLAVE_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "gsl/span"
#include "i2c_interface.h"

namespace microhal {
/* **************************************************************************************************************************************************
 * CLASS
 */
class I2CSlave {
 public:
    using span = gsl::span<uint8_t>;  // todo replace to std::span when will be available

    I2CSlave(uint8_t address, uint8_t *dataPtr) : address(address), dataPtr(dataPtr) {}
    I2CSlave(const I2CSlave &) = delete;
    I2CSlave &operator=(const I2CSlave &) = delete;
    virtual ~I2CSlave() {}

    uint8_t getAddress() const noexcept { return address; }
    /**
     * @note The prepareTransmitBuffer method can be called form interrupt.
     * This function is called when I2C master requested read access, in this function we should prepare buffer with data to send.
     * If preparing buffer is impossible function should return false and when transmit data will be ready slaveBufferReady method should be called.
     */
    virtual span prepareTransmitData() = 0;
    /**
     * @note The prepareReceiveBuffer method can be called from interrupt.
     * This function is called when I2C master requested write access, in this function we should prepare buffer for incoming data.
     * If from some reason preparing buffer is impossible function should return false, I2C lines will be stretched by I2C slave until
     * slaveBufferReady method will be called.
     */
    virtual span prepareReceiveBuffer() = 0;
    /**
     * @note The onWriteFinish method can be called from i2c driver interrupt.
     * This function is called when I2C master reception is finished.
     */
    virtual void onTransmitFinish(size_t transmitted) = 0;
    /**
     * @note The onReadFinish method can be called from i2c driver interrupt.
     * This function is called when transmission from I2C master to I2C slave was finished.
     */
    virtual void onReceiveFinish(size_t received) = 0;

 protected:
    uint8_t address;
    uint8_t *dataPtr;

    friend microhal::I2C;
};
}  // namespace microhal
#endif  // _MICROHAL_I2CSLAVE_H_
