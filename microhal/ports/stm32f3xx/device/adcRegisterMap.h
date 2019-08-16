/*
 * adcRegisterMap.h
 *
 *  Created on: Aug 15, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STM32F3XX_DEVICE_ADCREGISTERMAP_H_
#define _MICROHAL_PORTS_STM32F3XX_DEVICE_ADCREGISTERMAP_H_

#include <cstdint>
#include "bitfield.h"

namespace microhal {
namespace stm32f3xx {
namespace registers {

struct ADC {
    union CFGR_t {
        myBitfield<uint32_t, 0, 0> DMAEN;
        myBitfield<uint32_t, 1, 1> DMACFG;
        // bit 2 is reserved
        myBitfield<uint32_t, 3, 4> RES;
        myBitfield<uint32_t, 5, 5> ALIGN;
        myBitfield<uint32_t, 6, 9> EXTSEL;
        myBitfield<uint32_t, 10, 11> EXTEN;
        myBitfield<uint32_t, 12, 12> OVRMOD;
        myBitfield<uint32_t, 13, 13> CONT;
        myBitfield<uint32_t, 14, 14> AUTDLY;
        // bit 15 is reserved
        myBitfield<uint32_t, 16, 16> DISCEN;
        myBitfield<uint32_t, 17, 19> DISCNUM;
        myBitfield<uint32_t, 20, 20> JDISCEN;
        myBitfield<uint32_t, 21, 21> JQM;
        myBitfield<uint32_t, 22, 22> AWD1SGL;
        myBitfield<uint32_t, 23, 23> AWD1EN;
        myBitfield<uint32_t, 24, 24> JAWD1EN;
        myBitfield<uint32_t, 25, 25> JAUTO;
        myBitfield<uint32_t, 26, 30> AWD1CH;
    };

    union CR_t {
        myBitfield<uint32_t, 0, 0> ADEN;
        myBitfield<uint32_t, 1, 1> ADDIS;
        myBitfield<uint32_t, 2, 2> ADSTART;
        myBitfield<uint32_t, 3, 3> JADSTART;
        myBitfield<uint32_t, 4, 4> ADSTP;
        myBitfield<uint32_t, 5, 5> JADSTP;
        // bits from 6 to 27 are reserved
        myBitfield<uint32_t, 28, 29> ADVREGEN;
        myBitfield<uint32_t, 30, 30> ADCALDIF;
        myBitfield<uint32_t, 31, 31> ADCAL;
    };

    uint32_t ISR;          /*!< ADC Interrupt and Status Register,                 Address offset: 0x00 */
    uint32_t IER;          /*!< ADC Interrupt Enable Register,                     Address offset: 0x04 */
    volatile CR_t CR;      /*!< ADC control register,                              Address offset: 0x08 */
    CFGR_t CFGR;           /*!< ADC Configuration register,                        Address offset: 0x0C */
    uint32_t RESERVED0;    /*!< Reserved, 0x010                                                         */
    uint32_t SMPR1;        /*!< ADC sample time register 1,                        Address offset: 0x14 */
    uint32_t SMPR2;        /*!< ADC sample time register 2,                        Address offset: 0x18 */
    uint32_t RESERVED1;    /*!< Reserved, 0x01C                                                         */
    uint32_t TR1;          /*!< ADC watchdog threshold register 1,                 Address offset: 0x20 */
    uint32_t TR2;          /*!< ADC watchdog threshold register 2,                 Address offset: 0x24 */
    uint32_t TR3;          /*!< ADC watchdog threshold register 3,                 Address offset: 0x28 */
    uint32_t RESERVED2;    /*!< Reserved, 0x02C                                                         */
    uint32_t SQR1;         /*!< ADC regular sequence register 1,                   Address offset: 0x30 */
    uint32_t SQR2;         /*!< ADC regular sequence register 2,                   Address offset: 0x34 */
    uint32_t SQR3;         /*!< ADC regular sequence register 3,                   Address offset: 0x38 */
    uint32_t SQR4;         /*!< ADC regular sequence register 4,                   Address offset: 0x3C */
    uint32_t DR;           /*!< ADC regular data register,                         Address offset: 0x40 */
    uint32_t RESERVED3;    /*!< Reserved, 0x044                                                         */
    uint32_t RESERVED4;    /*!< Reserved, 0x048                                                         */
    uint32_t JSQR;         /*!< ADC injected sequence register,                    Address offset: 0x4C */
    uint32_t RESERVED5[4]; /*!< Reserved, 0x050 - 0x05C                                                 */
    uint32_t OFR1;         /*!< ADC offset register 1,                             Address offset: 0x60 */
    uint32_t OFR2;         /*!< ADC offset register 2,                             Address offset: 0x64 */
    uint32_t OFR3;         /*!< ADC offset register 3,                             Address offset: 0x68 */
    uint32_t OFR4;         /*!< ADC offset register 4,                             Address offset: 0x6C */
    uint32_t RESERVED6[4]; /*!< Reserved, 0x070 - 0x07C                                                 */
    uint32_t JDR1;         /*!< ADC injected data register 1,                      Address offset: 0x80 */
    uint32_t JDR2;         /*!< ADC injected data register 2,                      Address offset: 0x84 */
    uint32_t JDR3;         /*!< ADC injected data register 3,                      Address offset: 0x88 */
    uint32_t JDR4;         /*!< ADC injected data register 4,                      Address offset: 0x8C */
    uint32_t RESERVED7[4]; /*!< Reserved, 0x090 - 0x09C                                                 */
    uint32_t AWD2CR;       /*!< ADC  Analog Watchdog 2 Configuration Register,     Address offset: 0xA0 */
    uint32_t AWD3CR;       /*!< ADC  Analog Watchdog 3 Configuration Register,     Address offset: 0xA4 */
    uint32_t RESERVED8;    /*!< Reserved, 0x0A8                                                         */
    uint32_t RESERVED9;    /*!< Reserved, 0x0AC                                                         */
    uint32_t DIFSEL;       /*!< ADC  Differential Mode Selection Register,         Address offset: 0xB0 */
    uint32_t CALFACT;      /*!< ADC  Calibration Factors,                          Address offset: 0xB4 */
};

}  // namespace registers
}  // namespace stm32f3xx
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STM32F3XX_DEVICE_ADCREGISTERMAP_H_ */
