
#ifndef _MICROHAL_PORTS_IOMANAGER_STM32F334_H_
#define _MICROHAL_PORTS_IOMANAGER_STM32F334_H_

#include "../gpio_stm32f3xx.h"
#include "ports/stmCommon/ioManager/ioPinOrUnused.h"
#include "ports/stmCommon/ioManager/pinOption.h"

namespace microhal {
namespace stm32f3xx {

class IOManager {
    using AlternateFunction = GPIO::AlternateFunction;

    IOManager() = delete;
    IOManager(const IOManager&) = delete;
    IOManager(const IOManager&&) = delete;
    void operator=(const IOManager&) = delete;
    void operator=(const IOManager&&) = delete;
    ~IOManager() = delete;

    constexpr static GPIO::AlternateFunction get_alternate_function(Peripheral peripheral, PinFunction pin_function, IOPin pin) {
        using AF = GPIO::AlternateFunction;
        const std::array<const PinOption, 180> pin_options = {
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::TIM2, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::TIM2, PinFunction::Timer_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::SerialPort2, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::ADC1, PinFunction::ADC_Channel1, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::TIM2, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::SerialPort2, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::ADC1, PinFunction::ADC_Channel2, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM2, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::ADC1, PinFunction::ADC_Channel3, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM15, PinFunction::Timer_CH2, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::ADC1, PinFunction::ADC_Channel4, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SerialPort2, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::ADC2, PinFunction::ADC_Channel1, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::DAC1, PinFunction::DAC_OUT1, AF::DAC},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::TIM2, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::TIM2, PinFunction::Timer_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::ADC2, PinFunction::ADC_Channel2, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::DAC1, PinFunction::DAC_OUT2, AF::DAC},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::ADC2, PinFunction::ADC_Channel3, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::DAC2, PinFunction::DAC_OUT1, AF::DAC},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::ADC2, PinFunction::ADC_Channel4, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::TIM1, PinFunction::Timer_CH1, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::SerialPort1, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::HRTIM1, PinFunction::Timer_CHA1, AF::AF13},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM1, PinFunction::Timer_CH2, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM15, PinFunction::Timer_BKIN, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM2, PinFunction::Timer_CH3, AF::AF10},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::HRTIM1, PinFunction::Timer_CHA2, AF::AF13},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM17, PinFunction::Timer_BKIN, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF10},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::HRTIM1, PinFunction::Timer_CHB1, AF::AF13},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::SerialPort1, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::CAN1, PinFunction::CAN_RX, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::TIM1, PinFunction::Timer_CH4, AF::AF11},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::TIM1, PinFunction::Timer_BKIN2, AF::AF12},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::HRTIM1, PinFunction::Timer_CHB2, AF::AF13},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::SerialPort1, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::CAN1, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::TIM1, PinFunction::Timer_ETR, AF::AF11},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::HRTIM1, PinFunction::Timer_FLT1, AF::AF13},
            PinOption{IOPin{IOPin::PortA, 13}, Peripheral::TIM16, PinFunction::Timer_CH1N, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 13}, Peripheral::SerialPort3, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 14}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 14}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 14}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::TIM2, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::TIM2, PinFunction::Timer_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::HRTIM1, PinFunction::Timer_FLT2, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM3, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::HRTIM1, PinFunction::Timer_SCOUT, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::ADC1, PinFunction::ADC_Channel12, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 2}, Peripheral::HRTIM1, PinFunction::Timer_SCIN, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 2}, Peripheral::ADC2, PinFunction::ADC_Channel12, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::TIM2, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::TIM3, PinFunction::Timer_ETR, AF::AF10},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::HRTIM1, PinFunction::Timer_SCOUT, AF::AF12},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::HRTIM1, PinFunction::Timer_EEV9, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::TIM17, PinFunction::Timer_BKIN, AF::AF10},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::HRTIM1, PinFunction::Timer_EEV7, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM16, PinFunction::Timer_BKIN, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::I2C1, PinFunction::I2C_SMBA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::SerialPort2, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF10},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::HRTIM1, PinFunction::Timer_EEV6, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::TIM16, PinFunction::Timer_CH1N, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::HRTIM1, PinFunction::Timer_SCIN, AF::AF12},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::HRTIM1, PinFunction::Timer_EEV4, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::TIM17, PinFunction::Timer_CH1N, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF10},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::HRTIM1, PinFunction::Timer_EEV3, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::CAN1, PinFunction::CAN_RX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF12},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::HRTIM1, PinFunction::Timer_EEV8, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::CAN1, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::HRTIM1, PinFunction::Timer_EEV5, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::TIM2, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::HRTIM1, PinFunction::Timer_FLT3, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::HRTIM1, PinFunction::Timer_FLT4, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::SerialPort3, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::HRTIM1, PinFunction::Timer_CHC1, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::ADC2, PinFunction::ADC_Channel13, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::SerialPort3, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::HRTIM1, PinFunction::Timer_CHC2, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::ADC1, PinFunction::ADC_Channel13, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM15, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::SerialPort3, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::HRTIM1, PinFunction::Timer_CHD1, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::ADC2, PinFunction::ADC_Channel14, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM15, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::HRTIM1, PinFunction::Timer_CHD2, AF::AF13},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::ADC2, PinFunction::ADC_Channel15, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 0}, Peripheral::TIM1, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 0}, Peripheral::ADC1, PinFunction::ADC_Channel6, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 0}, Peripheral::ADC2, PinFunction::ADC_Channel6, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 1}, Peripheral::ADC1, PinFunction::ADC_Channel7, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 1}, Peripheral::ADC2, PinFunction::ADC_Channel7, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 2}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 2}, Peripheral::ADC1, PinFunction::ADC_Channel8, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 2}, Peripheral::ADC2, PinFunction::ADC_Channel8, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 3}, Peripheral::TIM1, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 3}, Peripheral::TIM1, PinFunction::Timer_BKIN2, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 3}, Peripheral::ADC1, PinFunction::ADC_Channel9, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 3}, Peripheral::ADC2, PinFunction::ADC_Channel9, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 4}, Peripheral::TIM1, PinFunction::Timer_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 4}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortC, 4}, Peripheral::ADC2, PinFunction::ADC_Channel5, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 5}, Peripheral::TIM15, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 5}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortC, 5}, Peripheral::ADC2, PinFunction::ADC_Channel11, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 6}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 6}, Peripheral::HRTIM1, PinFunction::Timer_EEV10, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 7}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 7}, Peripheral::HRTIM1, PinFunction::Timer_FLT5, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 8}, Peripheral::TIM3, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 8}, Peripheral::HRTIM1, PinFunction::Timer_CHE1, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::HRTIM1, PinFunction::Timer_CHE2, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 10}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::HRTIM1, PinFunction::Timer_EEV2, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortC, 12}, Peripheral::HRTIM1, PinFunction::Timer_EEV1, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 12}, Peripheral::SerialPort3, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortC, 13}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF4},
            PinOption{IOPin{IOPin::PortD, 2}, Peripheral::TIM3, PinFunction::Timer_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortF, 0}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF6},

        };
        for (const auto pin_option : pin_options) {
            if (pin_option.pin == pin && pin_option.peripheral == peripheral && pin_option.function == pin_function) {
                return pin_option.af;
            }
        }
        return AF::Invalid;
    }

 public:
    template <Peripheral peripheral, ADCChannel channel, IOPin pin>
    static void route_ADC(stm32f3xx::GPIO::PullType pull = stm32f3xx::GPIO::PullType::NoPull) {
        static_assert(is_adc(peripheral), "You need to pass one of ADC peripherals.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel1 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel1 can be connected only to PortA.0");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel2 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel2 can be connected only to PortA.1");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel3 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel3 can be connected only to PortA.2");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel4 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel4 can be connected only to PortA.3");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel1 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel1 can be connected only to PortA.4");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel2 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel2 can be connected only to PortA.5");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel3 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel3 can be connected only to PortA.6");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel4 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel4 can be connected only to PortA.7");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel12 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel12 can be connected only to PortB.1");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel12 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel12 can be connected only to PortB.2");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel13 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel13 can be connected only to PortB.12");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel13 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel13 can be connected only to PortB.13");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel14 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel14 can be connected only to PortB.14");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel15 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel15 can be connected only to PortB.15");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel6 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel6 can be connected only to PortC.0");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel6 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel6 can be connected only to PortC.0");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel7 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel7 can be connected only to PortC.1");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel7 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel7 can be connected only to PortC.1");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel8 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel8 can be connected only to PortC.2");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel8 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel8 can be connected only to PortC.2");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel9 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel9 can be connected only to PortC.3");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel9 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel9 can be connected only to PortC.3");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel5 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel5 can be connected only to PortC.4");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel11 || peripheral != Peripheral::ADC2,
                      "ADC2 Channel11 can be connected only to PortC.5");

        static_assert(alternate_function == AlternateFunction::ADC, "Invalid ADC channel.");

        GPIO gpio(pin);
        gpio.configureAsInput(pull);
    }

    template <Peripheral peripheral, DACChannel channel, IOPin pin>
    static void route_DAC(stm32f3xx::GPIO::PullType pull = GPIO::PullType::NoPull) {
        static_assert(is_dac(peripheral), "You need to pass one of DAC peripherals.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

        static_assert(alternate_function == AlternateFunction::DAC || pinFunction != PinFunction::DAC_OUT1 || peripheral != Peripheral::DAC1,
                      "DAC1 OUT1 can be connected only to PortA.4");
        static_assert(alternate_function == AlternateFunction::DAC || pinFunction != PinFunction::DAC_OUT2 || peripheral != Peripheral::DAC1,
                      "DAC1 OUT2 can be connected only to PortA.5");
        static_assert(alternate_function == AlternateFunction::DAC || pinFunction != PinFunction::DAC_OUT1 || peripheral != Peripheral::DAC2,
                      "DAC2 OUT1 can be connected only to PortA.6");

        static_assert(alternate_function == AlternateFunction::DAC, "Invalid DAC channel.");

        GPIO gpio(pin);
        gpio.configureAsInput(pull);
    }

    template <Peripheral peripheral, IOPin rx_pin, IOPin tx_pin>
    static void route_CAN() {
        static_assert(is_can(peripheral), "You need to pass CAN peripheral.");

        constexpr AlternateFunction rx_alternate_function = get_alternate_function(peripheral, PinFunction::CAN_RX, rx_pin);
        constexpr AlternateFunction tx_alternate_function = get_alternate_function(peripheral, PinFunction::CAN_TX, tx_pin);

        static_assert(rx_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::CAN1,
                      "CAN1_RX can be connected to: PortA.11 or PortB.8");

        static_assert(tx_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::CAN1,
                      "CAN1_TX can be connected to: PortA.12 or PortB.9");

        GPIO gpio_rx(rx_pin);
        gpio_rx.setAlternateFunction(rx_alternate_function);
        GPIO gpio_tx(tx_pin);
        gpio_tx.setAlternateFunction(tx_alternate_function);
    }

    template <Peripheral peripheral, IOPin sda_pin, IOPin scl_pin>
    static void route_I2C(stm32f3xx::GPIO::PullType sda_pull = GPIO::PullType::NoPull, stm32f3xx::GPIO::PullType scl_pull = GPIO::PullType::NoPull) {
        static_assert(is_i2c(peripheral), "You need to pass I2C peripheral.");

        constexpr AlternateFunction sda_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SDA, sda_pin);
        constexpr AlternateFunction scl_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SCL, scl_pin);

        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SDA' can be connected to: PortA.14, PortB.7 or PortB.9");

        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SCL' can be connected to: PortA.15, PortB.6 or PortB.8");

        GPIO gpio_sda(sda_pin);
        gpio_sda.setAlternateFunction(sda_alternate_function, sda_pull, GPIO::OutputType::OpenDrain);
        GPIO gpio_scl(scl_pin);
        gpio_scl.setAlternateFunction(scl_alternate_function, scl_pull, GPIO::OutputType::OpenDrain);
    }

    template <Peripheral peripheral, IOPinOrUnused rxd_pin, IOPinOrUnused txd_pin>
    static void route_SerialPort(stm32f3xx::GPIO::PullType rxd_pull = GPIO::PullType::NoPull,
                                 stm32f3xx::GPIO::OutputType txd_type = GPIO::OutputType::PushPull,
                                 stm32f3xx::GPIO::PullType txd_pull = GPIO::PullType::NoPull) {
        using AlternateFunction = GPIO::AlternateFunction;
        static_assert(is_serial_port(peripheral), "You need to pass SerialPort peripheral.");
        if constexpr (rxd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_RXD, rxd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort3,
                          "Serial Port 3, pin 'RX' can be connected to: PortB.8, PortB.11 or PortC.11");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'RX' can be connected to: PortA.3, PortA.15 or PortB.4");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'RX' can be connected to: PortA.10, PortB.7 or PortC.5");

            GPIO gpio_rxd(rxd_pin.getIOPin());
            gpio_rxd.setAlternateFunction(alternate_function, rxd_pull);
        }
        if constexpr (txd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_TXD, txd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort3,
                          "Serial Port 3, pin 'TX' can be connected to: PortB.9, PortB.10 or PortC.10");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'TX' can be connected to: PortA.2, PortA.14 or PortB.3");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'TX' can be connected to: PortA.9, PortB.6 or PortC.4");

            GPIO gpio_txd(txd_pin.getIOPin());
            gpio_txd.setAlternateFunction(alternate_function, txd_pull, txd_type);
        }
    }

    template <Peripheral peripheral, IOPinOrUnused miso_pin, IOPinOrUnused mosi_pin, IOPin sck_pin>
    static void route_SPI(stm32f3xx::GPIO::PullType miso_pull = GPIO::PullType::NoPull,
                          stm32f3xx::GPIO::OutputType mosi_type = GPIO::OutputType::PushPull) {
        static_assert(is_spi(peripheral), "You need to pass SPI peripheral.");

        constexpr AlternateFunction miso_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MISO, miso_pin.getIOPin());
        constexpr AlternateFunction mosi_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MOSI, mosi_pin.getIOPin());
        constexpr AlternateFunction sck_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_SCK, sck_pin);

        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MISO' can be connected to: PortA.6 or PortB.4");

        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MOSI' can be connected to: PortA.7 or PortB.5");

        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'SCK' can be connected to: PortA.5 or PortB.3");

        GPIO gpio_miso(miso_pin.getIOPin());
        gpio_miso.setAlternateFunction(miso_alternate_function, miso_pull);
        GPIO gpio_mosi(mosi_pin.getIOPin());
        gpio_mosi.setAlternateFunction(mosi_alternate_function, GPIO::PullType::NoPull, mosi_type);
        GPIO gpio_sck(sck_pin);
        gpio_sck.setAlternateFunction(sck_alternate_function, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
    }

    template <Peripheral peripheral, TimerChannel channel, IOPin pin>
    static void route_timer(stm32f3xx::GPIO::PullType pull = GPIO::PullType::NoPull, stm32f3xx::GPIO::OutputType type = GPIO::OutputType::PushPull) {
        static_assert(is_timer(peripheral), "You need to pass Timer peripheral.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH1,
                      "TIM1_CH1 can be connected to: PortA.8 or PortC.0");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH2,
                      "TIM1_CH2 can be connected only to: PortA.9");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH3,
                      "TIM1_CH3 can be connected to: PortA.10 or PortC.2");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH4,
                      "TIM1_CH4 can be connected to: PortA.11 or PortC.3");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH1,
                      "TIM2_CH1 can be connected to: PortA.0, PortA.5 or PortA.15");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH2,
                      "TIM2_CH2 can be connected to: PortA.1, PortA.2 or PortB.3");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH3,
                      "TIM2_CH3 can be connected to: PortA.9 or PortB.10");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH4,
                      "TIM2_CH4 can be connected to: PortA.3, PortA.10 or PortB.11");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH1,
                      "TIM3_CH1 can be connected to: PortA.6, PortB.4 or PortC.6");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH2,
                      "TIM3_CH2 can be connected to: PortA.4, PortA.7, PortB.5 or PortC.7");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH3,
                      "TIM3_CH3 can be connected to: PortB.0 or PortC.8");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH4,
                      "TIM3_CH4 can be connected to: PortB.1, PortB.7 or PortC.9");

        GPIO gpio(pin);
        gpio.setAlternateFunction(alternate_function, pull, type);
    }
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif  // _MICROHAL_PORTS_IOMANAGER_STM32F334_H_
