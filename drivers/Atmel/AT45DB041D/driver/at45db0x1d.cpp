/* =========================================================================================== */ /**
 @file        at45db0xxd.c
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       AT45DB0xxD driver
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details     Implementation of the AT45DB0XXD

 */ /* ============================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        */
/*****************************************************************************
 * INCLUDES
 */
#include "at45db0x1d.h"

/**
 * \brief This function initialize GPIO and SPI interface.
 *
 * \retval false if an error occurred.
 */
bool AT45DB0X1D::init(void) {
    ManufacturerID id;
    bool ret = true;

    // check manufacturer ID, AT45 driver will automatically distinguish the chip between supported ones
    if (manufacturerID(id) == true) {
        if (id == ManufacturerID::AT45DB041) {
            if (pagesCount != at45db041PagesCount) return false;
        } else if (id == ManufacturerID::AT45DB081) {
            if (pagesCount != at45db081PagesCount) return false;
        } else {
            return false;
        }
        // checking and configuring the page size
        if (!isPageSize256()) {
            if (pageSize != PageSize::Size_264) return false;
        }
        // checking and configuring memory protection
        if (isMemoryProtected()) {
            // protection enabled, disabling protection
            ret &= disableSectorProtection();
        }

        return ret;
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
bool AT45DB0X1D::pageErase(uint16_t pageAddress) {
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
bool AT45DB0X1D::bufferWrite(BufferNumber bufferNumber, uint16_t address, const void *src, uint16_t len) {
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
bool AT45DB0X1D::bufferRead(BufferNumber bufferNumber, uint16_t address, void *dst, uint16_t len) {
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
bool AT45DB0X1D::bufferToMainMemoryWithErase(BufferNumber bufferNumber, uint16_t pageAddress) {
    if (pageAddress >= pagesCount) return false;

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
bool AT45DB0X1D::bufferToMainMemoryWithoutErase(BufferNumber bufferNumber, uint16_t pageAddress) {
    if (pageAddress >= pagesCount) return false;

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
bool AT45DB0X1D::mainMemoryPageToBuffer(BufferNumber bufferNumber, uint16_t pageAddress) {
    if (pageAddress >= pagesCount) return false;
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
bool AT45DB0X1D::mainMemoryPageReadDirect(uint16_t pageAddress, uint16_t byteAddress, void *dst, uint16_t len) {
    if (pageAddress >= pagesCount) return false;
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
bool AT45DB0X1D::continuousArrayRead(uint16_t pageAddress, uint16_t byteAddress, void *dst, uint16_t len) {
    if (pageAddress >= pagesCount) return false;
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
bool AT45DB0X1D::blockErase(uint16_t blockAddress) {
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
bool AT45DB0X1D::sectorErase(uint8_t sectorAddress) {
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
bool AT45DB0X1D::chipErase(void) {
    uint8_t buffer[4] = {0xc7, 0x94, 0x80, 0x9a};
    return writeBuffer(buffer, sizeof(buffer), true);
}

/**
 *@brief This function enter flash device into Deep-Power down mode
 *@brief Current consumption is equal about 15uA
 *@brief Flash do not respond to any command.
 *
 *@param		none
 *
 *@return
 */
void AT45DB0X1D::DeepPowerDownEnter(void) {
    constexpr uint8_t opcodeUltra = 0xb9;
    write(opcodeUltra);
}
/**
 *@brief Wakes up from deep power down mode
 *
 *@param		none
 *
 *@return
 */
void AT45DB0X1D::DeepPowerDownWake(void) {
    constexpr uint8_t opcodeUltra = 0xb9;
    write(opcodeUltra);
}
/**
 *@brief Enters into ultra deep sleep power mode
 *@brief Works only witch AT45DB0XXE(last letter is important)
 *@brief Current consumption 400nA
 *
 *@param		none
 *
 *@return
 */
void AT45DB0X1D::ultraDeepPowerDownEnter(void) {
    constexpr uint8_t opcodeUltra = 0x79;
    write(opcodeUltra);
}
/**
 *@brief
 *
 *@param		none
 *
 *@return
 */
void AT45DB0X1D::ultraDeepPowerDownWake(void) {
    write(0);
}
