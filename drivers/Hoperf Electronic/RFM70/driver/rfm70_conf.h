/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 4-08-2012
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

#ifndef RFM70_CONF_H_
#define RFM70_CONF_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: CONFIG				adr: 0x00		disc: Configuration Register
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_MASK_RX_DR 0				//Mask interrupt caused by RX_DR
									//1: Interrupt not reflected on the IRQ pin
									//0: Reflect RX_DR as active low interrupt on the IRQ pin
//***********************************************************************************************************//									
#define B0_MASK_TX_DS 0				//Mask interrupt caused by TX_DS
									//1: Interrupt not reflected on the IRQ pin
									//0: Reflect TX_DS as active low interrupt on the IRQ pin
//***********************************************************************************************************//
#define B0_MASK_MAX_RT 1				//Mask interrupt caused by MAX_RT
									//1: Interrupt not reflected on the IRQ pin
									//0: Reflect MAX_RT as active low interrupt on the IRQ pin
//***********************************************************************************************************//									
#define B0_EN_CRC 1					//Enable CRC. Forced high if one of the bits in the EN_AA is high
//***********************************************************************************************************//									
#define B0_CRCO 1						//CRC encoding scheme
									//'0' - 1 byte
									//'1' - 2 bytes
//***********************************************************************************************************//
#define B0_PWR_UP 1					//1: POWER UP, 0:POWER DOWN
//***********************************************************************************************************//
#define B0_PRIM_RX 1				//RX/TX control,
									//1: PRX, 0: PTX
									
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: EN_AA				adr: 0x01		disc: Enable „Auto Acknowledgment‟ Function
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_ENAA_P5	1					//Enable auto acknowledgement data pipe 5
//***********************************************************************************************************//
#define B0_ENAA_P4	1					//Enable auto acknowledgement data pipe 4
//***********************************************************************************************************//
#define B0_ENAA_P3	1					//Enable auto acknowledgement data pipe 3
//***********************************************************************************************************//
#define B0_ENAA_P2	1					//Enable auto acknowledgement data pipe 2
//***********************************************************************************************************//
#define B0_ENAA_P1 1					//Enable auto acknowledgement data pipe 1
//***********************************************************************************************************//
#define B0_ENAA_P0 1					//Enable auto acknowledgement data pipe 0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: EN_RXADDR			adr: 0x02		disc: Enabled RX Addresses
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_ERX_P5 1					//Enable data pipe 5
//***********************************************************************************************************//
#define B0_ERX_P4 1					//Enable data pipe 4
//***********************************************************************************************************//
#define B0_ERX_P3 1					//Enable data pipe 3
//***********************************************************************************************************//
#define B0_ERX_P2 1					//Enable data pipe 2
//***********************************************************************************************************//
#define B0_ERX_P1 1					//Enable data pipe 1
//***********************************************************************************************************//
#define B0_ERX_P0 1					//Enable data pipe 0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: SETUP_AW				adr: 0x03		disc: Setup of Address Widths
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_AW 3						//RX/TX Address field width
									// 0 - Illegal
									// 1 - 3 bytes
									// 2 - 4 bytes
									// 3 - 5 bytes 
									//LSB bytes are used if address width is below 5 bytes

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: SETUP_RETR			adr: 0x04		disc: Setup of Automatic Retransmission
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_ARD 1						//Auto Retransmission Delay
									// 0 – Wait 250 us
									// 1 – Wait 500 us
									// 2 – Wait 750 us
									// ..
									// 15 – Wait 4000 us
//***********************************************************************************************************//
#define B0_ARC 15						//Auto Retransmission Count
									// 0 –Re-Transmit disabled
									// 1 – Up to 1 Re-Transmission on fail of AA
									// ..
									// 15 – Up to 15 Re-Transmission on fail of AA

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RF_CH				adr: 0x05		disc: Sets the frequency channel	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RF_CH 23						//RF_CH = 0 to 127 
									//Freq = 2400 + RF_CH [MHz]
									
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RF_SETUP				adr: 0x06		disc: RF Setup Register	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RF_DR 1						//Air Data Rate
									// 0 – 1Mbps
									// 1 – 2Mbps
//***********************************************************************************************************//
#define B0_RF_PWR 3					//Set RF output power in TX mode
									// 0 – -10 dBm
									// 1 – -5 dBm
									// 2 – 0 dBm
									// 3 – 5 dBm
//***********************************************************************************************************//
#define B0_LNA_HCURR 1					//Setup LNA gain
									// 0:Low gain(20dB down)
									// 1:High gain
									
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: STATUS				adr: 0x07		disc: Status Register	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	B0_RX_DR 1						//Data Ready RX FIFO interrupt
									//Asserted when new data arrives RX FIFO
									//Write 1 to clear bit.
//***********************************************************************************************************//
#define B0_TX_DS 1						//Data Sent TX FIFO interrupt
									//Asserted when packet transmitted on TX. If
									//AUTO_ACK is activated, this bit is set high
									//only when ACK is received.
									//Write 1 to clear bit.
//***********************************************************************************************************//
#define B0_MAX_RT 1					//Maximum number of TX retransmits interrupt
									//Write 1 to clear bit. If MAX_RT is asserted
									//it must be cleared to enable further communication.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_ADDR_P2			adr: 0x0C		disc: Receive address data pipe 2. Only LSB	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_ADDR_P2 0xC3				//Receive address data pipe 2. Only LSB
									//MSB bytes is equal to RX_ADDR_P1[39:8]

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_ADDR_P2			adr: 0x0D		disc: Receive address data pipe 3. Only LSB	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	B0_RX_ADDR_P3 0xC4				//Receive address data pipe 3. Only LSB
									//MSB bytes is equal to RX_ADDR_P1[39:8]

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_ADDR_P2			adr: 0x0E		disc: Receive address data pipe 4. Only LSB	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_ADDR_P4 0xC5				//Receive address data pipe 4. Only LSB.
									//MSB bytes is equal to RX_ADDR_P1[39:8]
									
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_ADDR_P2			adr: 0x0F		disc: Receive address data pipe 4. Only LSB	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_ADDR_P5 0xC6				//Receive address data pipe 5. Only LSB.
									//MSB bytes is equal to RX_ADDR_P1[39:8]
									
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_PW_P0				adr: 0x11		disc: Number of bytes in RX payload in data pipe 0	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	B0_RX_PW_P0 32					//Number of bytes in RX payload in data pipe
									//0 (1 to 32 bytes).
									//0: not used
									//1 = 1 byte
									//..
									//32 = 32 bytes
									
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_PW_P1				adr: 0x12		disc: Number of bytes in RX payload in data pipe 1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_PW_P1 32					//as RX_PW_P0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_PW_P2				adr: 0x13		disc: Number of bytes in RX payload in data pipe 1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_PW_P2 32					//as RX_PW_P0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_PW_P3				adr: 0x14		disc: Number of bytes in RX payload in data pipe 1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_PW_P3 32					//as RX_PW_P0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_PW_P4				adr: 0x15		disc: Number of bytes in RX payload in data pipe 1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_PW_P4 32					//as RX_PW_P0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: RX_PW_P5				adr: 0x16		disc: Number of bytes in RX payload in data pipe 1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_RX_PW_P5 32					//as RX_PW_P0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: DYNPD				adr: 0x1C		disc: Enable dynamic payload length
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_DPL_P5 1					//Enable dynamic payload length data pipe 5.
//***********************************************************************************************************//
#define B0_DPL_P4 1					//Enable dynamic payload length data pipe 4.
//***********************************************************************************************************//
#define B0_DPL_P3 1					//Enable dynamic payload length data pipe 3.
//***********************************************************************************************************//
#define B0_DPL_P2 1					//Enable dynamic payload length data pipe 2.
//***********************************************************************************************************//
#define B0_DPL_P1 1					//Enable dynamic payload length data pipe 1.
//***********************************************************************************************************//
#define B0_DPL_P0 1					//Enable dynamic payload length data pipe 0.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//reg: FEATUR				adr: 0x1D		disc: Feature Register
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define B0_EN_DPL 1					//Enables Dynamic Payload Length
//***********************************************************************************************************//
#define B0_EN_ACK_PAY 1				//Enables Payload with ACK
//***********************************************************************************************************//
#define B0_EN_DYN_ACK 1				//Enables the W_TX_PAYLOAD_NOACK command			
									
									
									
									
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										do not edit under this line											 //
//BANK 0
#define B0_CONFIG (B0_MASK_RX_DR << 6 | B0_MASK_TX_DS << 5 | B0_MASK_MAX_RT << 4 | B0_EN_CRC << 3 | B0_CRCO << 2 | B0_PWR_UP << 1 | B0_PRIM_RX)
#define B0_ENAA (B0_ENAA_P5 << 5 | B0_ENAA_P4 << 4 | B0_ENAA_P3 <<3 | B0_ENAA_P2 << 2 | B0_ENAA_P1 << 1 | B0_ENAA_P0)
#define B0_EN_RXADDR (B0_ERX_P5 << 5 | B0_ERX_P4 << 4 | B0_ERX_P3 << 3 | B0_ERX_P2 << 2 | B0_ERX_P1 << 1 | B0_ERX_P0)
#define B0_SETUP_AW B0_AW
#define B0_SETUP_RETR (B0_ARD << 4 | B0_ARC)
//RF_CH
#define B0_RF_SETUP (0x30 | B0_RF_DR << 3 | B0_RF_PWR << 1 | B0_LNA_HCURR)
#define B0_STATUS (B0_RX_DR << 6 | B0_TX_DS << 5 | B0_MAX_RT << 4)
#define B0_DYNPD (B0_DPL_P5 << 5 | B0_DPL_P4 << 4 | B0_DPL_P3 << 3 | B0_DPL_P2 << 2 | B0_DPL_P1 << 1 | B0_DPL_P0)
#define B0_FEATUR (B0_EN_DPL << 2 | B0_EN_ACK_PAY << 1 | B0_EN_DYN_ACK)

#endif /* RFM70_H_ */
