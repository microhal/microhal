/*
 * ethernetDma_stmCommon.cpp
 *
 *  Created on: May 18, 2021
 *      Author: pokas
 */

#include "ethernetDma_stmcommon.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

void EthernetDMA::configure(ReceiveTreshold receiveTreshold, TransmitTreshold transmitTreshold, Option options) {
    uint32_t optionMask = static_cast<uint32_t>(Option::DropTCPIPWithChecksumError) | static_cast<uint32_t>(Option::ReceiveStoreAndForward) |
                          static_cast<uint32_t>(Option::DisableFlushReceivedFrame) | static_cast<uint32_t>(Option::TransmitStoreAndForward) |
                          static_cast<uint32_t>(Option::FlushTransmitFIFO) | static_cast<uint32_t>(Option::ForwardErrorFrames) |
                          static_cast<uint32_t>(Option::ForwardUndersizedFrames) | static_cast<uint32_t>(Option::OperateOnSecondFrame);

    auto omr = dma.dmaomr.volatileLoad();
    omr.RTC = static_cast<uint32_t>(receiveTreshold);
    omr.TTC = static_cast<uint32_t>(transmitTreshold);
    omr &= ~optionMask;
    omr |= static_cast<uint32_t>(options);
    dma.dmaomr.volatileStore(omr);
}

void EthernetDMA::configureBurstTransfers(BurstLength transmitBurs, BurstLength receiveBurst, bool fixedBurst,
                                          bool adressAligned) {  // this function control DMABMR: FPM, USP, RDP, FB, PBL fields
    uint16_t transmitBurstLen = static_cast<uint16_t>(transmitBurs);
    uint16_t receiveBurstLen = static_cast<uint16_t>(transmitBurs);

    auto bmr = dma.dmabmr.volatileLoad();
    if (transmitBurs == receiveBurst) {
        bmr.USP.clear();
        if (transmitBurstLen > 32) {
            bmr.FPM.set();
            bmr.PBL = transmitBurstLen / 4;
        } else {
            bmr.FPM.clear();
            bmr.PBL = transmitBurstLen;
        }
    } else {
        bmr.USP.set();
        if (transmitBurstLen > 32 || receiveBurstLen > 32) {
            bmr.FPM.set();
            transmitBurstLen /= 4;
            receiveBurstLen /= 4;
            //    assert(transmitBurstLen > 0 && receiveBurstLen > 0);
        }
        bmr.RDP = receiveBurstLen;
        bmr.PBL = transmitBurstLen;
    }

    bmr.FB = fixedBurst ? 1 : 0;
    bmr.AAB = adressAligned ? 1 : 0;

    dma.dmabmr.volatileStore(bmr);
}

void EthernetDMA::configureArbitration(Arbitration arbitration) {
    auto bmr = dma.dmabmr.volatileLoad();
    if (arbitration == Arbitration::RxOverTx) {
        bmr.DA.set();
    } else {
        bmr.DA.clear();
        bmr.RTPR = static_cast<uint32_t>(arbitration);
    }
    dma.dmabmr.volatileStore(bmr);
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
