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

#include "mainMenu.h"
#include "IODevice.h"

#include <list>

namespace microhal {

void MainMenu::goBack(int count){
  for(int c = 0 ; c < count ; ++c){
    if(activeMenu.size() > 1){
      activeMenu.pop_back();
    } else
      break;
  }
}

void MainMenu::processCommand(std::list<char*>& words) {

  int visitedFolders = 0;
  SubMenu* activeSubMenu;	// active SubMenu

  while (!words.empty()) {
    /* Searching the tree, getting words from string */
    activeSubMenu = activeMenu.back();
    /* SubMenu branches searching */

    if (!strcmp("exit", words.front())) {
      /* Returning to root folder */
      while(activeMenu.size() > 1)
        activeMenu.pop_back();
      return;
    }
    if(!strcmp("..", words.front())) {
      /* Switching menu */
      if(activeMenu.size() > 1)
        activeMenu.pop_back();
      return;
    }
    if (!strcmp("help", words.front())) {
      /* There is a compare match, switching active menu */
      port.write("\n");
      port.write(activeSubMenu->help);
      goBack(visitedFolders);
      return;
    }

    for (std::list<MenuItem*>::iterator it = activeSubMenu->items.begin();
        it != activeSubMenu->items.end(); ++it) {
      if (!strcmp((*it)->name, words.front())) {
        /* There is a compare match, switching active menu */
        words.pop_front();
        if ((*it)->hasChildrens()) {
          ++visitedFolders;
          activeMenu.push_back(static_cast<SubMenu*>(*it));
        } else {
          (*it)->command(words, port);
          goBack(visitedFolders);  // after visiting menu going to last visited folder
          return;
        }
        break;
      }

      if (it
          == (--(std::list<MenuItem*>::iterator) activeSubMenu->items.end())) {
        port.write("\tno such command...");
        goBack(visitedFolders);
        return;
      }
    }
  }
}

int MainMenu::showCommands(std::list<char*> &words, int maxAppend) {

  SubMenu* pSubMenu = activeMenu.back();
  std::list<char*>::iterator itLastWord = words.end();
  --itLastWord;

  bool foundCommand = false;

  /* Just show commands */
  if (words.empty()) {
    for (std::list<MenuItem*>::iterator it = pSubMenu->items.begin(); it != pSubMenu->items.end(); ++it) {
      port.write("\n\t");
      port.write((*it)->name);
    }
    return -1;
  } else if (pSubMenu->hasChildrens() && (words.size() > 1)) {
    /* Searching catalogs  */
    for (auto word = words.begin(); word != itLastWord; ++word) {
      /* Checking whether the element is SubMenu or MenuItem */
      foundCommand = false;
      for (std::list<MenuItem*>::iterator it = pSubMenu->items.begin(); it != pSubMenu->items.end(); ++it) {

        if (!strcmp((*it)->name, (*word))) {
          foundCommand = true;
          pSubMenu = static_cast<SubMenu*>(*it);
          if (!pSubMenu->hasChildrens()) {
            port.write("\tit is a function, not a subfolder...");
            return -1;
          }
          break;
        }
      }
      if (!foundCommand) {
        port.write("\tno such subfolder...");
        return -1;
      }
    }
  }
  /* Processing and appending letters, pSubMenu is the current subfolder */
  /* Counting how many candidates has the same prefix */
  int samePrefixCnt = 0;
  int appendedChars;
  for (std::list<MenuItem*>::iterator it = pSubMenu->items.begin(); it != pSubMenu->items.end(); it++) {
    if (!strncmp(*itLastWord, (*it)->name, strlen(*itLastWord))) {
      ++samePrefixCnt;
    }
  }
  /* Show candidates with the same prefix */
  for (std::list<MenuItem*>::iterator it = pSubMenu->items.begin(); it != pSubMenu->items.end(); ++it) {
    if (!strncmp(*itLastWord, (*it)->name, strlen(*itLastWord))) {
      if (1 == samePrefixCnt) {
        /* Append letters */
        appendedChars = strlen((*it)->name) - strlen(*itLastWord);
        strncpy((*itLastWord), (*it)->name, maxAppend);
        return appendedChars;
      } else if (samePrefixCnt > 1) {
        port.write("\n\t\t ");
        port.write((*it)->name);
      }
    }
  }
  return -1;
}

void MainMenu::drawPrompt() {
  port.write("\n\r");
  std::list<SubMenu*>::iterator it = activeMenu.begin();
  ++it;
  for (; it != activeMenu.end(); ++it) {
    port.write("> ");
    port.write((*it)->name);
    port.write(" ");
  }
  port.write("> ");
}

}  // namespace microhal

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

