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

#include "sd.h"
#include "utils/packed.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

constexpr uint8_t readDataTokenRetryCount = 100;

struct CID {
    uint8_t MID;     // Manufacturer ID;
    uint16_t OID;    // OEM/Application ID
    uint8_t PNM[5];  // Product name
    uint8_t PRV;     // Product revision
    uint32_t PSN;    // Product serial number
    uint16_t reserved : 4;
    uint16_t MDT : 12;      // Manufacturing date
    uint8_t crc : 7;        // CRC7 checksum
    uint8_t alwaysOne : 1;  // not used, always 1
} MICROHAL_PACKED;

static_assert(sizeof(CID) == 16, "");

void deserializeCSD(Sd::CSDv1 &csd, uint8_t *data) {
    // clang-format off
    // tmp: |  [0]  | [1] | [2] | [3] | [4] |  [5]  |        [6]            | [7] |    [8]    |    [9]    |    [10]   |  [11] |      [12]     |    [13]   |          [14]         | [15]  |
    //      | 2   6 |  8  |  8  |  8  |   8 | 4   4 | 1   1   1   1   2   2 |  8  | 2   3   3 | 3   3   2 | 1   1   6 | 1   7 | 1   2   3   2 | 2   1   5 |                       |       |
    // csd: | 2 |'6'|  8  |  8  |  8  |   12    | 4 | 1 | 1 | 1 | 1 |'2'|      12     | 3 | 3 | 3 | 3 |   3   | 1 |   7   | 7 | 1 |'2'| 3 |   4   | 1 |'5'| 1 | 1 | 1 | 1 | 2 |'2'| 7 | 1 |
    // clang-format on
    csd.CSD_STRUCTURE = data[0] >> 6;
    // uint8_t reserved : 6;
    csd.TAAC = data[1];
    csd.NSAC = data[2];
    csd.TRAN_SPEED = data[3];
    csd.CCC = (data[4] << 4) | (data[5] >> 4);
    csd.READ_BL_LEN = data[5] & 0x0F;
    csd.READ_BL_PARTIAL = (data[6] >> 7) & 0x01;
    csd.WRITE_BLK_MISALIGN = (data[6] >> 6) & 0x01;
    csd.READ_BLK_MISALIGN = (data[6] >> 5) & 0x01;
    csd.DSR_IMP = (data[6] >> 4) & 0x01;
    // uint32_t reserved2 : 6;
    csd.C_SIZE = (((uint16_t)(data[6] & 0b11)) << 10) | (((uint16_t)data[7]) << 2) | (data[8] >> 6);
    csd.VDD_R_CURR_MIN = (data[8] >> 3) & 0b111;
    csd.VDD_R_CURR_MAX = data[8] & 0b111;
    csd.VDD_W_CURR_MIN = ((data[9] >> 5) & 0b111);
    csd.VDD_W_CURR_MAX = ((data[9] >> 2) & 0b111);
    csd.C_SIZE_MULT = ((data[9] << 1) & 0b110) | (data[10] >> 7);
    csd.ERASE_BLK_EN = (data[10] >> 6) & 0b1;
    csd.SECTOR_SIZE = (data[10] << 2) | (data[11] >> 7);
    csd.WP_GRP_SIZE = data[11] >> 1;
    csd.WP_GRP_ENABLE = data[12] >> 7;
    // uint32_t reserved3 : 2;
    csd.R2W_FACTOR = (data[12] >> 2) & 0b111;
    csd.WRITE_BL_LEN = (data[12] & 0b11) << 2 | data[13] >> 6;
    csd.WRITE_BL_PARTIAL = (data[13] >> 5) & 0b1;
    // uint32_t reserved4 : 5;
    csd.FILE_FORMAT_GRP = (data[14] >> 7) & 0b1;
    csd.COPY = (data[14] >> 6) & 0b1;
    csd.PERM_WRITE_PROTECT = (data[14] >> 5) & 0b1;
    csd.TMP_WRITE_PROTECT = (data[14] >> 4) & 0b1;
    csd.FILE_FORMAT = (data[14] >> 2) & 0b11;
    // uint8_t reserved5 : 2;
}

void deserializeCSD(Sd::CSDv2 &csd, uint8_t *data) {
    // clang-format off
    // tmp: |  [0]  | [1] | [2] | [3] | [4] |  [5]  |        [6]        |  [7]  | [8] | [9] |    [10]   |  [11] |      [12]     |    [13]   |          [14]         | [15]  |
    //      | 2   6 |  8  |  8  |  8  |   8 | 4   4 | 1   1   1   1   4 | 2   6 |  8  |  8  | 1   1   6 | 1   7 | 1   2   3   2 | 2   1   5 |                       |       |
    // csd: | 2 |'6'|  8  |  8  |  8  |   12    | 4 | 1 | 1 | 1 | 1 | '6'   |      22       |'1'| 1 |   7   | 7 | 1 |'2'| 3 |   4   | 1 |'5'| 1 | 1 | 1 | 1 | 2 |'2'| 7 | 1 |
    // clang-format on

    csd.CSD_STRUCTURE = data[0] >> 6;
    // uint8_t reserved : 6;
    csd.TAAC = data[1];
    csd.NSAC = data[2];
    csd.TRAN_SPEED = data[3];
    csd.CCC = (data[4] << 4) | (data[5] >> 4);
    csd.READ_BL_LEN = data[5] & 0x0F;
    csd.READ_BL_PARTIAL = (data[6] >> 7) & 0x01;
    csd.WRITE_BLK_MISALIGN = (data[6] >> 6) & 0x01;
    csd.READ_BLK_MISALIGN = (data[6] >> 5) & 0x01;
    csd.DSR_IMP = (data[6] >> 4) & 0x01;
    // uint32_t reserved2 : 2;
    csd.C_SIZE = (((uint32_t)(data[7] & 0b111111)) << 16) | (((uint32_t)data[8]) << 8) | data[9];

    csd.ERASE_BLK_EN = (data[10] >> 6) & 0b1;
    csd.SECTOR_SIZE = (data[10] << 2) | (data[11] >> 7);
    csd.WP_GRP_SIZE = data[11] >> 1;
    csd.WP_GRP_ENABLE = data[12] >> 7;
    // uint32_t reserved3 : 2;
    csd.R2W_FACTOR = (data[12] >> 2) & 0b111;
    csd.WRITE_BL_LEN = (data[12] & 0b11) << 2 | data[13] >> 6;
    csd.WRITE_BL_PARTIAL = (data[13] >> 5) & 0b1;
    // uint32_t reserved4 : 5;
    csd.FILE_FORMAT_GRP = (data[14] >> 7) & 0b1;
    csd.COPY = (data[14] >> 6) & 0b1;
    csd.PERM_WRITE_PROTECT = (data[14] >> 5) & 0b1;
    csd.TMP_WRITE_PROTECT = (data[14] >> 4) & 0b1;
    csd.FILE_FORMAT = (data[14] >> 2) & 0b11;
    // uint8_t reserved5 : 2;
}

Sd::~Sd() {
    // TODO Auto-generated destructor stub
}

Sd::Error operator|=(Sd::Error &a, Sd::Error b) {
    a = static_cast<Sd::Error>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    return a;
}

void printCID(CID &cid) {
    char productName[6];
    memcpy(productName, cid.PNM, 5);
    productName[5] = 0;
    char appID[3];
    memcpy(appID, &cid.OID, 2);
    appID[2] = 0;

    uint16_t tmp = cid.MDT;  // convertEndiannessIfRequired(cid.MDT, Endianness::BigEndian);
    uint32_t year = (tmp & 0xFF) + 2000;
    uint8_t month = (tmp >> 8) & 0xF;

    diagChannel << lock << Notice << endl
                << "------ CID structure content ------" << endl
                << "Manufacturer ID: " << cid.MID << endl
                << "OEM/Application ID: " << appID << endl
                << "Product name: " << productName << endl
                << "Product revision: " << (uint8_t)(cid.PRV >> 4) << "." << (uint8_t)(cid.PRV & 0x0F) << endl
                << "Product serial number: " << toHex(cid.PSN) << endl
                << "Manufacturing date: " << month << "-" << year << endl
                << "CRC7 checksum: " << toHex(cid.crc) << endl
                << "not used, always 1: " << cid.alwaysOne << endl
                << unlock;
}

void printCSD(Sd::CSDv1 &csd) {
    diagChannel << lock << Notice << endl
                << "------ CSD structure content ------" << endl
                << "CSD structure version: " << csd.CSD_STRUCTURE << endl
                << "data read access time: " << csd.TAAC << endl
                << "data read access time 2 in CLK cycles (NSAC * 100): " << csd.NSAC << endl
                << "max. data transfer rate: " << csd.TRAN_SPEED << endl
                << "card command classes: " << csd.CCC << endl
                << "max. read data block len: " << csd.READ_BL_LEN << endl
                << "partial blocks for read allowed: " << csd.READ_BL_PARTIAL << endl
                << "write block misalignment: " << csd.WRITE_BLK_MISALIGN << endl
                << "read block misalignment: " << csd.READ_BLK_MISALIGN << endl
                << "DSR implemented: " << csd.DSR_IMP << endl
                << "device size: " << csd.C_SIZE << endl
                << "max. read current @VDD min: " << csd.VDD_R_CURR_MIN << endl
                << "max. read current @VDD max: " << csd.VDD_R_CURR_MAX << endl
                << "max. write current @VDD min: " << csd.VDD_W_CURR_MIN << endl
                << "max. write current @VDD max: " << csd.VDD_W_CURR_MAX << endl
                << "device size multiplier: " << csd.C_SIZE_MULT << endl
                << "erase single block enable: " << csd.ERASE_BLK_EN << endl
                << "erase sector size: " << csd.SECTOR_SIZE << endl
                << "write protect group size: " << csd.WP_GRP_SIZE << endl
                << "write protect group enable: " << csd.WP_GRP_ENABLE << endl
                << "write speed factor: " << csd.R2W_FACTOR << endl
                << "max. write data block length: " << csd.WRITE_BL_LEN << endl
                << "partial blocks for write allowed: " << csd.WRITE_BL_PARTIAL << endl
                << "File format group: " << csd.FILE_FORMAT_GRP << endl
                << "copy flag: " << csd.COPY << endl
                << "permanent write protection: " << csd.PERM_WRITE_PROTECT << endl
                << "temporary write protection: " << csd.TMP_WRITE_PROTECT << endl
                << "File format: " << csd.FILE_FORMAT << endl
                << unlock;
}

void printCSD(Sd::CSDv2 &csd) {
    diagChannel << lock << Notice << endl
                << "------ CSD structure content ------" << endl
                << "CSD structure version: " << csd.CSD_STRUCTURE << endl
                << "data read access time: " << csd.TAAC << endl
                << "data read access time 2 in CLK cycles (NSAC * 100): " << csd.NSAC << endl
                << "max. data transfer rate: " << csd.TRAN_SPEED << endl
                << "card command classes: " << csd.CCC << endl
                << "max. data block len: " << csd.READ_BL_LEN << endl
                << "partial blocks for read allowed: " << csd.READ_BL_PARTIAL << endl
                << "write block misalignment: " << csd.WRITE_BLK_MISALIGN << endl
                << "read block misalignment: " << csd.READ_BLK_MISALIGN << endl
                << "DSR implemented: " << csd.DSR_IMP << endl
                << "device size: " << csd.C_SIZE << endl
                << "erase single block enable: " << csd.ERASE_BLK_EN << endl
                << "erase sector size: " << csd.SECTOR_SIZE << endl
                << "write protect group size: " << csd.WP_GRP_SIZE << endl
                << "write protect group enable: " << csd.WP_GRP_ENABLE << endl
                << "write speed factor: " << csd.R2W_FACTOR << endl
                << "max. write data block length: " << csd.WRITE_BL_LEN << endl
                << "partial blocks for write allowed: " << csd.WRITE_BL_PARTIAL << endl
                << "File format group: " << csd.FILE_FORMAT_GRP << endl
                << "copy flag: " << csd.COPY << endl
                << "permanent write protection: " << csd.PERM_WRITE_PROTECT << endl
                << "temporary write protection: " << csd.TMP_WRITE_PROTECT << endl
                << "File format: " << csd.FILE_FORMAT << endl
                << unlock;
}

bool Sd::init() {
    bool result = false;
    static const uint8_t ff[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    cs.reset();

    spi.write(ff, sizeof(ff), true);

    volatile uint32_t delay = 1000;
    while (delay--)
        ;

    // send CMD0 to reset SD card and set the card into SPI communication mode
    if (sendCMD(CMD0{})) {
        if (auto response = readResponseR1(4)) {
            if (*response == ResponseR1::InIdleState) {
                if (sendCMD(CMD8{CMD8::VoltageSupplied::Voltage2_7To3_6})) {
                    if (auto response = readResponseR7(2)) {
                        if (response->response & ResponseR1::IllegalCommand) {
                            // CMD8 is illegal command, this means that we work with SD Memory Card in version 1.X or Not SD Memory Card
                            auto ocr = readOCR();

                            result = initialize(false, 10s);
                            cardType = CardType::StandardCapacityVer1;
                        } else {
                            // got response for CMD8, our card is SD Memory card version 2 or later.
                            const uint32_t cmd8_response_data = response->data;
                            diagChannel << lock << MICROHAL_DEBUG << "CMD8 response data: " << toHex(cmd8_response_data) << endl << unlock;
                            if (cmd8_response_data == 0x000001AA) {
                                enableCRC();
                                if (initialize(true, 10s)) {
                                    if (auto ocr = readOCR()) {
                                        diagChannel << lock << MICROHAL_DEBUG << "OCR: " << toHex(ocr->getRawForDebug()) << endl << unlock;
                                        if (ocr->isCCSbitValid()) {
                                            if (ocr->getCCS()) {
                                                result = true;
                                                cardType = CardType::HighCapacityOrExtendedCapacity;
                                                diagChannel << lock << MICROHAL_DEBUG
                                                            << "CCS bit in OCR register set. Detected High Capacity or "
                                                               "Extended Capacity SD card version 2 or later."
                                                            << endl
                                                            << unlock;
                                            } else {
                                                cardType = CardType::StandardCapacityVer2;
                                                // this driver supports only 512 bytes blocks, so we need to change block size in standard Capacity
                                                // card to 512 bytes
                                                result = setBlockSize(512);
                                                diagChannel
                                                    << lock << MICROHAL_DEBUG
                                                    << "CCS bit in OCR register equal 0. Detected Standard Capacity SD card version 2 or later."
                                                    << endl
                                                    << unlock;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cs.set();

    if (result) {
        // card was initialized successfully, we need also to read card capacity
        if (auto csd = readCSD()) {
            if (csd->version == 1) {
                uint32_t MULT = 1 << (csd->v1.C_SIZE_MULT + 2);
                uint32_t BLOCKNR = (csd->v1.C_SIZE + 1) * MULT;
                uint32_t BLOCK_LEN = 1 << csd->v1.READ_BL_LEN;
                cardCapacity = BLOCKNR * BLOCK_LEN;
            } else if (csd->version == 2) {
                cardCapacity = ((uint64_t)csd->v2.C_SIZE + 1) * 512 * 1024;
            } else {
                diagChannel << lock << MICROHAL_ERROR << "Unsupported CSD version." << endl << unlock;
                result = false;
            }
        } else {
            diagChannel << lock << MICROHAL_ERROR << "Unable to read CSD." << endl << unlock;
            result = false;
        }
    }
    diagnostic::diagChannel << diagnostic::lock << MICROHAL_DEBUG << "sd init result: " << result << diagnostic::unlock;
    return result;
}

std::experimental::optional<Sd::OCR> Sd::readOCR() {
    std::experimental::optional<OCR> ocr;
    if (sendCMD(CMD58{})) {
        if (auto response = readResponseR1(2)) {
            if ((*response == 0x01) || (*response == 0x00)) {
                uint32_t tmp;
                spi.read(&tmp, sizeof(tmp), 0xFF);
                ocr = static_cast<OCR>(convertEndiannessIfRequired(tmp, Endianness::Big));
            }
        }
    }
    return ocr;
}

bool Sd::initialize(bool hcs, std::chrono::milliseconds timeout) {
    uint32_t acmdArg = hcs ? 0x40000000 : 0;
    ACMD41 acmd41(acmdArg);

    std::chrono::milliseconds waitTime = 2ms;
    do {
        sendACMD(acmd41);
        if (auto response = readResponseR1(1)) {
            if (*response != 0x01) {
                uint32_t ocr;
                spi.read(&ocr, sizeof(ocr), 0xFF);
                return true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{waitTime});
        timeout -= waitTime;
    } while (timeout > 0ms);

    return false;
}

std::optional<Sd::CSD> Sd::readCSD() {
    std::optional<Sd::CSD> result;

    cs.reset();
    sendCMD(CMD9{});
    if (auto response = readResponseR1(2)) {
        if (*response == 0) {
            uint8_t tmp[16];
            readDataPacket(&tmp, sizeof(tmp));
            diagChannel << lock << Debug << endl << "------ RAW CSD structure content ------" << toHex(tmp, sizeof(tmp)) << unlock;
            // checking CRC
            uint8_t crc = CRC7<>::calculate(tmp, sizeof(tmp) - 1);
            if (((crc << 1) | 1) == tmp[15]) {
                // crc and always one math
                if (tmp[0] == 0) {
                    // CSD v1
                    CSDv1 csd;
                    deserializeCSD(csd, tmp);
                    // printCSD(csd);
                    CSD tmp;
                    tmp.version = 1;
                    tmp.v1 = csd;
                    result = tmp;
                } else /*if (tmp[0] == 1)*/ {
                    // CSD v2
                    CSDv2 csd;
                    deserializeCSD(csd, tmp);
                    // printCSD(csd);
                    CSD tmp;
                    tmp.version = 2;
                    tmp.v2 = csd;
                    result = tmp;
                }
            } else {
                diagChannel << lock << MICROHAL_ERROR << "Reading CSD structure failed, CRC mismath." << unlock;
            }
        }
    }
    cs.set();
    return result;
}

bool Sd::readCID() {
    cs.reset();
    sendCMD(CMD10{});
    if (auto response = readResponseR1(2)) {
        if (*response == 0) {
            CID cid;
            readDataPacket(&cid, sizeof(cid));

            printCID(cid);
        }
    }
    cs.set();
    return true;
}

bool Sd::setBlockSize(uint32_t blockSize) {
    bool status = false;
    if (blockSize != 0 || blockSize <= 512) {
        cs.reset();
        sendCMD(CMD16{blockSize});
        if (auto response = readResponseR1(2)) {
            if (*response == 0x00) {
                this->blockSize = blockSize;
                status = true;
            }
        }
        cs.set();
    }
    return status;
}

Sd::DataResponse Sd::readDataResponse(std::chrono::milliseconds timeout) {
    DataResponse response = DataResponse::Timeout;
    auto time = timeout.count();
    do {
        uint8_t dataResponse;
        spi.read(dataResponse, 0xFF);
        if (dataResponse != 0xFF) {
            response = static_cast<DataResponse>(dataResponse & 0x1F);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    } while (time--);
    return response;
}

bool Sd::writeDataPacket(const gsl::not_null<const void *> data_ptr, uint8_t dataToken, uint16_t blockSize) {
    const uint16_t crc = convertEndiannessIfRequired(microhal::CRC16_XMODEM<>::calculate((uint8_t *)data_ptr.get(), blockSize), Endianness::Big);
    // write 0xFE to signalizes start of data transmission
    if (spi.write(dataToken, false) != SPI::Error::None) return false;
    // write data
    if (spi.write(data_ptr.get(), blockSize, false) != SPI::Error::None) return false;
    // write crc
    if (spi.write(&crc, sizeof(crc), false) != SPI::Error::None) return false;

    return true;
}

Sd::Error Sd::readBlock(const gsl::not_null<void *> data_ptr, uint32_t blockNumber) {
    Error error = Error::Unknown;
    cs.reset();
    if (sendCMD(CMD17{addressOrBlockNumber(blockNumber)})) {  // depending on card type, CMD17 parameter can be block number or memory address
        if (auto response = readResponseR1(2)) {
            if (*response == 0) {
                ReadDataError response = readDataPacket(data_ptr, blockSize);
                if (response == ReadDataError::None) {
                    error = Error::None;
                } else {
                    if ((response & ReadDataError::CRCMismatch) == ReadDataError::CRCMismatch) error |= Error::DataCRC;
                    if ((response & ReadDataError::OutOfRange) == ReadDataError::OutOfRange) error |= Error::OutOfRange;
                }
            } else {
                error = convertResponseR1ToError(*response);
                if (error == Error::None) error = Error::Unknown;
            }
        } else {
            diagChannel << lock << MICROHAL_ERROR << "DUPA." << unlock;
        }
    }
    cs.set();
    return error;
}

Sd::Error Sd::writeBlock(const gsl::not_null<const void *> data_ptr, uint32_t blockNumber) {
    Error error = Error::Unknown;
    cs.reset();
    if (sendCMD(CMD24{addressOrBlockNumber(blockNumber)})) {  // depending on card type, CMD24 parameter can be block number or memory address
        if (auto response = readResponseR1(2)) {
            if (*response == 0) {
                if (writeDataPacket(data_ptr, 0xFE, blockSize)) {
                    DataResponse response = readDataResponse(100ms);
                    if (response == DataResponse::Accepted) {
                        error = Error::None;
                        // send eight clocks to begin internal write operation
                        uint8_t tmp[] = {0xFF};
                        spi.write(tmp, sizeof(tmp), true);
                    } else {
                        error = Error::None;
                        if (response == DataResponse::CRCError) error |= Error::DataCRC;
                        if (response == DataResponse::WriteError) error |= Error::DataWriteError;
                    }
                }
            } else {
                error = convertResponseR1ToError(*response);
                if (error == Error::None) error = Error::Unknown;
            }
        }
    }
    cs.set();
    return error;
}

Sd::Error Sd::readMultipleBlock(const gsl::not_null<void *> data_ptr, uint32_t blockNumber, uint32_t blocksCount) {
    Error error = Error::Unknown;
    cs.reset();
    if (sendCMD(CMD18{addressOrBlockNumber(blockNumber)})) {  // depending on card type, CMD18 parameter can be block number or memory address
        if (auto response = readResponseR1(2)) {
            if (*response == 0) {
                uint8_t *ptr = static_cast<uint8_t *>(data_ptr.get());
                for (uint32_t i = 0; i < (blocksCount - 1); i++) {
                    ReadDataError response = readDataPacket(ptr, blockSize);
                    ptr += blockSize;
                    if (response != ReadDataError::None) {
                        if ((response & ReadDataError::CRCMismatch) == ReadDataError::CRCMismatch) error |= Error::DataCRC;
                        if ((response & ReadDataError::OutOfRange) == ReadDataError::OutOfRange) error |= Error::OutOfRange;
                        break;
                    }
                }
                // receive last block
                if (error == Error::Unknown) {
                    uint8_t token = readToken(readDataTokenRetryCount);
                    if (token == 0xFE) {
                        // lets try to synchronize end of CMD12 with end of data packet
                        uint16_t crc;
                        const size_t dataPacketSize = blockSize + sizeof(crc);
                        spi.read(ptr, dataPacketSize - sizeof(CMD12), 0xFF);
                        uint8_t tmp[sizeof(CMD12) + 1];
                        // tmp contain last packet data and CRC
                        CMD12 cmd12;
                        spi.writeRead(tmp, &cmd12, sizeof(cmd12) + 1);
                        // sendCMD(cmd12);
                        if (auto response = readResponseR1(2)) {
                            if (*response == 0) {
                                memcpy(ptr + dataPacketSize - sizeof(CMD12), tmp, sizeof(tmp) - sizeof(crc));
                                memcpy(&crc, tmp + sizeof(tmp) - sizeof(crc), sizeof(crc));

                                crc = convertEndiannessIfRequired(crc, Endianness::Big);
                                error = Error::None;
                            }
                        }
                    } else {
                        // an error occurred, and error token was received
                        if ((token & 0b1110'0000) == 0) {
                            // received error token
                            ReadDataError response = static_cast<ReadDataError>(token);

                            diagChannel << lock << MICROHAL_ERROR << "Received error data token: " << toBin(token) << unlock;
                            if (response != ReadDataError::None) {
                                error = Error::None;
                                if ((response & ReadDataError::CRCMismatch) == ReadDataError::CRCMismatch) error |= Error::DataCRC;
                                if ((response & ReadDataError::OutOfRange) == ReadDataError::OutOfRange) error |= Error::OutOfRange;
                                if (error == Error::None) error = Error::Unknown;
                            }
                        }
                    }
                }
            } else {
                error = convertResponseR1ToError(*response);
                if (error == Error::None) error = Error::Unknown;
            }
        }
    }
    cs.set();
    return error;
}

Sd::Error Sd::writeMultipleBlock(const gsl::not_null<const void *> data_ptr, uint32_t blockNumber, uint32_t blocksCount) {
    Error result = Error::Unknown;

    cs.reset();

    sendACMD(ACMD23{blocksCount}); /* Predefine number of sectors */
    readResponseR1(1);
    sendCMD(CMD25{addressOrBlockNumber(blockNumber)});  // depending on card type, CMD25 parameter can be block number or memory address
    if (auto response = readResponseR1(2)) {
        if (*response == 0) {
            // send one byte, required by standard
            spi.write(0xFF, false);
            // start sending data
            const uint8_t *ptr = static_cast<const uint8_t *>(data_ptr.get());
            bool errorOccurred = false;
            for (uint32_t block = 0; block < blocksCount; block++) {
                if (writeDataPacket(ptr, 0b1111'1100, blockSize)) {
                    ptr += blockSize;
                    DataResponse response = readDataResponse(100ms);
                    if (response != DataResponse::Accepted) {
                        result = Error::None;
                        if (response & DataResponse::CRCError) result |= Error::DataCRC;
                        if (response & DataResponse::WriteError) result |= Error::DataWriteError;
                        errorOccurred = true;
                        break;
                    }
                }
                // busy wait
                if (!spi.getMISOstate()) {
                    do {
                        spi.write(0xFF, true);
                    } while (!spi.getMISOstate());
                }
            }
            if (errorOccurred == false) {
                // write stop transmission token for CMD25
                constexpr uint8_t stopToken = 0b1111'1101;
                const uint8_t tmp[] = {stopToken, 0xFF};
                spi.write(tmp, sizeof(tmp), true);
                result = Error::None;
            }
        } else {
            result = convertResponseR1ToError(*response);
            if (result == Error::None) result = Error::Unknown;
        }
    }

    cs.set();
    return result;
}

Sd::Error Sd::convertResponseR1ToError(ResponseR1 response) {
    Error error = Error::None;
    if (response & ResponseR1::InIdleState) error |= Error::WrongState;
    if (response & ResponseR1::ParameterError) error |= Error::OutOfRange;
    return error;
}

bool Sd::sendCMD(const Command &command) {
    // check busy state
    if (!spi.getMISOstate()) {
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds{5});
            spi.write(0xFF, false);
        } while (!spi.getMISOstate());
    }
    uint8_t buffer[sizeof(command) + 1];
    buffer[0] = 0xFF;
    memcpy(&buffer[1], &command, sizeof(command));
    return spi.write(buffer, sizeof(buffer), true) == microhal::SPI::Error::None;
}

std::experimental::optional<Sd::ResponseR1> Sd::readResponseR1(uint8_t retryCount) {
    std::experimental::optional<ResponseR1> response;

    do {
        uint8_t resp;
        if (spi.read(resp, 0xFF) == microhal::SPI::Error::None) {
            // if MSB of resp byte is 0 then received R1 response
            if ((resp & 0b1000'0000) == 0) {
                //  microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_DEBUG
                //                                    << "Response: " << microhal::diagnostic::toHex(resp) << microhal::diagnostic::unlock;
                response = static_cast<ResponseR1>(resp);
                break;
            }
        }
    } while (retryCount--);
    return response;
}

/**
 * This response is send by SD card after receiving CMD8. R7 response contain R1 response and 32 bits of R7 response data.
 * @param retryCount
 * @return
 */
std::experimental::optional<Sd::ResponseR7> Sd::readResponseR7(uint8_t retryCount) {
    std::experimental::optional<Sd::ResponseR7> response;

    if (auto r1Response = readResponseR1(retryCount)) {
        ResponseR7 r7Response;
        r7Response.response = *r1Response;
        if ((*r1Response & ResponseR1::IllegalCommand) == 0) {
            // if not illegal command then read R7 response data
            uint32_t response_data;
            spi.read(&response_data, sizeof(response_data), 0xFF);
            response_data = convertEndiannessIfRequired(response_data, Endianness::Big);
            r7Response.data = response_data;
        }

        response = r7Response;
    }
    return response;
}

uint8_t Sd::readToken(uint8_t retryCount) {
    uint8_t token;
    do {
        spi.read(token, 0xFF);
    } while (token == 0xFF && retryCount--);
    return token;
}

Sd::ReadDataError Sd::readDataPacket(const gsl::not_null<void *> data_ptr, uint16_t size) {
    ReadDataError error = ReadDataError::Unknown;
    uint8_t token = readToken(readDataTokenRetryCount);
    if (token == 0xFE) {
        /*if (*/ spi.read(data_ptr.get(), size, 0xFF);  // == size) {
        uint16_t crc;
        spi.read(&crc, sizeof(crc), 0xFF);
        crc = convertEndiannessIfRequired(crc, Endianness::Big);
        // check crc
        const uint16_t crc_calculated = CRC16_XMODEM<>::calculate((uint8_t *)data_ptr.get(), size);
        if (crc_calculated == crc) {
            error = ReadDataError::None;
        } else {
            diagChannel << lock << MICROHAL_ERROR << "CRC mismatch while reading data, CRC received: " << toHex(crc)
                        << ", calculated: " << toHex(crc_calculated) << unlock;

            error = ReadDataError::CRCMismatch;
        }
        //}
    } else {
        // an error occurred, and error token was received
        if ((token & 0b1110'0000) == 0) {
            // received error token
            error = static_cast<ReadDataError>(token);

            diagChannel << lock << MICROHAL_ERROR << "Received error data token: " << toBin(token) << unlock;
        } else {
            error = ReadDataError::Unknown;
            diagChannel << lock << MICROHAL_ERROR << "Unable to receive data tocken." << toBin(token) << unlock;
        }
    }
    return error;
}
