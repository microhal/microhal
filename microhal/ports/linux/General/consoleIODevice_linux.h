/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      linux console redirection
 *
 * @authors    Pawel
 * created on: 21-04-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, microHAL
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

#ifndef _MICROHAL_CONSOLEIODEVICE_H_
#define _MICROHAL_CONSOLEIODEVICE_H_

/* ************************************************************************************************
 * INCLUDES
 */
#include "IODevice.h"
#include <iostream>
#include <thread>
#include <atomic>

#include "buffers/cyclicBuffer.h"

namespace microhal {
namespace linux {

class console_IODevice : public IODevice {
public:
	/**
	 *  \brief Default constructor
	 */
	console_IODevice();
	
	/**
	 *  \brief Default destructor
	 */
	virtual ~console_IODevice();

	/**
	 *  \brief Open console
	 *  
	 *  \param [in] mode Direction of operation
     *  \return true if everything was OK, false otherwise
	 *  
	 *  \details When read mode is enable, function starts reading thread,
	 *  	 which is testing console input for any character - it is stored in buffer.
	 *  	 All reads are performed throught that buffer.
	 */
	bool open(OpenMode mode) noexcept final;

	/**
	 *  \brief Close console
	 *  
	 *  \details Function close console in all opened directions.
	 *  		If input direction was choose at opening, it also stops reading thread and flushes buffer.
	 */
	void close() noexcept final;

	/**
	 *  \brief Checks if console is open
	 *  
	 *  \return true if console is opened at least one direction, false otherwise
	 */
	bool isOpen() const noexcept final;

	/**
	 *  \brief Read from console
	 *  
	 *  \param [in] buffer array where characters will be written.
	 *  			User should supply array with sufficient size.
	 *  \param [in] length length of provided array
	 *  \return Number of characters that were successfully read
	 *  
	 *  \details Function reads all avaliable data from buffer (returning its number) till size
	 *  		specified by @ref length.
	 *  
	 */
	size_t read(char *buffer, size_t length) noexcept final;

	/**
	 *  \brief Get number of characters in input buffer
	 *  
	 *  \return Number of characters avaliable to read
	 *  
	 */
	size_t availableBytes(void) const noexcept final {
		return inputBuffer.getLength();
	}

	/**
	 *  \brief Read line from console
	 *  
	 *  \param [in] buffer array where characters will be written.
	 *  			User should supply array with sufficient size.
	 *  \param [in] length length of provided array
	 *  \return Number of characters that were successfully read
	 *  
	 *  \details Function reads data till 0x10 or 0x13 from buffer and writes them to provided array.
	 *  		Last character in output array is one before new line symbol. After read, new line symbol is removed from buffer.
	 *  		If both 0x10 and 0x13 exsist in sequence, function will read characters as line and remove first of symbol from buffer.
	 *  		Next call of readLine will then return 0 and second symbol will be removed.
	 */
//	size_t readLine(char *buffer, size_t maxLength) final;

	/**
	 *  \brief Read char from console
	 *  
	 *  \param [in] c reference to place where read character will be stored.
	 *  \return ture if read was successful, false otherwise
	 */
	bool getChar(char &c)  noexcept final {
		if(1 == read(&c, 1)) {
			return true;
		}
		return false;
	}

	/**
	 *  \brief Write single character to console
	 *  
	 *  \param [in] c character to write
	 *  \return ture if write was successful, false otherwise
	 */
	bool putChar(char c) noexcept final {
		if(1 == write(&c, 1)) {
			return true;
		}
		return false;
	}

	/**
	 *  \brief Write data to console
	 *  
	 *  \param [in] data array with data to write
	 *  \param [in] length number of characters to write
	 *  \return number of characters successfully written
	 */
	size_t write(const char *data, size_t length) noexcept final;

	/**
	 *  \brief Clear input buffer
	 */
	void flushInput(void) {
		//if(nullptr != inputConsole) {
			inputBuffer.flush();
		//}
	}

	/**
	 *  \brief Inject character into input buffer - for tests only
	 *  
	 *  \param [in] c character to inject
	 *  \return ture if injection was successful, false otherwise
	 */
	bool addCharToInput(char c) {
		return inputBuffer.append(c);
	}

private:
	std::atomic<bool> runReadingThread;
	std::thread readingThread;

	//HANDLE inputConsole;
	//HANDLE outputConsole;

	CyclicBuffer_data<uint8_t, 128> inputBuffer;

	void procThread(void);
	void startReadingThread(void);
	void stopReadingThread(void);

};

extern console_IODevice consoleIODev;

}  // namespace linux
}  // namespace microhal

#endif  // _MICROHAL_CONSOLEIODEVICE_H_ */
