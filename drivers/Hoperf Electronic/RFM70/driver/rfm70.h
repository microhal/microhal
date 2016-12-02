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

#ifndef RFM70_H_
#define RFM70_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "microhal.h"
#include "SPIDevice/SPIDevice.h"
/* ************************************************************************************************
 * DEFINES
 */
//FIFO_STATUS
#define FIFO_STATUS_TX_REUSE    0x40
#define FIFO_STATUS_TX_FULL     0x20
#define FIFO_STATUS_TX_EMPTY    0x10

#define FIFO_STATUS_RX_FULL     0x02
#define FIFO_STATUS_RX_EMPTY    0x01
/* ************************************************************************************************
 * CLASS
 */
class RFM70: private microhal::SPIDevice {
private:
    /**
     * Hardware limitations
     */
    enum Hardware {
        BANK0_SIZE = 21 //!< BANK0_SIZE
    };
    /**
     * Registers bank definition
     */
    typedef enum {
        BANK0 = 0x00, //!< BANK0
        BANK1 = 0x80 //!< BANK1
    } Cfg;

    /**
     * Possible commands
     */
    enum Commands {
        READ_REG = 0x00,  ///< Define read command to register
        WRITE_REG = 0x20,  ///< Define write command to register
        RD_RX_PLOAD = 0x61,  ///< Define RX payload register address
        WR_TX_PLOAD = 0xA0,  ///< Define TX payload register address
        FLUSH_TX = 0xE1,  ///< Define flush TX register command
        FLUSH_RX = 0xE2,  ///< Define flush RX register command
        REUSE_TX_PL = 0xE3,  ///< Define reuse TX payload register command
        W_TX_PAYLOAD_NOACK_CMD = 0xB0,  //!< W_TX_PAYLOAD_NOACK_CMD
        W_ACK_PAYLOAD_CMD = 0xa8,     //!< W_ACK_PAYLOAD_CMD
        ACTIVATE_CMD = 0x50,          //!< ACTIVATE_CMD
        R_RX_PL_WID_CMD = 0x60,       //!< R_RX_PL_WID_CMD
        NOP_NOP = 0xFF,  ///< Define No Operation, might be used to read status register
    };
    /**
     * Registers addresses
     */
    enum Registers {
        CONFIG = 0x00,  ///< 'Config' register address
        EN_AA = 0x01,  ///< 'Enable Auto Acknowledgment' register address
        EN_RXADDR = 0x02,  ///< 'Enabled RX addresses' register address
        SETUP_AW = 0x03,  ///< 'Setup address width' register address
        SETUP_RETR = 0x04,  ///< 'Setup Auto. Retrans' register address
        RF_CH = 0x05,  ///< 'RF channel' register address
        RF_SETUP = 0x06,  ///< 'RF setup' register address
        STATUS = 0x07,  ///< 'Status' register address
        OBSERVE_TX = 0x08,  ///< 'Observe TX' register address
        CD = 0x09,  ///< 'Carrier Detect' register address
        RX_ADDR_P0 = 0x0A,  ///< 'RX address pipe0' register address
        RX_ADDR_P1 = 0x0B,  ///< 'RX address pipe1' register address
        RX_ADDR_P2 = 0x0C,  ///< 'RX address pipe2' register address
        RX_ADDR_P3 = 0x0D,  ///< 'RX address pipe3' register address
        RX_ADDR_P4 = 0x0E,  ///< 'RX address pipe4' register address
        RX_ADDR_P5 = 0x0F,  ///< 'RX address pipe5' register address
        TX_ADDR = 0x10,  ///< 'TX address' register address
        RX_PW_P0 = 0x11,  ///< 'RX payload width, pipe0' register address
        RX_PW_P1 = 0x12,  ///< 'RX payload width, pipe1' register address
        RX_PW_P2 = 0x13,  ///< 'RX payload width, pipe2' register address
        RX_PW_P3 = 0x14,  ///< 'RX payload width, pipe3' register address
        RX_PW_P4 = 0x15,  ///< 'RX payload width, pipe4' register address
        RX_PW_P5 = 0x16,  ///< 'RX payload width, pipe5' register address
        FIFO_STATUS = 0x17,  ///< 'FIFO Status Register' register address
        FEATURE = 0x1D,
    // PAYLOAD_WIDTH = 0x1F  ///< 'payload length of 256 bytes modes register address
    };
    /**
     * Registers flags
     */
    enum RegisterFlags {
        STATUS_RX_DR = 0x40, //!< STATUS_RX_DR
        STATUS_TX_DS = 0x20, //!< STATUS_TX_DS
        STATUS_MAX_RT = 0x10, //!< STATUS_MAX_RT
        STATUS_TX_FULL = 0x01 //!< STATUS_TX_FULL
    };
public:
    /**
     *  Value of fixed registers.
     */
    enum ConstRegisterValues {
        ID_VALUE = 0x00000063,                         //!< ID VALUE
    };
    /**
     * Possible packet types.
     */
    typedef enum {
        ACK_PIPE_0 = W_ACK_PAYLOAD_CMD | 0x00,                         //!< ACK_PIPE_0
        ACK_PIPE_1 = W_ACK_PAYLOAD_CMD | 0x01,                         //!< ACK_PIPE_1
        ACK_PIPE_2 = W_ACK_PAYLOAD_CMD | 0x02,                         //!< ACK_PIPE_2
        ACK_PIPE_3 = W_ACK_PAYLOAD_CMD | 0x03,                         //!< ACK_PIPE_3
        ACK_PIPE_4 = W_ACK_PAYLOAD_CMD | 0x04,                         //!< ACK_PIPE_4
        ACK_PIPE_5 = W_ACK_PAYLOAD_CMD | 0x05,                         //!< ACK_PIPE_5
        NO_ACK = W_TX_PAYLOAD_NOACK_CMD       //!< NO_ACK
    } PacketType;

    typedef enum {
        MAX_RETRANSMISION = 0x10, TX_DATA_SENT = 0x20, RX_NEW = 0x40
    } Interrupts;
//---------------------------------------- constructors ---------------------------------------
    RFM70(microhal::SPI &spi, microhal::GPIO::IOPin csnPin, microhal::GPIO::IOPin cePin) :
            microhal::SPIDevice(spi, csnPin), cePin(cePin, microhal::GPIO::Direction::Output) {
        this->cePin.set();
    }
//------------------------------------------ functions ----------------------------------------
    bool init(void);

    inline uint32_t getID();

    inline bool setChannelNumber(uint8_t channelNumber);
    inline bool getChannelNumber(uint8_t &channelNumber);

    void switchToRX();
    void switchToTX();

    inline bool sendPacket(uint8_t *buffer, uint16_t length, uint8_t pipe);
    inline bool sendPacket(uint8_t *buffer, uint16_t length, PacketType type = NO_ACK);

    bool getPacket(uint8_t *rx_buf);

    bool enableInterrupt(Interrupts interrupt);
    bool disableInterrupt(Interrupts interrupt);

    inline bool connectIRQ(void (*func)(RFM70 &rfm), const microhal::GPIO::IOPin pin) __attribute__ ((always_inline)) {
        //interrupt.setVectorNumber(hal::GPIO::PortA, pin);
       // return interrupt.connect((void (*)(void*))func, this, microhal::ExternalInterrupt::Trigger::OnFallingEdge, pin);
    }

    void enableIRQ() {
        //interrupt.enable();
    }

    void disableIRQ() {
        //interrupt.disable();
    }

    bool clearInterruptFlag(Interrupts interrupt) {
        return writeRegister(WRITE_REG | STATUS, interrupt);
    }
    bool getInterruptSource(Interrupts &interrupt) {
        return readRegister(STATUS, (uint8_t&) interrupt);
    }

    void bank0RegisterDump(void);

    void bank1RegisterDump(void);

    void debug();
private:
    microhal::GPIO cePin;
    //microhal::ExternalInterrupt interrupt;

    static const uint8_t Bank0_Reg[][2];
    static const uint8_t Bank1_Registers_0x00_to_0x05[][4];
    static const uint8_t Bank1_Reg0C[4];
    static const uint8_t Bank1_Reg0D[4];
    static const uint8_t Bank1_Reg0E[];

    static const uint8_t RX0_Address[];
    static const uint8_t RX1_Address[];
//------------------------------------------ functions ----------------------------------------
    void switchCFG(Cfg cfg);
    bool initBank0(void);
    bool initBank1(void);

};
/* ************************************************************************************************
 * INLINE FUNCTIONS
 */
/**
 * @brief This function set RF channel.
 *
 * @param channelNumber
 */
bool RFM70::setChannelNumber(uint8_t channelNumber) {
    return writeRegister((WRITE_REG | RF_CH), channelNumber);
}
/**
 *
 * @param channelNumber
 * @return
 */
bool RFM70::getChannelNumber(uint8_t &channelNumber) {
    return readRegister(RF_CH, channelNumber);
}
/**
 *
 * @return
 */
uint32_t RFM70::getID() {
    uint32_t id;

    switchCFG(BANK1);

    if (readRegister(0x08, id, microhal::Endianness::LittleEndian) == false) {
        id = 0x00000000;
    }

    switchCFG(BANK0);

    return id;
}
/**
 *
 * @param buffer
 * @param length
 * @param pipe
 * @return
 */
bool RFM70::sendPacket(uint8_t *buffer, uint16_t length, uint8_t pipe) {
    if (pipe <= 5) {
        return writeRegisters(W_ACK_PAYLOAD_CMD | pipe, buffer, length);
    }
    return false;
}
/**
 *
 * @param buffer
 * @param length
 * @param type
 * @return
 */
bool RFM70::sendPacket(uint8_t *buffer, uint16_t length, PacketType type) {
    return writeRegisters(type, buffer, length);
}
#endif /* RFM70_H_ */
