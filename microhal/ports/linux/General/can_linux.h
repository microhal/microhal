/*
 * can_linux.h
 *
 *  Created on: May 18, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_CAN_LINUX_H_
#define _MICROHAL_CAN_LINUX_H_

#include <linux/can.h>
#include <string_view>
#include <vector>
#include "can/canInterface.h"
#include "can/canMessage.h"

namespace microhal {
namespace linux {

class CAN final : public can::CAN_Interface {
 public:
    using Message = can::Message;
    enum class Mode { Normal, Loopback };

    CAN(std::string_view canName);
    ~CAN();
    bool isProtocolSupported(Protocol protocol) final{};

    bool transmit(const Message &message) final;
    bool receive(Message &message) final;

    bool waitForTransmitFinish(std::chrono::milliseconds timeout) const noexcept final;

    bool waitForMessage(std::chrono::milliseconds timeout) const noexcept final;

    bool addFilter(Message::ID id, uint32_t mask, bool isRemoteFrame) final;
    bool removeFilter(Message::ID id, uint32_t mask, bool isRemoteFrame) final;
    uint32_t receiveErrorCount() const final { return receiveErrors; };
    uint32_t transmitErrorCount() const final { return transmitErrors; };

    bool setMode(CAN::Mode mode);

 private:
    int socketHandle;
    std::vector<can_filter> filters;
    uint32_t transmitErrors = 0;
    uint32_t receiveErrors = 0;
};

}  // namespace linux
}  // namespace microhal
#endif /* _MICROHAL_CAN_LINUX_H_ */
