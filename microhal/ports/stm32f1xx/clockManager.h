/*
 * clockManager.h
 *
 *  Created on: Nov 12, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STM32F1XX_CLOCKMANAGER_H_
#define SRC_MICROHAL_PORTS_STM32F1XX_CLOCKMANAGER_H_

#include <cstdint>
#include <exception>
#include "gpio_stm32f1xx.h"
#include "rcc_stm32f103.h"

namespace microhal {
namespace stm32f1xx {

class ClockManager {
 public:
    using Frequency = uint32_t;

    static void enableGPIO(registers::GPIO *gpio) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        if ((int)gpio == IOPin::PortA) {
            apb2enr.IOPAEN.set();
        } else if ((int)gpio == IOPin::PortB) {
            apb2enr.IOPBEN.set();
        } else if ((int)gpio == IOPin::PortC) {
            apb2enr.IOPCEN.set();
        } else if ((int)gpio == IOPin::PortD) {
            apb2enr.IOPDEN.set();
        } else if ((int)gpio == IOPin::PortE) {
            apb2enr.IOPEEN.set();
        } else if ((int)gpio == IOPin::PortF) {
            apb2enr.IOPFEN.set();
        } else if ((int)gpio == IOPin::PortG) {
            apb2enr.IOPGEN.set();
        } else {
            std::terminate();
        }
        registers::rcc->apb2enr.volatileStore(apb2enr);
    }
    static void disableGPIO(registers::GPIO *gpio) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        if ((int)gpio == IOPin::PortA) {
            apb2enr.IOPAEN.clear();
        } else if ((int)gpio == IOPin::PortB) {
            apb2enr.IOPBEN.clear();
        } else if ((int)gpio == IOPin::PortC) {
            apb2enr.IOPCEN.clear();
        } else if ((int)gpio == IOPin::PortD) {
            apb2enr.IOPDEN.clear();
        } else if ((int)gpio == IOPin::PortE) {
            apb2enr.IOPEEN.clear();
        } else if ((int)gpio == IOPin::PortF) {
            apb2enr.IOPFEN.clear();
        } else if ((int)gpio == IOPin::PortG) {
            apb2enr.IOPGEN.clear();
        } else {
            std::terminate();
        }
        registers::rcc->apb2enr.volatileStore(apb2enr);
    }

    static void enableUSART(uint8_t number) {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
            case 1: {
                auto apb2enr = registers::rcc->apb2enr.volatileLoad();
                apb2enr.USART1EN.set();
                registers::rcc->apb2enr.volatileStore(apb2enr);
            } break;
            case 2:
                apb1enr.USART2EN.set();
                break;
            case 3:
                apb1enr.USART3EN.set();
                break;
            case 4:
                apb1enr.UART4EN.set();
                break;
            case 5:
                apb1enr.UART5EN.set();
                break;
            default:
                std::terminate();
        }

        registers::rcc->apb1enr.volatileStore(apb1enr);
    }

    static void enableSPI(uint8_t number) {
        if (number == 1) {
            auto apb2enr = registers::rcc->apb2enr.volatileLoad();
            apb2enr.SPI1EN.set();
            registers::rcc->apb2enr.volatileStore(apb2enr);
        } else {
            auto apb1enr = registers::rcc->apb1enr.volatileLoad();
            switch (number) {
                case 2:
                    apb1enr.SPI2EN.set();
                    break;
                case 3:
                    apb1enr.SPI3EN.set();
                    break;
            }
            registers::rcc->apb1enr.volatileStore(apb1enr);
        }
    }

    static void enableTimer(uint8_t number) {
        if (number == 1 || (number >= 8 && number <= 11)) {
            auto apb2enr = registers::rcc->apb2enr.volatileLoad();
            switch (number) {
                case 1:
                    apb2enr.TIM1EN.set();
                    break;
                case 8:
                    apb2enr.TIM8EN.set();
                    break;
                case 9:
                    apb2enr.TIM9EN.set();
                    break;
                case 10:
                    apb2enr.TIM10EN.set();
                    break;
                case 11:
                    apb2enr.TIM11EN.set();
                    break;
            }
            registers::rcc->apb2enr.volatileStore(apb2enr);
        } else {
            auto apb1enr = registers::rcc->apb1enr.volatileLoad();
            switch (number) {
                case 2:
                    apb1enr.TIM2EN.set();
                    break;
                case 3:
                    apb1enr.TIM3EN.set();
                    break;
                case 4:
                    apb1enr.TIM4EN.set();
                    break;
                case 5:
                    apb1enr.TIM5EN.set();
                    break;
                case 6:
                    apb1enr.TIM6EN.set();
                    break;
                case 7:
                    apb1enr.TIM7EN.set();
                    break;
                case 12:
                    apb1enr.TIM12EN.set();
                    break;
                case 13:
                    apb1enr.TIM13EN.set();
                    break;
                case 14:
                    apb1enr.TIM14EN.set();
                    break;
            }
            registers::rcc->apb1enr.volatileStore(apb1enr);
        }
    }

    static uint32_t TimerFrequency(uint8_t number) {
        if (number == 1 || (number >= 8 && number <= 11)) {
            return apb2Frequency();
        }
        return apb1Frequency();
    }

    static uint32_t USARTFrequency(uint8_t number) {
        if (number == 1) {
            return apb2Frequency();
        } else {
            return apb1Frequency();
        }
    }

    static uint32_t SPIFrequency(uint8_t number) {
        if (number == 1) {
            return apb2Frequency();
        }
        return apb1Frequency();
    }

    static uint32_t apb1Frequency() {
        uint32_t ppre1 = registers::rcc->cfgr.volatileLoad().PPRE1;
        if (ppre1 & 0b100) {
            static constexpr const uint8_t dividers[] = {2, 4, 8, 16};
            return ahbFrequency() / dividers[ppre1 & 0b11];
        }
        return ahbFrequency();
    }
    static uint32_t apb2Frequency() {
        uint32_t ppre2 = registers::rcc->cfgr.volatileLoad().PPRE2;
        if (ppre2 & 0b100) {
            static constexpr const uint8_t dividers[] = {2, 4, 8, 16};
            return ahbFrequency() / dividers[ppre2 & 0b11];
        }
        return ahbFrequency();
    }

    static uint32_t ahbFrequency() {
        uint32_t hpre = registers::rcc->cfgr.volatileLoad().HPRE;
        if (hpre & 0b1000) {
            static constexpr const uint16_t dividers[] = {2, 4, 8, 16, 64, 128, 256, 512};
            return sysclkFrequency() / dividers[hpre & 0b111];
        }
        return sysclkFrequency();
    }

    static uint32_t sysclkFrequency() { return 8'000'000; }
};
}  // namespace stm32f1xx
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STM32F1XX_CLOCKMANAGER_H_ */
