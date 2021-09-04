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

#ifndef SRC_RFM96HCW_RFM96HCW_H_
#define SRC_RFM96HCW_RFM96HCW_H_

#include "SPIDevice/SPIDevice.h"
#include "microhal.h"
#include "microhal_semaphore.h"
#include "utils/result.h"
#include "utils/types.h"

#define _MICROHAL_RFM96_USE_DIAGNOSTIC

#ifdef _MICROHAL_RFM96_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"
#endif

class RFM96HCW {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    // create alias to microhal::Address, we just want to type less
    template <typename T, T registerAddress>
    using Address = microhal::Address<T, registerAddress + 0b1000'0000, registerAddress>;

    static constexpr auto RegFifo = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
    static constexpr auto RegOpMode = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x01>{});
    static constexpr auto RegDataModul = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x02>{});
    static constexpr auto RegBitrate = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x03>{});
    static constexpr auto RegFdev = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x05>{});
    static constexpr auto RegFrf = microhal::makeRegister<microhal::uint24_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x07>{});
    static constexpr auto RegOsc1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0A>{});
    static constexpr auto RegAfcCtrl = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0B>{});
    static constexpr auto RegListen1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0D>{});
    static constexpr auto RegListen2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0E>{});
    static constexpr auto RegListen3 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0F>{});
    static constexpr auto RegVersion = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x10>{});
    static constexpr auto RegPaLevel = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x11>{});
    static constexpr auto RegPaRamp = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x12>{});
    static constexpr auto RegOcp = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x13>{});
    static constexpr auto RegLna = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x18>{});
    static constexpr auto RegRxBw = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x19>{});
    static constexpr auto RegAfcBw = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1A>{});
    static constexpr auto RegOokPeak = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1B>{});
    static constexpr auto RegOokAvg = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1C>{});
    static constexpr auto RegOokFix = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1D>{});
    static constexpr auto RegAfcFei = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x1E>{});
    static constexpr auto RegAfc = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0x1F>{});
    static constexpr auto RegFei = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0x21>{});
    static constexpr auto RegRssiConfig = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x23>{});
    static constexpr auto RegRssiValue = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x24>{});
    static constexpr auto RegDioMapping1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x25>{});
    static constexpr auto RegDioMapping2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x26>{});
    static constexpr auto RegIrqFlags1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x27>{});
    static constexpr auto RegIrqFlags2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x28>{});
    static constexpr auto RegRssiThresh = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x29>{});
    static constexpr auto RegRxTimeout1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2A>{});
    static constexpr auto RegRxTimeout2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2B>{});
    static constexpr auto RegPreamble = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Big>(Address<uint8_t, 0x2C>{});
    static constexpr auto RegSyncConfig = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2E>{});
    static constexpr auto RegSyncValue1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2F>{});
    static constexpr auto RegSyncValue2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x30>{});
    static constexpr auto RegSyncValue3 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x31>{});
    static constexpr auto RegSyncValue4 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x32>{});
    static constexpr auto RegSyncValue5 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x33>{});
    static constexpr auto RegSyncValue6 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x34>{});
    static constexpr auto RegSyncValue7 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x35>{});
    static constexpr auto RegSyncValue8 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x36>{});
    static constexpr auto RegPacketConfig1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x37>{});
    static constexpr auto RegPayloadLength = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x38>{});
    static constexpr auto RegNodeAdrs = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x39>{});
    static constexpr auto RegBroadcastAdrs = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x3A>{});
    static constexpr auto RegAutoModes = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x3B>{});
    static constexpr auto RegFifoThresh = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x3C>{});
    static constexpr auto RegPacketConfig2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x3D>{});
    static constexpr auto RegAesKey1 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x3E>{});
    static constexpr auto RegAesKey2 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x3F>{});
    static constexpr auto RegAesKey3 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x40>{});
    static constexpr auto RegAesKey4 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x41>{});
    static constexpr auto RegAesKey5 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x42>{});
    static constexpr auto RegAesKey6 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x43>{});
    static constexpr auto RegAesKey7 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x44>{});
    static constexpr auto RegAesKey8 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x45>{});
    static constexpr auto RegAesKey9 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x46>{});
    static constexpr auto RegAesKey10 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x47>{});
    static constexpr auto RegAesKey11 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x48>{});
    static constexpr auto RegAesKey12 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x49>{});
    static constexpr auto RegAesKey13 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x4A>{});
    static constexpr auto RegAesKey14 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x4B>{});
    static constexpr auto RegAesKey15 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x4C>{});
    static constexpr auto RegAesKey16 = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0x4D>{});
    static constexpr auto RegTemp1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x4E>{});
    static constexpr auto RegTemp2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x4F>{});
    static constexpr auto RegTestLna = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x58>{});
    static constexpr auto RegTestPa1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x5A>{});
    static constexpr auto RegTestPa2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x5C>{});
    static constexpr auto RegTestDagc = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x6F>{});
    static constexpr auto RegTestAfc = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x71>{});
    static constexpr auto RegTest = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x50>{});

 public:
    using Error = microhal::SPI::Error;

    enum DataMode {
        PacketMode,
        ContinousModeWithBitSynchronizer,
    };

    enum class PacketLength { Fixed = 0, Variable = 0b1000'0000 };
    enum class CRCMode { Disable = 0b0, EnableClearFifoOnFail = 0b0001'0000, EnableNotClearFifoOnFail = 0b0001'1000 };
    enum class EncodingMode { None = 0, Manchester, Whitening };
    enum class AddressFilteringMode { None = 0b000, MatchNodeAddress = 0b010, MatchNodeOrBroadcastAddress = 0b100 };
    enum class FSKGaussianFilter { Disable = 0, BT10 = 0b01, BT05 = 0b10, BT03 = 0b11 };
    enum class PARamp {
        RaiseFallRamp_3400us = 0,
        RaiseFallRamp_2000us,
        RaiseFallRamp_1000us,
        RaiseFallRamp_500us,
        RaiseFallRamp_250us,
        RaiseFallRamp_125us,
        RaiseFallRamp_100us,
        RaiseFallRamp_62us,
        RaiseFallRamp_50us,
        RaiseFallRamp_40us,
        RaiseFallRamp_31us,
        RaiseFallRamp_25us,
        RaiseFallRamp_20us,
        RaiseFallRamp_15us,
        RaiseFallRamp_12us,
        RaiseFallRamp_10us
    };
    enum class PAConfig : uint8_t {
        Enable_PA0 = 0b1000'0000,         //!< Enable PA0, connected to RFIO and LNA
        Enable_PA1 = 0b0100'0000,         //!< Enables PA1, on PA_BOOST pin
        Enable_PA1_And_PA2 = 0b0110'0000  //!< Enables PA1 and PA2, on PA_BOOST pin
    };

    enum class LNAImpedance { Ohm_50 = 0b0000'0000, Ohm_200 = 0b1000'0000 };

    enum class LNAGain {
        Automatic = 0b000,
        Max = 0b001,
        Max_minus6dB = 0b010,
        Max_minus12dB = 0b011,
        Max_minus24dB = 0b100,
        Max_minus36dB = 0b101,
        Max_minus48dB = 0b110
    };

    enum class Mode { Sleep = 0b0, Standby = 0b0'0100, FrequencySynthesier = 0b0'1000, Transmitter = 0b0'1100, Receiver = 0b1'0000 };

    enum class InterruptFlags1 {
        SyncAddressMatch = 0b0000'0001,
        AutoMode = 0b0000'0010,
        Timeout = 0b0000'0100,
        Rssi = 0b0000'1000,
        PllLock = 0b0001'0000,
        TxReady = 0b0010'0000,
        RxReady = 0b0100'0000,
        ModeReady = 0b1000'0000
    };

    enum class InterruptFlags2 {
        CrcOk = 0b0000'0010,
        PayloadReady = 0b0000'0100,
        PacketSent = 0b0000'1000,
        FifoOverrun = 0b0001'0000,
        FifoLevel = 0b0010'0000,
        FifoNotEmpty = 0b0100'0000,
        FifoFull = 0b1000'0000
    };

    enum class DIO0 { CrcOK_PacketSent = 0b00 << 6, PayloadReady_TxReady = 0b01 << 6, PllLock = 0b11 << 6 };
    enum class DIO1 { FifoLevel = 0b00 << 4, FifoFull = 0b01 << 4, FifoNotEmpty = 0b10 << 4, PllLock = 0b11 << 4 };

    template <size_t maxPacketDataLength = 64>
    struct Packet {
        uint8_t m_length;  // payload length
        uint8_t m_destinationAddress;
        uint8_t m_payload[maxPacketDataLength];
        static_assert(maxPacketDataLength <= 64);

        uint8_t destinationAddress() const { return m_destinationAddress; }
        gsl::span<uint8_t> payload() const { return {m_payload, m_length}; }
        gsl::span<uint8_t> payload() { return {m_payload, m_length}; }

        uint8_t airDataSize() const { return m_length + sizeof(m_length) + sizeof(m_destinationAddress); }

     protected:
        template <typename T>
        T *payload() {
            return reinterpret_cast<T *>(m_payload);
        }

        template <typename T>
        const T *payload() const {
            return reinterpret_cast<const T *>(m_payload);
        }
    };

    static constexpr const uint32_t fxosc = 32000000;  // in Hz
    static constexpr const uint32_t fstep = 61;        // in Hz
    static constexpr const std::array<microhal::SPI::Mode, 1> supportedSPIModes = {microhal::SPI::Mode::Mode0};
    static constexpr const uint32_t maxSckFrequency_Hz = 10'000'000;
    static constexpr const uint_fast8_t fifoSize = 66;

    static constexpr uint16_t packetSendTime_ms(uint16_t bps, uint_fast8_t packetLenBytes) { return (packetLenBytes * 8 * 1000) / bps; }

    RFM96HCW(microhal::SPI &spi, microhal::GPIO &ceGpio, microhal::IOPin dio0, microhal::GPIO &resetGpio);
    virtual ~RFM96HCW();

    void init();

    void reset();
    void chipEnablePinDelay(uint32_t delay_ns) { this->spi.chipEnablePinDelay(delay_ns); }
    //--------------------------------------------------------------------------
    //                           Generic configuration
    //--------------------------------------------------------------------------
    Error setMode(Mode mode);
    microhal::Result<RFM96HCW::Mode, RFM96HCW::Error, RFM96HCW::Error::None> getMode();

    Error outputPower(uint8_t outputPower_dBm);
    microhal::Result<int_fast8_t, RFM96HCW::Error, RFM96HCW::Error::None> outputPower();
    /**
     *
     * @param paConfig
     * @param ramp
     * @param outputPower_dBm
     * @param overcurrentProtectionTrim_mA
     * @return
     */
    Error configurePowerAmplifier(PAConfig paConfig, PARamp ramp, int8_t outputPower_dBm, int8_t overcurrentProtectionTrim_mA = 95);

    Error configureLNA(LNAImpedance impedance, LNAGain gain);
    microhal::Result<LNAGain, Error, Error::None> getLNAGain();

    microhal::Result<InterruptFlags1, Error, Error::None> readInterrupt1();
    microhal::Result<InterruptFlags2, Error, Error::None> readInterrupt2();

    microhal::Result<uint32_t, Error, Error::None> centerFrequency();

    Error triggerRssiMeasurement();
    microhal::Result<bool, Error, Error::None> rssiReady();
    microhal::Result<int_fast8_t, Error, Error::None> getRssi();

    microhal::Result<int_fast16_t, Error, Error::None> bitRate();

    Error configureDIO0(DIO0 dio);
    Error configureDIO1(DIO1 dio);
    //--------------------------------------------------------------------------
    //                           FSK Configuration
    //--------------------------------------------------------------------------
    Error configureFSKModulation(FSKGaussianFilter gausianFilter, uint32_t centerFrequency, uint32_t frequencyDeviation);
    microhal::Result<uint32_t, Error, Error::None> frequencyDeviation();
    //--------------------------------------------------------------------------
    //                           OOK Configuration
    //--------------------------------------------------------------------------
    bool configureOOKModulation() {
        // unimplemented
        return false;
    }
    //--------------------------------------------------------------------------
    //                           Packet mode functions
    //--------------------------------------------------------------------------
    Error sendPacket(uint8_t destinationAddress, gsl::span<uint8_t> data, bool lbt = false);
    template <size_t maxPacketDataLength>
    Error sendPacket(Packet<maxPacketDataLength> packet, bool lbt = false) {
        return sendPacket(packet.m_destinationAddress, packet.payload(), lbt);
    }

    microhal::Result<int_fast8_t, Error, Error::None> readPacket_to(gsl::span<uint8_t> data);
    microhal::Result<int_fast8_t, Error, Error::None> readPacket_to(uint8_t &address, gsl::span<uint8_t> data, std::chrono::milliseconds timeout);
    template <size_t maxPacketDataLength>
    Error readPacket_to(Packet<maxPacketDataLength> &packet, std::chrono::milliseconds timeout) {
        auto result = readPacket_to(packet.m_destinationAddress, packet.payload(), timeout);
        packet.m_length = result ? result.value() : 0;
        return result.error();
    }
    /**
     *
     * @param packetLengthMode @ref PacketLength
     * @param packetLength meaning of this field depend on packetLengthMode:
     *                     - PacketLength::Fixed field specified packet length
     *                     - PacketLength::Variable field specified maximal length of received packet
     * @return
     */
    Error configurePacketMode(PacketLength packetLengthMode, uint_fast8_t packetLength, uint16_t preambleSize, uint16_t bitRate, CRCMode crcMode,
                              EncodingMode encoding);

    Error configureSyncWord(gsl::span<const uint8_t> syncWord, uint8_t toleartedBitErrorsInSyncWord);
    Error enableSyncWordRecognition();
    Error disableSyncWordRecognition();

    Error fifoThreshold(uint_fast8_t threshold);

    Error configureAddressMode(AddressFilteringMode mode, uint8_t nodeAddress, uint8_t broadcastAddress);

    microhal::Result<uint16_t, RFM96HCW::Error, RFM96HCW::Error::None> preambleLength();
    microhal::Result<uint_fast8_t, RFM96HCW::Error, RFM96HCW::Error::None> syncWordLength();
    microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> isSyncWordEnabled();

    microhal::Result<uint8_t, RFM96HCW::Error, RFM96HCW::Error::None> nodeAddress();
    microhal::Result<uint8_t, RFM96HCW::Error, RFM96HCW::Error::None> broadcastAddress();

    Error enableCRC();
    Error disableCRC();
    microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> isCRCEnabled();

    //--------------------------------------------------------------------------
    //                           AES functions
    Error enableAES();
    Error disableAES();
    microhal::Result<bool, RFM96HCW::Error, RFM96HCW::Error::None> isAESEnabled();
    Error setAESkey(const std::array<uint8_t, 16> &key);

    //--------------------------------------------------------------------------
    //                           FIFO functions
    Error fifoWrite(gsl::span<uint8_t> data);
    Error fifoWrite(uint8_t data);
    Error fifoRead_to(uint8_t *data, uint_fast8_t length);

    //--------------------------------------------------------------------------
    //                         Continuous mode functions
    //--------------------------------------------------------------------------
    bool configureContinousMode() {
        // unimplemented
        return false;
    }

    //--------------------------------------------------------------------------
    //                           Listen functions
    //--------------------------------------------------------------------------
    Error configureListenMode();
    Error listenOn();
    Error listenAbort();

    //--------------------------------------------------------------------------
    //                           chip information function
    //--------------------------------------------------------------------------
    microhal::Result<uint8_t, Error, Error::None> version();

    //--------------------------------------------------------------------------
    //                   Temperature measurement functions
    //--------------------------------------------------------------------------
    Error startTemperatureMeasurement();
    microhal::Result<bool, Error, Error::None> isTemperatureMeasurementRunning();
    microhal::Result<uint8_t, Error, Error::None> getTemperature();

    void dumpConfiguration();

 private:
    microhal::SPIDevice spi;
    microhal::ExternalInterrupt m_dio0;
    microhal::GPIO &m_resetGpio;
    uint_fast8_t maxPacketLen = fifoSize;  // this depend on mode so it can't be const
    int_fast8_t lbtThreshold = -90;

 public:
    microhal::os::Semaphore txSendSemaphore;
    microhal::os::Semaphore rxReceivedSemaphore;
    volatile bool inTransmitMode = false;

    void irqDIO0Func();
    microhal::Slot_0<RFM96HCW, &RFM96HCW::irqDIO0Func> irq0Slot;
};

static_assert(RFM96HCW::packetSendTime_ms(8, 1) == 1000);
static_assert(RFM96HCW::packetSendTime_ms(4800, 6) == 10);

inline constexpr uint32_t operator&(RFM96HCW::InterruptFlags2 a, RFM96HCW::InterruptFlags2 b) {
    return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
}

#ifdef _MICROHAL_RFM96_USE_DIAGNOSTIC
template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, RFM96HCW::Mode mode) {
    constexpr const std::array<std::string_view, 6> str{"Sleep", "Standby", "FrequencySynthesier", "Transmitter", "Receiver"};
    return logChannel << str[static_cast<uint8_t>(mode) >> 2];
}
#endif

#endif /* SRC_RFM96HCW_RFM96HCW_H_ */
