/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic example main file
 *
 * @authors    Pawel Okas
 * created on: 27-06-2016
 * last modification: 27-06-2016
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

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "uCamII.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

uCAM_II uCam(bsp::cameraPort);

void saveFile(const char *filename, uint8_t *data, size_t size) {
    static char buff[2 * 1024];
    if (FILE *f = fopen(filename, "wb")) {
        setvbuf(f, buff, _IOFBF, sizeof(buff));  // this give little speed up when using embedded platform
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

void singleCaptureTest(void) {
    bsp::cameraPort.setBaudRate(115200);
    bsp::cameraPort.setDataBits(SerialPort::DataBits::Data8);
    bsp::cameraPort.setStopBits(SerialPort::StopBits::OneStop);
    bsp::cameraPort.setParity(SerialPort::Parity::NoParity);

    if (!uCam.reset(uCAM_II::ResetType::StateMachineReset)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to reset camera." << unlock;
        return;
    }

    if (!uCam.sync()) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to synchronize with camera." << unlock;
        return;
    }

    if (!uCam.initJPEG(uCAM_II::JpegResolution::Image_640x480)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to initialize camera." << unlock;
    }

    if (!uCam.setPackageSize(512)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to set package size" << unlock;
    }

    static uint8_t data[45000];
    size_t received;

    // --------------------------------------- get JPEG picture --------------------------------------------------------
    if (!uCam.getPicture(uCAM_II::PictureType::JPEGPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
    } else {
        diagChannel << lock << MICROHAL_DEBUG << "Saving jpeg picture." << unlock;
        saveFile("jpeg.jpg", data, received);
    }
    // --------------------------------------- get snapshot JPEG picture --------------------------------------------------------
    if (!uCam.snapshot(uCAM_II::SnapshotType::CompressedPictureJPEG)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to take snapshot." << unlock;
    }

    if (!uCam.getPicture(uCAM_II::PictureType::JPEGPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
    } else {
        saveFile("snapshot.jpg", data, received);
    }
    // --------------------------------------- reinit to RAW mode --------------------------------------------------------
    if (!uCam.initRAW(uCAM_II::ImageFormatRAW::Colour16bit, uCAM_II::RawResolution::Image_160x120)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to initialize camera." << unlock;
    }
    // --------------------------------------- get RAW picture --------------------------------------------------------
    if (!uCam.getPicture(uCAM_II::PictureType::RAWPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read RAW picture." << unlock;
    } else {
        diagChannel << lock << MICROHAL_DEBUG << "Saving raw picture." << unlock;
        saveFile("raw.dat", data, received);
    }
    // --------------------------------------- get snapshot RAW picture --------------------------------------------------------
    if (!uCam.snapshot(uCAM_II::SnapshotType::UncompressedPictureRAW)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to take snapshot" << unlock;
    }

    if (!uCam.getPicture(uCAM_II::PictureType::SnapshotPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
    } else {
        diagChannel << lock << MICROHAL_DEBUG << "Saving raw snapshot." << unlock;
        saveFile("snapshot.dat", data, received);
    }
}

void multipleCaptureTest(void) {
    bsp::cameraPort.setBaudRate(115200);
    bsp::cameraPort.setDataBits(SerialPort::DataBits::Data8);
    bsp::cameraPort.setStopBits(SerialPort::StopBits::OneStop);
    bsp::cameraPort.setParity(SerialPort::Parity::NoParity);

    if (!uCam.reset(uCAM_II::ResetType::StateMachineReset)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to reset camera." << unlock;
        return;
    }

    if (!uCam.sync()) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to synchronize with camera." << unlock;
        return;
    }
    diagChannel << lock << MICROHAL_DEBUG << "Synchronized with baudrate 115200." << unlock;

    uCam.setBaudrate(uCAM_II::Baudrate::Baud_921600);
    bsp::cameraPort.setBaudRate(921600);

    if (!uCam.sync()) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to synchronize with camera." << unlock;
        return;
    }

    diagChannel << lock << MICROHAL_DEBUG << "Synchronized with new transmission baudrate." << unlock;

    if (!uCam.initJPEG(uCAM_II::JpegResolution::Image_640x480)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to initialize camera." << unlock;
        return;
    }

    if (!uCam.setPackageSize(512)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to set package size" << unlock;
        return;
    }

    static uint8_t data[45000];
    size_t received;

    uint32_t i = 0;
    std::string fileName = "jpeg";
    while (i < 300) {
        i++;
        std::string temp(fileName + std::to_string(i) + ".jpg");
        // --------------------------------------- get JPEG picture --------------------------------------------------------
        if (!uCam.getPicture(uCAM_II::PictureType::JPEGPictureMode, data, received)) {
            diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
            return;
        } else {
            saveFile(temp.c_str(), data, received);
        }
    }
}

int main(void) {
    bsp::debugPort.open(IODevice::ReadWrite);
    bsp::debugPort.write("\n\r------------------- uCAM-II driver example -------------------------\n\r");

    // lets check if diagChannal is working
    diagChannel << lock << MICROHAL_EMERGENCY << "Information from diagnostic channel.\n" << unlock;

    if (!bsp::cameraPort.open(SerialPort::OpenMode::ReadWrite)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to open serial port." << unlock;
        return -1;
    }

    multipleCaptureTest();
    // singleCaptureTest();

    if (bsp::cameraPort.isOpen()) bsp::cameraPort.close();
    return 0;
}
