/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      RFM96 Radio module driver
 *
 * @authors    Pawel Okas
 * created on: 24-02-2021
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

#include "rfm96hcw.h"

#ifdef _MICROHAL_RFM96_USE_DIAGNOSTIC
using namespace microhal::diagnostic;
#endif

using namespace microhal;
using namespace std::literals;

enum { unused = 0 };
const uint8_t poutCorrection[] = {unused, unused, 18, 14, 18};

RFM96HCW::RFM96HCW(SPI &spi, GPIO &ceGpio, microhal::IOPin dio0, GPIO &resetGpio) : spi(spi, ceGpio), m_dio0(dio0), m_resetGpio(resetGpio) {
    m_dio0.connect(irq0Slot, *this, ExternalInterrupt::Trigger::OnRisingEdge);
    m_resetGpio.configureAsInput(GPIO::PullType::NoPull);
    chipEnablePinDelay(30);
}

RFM96HCW::~RFM96HCW() {
    setMode(RFM96HCW::Mode::Sleep);
}

void RFM96HCW::init() {
    spi.writeRegister(RegRxBw, 0x55);
    spi.writeRegister(RegAfcBw, 0x8B);
    spi.writeRegister(RegDioMapping2, 0x07);
    spi.writeRegister(RegRssiThresh, 0xE4);
    spi.writeRegister(RegTestDagc, 0x30);

    configureDIO0(RFM96HCW::DIO0::CrcOK_PacketSent);
    m_dio0.enable();
}

void RFM96HCW::reset() {
    m_resetGpio.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);
    m_resetGpio.set();
    std::this_thread::sleep_for(1ms);
    m_resetGpio.reset();
    m_resetGpio.configureAsInput(GPIO::PullType::NoPull);
    std::this_thread::sleep_for(5ms);
}

//--------------------------------------------------------------------------
//                           Generic configuration
//--------------------------------------------------------------------------
RFM96HCW::Error RFM96HCW::setMode(Mode mode) {
    auto status = spi.modifyBitsInRegister(RegOpMode, static_cast<uint8_t>(mode), 0b0001'1100);
    inTransmitMode = mode == Mode::Transmitter;
    return status;
}

Result<RFM96HCW::Mode, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::getMode() {
    auto data = spi.readRegister(RegOpMode);
    return {data.error(), static_cast<Mode>((data.value()) & 0b11100)};
}

RFM96HCW::Error RFM96HCW::outputPower(uint8_t outputPower_dBm) {
    const uint8_t poutCorrection[] = {18, 18, 14, 11};
    assert(outputPower_dBm <= 20);

    return spi.writeRegister(RegPaLevel, outputPower_dBm + poutCorrection[0]);
}

Result<int_fast8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::outputPower() {
    auto result = spi.readRegister(RegPaLevel);
    const uint8_t paSettings = (result.value() >> 5) & 0b111;
    return {result.error(), int_fast8_t(result.value() & 0b11111) - poutCorrection[paSettings]};
}
/**
 *
 * @param paConfig
 * @param ramp
 * @param outputPower_dBm
 * @param overcurrentProtectionTrim_mA
 * @return
 */
RFM96HCW::Error RFM96HCW::configurePowerAmplifier(PAConfig paConfig, PARamp ramp, int8_t outputPower_dBm, int8_t overcurrentProtectionTrim_mA) {
    switch (paConfig) {
        case PAConfig::Enable_PA0:
            assert(outputPower_dBm >= -18);
            assert(outputPower_dBm <= 13);
            break;
        case PAConfig::Enable_PA1:
            assert(outputPower_dBm >= -2);
            assert(outputPower_dBm <= 13);
            break;
        case PAConfig::Enable_PA1_And_PA2:
            assert(outputPower_dBm >= 2);
            assert(outputPower_dBm <= 17);
            break;
    }

    Error error = spi.writeRegister(
        RegPaLevel, static_cast<uint_fast8_t>(paConfig) | (outputPower_dBm + poutCorrection[static_cast<uint_fast8_t>(paConfig) >> 5]));
    if (error == Error::None) error = spi.writeRegister(RegPaRamp, static_cast<uint8_t>(ramp));
    if (overcurrentProtectionTrim_mA < 0) {
        // disable ocp
        if (error == Error::None) error = spi.writeRegister(RegOcp, 0);
    } else {
        if (overcurrentProtectionTrim_mA < 45 || overcurrentProtectionTrim_mA > 120) {
        } else {
            uint8_t ocp = (overcurrentProtectionTrim_mA / 5) - 45;
            if (error == Error::None) error = spi.writeRegister(RegOcp, ocp);
        }
    }
    return error;
}

RFM96HCW::Error RFM96HCW::configureLNA(LNAImpedance impedance, LNAGain gain) {
    return spi.writeRegister(RegLna, static_cast<unsigned int>(impedance) | static_cast<unsigned int>(gain));
}

Result<RFM96HCW::LNAGain, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::getLNAGain() {
    auto regLna = spi.readRegister(RegLna);
    return {regLna.error(), static_cast<LNAGain>((regLna.value() >> 3) & 0b111)};
}

microhal::Result<RFM96HCW::InterruptFlags1, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::readInterrupt1() {
    auto result = spi.readRegister(RegIrqFlags1);
    return {result.error(), static_cast<InterruptFlags1>(result.value())};
}

microhal::Result<RFM96HCW::InterruptFlags2, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::readInterrupt2() {
    auto result = spi.readRegister(RegIrqFlags2);
    return {result.error(), static_cast<InterruptFlags2>(result.value())};
}

microhal::Result<uint32_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::centerFrequency() {
    auto frf = spi.readRegister(RegFrf);
    return {frf.error(), frf.value() * fstep};
}

RFM96HCW::Error RFM96HCW::triggerRssiMeasurement() {
    return spi.setBitsInRegister(RegRssiConfig, 0b1);
}
microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::rssiReady() {
    auto rssiConfig = spi.readRegister(RegRssiConfig);
    return {rssiConfig.error(), bool(rssiConfig & 0b10)};
}
microhal::Result<int_fast8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::getRssi() {
    auto rssi = spi.readRegister(RegRssiValue);
    return {rssi.error(), int_fast8_t(rssi.value()) * -1};
}

Result<int_fast16_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::bitRate() {
    auto regBitrate = spi.readRegister(RegBitrate);
    return {regBitrate.error(), fxosc / regBitrate.value()};
}

RFM96HCW::Error RFM96HCW::configureDIO0(DIO0 dio) {
    return spi.modifyBitsInRegister(RegDioMapping1, static_cast<uint8_t>(dio), 0b11 << 6);
}

RFM96HCW::Error RFM96HCW::configureDIO1(DIO1 dio) {
    return spi.modifyBitsInRegister(RegDioMapping1, static_cast<uint8_t>(dio), 0b11 << 4);
}

//--------------------------------------------------------------------------
//                           FSK Configuration
//--------------------------------------------------------------------------
RFM96HCW::Error RFM96HCW::configureFSKModulation(FSKGaussianFilter gausianFilter, uint32_t centerFrequency, uint32_t frequencyDeviation) {
    Error error = spi.modifyBitsInRegister(RegDataModul, static_cast<uint8_t>(gausianFilter), 0b0001'1011);

    const uint32_t frf = centerFrequency / fstep;
    if (error == Error::None) error = spi.writeRegister(RegFrf, frf);

    const uint16_t fdev = frequencyDeviation / fstep;
    if (error == Error::None) error = spi.writeRegister(RegFdev, fdev);

    return error;
}

Result<uint32_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::frequencyDeviation() {
    auto fdev = spi.readRegister(RegFdev);
    return {fdev.error(), fdev.value() * fstep};
}
//--------------------------------------------------------------------------
//                           Packet mode functions
//--------------------------------------------------------------------------
RFM96HCW::Error RFM96HCW::sendPacket(uint8_t destinationAddress, gsl::span<uint8_t> data, bool lbt) {
    assert(data.size() <= maxPacketLen - 2);

    if (auto mode = getMode()) {
        if (mode.value() == Mode::Receiver) setMode(Mode::Standby);
        do {
            auto currentMode = getMode();
            if (currentMode.value() != Mode::Receiver) break;
        } while (1);
    } else {
        return mode.error();
    }

    uint8_t sizeAndAddress[2] = {uint8_t(data.size() + 1), destinationAddress};
    fifoWrite(sizeAndAddress);
    fifoWrite(data);
    if (lbt) {
        while (1) {
            if (auto rssi = getRssi()) {
                if (*rssi < lbtThreshold) {
                    break;
                }
            }
        }
    }
    inTransmitMode = true;
    setMode(Mode::Transmitter);
    if (!txSendSemaphore.wait(300ms)) {
#ifdef _MICROHAL_RFM96_USE_DIAGNOSTIC
        diagChannel << lock << MICROHAL_NOTICE << "Unable to send packet, semaphore timeout." << unlock;
#endif
        return Error::Timeout;
    }
    return Error::None;
}

microhal::Result<int_fast8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::readPacket_to(gsl::span<uint8_t> data) {
    //  fifoRead(data);
    return Error::None;
}

microhal::Result<int_fast8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::readPacket_to(uint8_t &address, gsl::span<uint8_t> data,
                                                                                              std::chrono::milliseconds timeout) {
    if (rxReceivedSemaphore.wait(timeout)) {
        uint8_t length = 0;
        fifoRead_to(&length, 1);
        if (length <= fifoSize && length >= 1) {
            fifoRead_to(&address, 1);
            fifoRead_to(data.data(), length - 1);
            return length - 1;
        }
    }
    return Error::Timeout;
}

RFM96HCW::Error RFM96HCW::configurePacketMode(PacketLength packetLengthMode, uint_fast8_t packetLength, uint16_t preambleSize, uint16_t bitRate,
                                              CRCMode crcMode, EncodingMode encoding) {
    const uint8_t packetConfig1 = static_cast<uint8_t>(packetLengthMode) | static_cast<uint8_t>(encoding) | static_cast<uint8_t>(crcMode);
    Error error = spi.modifyBitsInRegister(RegPacketConfig1, packetConfig1, 0b1111'1000);
    if (error == Error::None) error = spi.writeRegister(RegPayloadLength, packetLength);
    if (error == Error::None) error = spi.writeRegister(RegBitrate, fxosc / bitRate);
    if (error == Error::None) error = spi.clearBitsInRegister(RegDataModul, 0b0110'0000);  // set packet mode
    if (error == Error::None) error = spi.writeRegister(RegPreamble, preambleSize);
    return error;
}

RFM96HCW::Error RFM96HCW::configureSyncWord(gsl::span<const uint8_t> syncWord, uint8_t toleartedBitErrorsInSyncWord) {
    assert(syncWord.size() < 9);
    assert(toleartedBitErrorsInSyncWord < 8);

    if (syncWord.size() >= 1) spi.writeRegister(RegSyncValue1, syncWord[0]);
    if (syncWord.size() >= 2) spi.writeRegister(RegSyncValue2, syncWord[1]);
    if (syncWord.size() >= 3) spi.writeRegister(RegSyncValue3, syncWord[2]);
    if (syncWord.size() >= 4) spi.writeRegister(RegSyncValue4, syncWord[3]);
    if (syncWord.size() >= 5) spi.writeRegister(RegSyncValue5, syncWord[4]);
    if (syncWord.size() >= 6) spi.writeRegister(RegSyncValue6, syncWord[5]);
    if (syncWord.size() >= 7) spi.writeRegister(RegSyncValue7, syncWord[6]);
    if (syncWord.size() >= 8) spi.writeRegister(RegSyncValue8, syncWord[7]);

    return spi.modifyBitsInRegister(RegSyncConfig, ((syncWord.size() - 1) << 3) | toleartedBitErrorsInSyncWord, 0b0011'1111);
}

RFM96HCW::Error RFM96HCW::enableSyncWordRecognition() {
    return spi.setBitsInRegister(RegSyncConfig, 1 << 7);
}

RFM96HCW::Error RFM96HCW::disableSyncWordRecognition() {
    return spi.clearBitsInRegister(RegSyncConfig, 1 << 7);
}

RFM96HCW::Error RFM96HCW::fifoThreshold(uint_fast8_t threshold) {
    assert(threshold <= fifoSize);
    return spi.modifyBitsInRegister(RegFifoThresh, threshold, 0b0111'1111);
}

RFM96HCW::Error RFM96HCW::configureAddressMode(AddressFilteringMode mode, uint8_t nodeAddress, uint8_t broadcastAddress) {
    if (mode == AddressFilteringMode::MatchNodeOrBroadcastAddress) {
        spi.writeRegister(RegBroadcastAdrs, broadcastAddress);
    }
    if (mode == AddressFilteringMode::MatchNodeAddress || mode == AddressFilteringMode::MatchNodeOrBroadcastAddress) {
        spi.writeRegister(RegNodeAdrs, nodeAddress);
    }
    return spi.modifyBitsInRegister(RegPacketConfig1, static_cast<uint8_t>(mode), 0b110);
}

microhal::Result<uint16_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::preambleLength() {
    return spi.readRegister(RegPreamble);
}

microhal::Result<uint_fast8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::syncWordLength() {
    auto syncConfig = spi.readRegister(RegSyncConfig);
    return {syncConfig.error(), ((syncConfig.value() >> 3U) & 0b0000'0111) + 1};
}

microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::isSyncWordEnabled() {
    auto syncConfig = spi.readRegister(RegSyncConfig);
    return {syncConfig.error(), bool(syncConfig.value() & 0b1000'0000)};
}

microhal::Result<uint8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::nodeAddress() {
    return spi.readRegister(RegNodeAdrs);
}

microhal::Result<uint8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::broadcastAddress() {
    return spi.readRegister(RegBroadcastAdrs);
}

RFM96HCW::Error RFM96HCW::enableCRC() {
    return spi.setBitsInRegister(RegPacketConfig1, 1 << 4);
}
RFM96HCW::Error RFM96HCW::disableCRC() {
    return spi.clearBitsInRegister(RegPacketConfig1, 1 << 4);
}
microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::isCRCEnabled() {
    auto packetConfig1 = spi.readRegister(RegPacketConfig1);
    return {packetConfig1.error(), bool(packetConfig1.value() & 1 << 4)};
}
//--------------------------------------------------------------------------
//                           AES functions
RFM96HCW::Error RFM96HCW::enableAES() {
    return spi.setBitsInRegister(RegPacketConfig2, 1);
}
RFM96HCW::Error RFM96HCW::disableAES() {
    return spi.clearBitsInRegister(RegPacketConfig2, 1);
}
microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::isAESEnabled() {
    auto packetConfig = spi.readRegister(RegPacketConfig2);
    return {packetConfig.error(), bool(packetConfig.value() & 0b1)};
}
RFM96HCW::Error RFM96HCW::setAESkey(const std::array<uint8_t, 16> &key) {
    return spi.writeMultipleRegisters(key, RegAesKey1, RegAesKey2, RegAesKey3, RegAesKey4, RegAesKey5, RegAesKey6, RegAesKey7, RegAesKey8, RegAesKey9,
                                      RegAesKey10, RegAesKey11, RegAesKey12, RegAesKey13, RegAesKey14, RegAesKey15, RegAesKey16);
}
//--------------------------------------------------------------------------
//                           FIFO functions
RFM96HCW::Error RFM96HCW::fifoWrite(uint8_t data) {
    return spi.writeRegister(RegFifo, data);
}

RFM96HCW::Error RFM96HCW::fifoWrite(gsl::span<uint8_t> data) {
    return spi.writeFIFORegister(RegFifo, data);
}

RFM96HCW::Error RFM96HCW::fifoRead_to(uint8_t *data, uint_fast8_t length) {
    assert(length <= fifoSize);
    return spi.readFIFORegister_to(RegFifo, data, length);
}

//--------------------------------------------------------------------------
//                           Listen functions
//--------------------------------------------------------------------------
RFM96HCW::Error RFM96HCW::configureListenMode() {
    // unimplemented
    std::terminate();
}
RFM96HCW::Error RFM96HCW::listenOn() {
    return spi.setBitsInRegister(RegOpMode, 1 << 6);
}
RFM96HCW::Error RFM96HCW::listenAbort() {
    return spi.modifyBitsInRegister(RegOpMode, 0b10 << 5, 0b11 << 5);
}

//--------------------------------------------------------------------------
//                           chip information function
//--------------------------------------------------------------------------
microhal::Result<uint8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::version() {
    return spi.readRegister(RegVersion);
}

//--------------------------------------------------------------------------
//                   Temperature measurement functions
//--------------------------------------------------------------------------
RFM96HCW::Error RFM96HCW::startTemperatureMeasurement() {
    return spi.setBitsInRegister(RegTemp1, 1 << 3);
}

microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::isTemperatureMeasurementRunning() {
    auto result = spi.readRegister(RegTemp1);
    if (result) {
        return result.value() & (1 << 2);
    }
    return result.error();
}

microhal::Result<uint8_t, RFM96HCW::Error, RFM96HCW::Error::None> RFM96HCW::getTemperature() {
    return spi.readRegister(RegTemp2);
}

#ifdef _MICROHAL_RFM96_USE_DIAGNOSTIC
void RFM96HCW::dumpConfiguration() {
    std::array<uint8_t, 8> syncWord;
    spi.readMultipleRegisters_to(syncWord, RegSyncValue1, RegSyncValue2, RegSyncValue3, RegSyncValue4, RegSyncValue5, RegSyncValue6, RegSyncValue7,
                                 RegSyncValue8);

    diagChannel << lock << MICROHAL_DEBUG << "RFM69 Configuration dump:" << endl
                << "\tRFM version: " << version() << endl
                << "\tMode: " << getMode() << endl
                << "\tData Mode: " << endl
                << "\tModulation: " << endl
                << "\tBitrate: " << bitRate() << endl
                << "\tFrequency deviation: " << frequencyDeviation() << endl
                << "\tCenter frequency [Hz]: " << centerFrequency() << endl
                << "\tOutput Power: " << outputPower() << endl
                << "\tPreamble length: " << preambleLength() << endl
                << "\tSync word length: " << syncWordLength() << ", sync word enabled: " << isSyncWordEnabled() << ", sync word: " << toHex(syncWord)
                << endl
                << "\tCRC Enabled: " << isCRCEnabled() << endl
                << "\tNode Address: " << nodeAddress() << endl
                << "\tBroadcast Address: " << broadcastAddress() << endl
                << "\tAES Enabled: " << isAESEnabled() << endl
                << unlock;
}
#endif

void RFM96HCW::irqDIO0Func() {
    if (inTransmitMode)
        txSendSemaphore.giveFromISR();
    else
        rxReceivedSemaphore.giveFromISR();
}
