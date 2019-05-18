#include <cstdint>

#include "serialPort_tms320f281x.h"

namespace microhal {
namespace tms320f281x {

class SerialPort_FIFO : public SerialPort {
public:
    bool open(OpenMode mode) noexcept override;
    size_t write(const char *, size_t);
    using SerialPort::write;
    size_t read(char *, size_t, std::chrono::milliseconds) override;
    size_t inputQueueSize() const;
    size_t outputQueueSize() const;
    size_t availableBytes() const;
    bool waitForWriteFinish(std::chrono::milliseconds) const;
    bool clear(microhal::SerialPort::Direction);

};

} // namespace microhal
} // namespace tms320f281x
