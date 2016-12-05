/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief    Example of a project that incorporates CLI module.
 *
 * @authors    Kubaszek Mateusz <mathir.km.microhal@gmail.com>
 * created on: <23-01-2016>
 * last modification: <24-01-2016>
 *
 * @copyright Copyright (c) 2015, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *        2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *           documentation and/or other materials provided with the distribution.
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

/**
 * @defgroup microHAL
 * @{
 */

/**
 * @defgroup components
 * @{
 */

/**
 * @defgroup cli
 * @{
 */

#ifndef CLI_H_
#define CLI_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "IODevice.h"
#include "mainMenu.h"

namespace microhal {

#define LINELENGTH 80
#define BUFFERLENGTH 10

/**
 * @brief Provides chars processing functionalities, buffering, etc.
 */
class CLI {
 public:
    /**
     * @brief Initializes CLI device.
     * @param port - console IODevice port.
     * @param menu - MainMenu reference.
     */
    CLI(IODevice &port, MainMenu &menu) : port(port), menu(menu), length(0), activeBuffer(0), previousBuffer(0) { init(); }

    /**
     * @brief Initializes CLI device.
     * @param port - console IODevice port.
     * @param menu - MainMenu reference.
     * @param helloTxt - hello string.
     */
    CLI(IODevice &port, MainMenu &menu, const char *helloTxt) : port(port), menu(menu), length(0), activeBuffer(0), previousBuffer(0) {
        port.write(helloTxt);
        init();
    }

    /**
     * @brief Reads chars from console and process them. Should be called cyclically (ex. every 10ms in a thread).
     */
    void readInput() {
        uint16_t tmpLen;
        char tmpBuff[3];

        tmpLen = port.read(tmpBuff, 3);

        for (uint16_t i = 0; i < tmpLen; i++) {
            addSign(tmpBuff[i]);
        }
    }

 private:
    /**
     * @brief IODevice console port.
     */
    IODevice &port;
    /**
     * @brief  MainMenu instance.
     */
    MainMenu &menu;
    /**
     * @brief Chars buffer.
     */
    char dataBuffer[BUFFERLENGTH][LINELENGTH];
    /**
     * @brief Actual line position.
     */
    uint8_t length;
    /**
     * @brief Current buffer indicator.
     */
    uint8_t activeBuffer;
    /**
     * @brief Previous buffer indicator.
     */
    uint8_t previousBuffer;

    /**
     * @brief Initializes buffer.
     */
    void init();
    /**
     * @brief Append char to current buffer line if there is enough space. Always leaves
     *        space for null termination which could be appended to last left byte.
     * @param sign - char to store
     * @return  -1 if failed, sign if succeed.
     */
    inline int charAppend(char sign);
    /**
     * @brief Delete last char in buffer. Safe deletion.
     * @param count - are you a golfer?
     * @return  Count of chars deleted.
     */
    inline unsigned int charDelete(unsigned int count);
    /**
     * @brief Draws prompt according to current path.
     */
    inline void drawPrompt();
    /**
     * @brief Deletes chars from current line from console.
     * @param count - are you a golfer?
     * @return Number of chars deleted.
     */
    inline unsigned int cleanLine(unsigned int count);

    /**
     * @brief Add and processes an added char.
     * @param sign - maybe you are a golfer?
     */
    void addSign(char sign);
    /**
     * @brief Shows previous command from history if any. Switches current buffer.
     */
    void showPreviousCommand();
    /**
     * @brief Shows next command from buffer, unless it isn't the last called command.
     *        Switches current buffer.
     */
    void showNextCommand();
    /**
     * @brief If any command in history is edited, the buffer is switched to last entry
     *        and the content is copied.
     */
    void duplicateCommand();
    /**
     * @brief Called when tab was clicked.
     */
    void showCommands();
    /**
     * @brief Called when new line was clicked.
     */
    void processBuffer();

    /**
     * @defgroup constances
     * @{
     */
    const char del = '\b';
    const char space = ' ';
    /**
     * @}
     */
};

int CLI::charAppend(char sign) {
    if (length < LINELENGTH - 1) {
        dataBuffer[previousBuffer][length++] = sign;
        return sign;
    } else if (length < LINELENGTH && sign == 0) {
        dataBuffer[previousBuffer][length++] = sign;
        return 0;
    }
    return -1;
}

unsigned int CLI::charDelete(unsigned int count) {
    unsigned int deleted = 0;
    while (length > 0 && count != deleted) {
        dataBuffer[previousBuffer][--length] = 0;
        ++deleted;
    }
    return deleted;
}

unsigned int CLI::cleanLine(unsigned int count) {
    // send delete characters
    unsigned int iter;
    for (iter = 0; iter < count; iter++) {
        port.putChar(del);
        port.putChar(space);
        port.putChar(del);
    }
    return iter;
}

void CLI::drawPrompt() {
    menu.drawPrompt();
}

}  // namespace microhal

#endif /* CLI_H_ */

/**
 * example
 * @}
 */

/**
 * cli
 * @}
 */

/**
 * components
 * @}
 */
