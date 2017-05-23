/*
* m24C16.cpp
 *
 *  Created on: 7 cze 2016
 *      Author: pawel
 */

#include "m24c16.h"

M24C16::~M24C16() {
    // TODO Auto-generated destructor stub
}

bool M24C16::write(size_t address, const gsl::not_null<const void *> data, size_t data_size) {
    size_t dataToWrite = data_size;
    // write data chunk to align to full pages
    size_t size = 0xF - (address & 0xF) + 1;
    if (size > data_size) size = data_size;
    if (pageWrite(address, data, size) != Error::None) return false;
    dataToWrite -= size;
    // if we have more data to write
    if (dataToWrite != 0) {
        address += size;
        const uint8_t *ptr = static_cast<const uint8_t *>(data.get()) + size;
        // calculate number of pages to write
        size_t pagesToWrite = dataToWrite / pageSize;
        size_t lastPageSize = dataToWrite % pageSize;
        // write full pages
        for (size_t i = 0; i < pagesToWrite; i++) {
            // wait for finish previous write operation
            std::this_thread::sleep_for(std::chrono::milliseconds{5});
            if (pageWrite(address, ptr, pageSize) != Error::None) return false;
            address += pageSize;
            ptr += pageSize;
        }
        // wait for finish previous write operation
        std::this_thread::sleep_for(std::chrono::milliseconds{5});
        // write last page that might be empty
        if (pageWrite(address, ptr, lastPageSize) != Error::None) return false;
    }
    return true;
}
