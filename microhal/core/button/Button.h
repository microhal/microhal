/*
 * Button.h
 *
 *  Created on: 3 kwi 2014
 *      Author: pawel
 */

#ifndef BUTTON_H_
#define BUTTON_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "gpio.h"
#include "signalSlot/signalSlot.h"

namespace microhal {
/**
 * @addtogroup HAL Interfaces
 * @{
 * @class Button
 * @}
 * \brief   This class provides Button functions.
 */
/* ************************************************************************************************
 * CLASS
 */
class Button: private microhal::GPIO {
public:
    typedef enum {
        ACTIVE_LOW = 0x00, ACTIVE_HIGH = 0xFF
    } ActiveState;

    Button() = delete;

    Button(const GPIO::IOPin pin, const ActiveState activeState = ACTIVE_LOW) __attribute__((always_inline)) :
    GPIO(pin, GPIO::Direction::Input, GPIO::PullUp), activeState(activeState), debouncer(0xAA) {
        if (first == nullptr) {
            first = this;
        } else {
            Button *ptr = first;
            while (ptr->next != nullptr) {
                ptr = ptr->next;
            }
            ptr->next = this;
        }
    }

    bool isPressed() {
        return (debouncer == activeState);
    }

    Signal<void> onPressed;
private:
    static Button *first;
    Button *next = nullptr;

    const uint8_t activeState;
    uint8_t debouncer;
    bool signalEmited = false;
public:
    static void timeProc() {
        Button *active = first;

        while (active != nullptr) {
            //shift samples
            active->debouncer <<= 1;
            //if button pressed add 1 to last position;
            if (active->get()) {
                active->debouncer |= 0x01;
            }
            if (active->isPressed()) {
                if(active->signalEmited == false) {
                    active->signalEmited = true;
                    active->onPressed.emit();
                }
            }else{
                active->signalEmited = false;
            }
            active = active->next;
        }
    }
};

}
 // namespace microhal

#endif /* BUTTON_H_ */
