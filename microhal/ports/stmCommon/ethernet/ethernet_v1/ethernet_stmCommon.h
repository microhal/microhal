/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_PORTS_STMCOMMON_ETHERNET_V1_H_
#define _MICROHAL_PORTS_STMCOMMON_ETHERNET_V1_H_

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <array>
#include <cstdint>
#include <type_traits>
#include "../../registers/ethernet/ethernetMac_v1.h"
#include "../../stmCommonDefines.h"
#include "ethernetDma_stmcommon.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

struct MACAddress {
    union {
        std::array<uint8_t, 6> address;
        struct {
            uint32_t low;
            uint16_t high;
        };
    };
};

class EthernetMAC {
    enum HclkDiv { Div42 = 0b000, Div16 = 0b010, Div26 = 0b011 };

 public:
    enum class PhyInterface { MII = 0, RMII = 1 };
    enum class MACNumber { MAC1, MAC2, MAC3 };
    enum class MACMaskByte { Byte0, Byte1, Byte2, Byte3, Byte4, Byte5 };
    enum class CompareWith { SourceAddres, DestinationAddress };
    enum class InterframeGap {
        Gap40BitTimes = 0b111,
        Gap48BitTimes = 0b110,
        Gap56BitTimes = 0b101,
        Gap64BitTimes = 0b100,
        Gap72BitTimes = 0b011,
        Gap80BitTimes = 0b010,
        Gap88BitTimes = 0b001,
        Gap96BitTimes = 0b000
    };
    enum class Config {
        Watchdog,
        Jabber,
        CarrierSense,
        ReceiveOwnPacketInHalfDuplexMode,
        Loopback,
        FullDuplexMode,
        ChecksumOffload,
        RetryTransmission,
        AutomaticPadCrcStripping,
        DefferalCheck,
        Transmitter,
        Receiver
    };
    enum class BackOffLimit { min10, min8, min4, min1 };
    enum class Speed { FastEthernet10Mbit = 0b0, FastEthernet100Mbit = 0b1 };
    enum class VlanTagComparisonLen { Len16bit, Len12bit };
    enum class DuplexMode { Full = 0b1, Half = 0b0 };
    enum class Filter {
        ReceiveAll = 1 << 31,
        // HashOrPerfect,
        SourceAddress = 1 << 9,
        SourceAddressInverse = 1 << 8,
        PassControlFrames_blockAll = 0,
        PassControlFrames_forwartAll = 0b10 << 6,
        PassControlFrames_forwadrAddressFiltered = 0b11 << 6,
        BroadcastFreme = 1 << 5,
        PassAllMulticast = 1 << 4,
        HashMulticast = 1 << 2,
        HashUnicast = 1 << 1,
        PromiscousMode = 1 << 1
    };

    enum class FlowControl {
        None = 0,
        ZeroQuantaPause = 1 << 7,
        UnicastPauseFrameDetect = 1 << 3,
        ReceiveFlowControl = 1 << 2,
        TransmitFlowControl = 1 << 1,
        FlowControlBusyBackPressure = 1 << 0
    };

    enum class PauseThreshold { Minus4slotTimes = 0b00, Minus28slotTimes = 0b01, Minus144slotTimes = 0b10, Minus256slotTimes = 0b11 };

    bool init(PhyInterface interface);
    bool configure(Speed speed, InterframeGap interframeGap, BackOffLimit backoffLimit, Config enable);

    bool configureFiltering(Filter filter);

    bool configuteHashTable(uint64_t hashTable) {
        mac.machthr.volatileStore(hashTable >> 32);
        mac.machtlr.volatileStore(hashTable);
        return true;
    }

    bool configureFlowControl(uint16_t pauseTime, PauseThreshold pauseLowThreshold, FlowControl flowControl);

    void configureClockRange();

    void speed(Speed speed) {
        auto ccr = mac.maccr.volatileLoad();
        ccr.FES = static_cast<uint32_t>(speed);
        mac.maccr.volatileStore(ccr);
    }

    Speed speed() const { return static_cast<Speed>(mac.maccr.volatileLoad().FES.get()); }

    void duplexMode(DuplexMode mode) {
        auto ccr = mac.maccr.volatileLoad();
        ccr.DM = static_cast<uint32_t>(mode);
        mac.maccr.volatileStore(ccr);
    }

    DuplexMode duplexMode() const { return static_cast<DuplexMode>(mac.maccr.volatileLoad().DM.get()); }

    void enableTransmitter() {
        auto ccr = mac.maccr.volatileLoad();
        ccr.TE.set();
        mac.maccr.volatileStore(ccr);
    }

    void disableTransmitter() {
        auto ccr = mac.maccr.volatileLoad();
        ccr.TE.clear();
        mac.maccr.volatileStore(ccr);
    }

    void enableReceiver() {
        auto ccr = mac.maccr.volatileLoad();
        ccr.RE.set();
        mac.maccr.volatileStore(ccr);
    }

    void disableReceiver() {
        auto ccr = mac.maccr.volatileLoad();
        ccr.RE.clear();
        mac.maccr.volatileStore(ccr);
    }

    uint32_t readPHYRegister(uint16_t registerAddress);
    bool writePHYRegister(uint16_t registerAddress, uint32_t registerValue);

    void vlanTag(uint16_t vlan, VlanTagComparisonLen comparisonLen) {
        registers::ETHERNET_MAC::MACVLANTR vlantr{};
        vlantr.VLANTI = vlan;
        vlantr.VLANTC = static_cast<uint32_t>(comparisonLen);
        mac.macvlantr.volatileStore(vlantr);
    }

    void macAddress(const MACAddress& address);
    MACAddress macAddress();
    void configureMACAddressFilter(MACNumber macNumber, MACAddress addres, MACMaskByte mask, CompareWith compare);
    void enableMACAddressFilter(MACNumber macNumber);
    void disableMACAddressFilter(MACNumber macNumber);

    uint8_t phyAddress = 0;

 private:
    registers::ETHERNET_MAC& mac = *registers::ethernetMac;

    HclkDiv calculateClockRange();
};

constexpr EthernetMAC::Config operator|(EthernetMAC::Config a, EthernetMAC::Config b) {
    return static_cast<EthernetMAC::Config>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_ETHERNET_V1_H_ */
