/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 11-10-2016
 * last modification: 11-10-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

#ifndef _MICROHAL_DATABUS_STM32L4XX_H_
#define _MICROHAL_DATABUS_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <mutex>
#include "device/stm32l4xx.h"
#include "gpio_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class DataBus {
public:
    DataBus(const GPIO::Port port, const GPIO::Pin firstPin, const uint_fast8_t busWidth,
            const GPIO::OutputType type = GPIO::PushPull, const GPIO::PullType pull = GPIO::NoPull) __attribute__((always_inline)) :
            port(*reinterpret_cast<GPIO_TypeDef *>(port)), pin(firstPin), mask(((2 << busWidth) - 1) << firstPin), moder(0), mutex() {
        (void)pull;
    	uint_fast8_t width = busWidth;
        do {
            moder <<= 2;
            moder |= 1;
        }while(width--);
        moder <<= 2 * pin;

        this->port.OTYPER |= mask * type;
    }
    virtual ~DataBus() { }

    virtual void write(uint16_t data) {
        //uint32_t tmpData = (~data << 16 | data) << pin;
        //uint32_t tmpMask = mask << 16 | mask;
        //set port as output
        port.MODER |= moder;
        //write data
        // port.BSRR = tmpMask & tmpData;
        port.ODR = (port.ODR & ~mask) | ((data<<pin) & mask);
    }

    virtual void read(uint16_t &data) {
        //set port as input
        port.MODER &= ~moder;
        //read data
        data = (port.IDR & mask) >> pin;
    }

    virtual void lock() {
        mutex.lock();
    }

    virtual void unlock() {
        //set port as input
        port.MODER &= ~moder;
        //unlock
        mutex.unlock();
    }
protected:
    DataBus(const GPIO::Port port, const GPIO::Pin pin, const uint16_t mask, const uint32_t moder)
    :port(*reinterpret_cast<GPIO_TypeDef *>(port)), pin(pin), mask(mask), moder(moder), mutex() {
    }

private:
    GPIO_TypeDef &port;
    const GPIO::Pin pin;
    const uint16_t mask;
    uint32_t moder;

    std::mutex mutex;
};

}
 // namespace stm32l4xx
}// namespace microhal

#endif  // _MICROHAL_DATABUS_STM32L4XX_H_
