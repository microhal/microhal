/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 12-03-2014
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

#include "rfm70.h"
#include "rfm70_conf.h"

using namespace microhal;
using namespace diagnostic;

const uint8_t RFM70::RX0_Address[] = { 0x34, 0x43, 0x10, 0x10, 0x01 }; //Receive address data pipe 0
const uint8_t RFM70::RX1_Address[] = { 0x39, 0x38, 0x37, 0x36, 0xc2 }; ////Receive address data pipe 1

/**
 * This table contain configuration data for bank 0 registers.
 */
const uint8_t RFM70::Bank0_Reg[BANK0_SIZE][2] = { { WRITE_REG | 0x00, B0_CONFIG }, //
        { WRITE_REG | 0x01, B0_ENAA }, //
        { WRITE_REG | 0x02, B0_EN_RXADDR }, //
        { WRITE_REG | 0x03, B0_SETUP_AW }, //
        { WRITE_REG | 0x04, B0_SETUP_RETR }, //
        { WRITE_REG | 0x05, B0_RF_CH }, //
        { WRITE_REG | 0x06, B0_RF_SETUP }, //
        { WRITE_REG | 0x07, B0_STATUS }, //
        //{WRITE_REG| 0x08, 0}, //
        //{WRITE_REG| 0x09, 0}, //
        { WRITE_REG | 0x0C, B0_RX_ADDR_P2 }, //
        { WRITE_REG | 0x0D, B0_RX_ADDR_P3 }, //
        { WRITE_REG | 0x0E, B0_RX_ADDR_P4 }, //
        { WRITE_REG | 0x0F, B0_RX_ADDR_P5 }, //
        { WRITE_REG | 0x11, B0_RX_PW_P0 }, //
        { WRITE_REG | 0x12, B0_RX_PW_P1 }, //
        { WRITE_REG | 0x13, B0_RX_PW_P2 }, //
        { WRITE_REG | 0x14, B0_RX_PW_P3 }, //
        { WRITE_REG | 0x15, B0_RX_PW_P4 }, //
        { WRITE_REG | 0x16, B0_RX_PW_P5 }, //
        { WRITE_REG | 0x17, 0x00 }, //fifo status
        { WRITE_REG | 0x1C, B0_DYNPD }, //
        { WRITE_REG | 0x1D, B0_FEATUR } };
/**
 * This table contain configuration data for bank 1 registers.
 */
const uint8_t RFM70::Bank1_Registers_0x00_to_0x05[][4] = { { 0x40, 0x4B, 0x01, 0xE2 }, //register address 0x00
        { 0xC0, 0x4B, 0x00, 0x00 }, //register address 0x01
        { 0xD0, 0xFC, 0x8C, 0x02 }, //register address 0x02
        { 0x99, 0x00, 0x39, 0x41 }, //register address 0x03
        { 0xD9, 0x9E, 0x86, 0x0B }, //register address 0x04
        { 0x24, 0x06, 0x7F, 0xA6 } }; //register address 0x05

const uint8_t RFM70::Bank1_Reg0C[] = { 0x00, 0x12, 0x73, 0x00 };
const uint8_t RFM70::Bank1_Reg0D[] = { 0x36, 0xB4, 0x80, 0x00 };

const uint8_t RFM70::Bank1_Reg0E[] = { 0x41, 0x20, 0x08, 0x04, 0x81, 0x20, 0xCF, 0xF7, 0xFE, 0xFF,
        0xFF };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief This function switch transceiver mode to receiver.
 *
 */
void RFM70::switchToRX(void) {
    uint8_t tmp;

    diagChannel << Debug << "RFM70: Switching to RX mode" << endl;

    readRegister(STATUS, tmp);
    writeRegister(WRITE_REG | STATUS, tmp);

    writeRegister(FLUSH_RX, (uint8_t) 0x00); //flush Rx
    //write(FLUSH_RX);

    cePin.reset();

    readRegister(CONFIG, tmp);   // read register CONFIG's value
    tmp |= 0x01;   //set bit 1
    writeRegister(WRITE_REG | CONFIG, tmp); // Set PWR_UP bit, enable CRC(2 length) & Prim:RX. RX_DR enabled..

    cePin.set();
} /* switchToRX */
/**
 * @brief This function switch transceiver mode to transmitter.
 *
 */
void RFM70::switchToTX(void) {
    uint8_t tmp;

    diagChannel << Debug << "RFM70: Switchnig to TX mode" << endl;

    readRegister(STATUS, tmp);
    writeRegister(WRITE_REG | STATUS, tmp);

    writeRegister(FLUSH_TX, (uint8_t) 0);
    //write(FLUSH_TX);

    cePin.reset();

    readRegister(CONFIG, tmp); // read register CONFIG's value
    tmp &= 0xfe; //set bit 0
    writeRegister(WRITE_REG | CONFIG, tmp); // Set PWR_UP bit, enable CRC(2 length) & Prim:RX. RX_DR enabled.

    cePin.set();
} /* switchToTX */
/**
 * @brief This function switch registers bank.
 *
 * @param cfg - bank to switch.
 */
void RFM70::switchCFG(Cfg cfg) {
    uint8_t status;

    //read status register
    readRegister(STATUS, status);

    //if actual bank is different than bank in cfg parameter
    if ((status & 0x80) != cfg) {
        writeRegister(ACTIVATE_CMD, (uint8_t) 0x53);
    }
} /* switchCFG */
/**
 * @brief This function write bank 0 registers.
 *
 * @retval true if registers was initialized successfully.
 * @retval false if an error occurred.
 */
bool RFM70::initBank0(void) {
    uint8_t i = 0;

    switchCFG(BANK0);

    for (uint_fast8_t i = 0; i < BANK0_SIZE - 2; i++) {
        if (writeRegister(Bank0_Reg[i][0], Bank0_Reg[i][1]) == false) {
            return false;
        }
    }

    if (writeRegisters((WRITE_REG | RX_ADDR_P0), RX0_Address, 5) == false) return false;
    if (writeRegisters((WRITE_REG | RX_ADDR_P1), RX1_Address, 5) == false) return false;
    if (writeRegisters((WRITE_REG | TX_ADDR), RX0_Address, 5) == false) return false;

    if (readRegister(FEATURE, i) == true) {
        if (i == 0) { // i!=0 showed that chip has been actived.so do not active again.
            if (writeRegister(ACTIVATE_CMD, (uint8_t) 0x73) == false) return false; // Active
        }

        if (writeRegister(Bank0_Reg[BANK0_SIZE - 1][0], Bank0_Reg[BANK0_SIZE - 1][1]) == false) {
            return false;
        }
        if (writeRegister(Bank0_Reg[BANK0_SIZE - 2][0], Bank0_Reg[BANK0_SIZE - 2][1]) == false) {
            return false;
        }
        return true;
    }
    return false;
} /* initBank0 */
/**
 * @brief This function write bank 1 registers.
 *
 * @retval true if registers was initialized successfully.
 * @retval false if an error occurred.
 */
bool RFM70::initBank1(void) {
    //switch to bank 1
    switchCFG(BANK1);

    for (uint_fast8_t i = 0; i < 6; i++) {
        if (writeRegisters((WRITE_REG | i), Bank1_Registers_0x00_to_0x05[i], 4) == false) {
            return false;
        }
    }

    if (writeRegisters((WRITE_REG | 0x0C), Bank1_Reg0C, 4) == false) return false;
    if (writeRegisters((WRITE_REG | 0x0D), Bank1_Reg0D, 4) == false) return false;
    if (writeRegisters((WRITE_REG | 0x0E), Bank1_Reg0E, 11) == false) return false;

    return true;
} /* initBank1 */
/**
 * @brief This function check device availability and if is present initialize registers in bank 0 and bank 1.
 *
 * @retval true if initialization was completed.
 * @retval false if an error occurred.
 */
bool RFM70::init(void) {
    uint32_t id;
    diagChannel << Debug << "RFM70: Initializing" << endl;

    //if device id is diferent than expected return error.
    if (getID() != ID_VALUE) {
        diagChannel << Error << "RFM70: Error in init function, ID mismatch." << endl;
        return false;
    }

    //initialize bank 0, if an error occurred return false
    if (initBank0() == false) {
        diagChannel << Error << "RFM70: Error in init function, initializing bank 0 failed." << endl;
        return false;
    }

    //initialize bank 1, if an error occurred return false
    if (initBank1() == false) {
        diagChannel << Error << "RFM70: Error in init function, initializing bank 1 failed." << endl;
        return false;
    }

    //_delay_ms(10);

    switchCFG(BANK0);
    switchToRX();

    return true;
} /* init */
///**
// *
// * @param type
// * @param buf
// * @param len
// */
//void RFM70::sendPacket(uint8_t type, const uint8_t *buf, uint8_t len) {
//    //SwitchToTxMode();  //switch to tx mode
//    uint8_t status;
//
//    //_delay_ms(1);
//    writeRegisters(type, buf, len); // Writes data to buffer
//    //_delay_ms(1);
//    do {
//        readRegister(STATUS, status);
//    } while (!(status & 1 << 5));  //czekaj az dane zostana wslane
//
//    //SwitchToRxMode();
//}
/**
 *
 * @param rx_buf
 */
bool RFM70::getPacket(uint8_t *rx_buf) {
    uint8_t len, status;
//SwitchToRxMode();
    SPIDevice::readRegister(STATUS, status);   // read register STATUS's value

    if (status & STATUS_RX_DR) {
        readRegister(R_RX_PL_WID_CMD, len);  // read len
        if (len == 0) {
            return false;
        }
        SPIDevice::readRegisters((uint8_t) RD_RX_PLOAD, rx_buf, len); // read receive payload from RX_FIFO buffer
        writeRegister(FLUSH_RX, (uint8_t) 0x00);

        //SPI_Write_Reg(WRITE_REG|STATUS, sta);// clear RX_DR or TX_DS or MAX_RT interrupt flag
        writeRegister((WRITE_REG | STATUS), (uint8_t)(status | (1 << 6)));

        return true;
    }
    return false;
}

void RFM70::bank0RegisterDump(void) {
    uint8_t reg;

    for (uint_fast8_t addr = 0; addr <= 0x1d; addr++) {
        readRegister(addr, reg);

        diagChannel << Debug << "RFM70: reg: 0x" << toHex(addr) << " val: 0x" << toHex(reg) << endl;
    }
}

void RFM70::bank1RegisterDump(void) {
    uint32_t reg;

    switchCFG(BANK1);

    diagChannel << Debug << "RFM70: Registers in BANK 1" << endl << endl;

    for (uint_fast8_t addr = 0; addr <= 0x0E; addr++) {
        readRegister(addr, reg, Endianness::BigEndian);

        diagChannel << Debug << "RFM70: reg: 0x" << toHex(addr) << " val: 0x" << toHex(reg) << endl;
    }
    switchCFG(BANK0);
}

void RFM70::debug() {
    uint8_t conf;
    readRegister(CONFIG, conf);

    diagChannel << Debug << "RFM70: Config reg: " << toHex(conf) << endl;

    readRegister(STATUS, conf);

    diagChannel << Debug << "RFM70: Status reg: " << toHex(conf) << endl;
}
