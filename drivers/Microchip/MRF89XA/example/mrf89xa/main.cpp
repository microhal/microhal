/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      main file for MRF89XA example

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
#include "microhal.h"
#include "microhal_bsp.h"

#include "mrf89xa.h"

using namespace microhal;
using namespace diagnostic;

// create RF module objects
MRF89XA rfMod1(rfModule1_SPI, rfModule1_csCon, rfModule1_csDat, rfModule1_IRQ1);
MRF89XA rfMod2(rfModule2_SPI, rfModule2_csCon, rfModule2_csDat, rfModule2_IRQ1);

bool RFmoduleInit(MRF89XA &rfMod, uint8_t moduleAddress) {
	// RF channel configuration
	if (rfMod.configureChannel(MRF89XA::Band_863_870MHz, MRF89XA::FSK, 300000, 1, 10) == false) {
		diagChannel << Debug << "Unable to configure channel." << endl;
	}
	rfMod.selectChannel(MRF89XA::Channel::A);

	//mode configuration (2)
	rfMod.setDataMode(MRF89XA::Packet);	// (2.b)
	rfMod.setIFgain(MRF89XA::IF_0dB);		// (2.c)

	// bit rate configuration (3)
	rfMod.setBitRate(2000);//max 40kbps in fsk mode
	// FIFO size configuration (5)
	rfMod.setFIFOsize(MRF89XA::FIFO_64B);
	rfMod.setFIFOinterruptThreshold(1);

	// configure PA ramp (6)
	rfMod.setPAramp(MRF89XA::Ramp_23us);

	//configure sync world (network address) (11.c, 12)
	rfMod.configureSyncWord(MRF89XA::SyncLen_32b, 0xAABBCCDD, MRF89XA::SyncErrorToleration_0b);
	rfMod.enableSyncWordRecognition();

	// configure TX power (13)
	rfMod.setOutputPower(MRF89XA::PA_1dBm);	// (13.b)

	// configure external CLK pin (14)
	rfMod.disableCLKout();

	// configure packet frame (15)
	rfMod.enableMancheser();			// (15.a)
	rfMod.configurePacketLength(MRF89XA::PktLen_variable, 64); // (15.b)
	rfMod.setAddress(moduleAddress);			// (15.c)
	rfMod.setPreambleSize(MRF89XA::Preamble_4B);			// (15.d)
	if (rfMod.enableCRC() == false) {					// (15.e)
		diagChannel << lock << Debug << "Unable to enable CRC." << endl << unlock;
		return false;
	}
	if (rfMod.setAddressFilteringPolicy(MRF89XA::NodeAdressAnd_0x00_0xFF) == false) { // (15.f)
		diagChannel << lock << Debug << "Unable to set address filtering policy." << endl << unlock;
		return false;
	}

	return true;
}


void interrupt1_IRQ0() {
	diagChannel << Debug << "Interrupt Mod1 IRQ0" << endl;
	rfMod1.IRQ0_func();
}

void interrupt2_IRQ0() {
	diagChannel << Debug << "Interrupt Mod2 IRQ0" << endl;
	rfMod2.IRQ0_func();
}

/**
 * @brief These function will be called when new packet will be received.
 * Function change RF module mode to Stand-by and read received packet from RF module. After reading packet content of packet is
 * printed on default diagChannel.
 */
void rfMod1_onNewPacket() {
	//read packet
	uint8_t datatmp[100];
	rfMod1.readPacket(datatmp, sizeof(datatmp));

	//print packet content
	diagChannel << Debug << "Module 1 payload: " << (char*)datatmp << endl;
}

/**
 * @brief These function will be called when new packet will be received.
 * Function change RF module mode to Stand-by and read received packet from RF module. After reading packet content of packet is
 * printed on default diagChannel.
 */
void rfMod2_onNewPacket() {
	rfMod2.setMode(MRF89XA::Stand_by);

	//read packet
	uint8_t datatmp[100];
	rfMod2.readPacket(datatmp, sizeof(datatmp));

	//print packet content
	diagChannel << Debug << "Module 2 payload: " << (char*)datatmp << endl;
}

int main(void) {
	// wait for power voltage stabilization
	std::this_thread::sleep_for(std::chrono::milliseconds {2000});

	// show example information
	debugPort.write("\n\r------------------- MRF89XA Demo -------------------------\n\r");
	// set default diagChannel output device.
	diagChannel.setOutputDevice(debugPort);

	// initialize external interrupt controller.
	ExternalInterrupt::init();

	// configure interrupts for RF modules
	ExternalInterrupt rfMod1_IRQ0(rfModule1_IRQ0);
	ExternalInterrupt rfMod2_IRQ0(rfModule2_IRQ0);

	//configure RF modules reset pins
	//GPIO rfMod1_reset(rfModule1_RESET, GPIO::Direction::Output);
	//GPIO rfMod2_reset(rfModule2_RESET, GPIO::Direction::Output);
	// disable reset
	//rfMod1_reset.reset();
	//rfMod2_reset.reset();

	//rfMod1_IRQ0.connect(MRF89XA::IRQ0_func, rfMod1, ExternalInterrupt::Trigger::OnFallingEdge);
	rfMod1_IRQ0.connect(interrupt1_IRQ0, ExternalInterrupt::Trigger::OnRisingEdge);
	rfMod2_IRQ0.connect(interrupt2_IRQ0, ExternalInterrupt::Trigger::OnRisingEdge);

	rfMod1_IRQ0.enable();
	rfMod2_IRQ0.enable();

	//rfMod1.registerDump();
	//rfMod2.registerDump();
	// ------------------------------------------- initialize RF modules -------------------------------------------
	RFmoduleInit(rfMod1, 0xAA);
	RFmoduleInit(rfMod2, 0xAB);

	//connect function to on packet receive signal. Connected function will be called after receiving packet.
	rfMod1.onPacketReceive.connect(rfMod1_onNewPacket);
	rfMod2.onPacketReceive.connect(rfMod2_onNewPacket);

	// make sure that FIFO is empty before enabling interrupt
	rfMod1.resetFIFO();
	rfMod2.resetFIFO();
	// configure and enable interrupt
	rfMod1.configIRQ0(MRF89XA::SyncOrAddrMatch, MRF89XA::IRQ0_TXsource::FIFOnotEmpty);
	rfMod1.configIRQ1(MRF89XA::CRC_OK, MRF89XA::TXDone);

	rfMod2.configIRQ0(MRF89XA::SyncOrAddrMatch, MRF89XA::IRQ0_TXsource::FIFOnotEmpty);
	rfMod2.configIRQ1(MRF89XA::CRC_OK, MRF89XA::TXDone);
	// show register values after configuration
	rfMod1.registerDump(diagChannel);
	rfMod2.registerDump(diagChannel);

	// ------------------------------------------- configure RF module 2 as receiver ------------------------------------------
	rfMod2.setMode(MRF89XA::Recieve);
	std::this_thread::sleep_for(std::chrono::milliseconds {500});

	// -------------------------------------- send packet from RF module 1 to RF module 2 --------------------------------------
	const uint8_t data[] = "Ala ma kota.";
	rfMod1.sendPacket(0xAB, data, sizeof(data), false);
	// change mode
	rfMod1.setMode(MRF89XA::Recieve);

	while(1) {
		 std::this_thread::sleep_for(std::chrono::seconds {10});

		 const uint8_t text[] = "Information from module 2.";

		 rfMod2.sendPacket(0xAA, text, sizeof(text), false);
	}

	return 0;
}
