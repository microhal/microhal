/*
 * can_registers.h
 *
 *  Created on: Apr 5, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STM32F4XX_CAN_REGISTERS_H_
#define _MICROHAL_PORTS_STM32F4XX_CAN_REGISTERS_H_

#include <cstdint>
#include "device/stm32f4xx.h"

#undef CAN1
#undef CAN2

namespace microhal {
namespace stm32f4xx {
namespace registers {

/**
 * @brief Controller Area Network TxMailBox
 */
typedef struct {
    union TIxR {
        struct {
            uint32_t TXRQ : 1;       /*!< Transmit mailbox request */
            uint32_t RTR : 1;        /*!< Remote transmission request */
            uint32_t IDE : 1;        /*!< Identifier extension; 0: Standard identifier; 1: Extended identifier */
            uint32_t STID_EXID : 29; /*!< Standard identifier or extended identifier */
        } bitfield;
        uint32_t raw;
    };
    static_assert(sizeof(TIxR) == sizeof(uint32_t), "");
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

    volatile TIxR tir;      /*!< CAN TX mailbox identifier register */
    volatile TDTxR tdtr;    /*!< CAN mailbox data length control and time stamp register */
    volatile uint32_t TDLR; /*!< CAN mailbox data low register */
    volatile uint32_t TDHR; /*!< CAN mailbox data high register */

    //    bool isEmpty() { return tir.bitfield.TXRQ == 0; }
} CAN_TxMailBox_TypeDef;
static_assert(sizeof(CAN_TxMailBox_TypeDef) == 4 * sizeof(uint32_t), "");

/**
 * @brief Controller Area Network FIFOMailBox
 */
typedef struct {
    union RIR {
        struct {
            uint32_t Reserved : 1;
            uint32_t RTR : 1;        /*!< Remote transmission request */
            uint32_t IDE : 1;        /*!< Identifier extension */
            uint32_t STID_EXID : 29; /*!< Standard identifier or extended identifier */
        } bitfield;
        uint32_t raw;
    };
    union RDTR {
        struct {
            uint32_t DLC : 4;      /*!< Data length code */
            uint32_t Reserved : 4; /* */
            uint32_t FMI : 8;      /*!< Filter match index */
            uint32_t TIME : 16;    /*!< Message time stamp */
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
#if defined(CAN1_BASE)
        CAN1 = CAN1_BASE,
#endif
#if defined(CAN2_BASE)
        CAN2 = CAN2_BASE
#endif
    };

    static constexpr uint_fast8_t getNumber(Device device) {
        switch (device) {
#if defined(CAN1_BASE)
            case Device::CAN1:
                return 1;
#endif
#if defined(CAN2_BASE)
            case Device::CAN2:
                return 2;
#endif
        }
        return 0;
    }

    union MCR {
        struct {
            uint32_t INRQ : 1;     /*!< Initialization request */
            uint32_t SLEEP : 1;    /*!< Sleep mode request */
            uint32_t TXFP : 1;     /*!< Transmit FIFO priority */
            uint32_t RFLM : 1;     /*!< Receive FIFO locked mode */
            uint32_t NART : 1;     /*!< No automatic retransmission */
            uint32_t AWUM : 1;     /*!< Automatic wakeup mode */
            uint32_t ABOM : 1;     /*!< Automatic bus-off management */
            uint32_t TTCM : 1;     /*!< Time triggered communication mode */
            uint32_t Reserved : 7; /*  */
            uint32_t RESET : 1;    /*!< bxCAN software master reset */
            uint32_t DBF : 1;      /*!< Debug freeze */
        } bitfield;
        uint32_t raw;
    };

    union MSR {
        struct {
            uint32_t INAK : 1;     /*!< Initialization acknowledge */
            uint32_t SLAK : 1;     /*!< Sleep acknowledge */
            uint32_t ERRI : 1;     /*!< Error interrupt */
            uint32_t WKUI : 1;     /*!< Wakeup interrupt */
            uint32_t SLAKI : 1;    /*!< Sleep acknowledge interrupt */
            uint32_t reserved : 3; /* */
            uint32_t TXM : 1;      /*!< Transmit mode */
            uint32_t RXM : 1;      /*!< Receive mode */
            uint32_t SAMP : 1;     /*! Last sample point */
            uint32_t RX : 1;       /*!< CAN Rx signal */
        } bitfield;
        uint32_t raw;
    };

    union TSR {
        struct {
            uint32_t RQCP0 : 1; /*!< Request completed mailbox 0 */
            uint32_t TXOK0 : 1; /*!< Transmission OK of mailbox 0 */
            uint32_t ALST0 : 1; /*!< Arbitration lost for mailbox 0 */
            uint32_t TERR0 : 1; /*!< Transmission error of mailbox 0 */
            uint32_t Reserved1 : 3;
            uint32_t ABRQ0 : 1; /*!< Abort request for mailbox 0 */
            uint32_t RQCP1 : 1; /*!< Request completed mailbox1 */
            uint32_t TXOK1 : 1; /*!< Transmission OK of mailbox 1 */
            uint32_t ALST1 : 1; /*!< Arbitration lost for mailbox 1 */
            uint32_t TERR1 : 1; /*!< Transmission error of mailbox 1 */
            uint32_t Reserved2 : 3;
            uint32_t ABRQ1 : 1; /*!< Abort request for mailbox 1 */
            uint32_t RQCP2 : 1; /*!< Request completed mailbox2 */
            uint32_t TXOK2 : 1; /*!< Transmission OK of mailbox 2 */
            uint32_t ALST2 : 1; /*!< Arbitration lost for mailbox 2 */
            uint32_t TERR2 : 1; /*!< Transmission error of mailbox 2 */
            uint32_t Reserved3 : 3;
            uint32_t ABRQ2 : 1;         /*!< Abort request for mailbox 2 */
            uint32_t CODE : 2;          /*!< Mailbox code */
            volatile uint32_t TME0 : 1; /*!< Transmit mailbox 0 empty */
            uint32_t TME1 : 1;          /*!< Transmit mailbox 1 empty */
            uint32_t TME2 : 1;          /*!< Transmit mailbox 2 empty */
            uint32_t LOW0 : 1;          /*!< Lowest priority flag for mailbox 0 */
            uint32_t LOW1 : 1;          /*!< Lowest priority flag for mailbox 1 */
            uint32_t LOW2 : 1;          /*!< Lowest priority flag for mailbox 2 */
        } bitfield;
        uint32_t raw;
    };

    struct RFxR {
        union {
            struct {
                uint32_t FMPx : 2; /*!< FIFO x message pending */
                uint32_t Reserved : 1;
                uint32_t FULLx : 1; /*!< FIFO x full */
                uint32_t FOVRx : 1; /*!< FIFO x overrun */
                uint32_t RFOMx : 1; /*!< Release FIFO x output mailbox */
            } bitfield;
            uint32_t raw;
        };
        uint_fast8_t mesagesCount() const volatile { return bitfield.FMPx; }
        void releaseMessage() volatile { bitfield.RFOMx = 1; }
        bool isFull() const volatile { return bitfield.FULLx; }
    };

    union IER {
        struct {
            uint32_t TMEIE : 1;  /*!< Transmit mailbox empty interrupt enable */
            uint32_t FMPIE0 : 1; /*!< FIFO message pending interrupt enable */
            uint32_t FFIE0 : 1;  /*!< FIFO full interrupt enable */
            uint32_t FOVIE0 : 1; /*!< FIFO overrun interrupt enable */
            uint32_t FMPIE1 : 1; /*!< FIFO message pending interrupt enable */
            uint32_t FFIE1 : 1;  /*!< FIFO full interrupt enable */
            uint32_t FOVIE1 : 1; /*!< FIFO overrun interrupt enable */
            uint32_t Reserved1 : 1;
            uint32_t EWGIE : 1; /*!< Error warning interrupt enable */
            uint32_t EPVIE : 1; /*!< Error passive interrupt enable */
            uint32_t BOFIe : 1; /*!< Bus-off interrupt enable */
            uint32_t LECIe : 1; /*!< Last error code interrupt enable */
            uint32_t Reserved2 : 3;
            uint32_t ERRIE : 1; /*!< Error interrupt enable */
            uint32_t WKUIE : 1; /*!< Wakeup interrupt enable */
            uint32_t SLKIE : 1; /*!< Sleep interrupt enable */
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
            uint32_t EWGF : 1; /*!< Error warning flag */
            uint32_t EPVF : 1; /*!< Error passive flag */
            uint32_t BOFF : 1; /*!< Bus-off flag */
            uint32_t Reserved1 : 1;
            uint32_t LEC : 3; /*!< Last error code */
            uint32_t Reserved2 : 9;
            uint32_t TEC : 8; /*!< Least significant byte of the 9-bit transmit error counter */
            uint32_t REC : 8; /*!< Receive error counter */
        } bitfield;
        uint32_t raw;
    };

    union BTR {
        struct {
            uint32_t BRP : 10;      /*!< Baud rate prescaler */
            uint32_t Reserved1 : 6; /* */
            uint32_t TS1 : 4;       /*!< Time segment 1 */
            uint32_t TS2 : 3;       /*!< Time segment 2 */
            uint32_t Reserved2 : 1; /**/
            uint32_t SJW : 2;       /*!< Resynchronization jump width */
            uint32_t Reserved3 : 4; /* */
            uint32_t LBKM : 1;      /*!<  Loop back mode (debug) */
            uint32_t SILM : 1;      /*!< Silent mode (debug) */
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

    volatile MCR mcr;                                 /*!< CAN master control register,         Address offset: 0x00          */
    volatile MSR msr;                                 /*!< CAN master status register,          Address offset: 0x04          */
    volatile TSR tsr;                                 /*!< CAN transmit status register,        Address offset: 0x08          */
    volatile RFxR rf0r;                               /*!< CAN receive FIFO 0 register,         Address offset: 0x0C          */
    volatile RFxR rf1r;                               /*!< CAN receive FIFO 1 register,         Address offset: 0x10          */
    volatile IER ier;                                 /*!< CAN interrupt enable register,       Address offset: 0x14          */
    volatile ESR esr;                                 /*!< CAN error status register,           Address offset: 0x18          */
    volatile BTR btr;                                 /*!< CAN bit timing register,             Address offset: 0x1C          */
    uint32_t RESERVED0[88];                           /*!< Reserved, 0x020 - 0x17F                                            */
    volatile CAN_TxMailBox_TypeDef sTxMailBox[3];     /*!< CAN Tx MailBox,                      Address offset: 0x180 - 0x1AC */
    volatile CAN_FIFOMailBox_TypeDef sFIFOMailBox[2]; /*!< CAN FIFO MailBox,                    Address offset: 0x1B0 - 0x1CC */
    uint32_t RESERVED1[12];                           /*!< Reserved, 0x1D0 - 0x1FF                                            */
    volatile FMR fmr;                                 /*!< CAN filter master register,          Address offset: 0x200         */
    volatile uint32_t FM1R;                           /*!< CAN filter mode register,            Address offset: 0x204         */
    uint32_t RESERVED2;                               /*!< Reserved, 0x208                                                    */
    volatile uint32_t FS1R;                           /*!< CAN filter scale register,           Address offset: 0x20C         */
    uint32_t RESERVED3;                               /*!< Reserved, 0x210                                                    */
    volatile uint32_t FFA1R;                          /*!< CAN filter FIFO assignment register, Address offset: 0x214         */
    uint32_t RESERVED4;                               /*!< Reserved, 0x218                                                    */
    volatile uint32_t FA1R;                           /*!< CAN filter activation register,      Address offset: 0x21C         */
    uint32_t RESERVED5[8];                            /*!< Reserved, 0x220-0x23F                                              */
    CAN_FilterRegister_TypeDef sFilterRegister[28];   /*!< CAN Filter Register,                 Address offset: 0x240-0x31C   */
};
static_assert(offsetof(CAN, msr) == 4, "");
static_assert(offsetof(CAN, tsr) == 8, "");
static_assert(offsetof(CAN, rf0r) == 12, "");
static_assert(offsetof(CAN, rf1r) == 16, "");
static_assert(offsetof(CAN, ier) == 20, "");
static_assert(offsetof(CAN, esr) == 24, "");
static_assert(offsetof(CAN, btr) == 28, "");
static_assert(offsetof(CAN, sTxMailBox[0]) == 0x180, "");
static_assert(offsetof(CAN, sTxMailBox[1]) == 0x190, "");
static_assert(offsetof(CAN, sFIFOMailBox) == 0x1B0, "");
static_assert(offsetof(CAN, fmr) == 0x200, "");
static_assert(offsetof(CAN, FA1R) == 0x21C, "");

constexpr CAN::IER::Interrupt operator|(CAN::IER::Interrupt a, CAN::IER::Interrupt b) {
    return static_cast<CAN::IER::Interrupt>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

#if defined(CAN1_BASE)
extern CAN &can1;
#endif
#if defined(CAN2_BASE)
extern CAN &can2;
#endif

}  // namespace registers
}  // namespace stm32f4xx
}  // namespace microhal
#endif /* _MICROHAL_PORTS_STM32F4XX_CAN_REGISTERS_H_ */
