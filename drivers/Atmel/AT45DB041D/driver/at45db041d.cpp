/* =========================================================================================== */ /**
 @file        at45db041d.c
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       AT45DB041D driver
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details     Implementation of the AT45DB041D

 */ /* ============================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               */
/*****************************************************************************
 * INCLUDES
 */
#include "at45db041d.h"

/**
 * \brief This function initialize GPIO and SPI interface.
 *
 * \retval false if an error occurred.
 */
bool AT45DB041D::init(void) {
    uint16_t id;
    bool ret = true;

    if (manufacturerID(id) == true) {
        if (id == static_cast<uint16_t>(Hardware::Manufacturer_ID)) {
            // checking and configuring the page size
            if (isPageSize256()) {
                if (pageSize == PageSize::Size_264) {
                    return false;
                }
            } else {
                if (pageSize == PageSize::Size_256) {
                    ret &= switchToPowerOf2PageSize();
                }
            }
            // checking and configuring memory protection
            if (isMemoryProtected()) {
                // protection enabled, disabling protection
                ret &= disableSectorProtection();
            }

            return ret;
        }
    }
    return false;
}

/**
 *@brief	This function erase page.
 *
 *@param	pageAddress	address of page to erase.
 *
 *\retval	true if sector was protected
 *\retval	false if an error occurred
 */
bool AT45DB041D::pageErase(uint16_t pageAddress) {
    uint8_t tmpBuffer[4];

    tmpBuffer[0] = 0x81;  // page erase command
    tmpBuffer[1] = (uint8_t)(pageAddress >> (8 - Shift));
    tmpBuffer[2] = (uint8_t)(pageAddress << Shift);
    tmpBuffer[3] = 0;

    return writeBuffer(tmpBuffer, sizeof(tmpBuffer));  // write command
}
/**
 *@brief	This function copy data from src buffer to SRAM buffer in ad45db memory.
 *
 *@param    bufferNumber - buffer number
 *@param	address - address in buffer where data will be copied
 *@param	*src	- pointer to source data
 *@param	len		- number of bytes to be written
 *
 *\retval	true if sector was protected
 *\retval	false if an error occurred
 */
bool AT45DB041D::bufferWrite(BufferNumber bufferNumber, uint16_t address, const void *src, uint16_t len) {
    uint8_t tmpBuffer[4];

    if (len + address > static_cast<uint16_t>(pageSize)) {
        return false;
    }

    if (bufferNumber == BufferNumber::Buffer1) {
        tmpBuffer[0] = 0x84;  // operation code -> write to first buffer
    } else if (bufferNumber == BufferNumber::Buffer2) {
        tmpBuffer[0] = 0x87;  // operation code -> write to second buffer
    } else {
        return false;
    }

    tmpBuffer[1] = 0x00;                     // must be 0x00
    tmpBuffer[2] = (uint8_t)(address >> 8);  // address in buffer (MSB)
    tmpBuffer[3] = (uint8_t)(address);       // address in buffer

    if (writeBuffer(tmpBuffer, sizeof(tmpBuffer), false) == true) {  // write config data
        return writeBuffer((uint8_t *)src, len);                     // write buffer
    }

    return false;
}
/**
 *@brief	read data from buffer
 *
 *@param	bufferNumber - buffer number
 *@param	address	- address of first byte to read from buffer
 *@param	*dst	- pointer to buffer where data will be stored
 *@param	len		- number of bytes to read
 *
 *\retval	true if reading was successful
 *\retval	false if an error occurred
 */
bool AT45DB041D::bufferRead(BufferNumber bufferNumber, uint16_t address, void *dst, uint16_t len) {
    uint8_t tmpBuffer[4];

    // dbg_writeTxt("reading from buffer\n\r");

    if (bufferNumber == BufferNumber::Buffer1) {
        tmpBuffer[0] = 0xD1;  // operation code -> read from first buffer
    } else {
        tmpBuffer[0] = 0xD3;  // operation code -> read from second buffer
    }

    tmpBuffer[1] = 0x00;                     // must be 0x00
    tmpBuffer[2] = (uint8_t)(address >> 8);  // address in buffer (MSB)
    tmpBuffer[3] = (uint8_t)(address);       // address in buffer

    if (SPIDevice::writeBuffer(tmpBuffer, sizeof(tmpBuffer), false) == true) {
        return SPIDevice::readBuffer((uint8_t *)dst, len);
    }
    return false;
}
/**
 *@brief	writing data from buffer to flash memory with previous page erase
 *
 *@param	bufferNumber - buffer number
 *@param	pageAddress - address of page in flash memory
 *
 *\retval	true if data was successfully written
 *\retval	false if an error occurred
 */
bool AT45DB041D::bufferToMainMemoryWithErase(BufferNumber bufferNumber, uint16_t pageAddress) {
    if (pageAddress >= 2048) return false;

    uint8_t tmpBuffer[4];
    memset(tmpBuffer, 0x00, 4);

    if (bufferNumber == BufferNumber::Buffer1) {
        tmpBuffer[0] = 0x83;  // operation code for first buffer
    } else {
        tmpBuffer[0] = 0x86;  // operation code for second buffer
    }

    tmpBuffer[1] = (pageAddress >> (8 - Shift));
    tmpBuffer[2] = (pageAddress << Shift);

    if (!busyWait()) {
        return false;
    }
    if (!pageErase(pageAddress)) return false;
    if (!busyWait()) {
        return false;
    }

    return writeBuffer(tmpBuffer, sizeof(tmpBuffer));
}
/**
 *@brief	writing data from buffer to flash memory without previous page erase
 *
 *@param	bufferNumber - buffer number
 *@param	pageAddress - address of page in flash memory
 *
 *\retval	true if data was successfully written
 *\retval	false if an error occurred
 */
bool AT45DB041D::bufferToMainMemoryWithoutErase(BufferNumber bufferNumber, uint16_t pageAddress) {
    if (pageAddress >= 2048) return false;

    uint8_t tmpBuffer[4];

    if (bufferNumber == BufferNumber::Buffer1) {
        tmpBuffer[0] = 0x88;  // operation code for first buffer
    } else {
        tmpBuffer[0] = 0x89;  // operation code for second buffer
    }

    tmpBuffer[0] = 0x03;  // operation code
    tmpBuffer[1] = (pageAddress >> (8 - Shift));
    tmpBuffer[2] = (pageAddress << Shift);

    return writeBuffer(tmpBuffer, sizeof(tmpBuffer));
}

/**
 *@brief
 *
 *@param		bufferNumber
 *@param		pageAddress
 *
 *@return
 */
bool AT45DB041D::mainMemoryPageToBuffer(BufferNumber bufferNumber, uint16_t pageAddress) {
    if (pageAddress >= 2048) return false;
    uint8_t tmpBuffer[4];

    if (bufferNumber == BufferNumber::Buffer1) {
        tmpBuffer[0] = 0x53;  // operation code for first buffer
    } else {
        tmpBuffer[0] = 0x55;  // operation code for second buffer
    }

    tmpBuffer[0] = 0x03;  // operation code
    tmpBuffer[1] = (pageAddress >> (8 - Shift));
    tmpBuffer[2] = (pageAddress << Shift);

    return writeBuffer(tmpBuffer, sizeof(tmpBuffer));
}
/**
 *@brief
 *
 *@param		pageAddress
 *@param		byteAddress
 *
 *@return      AT45DB_Result
 */
bool AT45DB041D::mainMemoryPageReadDirect(uint16_t pageAddress, uint16_t byteAddress, void *dst, uint16_t len) {
    if (pageAddress >= PagesCount) return false;
    if (byteAddress >= static_cast<uint16_t>(pageSize)) return false;

    uint8_t tmpBuffer[8];

    tmpBuffer[0] = 0xD2;  // operation code

    tmpBuffer[0] = 0x03;  // operation code
    tmpBuffer[1] = (pageAddress >> (8 - Shift));
    tmpBuffer[2] = (pageAddress << Shift);
    tmpBuffer[2] |= (byteAddress >> 8);
    tmpBuffer[3] = byteAddress;

    if (writeBuffer(tmpBuffer, sizeof(tmpBuffer), false) == true) {
        return SPIDevice::readBuffer((uint8_t *)dst, len);
    }
    return false;
}
/**
 *@brief	function read data from flash memory
 *
 *@param	pageAddress
 *@param	byteAddress
 *@param	*dst
 *@param	len
 *
 *@return      AT45DB_Result
 */
bool AT45DB041D::continuousArrayRead(uint16_t pageAddress, uint16_t byteAddress, void *dst, uint16_t len) {
    if (pageAddress >= 2048) return false;
    if (byteAddress > static_cast<uint16_t>(pageSize)) return false;

    uint8_t tmpBuffer[4];

    tmpBuffer[0] = 0x03;  // operation code
    tmpBuffer[1] = (pageAddress >> (8 - Shift));
    tmpBuffer[2] = (pageAddress << Shift);
    tmpBuffer[2] |= (byteAddress >> 8);
    tmpBuffer[3] = byteAddress;

    if (writeBuffer(tmpBuffer, sizeof(tmpBuffer), false) == true) {
        return SPIDevice::readBuffer((uint8_t *)dst, len);
    }
    return false;
}
/**
 *@brief
 *
 *@param		blockAddress
 *
 *@return      AT45DB_Result
 */
bool AT45DB041D::blockErase(uint16_t blockAddress) {
    (void)blockAddress;
    // todo
    return false;
}
/**
 *@brief
 *
 *@param		sectorAddress
 *
 *@return      AT45DB_Result
 */
bool AT45DB041D::sectorErase(uint8_t sectorAddress) {
    // todo
    (void)sectorAddress;
    return false;
}
/**
 *@brief
 *
 *@param		none
 *
 *@return      AT45DB_Result
 */
bool AT45DB041D::chipErase(void) {
    uint8_t buffer[4] = {0xc7, 0x94, 0x80, 0x9a};
    return writeBuffer(buffer, sizeof(buffer), true);
}
