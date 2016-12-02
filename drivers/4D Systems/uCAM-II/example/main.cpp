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

#include <string.h>
#include <fstream>
#include <iostream>

#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "microhal_bsp.h"

#include "uCamII.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

uCAM_II uCam(cameraPort);

void saveFile(const char *filename, uint8_t *data, size_t size) {
	 std::ofstream myfile(filename, std::ios::out | std::ios::binary);
	 for (size_t i = 0; i < size; i++) {
		 myfile << data[i];
	 }
	 myfile.close();
}

void singleCaptureTest(void)
{
    cameraPort.setBaudRate(115200);
    cameraPort.setDataBits(SerialPort::DataBits::Data8);
    cameraPort.setStopBits(SerialPort::StopBits::OneStop);
    cameraPort.setParity(SerialPort::Parity::NoParity);

	if (!uCam.reset(uCAM_II::ResetType::StateMachineReset))
	{
		diagChannel << lock << MICROHAL_DEBUG << "Unable to reset camera." << unlock;
		return;
	}

    if (!uCam.sync()) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to synchronize with camera." << unlock;
    }

    if (!uCam.initJPEG(uCAM_II::JpegResolution::Image_640x480)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to initialize camera." << unlock;
    }

    if (!uCam.setPackageSize(512)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to set package size" << unlock;
    }

    uint8_t data[1000000];
    size_t received;

    // --------------------------------------- get JPEG picture --------------------------------------------------------
    if (!uCam.getPicture(uCAM_II::PictureType::JPEGPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
    } else {
        saveFile("jpeg.jpg", data, received);
    }
    // --------------------------------------- get snapshot JPEG picture --------------------------------------------------------
    if (!uCam.snapshot(uCAM_II::SnapshotType::CompressedPictureJPEG)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to take snapshot" << unlock;
    }

    if (!uCam.getPicture(uCAM_II::PictureType::JPEGPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
    } else {
        saveFile("snapshot_jpeg.jpg", data, received);
    }

    // --------------------------------------- reinit to RAW mode --------------------------------------------------------
    if (!uCam.initRAW(uCAM_II::ImageFormatRAW::Colour16bit, uCAM_II::RawResolution::Image_160x120)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to initialize camera." << unlock;
    }

    // --------------------------------------- get RAW picture --------------------------------------------------------
    if (!uCam.getPicture(uCAM_II::PictureType::RAWPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read RAW picture." << unlock;
    } else {
        saveFile("raw.dat", data, received);
    }
    // --------------------------------------- get snapshot RAW picture --------------------------------------------------------
    if (!uCam.snapshot(uCAM_II::SnapshotType::UncompressedPictureRAW)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to take snapshot" << unlock;
    }

    if (!uCam.getPicture(uCAM_II::PictureType::SnapshotPictureMode, data, received)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
    } else {
        saveFile("snapshot_raw.dat", data, received);
    }

}

void multipleCaptureTest(void)
{
	cameraPort.setBaudRate(115200);
	cameraPort.setDataBits(SerialPort::DataBits::Data8);
	cameraPort.setStopBits(SerialPort::StopBits::OneStop);
	cameraPort.setParity(SerialPort::Parity::NoParity);

	if (!uCam.reset(uCAM_II::ResetType::StateMachineReset))
	{
		diagChannel << lock << MICROHAL_DEBUG << "Unable to reset camera." << unlock;
		return;
	}

	if (!uCam.sync())
	{
		diagChannel << lock << MICROHAL_DEBUG << "Unable to synchronize with camera."
				<< unlock;
		return;
	}
	diagChannel << lock << MICROHAL_DEBUG << "Synchronized with baudrate 115200." << unlock;

	uCam.setBaudrate(uCAM_II::Baudrate::Baud_460800);

	cameraPort.setBaudRate(460800);

	if (!uCam.sync())
	{
		diagChannel << lock << MICROHAL_DEBUG << "Unable to synchronize with camera."
				<< unlock;
		return;
	}

	diagChannel << lock << MICROHAL_DEBUG << "Synchronized with new transmission baudrate."
			<< unlock;

	if (!uCam.initJPEG(uCAM_II::JpegResolution::Image_640x480))
	{
		diagChannel << lock << MICROHAL_DEBUG << "Unable to initialize camera."
				<< unlock;
		return;
	}

	if (!uCam.setPackageSize(512))
	{
		diagChannel << lock << MICROHAL_DEBUG << "Unable to set package size" << unlock;
		return;
	}

	uint8_t data[1000000];
	size_t received;

	int i = 0;
	std::string fileName = "jpeg";
	std::string prefix = std::to_string(rand()%10000);
	while (1)
	{
		i++;
		std::string temp(fileName + std::to_string(i)+ prefix + ".jpg");
		// --------------------------------------- get JPEG picture --------------------------------------------------------
		if (!uCam.getPicture(uCAM_II::PictureType::JPEGPictureMode, data,
				received))
		{
			diagChannel << lock << MICROHAL_DEBUG << "Unable to read picture." << unlock;
		}
		else
		{
			saveFile(temp.c_str(), data, received);
		}
	}
}
int main(void) {
    debugPort.open(IODevice::ReadWrite);
    debugPort.write("\n\r------------------- uCAM-II driver example -------------------------\n\r");

    diagChannel.setOutputDevice(debugPort);
    // lets check if diagChannal is working
    diagChannel << lock << MICROHAL_EMERGENCY << "Information from diagnostic channel.\n" << unlock;

    if (!cameraPort.open(SerialPort::OpenMode::ReadWrite)) {  //! cameraPort.open(SerialPort::OpenMode::ReadWrite, 115200,
                                                              //! SerialPort::DataBits::Data8, SerialPort::StopBits::OneStop,
        // SerialPort::Parity::NoParity)) {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to open serial port." << unlock;
        return -1;
    }

    multipleCaptureTest();

    if (cameraPort.isOpen()) cameraPort.close();
    return 0;
}
