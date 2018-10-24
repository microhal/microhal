/*
 * slaveIOExpander.h
 *
 *  Created on: Sep 25, 2018
 *      Author: pokas
 */

#ifndef _SLAVEIOEXPANDER_H_
#define _SLAVEIOEXPANDER_H_

#include <cstdint>

#include "gpio.h"
#include "interfaces/i2cSlave.h"

class SlaveIOExpander : public microhal::I2CSlave {
 public:
    using GPIO = microhal::GPIO;

    SlaveIOExpander(microhal::GPIO *(&gpio)[8]) : microhal::I2CSlave(0b001010000, buff), gpio(gpio) {}
    SlaveIOExpander(const SlaveIOExpander &) = delete;
    SlaveIOExpander &operator=(const SlaveIOExpander &) = delete;

    void updateDirection(uint8_t direction) {
        for (size_t i = 0; i < 8; i++) {
            if (gpio[i]) {
                if (direction & (1 << i)) {
                    gpio[i]->setDirectionOutput(GPIO::OutputType::PushPull, pullup & (1 << i) ? GPIO::PullType::PullUp : GPIO::PullType::NoPull);
                } else {
                    gpio[i]->setDirectionInput(pullup & (1 << i) ? GPIO::PullType::PullUp : GPIO::PullType::NoPull);
                }
            }
        }
    }
    void updatePullup(uint8_t) {}
    void updateValue(uint8_t value) {
        for (size_t i = 0; i < 8; i++) {
            if (gpio[i]) {
                value &(1 << i) ? gpio[i]->set() : gpio[i]->reset();
            }
        }
    }

 private:
    uint8_t direction = {};
    uint8_t pullup = {};
    uint8_t value = {};
    uint8_t buffPtr = 0;
    uint8_t buff[3];

    microhal::GPIO *(&gpio)[8];

    span prepareReceiveBuffer() final;
    // void prepareReadData();
    span prepareTransmitData() final;
    void onReceiveFinish(size_t received) final;
    void onTransmitFinish(size_t transmitted) final;
};

#endif /* _SLAVEIOEXPANDER_H_ */
