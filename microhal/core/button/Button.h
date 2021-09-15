/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      Button implementation
 *
 * @authors    Pawel Okas
 * created on: 3-04-2014
 *
 * @copyright Copyright (c) 2014-2021, Pawel Okas
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
class Button {
    enum class Flag : uint8_t { Pressed = 0b1, SignalEmited = 0b10, ActiveHigh = 0b100 };

 public:
    typedef enum { ACTIVE_LOW = 0x00, ACTIVE_HIGH = 0xFF } ActiveState;

    Button() = delete;

    Button(GPIO &gpio, ActiveState activeState = ACTIVE_LOW) : onPressed(), gpio(gpio), activeState(activeState), debouncer(0xAA) {
        gpio.configureAsInput(activeState == ACTIVE_LOW ? GPIO::PullType::PullUp : GPIO::PullType::PullDown);
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

    bool isPressed() { return getFlag(Flag::Pressed); }

    Signal<void> onPressed;

 private:
    GPIO &gpio;
    static Button *first;
    Button *next = nullptr;

    const uint8_t activeState;
    uint8_t debouncer;
    Flag flags{};

    bool getFlag(Flag flag) { return static_cast<unsigned int>(flags) & static_cast<unsigned int>(flag); }
    void setFlag(Flag flag) { flags = static_cast<Flag>(static_cast<unsigned int>(flags) | static_cast<unsigned int>(flag)); }
    void clearFlag(Flag flag) { flags = static_cast<Flag>(static_cast<unsigned int>(flags) & ~static_cast<unsigned int>(flag)); }

 public:
    static void timeProc() {
        Button *active = first;

        while (active != nullptr) {
            if (const auto state = active->gpio.get()) {
                // shift samples
                active->debouncer <<= 1;
                // if button pressed add 1 to last position;
                if (state.value() == GPIO::State::High) {
                    active->debouncer |= 0x01;
                }
                if (active->debouncer == active->activeState) active->setFlag(Flag::Pressed);
                if (active->debouncer == uint8_t(~active->activeState)) active->clearFlag(Flag::Pressed);
                if (active->isPressed()) {
                    if (active->getFlag(Flag::SignalEmited) == false) {
                        active->setFlag(Flag::SignalEmited);

                        active->onPressed.emit();
                    }
                } else {
                    active->clearFlag(Flag::SignalEmited);
                }
            }
            active = active->next;
        }
    }
};

}  // namespace microhal

#endif /* BUTTON_H_ */
