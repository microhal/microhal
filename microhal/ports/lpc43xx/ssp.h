/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 23-03-2019
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
#ifndef _MICROHAL_SSP_H_
#define _MICROHAL_SSP_H_

#include <cmath>
#include <cstdint>
#include <exception>
#include "IOPin.h"
#include "interfaces/spi_interface.h"

namespace microhal {
namespace lpc43xx {

class SSP final {
    typedef union {
        struct {
            uint32_t dss : 4;
            uint32_t frf : 2;
            uint32_t cpol : 1;
            uint32_t cpha : 1;
            uint32_t scr : 8;
        } bitfields;
        uint32_t raw;
    } CR0;

    typedef union {
        struct {
            uint32_t lbm : 1;
            uint32_t sse : 1;
            uint32_t ms : 1;
            uint32_t sod : 1;
        } bitfields;
        uint32_t raw;
    } CR1;

    typedef union {
        struct {
            uint32_t tfe : 1;  //!< Transmit FIFO Empty. This bit is 1 is the Transmit FIFO is empty, 0 if not.
            uint32_t tnf : 1;  //!< Transmit FIFO Not Full. This bit is 0 if the Tx FIFO is full, 1 if not.
            uint32_t rne : 1;  //!< Receive FIFO Not Empty. This bit is 0 if the Receive FIFO is empty, 1 if not.
            uint32_t rff : 1;  //!< Receive FIFO Full. This bit is 1 if the Receive FIFO is full, 0 if not.
            uint32_t bsy : 1; /*!< Busy. This bit is 0 if the SSPn controller is idle, or 1 if it is currently sending/receiving a frame and/or the Tx
                                 FIFO is not empty.*/
        } bitfields;
        uint32_t raw;
    } SR;

 public:
    enum class FrameFormat { SPI = 0, TI, Microwire };
    using Mode = SPI::Mode;

    void setFrameFormat(FrameFormat frameFormat) { cr0.bitfields.frf = static_cast<uint32_t>(frameFormat); }
    void setSPIMode(Mode mode) {
        switch (mode) {
            case Mode::Mode0:
                cr0.bitfields.cpol = 0;
                cr0.bitfields.cpha = 0;
                break;
            case Mode::Mode1:
                cr0.bitfields.cpol = 0;
                cr0.bitfields.cpha = 1;
                break;
            case Mode::Mode2:
                cr0.bitfields.cpol = 1;
                cr0.bitfields.cpha = 0;
                break;
            case Mode::Mode3:
                cr0.bitfields.cpol = 1;
                cr0.bitfields.cpha = 1;
                break;
        }
    }
    void setDataSize(uint_fast8_t dataSize) {
        if (dataSize >= 4 && dataSize <= 16) {
            cr0.bitfields.dss = dataSize - 1;
        } else {
            // unsupported data size
            std::terminate();
        }
    }
    void enable() { cr1.bitfields.sse = 1; }
    void disable() { cr1.bitfields.sse = 0; }
    bool isEnabled() { return cr1.bitfields.sse; }
    void configureAsMaster() { cr1.bitfields.ms = 0; }
    void configureAsSlave(bool disableOutput) {
        cr1.bitfields.ms = 1;
        cr1.bitfields.sod = disableOutput;
    }
    void writeData(uint16_t data) { dr = data; }
    uint16_t readData() { return dr; }

    bool isTxFifoEmpty() { return sr.bitfields.tfe; }
    bool isTxFifoNotFull() { return sr.bitfields.tnf; }
    bool isRxFifoNotEmpty() { return sr.bitfields.rne; }
    bool isRxFifoFull() { return sr.bitfields.rff; }
    bool isBusy() { return sr.bitfields.bsy; }

    uint32_t baudrate() {
        uint_fast8_t cpsdvsr = cpsr & 0x000000FF;
        uint32_t pclk = 96000000;
        return pclk / (cpsdvsr + (cr0.bitfields.scr + 1));
    }

    void baudrate(uint32_t baud) {
        uint32_t pclk = 96000000;
        int32_t x = pclk / (2 * baud);
        uint32_t bestDiff = 0xFFFF'FFFF;
        uint32_t bestScr;
        uint32_t bestSpsr_half;
        for (int_fast16_t spsr_half = 1; spsr_half < 128; spsr_half++) {
            for (int_fast16_t scr = 1; scr <= 256; scr++) {
                uint32_t diff = std::abs(x - (spsr_half * scr));
                if (diff < bestDiff) {
                    bestDiff = diff;
                    bestScr = scr;
                    bestSpsr_half = spsr_half;
                }
            }
        }
        cpsr = bestSpsr_half * 2;
        cr0.bitfields.scr = bestScr - 1;
    }

 private:
    volatile CR0 cr0;        /*!< Control Register 0. Selects the serial clock rate, bus type, and data size. */
    volatile CR1 cr1;        /*!< Control Register 1. Selects master/slave and other modes. */
    volatile uint32_t dr;    /*!< Data Register. Writes fill the transmit FIFO, and reads empty the receive FIFO. */
    volatile SR sr;          /*!< Status Register        */
    volatile uint32_t cpsr;  /*!< Clock Prescale Register */
    volatile uint32_t IMSC;  /*!< Interrupt Mask Set and Clear Register */
    volatile uint32_t RIS;   /*!< Raw Interrupt Status Register */
    volatile uint32_t MIS;   /*!< Masked Interrupt Status Register */
    volatile uint32_t ICR;   /*!< SSPICR Interrupt Clear Register */
    volatile uint32_t DMACR; /*!< SSPn DMA control register */
};
}  // namespace lpc43xx
}  // namespace microhal

#endif  // _MICROHAL_SSP_H_
