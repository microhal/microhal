/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 27-06-2016
 * last modification: 1-07-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

#ifndef _MICROHAL_UCAMII_H_
#define _MICROHAL_UCAMII_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <experimental/optional>
#include <numeric>
#include "gsl/span"
#include "microhal.h"
#include "diagnostic/diagnostic.h"
/* **************************************************************************************************************************************************
 * CLASS
 */

class uCAM_II {
	enum class ImageFormat : uint8_t { GrayScalse8bit = 0x03, CrYCbY16bit = 0x08, Colour16bit = 0x06, JPEG = 0x07 };
    enum class CommandType : uint8_t {
        Initial = 0x01,
        GetPicture = 0x04,
        Snapshot = 0x05,
        SetPackageSize = 0x06,
        SetBaudRate = 0x07,
        Reset = 0x08,
        Data = 0x0A,
        Sync = 0x0D,
        Ack = 0x0E,
        Nak = 0x0F,
        Light = 0x13
    };

 public:
    enum class ImageFormatRAW : uint8_t {
    	GrayScalse8bit = ImageFormat::GrayScalse8bit,
    	CrYCbY16bit = ImageFormat::CrYCbY16bit,
		Colour16bit = ImageFormat::Colour16bit
    };
    enum class RawResolution : uint8_t { Image_80x60 = 0x01, Image_160x120 = 0x03, Image_128x128 = 0x09, Image_128x96 = 0x0B };
    enum class JpegResolution : uint8_t { Image_160x128 = 0x03, Image_320x240 = 0x05, Image_640x480 = 0x07 };
    enum class PictureType : uint8_t { SnapshotPictureMode = 0x01, RAWPictureMode = 0x02, JPEGPictureMode = 0x05 };
    enum class SnapshotType : uint8_t { CompressedPictureJPEG = 0x00, UncompressedPictureRAW = 0x01 };

    enum class ResetType : uint8_t { SystemReset = 0x00, StateMachineReset = 0x01 };
    enum class DataType : uint8_t { SnapshotPicture = 0x01, RAWPicture = 0x02, JPEGPicture = 0x05 };
    enum class ErrorNumber : uint8_t {
        PictureTypeError = 0x01,
        ParameterError = 0x0B,
        PictureUpScale = 0x02,
        SendRegisterTimeout = 0x0C,
        PictureScaleError = 0x03,
        CommandIDError = 0x0D,
        UnexpectedReply = 0x04,
        PictureNotReady = 0x0F,
        SendPictureTimeout = 0x05,
        TransferPackageNumberError = 0x10,
        UnexpectedCommand = 0x06,
        SetTransferPackageSizeWrong = 0x11,
        SRAMJPEGTypeError = 0x07,
        CommandHeaderError = 0xF0,
        SRAMJPEGSizeError = 0x08,
        CommandLengthError = 0xF1,
        PictureFormatError = 0x09,
        SendPictureError = 0xF5,
        PictureSizeError = 0x0A,
        SendCommandError = 0xFF,
    };
    enum class LightFrequencyType : uint8_t {
        light50Hz = 0x00,
        light60Hz = 0x01,
    };
    enum class Baudrate {
        Baud_2400,
        Baud_4800,
        Baud_9600,
        Baud_19200,
        Baud_38400,
        Baud_57600,
        Baud_115200,
        Baud_153600,
        Baud_230400,
        Baud_460800,
        Baud_921600,
        Baud_1228800,
        Baud_1843200,
        Baud_3686400
    };

 private:
    class Command {
     public:
        Command() = default;

        constexpr CommandType getID() const { return IDNumber; }

     protected:
        constexpr Command(CommandType type, uint8_t parameter1 = 0x00, uint8_t parameter2 = 0x00, uint8_t parameter3 = 0x00, uint8_t parameter4 = 0x00)
        : IDNumber(type), parameter1(parameter1), parameter2(parameter2), parameter3(parameter3), parameter4(parameter4) {
        }

        template <typename T1 = uint8_t, typename T2 = uint8_t, typename T3 = uint8_t, typename T4 = uint8_t>
        constexpr Command(CommandType type, T1 parameter1 = 0x00, T2 parameter2 = 0x00, T3 parameter3 = 0x00, T4 parameter4 = 0x00)
            : IDNumber(type),
              parameter1(static_cast<uint8_t>(parameter1)),
              parameter2(static_cast<uint8_t>(parameter2)),
              parameter3(static_cast<uint8_t>(parameter3)),
              parameter4(static_cast<uint8_t>(parameter4)) {
        	static_assert(sizeof(T1) == 1, "");
        	static_assert(sizeof(T2) == 1, "");
        	static_assert(sizeof(T3) == 1, "");
        	static_assert(sizeof(T4) == 1, "");
        }

        constexpr uint8_t lsb(uint16_t a) { return a & 0xFF; }
        constexpr uint8_t msb(uint16_t a) { return (a >> 8) & 0xFF; }

        uint8_t preamble = 0xAA;
        CommandType IDNumber;
        uint8_t parameter1;
        uint8_t parameter2;
        uint8_t parameter3;
        uint8_t parameter4;
    };

    class Initial final : public Command {
     public:
        explicit constexpr Initial(ImageFormat imageFormat, RawResolution rawResolution, JpegResolution jpegResolution) noexcept
            : Command(CommandType::Initial, static_cast<uint8_t>(0x00), imageFormat, rawResolution, jpegResolution) {}
        static constexpr CommandType getID() { return CommandType::Initial; }
    };

    class GetPicture final : public Command {
     public:
        explicit constexpr GetPicture(PictureType pictureType) noexcept : Command(CommandType::GetPicture, pictureType) {}
        constexpr void setPictureType(PictureType type) noexcept { parameter1 = static_cast<uint8_t>(type); }
        static constexpr CommandType getID() { return CommandType::GetPicture; }
    };

    class Snapshot final : public Command {
     public:
        explicit constexpr Snapshot(SnapshotType snapshotType, uint32_t skipFrame = 0) noexcept : Command(CommandType::Snapshot, snapshotType, lsb(skipFrame), msb(skipFrame)) {}
        static constexpr CommandType getID() { return CommandType::Snapshot; }
    };

    class SetPackageSize final : public Command {
     public:
        explicit constexpr SetPackageSize(uint16_t size) noexcept : Command(CommandType::SetPackageSize, static_cast<uint8_t>(0x08), lsb(size), msb(size)) {}
        static constexpr CommandType getID() { return CommandType::SetPackageSize; }
    };

    class SetBaudrate final : public Command {
     public:
        explicit constexpr SetBaudrate() noexcept : Command(CommandType::SetBaudRate) {}
        static constexpr CommandType getID() { return CommandType::SetBaudRate; }

        bool setBaudrate(Baudrate baudrate);
    };

    class Reset final : public Command {
     public:
        explicit constexpr Reset(ResetType reset) noexcept : Command(CommandType::Reset, reset,(uint8_t)0,(uint8_t)0,(uint8_t)0xff) {}
        static constexpr CommandType getID() { return CommandType::Reset; }
    };

    class Data final : public Command {
     public:
        explicit constexpr Data(DataType dataType) noexcept : Command(CommandType::Data, dataType) {}
        static constexpr CommandType getID() { return CommandType::Data; }
        size_t size() const { return parameter4 << 16 | parameter3 << 8 | parameter2; }

        DataType getImageType() const { return static_cast<DataType>(parameter1); }
    };

    class Sync final : public Command {
     public:
        constexpr Sync() noexcept : Command(CommandType::Sync) {}
        static constexpr CommandType getID() { return CommandType::Sync; }
    };

    class Ack final : public Command {
     public:
        constexpr Ack() noexcept : Command(CommandType::Ack) {}
        explicit constexpr Ack(const Command &toAck) noexcept : Command(CommandType::Ack, toAck.getID()) {}
		static constexpr CommandType getID() { return CommandType::Ack; }
        bool isAcknowledged(const Command &command) const noexcept {
            if (command.getID() == getCommandID()) return true;
            return false;
        }

        void acknowledgeDataImagePacket(uint16_t packetNumber) noexcept {
            parameter1 = 0x00;
            setPackteID(packetNumber);
        }
        void setCommandID(CommandType type) { parameter1 = static_cast<uint8_t>(type); }
        CommandType getCommandID() const { return static_cast<CommandType>(parameter1); }

        void setCounter(uint8_t count) { parameter2 = count; }

     private:
        void setPackteID(uint16_t packetNumber) {
            // packageIDByte0
            parameter3 = lsb(packetNumber);
            // packageIDByte1
            parameter4 = msb(packetNumber);
        }
    };

    class Nak final : public Command {
     public:
        constexpr Nak(ErrorNumber error) noexcept : Command(CommandType::Nak, static_cast<uint8_t>(0x00), static_cast<uint8_t>(0x00), error) {}
        uint8_t getNACKCounter() { return parameter2;}
        ErrorNumber getError() { return static_cast<ErrorNumber>(parameter3); }
    };

    class Light final : public Command {
     public:
        explicit constexpr Light(LightFrequencyType frequency) noexcept : Command(CommandType::Light, frequency) {}
        LightFrequencyType getFrequency() { return static_cast<LightFrequencyType>(parameter1); }
    };

    class ImageDataPackage {
     public:
        uint16_t getID() const { return id; }
        size_t getImageSize() const { return size; }
        const uint8_t *getImagePtr() const { return imageData; }
        bool verify() const {
            const uint32_t sum = std::accumulate(begin(), end() - 2, 0);
            if ((sum & 0xFF) != getVerifyCode()) {
                microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Debug
                                                  << "Verification fail, calculated: " << (sum & 0xFF) << " expected: " << getVerifyCode()
                                                  << microhal::diagnostic::unlock;
                return false;
            }
            return true;
        }

     private:
        struct {
            uint16_t id;
            uint16_t size;
            uint8_t imageData[512 - 6];
            uint16_t verifyCode;
        };

        uint16_t getVerifyCode() const {
            const uint16_t *codePtr = reinterpret_cast<const uint16_t *>(end() - 2);
            return *codePtr;
        }

        size_t getSize() const { return sizeof(id) + sizeof(size) + getImageSize() + sizeof(verifyCode); }

        const uint8_t *begin() const { return reinterpret_cast<const uint8_t *>(&id); }
        const uint8_t *end() const { return begin() + getSize(); }
    };

 public:
    constexpr explicit uCAM_II(microhal::SerialPort &serial) noexcept : serial(serial) {}
    virtual ~uCAM_II();

    bool sync();

    bool initJPEG(JpegResolution resolution) noexcept {
        Initial init(ImageFormat::JPEG, RawResolution::Image_128x128, resolution);
        return writeCommandWaitForACK(init, std::chrono::milliseconds {140});
    }

    bool initRAW(ImageFormatRAW imageFormat, RawResolution resolution) noexcept {
        Initial init(static_cast<ImageFormat>(imageFormat), resolution, JpegResolution::Image_160x128);
        return writeCommandWaitForACK(init, std::chrono::milliseconds {140});
    }

    bool setBaudrate(Baudrate baudrate);

  	/**
  	 * This function set size of data package used to image send.
  	 */
    bool setPackageSize(size_t size) noexcept {
        if (size > 512) return false;
        SetPackageSize packageSize(size);
        return writeCommandWaitForACK(packageSize, std::chrono::milliseconds {140});
    }

    bool snapshot(SnapshotType snapshotType) noexcept {
        Snapshot command(snapshotType);
        lastSnapshot = snapshotType;
        return writeCommandWaitForACK(command, std::chrono::milliseconds {140});
    }

    bool getPicture(PictureType pictureType, gsl::span<uint8_t> pictureBuffer, size_t &received);

    constexpr static size_t calculateNumberOfPackets(size_t pictureSize, size_t packageSize) noexcept {
        const size_t fullPages = pictureSize / (packageSize - 6);
        const size_t notFullPage = pictureSize % (packageSize - 6) ? 1 : 0;
        return fullPages + notFullPage;
    }

    bool reset(ResetType resetType) noexcept {
    	Reset reset(resetType);
    	return writeCommand(reset);
    }

 private:
    microhal::SerialPort &serial;
    SnapshotType lastSnapshot = {};

    bool readPicturePackage(ImageDataPackage &package);
    bool receiveRAWPictureData(uint8_t *pictureData, size_t pictureSize);
    bool receiveJPEGPictureData(uint8_t *pictureData, size_t pictureSize, size_t packageSize);

    template <typename T>
    bool sendACK(const T &command) noexcept {
        Ack ack(command);
        return sendACK(ack);
    }

    bool sendACK(Ack &ack) noexcept {
        static uint8_t counter;
        ack.setCounter(counter++);
        return writeCommand(ack);
    }

    bool sendACK(const ImageDataPackage &imageDataPackage, bool isLast) noexcept {
        Ack ack;
        if (isLast) {
            ack.acknowledgeDataImagePacket(0xF0F0);
        } else {
            ack.acknowledgeDataImagePacket(imageDataPackage.getID());
        }
        return writeCommand(ack);
    }

    bool sendAck(const Data &data) noexcept {
        Ack ack;
        if (data.getImageType() == DataType::RAWPicture) {
            ack.setCommandID(CommandType::Data);
            return sendACK(ack);
        } else {
            ack.acknowledgeDataImagePacket(0x0000);
        }

        return writeCommand(ack);
    }

    bool writeCommand(const Command &command) {
        const bool status = serial.write(reinterpret_cast<const char *>(&command), sizeof(command));
        serial.waitForWriteFinish(std::chrono::milliseconds {100});
        return status;
    }

    std::experimental::optional<Command> readCommand(std::chrono::milliseconds timeout) {
        std::experimental::optional<Command> command;
        Command tmp;

        size_t size = serial.read((char *)&tmp, sizeof(tmp), timeout);
        if (size == sizeof(tmp)) {
            command = tmp;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Debug << "Unable to read command, received size: " << (uint32_t)size
                                              << "\n" << microhal::diagnostic::unlock;
        }
        return command;
    }

    bool writeCommandWaitForACK(const Command &command, std::chrono::milliseconds timeout) {
        if (writeCommand(command)) {
            if (auto receivedCommand = readCommand(timeout)) {
                switch (receivedCommand->getID()) {
                    case CommandType::Ack: {
                        Ack &ack = static_cast<Ack &>(*receivedCommand);
                        if (ack.isAcknowledged(command)) return true;
                    } break;
                        //				case CommandType::Nak:
                        //					break;
                    default: {
                    	microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Debug
							<< "After command: " << static_cast<uint8_t>(command.getID()) << "Expected ACK but, received command: " << static_cast<uint8_t>(receivedCommand->getID())<<"\n" << microhal::diagnostic::unlock;
                    //	asm volatile("BKPT #01");
                    }
                }
            }
        }
        return false;
    }
};

#endif  // _MICROHAL_UCAMII_H_
