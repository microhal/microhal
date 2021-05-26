/*
 * consoleIODevice.cpp
 *
 *  Created on: 23 cze 2015
 *      Author: Pawel
 */

#include "consoleIODevice_linux.h"
#include <fcntl.h>
#include <ncurses.h>
#include <stdio.h>
#include <cstdio>
#include <ios>

namespace microhal {
namespace linux {

console_IODevice consoleIODev;

console_IODevice::console_IODevice() {
    initscr(); /* Start curses mode 		*/
}

console_IODevice::~console_IODevice() {
    stopReadingThread();
    endwin(); /* End curses mode		  */
}

bool console_IODevice::open(OpenMode mode) noexcept {
    if (isOpen()) {  // should be first properly close
        return false;
    }

    switch (mode) {
        case ReadOnly:
            inputBuffer.flush();
            startReadingThread();
            this->mode = OpenMode::ReadOnly;
            break;
        case WriteOnly:
            this->mode = OpenMode::WriteOnly;
            break;
        case ReadWrite:
            inputBuffer.flush();
            startReadingThread();
            this->mode = OpenMode::ReadWrite;
            break;
        default:
            return false;
    }

    //		if(nullptr != outputConsole) {
    //		  // redirect unbuffered STDOUT to the console
    //		  int hConHandle = _open_osfhandle((intptr_t)outputConsole, _O_TEXT);
    //		  FILE *fp = _fdopen(hConHandle, "w");
    //		  *stdout = *fp;
    //		  setvbuf(stdout, NULL, _IONBF, 0);
    //
    //		  // redirect unbuffered STDERR to the console
    //		  hConHandle = _open_osfhandle((intptr_t)outputConsole, _O_TEXT);
    //		  fp = _fdopen( hConHandle, "w" );
    //		  *stderr = *fp;
    //		  setvbuf(stderr, NULL, _IONBF, 0);
    //
    //		  // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
    //		  std::ios::sync_with_stdio();
    //		}
    //		if(nullptr != inputConsole) {
    //		  // redirect unbuffered STDIN to the console
    //		  int hConHandle = _open_osfhandle((intptr_t)inputConsole, _O_TEXT);
    //		  FILE *fp = _fdopen( hConHandle, "r" );
    //		  *stdin = *fp;
    //		  setvbuf(stdin, NULL, _IONBF, 0);
    //		  // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
    //		  std::ios::sync_with_stdio();
    //		}
    return true;
}

void console_IODevice::close() noexcept {
    inputBuffer.flush();
    stopReadingThread();
    mode = OpenMode::NotOpen;
}

bool console_IODevice::isOpen() const noexcept {
    return mode != OpenMode::NotOpen;
}

size_t console_IODevice::read(char *buffer, size_t length) noexcept {
    if (mode == OpenMode::ReadWrite || mode == OpenMode::ReadOnly) {
        std::lock_guard<std::mutex> g(inputBufferMutex);

        size_t readDataLength = inputBuffer.getLength();
        if (readDataLength > length) {
            readDataLength = length;
        }

        size_t i;
        for (i = 0; i < readDataLength; ++i) {
            if (inputBuffer.isEmpty()) {
                break;
            }
            buffer[i] = inputBuffer.get();
        }

        return i;
    }
    return 0;
}

// size_t console_IODevice::readLine(char *buffer, size_t maxLength) {
//	size_t count = 0;
//	char single;
//
//	while (count < maxLength) {
//		if (!getChar(single)) {
//			return count;
//		}
//
//		if (single == 10 || single == 13) {
//			return count;
//		}
//		buffer[count] = single;
//		count++;
//	}
//
//	return count;
//}

size_t console_IODevice::write(const char *data, size_t length) noexcept {
    size_t bytesWritten = 0;
    if (mode == OpenMode::ReadWrite || mode == OpenMode::WriteOnly) {
        if (waddnstr(stdscr, data, length) != ERR) {
            bytesWritten = length;
            refresh();
        }
    }
    return bytesWritten;
}

void console_IODevice::startReadingThread(void) {
    readingThread = std::jthread(&console_IODevice::procThread, this);
}

void console_IODevice::stopReadingThread(void) {
    readingThread.request_stop();
}

void console_IODevice::procThread() {
    auto stoken = readingThread.get_stop_token();

    raw();                /* Line buffering disabled	*/
    keypad(stdscr, TRUE); /* We get F1, F2 etc..		*/
    noecho();             /* Don't echo() while we do getch */
    timeout(-1);

    while (!stoken.stop_requested()) {
        auto c = getch();
        if (c != ERR) {
            std::lock_guard<std::mutex> g(inputBufferMutex);
            switch (c) {
                case KEY_LEFT:
                    inputBuffer.append(0xe0);
                    inputBuffer.append(0x4b);
                    break;
                case KEY_RIGHT:
                    inputBuffer.append(0xe0);
                    inputBuffer.append(0x4d);
                    break;
                case KEY_UP:
                    inputBuffer.append(0xe0);
                    inputBuffer.append(0x48);
                    break;
                case KEY_DOWN:
                    inputBuffer.append(0xe0);
                    inputBuffer.append(0x50);
                    break;
                case KEY_ENTER:
                    inputBuffer.append('\n');
                    break;
                case KEY_BACKSPACE:
                    inputBuffer.append('\b');
                    break;
                case 3:
                    std::terminate();
                    break;
                default:
                    inputBuffer.append(c);
                    break;
            }
        }
    }
}

}  // namespace linux
}  // namespace microhal
