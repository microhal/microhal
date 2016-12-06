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

#include "hostComm.h"

namespace microhal {

using namespace diagnostic;
using namespace std::chrono_literals;

bool HostComm::send(HostCommPacket &packet) {
    std::lock_guard<std::mutex> lock_mutex(sendMutex);
    // count up from 0 to 15
    sentCounter = (sentCounter + 1) & 0x0F;
    //
    packet.setNumber(sentCounter);
    packet.calculateCRC();

    log << lock << MICROHAL_INFORMATIONAL << "Sending packet, with type: " << (uint32_t)packet.getType()
        << ", number: " << (uint32_t)packet.getNumber() << ", data size: " << packet.getSize() << endl
        << unlock;
    if (sentPacktToIODevice(packet) == false) {
        log << lock << MICROHAL_WARNING << "Unable to sent packet." << endl << unlock;
        return false;
    }

    if (packet.requireACK()) {
        // txPendingPacket = &packet;
        for (uint16_t retransmission = 0; retransmission < maxRetransmissionTry; retransmission++) {
            log << lock << MICROHAL_INFORMATIONAL << "Waiting for ACK..." << unlock;
            if (waitForACK(packet)) {
                log << lock << Informational << "ACK OK" << endl << unlock;
                return true;
            }
            log << lock << Informational << "ACK Missing" << endl << "Retransmitting packet." << endl << unlock;
            if (sentPacktToIODevice(packet) == false) {
                log << lock << MICROHAL_WARNING << "Unable to sent packet." << endl << unlock;
                return false;
            }
        }
        // unable to deliver packet
        return false;
    }
    return true;
}

bool HostComm::sentPacktToIODevice(HostCommPacket &packet) {
    // check if packet info data and payload are continous in memory
    if (packet.getSize() == 0 || reinterpret_cast<void *>(&packet.packetInfo + sizeof(HostCommPacket::PacketInfo)) == packet.getDataPtr()) {
        const size_t sizeToTransfer = packet.getSize() + sizeof(HostCommPacket::PacketInfo);
        if (ioDevice.write((char *)(&packet.packetInfo), sizeToTransfer) == sizeToTransfer) return true;
    } else {
        if (ioDevice.write((char *)(&packet.packetInfo), sizeof(HostCommPacket::PacketInfo)) != sizeof(HostCommPacket::PacketInfo)) return false;
        if (ioDevice.write(packet.getDataPtr<char>(), packet.getSize()) == packet.getSize()) return true;
    }
    return false;
}

bool HostComm::ping(bool waitForResponse) {
    // std::lock_guard<std::mutex> guard(sendMutex);

    log << lock << MICROHAL_INFORMATIONAL << "HostComm: Sending PING..." << unlock;
    const bool status = send(pingPacket);
    if (status == true) {
        log << lock << Informational << "OK" << endl << unlock;
        if (waitForResponse == true) {
            log << lock << MICROHAL_INFORMATIONAL << "HostComm: Waiting for PONG..." << endl << unlock;

            if (ackSemaphore.wait(ackTimeout)) {
                if (receivedPacket.getType() == HostCommPacket::PONG) {
                    log << lock << Informational << "OK" << endl << unlock;
                    return true;
                } else {
                    log << lock << Informational << "ERROR" << endl << unlock;
                    return false;
                }
            } else {
                log << lock << MICROHAL_WARNING << "Unable to receive PONG, semaphore timeout." << endl << unlock;
                return false;
            }
        }
    }
    return status;
}

bool HostComm::waitForACK(HostCommPacket &packetToACK) {
    if (ackSemaphore.wait(ackTimeout)) {
        HostCommPacket_ACK &ack = static_cast<HostCommPacket_ACK &>(receivedPacket);
        if (ack.isAcknowledged(packetToACK)) {
            return true;
        }
    } else {
        log << lock << MICROHAL_INFORMATIONAL << "Unable to receive ACK, semaphore timeout." << endl << unlock;
    }
    return false;
}

void HostComm::timeProc() {
    if (readPacket() == true) {
        log << lock << MICROHAL_INFORMATIONAL << "HostComm: got packet, type: " << receivedPacket.getType() << ", size: " << receivedPacket.getSize()
            << endl
            << unlock;
        if (receivedPacket.checkCRC() == true) {
            // if need do send ack
            if (receivedPacket.requireACK() == true) {
                // send ack
                ACKpacket.setPacketToACK(receivedPacket);
                if (send(ACKpacket)) {
                    log << lock << MICROHAL_INFORMATIONAL << "HostComm: ACK sent" << endl << unlock;
                } else {
                    log << lock << MICROHAL_WARNING << "HostComm: unable to send ACK." << endl << unlock;
                }
            }

            // if packet wasn't received
            if (receivedPacket.getNumber() != receiveCounter) {
                receiveCounter = receivedPacket.getNumber();

                switch (receivedPacket.getType()) {
                    case HostCommPacket::ACK: {
                        log << lock << MICROHAL_INFORMATIONAL << "HostComm: got ACK." << unlock;
                        ackSemaphore.give();
                    } break;
                    case HostCommPacket::PING:
                        log << lock << MICROHAL_INFORMATIONAL << "HostComm: got PING. Sending PONG... " << unlock;
                        if (send(pongPacket) == false) {
                            log << lock << Informational << "Error" << endl << unlock;
                        } else {
                            log << lock << Informational << "Ok" << endl << unlock;
                        }
                        break;

                    case HostCommPacket::PONG:
                        log << lock << MICROHAL_INFORMATIONAL << "HostComm: got PONG." << endl << unlock;
                        ackSemaphore.give();
                        break;
                    case HostCommPacket::DEVICE_INFO_REQUEST:
                        log << lock << MICROHAL_INFORMATIONAL << "HostComm: got DeviceInfoPacket Request, unimplementde." << endl << unlock;
                        break;

                    default:
                        receivedPacket.debug(log);
                        incommingPacket.emit(receivedPacket);
                }
            } else {
                log << lock << MICROHAL_NOTICE << "HostComm: discarding packet, was earlier processed." << endl << unlock;
            }
        } else {
            log << lock << MICROHAL_WARNING << "HostComm: packet CRC error." << endl << unlock;
        }
    }
}

inline bool HostComm::readPacketInfo() {
    uint_fast8_t repeat = 100;
    while (repeat--) {
        // find 0xFF in received data
        const size_t bytesAvailable = ioDevice.availableBytes();

        if (bytesAvailable >= sizeof(HostCommPacket::PacketInfo)) {
            // read packet start byte (longOne)
            uint8_t longOne;
            ioDevice.getChar(reinterpret_cast<char &>(longOne));

            if (longOne != 0xFF) {
                // some error situation, packet info longOne pool is different than 0xFF
                log << lock << MICROHAL_CRITICAL << "Packet info longOne: " << longOne << ", but should be equal 0xFF." << endl << unlock;
                continue;
            } else {
                // found packet begin, set it to packetInfo structure
                receivedPacket.packetInfo.longOne = 0xFF;
                // read rest of packetInfo structure
                ioDevice.read(reinterpret_cast<char *>(&receivedPacket.packetInfo.control),
                              sizeof(HostCommPacket::PacketInfo) - sizeof(HostCommPacket::PacketInfo::longOne));

                // ok, now check packet size
                if (receivedPacket.getSize() <= HostCommPacket::maxPacketDataSize) {
                    // read all available data
                    uint16_t toRead = receivedPacket.getSize();
                    toRead -= ioDevice.read(receivedPacket.getDataPtr<char>(), toRead);

                    // if all packet was received
                    if (toRead == 0) {
                        dataToRead = 0;
                        return true;
                    } else {
                        dataToRead = toRead;
                        return false;
                    }
                } else {
                    log << lock << MICROHAL_CRITICAL << "Packet data size to large: " << receivedPacket.getSize()
                        << ", max payload size is: " << (uint32_t)HostCommPacket::maxPacketDataSize << endl
                        << unlock;
                    continue;
                }
            }
        } else {
            dataToRead = 0;
            return false;
        }
    }

    return false;
}

bool HostComm::readPacket() {
    // if receiving new packet
    if (dataToRead == 0) {
        return readPacketInfo();
    } else {  // else finish receiving packet
        const size_t bytesAvailable = ioDevice.availableBytes();
        if (bytesAvailable >= dataToRead || bytesAvailable >= 100) {
            char *readPtr = receivedPacket.getDataPtr<char>();
            readPtr += receivedPacket.getSize() - dataToRead;

            dataToRead -= ioDevice.read(readPtr, dataToRead);
            // if all packet was received
            if (dataToRead == 0) {
                return true;
            }
        }
    }
    return false;
}

void HostComm::startHostCommThread(void) {
    runThread = true;
    runningThread = std::thread(&HostComm::procThread, this);
}
void HostComm::stopHostCommThread(void) {
    runThread = false;
    if (runningThread.joinable()) {
        runningThread.join();
    }
}
void HostComm::procThread(void) {
    while (runThread) {
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
        timeProc();
    }
}

}  // namespace microhal
