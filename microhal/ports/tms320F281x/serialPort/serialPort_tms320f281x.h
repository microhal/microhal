/*
 * serialPort_tms320f281x.h
 *
 *  Created on: Feb 17, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_TMS320F281X_SERIALPORT_TMS320F281X_H_
#define SRC_MICROHAL_PORTS_TMS320F281X_SERIALPORT_TMS320F281X_H_

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#define noexcept

#include <cstdint>
#include "interfaces/serialPort_interface.h"
#include "microhalPortConfig_tms320f281x.h"

namespace microhal {
namespace tms320f281x {

/* **************************************************************************************************************************************************
 * CLASS
 */
class SerialPort : public microhal::SerialPort {
 public:
    bool open(OpenMode mode) noexcept override = 0;
    bool isOpen(void) const noexcept final {}
    void close() noexcept final {}

    bool setBaudRate(uint32_t baudRate) noexcept final;
    uint32_t getBaudRate() const noexcept final;
    bool setParity(SerialPort::Parity parity) noexcept final;
    bool setStopBits(SerialPort::StopBits stopBits) noexcept final;
    bool setDataBits(SerialPort::DataBits dataBits) noexcept final;

    void priority(uint32_t priority) {}

    //bool SerialPort::open(microhal::IODevice::OpenMode);
   // size_t SerialPort::read(char *, size_t, std::chrono::milliseconds);
   // size_t SerialPort::inputQueueSize() const ;
   // size_t SerialPort::outputQueueSize() const ;
   // size_t SerialPort::availableBytes() const ;
  //  bool SerialPort::waitForWriteFinish(std::chrono::milliseconds) const ;
   // bool SerialPort::clear(microhal::SerialPort::Direction);
  //  size_t write(const char *, size_t);

// protected:
    //------------------------------------------- variables -----------------------------------------//

//------------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SerialPort() noexcept
        {
    }

    void enableInterrupt(uint32_t priority) {
    }

    uint32_t number() {
        std::terminate();
    }
};

}
}


#endif /* SRC_MICROHAL_PORTS_TMS320F281X_SERIALPORT_TMS320F281X_H_ */
