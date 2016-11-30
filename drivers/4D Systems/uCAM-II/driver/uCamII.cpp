/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "uCamII.h"

using namespace std::chrono_literals;

uCAM_II::~uCAM_II() {
    // TODO Auto-generated destructor stub
}

bool uCAM_II::sync() {
	static constexpr uint8_t retryCount = 60;
    Sync sync;
    for (uint8_t i = 0; i < retryCount; i++) {
        if (writeCommandWaitForACK(sync, 1ms)) {
            // after ACK we should receive SYNC frame
            if (auto command = readCommand(1ms)) {
                if (command->getID() == Sync::getID()) {
                    // we need to respond with ACK
                    sendACK(*command);
                    return true;
                }
            }
        }
        // delay according to 4d systems documentation
        std::chrono::milliseconds delay(5 + i);
        std::this_thread::sleep_for(delay);
    }
    return false;
}
//todo
bool uCAM_II::setBaudrate(Baudrate baudrate) {
    SetBaudrate baud;
    baud.setBaudrate(baudrate);
    return writeCommandWaitForACK(baud, 100ms);
}

bool uCAM_II::SetBaudrate::setBaudrate(Baudrate baudrate) {
	uint8_t &firstDivider = parameter1;
	uint8_t &secondDivider = parameter2;
	switch (baudrate) {
		case Baudrate::Baud_2400:
			firstDivider = 31;
			secondDivider = 47;
			break;
		case Baudrate::Baud_4800:
			firstDivider = 31;
			secondDivider = 23;
			break;
		case Baudrate::Baud_9600:
			firstDivider = 31;
			secondDivider = 11;
			break;
		case Baudrate::Baud_19200:
			firstDivider = 31;
			secondDivider = 5;
			break;
		case Baudrate::Baud_38400:
			firstDivider = 31;
			secondDivider = 2;
			break;
		case Baudrate::Baud_57600:
			firstDivider = 31;
			secondDivider = 1;
			break;
		case Baudrate::Baud_115200:
			firstDivider = 31;
			secondDivider = 0;
			break;
		case Baudrate::Baud_153600:
			firstDivider = 7;
			secondDivider = 2;
			break;
		case Baudrate::Baud_230400:
			firstDivider = 7;
			secondDivider = 1;
			break;
		case Baudrate::Baud_460800:
			firstDivider = 7;
			secondDivider = 0;
			break;
		case Baudrate::Baud_921600:
			firstDivider = 1;
			secondDivider = 1;
			break;
		case Baudrate::Baud_1228800:
			firstDivider = 2;
			secondDivider = 0;
			break;
		case Baudrate::Baud_1843200:
			firstDivider = 1;
			secondDivider = 0;
			break;
		case Baudrate::Baud_3686400:
			firstDivider = 0;
			secondDivider = 0;
			break;
	}
	return false;
}
bool uCAM_II::getPicture(PictureType pictureType, gsl::span<uint8_t> pictureBuffer, size_t &received) {
    GetPicture picture(pictureType);
    if (writeCommandWaitForACK(picture, 1ms)) {
        if (auto command = readCommand(200ms)) {
        	if (command->getID() == Data::getID()) {
				Data &data = static_cast<Data &>(*command);
				microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Informational << "Image size: " << (uint32_t)data.size()
												  << microhal::diagnostic::endl << microhal::diagnostic::unlock;
				if (pictureBuffer.size_bytes() >= data.size()) {
					received = data.size();
					if (data.getImageType() == DataType::JPEGPicture ||
						((data.getImageType() == DataType::SnapshotPicture) && (lastSnapshot == SnapshotType::CompressedPictureJPEG))) {
						sendAck(data);
						return receiveJPEGPictureData(pictureBuffer.data(), data.size(), 512);
					} else if (data.getImageType() == DataType::RAWPicture ||
							   ((data.getImageType() == DataType::SnapshotPicture) && (lastSnapshot == SnapshotType::UncompressedPictureRAW))) {
						bool status = receiveRAWPictureData(pictureBuffer.data(), data.size());
						sendACK(data);
						return status;
					}
				} else {
					microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Warning
													  << "Image size is to large to fits into passed pictureBuffer." << microhal::diagnostic::unlock;
				}
        	}
        }
    }
    return false;
}

bool uCAM_II::readPicturePackage(ImageDataPackage &package) {
    char *packagePtr = reinterpret_cast<char *>(&package);
    size_t size;
    if((size = serial.read(packagePtr, 4, 1s)) == 4) {
        size_t packetSize = package.getImageSize() + 2;
        if ((size = serial.read(packagePtr + 4, packetSize, 1s)) == packetSize) {
        	return true;
        }
    }

    microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_ERROR << "Timeout while reading picture. Data in buffer: "
    		<< (uint32_t)size << microhal::diagnostic::unlock;
    return false;
}

bool uCAM_II::receiveRAWPictureData(uint8_t *pictureData, size_t pictureSize) {
    size_t leftToRead = pictureSize;
    uint32_t timeout = 0;
    do {
        size_t read = serial.read(reinterpret_cast<char *>(pictureData), leftToRead);
        pictureData += read;
        leftToRead -= read;
        if (leftToRead == 0) return true;

        std::this_thread::sleep_for(std::chrono::milliseconds{5});
        timeout++;
    } while (leftToRead && timeout < 10000);

    microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_ERROR << "Error, left to read: " << (uint32_t)leftToRead <<
    microhal::diagnostic::unlock;
    return false;
}
bool uCAM_II::receiveJPEGPictureData(uint8_t *pictureData, size_t pictureSize, size_t packageSize) {
    const size_t numberOfPackages = calculateNumberOfPackets(pictureSize, packageSize);
    const uint8_t *end = pictureData + pictureSize;
    ImageDataPackage imageDataPackage;

    for (uint16_t currentPacket = 0; currentPacket < numberOfPackages; currentPacket++) {
        if (readPicturePackage(imageDataPackage)) {
            if (imageDataPackage.verify()) {
                memcpy(pictureData, imageDataPackage.getImagePtr(), imageDataPackage.getImageSize());
                pictureData += imageDataPackage.getImageSize();
                sendACK(imageDataPackage, pictureData == end);
            } else {
                microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Error << "Error in package verification."
                                                  << microhal::diagnostic::unlock;
            }
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Error << "Error while reading image package"
                                              << microhal::diagnostic::unlock;
            return false;
        }
    }
    return true;
}
