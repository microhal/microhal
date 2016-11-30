/*
 * dataBus.h
 *
 *  Created on: 10 kwi 2014
 *      Author: pawel
 */

#ifndef DATABUS_STM32F4XX_H_
#define DATABUS_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <mutex>
#include "stm32f4xx.h"
#include "gpio_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
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
 // namespace stm32f4xx
}// namespace microhal

#endif // DATABUS_STM32F4XX_H_
