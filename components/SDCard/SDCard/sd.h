/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      SD Card driver file
 *
 * @authors    Pawel Okas
 * created on: 29-07-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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

#ifndef SD_H_
#define SD_H_

#include <experimental/optional>
#include "diagnostic/diagnostic.h"
#include "gsl/gsl"
#include "microhal-crc.hpp"
#include "microhal.h"
#include "ports/manager/hardware.h"
#include "utils/byteswap.h"
#include "utils/packed.h"

class Sd final {
 public:
    using GPIO = microhal::GPIO;

    struct CSDv1 {
        uint8_t CSD_STRUCTURE : 2;
        // reserved : 6
        uint8_t TAAC;                    // data read access time 1
        uint8_t NSAC;                    // data read access time 2 in CLK cycles (NSAC * 100)
        uint8_t TRAN_SPEED;              // max. data transfer rate
        uint16_t CCC;                    // : 12;               // card command classes
        uint8_t READ_BL_LEN : 4;         // max. data block len
        uint8_t READ_BL_PARTIAL : 1;     // partial blocks for read allowed
        uint8_t WRITE_BLK_MISALIGN : 1;  // write block misalignment
        uint8_t READ_BLK_MISALIGN : 1;   // read block misalignment
        uint8_t DSR_IMP : 1;             // DSR implemented
        uint16_t C_SIZE;                 // : 12;            // device size
        uint8_t VDD_R_CURR_MIN : 3;      // max. read current @VDD min
        uint8_t VDD_R_CURR_MAX : 3;      // max. read current @VDD max
        uint8_t VDD_W_CURR_MIN : 3;      // max. write current @VDD min
        uint8_t VDD_W_CURR_MAX : 3;      // max. write current @VDD max
        uint8_t C_SIZE_MULT : 3;         // device size multiplier
        uint8_t ERASE_BLK_EN : 1;        // erase single block enable
        uint8_t SECTOR_SIZE : 7;         // erase sector size
        uint8_t WP_GRP_SIZE : 7;         // write protect group size
        uint8_t WP_GRP_ENABLE : 1;       // write protect group enable
        uint8_t R2W_FACTOR : 3;          // write speed factor
        uint8_t WRITE_BL_LEN : 4;        // max. write data block length
        uint8_t WRITE_BL_PARTIAL : 1;    // partial blocks for write allowed
        uint8_t FILE_FORMAT_GRP : 1;     // File format group
        uint8_t COPY : 1;                // copy flag
        uint8_t PERM_WRITE_PROTECT : 1;  // permanent write protection
        uint8_t TMP_WRITE_PROTECT : 1;   // temporary write protection
        uint8_t FILE_FORMAT : 2;         // File format
        // crc : 7
        // not used, alvays '1' : 1
    };

    struct CSDv2 {
        uint8_t CSD_STRUCTURE : 2;
        // reserved : 6
        uint8_t TAAC;                    // data read access time 1
        uint8_t NSAC;                    // data read access time 2 in CLK cycles (NSAC * 100)
        uint8_t TRAN_SPEED;              // max. data transfer rate
        uint16_t CCC;                    // : 12;                // card command classes
        uint8_t READ_BL_LEN : 4;         // max. data block len
        uint8_t READ_BL_PARTIAL : 1;     // partial blocks for read allowed
        uint8_t WRITE_BLK_MISALIGN : 1;  // write block misalignment
        uint8_t READ_BLK_MISALIGN : 1;   // read block misalignment
        uint8_t DSR_IMP : 1;             // DSR implemented
        // reserved : 6
        uint32_t C_SIZE;  // : 22;         // device size
        // reserved : 1
        uint8_t ERASE_BLK_EN : 1;   // erase single block enable
        uint8_t SECTOR_SIZE : 7;    // erase sector size
        uint8_t WP_GRP_SIZE : 7;    // write protect group size
        uint8_t WP_GRP_ENABLE : 1;  // write protect group enable
        // reserved : 2
        uint8_t R2W_FACTOR : 3;        // write speed factor
        uint8_t WRITE_BL_LEN : 4;      // max. write data block length
        uint8_t WRITE_BL_PARTIAL : 1;  // partial blocks for write allowed
        // reserved : 5
        uint8_t FILE_FORMAT_GRP : 1;     // File format group
        uint8_t COPY : 1;                // copy flag
        uint8_t PERM_WRITE_PROTECT : 1;  // permanent write protection
        uint8_t TMP_WRITE_PROTECT : 1;   // temporary write protection
        uint8_t FILE_FORMAT : 2;         // File format
        // reserved : 2
        // crc : 7
        // not used, always '1' : 1
    };

    // temporary type, when std::variant will be available this will be replaced
    struct CSD {
        uint8_t version;
        union {
            CSDv1 v1;
            CSDv2 v2;
        };
    };

    enum DataResponse { Accepted = 0b00000101, CRCError = 0b00001011, WriteError = 0b0000110, Timeout = 0b10000000 };

    enum class ReadDataError : uint8_t {
        None = 0,
        Error = 0b0001,
        CCError = 0b0010,
        CardECCFailed = 0b0100,
        OutOfRange = 0b1000,
        CRCMismatch = 0b0001'0000,
        Unknown = 0b1000'0000
    };

    class OCR {
     public:
        explicit OCR(uint32_t ocr) noexcept : ocr(ocr) {}
        bool getCCS() { return (ocr >> 30) & 0b1; }
        bool isCCSbitValid() { return getPowerStatus(); }
        bool getPowerStatus() { return (ocr >> 31) & 0b1; }

        uint32_t getRawForDebug() { return ocr; }

     private:
        uint32_t ocr;
    };

    enum class CardType { StandardCapacityVer1, StandardCapacityVer2, HighCapacityOrExtendedCapacity, Unknown };

    enum class Error {
        None = 0,
        WrongState = 0b0000'0001,
        // IllegalCommand = 0b0000'0010,
        WrongAddress = 0b0000'0100,
        DataCRC = 0b0000'1000,
        DataWriteError = 0b0001'0000,

        // CCError = 0b0010'0000,
        // ECCFailed = 0b0100'0000,
        OutOfRange = 0b0000'0010,
        // CardLocked,

        Unknown = 0b1000'0000
    };

    Sd(microhal::SPI &spi, GPIO &chipSelect) noexcept : spi(spi), cs(chipSelect) {
        cs.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);
        cs.set();
    }
    ~Sd();

    bool init();

    uint32_t setSpeed(uint32_t speed) {
        const auto newSpeed = spi.speed(speed);
        return newSpeed;
    }
    uint32_t getSpeed() const { return spi.speed(); }
    uint32_t getMaxSpeed() const { return spi.maxCLKFrequency(); }

    CardType getCardType() const noexcept { return cardType; }

    uint64_t getCardCapacity() const noexcept { return cardCapacity; }

    uint32_t getSecrorCount() const noexcept { return getCardCapacity() / blockSize; }

    uint32_t getLastPageNumber() const noexcept { return getCardCapacity() / 512 - 1; }

    uint32_t getBlockSize() const noexcept { return blockSize; }
    uint32_t getSectorSize() const noexcept { return blockSize; }
    /**
     * @brief This function may be used to set data block size. The function will work only with Standard Capacity
     * SC Memory Cards. In different SD Card types data block size is fixed to 512B and can't be adjusted. When working
     * with Standard Capacity SD Cards you can set block size from 1 to 512B.
     *
     * @param blockSize that will be set. Accepted range is from 1 to 512.
     */
    bool setBlockSize(uint32_t blockSize);

    Error readBlock(const gsl::not_null<void *> data_ptr, uint32_t address);

    Error writeBlock(const gsl::not_null<const void *> data_ptr, uint32_t address);

    Error readMultipleBlock(const gsl::not_null<void *> data_ptr, uint32_t address, uint32_t blocksCount);

    Error writeMultipleBlock(const gsl::not_null<const void *> data_ptr, uint32_t address, uint32_t blocksCount);

    std::optional<CSD> readCSD();

    bool readCID();

    std::experimental::optional<OCR> readOCR();

 private:
    microhal::SPI &spi;
    microhal::GPIO &cs;
    uint16_t blockSize = 512;
    CardType cardType = CardType::Unknown;
    uint64_t cardCapacity = 0;

    class Command {
     public:
        constexpr Command(uint8_t cmdIndex, uint32_t argument, uint8_t crc) noexcept
            : startBitTransmissionBitAndCommandIndex(0x40 | cmdIndex),
              argument(convertEndiannessIfRequired(argument, microhal::Endianness::Big)),
              crcAndEndBit(crc) {}

        void calculateCRC() {
            crcAndEndBit = microhal::CRC7<>::calculate(&startBitTransmissionBitAndCommandIndex, 5) << 1;
            crcAndEndBit |= 0x01;
        }

        void setArgument(uint32_t arg) {
            argument = convertEndiannessIfRequired(arg, microhal::Endianness::Big);
            calculateCRC();
        }

        uint32_t getArgument() { return convertEndiannessIfRequired(argument, microhal::Endianness::Big); }

     protected:
        Command(uint8_t cmdIndex, uint32_t argument) noexcept
            : startBitTransmissionBitAndCommandIndex(0x40 | cmdIndex), argument(convertEndiannessIfRequired(argument, microhal::Endianness::Big)) {
            calculateCRC();
        }

     private:
        uint8_t startBitTransmissionBitAndCommandIndex;  // start bit (MSB) always equal 0, transmission bit always equal 1, command index 6 LSB bits
        uint32_t argument;
        uint8_t crcAndEndBit;  // 7 MSB is CRC, 1 bit LSB is end bit always equal 1
    } MICROHAL_PACKED;
    static_assert(sizeof(Command) == 6, "");

    class CMD0 : public Command {
     public:
        explicit constexpr CMD0() : Command(0x00, 0x0, 0x95) {}
    };

    class CMD8 : public Command {
     public:
        enum class VoltageSupplied : uint8_t {
            NotDefined = 0x00,
            Voltage2_7To3_6 = 0x01,
        };

        explicit CMD8(VoltageSupplied voltage) : Command(0x08, 0x000000AA | (static_cast<uint8_t>(voltage) << 8)) {}

        void setVoltage(VoltageSupplied voltage) { setArgument(getArgument() | (static_cast<uint8_t>(voltage) << 8)); }
    };

    class CMD9 : public Command {
     public:
        explicit CMD9() : Command(9, 0x00) {}  // todo CRC can be calculated before compilation
    };

    class CMD10 : public Command {
     public:
        explicit CMD10() : Command(10, 0x00) {}  // todo CRC can be calculated before compilation
    };

    class CMD12 : public Command {
     public:
        explicit CMD12() : Command(12, 0, 0x61) {}
    };

    class CMD16 : public Command {
     public:
        explicit CMD16(uint32_t blockSize) : Command(16, blockSize) {}  // todo CRC can be calculated before compilation
    };

    class CMD17 : public Command {
     public:
        explicit CMD17(uint32_t address) : Command(17, address) {}
    };

    class CMD18 : public Command {
     public:
        explicit CMD18(uint32_t address) : Command(18, address) {}
    };

    class CMD24 : public Command {
     public:
        explicit CMD24(uint32_t addressOrBlockNumber) : Command(24, addressOrBlockNumber) {}
    };

    class CMD25 : public Command {
     public:
        explicit CMD25(uint32_t addressOrBlockNumber) : Command(25, addressOrBlockNumber) {}
    };
    // Application command indication
    class CMD55 : public Command {
     public:
        explicit CMD55() : Command(55, 0x0) {}  // todo CRC can be calculated before compilation
    };
    // read OCR command
    class CMD58 : public Command {
     public:
        explicit CMD58() : Command(58, 0x00) {}  // todo CRC can be calculated before compilation
    };
    // enable/disable CRC command
    class CMD59 : public Command {
     public:
        explicit CMD59(bool enable) : Command(59, enable ? 1 : 0) {}
    };

    class ACMD41 : public Command {
     public:
        explicit ACMD41(uint32_t hcs) : Command(41, 0x40000000) {}
    };

    class ACMD23 : public Command {
     public:
        explicit ACMD23(uint32_t count) : Command(23, count) {}
    };

    enum ResponseR1 : uint8_t {
        InIdleState = 0b0000'0001,
        EraseReset = 0b0000'0010,
        IllegalCommand = 0b0000'0100,
        ComCRCError = 0b000'1000,
        EraseSequenceError = 0b0001'0000,
        AddressError = 0b0010'0000,
        ParameterError = 0b0100'0000
    };

    struct ResponseR7 {
        ResponseR1 response;
        uint32_t data;
    };

    static constexpr uint32_t convertEndiannessIfRequired(uint32_t a, microhal::Endianness endianness) {
        return (endianness == microhal::hardware::Device::endianness) ? a : microhal::byteswap(a);
    }

    static constexpr uint16_t convertEndiannessIfRequired(uint16_t a, microhal::Endianness endianness) {
        return (endianness == microhal::hardware::Device::endianness) ? a : microhal::byteswap(a);
    }

    bool sendCMD(const Command &command);

    Error convertResponseR1ToError(ResponseR1);

    bool sendACMD(const Command &command) {
        static const CMD55 cmd55;
        sendCMD(cmd55);
        if (auto response = readResponseR1(1)) {
            return sendCMD(command);
        }
        return false;
    }

    bool enableCRC() {
        static const CMD59 cmd59(true);
        sendCMD(cmd59);
        if (auto response = readResponseR1(1)) {
            return true;
        }
        return false;
    }

    bool disableCRC() {
        static const CMD59 cmd59(false);
        sendCMD(cmd59);
        if (auto response = readResponseR1(1)) {
            return true;
        }
        return false;
    }

    bool readResponse(uint8_t &response) { return spi.read(response, 0xFF) == microhal::SPI::Error::None; }

    std::experimental::optional<ResponseR1> readResponseR1(uint8_t retryCount);

    bool initialize(bool hcs, std::chrono::milliseconds timeout);

    uint8_t readToken(uint8_t retryCount);

    ReadDataError readDataPacket(const gsl::not_null<void *> data_ptr, uint16_t size);

    bool writeDataPacket(const gsl::not_null<const void *> data_ptr, uint8_t dataToken, uint16_t blockSize);

    DataResponse readDataResponse(std::chrono::milliseconds timeout);

    uint32_t addressOrBlockNumber(uint32_t blockNumber) {
        return (cardType == CardType::StandardCapacityVer1 || cardType == CardType::StandardCapacityVer2) ? blockNumber * 512 : blockNumber;
    }

    //    bool readResponseR3(uint8_t &r1, uint32_t &ocr, uint8_t retryCount) {
    //        // do {
    //        //  if (auto response = readResponseR1(2)) {
    //        //    if (*response != 0xFF) {
    //        //      r1 = *response;
    //        // if ((*response & 0xFE) == 0) {
    //        // return spi.readBuffer(&ocr, sizeof(ocr), 0xFF) == microhal::SPI::Error::NoError;
    //        spi.read(r1, 0xFF);
    //        ocr = r1;
    //        spi.read(r1, 0xFF);
    //        ocr = r1 << 8;
    //        spi.read(r1, 0xFF);
    //        ocr = r1 << 16;
    //        spi.read(r1, 0xFF);
    //        ocr = r1 << 24;
    //        return true;
    //        //}
    //        // return false;
    //        //  }
    //        //}
    //        //} while (retryCount--);
    //        // return false;
    //    }

    std::experimental::optional<ResponseR7> readResponseR7(uint8_t retryCount);
};

constexpr Sd::Error operator&(Sd::Error a, Sd::Error b) {
    return static_cast<Sd::Error>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

constexpr Sd::ReadDataError operator&(Sd::ReadDataError a, Sd::ReadDataError b) {
    return static_cast<Sd::ReadDataError>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

#endif /* SD_H_ */
