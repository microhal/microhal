/*
 * IODeviceNull.h
 *
 *  Created on: 7 pa� 2014
 *      Author: Dell
 */

#ifndef _MICROHAL_IODEVICENULL_H_
#define _MICROHAL_IODEVICENULL_H_

#include "../IODevice.h"

namespace microhal {

class IODeviceNull: public microhal::IODevice {
public:
    IODeviceNull() {}

    ~IODeviceNull() {}

    bool open(OpenMode mode __attribute__ ((unused))) noexcept final { return false; }
    /**
     *
     */
    void close() noexcept final {}
    /**
     *
     * @return
     */
    bool isOpen() const noexcept final { return false; }

    /**
     *
     * @param[out] buffer
     * @param[in] length
     * @retval
     */
    size_t read(char *buffer __attribute__ ((unused)), size_t length __attribute__ ((unused))) noexcept final { return 0; }
    /**
     *
     */
    size_t availableBytes(void) const noexcept final { return 0; }
    /**
     *
     * @param[out] buffer
     * @param[in] maxLength
     * @return
     */
//    size_t readLine(char *buffer __attribute__ ((unused)), size_t maxLength __attribute__ ((unused))) noexcept final {
//        return 0;
//    }
    /**
     *
     * @return
     */
    bool getChar(char &c __attribute__ ((unused))) noexcept final { return false; }
    /**
     *
     * @param[in] c
     * @return
     */
    bool putChar(char c __attribute__ ((unused))) noexcept final { return true; }
    /**
     *
     * @param[in] data - pointer to data buffer
     * @param[in] length - length of data to write
     * @return number of bytes that was copy to buffer.
     */
    size_t write(const char *data __attribute__ ((unused)), size_t length __attribute__ ((unused))) noexcept final { return length; }

};

extern IODeviceNull nullIODevice;

}  // namespace microhal

#endif  // IODEVICENULL_H_
