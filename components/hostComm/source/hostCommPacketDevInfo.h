/*
 * hostCommPacketDevInfo.h
 *
 *  Created on: 23 wrz 2015
 *      Author: Pawel
 */

#ifndef HOSTCOMMPACKETDEVINFO_H_
#define HOSTCOMMPACKETDEVINFO_H_

#include "hostComm.h"
#include "hostCommPacket.h"

namespace microhal {

class DeviceInfoPacket : private HostCommPacket {
 public:
    static constexpr const uint8_t PacketType = HostCommPacket::DEVICE_INFO;
    static constexpr const uint8_t Request = HostCommPacket::DEVICE_INFO_REQUEST;

    DeviceInfoPacket(uint64_t manufacturerID, uint32_t productID, uint64_t deviceSerialNumber = 0)
        : HostCommPacket(this, sizeof(*this), PacketType, true, true),
          manufacturerID(manufacturerID),
          productID(productID),
          deviceSerialNumber(deviceSerialNumber) {}

 private:
    uint64_t manufacturerID;
    uint64_t deviceSerialNumber;
    uint32_t productID = 0x0;
    const uint16_t maxPacketDataSize = HostCommPacket::maxPacketDataSize;
    const uint8_t hostCommVersion = HostComm::version;
    char deviceName[20] = "Unnamed device.";
};
static_assert(sizeof(DeviceInfoPacket) <= HostCommPacket::maxPacketDataSize,
              "HostComm max packet size has to be greater than size of DeviceInfoPacket");

} /* namespace microhal */

#endif /* HOSTCOMMPACKETDEVINFO_H_ */
