/*
 * consoleIODevice.cpp
 *
 *  Created on: 23 cze 2015
 *      Author: Pawel
 */

#include "consoleIODevice.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
//#include <ios>
//#include <cstdio>
//#include <io.h>
//#include <fcntl.h>
namespace microhal {
namespace windows {

console_IODevice consoleIODev;

console_IODevice::console_IODevice() : runReadingThread(false), readingThread(),
		inputConsole(nullptr), outputConsole(nullptr) {

}

console_IODevice::~console_IODevice() {
	stopReadingThread();
}

bool console_IODevice::open(OpenMode mode) noexcept {

	if (isOpen()) { // should be first properly close
		return false;
	}
	AllocConsole();
	switch (mode) {
	case ReadOnly:
		inputConsole = GetStdHandle(STD_INPUT_HANDLE);
		// set to non-echo and not waiting for ENTER key
		SetConsoleMode(inputConsole, ENABLE_PROCESSED_OUTPUT);

		inputBuffer.flush();
		startReadingThread();
		break;
	case WriteOnly:
		outputConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	case ReadWrite:
		inputConsole = GetStdHandle(STD_INPUT_HANDLE);
		// set to non-echo and not waiting for ENTER key
		SetConsoleMode(inputConsole, ENABLE_PROCESSED_OUTPUT);

		inputBuffer.flush();
		startReadingThread();
		outputConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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
	inputConsole = nullptr;
	outputConsole = nullptr;

	FreeConsole();
}

bool console_IODevice::isOpen() const noexcept {
	if (nullptr != inputConsole || nullptr != outputConsole) {
		return true;
	}
	return false;
}

size_t console_IODevice::read(char *buffer, size_t length) noexcept {
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

//size_t console_IODevice::readLine(char *buffer, size_t maxLength) noexcept {
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

	DWORD bytesWritten = 0;
	if (nullptr != outputConsole) {
		char *sign = (char*) data;
		bytesWritten = length;
		while (length--) {
			putchar(*sign++);
		}
		fflush(stdout);
	}
	return (size_t) bytesWritten;
}
void console_IODevice::startReadingThread(void) {
	runReadingThread = true;
	readingThread = std::thread(&console_IODevice::procThread, this);
}
void console_IODevice::stopReadingThread(void) {
	runReadingThread = false;
	if (readingThread.joinable()) {
		readingThread.join();
	}
}
void console_IODevice::procThread(void) {

	HANDLE console;
	INPUT_RECORD buf;
	DWORD read;

	// get console handle
	console = GetStdHandle(STD_INPUT_HANDLE);
	if (console == INVALID_HANDLE_VALUE)
		// TODO error code...

	// set to non-echo and not waiting for ENTER key
	SetConsoleMode(console, ENABLE_PROCESSED_OUTPUT);

	while (runReadingThread) {
		ReadConsoleInput(console, &buf, 1, &read);
		if (buf.Event.KeyEvent.bKeyDown && buf.EventType == KEY_EVENT) {
			switch (buf.Event.KeyEvent.wVirtualKeyCode) {
			case VK_LEFT:
				inputBuffer.append(0xe0);
				inputBuffer.append(0x4b);
				break;
			case VK_RIGHT:
				inputBuffer.append(0xe0);
				inputBuffer.append(0x4d);
				break;
			case VK_UP:
				inputBuffer.append(0xe0);
				inputBuffer.append(0x48);
				break;
			case VK_DOWN:
				inputBuffer.append(0xe0);
				inputBuffer.append(0x50);
				break;
			default:
				if (buf.Event.KeyEvent.uChar.AsciiChar)
					inputBuffer.append((uint8_t) buf.Event.KeyEvent.uChar.AsciiChar);
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds { 10 });
	}
}

} /* namespace Windows */
} // namespace microhal
