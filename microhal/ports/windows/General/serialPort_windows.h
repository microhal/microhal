/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      Serial port for Windows

 @authors    Pawel
 created on: 11-08-2015
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 in the documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *//* ========================================================================================================================== */
#ifndef SERIALPORTWINDOWS_H_
#define SERIALPORTWINDOWS_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "interfaces/serialPort_interface.h"
#include <stdint.h>
#include <iostream>

#include <windows.h>

namespace microhal {
namespace windows {
/* ************************************************************************************************
 * CLASS
 */
class SerialPort: public microhal::SerialPort {
    public:/**
     *  \brief Open serial port and set default parameters
     *  
     *  \param [in] mode &ref OpenMode - direction of operations
     *  \return true if everything was OK, false otherwise
     *  
     */
    bool open(OpenMode mode) noexcept final {
    	bool result = false;
    	do {
    		if(true == isOpen()) {
    			break;
    		}

    		DWORD openFlag = 0;

    		switch (mode) {
				case ReadOnly:	openFlag = GENERIC_READ; break;
				case WriteOnly:	openFlag = GENERIC_WRITE; break;
				case ReadWrite:	openFlag = GENERIC_WRITE | GENERIC_READ; break;
				default:
					break;//unsupported mode
    		}

            hSerial = CreateFileA ((LPCSTR)portName, openFlag, 0, NULL,
                    OPEN_EXISTING, 0, NULL);

            if(hSerial == INVALID_HANDLE_VALUE) {
            	break;
            }

            if(false == SetCommState(hSerial, &dcbSerialParams)) {
            	volatile DWORD error = GetLastError();
            	std::cout << "ERROR: " << error << "\n\r";
            	break;
            }
            COMMTIMEOUTS timeouts;

            timeouts.ReadIntervalTimeout = 1;
            timeouts.ReadTotalTimeoutMultiplier = 1;
            timeouts.ReadTotalTimeoutConstant = 1;
            timeouts.WriteTotalTimeoutMultiplier = 1;
            timeouts.WriteTotalTimeoutConstant = 1;
            if (false == SetCommTimeouts(hSerial, &timeouts)) {
            	break;
            }

            result = true;
    	} while(0);
    	return result;
    }
    /**
     * @brief This function check is serial port open. When port is open function return true in other cases return false;
     *
     * @retval true if port is open
     * @retval false if port is close
     */
    bool isOpen() const noexcept final {
        if(hSerial != INVALID_HANDLE_VALUE) {
    		return true;
    	}
        return false;
    }
    /**
     * @brief This function close serial port
     */
    void close() noexcept final {
        if(hSerial != INVALID_HANDLE_VALUE) {
        	CloseHandle(hSerial);
        }
        hSerial = INVALID_HANDLE_VALUE;
    }

    /**
     *  \brief Read signel character from serial port
     *  
     *  \param [in] c reference to char, where read value will be stored
     *  \return true if everything was OK, false otherwise
     *  
     */
    bool getChar(char &c) noexcept final {
    	bool result = false;
    	do {
    		if(!isOpen()) {
    			break;
    		}

            size_t dwBytesRead;
            if(false == ReadFile(hSerial, &c, 1, (LPDWORD)&dwBytesRead, NULL)) {
            	break;
            }
            if(1 != dwBytesRead) {
            	break;
            }

            result = true;
    	} while(0);
    	return result;
    }

    /**
     *  \brief Send single character via serial port
     *  
     *  \param [in] c Character to send
     *  \return true if everything was OK, false otherwise
     *  
     *  \details Details
     */
    bool putChar(char c) noexcept final {
    	bool result = false;
    	do {
    		if(!isOpen()) {
    			break;
    		}

            size_t dwBytesWrite;
            if(false == WriteFile(hSerial, &c, 1, (LPDWORD)&dwBytesWrite, NULL)) {
            	break;
            }
            if(1 != dwBytesWrite) {
            	break;
            }

            result = true;
    	} while(0);
    	return result;
    }
    /**
     *  \brief Write sequence of bytes into serial port
     *  
     *  \param [in] data pointer to data
     *  \param [in] length number of bytes of data
     *  \return number of bytes correctly writen
     *  
     */
    inline size_t write(const char *data, size_t length) noexcept final;
	
    	/**
     *  \brief Read sequence of bytes from serial port
     *  
     *  \param [in] buffer buffer where read bytes will be stored
     *  \param [in] length number of bytes to read
     *  \return number of bytes correctly read
     *  
     */
    inline size_t read(char *buffer, size_t length, std::chrono::milliseconds timeout) noexcept final;

    /**
     *  \brief Get number of avaliable bytes to read
     *  
     *  \return number of avaliable bytes to read
     *  
     */
    inline size_t availableBytes(void) const noexcept final;

    	/**
     *  \brief Set speed of serial port
     *  
     *  \param [in] baudRate Baudrate
     *  \param [in] dir interface compatibility
     *  \return true if everything was OK, false otherwise
     *  
     */
    bool setBaudRate(uint32_t baudRate) noexcept;
	
    /**
     *  \brief Get speed of serial port 
     *  
     *  \param [in] dir interface compatibility
     *  \return Baudrate
     *  
     */
    uint32_t getBaudRate() const noexcept;

    /**
     *  \brief Set parity
     *  
     *  \param [in] parity @ref Parity
     *  \return true if everything was OK, false otherwise
     *  
     */
    bool setParity(SerialPort::Parity parity) noexcept final;
    /**
     *  \brief Set number of stop bits
     *  
     *  \param [in] stopBits @ref StopBits
     *  \return true if everything was OK, false otherwise
     *  
     */	
    bool setStopBits(SerialPort::StopBits stopBits) noexcept final;
    /**
     *  \brief Set number of data bytes in single transfer 
     *  
     *  \param [in] dataBits @ref DataBits
     *  \return true if everything was OK, false otherwise
     *  
     */
    bool setDataBits(SerialPort::DataBits dataBits) noexcept final;

    	/**
     *  \brief Get size of input buffer
     *  
     *  \return size of input buffer
     *  
     */
    size_t inputQueueSize() const noexcept final {
    	COMMPROP commProp;

        if(hSerial != INVALID_HANDLE_VALUE) {
        	if(GetCommProperties(hSerial, &commProp)) {
        		if(0 == commProp.dwMaxRxQueue) { // not determined by driver
        			return 0xffffffff;
        		} else {
        			return commProp.dwMaxRxQueue;
        		}
        	}
        }

        return 0xffffffff;
    }
    	/**
     *  \brief Get size of output buffer
     *  
     *  \return size of output buffer
     *  
     */
    size_t outputQueueSize() const noexcept final {
    	COMMPROP commProp;

        if(hSerial != INVALID_HANDLE_VALUE) {
        	if(GetCommProperties(hSerial, &commProp)) {
        		if(0 == commProp.dwMaxTxQueue) { // not determined by driver
        			return 0xffffffff;
        		} else {
        			return commProp.dwMaxTxQueue;
        		}
        	}
        }

        return 0xffffffff;
    }

    bool waitForWriteFinish(std::chrono::milliseconds timeout) const noexcept final { // not used in Nonoverlapped mode
       	return true;
    }

    bool waitForReadyRead(int msecs) { // not used in Nonoverlapped mode
    	return true;
    }

	/**
     *  \brief Flush write buffer - force write
     *  
     */
    void flush(){
    	if(isOpen())
    	{
    		FlushFileBuffers(hSerial);
    	}
    }

	 /**
     *  \brief Clear serial port buffers
     *  
     *  \param [in] dir @ref Direction determine which buffer should be cleared
     *  \return true if everything was OK, false otherwise
     *  
     */
    bool clear(SerialPort::Direction dir = AllDirections) noexcept {
		DWORD clearFlag = 0;

		switch (dir) {
			case Input:	clearFlag = PURGE_RXABORT | PURGE_RXCLEAR; break;
			case Output: clearFlag = PURGE_TXABORT| PURGE_TXCLEAR; break;
			case AllDirections:	clearFlag = PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT| PURGE_TXCLEAR; break;
			default:
				break;//unsupported mode
		}
		if(isOpen()) {
			return PurgeComm(hSerial, clearFlag);
		}
		return false;
    }

     /**
     *  \brief Set serial port name
     *  
     *  \param [in] portName Port name
     *  \return true if everything was OK, false otherwise
     *  
     */
    bool setPortName(const char *portName){
		if(portName != nullptr){
			this->portName = (LPCWSTR) portName;
			return true;
		}
        return false;
    }

    //------------------------------------------- constructors --------------------------------------//
    SerialPort(const char * name) :
            portName((LPCWSTR)name), hSerial(INVALID_HANDLE_VALUE) {
        dcbSerialParams.DCBlength = sizeof(DCB);
        dcbSerialParams.BaudRate = 9600;
        dcbSerialParams.fParity = FALSE;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.StopBits = ONESTOPBIT;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
        dcbSerialParams.fRtsControl = DTR_CONTROL_DISABLE;
        dcbSerialParams.fOutxCtsFlow = FALSE;
        dcbSerialParams.fOutxDsrFlow = FALSE;
        dcbSerialParams.fDsrSensitivity = FALSE;
        dcbSerialParams.fAbortOnError = FALSE;
        dcbSerialParams.fOutX = FALSE;
        dcbSerialParams.fInX = FALSE;
        dcbSerialParams.fErrorChar = FALSE;
        dcbSerialParams.fNull = FALSE;
    }
    ~SerialPort() {
    }
private:
//------------------------------------------- variables -----------------------------------------//
    LPCWSTR portName;
    HANDLE hSerial;      /* Handle to Serial Port */
    DCB dcbSerialParams;
};
/* ************************************************************************************************
 * INLINE FUNCTIONS
 */

size_t SerialPort::availableBytes(void) const noexcept {
	//COMMPROP commProp;
	COMSTAT status;
	DWORD errors;
    if(hSerial != INVALID_HANDLE_VALUE) {
//    	if(GetCommProperties(hSerial, &commProp)) {
//    		return commProp.dwCurrentRxQueue;
//    	}
		if (0 != ClearCommError(hSerial,&errors,&status)) {
			return status.cbInQue;
		}
    }

    return 0;
}

size_t SerialPort::write(const char *data, size_t length) noexcept {
    size_t dwBytesRead = 0;
    if(hSerial != INVALID_HANDLE_VALUE) {
    	WriteFile(hSerial, data, length, (LPDWORD)&dwBytesRead, NULL);
    }
	return dwBytesRead;
}

size_t SerialPort::read(char *buffer, size_t length, std::chrono::milliseconds timeout) noexcept {
    size_t dwBytesRead = 0;

    if(hSerial != INVALID_HANDLE_VALUE) {
    	ReadFile(hSerial, buffer, length, (LPDWORD)&dwBytesRead, NULL);
    }
	return dwBytesRead;
}

} // namespace windows
} // namespace microhal

#endif /* SERIALPORTLINUX_H_ */
