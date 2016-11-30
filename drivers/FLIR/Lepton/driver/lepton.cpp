/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      FLIR Lepton IR camera driver file
 *
 * @authors    Pawel Okas
 * created on: 7-07-2016
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

#include "lepton.h"

//constexpr microhal::CRC16 Lepton::VoSPIPacket::crcCalculator;


void Lepton::timeProc() {
	bool packetReceptionInProgress = false;
	do {
		if (readImagePacket()) {
			if (imagePacket.isDiscard() == false && pictureReady == false) {
				if (imagePacket.verifyCRC()) {
					packetReceptionInProgress = true;
//					microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Debug <</* "packet nr: " << */imagePacket.getNumber() << microhal::diagnostic::endl << microhal::diagnostic::unlock;
					if (imagePacket.getNumber() <= 59) {
	 					memcpy(&pictureBuffer[imagePacket.getNumber() * imagePacket.size()], imagePacket.getImageDataPtr(), imagePacket.size());
						if (imagePacket.getNumber() == 59) {
							pictureReady = true;
							break;
						}
					} else {
//						microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Error
//									<< "Received packet with number: " << imagePacket.getNumber() << ", but maximal allowed value is 59."
//									<< microhal::diagnostic::endl << microhal::diagnostic::unlock;
					}
				}
			}
		}
	} while(packetReceptionInProgress);
}
