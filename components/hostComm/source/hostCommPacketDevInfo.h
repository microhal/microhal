/*
 * hostCommPacketDevInfo.h
 *
 *  Created on: 23 wrz 2015
 *      Author: Pawel
 */

#ifndef HOSTCOMMPACKETDEVINFO_H_
#define HOSTCOMMPACKETDEVINFO_H_

#include "hostCommPacket.h"

namespace microhal {

//class DeviceInfoPacket : private HostCommPacket {
//	//static_assert(sizeof(DeviceInfoPacket) <= HostCommPacket::maxPacketDataSize, "HostComm max packet size has to be greater than size of DeviceInfoPacket");
//public:
//	static constexpr uint8_t PacketType = HostCommPacket::DEVICE_INFO;
//	static constexpr uint8_t Request = HostCommPacket::DEVICE_INFO_REQUEST;
//
//	DeviceInfoPacket(uint64_t manufacturerID, uint32_t productID, uint64_t deviceSerialNumber = 0)
//		: HostCommPacket(this, sizeof(*this), PacketType, true, true) {
//	}
//private:
//	uint8_t hostCommVersion ;//= HostComm::version;
//	uint16_t maxPacketDataSize;// = HostCommPacket::maxPacketDataSize;
//	uint64_t manufacturerID = 0x0;
//	uint32_t productID = 0x0;
//	uint64_t deviceSerialNumber = 0;
//	char deviceName[20] = "Unnamed device.";
//};

} /* namespace microhal */

#endif /* HOSTCOMMPACKETDEVINFO_H_ */
