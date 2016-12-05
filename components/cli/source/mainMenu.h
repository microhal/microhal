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

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <stdint.h>
#include "menuItem.h"
#include "serialPort.h"
#include "string.h"
#include "subMenu.h"

#include <list>

namespace microhal {
class CLI;

/**
 * @brief Processes the text given by CLI. Implemented functions for moving through the
 *        menu tree, executing commands, command completion, and others.
 */
class MainMenu : public SubMenu {
    friend SubMenu;
    friend CLI;

 private:
    /**
     * @brief Console port.
     */
    IODevice& port;
    /**
     * @brief List indicating current position in folder tree.
     */
    std::list<SubMenu*> activeMenu;

    /**
     * @brief Explores the tree of catalogs. Go into sub-folders, executes commands. Puts
     *        text on screen as a result of its work.
     * @param words - list of words to process (order matters of course).
     */
    void processCommand(std::list<char*>& words);

    /**
     * @brief Goes count steps back to root folder. Safe.
     * @param count
     */
    void goBack(int count);
    /**
     * @brief Draws a prompt accordingly to current path.
     */
    void drawPrompt();

    /**
     * @brief Function for showing sub-folder content. Additionally word completion is done.
     * @param words - list of given words.
     * @param maxAppend - count of letters that could be added (without null termination).
     * @return  Returns:
     *          - number of appended letters (if there was just one command conforming given name)
     *          - -1 if there is no given sub-folder or there was more conforming items
     */
    int showCommands(std::list<char*>& words, int maxAppend);

 public:
    /**
     * @brief Creates a menu.
     * @param port - IODevice console port.
     */
    MainMenu(IODevice& port) : SubMenu((const char*)0, (const char*)0), port(port) { activeMenu.push_back(this); }

    /**
     * @brief Creates a menu.
     * @param port - IODevice console port.
     * @param helpText - default help text.
     */
    MainMenu(IODevice& port, const char* helpText) : SubMenu((char*)0, helpText), port(port) { activeMenu.push_back(this); }
};

} /* namespace hal */

#endif /* MAINMENU_H_ */

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
