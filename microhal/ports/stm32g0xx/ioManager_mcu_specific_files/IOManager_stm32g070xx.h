
#ifndef _MICROHAL_PORTS_IOMANAGER_STM32G070_H_
#define _MICROHAL_PORTS_IOMANAGER_STM32G070_H_

#include "gpio_stm32g0xx.h"
#include "ports/stmCommon/ioManager/ioPinOrUnused.h"
#include "ports/stmCommon/ioManager/pinOption.h"

namespace microhal {
namespace stm32g0xx {

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
        const std::array<const PinOption, 203> pin_options = {
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::SerialPort2, PinFunction::SerialPort_CTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::SerialPort4, PinFunction::SerialPort_TXD, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::ADC1, PinFunction::ADC_Channel0, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::I2S1, PinFunction::I2S_CK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::SerialPort2, PinFunction::SerialPort_RTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::SerialPort4, PinFunction::SerialPort_RXD, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::I2C1, PinFunction::I2C_SMBA, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::ADC1, PinFunction::ADC_Channel1, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::I2S1, PinFunction::I2S_SD, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM15, PinFunction::Timer_CH1, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::ADC1, PinFunction::ADC_Channel2, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM15, PinFunction::Timer_CH2, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::ADC1, PinFunction::ADC_Channel3, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::I2S1, PinFunction::I2S_WS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::ADC1, PinFunction::ADC_Channel4, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::I2S1, PinFunction::I2S_CKCEC, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::ADC1, PinFunction::ADC_Channel5, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::I2S1, PinFunction::I2S_MCK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::SerialPort3, PinFunction::SerialPort_CTS, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::ADC1, PinFunction::ADC_Channel6, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::I2S1, PinFunction::I2S_SD, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::ADC1, PinFunction::ADC_Channel7, AF::ADC},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::TIM1, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM1, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM15, PinFunction::Timer_BKIN, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM17, PinFunction::Timer_BKIN, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::I2S1, PinFunction::I2S_MCK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::SerialPort1, PinFunction::SerialPort_CTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::TIM1, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::TIM1, PinFunction::Timer_BKIN2, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::I2C2, PinFunction::I2C_SCL, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::I2S1, PinFunction::I2S_SD, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::SerialPort1, PinFunction::SerialPort_RTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::TIM1, PinFunction::Timer_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::I2S, PinFunction::I2S_CKIN, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::I2C2, PinFunction::I2C_SDA, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 14}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::I2S1, PinFunction::I2S_WS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SerialPort4, PinFunction::SerialPort_RTS, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SerialPort3, PinFunction::SerialPort_RTS, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::I2S1, PinFunction::I2S_WS, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM3, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::ADC1, PinFunction::ADC_Channel8, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::SerialPort3, PinFunction::SerialPort_RTS, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::ADC1, PinFunction::ADC_Channel9, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 2}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 2}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 2}, Peripheral::ADC1, PinFunction::ADC_Channel10, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::I2S1, PinFunction::I2S_CK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::TIM1, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::SerialPort1, PinFunction::SerialPort_RTS, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::I2S1, PinFunction::I2S_MCK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::SerialPort1, PinFunction::SerialPort_CTS, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::TIM17, PinFunction::Timer_BKIN, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::I2S1, PinFunction::I2S_SD, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM16, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::I2C1, PinFunction::I2C_SMBA, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::TIM16, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::TIM17, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::SerialPort4, PinFunction::SerialPort_CTS, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::TIM15, PinFunction::Timer_BKIN, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::I2C2, PinFunction::I2C_SCL, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::ADC1, PinFunction::ADC_Channel11, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::I2C2, PinFunction::I2C_SDA, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::ADC1, PinFunction::ADC_Channel15, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::TIM15, PinFunction::Timer_BKIN, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::ADC1, PinFunction::ADC_Channel16, AF::ADC},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::SerialPort3, PinFunction::SerialPort_CTS, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::I2C2, PinFunction::I2C_SCL, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::SerialPort3, PinFunction::SerialPort_RTS, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM15, PinFunction::Timer_CH1, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::I2C2, PinFunction::I2C_SDA, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM15, PinFunction::Timer_CH2, AF::AF5},
            PinOption{IOPin{IOPin::PortC, 1}, Peripheral::TIM15, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 2}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 2}, Peripheral::TIM15, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 3}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 4}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF0},
            PinOption{IOPin{IOPin::PortC, 4}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 4}, Peripheral::ADC1, PinFunction::ADC_Channel17, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 5}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF0},
            PinOption{IOPin{IOPin::PortC, 5}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 5}, Peripheral::ADC1, PinFunction::ADC_Channel18, AF::ADC},
            PinOption{IOPin{IOPin::PortC, 6}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 7}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 8}, Peripheral::TIM3, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 8}, Peripheral::TIM1, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::I2S, PinFunction::I2S_CKIN, AF::AF0},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::TIM1, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 10}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF0},
            PinOption{IOPin{IOPin::PortC, 10}, Peripheral::SerialPort4, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 10}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF0},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::SerialPort4, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::TIM1, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 12}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 13}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 14}, Peripheral::TIM1, PinFunction::Timer_BKIN2, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 15}, Peripheral::TIM15, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 0}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 0}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 1}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 1}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 2}, Peripheral::SerialPort3, PinFunction::SerialPort_RTS, AF::AF0},
            PinOption{IOPin{IOPin::PortD, 2}, Peripheral::TIM3, PinFunction::Timer_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 2}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 3}, Peripheral::SerialPort2, PinFunction::SerialPort_CTS, AF::AF0},
            PinOption{IOPin{IOPin::PortD, 3}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 3}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 4}, Peripheral::SerialPort2, PinFunction::SerialPort_RTS, AF::AF0},
            PinOption{IOPin{IOPin::PortD, 4}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 4}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 5}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF0},
            PinOption{IOPin{IOPin::PortD, 5}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 5}, Peripheral::I2S1, PinFunction::I2S_MCK, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 5}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 6}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF0},
            PinOption{IOPin{IOPin::PortD, 6}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 6}, Peripheral::I2S1, PinFunction::I2S_SDLP, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 8}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF0},
            PinOption{IOPin{IOPin::PortD, 8}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 8}, Peripheral::I2S1, PinFunction::I2S_CKLP, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 9}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF0},
            PinOption{IOPin{IOPin::PortD, 9}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 9}, Peripheral::I2S1, PinFunction::I2S_WS, AF::AF1},
            PinOption{IOPin{IOPin::PortD, 9}, Peripheral::TIM1, PinFunction::Timer_BKIN2, AF::AF2},
            PinOption{IOPin{IOPin::PortF, 0}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortF, 1}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF2},

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
    static void route_ADC(stm32g0xx::GPIO::PullType pull = stm32g0xx::GPIO::PullType::NoPull) {
        static_assert(is_adc(peripheral), "You need to pass one of ADC peripherals.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel0 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel0 can be connected only to PortA.0");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel1 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel1 can be connected only to PortA.1");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel2 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel2 can be connected only to PortA.2");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel3 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel3 can be connected only to PortA.3");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel4 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel4 can be connected only to PortA.4");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel5 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel5 can be connected only to PortA.5");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel6 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel6 can be connected only to PortA.6");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel7 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel7 can be connected only to PortA.7");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel8 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel8 can be connected only to PortB.0");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel9 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel9 can be connected only to PortB.1");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel10 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel10 can be connected only to PortB.2");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel11 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel11 can be connected only to PortB.10");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel15 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel15 can be connected only to PortB.11");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel16 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel16 can be connected only to PortB.12");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel17 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel17 can be connected only to PortC.4");
        static_assert(alternate_function == AlternateFunction::ADC || pinFunction != PinFunction::ADC_Channel18 || peripheral != Peripheral::ADC1,
                      "ADC1 Channel18 can be connected only to PortC.5");

        static_assert(alternate_function == AlternateFunction::ADC, "Invalid ADC channel.");

        GPIO gpio(pin);
        gpio.configureAsInput(pull);
    }

    template <Peripheral peripheral, IOPin sda_pin, IOPin scl_pin>
    static void route_I2C(stm32g0xx::GPIO::PullType sda_pull = GPIO::PullType::NoPull, stm32g0xx::GPIO::PullType scl_pull = GPIO::PullType::NoPull) {
        static_assert(is_i2c(peripheral), "You need to pass I2C peripheral.");

        constexpr AlternateFunction sda_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SDA, sda_pin);
        constexpr AlternateFunction scl_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SCL, scl_pin);

        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SDA' can be connected to: PortA.10, PortB.7 or PortB.9");
        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C2,
                      "I2C2 pin 'SDA' can be connected to: PortA.12, PortB.11 or PortB.14");

        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SCL' can be connected to: PortA.9, PortB.6 or PortB.8");
        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C2,
                      "I2C2 pin 'SCL' can be connected to: PortA.11, PortB.10 or PortB.13");

        GPIO gpio_sda(sda_pin);
        gpio_sda.setAlternateFunction(sda_alternate_function, sda_pull, GPIO::OutputType::OpenDrain);
        GPIO gpio_scl(scl_pin);
        gpio_scl.setAlternateFunction(scl_alternate_function, scl_pull, GPIO::OutputType::OpenDrain);
    }

    template <Peripheral peripheral, IOPinOrUnused rxd_pin, IOPinOrUnused txd_pin>
    static void route_SerialPort(stm32g0xx::GPIO::PullType rxd_pull = GPIO::PullType::NoPull,
                                 stm32g0xx::GPIO::OutputType txd_type = GPIO::OutputType::PushPull,
                                 stm32g0xx::GPIO::PullType txd_pull = GPIO::PullType::NoPull) {
        using AlternateFunction = GPIO::AlternateFunction;
        static_assert(is_serial_port(peripheral), "You need to pass SerialPort peripheral.");
        if constexpr (rxd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_RXD, rxd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort3,
                          "Serial Port 3, pin 'RX' can be connected to: PortB.0, PortB.9, PortB.11, PortC.5, PortC.11 or PortD.9");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'RX' can be connected to: PortA.3, PortA.15 or PortD.6");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort4,
                          "Serial Port 4, pin 'RX' can be connected to: PortA.1 or PortC.11");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'RX' can be connected to: PortA.10, PortB.7 or PortC.5");

            GPIO gpio_rxd(rxd_pin.getIOPin());
            gpio_rxd.setAlternateFunction(alternate_function, rxd_pull, stm32g0xx::GPIO::OutputType::OpenDrain);
        }
        if constexpr (txd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_TXD, txd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort3,
                          "Serial Port 3, pin 'TX' can be connected to: PortA.5, PortB.2, PortB.8, PortB.10, PortC.4, PortC.10 or PortD.8");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'TX' can be connected to: PortA.2, PortA.14 or PortD.5");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort4,
                          "Serial Port 4, pin 'TX' can be connected to: PortA.0 or PortC.10");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'TX' can be connected to: PortA.9, PortB.6 or PortC.4");

            GPIO gpio_txd(txd_pin.getIOPin());
            gpio_txd.setAlternateFunction(alternate_function, txd_pull, txd_type);
        }
    }

    template <Peripheral peripheral, IOPinOrUnused miso_pin, IOPinOrUnused mosi_pin, IOPin sck_pin>
    static void route_SPI(stm32g0xx::GPIO::PullType sda_pull = GPIO::PullType::NoPull, stm32g0xx::GPIO::PullType scl_pull = GPIO::PullType::NoPull) {
        static_assert(is_i2c(peripheral), "You need to pass I2C peripheral.");

        constexpr AlternateFunction miso_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MISO, miso_pin.getIOPin());
        constexpr AlternateFunction mosi_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MOSI, mosi_pin.getIOPin());
        constexpr AlternateFunction sck_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_SCK, sck_pin);

        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MISO' can be connected to: PortA.6, PortA.11, PortB.4 or PortD.5");
        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'MISO' can be connected to: PortA.3, PortA.9, PortB.2, PortB.6, PortB.14, PortC.2 or PortD.3");

        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MOSI' can be connected to: PortA.2, PortA.7, PortA.12, PortB.5 or PortD.6");
        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'MOSI' can be connected to: PortA.4, PortA.10, PortB.7, PortB.11, PortB.15, PortC.3 or PortD.4");

        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'SCK' can be connected to: PortA.1, PortA.5, PortB.3 or PortD.8");
        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'SCK' can be connected to: PortA.0, PortB.8, PortB.10, PortB.13 or PortD.1");

        GPIO gpio_miso(miso_pin.getIOPin());
        gpio_miso.setAlternateFunction(miso_alternate_function, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
        GPIO gpio_mosi(mosi_pin.getIOPin());
        gpio_mosi.setAlternateFunction(mosi_alternate_function, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
        GPIO gpio_sck(sck_pin);
        gpio_sck.setAlternateFunction(sck_alternate_function, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
    }
};

}  // namespace stm32g0xx
}  // namespace microhal

#endif  // _MICROHAL_PORTS_IOMANAGER_STM32G070_H_
