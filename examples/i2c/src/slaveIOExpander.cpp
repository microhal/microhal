/*
 * slaveIOExpander.cpp
 *
 *  Created on: Sep 25, 2018
 *      Author: pokas
 */

#include "slaveIOExpander.h"

// void SlaveIOExpander::prepareReadData() {}

SlaveIOExpander::span SlaveIOExpander::prepareTransmitData() {
    uint8_t data[3];

    buff[0] = data[buffPtr];
    buff[1] = data[(buffPtr + 1) % 3];
    buff[2] = data[(buffPtr + 2) % 3];
    return buff;
}

SlaveIOExpander::span SlaveIOExpander::prepareReceiveBuffer() {
    return buff;
}

void SlaveIOExpander::onReceiveFinish(size_t received) {
    for (uint8_t i = 0; i < received; i++) {
        if (buffPtr == 0) {
            if (buff[i] != direction) {
                updateDirection(buff[i]);
            }
        }
        if (buffPtr == 1) {
            if (buff[i] != pullup) {
                updatePullup(buff[i]);
            }
        }
        if (buffPtr == 2) {
            if (buff[i] != value) {
                updateValue(buff[i]);
            }
        }
    }
    buffPtr = (buffPtr + received) % sizeof(buff);
}

void SlaveIOExpander::onTransmitFinish(size_t transmitted) {
    buffPtr = (buffPtr + transmitted) % sizeof(buff);
}
