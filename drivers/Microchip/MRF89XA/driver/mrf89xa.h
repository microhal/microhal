/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 11-08-2015
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
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
#ifndef _MRF89XA_H_
#define _MRF89XA_H_

#include "microhal.h"
#include "SPIDevice/SPIDevice.h"
#include "signalSlot/signalSlot.h"
#include "microhal_semaphore.h"

class MRF89XA : private microhal::SPIDevice {
	/**
	 * @brief This enum contain registers addresses
	 */
	enum Registers : uint8_t {
		// General registers
		GCONREG = 0x00 << 1,   //!< GCONREG -> General Configuration Register
		DMODREG = 0x01 << 1,   //!< DMODREG -> Data and Modulation Configuration Register
		FDEVREG = 0x02 << 1,   //!< FDEVREG -> Frequency Deviation Control Register
		BRSREG = 0x03 << 1,    //!< BRSREG -> Bit Rate Set Register
		FLTHREG = 0x04 << 1,   //!< FLTHREG -> Floor Threshold Control Register
		FIFOCREG = 0x05 << 1,  //!< FIFOCREG -> FIFO Configuration Register
		R1CREG = 0x06 << 1,    //!< R1CREG -> R1 Counter Set Register
		P1CREG = 0x07 << 1,    //!< P1CREG -> P1 Counter Set Register
		S1CREG = 0x08 << 1,    //!< S1CREG -> S1 Counter Set Register
		R2CREG = 0x09 << 1,    //!< R2CREG -> R2 Counter Set Register
		P2CREG = 0x0A << 1,    //!< P2CREG -> P2 Counter Set Register
		S2CREG = 0x0B << 1,    //!< S2CREG -> S2 Counter Set Register
		PACREG = 0x0C << 1,    //!< PACREG -> Power Amplifier Control Register
		FTXRXIREG = 0x0D << 1, //!< FTXRXIREG -> FIFO, Transmit and Receive Interrupt Request Configuration Register
		// Interrupt registers
		FTPRIREG = 0x0E << 1,  //!< FTPRIREG -> FIFO Transmit PLL and RSSI Interrupt Configuration Register
		RSTHIREG = 0x0F << 1,  //!< RSTHIREG -> RSSI Threshold Interrupt Request Configuration Register
		FILCREG = 0x10 << 1,   //!< FILCREG -> Filter Configuration Register
		// Receiver registers
		PFCREG = 0x11 << 1,    //!< PFCREG -> Polyphase Filter Configuration Register
		SYNCREG = 0x12 << 1,   //!< SYNCREG -> Sync Control Register
		RESVREG = 0x13 << 1,   //!< RESVREG -> Reserved Register (for future use)
		RSTSREG = 0x14 << 1,   //!< RSTSREG -> RSSI Status Read Register
		OOKCREG = 0x15 << 1,   //!< OOKCREG -> OOK Configuration Register
		// Sync word
		SYNCV31REG = 0x16 << 1,//!< SYNCV31REG -> Sync Value 1st Byte Configuration Register
		SYNCV23REG = 0x17 << 1,//!< SYNCV23REG -> Sync Value 2nd Byte Configuration Register
		SYNCV15REG = 0x18 << 1,//!< SYNCV15REG -> Sync Value 3rd Byte Configuration Register
		SYNCV07REG = 0x19 << 1,//!< SYNCV07REG -> Sync Value 4th Byte Configuration Register
		// Transmitter registers
		TXCONREG = 0x1A << 1,  //!< TXCONREG -> Transmit Configuration Register
		// CLK out registers
		CLKOUTREG = 0x1B << 1,   //!< CLKOREG -> Clock Output Control Register
		// Packet control registers
		PLOADREG = 0x1C << 1,  //!< PLOADREG -> Payload Configuration Register
		NADDSREG = 0x1D << 1,  //!< NADDSREG -> Node Address Set Register
		PKTCREG = 0x1E << 1,   //!< PKTCREG -> Packet Configuration Register
		FCRCREG = 0x1F << 1    //!< FCRCREG -> FIFO CRC Configuration Register
	};
	/**
	 * @brief These enum contain SPI registers commands.
	 */
	enum RegisterCommands : uint8_t {
		READ_REGISTER_CMD = 0x40/**< READ_REGISTER_CMD register address logically summed with these value will couses read form
									register.*/
	};
	/**
	 * @brief These enum contain registers bit fields.
	 */
	enum BitFields : uint8_t {
		//GCONREG
		CMOD2 = 0x80,          //!< CMOD2
		CMOD1 = 0x40,          //!< CMOD1
		CMOD0 = 0x20,          //!< CMOD0
		FBS1 = 0x10,           //!< FBS1
		FBS0 = 0x08,           //!< FBS0
		RPS = 0x01,            //!< RPS
		//DMODREG
		MODSEL1 = 0x80,        //!< MODSEL1
		MODSEL0 = 0x40,        //!< MODSEL0
		DMODE0 = 0x20,         //!< DMODE0
		DMODE1 = 0x04,         //!< DMODE1
		IFGAIN1 = 0x02,        //!< IFGAIN1
		IFGAIN0 = 0x01,        //!< IFGAIN0
		//FIFOCREG
		FSIZE1 = 0x80,         //!< FSIZE1
		FSIZE0 = 0x40,         //!< FSIZE0
		FTINT5 = 0x20,         //!< FTINT5
		FTINT4 = 0x10,         //!< FTINT4
		FTINT3 = 0x08,         //!< FTINT3
		FTINT2 = 0x04,         //!< FTINT2
		FTINT1 = 0x02,         //!< FTINT1
		FTINT0 = 0x01,         //!< FTINT0
		//PACREG
		PARC1 = 0x10,          //!< PARC1
		PARC0 = 0x08,          //!< PARC0
		//FTXRXIREG
		IRQ0RXS1 = 0x80,       //!< IRQ0RXS1
		IRQ0RXS0 = 0x40,       //!< IRQ0RXS0
		IRQ1RXS1 = 0x20,       //!< IRQ1RXS1
		IRQ1RXS0 = 0x10,       //!< IRQ1RXS0
		IRQ1TX = 0x08,         //!< IRQ1TX
		//FTPRIREG
		TXDONE = 0x20,         //!< TXDONE
		IRQ0TXST = 0x10,       //!< IRQ0TXST
		LSTSPLL = 0x02,        //!< LSTSPLL
		//SYNCREG
		SYNCREN = 0x20,        //!< SYNCREN
		SYNCWSZ1 = 0x10,       //!< SYNCWSZ1
		SYNCWSZ0 = 0x08,       //!< SYNCWSZ0
		SYNCTEN1 = 0x04,       //!< SYNCTEN1
		SYNCTEN0 = 0x02,       //!< SYNCTEN0
		//TXCONREG
		TXOPVAL2 = 0x80,       //!< TXOPVAL2
		TXOPVAL1 = 0x40,       //!< TXOPVAL1
		TXOPVAL0 = 0x20,       //!< TXOPVAL0
		//CLKOUTREG
		CLKOCNTRL = 0x80,      //!< CLKOCNTRL
		CLKOFREQ4 = 0x40,      //!< CLKOFREQ4
		CLKOFREQ3 = 0x20,      //!< CLKOFREQ3
		CLKOFREQ2 = 0x10,      //!< CLKOFREQ2
		CLKOFREQ1 = 0x08,      //!< CLKOFREQ1
		CLKOFREQ0 = 0x04,      //!< CLKOFREQ0
		//PLOADREG
		MCHSTREN = 0x80,       //!< MCHSTREN
		//PKTCREG
		PKTLENF = 0x80,        //!< PKTLENF
		PRESIZE0 = 0x20,       //!< PRESIZE0
		PRESIZE1 = 0x40,       //!< PRESIZE1
		WHITEON = 0x10,        //!< WHITEON
		ADDFIL0 = 0x02,        //!< ADDFIL0
		ADDFIL1 = 0x04,        //!< ADDFIL1
		CHKCRCEN = 0x08,       //!< CHKCRCEN
		STSCRCEN = 0x01,       //!< STSCRCEN
		//FCRCREG
		ACFCRC = (uint8_t)0x80,//!< ACFCRC
		FRWAXS = 0x40,         //!< FRWAXS
	};
	/**
	 * @brief These enum contain possible values of FIFO access. To change FIFO access call @ref setFIFOaccess function.
	 */
	enum FIFOaccess : uint8_t {
		Read = FRWAXS,//!< Read from FIFO
		Write = 0x00  //!< Write to FIFO
	};

	enum PLLconfig : uint8_t {
		PLL_1 = 0x00,
		PLL_2 = RPS
	};
public:
	/**
	 * @brief These enum contain possible transceiver modes. This mode can be set by @ref setMode function.
	 */
	enum Mode : uint8_t {
		Sleep = 0x00,				//!< Sleep
		Stand_by = 0x20,        	//!< Stand-by this mode is default
		FrequencySynthesizer = 0x40,//!< FrequencySynthesizer
		Recieve = 0x60,             //!< Receive
		Transmit = 0x80            	//!< Transmit
	};
	/**
	 * @brief These enum contain possible sub-GHz frequency bands. Band can be selected by @ref configureChannel function.
	 */
	enum FrequencyBandSelect : uint8_t {
		Band_902_915MHz = 0x00,     //!< Band_902_915MHz
		Band_915_928MHz = 0x08,     //!< Band_915_928MHz this mode is default
		Band_950_960MHz = 0x10,		//!< Band_950_960MHz same as 863MHz mode, application circuit dependent
		Band_863_870MHz = 0x11,		//!< Band_863_870MHz band configuration same as 950MHz application circuit dependent
	};
	/**
	 * @brief These enum contain possible modulation types. To set modulation use @ref setModulation function.
	 */
	enum Modulation : uint8_t {
		OOK = 0x40,//!< OOK
		FSK = 0x80 //!< FSK default
	};
	/**
	 * @brief These enum contain possible transceiver data modes. To set data mode use @ref setDataMode function.
	 */
	enum DataMode : uint8_t {
		Continuous = 0x00,//!< Continuous - FIFO unused, transceiver is transparent for communication. Data sending bit by bit.
		Buffer = 0x20,   //!< Buffer - FIFO used, transmission is going by FIFO
		Packet = 0x04    /**!< Packet - FIFO used, automatically CRC packet checking is possible, also module addressing
		 	 	 	 	 	 and network ID can be configured.*/
	};
	/**
	 * @brief These enum contain possible gain of IF chain. To set IF gain use @ref setIFgain function
	 */
	enum IFGain : uint8_t {
		IF_13_5dB,//!< IF_13_5dB -> -13.5 dB
		IF_9dB,   //!< IF_9dB -> -9 dB
		IF_4_5dB, //!< IF_4_5dB -> -4.5 dB
		IF_0dB    //!< IF_0dB -> 0 dB
	};
	/**
	 * @brief These enum contain possible FIFO sizes, to set FIFO size use @ref setFIFOsize function.
	 */
	enum FIFOsize : uint8_t {
		FIFO_16B = 0x00,//!< FIFO_16B -> 16 Byte
		FIFO_32B = 0x40,//!< FIFO_32B -> 32 Byte
		FIFO_48B = 0x80,//!< FIFO_48B -> 48 Byte
		FIFO_64B = 0xC0 //!< FIFO_64B -> 64 Byte
	};
	/**
	 * These enum contain possible rise and fall times of the TX PA regulator output voltage in OOK mode.
	 */
	enum PowerAmplifierRamp : uint8_t {
		Ramp_3us = 0x00,  //!< Ramp_3us -> 3us
		Ramp_15us = 0x10, //!< Ramp_15us -> 15us
		Ramp_8_5us = 0x08,//!< Ramp_8_5us -> 8.5us
		Ramp_23us = 0x18  //!< Ramp_23us -> 23us default
	};

	enum IRQ0_RXsource : uint8_t {
		PayloadReady = 0x00,
		WriteByte = IRQ0RXS0,
		FIFOnotEmpty = IRQ0RXS1,
		SyncOrAddrMatch = IRQ0RXS0 | IRQ0RXS1
	};

	enum IRQ1_RXsource : uint8_t {
		CRC_OK = 0x00,
		FIFOfull = IRQ1RXS0,
		RSSI = IRQ1RXS1,
		FIFOthreshold = IRQ1RXS0 | IRQ1RXS1
	};

	enum class IRQ0_TXsource : uint8_t {
		FIFOthreshold = 0x00,
		FIFOnotEmpty = IRQ0TXST,
	};

	enum IRQ1_TXsource : uint8_t {
		FIFOFull = 0x00,
		TXDone = IRQ1TX
	};
	/**
	 *
	 */
	enum class TXstartCondition : uint8_t {
		FIFOnotEmpty,//!< FIFOnotEmpty -> Transmit starts if FIFO is not empty, IRQ0 mapped to
		FIFOthreshold/**!< FIFOthreshold -> Start transmission when the number of bytes in FIFO is greater than or equal
					 	to threshold set by @ref setFIFOinterruptThreshold function.*/
	};
	/**
	 * @brief These enum contain possible output power of RF amplifier. To set Transmit output power call
	 * @ref setOutputPower function.
	 */
	enum PowerAmplifierOutputPower : uint8_t {
		PA_N_8dBm,//!< PA_N_8dBm
		PA_N_5dBm,//!< PA_N_5dBm
		PA_N_2dBm,//!< PA_N_2dBm
		PA_1dBm,  //!< PA_1dBm
		PA_4dBm,  //!< PA_4dBm
		PA_7dBm,  //!< PA_7dBm
		PA_10dBm, //!< PA_10dBm
		PA_13dBm, //!< PA_13dBm
	};
	/**
	 * @brief These enum contain possible prescaler for external clock output.
	 */
	enum OutCLKdivider : uint8_t {
		Div_1 = 0x00, //!< Div_1
		Div_2 = 0x04, //!< Div_2
		Div_4 = 0x08, //!< Div_4
		Div_6 = 0x0C, //!< Div_6
		Div_8 = 0x10, //!< Div_8
		Div_10 = 0x14,//!< Div_10
		Div_12 = 0x18,//!< Div_12
		Div_14 = 0x1C,//!< Div_14
		Div_16 = 0x20,//!< Div_16
		Div_18 = 0x24,//!< Div_18
		Div_20 = 0x28,//!< Div_20
		Div_22 = 0x2C,//!< Div_22
		Div_24 = 0x30,//!< Div_24
		Div_26 = 0x34,//!< Div_26
		Div_28 = 0x38,//!< Div_28
		Div_30 = 0x3C,//!< Div_30
		Div_32 = 0x40,//!< Div_32
		Div_34 = 0x44,//!< Div_34
		Div_36 = 0x48,//!< Div_36
		Div_38 = 0x4C,//!< Div_38
		Div_40 = 0x50,//!< Div_40
		Div_42 = 0x54,//!< Div_42
		Div_44 = 0x58,//!< Div_44
		Div_46 = 0x5C,//!< Div_46
		Div_48 = 0x60,//!< Div_48
		Div_50 = 0x64,//!< Div_50
		Div_52 = 0x68,//!< Div_52
		Div_54 = 0x6C,//!< Div_54
		Div_56 = 0x70,//!< Div_56
		Div_58 = 0x74,//!< Div_58
		Div_60 = 0x78,//!< Div_60
		Div_62 = 0x7C,//!< Div_62
	};
	/**
	 * @brief These enum specify possible packet length configurations. To change packet length format use @ref
	 * 	configurePacketLength function.
	 */
	enum PacketLenghtFormat :uint8_t {
		PktLen_fixed = 0x00,     //!< PktLen_fixed -> packet length is fixed
		PktLen_variable = PKTLENF//!< PktLen_variable -> packet length may varied depending on packet type (application dependent)
	};
	/**
	 * @brief These enum contain possible preamble sizes, to change preamble size use @ref setPreambleSize function.
	 */
	enum PreambleSize : uint8_t {
		Preamble_4B = PRESIZE0 | PRESIZE1,//!< Preamble_4B -> preamble is 4 bytes long.
		Preamble_3B = PRESIZE1,           //!< Preamble_3B -> preamble is 3 bytes long.
		Preamble_2B = PRESIZE0,           //!< Preamble_2B -> preamble is 2 bytes long.
		Preamble_1B = 0x00,               //!< Preamble_1B -> preamble is 1 bytes long.
	};
	/**
	 * @brief These enum contain possible Address fildering policy settings. To change address filtering policy call @ref
	 *  setAddressFilteringPolicy function.
	 */
	enum AddrFilterPolicy : uint8_t {
		OFF = 0x00,                                 //!< OFF -> address filtering is inactive
		NodeAddressOnly = ADDFIL0,                  //!< NodeAddressOnly -> only packets with node address will be stored in FIFO
		NodeAdressAnd_0x00 = ADDFIL1,               //!< NodeAdressAnd_0x00 -> packets with node address and 0x00 address will be stored in FIFO
		NodeAdressAnd_0x00_0xFF = ADDFIL0 | ADDFIL1,//!< NodeAdressAnd_0x00_0xFF -> packets with node address, 0x00 address and 0xFF address will be stored in FIFO
	};

	/**
	 * @brief These enum contain possible sync word lengths.
	 */
	enum SyncWordLen {
		SyncLen_8b = 0x00,                //!< SyncLen_8b -> 1 byte of sync word length
		SyncLen_16b = SYNCWSZ0,           //!< SyncLen_16b -> 2 bytes of sync word length
		SyncLen_24b = SYNCWSZ1,           //!< SyncLen_24b -> 3 bytes of sync word length
		SyncLen_32b = SYNCWSZ0 | SYNCWSZ1,//!< SyncLen_32b -> 4 bytes of sync word length
	};
	/**
	 * @brief These enum contain possible values of maximal number of errors in sync word detection.
	 */
	enum SyncWordErrorToleration : uint8_t {
		SyncErrorToleration_0b = 0x00,               //!< SyncErrorToleration_0b -> no error toleration
		SyncErrorToleration_1b = SYNCTEN0,           //!< SyncErrorToleration_1b -> maximally one bit can be mismatched
		SyncErrorToleration_2b = SYNCTEN1,           //!< SyncErrorToleration_2b -> maximally two bits can be mismatched
		SyncErrorToleration_3b = SYNCTEN0 | SYNCTEN1,//!< SyncErrorToleration_3b -> maximally three bits can be mismatched
	};
	/**
	 * @brief These enum contain name of possible channels configuration. To select active channel use @ref selectChannel function.
	 * 	To set channels configuration use @ref configureChannel function.
	 */
	enum class Channel : uint8_t {
		A,//!< A channel
		B //!< B channel
	};
public:
	const uint32_t fxtal = 12800; ///<! defines crystal frequency in kHz
	/**
	 * @brief Default constructor for MRF89XA driver.
	 *
	 * @param spi - reference to SPI driver interface.
	 * @param csCon - chip select pin for commands
	 * @param csDat - chip select pin for data
	 * @param IRQ1pin - interrupt 1 pin
	 * @param log - reference to diagnostic channel where driver logs will be printed
	 */
	MRF89XA(microhal::SPI &spi, microhal::GPIO::IOPin csCon, microhal::GPIO::IOPin csDat, microhal::GPIO::IOPin IRQ1pin,
				microhal::diagnostic::Diagnostic<microhal::diagnostic::LogLevel::Debug> &log = microhal::diagnostic::diagChannel)
		: microhal::SPIDevice(spi, csCon), data(spi, csDat), IRQ1interrupt(IRQ1pin), log(log) {
	}
	/**
	 * @brief These function configure RF channels. MRF89XA contain two sets of PLL registers that can be used to quick carrier frequency change.
	 *  First set of PLL register values is calculated by this function from channelAcarrier parameter, the second one is calculated form channalB
	 *  carrier. To select one of these carrier frequency use @ref selectChannel function.
	 *
	 * @param band specify frequency band, these parameter is hardware dependent.
	 * @param mod parameter specify modulation used by module
	 *
	 * @param channelSpacing
	 * @param channelANum
	 * @param channelBNum
	 * @return
	 */
	bool configureChannel(FrequencyBandSelect band, Modulation mod, uint32_t channelSpacing, uint8_t channelANum, uint8_t channelBNum) {
		uint32_t startFreq; // in [kHz]
		switch(band) {
		case Band_902_915MHz:
			startFreq = 902000;
			break;
		case Band_915_928MHz:
			startFreq = 915000;
			break;
		case Band_950_960MHz:
			startFreq = 950000;
			break;
		case Band_863_870MHz:
			startFreq = 863000;
			break;
		}
		// A channel configuration
		const uint32_t channelAFreq = startFreq + (uint32_t)channelANum * channelSpacing;
		const uint32_t channelACompare = (channelAFreq * 8 * 101) / (9 * fxtal);

		const uint8_t pA = ((channelACompare - 75) / 76) + 1;
		const uint8_t sA = channelACompare - (75 * (pA + 1));
		// B channel configuration
		const uint32_t channelBFreq = startFreq + channelBNum * channelSpacing;
		const uint32_t channelBCompare = (channelBFreq * 8 * 101) / (9 * fxtal);

		const uint8_t pB = ((channelBCompare - 75) / 76) + 1;
		const uint8_t sB = channelBCompare - (75 * (pB + 1));

		log << microhal::diagnostic::lock << microhal::diagnostic::Debug << "Setting A channel PLL, R = 125, P = " << pA
			<< ", S = "	<< sA << microhal::diagnostic::endl	<< "B channel PLL, R = 100, P = " << pB << ", S = " << sB
			<< microhal::diagnostic::endl << microhal::diagnostic::unlock;

		//set channal registers
		if (setBand(band) == false) return false;
		if (setModulation(mod) == false) return false;

		if (setPLL1_config(125, 100, 20) == false) return false;
		//if (setPLL1_config(89, 72, 25) == false) return false;

		//if (setPLL1_config(100, pA, sA) == false) return false;
		if (setPLL2_config(100, pB, sB) == false) return false;

		return true;
	}
	/**
	 * @brief These function select set of PLL registers to generate carrier frequency. Register values are calculated in
	 * 		@ref configureChannel function.
	 *
	 * @param channel
	 *
	 * @return true if carrier frequency was changed or false if an error occurred.
	 */
	bool selectChannel(Channel channel) {
		switch(channel) {
			case Channel::A: return selectPLLconfig(PLL_1);
			case Channel::B: return selectPLLconfig(PLL_2);
		}
		return false;
	}
	/**
	 * @brief These function configure sync word recognition but don't enabled it. To enable sync word recognition call @ref
	 * 	enableSyncWordRecognition function.
	 *
	 * @param length of sync word, to see possible parameters check @ref SyncWordLen enum.
	 * @param syncWord (Network ID) code.
	 * @param error these parameter specify maximum number of bits that can be mismatched when receiving sync word. Possible
	 * 			values of these parameter are stored in @ref SyncWordErrorToleration enum.
	 *
	 * @return true if sync word was configured successfully or false if an error occurred.
	 */
	bool configureSyncWord(SyncWordLen length, uint32_t syncWord, SyncWordErrorToleration error) {
		if (setSyncWordLen(length) == false) return false;
		if (setSyncWord(syncWord) == false) return false;
		if (setSyncWordErrorToleration(error) == false) return false;

		return true;
	}
	/**
	 * @brief These function enable sync world recognition. Before calling these function configure sync word using @ref configureSyncWord
	 * 	function.
	 *
	 * @return true if sync word recognition was enabled or false if an error occurred.
	 */
	bool enableSyncWordRecognition() {
		return setBitsInRegister(SYNCREG, SYNCREN);
	}
	/**
	 * @brief These function disable sync word recognition.
	 *
	 * @return true if sync word recognition was disabled or false if an error occurred.
	 */
	bool disableSyncWordRecognition() {
		return clearBitsInRegister(SYNCREG, SYNCREN);
	}
	/**
	 * @brief These function configure the transmitter start condition. Function also enable interrupts.
	 *
	 * @param start
	 *
	 * @return
	 */
	bool configureTransmitterStartCondition(TXstartCondition start) {
		return writeRegisterWithMask(FTPRIREG, static_cast<uint8_t>(start), IRQ0TXST);
	}

	bool sendPacket(uint8_t dstAddress, const uint8_t *payload, uint8_t payloadSize, bool CCA);
	uint8_t readPacket(uint8_t *payload, uint8_t maxPayloadSize);
	/**
	 * @brief These function set operating mode of the transceiver.
	 *
	 * @param mode to set.
	 * @return true if mode was set successfully or false if an error occurred.
	 */
	bool setMode(Mode mode) {
		return writeRegisterWithMask(GCONREG, mode, CMOD0 | CMOD1 | CMOD2);
	}
	/**
	 * @brief These function return transceiver operating mode.
	 *
	 * @param error
	 * @return
	 */
	Mode getMode(bool *error = nullptr) {
		uint8_t gconreg;
		if(readRegister(GCONREG | READ_REGISTER_CMD, gconreg) == false) {
			if(error) *error = true;
		}

		return static_cast<Mode>(gconreg & (CMOD0 | CMOD1 | CMOD2));
	}
	/**
	 * @brief These function set modulation type to be used in Sub-GHz range.
	 *
	 * @param mod modulation to set
	 * @return true if modulation was set successfully or false if an error occurred.
	 */
	bool setModulation(Modulation mod) {
		return writeRegisterWithMask(DMODREG, mod, MODSEL0 | MODSEL1);
	}
	/**
	 * @brief These function set data operation mode.
	 *
	 * @param mode to set
	 * @return true if new mode was set successfully or false if an error occurred.
	 */
	bool setDataMode(DataMode mode) {
		return writeRegisterWithMask(DMODREG, mode, DMODE0 | DMODE1);
	}
	/**
	 * @brief These function set gain of IF chain.
	 *
	 * @param gain to set.
	 * @return true if gain was set or false if an error occurred.
	 */
	bool setIFgain(IFGain gain) {
		return writeRegisterWithMask(DMODREG, gain, IFGAIN0 | IFGAIN1);
	}

	/**
	 * @brief These function try set bit rate if passed parameter is to high then this function set highest possible bit rate.
	 * If bit rate passed as parameter isn't possible to set then this function set closest possible value.
	 *
	 * @param bitRate to set in bits per second (bps)
	 * @return 0 if an error occurred or actual bit rate
	 */
	uint32_t setBitRate(uint32_t bitRate) {
		uint8_t brval = (fxtal / (64 * bitRate)) - 1;
		// check if brval isn't to high
		if(brval > 127) brval = 127;

		if(writeRegister(BRSREG, brval)) {
			return fxtal / (64 * (brval + 1));
		} else {
			return 0;
		}
	}
	/**
	 * @brief These function return actually set bit rate in bits per second (bps).
	 * @return actually set bit rate in bps or 0 if an error occurred
	 */
	uint32_t getBitRate() {
		uint8_t brval;
		if(readRegister(BRSREG | READ_REGISTER_CMD, brval)){
			return fxtal / (64 * (brval + 1));
		} else {
			// unable to read register
			return 0;
		}
	}
	/**
	 * @brief These function set Floor threshold in OOK receive mode. Input parameter assumes 0.5 dB RSSI step.
	 *
	 * @param thr threshold to set with 0.5 dB step. Passing 12 as parameter will set 6 dB threshold.
	 * @return
	 */
	bool setOOKflorTreshold(uint8_t thr) {
		return writeRegister(FLTHREG, thr);
	}
	/**
	 * @brief These function set size of FIFO.
	 *
	 * @param size of FIFO to set
	 * @return true if new size was set successfully or false if an error occurred.
	 */
	bool setFIFOsize(FIFOsize size) {
		return writeRegisterWithMask(FIFOCREG, size, FSIZE0 | FSIZE1);
	}
	/**
	 * @brief These function set FIFO interrupt threshold.
	 *
	 * @param threshold
	 * @return
	 */
	bool setFIFOinterruptThreshold(uint8_t threshold) {
		return writeRegisterWithMask(FIFOCREG, threshold, FTINT0 | FTINT1 | FTINT2 | FTINT3 | FTINT4 | FTINT5);
	}
	/**
	 * @brief These function set the Power Amplifier ramp rise and fall times in OOK mode.
	 * @param ramp
	 * @retval true if power amplifier ramp was set successfully
	 * @retval false if an error occurred
	 */
	bool setPAramp(PowerAmplifierRamp ramp) {
		return writeRegisterWithMask(PACREG, ramp, PARC0 | PARC1);
	}
// Interrupt configuration
	bool configIRQ0(IRQ0_RXsource rxSource, IRQ0_TXsource txSource) {
		if (writeRegisterWithMask(FTXRXIREG, rxSource, IRQ0RXS0 | IRQ0RXS1)) {
			return writeRegisterWithMask(FTPRIREG, static_cast<uint8_t>(txSource), IRQ0TXST);
		}

		return false;
	}

	bool configIRQ1(IRQ1_RXsource rxSource, IRQ1_TXsource txSource) {
		const bool status = writeRegisterWithMask(FTXRXIREG, rxSource | txSource, IRQ1RXS0 | IRQ1RXS1 | IRQ1TX);
		IRQ1interrupt.connect(irq1_slot, *this, microhal::ExternalInterrupt::Trigger::OnRisingEdge); // fixme these function should be called only once
		IRQ1interrupt.enable();
		return status;
	}

	bool setRSSIthreshold(uint8_t thr) {
		return writeRegister(RSTHIREG, thr);
	}
// Receiver configuration
	bool getRSSI(uint8_t &rssi) {
		return readRegister(RSTSREG  | READ_REGISTER_CMD, rssi);
	}

// Transmitter configuration
	bool setTransmisisionFilterCutOffFreq() {
		//todo implement
		return false;
	}
	/**
	 * @brief These function set RF Power Amplifier Output power.
	 *
	 * @param power to set
	 * @return true if new setting was set or false if an error occurred.
	 */
	bool setOutputPower(PowerAmplifierOutputPower power) {
		return writeRegisterWithMask(TXCONREG, power, TXOPVAL0 | TXOPVAL1 | TXOPVAL2);
	}
// CLK out control
	/**
	 * @brief These function enable output clock pin
	 *
	 * @return true if pin was successfully enabled or false if an error occurred
	 */
	bool enableCLKout() {
		return setBitsInRegister(CLKOUTREG, CLKOCNTRL);
	}
	/**
	 * @brief These function disable output clock pin
	 *
	 * @return true if pin was successfully disabled or false if an error occurred
	 */
	bool disableCLKout() {
		return clearBitsInRegister(CLKOUTREG, CLKOCNTRL);
	}
	/**
	 * @brief These function set output clock divider. Output frequency will be crystal oscillator frequency divided by set divider.
	 *
	 * @param div divider to set
	 * @return true if divider was set or false if an error occurred.
	 */
	bool setOutCLKdivider(OutCLKdivider div) {
		return writeRegisterWithMask(CLKOUTREG, div, CLKOFREQ0 | CLKOFREQ1 | CLKOFREQ2 | CLKOFREQ3 | CLKOFREQ4);
	}
// Packet configuration
	/**
	 * @brief These function enable Manchester Encoding/Decoding
	 *
	 * @return true if Manchester Encoding/Decoding was enabled or false if an error occurred
	 */
	bool enableMancheser() {
		return setBitsInRegister(PLOADREG, MCHSTREN);
	}
	/**
	 * @brief These function disable Manchester Encoding/Decoding
	 *
	 * @return true if Manchester Encoding/Decoding was disabled or false if an error occurred
	 */
	bool disableManchester() {
		return clearBitsInRegister(PLOADREG, MCHSTREN);
	}
	/**
	 * @brief These function can be used only in packet data mode. These function configure packet length format and packet length.
	 * 	If packet length format is fixed them length parameter is packet length. If packet length format is set as variable then
	 * 	in length parameter you should specify maximum packet length. Also length of packet can not be grater than FIFO size.
	 * 	FIFO size can be configured by @ref setFIFOsize function.
	 *
	 * @param format packet length format to set see @ref PacketLenghtFormat enum.
	 * @param length depending on format parameter. If format is fixed then this parameter contain payload size. If format is set
	 * 			as variable then this parameter specify maximal payload size.
	 *
	 * @return true if packet configuration succeed or false if an error occurred.
	 */
	bool configurePacketLength(PacketLenghtFormat format, uint8_t length) {
		if (setPacketLenghtFormat(format)){
			return setPayloadLength(length);
		}
		return false;
	}
	/**
	 * @brief These function set 8 bit local node address. Using these function make sense only in packet data mode.
	 *
	 * @param addr to set
	 * @return true if address was set successfully or false if an error occurred
	 */
	bool setAddress(uint8_t addr) {
		return writeRegister(NADDSREG, addr);
	}
	/**
	 * @brief These function read 8 bit local node address. Using these function make sense only in packet data mode.
	 *
	 * @param addr reference to memory where readed address will be stored.
	 * @return true if address was read successfully or false if an error occurred
	 */
	bool getAddress(uint8_t &addr){
		return readRegister(NADDSREG  | READ_REGISTER_CMD, addr);
	}
	/**
	 * @brief These function configure preamble size, all possible values are defined in @ref PreambleSize enumerator.
	 * @param size to set.
	 * @return true if preamble size was set or false if an error occurred.
	 */
	bool setPreambleSize(PreambleSize size) {
		return writeRegisterWithMask(PKTCREG, size, PRESIZE0 | PRESIZE1);
	}
	/**
	 * @brief These function enable data whitening.
	 * @return true if data whitening was enabled successfully or false if an error occurred.
	 */
	bool enableWhitening() {
		return setBitsInRegister(PKTCREG, WHITEON);
	}
	/**
	 * @brief These function disable data whitening.
	 * @return true if data whitening was disabled successfully or false if an error occurred.
	 */
	bool disableWhitening() {
		return clearBitsInRegister(PKTCREG, (uint8_t)WHITEON);
	}
	/**
	 * @brief These function enable CRC calculation and check.
	 *
	 * @return true if CRC was enabled successfully or false if an error occurred.
	 */
	bool enableCRC() {
		return setBitsInRegister(PKTCREG, CHKCRCEN);
	}
	/**
	 * @brief These function disable CRC calculation and check.
	 *
	 * @return true if CRC was disabled successfully or false if an error occurred.
	 */
	bool disableCRC() {
		return setBitsInRegister(PKTCREG, CHKCRCEN);
	}
	/**
	 * @brief these function return status of CRC bit. Function have meaning only in packet data mode.
	 * @return true CRC OK, false error when checking CRC sum.
	 */
	bool checkCRCstatus() {
		uint8_t regVal;
		if (readRegister(PKTCREG, regVal)) {
			return regVal &	STSCRCEN;
		}
		return false;
	}
	/**
	 * @brief These function set address filtering policy.
	 *
	 * @param policy of address filtering
	 * @return true if address filtering policy was set or false if an error occurred.
	 */
	bool setAddressFilteringPolicy(AddrFilterPolicy policy) {
		return writeRegisterWithMask(PKTCREG, policy, ADDFIL0 | ADDFIL1);
	}
	/**
	 * @brief These function checks the status/result of the CRC of the current packet.
	 *
	 * @return true if CRC was OK or false if an error occurred or CRC was not OK.
	 */
	bool getCRCstatus() {
		uint8_t pktcreg;
		//fixme check status
		if (readRegister(PKTCREG  | READ_REGISTER_CMD, pktcreg)) {
			return pktcreg & STSCRCEN;
		} else {
			return false;
		}
	}
	/**
	 * @brief These function enable auto FIFO clear if CRC of the current packet failed.
	 *
	 * @return true if auto FIFO clear was enabled or false if an error occurred.
	 */
	bool enableFIFOclearOnCRCerror() {
		return clearBitsInRegister(FCRCREG, ACFCRC);
	}
	/**
	 * @brief These function disable auto FIFO clear if CRC of the current packet failed.
	 *
	 * @return true if auto FIFO clear was disabled or false if an error occurred.
	 */
	bool disableFIFOclearOnCRCerror() {
		return setBitsInRegister(FCRCREG, ACFCRC);
	}
	/**
	 * @brief These function set FIFO access in stand-by mode.
	 *
	 * @param access to set
	 * @return true if FIFO access was set successfully or false if an error occurred.
	 */
	bool setFIFOaccess(FIFOaccess access) {
		return writeRegisterWithMask(FCRCREG, access, FRWAXS);
	}

	bool checkPLLlockFlag(bool &flag) {
		uint8_t value;
		bool status = readRegister(FTPRIREG | READ_REGISTER_CMD, value);

		flag = (value & LSTSPLL);// == LSTSPLL;

		return status;
	}

	bool clearPLLlockFlag() {
		return setBitsInRegister(FTPRIREG, LSTSPLL);
	}

	void IRQ0_func();
	/**
	 * @brief These function show all registers values on diagnosti channel passed as parameter, in case of lack of parameter
	 * 	all values will be shown on default diagnostic channel.
	 *
	 * @param log reference to diagnostic channel, parameter is optional.
	 */
	template<microhal::diagnostic::LogLevel level>
	void registerDump(microhal::diagnostic::Diagnostic<level> &log) {
		log << microhal::diagnostic::lock << microhal::diagnostic::Debug << microhal::diagnostic::endl
				<< "-------------- MRF89XA registers --------------" << microhal::diagnostic::endl;

		uint8_t regValue;
		for (uint8_t regAddr = 0; regAddr <= FCRCREG; regAddr += 2) {
			if (readRegister(regAddr | READ_REGISTER_CMD, regValue)) {
				log << microhal::diagnostic::Debug << "Register address = " << microhal::diagnostic::toHex(regAddr >> 1) << ", value = "
						<< microhal::diagnostic::toHex(regValue) << microhal::diagnostic::endl;
			} else {
				log << microhal::diagnostic::Debug << "Unable to read register with address = " << microhal::diagnostic::toHex(regAddr >> 1)
					<< microhal::diagnostic::endl;
			}
		}

		log << microhal::diagnostic::unlock;
	}
	/**
	 * @brief These function clear content of FIFO. Use these function only in packet or bufferred data mode.
	 * @return true if FIFO was reset successfully of false if an error occurred.
	 */
	bool resetFIFO() {
		return setBitsInRegister(FTXRXIREG, 0x01);
	}
	/**
	 * @brief Signal that will be emitted when new packet will be received. Function connected by these signal will be called
	 * 	from interrupt so should be as short as possible. For more information about Signals and Slots see mircoHAL
	 * 	documentation.
	 */
	microhal::Signal<void> onPacketReceive;
private:
	/**
	 * @brief function that is used to process interrupts from IRQ1 line.
	 */
	void IRQ1_func(); // these must be declared before slot

	microhal::SPIDevice data;
	microhal::ExternalInterrupt IRQ1interrupt;
	microhal::diagnostic::Diagnostic<microhal::diagnostic::LogLevel::Debug> &log;
	microhal::os::Semaphore semaphoreSend;
	std::chrono::milliseconds maxPacktetSendTime {10000}; // fixme add calculation of max transmission time to setBaudrate function
	microhal::Slot_0<MRF89XA, &MRF89XA::IRQ1_func> irq1_slot;

	/**
	 * @brief These function set the frequency band to be used in sub-GHz range.
	 *
	 * @param band to set.
	 * @return true if band was successfully set or false if an error occurred.
	 */
	bool setBand(FrequencyBandSelect band) {
		return writeRegisterWithMask(GCONREG, band, FBS0 | FBS1);
	}

	FrequencyBandSelect getBand(bool *error = nullptr) {
		uint8_t gconreg;
		if(readRegister(GCONREG | READ_REGISTER_CMD, gconreg) == false) {
			if(error) *error = true;
		}

		return static_cast<FrequencyBandSelect>(gconreg & (FBS0 | FBS1));
	}

	bool selectPLLconfig(PLLconfig config) {
		return writeRegisterWithMask(GCONREG, config, RPS);
	}
	/**
	 * @brief These function set Frequency deviation. If frequency deviation passed as parameter is unable to set then
	 * function sets closest possible value.
	 *
	 * @param fdev frequency deviation in Hz
	 * @return actually set frequency deviation or 0 if an error occurred
	 */
	uint32_t setFrequencyDeviation(uint32_t fdev) {
		uint8_t fdval = (fxtal / (32 * fdev)) - 1;

		if(writeRegister(FDEVREG, fdval)){
			return fxtal / (32 * (fdval + 1));
		} else {
			return 0;
		}
	}
	/**
	 * @brief These function read actually set frequency deviation.
	 *
	 * @return Frequency deviation in Hz or 0 if an error occurred.
	 */
	uint32_t getFrequencyDeviation() {
		uint8_t fdval;
		if(readRegister(FDEVREG | READ_REGISTER_CMD, fdval)) {
			return fxtal / (32 * (fdval + 1));
		} else {
			return 0;
		}
	}

	bool setPLL1_config(uint8_t r, uint8_t p, uint8_t s) {
		if (writeRegister(R1CREG, r) == false) return false;
		if (writeRegister(P1CREG, p) == false) return false;
		if (writeRegister(S1CREG, s) == false) return false;

		return true;
	}

	bool setPLL2_config(uint8_t r, uint8_t p, uint8_t s) {
		if (writeRegister(R2CREG, r) == false) return false;
		if (writeRegister(P2CREG, p) == false) return false;
		if (writeRegister(S2CREG, s) == false) return false;

		return true;
	}
	// Sync word configuration
	bool setSyncWordLen(SyncWordLen len) {
		return writeRegisterWithMask(SYNCREG, len, SYNCWSZ0 | SYNCWSZ1);
	}

	bool setSyncWordErrorToleration(SyncWordErrorToleration error) {
		return writeRegisterWithMask(SYNCREG, error, SYNCTEN0 | SYNCTEN1);
	}
	/**
	 * @brief These function set sync world. If using less than 32 bit sync world set bits from MSB
	 *
	 * @param sync world to set
	 * @return true if sync world was set successfully or false if an error occurred.
	 */
	bool setSyncWord(uint32_t sync) {
		union SyncWord {
			const uint32_t syncWord;
			const uint8_t byte[4];
		};

		if(microhal::DeviceEndianness == microhal::Endianness::LittleEndian) sync = microhal::byteswap(sync);

		SyncWord syncWord = {sync};

		// write 4 sync registers
		if (writeRegister(SYNCV31REG, syncWord.byte[0]) == false) return false;
		if (writeRegister(SYNCV23REG, syncWord.byte[1]) == false) return false;
		if (writeRegister(SYNCV15REG, syncWord.byte[2]) == false) return false;
		if (writeRegister(SYNCV07REG, syncWord.byte[3]) == false) return false;

		return true;
	}
	/**
	 * @brief These function set packet length format, possible formats are stored in @ref PacketLenghtFormat enum.
	 * @param format to set.
	 * @return true if packet length format was set successfully of false if an error occurred.
	 */
	bool setPacketLenghtFormat(PacketLenghtFormat format) {
		return writeRegisterWithMask(PKTCREG, format, PKTLENF);
	}
	/**
	 * @brief These function set RX payload length in fixed Packet length or maximum RX payload size when packet payload length is variable.
	 * Maximum payload length is 127 bytes.
	 *
	 * @param len payload length to set in bytes.
	 * @return true if payload was set or false if an error occurred.
	 */
	bool setPayloadLength(uint8_t len) {
		if(len < 128) {
			return writeRegisterWithMask(PLOADREG, len, 0x7F);
		} else {
			return false;
		}
	}
	/**@brief This function write bits from data parameter to 8bit register but only bits set to one in mask parameter are changed.
	*
	* @param registerAddress address of register where bit will be cleared
	* @param data to write
	* @param bitMask if set then corresponding bit in register will be written.
	*
	* @retval true if bits was set
	* @retval false if an error occurred
	*/
	bool writeRegisterWithMask(uint8_t registerAddress, uint8_t data, uint8_t mask){
		bool status = false;
		SPIlock();

		uint8_t readData;
		if(readRegister_noLock(registerAddress | READ_REGISTER_CMD, readData)){
			//clear bits
			readData &= ~mask;
			//set new value
			readData |= (data & mask);
			status = writeRegister_noLock(registerAddress, readData);
		}

		SPIunlock();
		return status;
	}
	/**
	 * @brief This function modify 8 bit register. Function set bitMask corresponding bits in register.
	 * Function is thread safe.
	 *
	 * @param registerAddress - address of register where bits will be set
	 * @param bitMask - bits to set
	 *
	 * @retval true if data was sent.
	 * @retval false if an error occurred.
	 */
	bool setBitsInRegister(uint8_t registerAddress, uint8_t bitMask) {
		uint8_t registerValue;
		bool status = false;

		SPIlock();
		if (readRegister_noLock(registerAddress | READ_REGISTER_CMD, registerValue) == true) {
			registerValue |= bitMask;
			status = writeRegister_noLock(registerAddress, registerValue);
		}
		SPIunlock();
		return status;
	}
	/**
	 * @brief This function modify 8 bit register. Function clear bitMask corresponding bits in register.
	 * Function is thread safe.
	 *
	 * @param registerAddress - address of register where bits will be clear
	 * @param bitMask - bits to clear
	 *
	 * @retval true if data was sent.
	 * @retval false if an error occurred.
	 */
	bool clearBitsInRegister(uint8_t registerAddress, uint8_t bitMask) {
	    uint8_t registerValue;
	    bool status = false;

	    SPIlock();
	    if (readRegister_noLock(registerAddress | READ_REGISTER_CMD, registerValue) == true) {
	        registerValue &= ~bitMask;
	        status = writeRegister_noLock(registerAddress, registerValue);
	    }
	    SPIunlock();
	    return status;
	}
};


#endif // _MRF89XA_H_
