/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 11-08-2015
 last modification: <DD-MM-YYYY>

 Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 in the documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *//* ========================================================================================================================== */

#include "mrf89xa.h"

using namespace microhal;
using namespace diagnostic;

/**
 * @brief These function send packet to another device. These function will block application until packet will be send or
 * 	timeout will be reached. These function after packet send will change RF module mode to Stand_by and this is default
 * 	end state.
 *
 * @param dstAddress destynation device address
 * @param payload - pointer to packet payload
 * @param payloadSize - size of packet payload
 * @param CCA clear channel assessment enable flag
 *
 * @return true if packet was send successfully or false if an error occurred.
 */
bool MRF89XA::sendPacket(uint8_t dstAddress, const uint8_t *payload, uint8_t payloadSize, bool CCA) {
	uint32_t rssiThreshold = 0;

	if (payloadSize == 0 || payload == nullptr) {
		log << lock << Debug << "Wrong parameters passed to sendPacket function, aborting packet sending." << endl << unlock;
	} else {
		log << lock << Debug << "Starting sending packet, destination address = " << dstAddress
				<< ", payload size = " << payloadSize << ", CCA enabled = " << CCA << endl << unlock;

		if (CCA) {
			uint32_t retry = 100;
			do {
				uint8_t rssi;
				if (getRSSI(rssi)){
					if(rssi <= rssiThreshold) {
						break;
					} else {
						std::this_thread::sleep_for(std::chrono::microseconds {10});
						if(retry-- == 0) {
							log << lock << Debug << "CCA failed, aborting packet sending." << endl << unlock;
							return false;
						}
					}
				}
			} while(retry);
		}

		if (setMode(Stand_by) == false) {
			log << lock << Debug << "Unable to set Stand by mode, aborting packet sending." << endl << unlock;
			return false;
		}

		if (resetFIFO() == false) {
			log << lock << Debug << "Unable to reset FIFO, aborting packet sending." << endl << unlock;
			return false;
		}

		if (setFIFOaccess(Write) == false) {
			log << lock << Debug << "Unable to set FIFO access, aborting packet sending." << endl << unlock;
			return false;
		}

		if (data.write(payloadSize + 1) == false) { // payload size + destination address field
			log << lock << Debug << "Unable to write packet size, aborting packet sending." << endl << unlock;
		} else {
			if (data.write(dstAddress) == false) {
				log << lock << Debug << "Unable to write destination address, aborting packet sending." << endl << unlock;
			} else {
				do {
					data.write(*payload++);
				} while(payloadSize--);
			}
		}

		if (setMode(Transmit) == false) {
			log << lock << Debug << "Unable to set Transmit mode, aborting packet sending." << endl << unlock;
			return false;
		}

		bool status = true;
		// Wait for packet send (TXDONE interrupt) or timeout
		if (semaphoreSend.wait(maxPacktetSendTime) == false) {
			log << lock << Debug << "Unable to set send packet, semaphore timeout." << endl << unlock;
			status = false;
		}

		if (setMode(Stand_by) == false) {
			log << lock << Debug << "Unable to set Stand-by mode" << endl << unlock;
			status = false;
		}

		return status;
	}
	return false;
}

/**
 * @brief These function can be used only in packed mode, to read FIFO in buffered mode try other function (actually
 * 	unimplemented). These function read packet stored in MRF89XA FIFO, before using these function make sure that packet is
 * 	already stored in FIFO. To detect incoming packet you can enable interrupt and set callbackOnPacketReceive. First byte
 * 	returned from these function in payload parameter contain destination address, payload starts from second position.
 *
 * 	Notice: when allocating memory for payload allocate one more byte for destination address.
 *
 * @param payload pointer to allocated memory where packet will be stored.
 * @param maxPayloadSize size of allocated memory passed in payload parameter. These is used to avoid buffer overflow.
 *
 * @return Function return size of received packet or 0 if an error occurred.
 */
uint8_t MRF89XA::readPacket(uint8_t *payload, uint8_t maxPayloadSize) {
	if (payload == nullptr || maxPayloadSize == 0) {
		log << lock << Debug << "Wrong parameters passed to readPacket function, aborting packet receiving." << endl << unlock;
	} else {
		if (setFIFOaccess(Read) == false) {
			log << lock << Debug << "Unable to set FIFO access, aborting packet receiving." << endl << unlock;
			return false;
		}
		uint8_t payloadSize = 0;

		if (data.read(payloadSize) == false) {
			log << lock << Debug << "Unable to read packet size, aborting packet receiving." << endl << unlock;
		} else {
			if(payloadSize <= maxPayloadSize) {
				for(uint8_t i = 0; i < payloadSize; i++){
					if (data.read(payload[i]) == false) {
						log << lock << Debug << "Unable to read packet data, aborting packet receiving." << endl << unlock;
						return 0;
					}
				};
				log << lock << Debug << "Payload size = " << payloadSize << "Payload = " << toHex(payload, payloadSize) << endl << unlock;
			} else {
				log << lock << Debug << "Packet payload is grater than reserved memory for it, aborting packet receiving." << endl << unlock;
			}
		}
	}
	return 0;
}

void MRF89XA::IRQ0_func() {
	uint8_t ftprireg;
	readRegister(FTPRIREG | READ_REGISTER_CMD, ftprireg);
	//in TX mode
	if(ftprireg & IRQ0TXST) {
		//fifo not empty interrupt
		log << Debug << "FIFO not empty interrupt." << endl;
	} else {
		// fifo threshold interrup
		log << Debug << "FIFO threshold interrupt." << endl;
	}

	//in RX mode
	switch (ftprireg & (IRQ0RXS0 | IRQ0RXS1)) {
		case 0:	//PLREADY
			break;
		case IRQ0RXS0: // WRITEBYTE
			log << Debug << "Write byte interrupt." << endl;
			break;
		case IRQ0RXS1: // FIFOEMPTY
			log << Debug << "FIFO empty interrupt." << endl;
			break;
		case IRQ0RXS0 | IRQ0RXS1: // sync/Address match interrupt
			log << Debug << "Sync/Address match interrupt." << endl;
			break;
	}
}

void MRF89XA::IRQ1_func() {
	const Mode mode = getMode();

	if(mode == Transmit) {
		uint8_t ftprireg;
		if(readRegister(FTPRIREG | READ_REGISTER_CMD, ftprireg) == false) {
			log << lock << Debug << "Unable to read FTPRIREG register" << unlock;
		} else {
			//in TX mode
			if(ftprireg & TXDONE) {
				// TXDONE
				log << Debug << "TX done interrupt." << endl;
				semaphoreSend.give();
			} else {
				// FIFOFULL
				log << Debug << "FIFO full interrupt, TX mode." << endl;
			}
		}
	} else if (mode == Recieve) {
		uint8_t ftxrxireg;
		if(readRegister(FTXRXIREG | READ_REGISTER_CMD, ftxrxireg) == false) {
			log << lock << Debug << "Unable to read FTPRIREG register" << unlock;
		} else {
			//in RX mode
			switch (ftxrxireg & (IRQ1RXS0 | IRQ1RXS1)) {
				case 0:	//CRCOK
					log << Debug << "CRC OK interrupt." << endl;
					uint8_t pktcreg;
					readRegister(PKTCREG | READ_REGISTER_CMD, pktcreg);

					if (pktcreg & CHKCRCEN){
						log << Debug << "CRC OK." << endl;
						onPacketReceive.emit();
					}
					break;
				case IRQ1RXS0: // FIFOFULL
					log << Debug << "FIFO full interrupt, RX mode." << endl;
					break;
				case IRQ1RXS1: // RSSI
					log << Debug << "RSSI interrupt." << endl;
					break;
				case IRQ1RXS0 | IRQ1RXS1: // FIFO_THRESHOLD
					log << Debug << "FIFO threshold interrupt." << endl;
					break;
			}
		}
	} else {
		log << MICROHAL_ERROR << "ERROR: unexpected mode." << endl;
	}
}
