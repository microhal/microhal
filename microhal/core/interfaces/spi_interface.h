/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      SPI driver interface
 *
 * @authors    Pawel Okas
 * created on: 22-04-2014
 * last modification: 27-06-2016
 *
 * @copyright Copyright (c) 2014-2016, microHAL
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

#ifndef _MICROHAL_SPI_INTERFACE_H_
#define _MICROHAL_SPI_INTERFACE_H_

#include <stdint.h>
#include <mutex>

namespace microhal {

class SPI {
 public:
    typedef enum { NoError, MasterModeFault, OverrunError, CRCError, UnknownError } Error;

    /**
     *
     */
    typedef enum {
        Mode0 = 0,  //!< MODE0 CPOL/CKP = 0 CKE/NCPHA = 1
        Mode1,      //!< MODE1 CPOL/CKP = 0 CKE/NCPHA = 0
        Mode2,      //!< MODE2 CPOL/CKP = 1 CKE/NCPHA = 1
        Mode3       //!< MODE3 CPOL/CKP = 1 CKE/NCPHA = 0
    } Mode;

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

    virtual uint32_t speed(uint32_t speed) = 0;

    virtual bool setMode(Mode mode) = 0;
    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual bool getMISOstate() = 0;

    virtual Error write(const uint8_t data, bool last) = 0;
    virtual Error read(uint8_t &data, const uint8_t write = 0x00) = 0;
    virtual Error writeBuffer(const void *data, const size_t length, bool last) = 0;
    virtual Error readBuffer(void *data, const size_t length, const uint8_t write = 0x00) = 0;
    virtual Error readWrite(void *dataRead, const void *dataWrite, size_t readWriteLength) { return Error::UnknownError; }
    //	virtual SPI::Error write(const uint16_t data, bool last) {
    //		Error error;
    //		error = write((uint8_t) (data >> 8), false);
    //		if (error == NO_ERROR){
    //			return write((uint8_t) (data), last);
    //		}
    //		return error;
    //	}
    //	virtual SPI::Error read(uint16_t &data, const uint16_t write = 0x0000){
    //		uint8_t msb, lsb;
    //		Error error = read(msb, write >> 8);
    //		if(error == NO_ERROR){
    //			error = read(lsb, write);
    //			data = msb << 8 | lsb;
    //			return error;
    //		}
    //		return error;
    //	}
    virtual ~SPI() {}

 protected:
    bool noLock = false;

#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SPI()
        : mutex() {
    }

 private:
    std::mutex mutex;

    friend class SPIDevice;
};

}  // namespace microhal

#endif  // _MICROHAL_SPI_INTERFACE_H_
