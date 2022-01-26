/*
 * can_linux.cpp
 *
 *  Created on: May 18, 2019
 *      Author: pokas
 */

#include "can_linux.h"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;

bool operator==(const can_filter &a, const can_filter &b);

namespace microhal {
namespace linux {

canid_t convertCanId(const CAN::Message::ID &id);
can_filter makeCanFilter(const can::Filter &flt);

CAN::CAN(std::string_view canName) {
    socketHandle = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socketHandle >= 0) {
        ifreq ifr;
        strncpy(ifr.ifr_name, canName.data(), IFNAMSIZ - 1);
        if (ioctl(socketHandle, SIOCGIFINDEX, &ifr)) {
            diagChannel << MICROHAL_ERROR << "ioctl error." << endl;
        }
        // bind the socket into a CAN interface
        sockaddr_can addr;
        memset(&addr, 0, sizeof(addr));
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        if (bind(socketHandle, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) != 0) {
            diagChannel << MICROHAL_ERROR << "Unable to bind socket." << endl;
        }
        // Set the filter rules
        setsockopt(socketHandle, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
        // Set receive timeout to 0
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1;
        setsockopt(socketHandle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    } else {
        diagChannel << MICROHAL_ERROR << "Unable to open socket." << endl;
    }
}

CAN::~CAN() {
    if (socketHandle) {
        close(socketHandle);
    }
}

bool CAN::transmit(const Message &message) {
    can_frame frame;
    frame.can_id = message.getID().getID();
    if (message.isExtendedID()) {
        frame.can_id &= CAN_EFF_MASK;
        frame.can_id |= CAN_EFF_FLAG;
    } else {
        frame.can_id &= CAN_SFF_MASK;
    }
    if (message.isRemoteFrame()) frame.can_id |= CAN_RTR_FLAG;

    auto data = message.getData();
    frame.can_dlc = data.size();
    std::copy_n(data.begin(), data.size(), std::begin(frame.data));

    auto count = write(socketHandle, &frame, sizeof(frame));
    if (count < 0) diagChannel << MICROHAL_ERROR << "Unable to write data, errno: " << strerror(errno) << endl;
    if (count != sizeof(frame)) diagChannel << MICROHAL_ERROR << "Unable to write data, sent count: " << count << endl;
    return count == sizeof(frame);
}

bool CAN::receive(Message &message) {
    can_frame frame;
    auto readCount = read(socketHandle, &frame, sizeof(frame));
    if (readCount) {
        if (frame.can_id & CAN_EFF_FLAG) {
            message.setExtendedID(frame.can_id & CAN_EFF_MASK);
        } else {
            message.setStandardID(frame.can_id & CAN_SFF_MASK);
        }
        if (frame.can_id & CAN_RTR_FLAG) {
            message.setRemoteRequest(frame.can_dlc);
        } else {
            message.setDataFrame({frame.data, frame.can_dlc});
        }
        return true;
    }
    return false;
}

bool CAN::waitForTransmitFinish(std::chrono::milliseconds timeout) const noexcept {
    return true;
}

bool CAN::waitForMessage(std::chrono::milliseconds timeout) const noexcept {
    pollfd fd;
    fd.fd = socketHandle;
    fd.events = POLLIN;
    auto ret = poll(&fd, 1, timeout.count());
    return ret > 0;
}

canid_t convertCanId(const CAN::Message::ID &id) {
    canid_t socketCanId = id.getID();
    if (id.isExtended()) {
        socketCanId |= CAN_EFF_FLAG;
    } else {
        socketCanId &= CAN_SFF_MASK;
    }
    return socketCanId;
}

can_filter makeCanFilter(const can::Filter &flt) {
    can_filter filter;
    filter.can_id = convertCanId(flt.getID());
    filter.can_mask = flt.getIDMask().getID();
    if (flt.getID().isExtended()) {
        filter.can_mask |= CAN_EFF_FLAG;
    } else {
        filter.can_mask &= CAN_SFF_MASK;
    }
    if (flt.getMatchType() == can::Filter::Match::RemoteFrame) {
        filter.can_id |= CAN_RTR_FLAG;
        filter.can_mask |= CAN_RTR_FLAG;
    }
    return filter;
}

bool CAN::addFilter(const can::Filter &flt) {
    can_filter filter = makeCanFilter(flt);
    filters.push_back(filter);
    setsockopt(socketHandle, SOL_CAN_RAW, CAN_RAW_FILTER, filters.data(), sizeof(can_filter) * filters.size());
    return true;
}

bool CAN::removeFilter(const can::Filter &flt) {
    can_filter filter = makeCanFilter(flt);
    auto sizeBeforeRemove = filters.size();
    setsockopt(socketHandle, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    filters.erase(std::remove(filters.begin(), filters.end(), filter), filters.end());
    setsockopt(socketHandle, SOL_CAN_RAW, CAN_RAW_FILTER, filters.data(), sizeof(can_filter) * filters.size());
    return sizeBeforeRemove > filters.size();
}

bool CAN::setMode(CAN::Mode mode) {
    int loopback = 1;
    int recvOwnMsgs = 0;
    switch (mode) {
        case Mode::Normal:
            recvOwnMsgs = 0;
            break;
        case Mode::Loopback:
            recvOwnMsgs = 1;
            break;
    }
    setsockopt(socketHandle, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
    setsockopt(socketHandle, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recvOwnMsgs, sizeof(recvOwnMsgs));
    return true;
}

}  // namespace linux
}  // namespace microhal

bool operator==(const can_filter &a, const can_filter &b) {
    if (a.can_id != b.can_id) return false;
    if (a.can_mask != b.can_mask) return false;
    return true;
}
