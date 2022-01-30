
#ifndef _MICROHAL_PORTS_IOMANAGER_STM32F40XXX_H_
#define _MICROHAL_PORTS_IOMANAGER_STM32F40XXX_H_

#include "../gpio_stm32f4xx.h"
#include "ports/stmCommon/ioManager/ioPinOrUnused.h"
#include "ports/stmCommon/ioManager/pinOption.h"

namespace microhal {
namespace stm32f4xx {

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
        const std::array<const PinOption, 220> pin_options = {
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::TIM2, PinFunction::Timer_CH1_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::TIM5, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::TIM8, PinFunction::Timer_ETR, AF::AF3},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::SerialPort2, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::TIM2, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::TIM5, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::SerialPort2, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM2, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM5, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM9, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM5, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM9, PinFunction::Timer_CH2, AF::AF3},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SPI3, PinFunction::SPI_NSS, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::I2S3, PinFunction::I2S_WS, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SerialPort2, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::TIM2, PinFunction::Timer_CH1_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::TIM8, PinFunction::Timer_CH1N, AF::AF3},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM8, PinFunction::Timer_BKIN, AF::AF3},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM13, PinFunction::Timer_CH1, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM8, PinFunction::Timer_CH1N, AF::AF3},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::TIM1, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::I2C3, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::SerialPort1, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM1, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::I2C3, PinFunction::I2C_SMBA, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::TIM1, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::SerialPort1, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::CAN1, PinFunction::CAN_RX, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::TIM1, PinFunction::Timer_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::SerialPort1, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::CAN1, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::TIM2, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::TIM2, PinFunction::Timer_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SPI3, PinFunction::SPI_NSS, AF::AF6},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::I2S3, PinFunction::I2S_WS, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM3, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM8, PinFunction::Timer_CH2N, AF::AF3},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM8, PinFunction::Timer_CH3N, AF::AF3},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::TIM2, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::SPI3, PinFunction::SPI_SCK, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::I2S3, PinFunction::I2S_CK, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::SPI3, PinFunction::SPI_MISO, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::I2S3ext, PinFunction::I2S_SD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::I2C1, PinFunction::I2C_SMBA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::SPI3, PinFunction::SPI_MOSI, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::I2S3, PinFunction::I2S_SD, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::CAN2, PinFunction::CAN_RX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::TIM4, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::CAN2, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::TIM4, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::TIM4, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::TIM10, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::CAN1, PinFunction::CAN_RX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::TIM4, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::TIM11, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::I2S2, PinFunction::I2S_WS, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::CAN1, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::TIM2, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::I2C2, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::I2S2, PinFunction::I2S_CK, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::I2C2, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::I2C2, PinFunction::I2C_SMBA, AF::AF4},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::I2S2, PinFunction::I2S_WS, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::SerialPort3, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::CAN2, PinFunction::CAN_RX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::I2S2, PinFunction::I2S_CK, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::SerialPort3, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::CAN2, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM8, PinFunction::Timer_CH2N, AF::AF3},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::I2S2ext, PinFunction::I2S_SD, AF::AF6},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::SerialPort3, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM12, PinFunction::Timer_CH1, AF::AF9},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM8, PinFunction::Timer_CH3N, AF::AF3},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::I2S2, PinFunction::I2S_SD, AF::AF5},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM12, PinFunction::Timer_CH2, AF::AF9},
            PinOption{IOPin{IOPin::PortC, 2}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF5},
            PinOption{IOPin{IOPin::PortC, 2}, Peripheral::I2S2ext, PinFunction::I2S_SD, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 3}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF5},
            PinOption{IOPin{IOPin::PortC, 3}, Peripheral::I2S2, PinFunction::I2S_SD, AF::AF5},
            PinOption{IOPin{IOPin::PortC, 6}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 6}, Peripheral::TIM8, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 6}, Peripheral::I2S2, PinFunction::I2S_MCK, AF::AF5},
            PinOption{IOPin{IOPin::PortC, 6}, Peripheral::SerialPort6, PinFunction::SerialPort_TXD, AF::AF8},
            PinOption{IOPin{IOPin::PortC, 7}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 7}, Peripheral::TIM8, PinFunction::Timer_CH2, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 7}, Peripheral::I2S3, PinFunction::I2S_MCK, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 7}, Peripheral::SerialPort6, PinFunction::SerialPort_RXD, AF::AF8},
            PinOption{IOPin{IOPin::PortC, 8}, Peripheral::TIM3, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 8}, Peripheral::TIM8, PinFunction::Timer_CH3, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 8}, Peripheral::SerialPort6, PinFunction::SerialPort_CK, AF::AF8},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::TIM8, PinFunction::Timer_CH4, AF::AF3},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::I2C3, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortC, 9}, Peripheral::I2S, PinFunction::I2S_CKIN, AF::AF5},
            PinOption{IOPin{IOPin::PortC, 10}, Peripheral::SPI3, PinFunction::SPI_SCK, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 10}, Peripheral::I2S3, PinFunction::I2S_CK, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 10}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::I2S3ext, PinFunction::I2S_SD, AF::AF5},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::SPI3, PinFunction::SPI_MISO, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 11}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortC, 12}, Peripheral::SPI3, PinFunction::SPI_MOSI, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 12}, Peripheral::I2S3, PinFunction::I2S_SD, AF::AF6},
            PinOption{IOPin{IOPin::PortC, 12}, Peripheral::SerialPort3, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 0}, Peripheral::CAN1, PinFunction::CAN_RX, AF::AF9},
            PinOption{IOPin{IOPin::PortD, 1}, Peripheral::CAN1, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortD, 2}, Peripheral::TIM3, PinFunction::Timer_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 3}, Peripheral::SerialPort2, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 4}, Peripheral::SerialPort2, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 5}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 6}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 7}, Peripheral::SerialPort2, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 8}, Peripheral::SerialPort3, PinFunction::SerialPort_TXD, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 9}, Peripheral::SerialPort3, PinFunction::SerialPort_RXD, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 10}, Peripheral::SerialPort3, PinFunction::SerialPort_CK, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 11}, Peripheral::SerialPort3, PinFunction::SerialPort_CTS, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 12}, Peripheral::TIM4, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 12}, Peripheral::SerialPort3, PinFunction::SerialPort_RTS, AF::AF7},
            PinOption{IOPin{IOPin::PortD, 13}, Peripheral::TIM4, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 14}, Peripheral::TIM4, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortD, 15}, Peripheral::TIM4, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortE, 0}, Peripheral::TIM4, PinFunction::Timer_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortE, 5}, Peripheral::TIM9, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortE, 6}, Peripheral::TIM9, PinFunction::Timer_CH2, AF::AF3},
            PinOption{IOPin{IOPin::PortE, 7}, Peripheral::TIM1, PinFunction::Timer_ETR, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 8}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 9}, Peripheral::TIM1, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 10}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 11}, Peripheral::TIM1, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 12}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 13}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 14}, Peripheral::TIM1, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortE, 15}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF1},
            PinOption{IOPin{IOPin::PortF, 0}, Peripheral::I2C2, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortF, 1}, Peripheral::I2C2, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortF, 2}, Peripheral::I2C2, PinFunction::I2C_SMBA, AF::AF4},
            PinOption{IOPin{IOPin::PortF, 6}, Peripheral::TIM10, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortF, 7}, Peripheral::TIM11, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortF, 8}, Peripheral::TIM13, PinFunction::Timer_CH1, AF::AF9},
            PinOption{IOPin{IOPin::PortF, 9}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF9},
            PinOption{IOPin{IOPin::PortG, 7}, Peripheral::SerialPort6, PinFunction::SerialPort_CK, AF::AF8},
            PinOption{IOPin{IOPin::PortG, 8}, Peripheral::SerialPort6, PinFunction::SerialPort_RTS, AF::AF8},
            PinOption{IOPin{IOPin::PortG, 9}, Peripheral::SerialPort6, PinFunction::SerialPort_RXD, AF::AF8},
            PinOption{IOPin{IOPin::PortG, 12}, Peripheral::SerialPort6, PinFunction::SerialPort_RTS, AF::AF8},
            PinOption{IOPin{IOPin::PortG, 13}, Peripheral::SerialPort6, PinFunction::SerialPort_CTS, AF::AF8},
            PinOption{IOPin{IOPin::PortG, 14}, Peripheral::SerialPort6, PinFunction::SerialPort_TXD, AF::AF8},
            PinOption{IOPin{IOPin::PortG, 15}, Peripheral::SerialPort6, PinFunction::SerialPort_CTS, AF::AF8},
            PinOption{IOPin{IOPin::PortH, 4}, Peripheral::I2C2, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortH, 5}, Peripheral::I2C2, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortH, 6}, Peripheral::I2C2, PinFunction::I2C_SMBA, AF::AF4},
            PinOption{IOPin{IOPin::PortH, 6}, Peripheral::TIM12, PinFunction::Timer_CH1, AF::AF9},
            PinOption{IOPin{IOPin::PortH, 7}, Peripheral::I2C3, PinFunction::I2C_SCL, AF::AF4},
            PinOption{IOPin{IOPin::PortH, 8}, Peripheral::I2C3, PinFunction::I2C_SDA, AF::AF4},
            PinOption{IOPin{IOPin::PortH, 9}, Peripheral::I2C3, PinFunction::I2C_SMBA, AF::AF4},
            PinOption{IOPin{IOPin::PortH, 9}, Peripheral::TIM12, PinFunction::Timer_CH2, AF::AF9},
            PinOption{IOPin{IOPin::PortH, 10}, Peripheral::TIM5, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortH, 11}, Peripheral::TIM5, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortH, 12}, Peripheral::TIM5, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortH, 13}, Peripheral::TIM8, PinFunction::Timer_CH1N, AF::AF3},
            PinOption{IOPin{IOPin::PortH, 13}, Peripheral::CAN1, PinFunction::CAN_TX, AF::AF9},
            PinOption{IOPin{IOPin::PortH, 14}, Peripheral::TIM8, PinFunction::Timer_CH2N, AF::AF3},
            PinOption{IOPin{IOPin::PortH, 15}, Peripheral::TIM8, PinFunction::Timer_CH3N, AF::AF3},
            PinOption{IOPin{IOPin::PortI, 0}, Peripheral::TIM5, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortI, 0}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF5},
            PinOption{IOPin{IOPin::PortI, 0}, Peripheral::I2S2, PinFunction::I2S_WS, AF::AF5},
            PinOption{IOPin{IOPin::PortI, 1}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF5},
            PinOption{IOPin{IOPin::PortI, 1}, Peripheral::I2S2, PinFunction::I2S_CK, AF::AF5},
            PinOption{IOPin{IOPin::PortI, 2}, Peripheral::TIM8, PinFunction::Timer_CH4, AF::AF3},
            PinOption{IOPin{IOPin::PortI, 2}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF5},
            PinOption{IOPin{IOPin::PortI, 2}, Peripheral::I2S2ext, PinFunction::I2S_SD, AF::AF6},
            PinOption{IOPin{IOPin::PortI, 3}, Peripheral::TIM8, PinFunction::Timer_ETR, AF::AF3},
            PinOption{IOPin{IOPin::PortI, 3}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF5},
            PinOption{IOPin{IOPin::PortI, 3}, Peripheral::I2S2, PinFunction::I2S_SD, AF::AF5},
            PinOption{IOPin{IOPin::PortI, 4}, Peripheral::TIM8, PinFunction::Timer_BKIN, AF::AF3},
            PinOption{IOPin{IOPin::PortI, 5}, Peripheral::TIM8, PinFunction::Timer_CH1, AF::AF3},
            PinOption{IOPin{IOPin::PortI, 6}, Peripheral::TIM8, PinFunction::Timer_CH2, AF::AF3},
            PinOption{IOPin{IOPin::PortI, 7}, Peripheral::TIM8, PinFunction::Timer_CH3, AF::AF3},
            PinOption{IOPin{IOPin::PortI, 9}, Peripheral::CAN1, PinFunction::CAN_RX, AF::AF9},

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
    static void route_ADC(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull) {
        static_assert(is_adc(peripheral), "You need to pass one of ADC peripherals.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

        static_assert(alternate_function == AlternateFunction::ADC, "Invalid ADC channel.");

        GPIO gpio(pin);
        gpio.configureAsInput(pull);
    }

    template <Peripheral peripheral, DACChannel channel, IOPin pin>
    static void route_DAC(stm32f4xx::GPIO::PullType pull = GPIO::PullType::NoPull) {
        static_assert(is_dac(peripheral), "You need to pass one of DAC peripherals.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

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
                      "CAN1_RX can be connected to: PortA.11, PortB.8, PortD.0 or PortI.9");
        static_assert(rx_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::CAN2,
                      "CAN2_RX can be connected to: PortB.5 or PortB.12");

        static_assert(tx_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::CAN1,
                      "CAN1_TX can be connected to: PortA.12, PortB.9, PortD.1 or PortH.13");
        static_assert(tx_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::CAN2,
                      "CAN2_TX can be connected to: PortB.6 or PortB.13");

        GPIO gpio_rx(rx_pin);
        gpio_rx.setAlternateFunction(rx_alternate_function);
        GPIO gpio_tx(tx_pin);
        gpio_tx.setAlternateFunction(tx_alternate_function);
    }

    template <Peripheral peripheral, IOPin sda_pin, IOPin scl_pin>
    static void route_I2C(stm32f4xx::GPIO::PullType sda_pull = GPIO::PullType::NoPull, stm32f4xx::GPIO::PullType scl_pull = GPIO::PullType::NoPull) {
        static_assert(is_i2c(peripheral), "You need to pass I2C peripheral.");

        constexpr AlternateFunction sda_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SDA, sda_pin);
        constexpr AlternateFunction scl_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SCL, scl_pin);

        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SDA' can be connected to: PortB.7 or PortB.9");
        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C2,
                      "I2C2 pin 'SDA' can be connected to: PortB.11, PortF.0 or PortH.5");
        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C3,
                      "I2C3 pin 'SDA' can be connected to: PortC.9 or PortH.8");

        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SCL' can be connected to: PortB.6 or PortB.8");
        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C2,
                      "I2C2 pin 'SCL' can be connected to: PortB.10, PortF.1 or PortH.4");
        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C3,
                      "I2C3 pin 'SCL' can be connected to: PortA.8 or PortH.7");

        GPIO gpio_sda(sda_pin);
        gpio_sda.setAlternateFunction(sda_alternate_function, sda_pull, GPIO::OutputType::OpenDrain);
        GPIO gpio_scl(scl_pin);
        gpio_scl.setAlternateFunction(scl_alternate_function, scl_pull, GPIO::OutputType::OpenDrain);
    }

    template <Peripheral peripheral, IOPinOrUnused rxd_pin, IOPinOrUnused txd_pin>
    static void route_SerialPort(stm32f4xx::GPIO::PullType rxd_pull = GPIO::PullType::NoPull,
                                 stm32f4xx::GPIO::OutputType txd_type = GPIO::OutputType::PushPull,
                                 stm32f4xx::GPIO::PullType txd_pull = GPIO::PullType::NoPull) {
        using AlternateFunction = GPIO::AlternateFunction;
        static_assert(is_serial_port(peripheral), "You need to pass SerialPort peripheral.");
        if constexpr (rxd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_RXD, rxd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'RX' can be connected to: PortA.3 or PortD.6");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort6,
                          "Serial Port 6, pin 'RX' can be connected to: PortC.7 or PortG.9");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'RX' can be connected to: PortA.10 or PortB.7");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort5,
                          "UART5_RX can be connected only to: PortD.2");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort3,
                          "Serial Port 3, pin 'RX' can be connected to: PortB.11, PortC.11 or PortD.9");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort4,
                          "UART4_RX can be connected to: PortA.1 or PortC.11");

            GPIO gpio_rxd(rxd_pin.getIOPin());
            gpio_rxd.setAlternateFunction(alternate_function, rxd_pull);
        }
        if constexpr (txd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_TXD, txd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'TX' can be connected to: PortA.2 or PortD.5");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort6,
                          "Serial Port 6, pin 'TX' can be connected to: PortC.6 or PortG.14");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'TX' can be connected to: PortA.9 or PortB.6");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort5,
                          "UART5_TX can be connected only to: PortC.12");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort3,
                          "Serial Port 3, pin 'TX' can be connected to: PortB.10, PortC.10 or PortD.8");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort4,
                          "UART4_TX can be connected to: PortA.0 or PortC.10");

            GPIO gpio_txd(txd_pin.getIOPin());
            gpio_txd.setAlternateFunction(alternate_function, txd_pull, txd_type);
        }
    }

    template <Peripheral peripheral, IOPinOrUnused miso_pin, IOPinOrUnused mosi_pin, IOPin sck_pin>
    static void route_SPI(stm32f4xx::GPIO::PullType miso_pull = GPIO::PullType::NoPull,
                          stm32f4xx::GPIO::OutputType mosi_type = GPIO::OutputType::PushPull) {
        static_assert(is_spi(peripheral), "You need to pass SPI peripheral.");

        constexpr AlternateFunction miso_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MISO, miso_pin.getIOPin());
        constexpr AlternateFunction mosi_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MOSI, mosi_pin.getIOPin());
        constexpr AlternateFunction sck_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_SCK, sck_pin);

        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MISO' can be connected to: PortA.6 or PortB.4");
        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'MISO' can be connected to: PortB.14, PortC.2 or PortI.2");
        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI3,
                      "SPI3 pin 'MISO' can be connected to: PortB.4 or PortC.11");

        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MOSI' can be connected to: PortA.7 or PortB.5");
        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'MOSI' can be connected to: PortB.15, PortC.3 or PortI.3");
        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI3,
                      "SPI3 pin 'MOSI' can be connected to: PortB.5 or PortC.12");

        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'SCK' can be connected to: PortA.5 or PortB.3");
        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'SCK' can be connected to: PortB.10, PortB.13 or PortI.1");
        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI3,
                      "SPI3 pin 'SCK' can be connected to: PortB.3 or PortC.10");

        GPIO gpio_miso(miso_pin.getIOPin());
        gpio_miso.setAlternateFunction(miso_alternate_function, miso_pull);
        GPIO gpio_mosi(mosi_pin.getIOPin());
        gpio_mosi.setAlternateFunction(mosi_alternate_function, GPIO::PullType::NoPull, mosi_type);
        GPIO gpio_sck(sck_pin);
        gpio_sck.setAlternateFunction(sck_alternate_function, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
    }

    template <Peripheral peripheral, TimerChannel channel, IOPin pin>
    static void route_timer(stm32f4xx::GPIO::PullType pull = GPIO::PullType::NoPull, stm32f4xx::GPIO::OutputType type = GPIO::OutputType::PushPull) {
        static_assert(is_timer(peripheral), "You need to pass Timer peripheral.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH1,
                      "TIM1_CH1 can be connected to: PortA.8 or PortE.9");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH2,
                      "TIM1_CH2 can be connected to: PortA.9 or PortE.11");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH3,
                      "TIM1_CH3 can be connected to: PortA.10 or PortE.13");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH4,
                      "TIM1_CH4 can be connected to: PortA.11 or PortE.14");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH1,
                      "TIM2_CH1 can be connected only to: PortA.15");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH2,
                      "TIM2_CH2 can be connected to: PortA.1 or PortB.3");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH3,
                      "TIM2_CH3 can be connected to: PortA.2 or PortB.10");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH4,
                      "TIM2_CH4 can be connected to: PortA.3 or PortB.11");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH1,
                      "TIM3_CH1 can be connected to: PortA.6, PortB.4 or PortC.6");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH2,
                      "TIM3_CH2 can be connected to: PortA.7, PortB.5 or PortC.7");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH3,
                      "TIM3_CH3 can be connected to: PortB.0 or PortC.8");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH4,
                      "TIM3_CH4 can be connected to: PortB.1 or PortC.9");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM4 || pinFunction != PinFunction::Timer_CH1,
                      "TIM4_CH1 can be connected to: PortB.6 or PortD.12");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM4 || pinFunction != PinFunction::Timer_CH2,
                      "TIM4_CH2 can be connected to: PortB.7 or PortD.13");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM4 || pinFunction != PinFunction::Timer_CH3,
                      "TIM4_CH3 can be connected to: PortB.8 or PortD.14");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM4 || pinFunction != PinFunction::Timer_CH4,
                      "TIM4_CH4 can be connected to: PortB.9 or PortD.15");

        GPIO gpio(pin);
        gpio.setAlternateFunction(alternate_function, pull, type);
    }
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_PORTS_IOMANAGER_STM32F40XXX_H_
