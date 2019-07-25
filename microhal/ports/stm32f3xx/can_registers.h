/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_STM32F3XX_H_
#define _MICROHAL_STM32F3XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "device/stm32f3xx.h"

#undef CAN
#undef CAN1
#undef CAN2

namespace microhal {
namespace stm32f3xx {
namespace registers {
/**
 * @brief Controller Area Network TxMailBox
 */
typedef struct {
    union TIR {
        struct {
            uint32_t TXRQ : 1;
            uint32_t RTR : 1;
            uint32_t IDE : 1;
            uint32_t STID_EXID : 29;
        } bitfield;
        uint32_t raw;
    };
    union TDTxR {
        struct {
            uint32_t DLC : 4;       /*!< Data length code */
            uint32_t Reserved0 : 4; /* */
            uint32_t TGT : 1;       /*!< Transmit global time */
            uint32_t Reserved1 : 7; /* */
            uint32_t TIME : 16;     /*!< Message time stamp */
        } bitfield;
        uint32_t raw;
    };
    static_assert(sizeof(TDTxR) == sizeof(uint32_t), "");
    volatile TIR tir;       /*!< CAN TX mailbox identifier register */
    volatile TDTxR tdtr;    /*!< CAN mailbox data length control and time stamp register */
    volatile uint32_t TDLR; /*!< CAN mailbox data low register */
    volatile uint32_t TDHR; /*!< CAN mailbox data high register */

} CAN_TxMailBox_TypeDef;

/**
 * @brief Controller Area Network FIFOMailBox
 */
typedef struct {
    union RIR {
        struct {
            uint32_t Reserved : 1;
            uint32_t RTR : 1;
            uint32_t IDE : 1;
            uint32_t STID_EXID : 29;
        } bitfield;
        uint32_t raw;
    };
    union RDTR {
        struct {
            uint32_t DLC : 4;
            uint32_t Reserved : 4;
            uint32_t FMI : 8;
            uint32_t TIME : 16;
        } bitfield;
        uint32_t raw;
    };
    volatile RIR rir;       /*!< CAN receive FIFO mailbox identifier register */
    volatile RDTR rdtr;     /*!< CAN receive FIFO mailbox data length control and time stamp register */
    volatile uint32_t RDLR; /*!< CAN receive FIFO mailbox data low register */
    volatile uint32_t RDHR; /*!< CAN receive FIFO mailbox data high register */
} CAN_FIFOMailBox_TypeDef;

/**
 * @brief Controller Area Network FilterRegister
 */
typedef struct {
    struct ID {
        uint32_t zero : 1;
        uint32_t rtr : 1;
        uint32_t ide : 1;
        uint32_t stid_exid : 29;
    };
    union {
        struct {
            __IO uint32_t FR1; /*!< CAN Filter bank register 1 */
            __IO uint32_t FR2; /*!< CAN Filter bank register 1 */
        } raw;
        struct {
            volatile ID id;   /*!< CAN Filter bank register 1 */
            volatile ID mask; /*!< CAN Filter bank register 1 */
        } identifierMask;
        struct {
            volatile ID id[2];
        } identifierList;
        struct {
            uint16_t id1;
            uint16_t mask1;
            uint16_t id2;
            uint16_t mask2;
        } identifierMask16bit;
        struct {
            uint16_t id[4];
        } identifierList16bit;
    };
} CAN_FilterRegister_TypeDef;
static_assert(sizeof(CAN_FilterRegister_TypeDef) == 2 * sizeof(uint32_t), "");

/**
 * @brief Controller Area Network
 */
class CAN {
 public:
    enum class Device {
#if defined(CAN_BASE)
        CAN1 = CAN_BASE
#endif
    };

    static constexpr uint_fast8_t getNumber(Device device) {
        switch (device) {
#if defined(CAN_BASE)
            case Device::CAN1:
                return 1;
#endif
        }
        return 0;
    }

    union MCR {
        struct {
            uint32_t INRQ : 1;
            uint32_t SLEEP : 1;
            uint32_t TXFP : 1;
            uint32_t RFLM : 1;
            uint32_t NART : 1;
            uint32_t AWUM : 1;
            uint32_t ABOM : 1;
            uint32_t TTCM : 1;
            uint32_t Reserved : 7;
            uint32_t RESET : 1;
            uint32_t DBF : 1;
        } bitfield;
        uint32_t raw;
    };

    union MSR {
        struct {
            const uint32_t INAK : 1;
            const uint32_t SLAK : 1;
            uint32_t ERRI : 1;
            uint32_t WKUI : 1;
            uint32_t SLAKI : 1;
            uint32_t reserved : 3;
            uint32_t TXM : 1;
            uint32_t RXM : 1;
            uint32_t SAMP : 1;
            uint32_t RX : 1;
        } bitfield;
        uint32_t raw;
    };

    union TSR {
        struct {
            uint32_t RQCP0 : 1;
            uint32_t TXOK0 : 1;
            uint32_t ALST0 : 1;
            uint32_t TERR0 : 1;
            const uint32_t Reserved1 : 3;
            uint32_t ABRQ0 : 1;
            uint32_t RQCP1 : 1;
            uint32_t TXOK1 : 1;
            uint32_t ALST1 : 1;
            uint32_t TERR1 : 1;
            const uint32_t Reserved2 : 3;
            uint32_t ABRQ1 : 1;
            uint32_t RQCP2 : 1;
            uint32_t TXOK2 : 1;
            uint32_t TERR2 : 1;
            const uint32_t Reserved3 : 3;
            uint32_t ABRQ2 : 1;
            uint32_t CODE : 2;
            uint32_t TME0 : 1;
            uint32_t TME1 : 1;
            uint32_t TME2 : 1;
            uint32_t LOW0 : 1;
            uint32_t LOW1 : 1;
            uint32_t LOW2 : 1;
        } bitfield;
        uint32_t raw;
    };

    union RFxR {
        struct {
            uint32_t FMPx : 2;
            uint32_t Reserved : 1;
            uint32_t FULLx : 1;
            uint32_t FOVRx : 1;
            uint32_t RFOMx : 1;
        } bitfield;
        uint32_t raw;

        uint_fast8_t mesagesCount() const volatile { return bitfield.FMPx; }
        void releaseMessage() volatile { bitfield.RFOMx = 1; }
        bool isFull() const volatile { return bitfield.FULLx; }
    };

    union IER {
        struct {
            uint32_t TMEIE : 1;
            uint32_t FMPIO0 : 1;
            uint32_t FFIO0 : 1;
            uint32_t FOVIE0 : 1;
            uint32_t FMPIE1 : 1;
            uint32_t FFIE1 : 1;
            uint32_t FOVIE1 : 1;
            uint32_t Reserved1 : 1;
            uint32_t EWGIE : 1;
            uint32_t EPVIE : 1;
            uint32_t BOFIO : 1;
            uint32_t LECIO : 1;
            uint32_t Reserved2 : 3;
            uint32_t ERRIE : 1;
            uint32_t WKUIE : 1;
            uint32_t SLKIE : 1;
        } bitfield;
        uint32_t raw;

        enum class Interrupt {
            Sleep = 1 << 17,
            Wakeup = 1 << 16,
            Error = 1 << 15,
            LastErrorCode = 1 << 11,
            BussOff = 1 << 10,
            ErrorPasive = 1 << 9,
            ErrorWarning = 1 << 8,
            FIFO1_Overrun = 1 << 6,
            FIFO1_Full = 1 << 5,
            FIFO1_MessagePending = 1 << 4,
            FIFO0_Overrun = 1 << 3,
            FIFO0_Full = 1 << 2,
            FIFO0_MessagePending = 1 << 1,
            TransmitMailboxEmpty = 1 << 0
        };
        void enableInterrupt(Interrupt interrupt) volatile { raw |= static_cast<uint32_t>(interrupt); }
        void disableInterrupt(Interrupt interrupt) volatile { raw &= ~static_cast<uint32_t>(interrupt); }
    };

    union ESR {
        struct {
            uint32_t EWGF : 1;
            uint32_t EPVF : 1;
            uint32_t BOFF : 1;
            uint32_t Reserved1 : 1;
            uint32_t LEC : 3;
            uint32_t Reserved2 : 9;
            uint32_t TEC : 8;
            uint32_t REC : 8;
        } bitfield;
        uint32_t raw;
    };

    union BTR {
        struct {
            uint32_t BRP : 10;
            uint32_t Reserved1 : 6;
            uint32_t TS1 : 4;
            uint32_t TS2 : 3;
            uint32_t Reserved2 : 1;
            uint32_t SJW : 2;
            uint32_t Reserved3 : 4;
            uint32_t LBKM : 1;
            uint32_t SILM : 1;
        } bitfield;
        uint32_t raw;
    };

    union FMR {
        struct {
            uint32_t FINIT : 1;
            uint32_t Reserved : 7;
            uint32_t CAN2SB : 6;
        } bitfield;
        uint32_t raw;
    };

    volatile MCR mcr;                               /*!< CAN master control register,         Address offset: 0x00          */
    volatile MSR msr;                               /*!< CAN master status register,          Address offset: 0x04          */
    volatile TSR tsr;                               /*!< CAN transmit status register,        Address offset: 0x08          */
    volatile RFxR rf0r;                             /*!< CAN receive FIFO 0 register,         Address offset: 0x0C          */
    volatile RFxR rf1r;                             /*!< CAN receive FIFO 1 register,         Address offset: 0x10          */
    volatile IER ier;                               /*!< CAN interrupt enable register,       Address offset: 0x14          */
    volatile ESR esr;                               /*!< CAN error status register,           Address offset: 0x18          */
    volatile BTR btr;                               /*!< CAN bit timing register,             Address offset: 0x1C          */
    uint32_t RESERVED0[88];                         /*!< Reserved, 0x020 - 0x17F                                            */
    CAN_TxMailBox_TypeDef sTxMailBox[3];            /*!< CAN Tx MailBox,                      Address offset: 0x180 - 0x1AC */
    CAN_FIFOMailBox_TypeDef sFIFOMailBox[2];        /*!< CAN FIFO MailBox,                    Address offset: 0x1B0 - 0x1CC */
    uint32_t RESERVED1[12];                         /*!< Reserved, 0x1D0 - 0x1FF                                            */
    volatile FMR fmr;                               /*!< CAN filter master register,          Address offset: 0x200         */
    volatile uint32_t FM1R;                         /*!< CAN filter mode register,            Address offset: 0x204         */
    uint32_t RESERVED2;                             /*!< Reserved, 0x208                                                    */
    volatile uint32_t FS1R;                         /*!< CAN filter scale register,           Address offset: 0x20C         */
    uint32_t RESERVED3;                             /*!< Reserved, 0x210                                                    */
    volatile uint32_t FFA1R;                        /*!< CAN filter FIFO assignment register, Address offset: 0x214         */
    uint32_t RESERVED4;                             /*!< Reserved, 0x218                                                    */
    volatile uint32_t FA1R;                         /*!< CAN filter activation register,      Address offset: 0x21C         */
    uint32_t RESERVED5[8];                          /*!< Reserved, 0x220-0x23F                                              */
    CAN_FilterRegister_TypeDef sFilterRegister[28]; /*!< CAN Filter Register,                 Address offset: 0x240-0x31C   */
};

constexpr CAN::IER::Interrupt operator|(CAN::IER::Interrupt a, CAN::IER::Interrupt b) {
    return static_cast<CAN::IER::Interrupt>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

}  // namespace registers
}  // namespace stm32f3xx
}  // namespace microhal

#endif  // _MICROHAL_STM32F3XX_H_
