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

#include "ethernet_stmCommon.h"
#include <ports/stmCommon/clockManager/afioClock.h>
#include <ports/stmCommon/clockManager/hclkClock.h>
#include <ports/stmCommon/registers/afio_registers.h>
#include <cassert>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

void selectInterfaceMode(EthernetMAC::PhyInterface interface) {
    ClockManager::enableAFIO();

    auto mapr = registers::afio1->mapr.volatileLoad();
    mapr.MII_RMII_SEL = static_cast<uint32_t>(interface);
    registers::afio1->mapr.volatileStore(mapr);
}

bool EthernetMAC::init(PhyInterface interface) {
    // auto pmc = registers::syscfg->pmc.volatileRead();
    // pmc.MII_RMII_SEL = interface == PhyInterface::MII;

    selectInterfaceMode(interface);  // select MII or RMII
    return true;
}

bool EthernetMAC::configure(Speed speed, InterframeGap interframeGap, BackOffLimit backoffLimit, Config enable) {
    uint32_t configMask = static_cast<uint32_t>(Config::Watchdog) | static_cast<uint32_t>(Config::Jabber) |
                          static_cast<uint32_t>(Config::CarrierSense) | static_cast<uint32_t>(Config::ReceiveOwnPacketInHalfDuplexMode) |
                          static_cast<uint32_t>(Config::Loopback) | static_cast<uint32_t>(Config::FullDuplexMode) |
                          static_cast<uint32_t>(Config::ChecksumOffload) | static_cast<uint32_t>(Config::RetryTransmission) |
                          static_cast<uint32_t>(Config::AutomaticPadCrcStripping) | static_cast<uint32_t>(Config::DefferalCheck) |
                          static_cast<uint32_t>(Config::Transmitter) | static_cast<uint32_t>(Config::Receiver);
    auto cr = mac.maccr.volatileLoad();
    cr.FES = static_cast<uint32_t>(speed);
    cr.IFG = static_cast<uint32_t>(interframeGap);
    cr.BL = static_cast<uint32_t>(backoffLimit);
    cr &= ~configMask;
    cr |= static_cast<uint32_t>(enable);
    mac.maccr.volatileStore(cr);
    return true;
}

bool EthernetMAC::configureFiltering(Filter filter) {
    uint32_t filterMask = static_cast<uint32_t>(Filter::ReceiveAll) | static_cast<uint32_t>(Filter::SourceAddress) |
                          static_cast<uint32_t>(Filter::SourceAddressInverse) |
                          static_cast<uint32_t>(Filter::PassControlFrames_forwadrAddressFiltered) | static_cast<uint32_t>(Filter::BroadcastFreme) |
                          static_cast<uint32_t>(Filter::PassAllMulticast) | static_cast<uint32_t>(Filter::HashMulticast) |
                          static_cast<uint32_t>(Filter::HashUnicast) | static_cast<uint32_t>(Filter::PromiscousMode);
    auto ffr = mac.macffr.volatileLoad();
    ffr &= ~filterMask;
    ffr |= static_cast<uint32_t>(filter);
    mac.macffr.volatileStore(ffr);
    return true;
}

bool EthernetMAC::configureFlowControl(uint16_t pauseTime, PauseThreshold pauseLowThreshold, FlowControl flowControl) {
    uint32_t flowControlMask = static_cast<uint32_t>(FlowControl::ZeroQuantaPause) | static_cast<uint32_t>(FlowControl::UnicastPauseFrameDetect) |
                               static_cast<uint32_t>(FlowControl::ReceiveFlowControl) | static_cast<uint32_t>(FlowControl::TransmitFlowControl) |
                               static_cast<uint32_t>(FlowControl::FlowControlBusyBackPressure);
    registers::ETHERNET_MAC::MACFCR fcr{};
    fcr.PT = pauseTime;
    fcr.PLT = static_cast<uint32_t>(pauseLowThreshold);
    fcr &= ~flowControlMask;
    fcr |= static_cast<uint32_t>(flowControl);
    mac.macfcr.volatileStore(fcr);
    return true;
}

uint32_t EthernetMAC::readPHYRegister(uint16_t registerAddress) {
    assert(registerAddress < 32);

    auto miiar = mac.macmiiar.volatileLoad();
    if (!miiar.MB) {
        miiar.PA = phyAddress;
        miiar.MR = registerAddress;
        miiar.MW.clear();  // clear write flag (read request)
        miiar.MB.set();
        mac.macmiiar.volatileStore(miiar);

        while (1) {
            if (!mac.macmiiar.volatileLoad().MB) {
                return mac.macmiidr.volatileLoad();
            }
        }
    }
    return 0;
}

bool EthernetMAC::writePHYRegister(uint16_t registerAddress, uint32_t registerValue) {
    assert(registerAddress < 32);

    auto miiar = mac.macmiiar.volatileLoad();
    if (!miiar.MB) {
        mac.macmiidr.volatileStore(registerValue);

        miiar.PA = phyAddress;
        miiar.MR = registerAddress;
        miiar.MW.set();  // set write flag
        miiar.MB.set();
        mac.macmiiar.volatileStore(miiar);

        // wait for write finish
        while (1) {
            if (!mac.macmiiar.volatileLoad().MB) return true;
        }
    }
    return false;
}

void EthernetMAC::macAddress(const MACAddress &address) {
    registers::ETHERNET_MAC::MACA0HR macHigh{};
    macHigh.MO.set();  // Always 1
    macHigh.MACA0H = address.high;
    mac.maca0hr.volatileStore(macHigh);

    registers::ETHERNET_MAC::MACA0LR macLow{};
    macLow.MACA0L = address.low;
    mac.maca0lr.volatileStore(macLow);
}

MACAddress EthernetMAC::macAddress() {
    MACAddress addr;
    addr.low = mac.maca0lr.volatileLoad();
    addr.high = mac.maca0hr.volatileLoad().MACA0H;
    return addr;
}

void EthernetMAC::configureMACAddressFilter(MACNumber macNumber, MACAddress address, MACMaskByte mask, CompareWith compare) {
    registers::ETHERNET_MAC::MACAxHR macHigh{};
    macHigh.AE.clear();
    macHigh.SA = static_cast<uint32_t>(compare);
    macHigh.MBC = static_cast<uint32_t>(mask);
    macHigh.MACA1H = address.high;

    registers::ETHERNET_MAC::MACAxLR macLow{};
    macLow.MACA1L = address.low;
    if (macNumber == MACNumber::MAC1) {
        mac.maca1hr.volatileStore(macHigh);
        mac.maca1lr.volatileStore(macLow);
    } else if (macNumber == MACNumber::MAC2) {
        mac.maca2hr.volatileStore(macHigh);
        mac.maca2lr.volatileStore(macLow);
    } else {
        mac.maca3hr.volatileStore(macHigh);
        mac.maca3lr.volatileStore(macLow);
    }
}

void EthernetMAC::enableMACAddressFilter(MACNumber macNumber) {
    if (macNumber == MACNumber::MAC1) {
        auto maca = mac.maca1hr.volatileLoad();
        maca.AE.set();
        mac.maca1hr.volatileStore(maca);
    } else if (macNumber == MACNumber::MAC2) {
        auto maca = mac.maca2hr.volatileLoad();
        maca.AE.set();
        mac.maca2hr.volatileStore(maca);
    } else {
        auto maca = mac.maca3hr.volatileLoad();
        maca.AE.set();
        mac.maca3hr.volatileStore(maca);
    }
}

void EthernetMAC::disableMACAddressFilter(MACNumber macNumber) {
    if (macNumber == MACNumber::MAC1) {
        auto maca = mac.maca1hr.volatileLoad();
        maca.AE.clear();
        mac.maca1hr.volatileStore(maca);
    } else if (macNumber == MACNumber::MAC2) {
        auto maca = mac.maca2hr.volatileLoad();
        maca.AE.clear();
        mac.maca2hr.volatileStore(maca);
    } else {
        auto maca = mac.maca3hr.volatileLoad();
        maca.AE.clear();
        mac.maca3hr.volatileStore(maca);
    }
}

void EthernetMAC::configureClockRange() {
    auto miiar = mac.macmiiar.volatileLoad();
    miiar.CR = calculateClockRange();
    mac.macmiiar.volatileStore(miiar);
}

EthernetMAC::HclkDiv EthernetMAC::calculateClockRange() {
    uint32_t freq = ClockManager::HCLK::frequency() / 1000000;
    assert(freq > 20 && freq <= 72);

    if (freq > 20 && freq <= 35) return HclkDiv::Div16;
    if (freq > 35 && freq <= 60) return HclkDiv::Div26;
    if (freq > 60 && freq <= 72) return HclkDiv::Div42;

    std::terminate();
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
