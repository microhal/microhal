/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      hostComm class

 @authors    Pawel Okas
 created on: 13-06-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 in the documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */ /* ==========================================================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 */

#ifndef HOSTCOMM_H_
#define HOSTCOMM_H_

#include "IODevice.h"
#include "diagnostic/diagnostic.h"
#include "hostCommPacket.h"
#include "hostCommPacketACK.h"
#include "hostCommPacketDevInfo.h"
#include "microhal_semaphore.h"
#include "signalSlot/signalSlot.h"

#include <atomic>
#include <cstdint>
#include <mutex>
#include <thread>

#ifndef MICROHAL_HOSTCOMM_LOG_LEVEL
#define MICROHAL_HOSTCOMM_LOG_LEVEL Warning
#endif

namespace microhal {

class HostComm {
 public:
    static constexpr uint8_t version = 1;
    //	HostComm(IODevice &ioDevice, diagnostic::Diagnostic &log = diagnostic::diagChannel) :
    //			ioDevice(ioDevice), log(log), receivedPacket(packetBuffer, sizeof(packetBuffer)) {
    //	}

    HostComm(IODevice &ioDevice, IODevice &logDevice, const char *logHeader = "HostComm: ")
        : ioDevice(ioDevice), log(logHeader, logDevice), receivedPacket(packetBuffer, sizeof(packetBuffer)), runThread(false), runningThread() {}
    ~HostComm() { stopHostCommThread(); }
    bool send(HostCommPacket &packet);

    void timeProc();

    template <typename _Rep, typename _Period>
    void setACKtimeout(const std::chrono::duration<_Rep, _Period> &timeout) {
        ackTimeout = timeout;
    }

    void setMaxRetransmissionCount(uint8_t retransmission) { maxRetransmissionTry = retransmission; }

    bool ping(bool waitForResponse);
    bool isAvailablePacket();

    bool getPendingPacket(HostCommPacket *&packet) {
        packet = &receivedPacket;
        return true;
    }

    Signal<HostCommPacket &> incommingPacket;

    void startHostCommThread(void);
    void stopHostCommThread(void);

 private:
    os::Semaphore ackSemaphore;

    std::mutex sendMutex;
    std::chrono::milliseconds ackTimeout = {std::chrono::milliseconds{1000}};
    uint8_t sentCounter =
        0;  // this counter contain last number of sent frame. This counter is increased when sending new frame but now when retransmitting frame.
    uint8_t receiveCounter = 0;  // this counter contain last number of received frame, is used to detect receive of retransmitted frame.
    uint8_t maxRetransmissionTry = 3;
    size_t dataToRead = 0;
    IODevice &ioDevice;
    diagnostic::Diagnostic<diagnostic::LogLevel::MICROHAL_HOSTCOMM_LOG_LEVEL> log;

    struct {
        uint32_t sentPacketCounter = 0;
        uint32_t receivedPacketCounter = 0;
    } statistics;

    uint8_t packetBuffer[HostCommPacket::maxPacketDataSize + sizeof(HostCommPacket::PacketInfo)];
    HostCommPacket receivedPacket;
    HostCommPacket *txPendingPacket = nullptr;

    HostCommPacket_ACK ACKpacket;
    HostCommPacket pingPacket = {HostCommPacket::PING, false};
    HostCommPacket pongPacket = {HostCommPacket::PONG, false};

    //	DeviceInfoPacket &devInfo;

    bool sentPacktToIODevice(HostCommPacket &packet);
    bool waitForACK(HostCommPacket &packetToACK);
    bool readPacket();
    bool readPacketInfo();

    std::atomic<bool> runThread;
    std::thread runningThread;

    void procThread(void);
};

}  // namespace microhal

#endif /* HOSTCOMM_H_ */
