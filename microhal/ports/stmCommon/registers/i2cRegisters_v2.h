#ifndef _MICROHAL_PERIPHERAL_REGISTER_I2C
#define _MICROHAL_PERIPHERAL_REGISTER_I2C

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32L4x6, STM32L4x3, STM32L4x1, STM32L4x2, STM32L4x5, STM32F7x9, STM32F765, STM32F7x8, STM32F745, STM32F750, STM32F7x7,
//                          STM32F7x6, STM32G474xx, STM32G484xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx,
//                          STM32L0x3, STM32L0x2, STM32L0x1, STM32F410, STM32L4R7, STM32L4S9, STM32L4R9, STM32L4S5, STM32L4S7, STM32L4R5, STM32H742x,
//                          STM32H7x3, STM32H743x, STM32H753x, STM32H7x5_CM7, STM32H7x5_CM4, STM32F412,  STM32H7x7_CM4, STM32H750x, STM32H7x7_CM7
// Supported MCU version 2: STM32F7x3, STM32F7x2, STM32F730
// Supported MCU version 3: STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0, STM32F302, STM32F303, STM32F3x4, STM32F301, STM32F3x8, STM32F373

#if defined(STM32L4x6) || defined(STM32L4x3) || defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32L4x5) || defined(STM32F7x9) ||             \
    defined(STM32F765) || defined(STM32F7x8) || defined(STM32F745) || defined(STM32F750) || defined(STM32F7x7) || defined(STM32F7x6) ||             \
    defined(STM32G474xx) || defined(STM32G484xx) || defined(STM32G471xx) || defined(STM32G431xx) || defined(STM32G473xx) || defined(STM32G441xx) || \
    defined(STM32GBK1CBT6) || defined(STM32G483xx) || defined(STM32L0x3) || defined(STM32L0x2) || defined(STM32L0x1) || defined(STM32F410) ||       \
    defined(STM32L4R7) || defined(STM32L4S9) || defined(STM32L4R9) || defined(STM32L4S5) || defined(STM32L4S7) || defined(STM32L4R5) ||             \
    defined(STM32H742x) || defined(STM32H7x3) || defined(STM32H743x) || defined(STM32H753x) || defined(STM32H7x5_CM7) || defined(STM32H7x5_CM4) ||  \
    defined(STM32F412) || defined(STM32H7x7_CM4) || defined(STM32H750x) || defined(STM32H7x7_CM7)  // version 1
#define _MICROHAL_REGISTERS_I2C_CR1_HAS_WUPEN
#endif

#if defined(STM32F0x2) || defined(STM32F0x8) || defined(STM32F0x1) || defined(STM32F0x0) || defined(STM32F302) || defined(STM32F303) || \
    defined(STM32F3x4) || defined(STM32F301) || defined(STM32F3x8) || defined(STM32F373)  // version 3
#define _MICROHAL_REGISTERS_I2C_CR1_HAS_SWRST
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct I2C {
    // Control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PE;      /*!< Peripheral enable Note: When PE=0, the I2C SCL and SDA lines are released. Internal state
                                                         * machines and status bits are put back to their reset value. When cleared, PE must be kept low
                                                         * for at least 3 APB clock cycles.
                                                         */
            microhal::Bitfield<uint32_t, 1, 1> TXIE;    /*!< TX Interrupt enable */
            microhal::Bitfield<uint32_t, 2, 1> RXIE;    /*!< RX Interrupt enable */
            microhal::Bitfield<uint32_t, 3, 1> ADDRIE;  /*!< Address match interrupt enable (slave only) */
            microhal::Bitfield<uint32_t, 4, 1> NACKIE;  /*!< Not acknowledge received interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> STOPIE;  /*!< STOP detection Interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> TCIE;    /*!< Transfer Complete interrupt enable Note: Any of these events will generate an interrupt:
                                                         * Transfer Complete (TC) Transfer Complete Reload (TCR)
                                                         */
            microhal::Bitfield<uint32_t, 7, 1> ERRIE;   /*!< Error interrupts enable Note: Any of these errors generate an interrupt: Arbitration
                                                         * Loss (ARLO) Bus Error detection (BERR) Overrun/Underrun (OVR) Timeout detection (TIMEOUT) PEC
                                                         * error detection (PECERR) Alert pin event detection (ALERT)
                                                         */
            microhal::Bitfield<uint32_t, 8, 4> DNF;     /*!< Digital noise filter These bits are used to configure the digital noise filter on SDA and
                                                         * SCL input. The digital filter will filter spikes with a length of up to
                                                         * DNF[3:0] * tI2CCLK ... Note: If the analog filter is also enabled, the digital filter is added
                                                         * to the analog filter. This filter can only be programmed when the I2C is disabled (PE = 0).
                                                         */
            microhal::Bitfield<uint32_t, 12, 1> ANFOFF; /*!< Analog noise filter OFF Note: This bit can only be programmed when
                                                         * the I2C is disabled (PE = 0). */
#ifdef _MICROHAL_REGISTERS_I2C_CR1_HAS_SWRST
            microhal::Bitfield<uint32_t, 13, 1> SWRST; /*!< Software reset */
#endif
            microhal::Bitfield<uint32_t, 14, 1> TXDMAEN;   /*!< DMA transmission requests enable */
            microhal::Bitfield<uint32_t, 15, 1> RXDMAEN;   /*!< DMA reception requests enable */
            microhal::Bitfield<uint32_t, 16, 1> SBC;       /*!< Slave byte control This bit is used to enable hardware byte control in slave mode. */
            microhal::Bitfield<uint32_t, 17, 1> NOSTRETCH; /*!< Clock stretching disable This bit is used to disable clock stretching in slave mode.
                                                            * It must be kept cleared in master mode. Note: This bit can only be programmed when the
                                                            * I2C is disabled (PE = 0).
                                                            */
#ifdef _MICROHAL_REGISTERS_I2C_CR1_HAS_WUPEN
            microhal::Bitfield<uint32_t, 18, 1>
                WUPEN; /*!< Wakeup from Stop mode enable Note: If the Wakeup from Stop mode feature is not supported, this bit is reserved and forced
                          by hardware to 0. Please refer to Section25.3: I2C implementation. Note: WUPEN can be set only when DNF = 0000 */
#endif
            microhal::Bitfield<uint32_t, 19, 1> GCEN;   /*!< General call enable */
            microhal::Bitfield<uint32_t, 20, 1> SMBHEN; /*!< SMBus Host address enable Note: If the SMBus feature is not supported, this bit is
                                                           reserved and forced by hardware to 0. Please refer to Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 21, 1>
                SMBDEN; /*!< SMBus Device Default address enable Note: If the SMBus feature is not supported, this bit is reserved and forced by
                           hardware to 0. Please refer to Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 22, 1>
                ALERTEN; /*!< SMBus alert enable Device mode (SMBHEN=0): Host mode (SMBHEN=1): Note: When ALERTEN=0, the SMBA pin can be used as a
                            standard GPIO. If the SMBus feature is not supported, this bit is reserved and forced by hardware to 0. Please refer to
                            Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 23, 1> PECEN; /*!< PEC enable Note: If the SMBus feature is not supported, this bit is reserved and forced by
                                                          hardware to 0. Please refer to Section25.3: I2C implementation. */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR1 operator&(uint32_t value) const {
            CR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR1 operator|(uint32_t value) const {
            CR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR1, AccessType::ReadOnly>;
        friend class VolatileRegister<CR1, AccessType::WriteOnly>;
        friend class VolatileRegister<CR1, AccessType::ReadWrite>;
    };

    // Control register 2
    union CR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 10> SADD;    /*!< Slave address bit (master mode) */
            microhal::Bitfield<uint32_t, 10, 1> RD_WRN;  /*!< Transfer direction (master mode) */
            microhal::Bitfield<uint32_t, 11, 1> ADD10;   /*!< 10-bit addressing mode (master mode) */
            microhal::Bitfield<uint32_t, 12, 1> HEAD10R; /*!< 10-bit address header only read direction (master receiver mode) */
            microhal::Bitfield<uint32_t, 13, 1> START;   /*!< Start generation */
            microhal::Bitfield<uint32_t, 14, 1> STOP;    /*!< Stop generation (master mode) */
            microhal::Bitfield<uint32_t, 15, 1> NACK;    /*!< NACK generation (slave mode) */
            microhal::Bitfield<uint32_t, 16, 8> NBYTES;  /*!< Number of bytes */
            microhal::Bitfield<uint32_t, 24, 1> RELOAD;  /*!< NBYTES reload mode */
            microhal::Bitfield<uint32_t, 25, 1> AUTOEND; /*!< Automatic end mode (master mode) */
            microhal::Bitfield<uint32_t, 26, 1> PECBYTE; /*!< Packet error checking byte */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR2 operator&(uint32_t value) const {
            CR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR2 operator|(uint32_t value) const {
            CR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR2, AccessType::ReadOnly>;
        friend class VolatileRegister<CR2, AccessType::WriteOnly>;
        friend class VolatileRegister<CR2, AccessType::ReadWrite>;
    };

    // Own address register 1
    union OAR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 10>
                OA1; /*!< Interface address 7-bit addressing mode: dont care 10-bit addressing mode: bits 9:8 of address Note: These bits can be
                        written only when OA1EN=0. OA1[7:1]: Interface address Bits 7:1 of address Note: These bits can be written only when OA1EN=0.
                        OA1[0]: Interface address 7-bit addressing mode: dont care 10-bit addressing mode: bit 0 of address Note: This bit can be
                        written only when OA1EN=0. */
            microhal::Bitfield<uint32_t, 10, 1> OA1MODE; /*!< Own Address 1 10-bit mode Note: This bit can be written only when OA1EN=0. */
            microhal::Bitfield<uint32_t, 15, 1> OA1EN;   /*!< Own Address 1 enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OAR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OAR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OAR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OAR1 operator&(uint32_t value) const {
            OAR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OAR1 operator|(uint32_t value) const {
            OAR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OAR1, AccessType::ReadOnly>;
        friend class VolatileRegister<OAR1, AccessType::WriteOnly>;
        friend class VolatileRegister<OAR1, AccessType::ReadWrite>;
    };

    // Own address register 2
    union OAR2 {
        union {
            microhal::Bitfield<uint32_t, 1, 7> OA2; /*!< Interface address bits 7:1 of address Note: These bits can be written only when OA2EN=0. */
            microhal::Bitfield<uint32_t, 8, 3>
                OA2MSK; /*!< Own Address 2 masks Note: These bits can be written only when OA2EN=0. As soon as OA2MSK is not equal to 0, the reserved
                           I2C addresses (0b0000xxx and 0b1111xxx) are not acknowledged even if the comparison matches. */
            microhal::Bitfield<uint32_t, 15, 1> OA2EN; /*!< Own Address 2 enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OAR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OAR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OAR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OAR2 operator&(uint32_t value) const {
            OAR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OAR2 operator|(uint32_t value) const {
            OAR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OAR2, AccessType::ReadOnly>;
        friend class VolatileRegister<OAR2, AccessType::WriteOnly>;
        friend class VolatileRegister<OAR2, AccessType::ReadWrite>;
    };

    // Timing register
    union TIMINGR {
        union {
            microhal::Bitfield<uint32_t, 0, 8>
                SCLL; /*!< SCL low period (master mode) This field is used to generate the SCL low period in master mode. tSCLL = (SCLL+1) x tPRESC
                         Note: SCLL is also used to generate tBUF and tSU:STA timings. */
            microhal::Bitfield<uint32_t, 8, 8>
                SCLH; /*!< SCL high period (master mode) This field is used to generate the SCL high period in master mode. tSCLH = (SCLH+1) x tPRESC
                         Note: SCLH is also used to generate tSU:STO and tHD:STA timing. */
            microhal::Bitfield<uint32_t, 16, 4>
                SDADEL; /*!< Data hold time This field is used to generate the delay tSDADEL between SCL falling edge and SDA edge. In master mode and
                           in slave mode with NOSTRETCH = 0, the SCL line is stretched low during tSDADEL. tSDADEL= SDADEL x tPRESC Note: SDADEL is
                           used to generate tHD:DAT timing. */
            microhal::Bitfield<uint32_t, 20, 4>
                SCLDEL; /*!< Data setup time This field is used to generate a delay tSCLDEL between SDA edge and SCL rising edge. In master mode and
                           in slave mode with NOSTRETCH = 0, the SCL line is stretched low during tSCLDEL. tSCLDEL = (SCLDEL+1) x tPRESC Note: tSCLDEL
                           is used to generate tSU:DAT timing. */
            microhal::Bitfield<uint32_t, 28, 4>
                PRESC; /*!< Timing prescaler This field is used to prescale I2CCLK in order to generate the clock period tPRESC used for data setup
                          and hold counters (refer to I2C timings on page9) and for SCL high and low level counters (refer to I2C master
                          initialization on page24). tPRESC = (PRESC+1) x tI2CCLK */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TIMINGR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TIMINGR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TIMINGR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TIMINGR operator&(uint32_t value) const {
            TIMINGR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TIMINGR operator|(uint32_t value) const {
            TIMINGR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TIMINGR, AccessType::ReadOnly>;
        friend class VolatileRegister<TIMINGR, AccessType::WriteOnly>;
        friend class VolatileRegister<TIMINGR, AccessType::ReadWrite>;
    };

    // Timeout register
    union TIMEOUTR {
        union {
            microhal::Bitfield<uint32_t, 0, 12>
                TIMEOUTA; /*!< Bus Timeout A This field is used to configure: The SCL low timeout condition tTIMEOUT when TIDLE=0 tTIMEOUT=
                             (TIMEOUTA+1) x 2048 x tI2CCLK The bus idle condition (both SCL and SDA high) when TIDLE=1 tIDLE= (TIMEOUTA+1) x 4 x
                             tI2CCLK Note: These bits can be written only when TIMOUTEN=0. */
            microhal::Bitfield<uint32_t, 12, 1> TIDLE;    /*!< Idle clock timeout detection Note: This bit can be written only when TIMOUTEN=0. */
            microhal::Bitfield<uint32_t, 15, 1> TIMOUTEN; /*!< Clock timeout enable */
            microhal::Bitfield<uint32_t, 16, 12>
                TIMEOUTB; /*!< Bus timeout B This field is used to configure the cumulative clock extension timeout: In master mode, the master
                             cumulative clock low extend time (tLOW:MEXT) is detected In slave mode, the slave cumulative clock low extend time
                             (tLOW:SEXT) is detected tLOW:EXT= (TIMEOUTB+1) x 2048 x tI2CCLK Note: These bits can be written only when TEXTEN=0. */
            microhal::Bitfield<uint32_t, 31, 1> TEXTEN; /*!< Extended clock timeout enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TIMEOUTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TIMEOUTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TIMEOUTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TIMEOUTR operator&(uint32_t value) const {
            TIMEOUTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TIMEOUTR operator|(uint32_t value) const {
            TIMEOUTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TIMEOUTR, AccessType::ReadOnly>;
        friend class VolatileRegister<TIMEOUTR, AccessType::WriteOnly>;
        friend class VolatileRegister<TIMEOUTR, AccessType::ReadWrite>;
    };

    // Interrupt and Status register
    union ISR {
        union {
            microhal::Bitfield<uint32_t, 0, 1>
                TXE; /*!< Transmit data register empty (transmitters) This bit is set by hardware when the I2C_TXDR register is empty. It is cleared
                        when the next data to be sent is written in the I2C_TXDR register. This bit can be written to 1 by software in order to flush
                        the transmit data register I2C_TXDR. Note: This bit is set by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 1, 1>
                TXIS; /*!< Transmit interrupt status (transmitters) This bit is set by hardware when the I2C_TXDR register is empty and the data to be
                         transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be sent is written in the I2C_TXDR
                         register. This bit can be written to 1 by software when NOSTRETCH=1 only, in order to generate a TXIS event (interrupt if
                         TXIE=1 or DMA request if TXDMAEN=1). Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 2, 1>
                RXNE; /*!< Receive data register not empty (receivers) This bit is set by hardware when the received data is copied into the I2C_RXDR
                         register, and is ready to be read. It is cleared when I2C_RXDR is read. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 3, 1>
                ADDR; /*!< Address matched (slave mode) This bit is set by hardware as soon as the received slave address matched with one of the
                         enabled slave addresses. It is cleared by software by setting ADDRCF bit. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 4, 1>
                NACKF; /*!< Not Acknowledge received flag This flag is set by hardware when a NACK is received after a byte transmission. It is
                          cleared by software by setting the NACKCF bit. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 5, 1>
                STOPF; /*!< Stop detection flag This flag is set by hardware when a Stop condition is detected on the bus and the peripheral is
                          involved in this transfer: either as a master, provided that the STOP condition is generated by the peripheral. or as a
                          slave, provided that the peripheral has been addressed previously during this transfer. It is cleared by software by setting
                          the STOPCF bit. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 6, 1>
                TC; /*!< Transfer Complete (master mode) This flag is set by hardware when RELOAD=0, AUTOEND=0 and NBYTES data have been transferred.
                       It is cleared by software when START bit or STOP bit is set. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 7, 1>
                TCR; /*!< Transfer Complete Reload This flag is set by hardware when RELOAD=1 and NBYTES data have been transferred. It is cleared by
                        software when NBYTES is written to a non-zero value. Note: This bit is cleared by hardware when PE=0. This flag is only for
                        master mode, or for slave mode when the SBC bit is set. */
            microhal::Bitfield<uint32_t, 8, 1>
                BERR; /*!< Bus error This flag is set by hardware when a misplaced Start or Stop condition is detected whereas the peripheral is
                         involved in the transfer. The flag is not set during the address phase in slave mode. It is cleared by software by setting
                         BERRCF bit. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 9, 1> ARLO; /*!< Arbitration lost This flag is set by hardware in case of arbitration loss. It is cleared by
                                                        software by setting the ARLOCF bit. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 10, 1>
                OVR; /*!< Overrun/Underrun (slave mode) This flag is set by hardware in slave mode with NOSTRETCH=1, when an overrun/underrun error
                        occurs. It is cleared by software by setting the OVRCF bit. Note: This bit is cleared by hardware when PE=0. */
            microhal::Bitfield<uint32_t, 11, 1>
                PECERR; /*!< PEC Error in reception This flag is set by hardware when the received PEC does not match with the PEC register content. A
                           NACK is automatically sent after the wrong PEC reception. It is cleared by software by setting the PECCF bit. Note: This
                           bit is cleared by hardware when PE=0. If the SMBus feature is not supported, this bit is reserved and forced by hardware to
                           0. Please refer to Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 12, 1>
                TIMEOUT; /*!< Timeout or tLOW detection flag This flag is set by hardware when a timeout or extended clock timeout occurred. It is
                            cleared by software by setting the TIMEOUTCF bit. Note: This bit is cleared by hardware when PE=0. If the SMBus feature is
                            not supported, this bit is reserved and forced by hardware to 0. Please refer to Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 13, 1>
                ALERT; /*!< SMBus alert This flag is set by hardware when SMBHEN=1 (SMBus host configuration), ALERTEN=1 and a SMBALERT event (falling
                          edge) is detected on SMBA pin. It is cleared by software by setting the ALERTCF bit. Note: This bit is cleared by hardware
                          when PE=0. If the SMBus feature is not supported, this bit is reserved and forced by hardware to 0. Please refer to
                          Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 15, 1>
                BUSY; /*!< Bus busy This flag indicates that a communication is in progress on the bus. It is set by hardware when a START condition
                         is detected. It is cleared by hardware when a Stop condition is detected, or when PE=0. */
            microhal::Bitfield<uint32_t, 16, 1>
                DIR; /*!< Transfer direction (Slave mode) This flag is updated when an address match event occurs (ADDR=1). */
            microhal::Bitfield<uint32_t, 17, 7>
                ADDCODE; /*!< Address match code (Slave mode) These bits are updated with the received address when an address match event occurs
                            (ADDR = 1). In the case of a 10-bit address, ADDCODE provides the 10-bit header followed by the 2 MSBs of the address. */
        };

        enum Transfer { Write = 0, Read = 1 };

        uint8_t slaveAddress() const { return ADDCODE; }           //!< Only in slave mode
        Transfer direction() const { return DIR ? Read : Write; }  //!< Only in slave mode
        bool busIsBusy() const { return BUSY; }
        bool timeout() const { return TIMEOUT; }
        bool overrunOrUnderrunOccured() const { return OVR; }  //!< Only in slave mode
        bool arbitrationLost() const { return ARLO; }
        bool busErrorDetected() const { return BERR; }
        bool transferCompleteReload() const { return TCR; }
        bool transferComplete() const { return TC; }
        bool stopDetected() const { return STOPF; }
        bool nackReceived() const { return NACKF; }
        bool slaveAddressMatched() const { return ADDR; }
        bool receiveRegisterNotEmpty() const { return RXNE; }
        bool transmitInterrupsStatus() const { return TXIS; }
        bool transmitRegisterEmpty() const { return TXE; }

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ISR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ISR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ISR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ISR operator&(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ISR operator|(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ISR, AccessType::ReadOnly>;
        friend class VolatileRegister<ISR, AccessType::WriteOnly>;
        friend class VolatileRegister<ISR, AccessType::ReadWrite>;
    };

    // Interrupt clear register
    union ICR {
        union {
            microhal::Bitfield<uint32_t, 3, 1> ADDRCF; /*!< Address matched flag clear Writing 1 to this bit clears the ADDR flag in the I2C_ISR
                                                          register. Writing 1 to this bit also clears the START bit in the I2C_CR2 register. */
            microhal::Bitfield<uint32_t, 4, 1>
                NACKCF; /*!< Not Acknowledge flag clear Writing 1 to this bit clears the ACKF flag in I2C_ISR register. */
            microhal::Bitfield<uint32_t, 5, 1>
                STOPCF; /*!< Stop detection flag clear Writing 1 to this bit clears the STOPF flag in the I2C_ISR register. */
            microhal::Bitfield<uint32_t, 8, 1>
                BERRCF; /*!< Bus error flag clear Writing 1 to this bit clears the BERRF flag in the I2C_ISR register. */
            microhal::Bitfield<uint32_t, 9, 1>
                ARLOCF; /*!< Arbitration Lost flag clear Writing 1 to this bit clears the ARLO flag in the I2C_ISR register. */
            microhal::Bitfield<uint32_t, 10, 1>
                OVRCF; /*!< Overrun/Underrun flag clear Writing 1 to this bit clears the OVR flag in the I2C_ISR register. */
            microhal::Bitfield<uint32_t, 11, 1>
                PECCF; /*!< PEC Error flag clear Writing 1 to this bit clears the PECERR flag in the I2C_ISR register. Note: If the SMBus feature is
                          not supported, this bit is reserved and forced by hardware to 0. Please refer to Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 12, 1> TIMOUTCF; /*!< Timeout detection flag clear Writing 1 to this bit clears the TIMEOUT flag in the
                                                             I2C_ISR register. Note: If the SMBus feature is not supported, this bit is reserved and
                                                             forced by hardware to 0. Please refer to Section25.3: I2C implementation. */
            microhal::Bitfield<uint32_t, 13, 1>
                ALERTCF; /*!< Alert flag clear Writing 1 to this bit clears the ALERT flag in the I2C_ISR register. Note: If the SMBus feature is not
                            supported, this bit is reserved and forced by hardware to 0. Please refer to Section25.3: I2C implementation. */
        };

        void clearAlertFlag() { ALERTCF.set(); }
        void clearTimeoutFlag() { TIMOUTCF.set(); }
        void clearArbitrationLostFlag() { ARLOCF.set(); }
        void clearBusErrorFlag() { BERRCF.set(); }
        void clearStopFlag() { STOPCF.set(); }
        void clearNackFlag() { NACKCF.set(); }
        void clearAddressMatchedFlag() { ADDRCF.set(); }

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ICR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ICR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ICR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ICR operator&(uint32_t value) const {
            ICR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ICR operator|(uint32_t value) const {
            ICR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ICR, AccessType::ReadOnly>;
        friend class VolatileRegister<ICR, AccessType::WriteOnly>;
        friend class VolatileRegister<ICR, AccessType::ReadWrite>;
    };

    // PEC register
    union PECR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> PEC; /*!< Packet error checking register This field contains the internal PEC when PECEN=1. The PEC is
                                                       cleared by hardware when PE=0. */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PECR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PECR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PECR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PECR operator&(uint32_t value) const {
            PECR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PECR operator|(uint32_t value) const {
            PECR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PECR, AccessType::ReadOnly>;
        friend class VolatileRegister<PECR, AccessType::WriteOnly>;
        friend class VolatileRegister<PECR, AccessType::ReadWrite>;
    };

    // Receive data register
    union RXDR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> RXDATA; /*!< 8-bit receive data */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RXDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RXDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RXDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RXDR operator&(uint32_t value) const {
            RXDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RXDR operator|(uint32_t value) const {
            RXDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RXDR, AccessType::ReadOnly>;
        friend class VolatileRegister<RXDR, AccessType::WriteOnly>;
        friend class VolatileRegister<RXDR, AccessType::ReadWrite>;
    };

    // Transmit data register
    union TXDR {
        union {
            microhal::Bitfield<uint32_t, 0, 8>
                TXDATA; /*!< 8-bit transmit data Data byte to be transmitted to the I2C bus. Note: These bits can be written only when TXE=1. */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TXDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TXDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TXDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TXDR operator&(uint32_t value) const {
            TXDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TXDR operator|(uint32_t value) const {
            TXDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TXDR, AccessType::ReadOnly>;
        friend class VolatileRegister<TXDR, AccessType::WriteOnly>;
        friend class VolatileRegister<TXDR, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;           /*!< Control register 1	Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;           /*!< Control register 2	Address offset: 0x4 */
    VolatileRegister<OAR1, AccessType::ReadWrite> oar1;         /*!< Own address register 1	Address offset: 0x8 */
    VolatileRegister<OAR2, AccessType::ReadWrite> oar2;         /*!< Own address register 2	Address offset: 0xC */
    VolatileRegister<TIMINGR, AccessType::ReadWrite> timingr;   /*!< Timing register	Address offset: 0x10 */
    VolatileRegister<TIMEOUTR, AccessType::ReadWrite> timeoutr; /*!< Timeout register	Address offset: 0x14 */
    VolatileRegister<ISR, AccessType::ReadWrite> isr;           /*!< Interrupt and Status register	Address offset: 0x18 */
    VolatileRegister<ICR, AccessType::ReadWrite> icr;           /*!< Interrupt clear register	Address offset: 0x1C */
    VolatileRegister<PECR, AccessType::ReadWrite> pecr;         /*!< PEC register	Address offset: 0x20 */
    VolatileRegister<RXDR, AccessType::ReadWrite> rxdr;         /*!< Receive data register	Address offset: 0x24 */
    VolatileRegister<TXDR, AccessType::ReadWrite> txdr;         /*!< Transmit data register	Address offset: 0x28 */
};

#if defined(_MICROHAL_I2C1_BASE_ADDRESS)
[[maybe_unused]] static I2C *const i2c1 = reinterpret_cast<I2C *>(_MICROHAL_I2C1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_I2C2_BASE_ADDRESS)
[[maybe_unused]] static I2C *const i2c2 = reinterpret_cast<I2C *>(_MICROHAL_I2C2_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_I2C3_BASE_ADDRESS)
[[maybe_unused]] static I2C *const i2c3 = reinterpret_cast<I2C *>(_MICROHAL_I2C3_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_I2C
