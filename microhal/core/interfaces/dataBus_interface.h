/*
 * dataBus.h
 *
 *  Created on: 10 kwi 2014
 *      Author: pawel
 */

#ifndef DATABUS_INTERFACE_H_
#define DATABUS_INTERFACE_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <mutex>

namespace microhal {
/* ************************************************************************************************
 * CLASS
 */
class DataBus {
public:
    virtual void write(uint16_t data) = 0;

    virtual void read(uint16_t &data) = 0;

    virtual void lock() {
        mutex.lock();
    }

    virtual void unlock() {
        mutex.unlock();
    }

    virtual ~DataBus() { }
private:
    std::mutex mutex;
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
	DataBus():mutex() { }
};
} // namespace microhal

#endif // DATABUS_INTERFACE_H_
