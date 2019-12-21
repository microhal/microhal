/*
 * dac_stm32f3xx.h
 *
 *  Created on: May 23, 2019
 *      Author: pokas
 */

#include <algorithm>
#include <cstdint>
#include "device/stm32f3xx.h"
#include "gsl/span"
#include "ports/stmCommon/clockManager/dacClock.h"

#undef DAC  // undefine DAC from device/stm32f3xx.h

namespace microhal {
namespace stm32f3xx {

/**
 * Structure that describes DAC registers memory map
 */
struct DACRegisterMap {
    /**
     * Union that describes DAC control register structure
     */
    union CR_t {
        struct Bitfields {
            uint32_t EN1 : 1;           // DAC channel1 enable
            uint32_t BOFF1_OUTEN1 : 1;  // BOFF1: DAC1 channel1 output buffer disable or OUTEN1: DAC2 channel1 output switch enable
            uint32_t TEN1 : 1;          // DAC channel1 trigger enable
            uint32_t TSEL1 : 3;         // DAC channel1 trigger selection
            uint32_t WAVE1 : 2;         // DAC channel1 noise/triangle wave generation enable
            uint32_t MAMP1 : 4;         // DAC channel1 mask/amplitude selector
            uint32_t DMAEN1 : 1;        // DAC channel1 DMA enable
            uint32_t DMAUDRIE1 : 1;     // DAC channel1 DMA underrun Interrupt enable
            uint32_t Reserved1 : 2;     // Reserved, must be kept at reset value.
            uint32_t EN2 : 1;           // DAC channel2 enable
            uint32_t OUTEN2 : 1;        // DAC channel2 output switch enable
            uint32_t TEN2 : 1;          // DAC channel2 trigger enable
            uint32_t TSEL2 : 3;         // DAC channel2 trigger selection
            uint32_t WAVE2 : 2;         // DAC channel2 noise/triangle wave generation enable
            uint32_t MAMP2 : 4;         // DAC channel2 mask/amplitude selector
            uint32_t DMAEN2 : 1;        // DAC channel2 DMA enable
            uint32_t DMAUDRIE2 : 1;     // DAC channel2 DMA underrun interrupt enable
            uint32_t Reserved2 : 2;     // Reserved, must be kept at reset value.
        };
        Bitfields bitfield;
        uint32_t raw;
    };
    static_assert(sizeof(CR_t) == sizeof(uint32_t), "Microhal internal error, incorrect DMA::CR_t size. Possible invalid compiler settings.");

    /**
     * Union that describes DAC software trigger register structure
     */
    union SWTRIGR_t {
        struct Bitfields {
            uint32_t SWTRIG1 : 1;  // DAC channel1 software trigger
            uint32_t SWTRIG2 : 1;  // DAC channel2 software trigger
        };
        Bitfields bitfield;
        uint32_t raw;
    };

    CR_t CR;            // DAC control register
    SWTRIGR_t SWTRIGR;  // DAC software trigger register
    uint32_t DHR12R1;   // DAC channel1 12-bit right-aligned data holding register
    uint32_t DHR12L1;   // DAC channel1 12-bit left-aligned data holding register
    uint32_t DHR8R1;    // DAC channel1 8-bit right-aligned data holding register
    uint32_t DHR12R2;   // DAC channel2 12-bit right-aligned data holding register
    uint32_t DHR12L2;   // DAC channel2 12-bit left-aligned data holding register
    uint32_t DHR8R2;    // DAC channel2 8-bit right-aligned data holding register
    uint32_t DHR12RD;   // Dual DAC 12-bit right-aligned data holding register
    uint32_t DHR12LD;   // Dual DAC 12-bit left-aligned data holding register
    uint32_t DHR8RD;    // Dual DAC 8-bit right-aligned data holding register
    uint32_t DOR1;      // DAC channel1 data output register
    uint32_t DOR2;      // DAC channel2 data output register
    uint32_t SR;        // DAC status registers
};

class DAC {
 public:
    template <typename T>
    using span = gsl::span<T>;  // fixme replace to std::span when it will be available
    enum class TriggerSource {
        Timer6 = 0,
        Timer3 = 1,
        Timer7,
        Timer15,
        Timer2,
        HRTIM1_DACTRG2,
        ExternalInterruptLine9,
        Software,
        HRTIM1_DACTRG1,
        HRTIM1_DACTRG3
    };
    enum class SampleSource { NoiseGenerator, TriangleGenerator, Samples };

    DAC(void *addr) : dac(*static_cast<DACRegisterMap *>(addr)) { ClockManager::enable(1); }

    /**
     * Enable DAC channel specified by channel parameter. Channels are enumerated from 0 to 1;
     *
     * @param channel number to enable
     * @return true when channel was enabled successfully, false when an error occurred or @param channel have invalid value
     */
    bool enable(uint8_t channel) {
        if (channel == 0) {
            dac.CR.bitfield.EN1 = 1;
            return true;
        } else if (channel == 1) {
            dac.CR.bitfield.EN2 = 1;
            return true;
        }
        return false;
    }
    /**
     * Disable DAC channel specified by channel parameter. Channels are enumerated form 0 to 1.
     *
     * @param channel number for disable
     * @return true when channel was disabled successfully, false when an error occurred or @param channel have invalid value.
     */
    bool disable(uint8_t channel) {
        if (channel == 0) {
            dac.CR.bitfield.EN1 = 0;
            return true;
        } else if (channel == 1) {
            dac.CR.bitfield.EN2 = 0;
            return true;
        }
        return false;
    }

    bool enableTrigger(uint8_t channel, TriggerSource source) {
        if (source == TriggerSource::Timer3) SYSCFG->CFGR1 |= SYSCFG_CFGR1_DAC1_TRIG1_RMP;
        if (channel == 0) {
            if (source == TriggerSource::HRTIM1_DACTRG3) return false;
            uint32_t tselValue = static_cast<uint32_t>(source);
            if (source == TriggerSource::Timer15) {
                SYSCFG->CFGR3 &= ~SYSCFG_CFGR3_DAC1_TRG3_RMP;
                tselValue = 0b011;
            }
            if (source == TriggerSource::HRTIM1_DACTRG1) {
                SYSCFG->CFGR3 |= SYSCFG_CFGR3_DAC1_TRG3_RMP;
                tselValue = 0b011;
            }
            if (source == TriggerSource::HRTIM1_DACTRG2) SYSCFG->CFGR3 |= SYSCFG_CFGR3_DAC1_TRG5_RMP;
            dac.CR.bitfield.TSEL1 = tselValue;
            dac.CR.bitfield.TEN1 = 1;
            return true;
        } else if (channel == 1) {
            if (source == TriggerSource::HRTIM1_DACTRG1) return false;
            if (source == TriggerSource::HRTIM1_DACTRG2) return false;
            if (source == TriggerSource::HRTIM1_DACTRG3) {
                dac.CR.bitfield.TSEL2 = 0b101;
            } else {
                dac.CR.bitfield.TSEL2 = static_cast<uint32_t>(source);
            }
            dac.CR.bitfield.TEN2 = 1;
            return true;
        }
        return false;
    }
    bool disableTrigger(uint8_t channel) {
        if (channel == 0) {
            dac.CR.bitfield.TEN1 = 0;
            return true;
        } else if (channel == 1) {
            dac.CR.bitfield.TEN2 = 0;
            return true;
        }
        return false;
    }

    /**
     * This function enables noise generation mode. DAC data are loaded from internal LFSR generator. New sample is loaded according to trigger
     * configuration. Calling this function will disable Triangle Wave generation if it was previously enabled.
     *
     * @param mask 0: Unmask bit0 of LFSR
     *             1: Unmask bits[1:0] of LFSR
     *             2: Unmask bits[2:0] of LFSR
     *             3: Unmask bits[3:0] of LFSR
     *             4: Unmask bits[4:0] of LFSR
     *             5: Unmask bits[5:0] of LFSR
     *             6: Unmask bits[6:0] of LFSR
     *             7: Unmask bits[7:0] of LFSR
     *             8: Unmask bits[8:0] of LFSR
     *             9: Unmask bits[9:0] of LFSR
     *             10: Unmask bits[10:0] of LFSR
     *             ≥11: Unmask bits[11:0] of LFSR
     *
     * @note Calling writeSamples have no effect when noise generation is enabled.
     * @return true if noise generation was enabled. false if an error occurred or mask parameter is invalid.
     */
    bool enableNoiseGeneration(uint8_t channel, uint_fast16_t mask) {
        if (channel == 0) {
            dac.CR.bitfield.WAVE1 = 0b01;
            dac.CR.bitfield.MAMP1 = mask;
            return true;
        } else if (channel == 1) {
            dac.CR.bitfield.WAVE2 = 0b01;
            dac.CR.bitfield.MAMP2 = mask;
            return true;
        }
        return false;
    }

    /**
     * This function enables triangle generation mode. DAC data are loaded from internal counter. New sample is loaded according to trigger
     * configuration
     * @param amplitude parameter have to have value from set: 1, 3, 7, 12, 31, 63, 127, 255, 511, 1023, 2047, 4095
     *
     * @note  Calling writeSamples have no effect when triangle wave generation is enabled.
     * @return true if triangle wave generation was enabled. false if an error occurred or amplitude parameter is invalid.
     */
    bool enableTriangleWaveGeneration(uint8_t channel, uint_fast16_t amplitude) {
        static std::array<uint16_t, 12> amp = {1, 3, 7, 12, 31, 63, 127, 255, 511, 1023, 2047, 4095};
        if (auto found = std::find(amp.begin(), amp.end(), amplitude); found != amp.end()) {
            auto index = std::distance(amp.begin(), found);
            if (channel == 0) {
                dac.CR.bitfield.WAVE1 = 0b10;
                dac.CR.bitfield.MAMP1 = index;
                return true;
            } else if (channel == 1) {
                dac.CR.bitfield.WAVE2 = 0b10;
                dac.CR.bitfield.MAMP2 = index;
                return true;
            }
        }
        return false;
    }

    bool disableNoiseOrTriangleGeneration(uint8_t channel) {
        if (channel == 0) {
            dac.CR.bitfield.WAVE1 = 0;
            return true;
        } else if (channel == 1) {
            dac.CR.bitfield.WAVE2 = 0;
            return true;
        }
        return false;
    }

    void softwareTrigger(uint8_t channel) {
        if (channel == 0) {
            dac.SWTRIGR.bitfield.SWTRIG1 = 1;
        } else if (channel == 1) {
            dac.SWTRIGR.bitfield.SWTRIG2 = 1;
        }
    }

    bool writeSamples(span<uint16_t> samples);
    bool writeSamples(span<uint8_t> samples);

 private:
    DACRegisterMap &dac;
};

}  // namespace stm32f3xx
}  // namespace microhal
