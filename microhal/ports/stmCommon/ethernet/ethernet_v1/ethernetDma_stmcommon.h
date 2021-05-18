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

#ifndef _MICROHAL_PORTS_STMCOMMON_ETHERNETDMA_V1_H_
#define _MICROHAL_PORTS_STMCOMMON_ETHERNETDMA_V1_H_

#include "../../registers/ethernet/ethernetDma_v1.h"
#include "../../stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

struct RxDMADescriptor {
    union RDES0 {
        microhal::Bitfield<uint32_t, 0, 1> PCE;           //!< Payload checksum error
        microhal::Bitfield<uint32_t, 1, 1> CE;            //!< CRC error
        microhal::Bitfield<uint32_t, 2, 1> DribbleE;      //!< Dribble bit error
        microhal::Bitfield<uint32_t, 3, 1> RE;            //!< Receive error
        microhal::Bitfield<uint32_t, 4, 1> RWT;           //!< Receive watchdog timeout
        microhal::Bitfield<uint32_t, 5, 1> FT;            //!< Frame type
        microhal::Bitfield<uint32_t, 6, 1> LCO;           //!< Late collision
        microhal::Bitfield<uint32_t, 7, 1> IPHCE;         //!< IPv header checksum error
        microhal::Bitfield<uint32_t, 8, 1> LS;            //!< Last descriptor
        microhal::Bitfield<uint32_t, 9, 1> FS;            //!< First descriptor
        microhal::Bitfield<uint32_t, 10, 1> VLAN;         //!< VLAN tag
        microhal::Bitfield<uint32_t, 11, 1> OE;           //!< Overflow error
        microhal::Bitfield<uint32_t, 12, 1> LE;           //!< Length error
        microhal::Bitfield<uint32_t, 13, 1> SAF;          //!< Source address filter fail
        microhal::Bitfield<uint32_t, 14, 1> DescriptorE;  //!< Descriptor error
        microhal::Bitfield<uint32_t, 15, 1> ES;           //!< Error summary
        microhal::Bitfield<uint32_t, 16, 14> FL;          //!< Destination address filter fail
        microhal::Bitfield<uint32_t, 30, 1> AFM;          //!< Destination address filter fail
        microhal::Bitfield<uint32_t, 31, 1> OWN;          //!< Own bit
    } rdes0;

    union RDES1 {
        microhal::Bitfield<uint32_t, 0, 13> RBS1;   //!< Receive buffer 1 size
        microhal::Bitfield<uint32_t, 14, 1> RCH;    //!< Second address chained
        microhal::Bitfield<uint32_t, 15, 1> RER;    //!< Receive end of ring
        microhal::Bitfield<uint32_t, 16, 13> RBS2;  //!< Receive buffer 2 size
        microhal::Bitfield<uint32_t, 31, 1> DIC;    //!< Disable interrupt on completion
    } rdes1;

    union RDES2 {
        uint32_t RBAP1;  //!< Receive buffer 1 address pointer
        uint8_t* buffer1Address;
        uint32_t RTSL;  //!< Receive frame time stamp low
    } rdes2;

    union RDES3 {
        uint32_t RBAP2;  //!< Receive buffer 2 address pointer
        RxDMADescriptor* nextDescriptorAddress;
        uint32_t RTSH;  //!< Receive frame time stamp high
    } rdes3;

    bool isOwnByDMA() const { return rdes0.OWN == 1; }
};

struct TxDMADescriptor {
    union TDES0 {
        microhal::Bitfield<uint32_t, 0, 1> DB;     //!< Deferred bit
        microhal::Bitfield<uint32_t, 1, 1> UF;     //!< Underflow error
        microhal::Bitfield<uint32_t, 2, 1> ED;     //!< Excessive deferral
        microhal::Bitfield<uint32_t, 3, 4> CC;     //!< Collision count
        microhal::Bitfield<uint32_t, 7, 1> VF;     //!< VLAN frame
        microhal::Bitfield<uint32_t, 8, 1> EC;     //!< Excessive collision
        microhal::Bitfield<uint32_t, 9, 1> LCO;    //!< Late collision
        microhal::Bitfield<uint32_t, 10, 1> NC;    //!< No carrier
        microhal::Bitfield<uint32_t, 11, 1> LCA;   //!< Loss of carrier
        microhal::Bitfield<uint32_t, 12, 1> IPE;   //!< IP payload error
        microhal::Bitfield<uint32_t, 13, 1> FF;    //!< Frame flushed
        microhal::Bitfield<uint32_t, 14, 1> JT;    //!< Jabber timeout
        microhal::Bitfield<uint32_t, 15, 1> ES;    //!< Error summary
        microhal::Bitfield<uint32_t, 16, 1> IHE;   //!< IP header error
        microhal::Bitfield<uint32_t, 17, 1> TTSS;  //!< Transmit time stamp status
        microhal::Bitfield<uint32_t, 20, 1> TCH;   //!< Second address chained
        microhal::Bitfield<uint32_t, 21, 1> TER;   //!< Transmit end of ring
        microhal::Bitfield<uint32_t, 22, 2> CIC;   //!< Checksum insertion control
        microhal::Bitfield<uint32_t, 25, 1> TTSE;  //!< Transmit time stamp enable
        microhal::Bitfield<uint32_t, 26, 1> DP;    //!< Disable pad
        microhal::Bitfield<uint32_t, 27, 1> DC;    //!< Disable CRC
        microhal::Bitfield<uint32_t, 28, 1> FS;    //!< First segment
        microhal::Bitfield<uint32_t, 29, 1> LS;    //!< Last segment
        microhal::Bitfield<uint32_t, 30, 1> IC;    //!< Interrupt on completion
        microhal::Bitfield<uint32_t, 31, 1> OWN;   //!< Own bit
    } tdes0;

    union TDES1 {
        microhal::Bitfield<uint32_t, 0, 13> TBS1;   //!< Transmit buffer 1 size
        microhal::Bitfield<uint32_t, 16, 13> TBS2;  //!< Transmit buffer 2 size
    } tdes1;

    union TEDS2 {
        uint32_t TBAP1;  //!< Transmit buffer 1 address pointer
        uint8_t* buffer1Address;
        uint32_t TTSL;  //!< Transmit frame time stamp low
    } teds2;

    union TEDS3 {
        uint32_t TBAP2;  //!< Transmit buffer 2 address pointer
        uint8_t* buffer2Address;
        TxDMADescriptor* nextDescriptorAddress;
        uint32_t TTSH;  //!< Transmit frame time stamp high
    } teds3;

    bool isOwnByDMA() const { return tdes0.OWN == 1; }
};

class EthernetDMA {
 public:
    enum class Interrupt {
        Transmit = 0b0000'0000'0000'0001,
        TransmitProcessStoped = 0b0000'0000'0000'0010,
        TransmitBufferUnavailable = 0b0000'0000'0000'0100,
        TransmitJabberTimeout = 0b0000'0000'0000'1000,
        Overflow = 0b0000'0000'0001'0000,
        Underflow = 0b0000'0000'0010'0000,
        Receive = 0b0000'0000'0100'0000,
        ReceiveBufferUnavailable = 0b0000'0000'1000'0000,
        ReceiveProcessStopped = 0b0000'0001'0000'0000,
        ReceiveWatchdogTimeout = 0b0000'0010'0000'0000,
        EarlyTransmit = 0b0000'0100'0000'0000,
        FatalBusError = 0b0010'0000'0000'0000,
        EarlyReceive = 0b0100'0000'0000'0000,
        AbnormalInterruptSummary = 0b1000'0000'0000'0000,
        NormalInterruptSummary = 0b1'0000'0000'0000'0000,
        All = Transmit | TransmitProcessStoped | TransmitBufferUnavailable | TransmitJabberTimeout | Overflow | Underflow | Receive |
              ReceiveBufferUnavailable | ReceiveProcessStopped | ReceiveWatchdogTimeout | EarlyTransmit | FatalBusError | EarlyReceive |
              AbnormalInterruptSummary | NormalInterruptSummary
    };

    enum class Status {
        FrameTransmissionFinished = 0b0000'0000'0000'0001,
        TransmitProcessStopped = 0b0000'0000'0000'0010,
        TransmitBufferUnavailable = 0b0000'0000'0000'0100,
        TrasmitJabberTimeout = 0b0000'0000'0000'1000,
        ReceiveOverflow = 0b0000'0000'0001'0000,
        TransmitUnderflow = 0b0000'0000'0010'0000,
        FrameReceptionFinished = 0b0000'0000'0100'0000,
        ReceiveBufferUnavailable = 0b0000'0000'1000'0000,
        ReceiveProcessStopped = 0b0000'0001'0000'0000,
        ReceiveWatchdogTimeout = 0b0000'0010'0000'0000,
        EarlyTransmit = 0b0000'0100'0000'0000,
        FatalBusError = 0b0010'0000'0000'0000,
        EarlyReceive = 0b0100'0000'0000'0000,
        AbnormalInterruptSummary = 0b1000'0000'0000'0000,
        NormalInterruptSummary = 0b1'0000'0000'0000'0000,
    };

    enum class Option {
        DropTCPIPWithChecksumError = 1 << 26,
        ReceiveStoreAndForward = 1 << 25,
        DisableFlushReceivedFrame = 1 << 24,
        TransmitStoreAndForward = 1 << 21,
        FlushTransmitFIFO = 1 << 20,
        ForwardErrorFrames = 1 << 7,
        ForwardUndersizedFrames = 1 << 6,
        OperateOnSecondFrame = 1 << 2
    };

    enum class TransmitTreshold { Threshold64 = 0, Threshold128, Threshold192, Threshold256, Threshold40, Threshold32, Threshold24, Threshld16 };
    enum class ReceiveTreshold { Threshold64 = 0, Threshold32, Threshold96, Threshold128 };

    enum class Mode { AdressAlignedBeats };

    enum class BurstLength { Max1 = 1, Max2 = 2, Max4 = 4, Max8 = 8, Max16 = 16, Max32 = 32, Max64 = 64, Max128 = 128 };

    enum class Arbitration {
        RoundRobin_1Rx1Tx = 0b000,
        RoundRobin_2Rx1Tx = 0b001,
        RoundRobin_3Rx1Tx = 0b010,
        RoundRobin_4Rx1Tx = 0b011,
        RxOverTx = 0b100
    };

    void configure(ReceiveTreshold receiveTreshold, TransmitTreshold transmitTreshold, Option options);

    void configureBurstTransfers(BurstLength transmitBurs, BurstLength receiveBurst, bool fixedBurst,
                                 bool adressAligned);  // this function control DMABMR: FPM, USP, RDP, FB, PBL fields

    void configureArbitration(Arbitration arbitration);

    void descriptorSkipLength(uint16_t length) {
        auto bmr = dma.dmabmr.volatileLoad();
        bmr.DSL = length;
        dma.dmabmr.volatileStore(bmr);
    }

    void reset() {
        auto bmr = dma.dmabmr.volatileLoad();
        bmr.SR.set();
        dma.dmabmr.volatileStore(bmr);
    }

    bool checkResetFlag() const { return dma.dmabmr.volatileLoad().SR.get(); }

    void enableInterrupts(Interrupt interrupt) {
        auto ier = dma.dmaier.volatileLoad();
        ier |= static_cast<uint32_t>(interrupt);
        dma.dmaier.volatileStore(ier);
    }

    void disableInterrupts(Interrupt interrupt) {
        auto ier = dma.dmaier.volatileLoad();
        ier &= ~static_cast<uint32_t>(interrupt);
        dma.dmaier.volatileStore(ier);
    }

    void flushTransmitFIFO() {
        auto omr = dma.dmaomr.volatileLoad();
        omr.FTF.set();
        dma.dmaomr.volatileStore(omr);
    }

    void startTransmission() {
        auto omr = dma.dmaomr.volatileLoad();
        omr.ST.set();
        dma.dmaomr.volatileStore(omr);
    }

    void stopTransmission() {
        auto omr = dma.dmaomr.volatileLoad();
        omr.ST.clear();
        dma.dmaomr.volatileStore(omr);
    }

    void startReception() {
        auto omr = dma.dmaomr.volatileLoad();
        omr.SR.set();
        dma.dmaomr.volatileStore(omr);
    }

    void stopReception() {
        auto omr = dma.dmaomr.volatileLoad();
        omr.SR.clear();
        dma.dmaomr.volatileStore(omr);
    }

    Status status() const { return static_cast<Status>((uint32_t)dma.dmasr.volatileLoad()); }
    bool checkStatus(Status status) { return checkStatus(status, status); }
    bool checkStatus(Status status, Status mask) { return (dma.dmasr.volatileLoad() & static_cast<uint32_t>(mask)) == static_cast<uint32_t>(status); }
    void clearStatus(Status status) { dma.dmasr.volatileStore(static_cast<uint32_t>(status)); }

    void resumeTransmission() { dma.dmatpdr.volatileStore(0); }
    void resumeReception() { dma.dmarpdr.volatileStore(0); }

    void receiveDescriptorListAddress(RxDMADescriptor* descriptorList) { dma.dmardlar.volatileStore((uint32_t)descriptorList); }
    RxDMADescriptor* receiveDescriptorListAddress() { return reinterpret_cast<RxDMADescriptor*>(dma.dmardlar.volatileLoad().SRL.get()); }

    void transmiterDescriptorListAddress(TxDMADescriptor* descriptorList) { dma.dmatdlar.volatileStore((uint32_t)descriptorList); }
    TxDMADescriptor* transmiterDescriptorListAddress() { return reinterpret_cast<TxDMADescriptor*>(dma.dmatdlar.volatileLoad().STL.get()); }

 private:
    registers::ETHERNET_DMA& dma = *registers::ethernetDma;
};  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE

constexpr EthernetDMA::Option operator|(EthernetDMA::Option a, EthernetDMA::Option b) {
    return static_cast<EthernetDMA::Option>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

constexpr EthernetDMA::Interrupt operator|(EthernetDMA::Interrupt a, EthernetDMA::Interrupt b) {
    return static_cast<EthernetDMA::Interrupt>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

constexpr EthernetDMA::Status operator|(EthernetDMA::Status a, EthernetDMA::Status b) {
    return static_cast<EthernetDMA::Status>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

constexpr EthernetDMA::Status operator&(EthernetDMA::Status a, EthernetDMA::Status b) {
    return static_cast<EthernetDMA::Status>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

constexpr bool operator==(EthernetDMA::Status a, int b) {
    return static_cast<uint32_t>(a) == b;
}

constexpr bool operator!=(EthernetDMA::Status a, int b) {
    return static_cast<uint32_t>(a) != b;
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_ETHERNETDMA_V1_H_ */
