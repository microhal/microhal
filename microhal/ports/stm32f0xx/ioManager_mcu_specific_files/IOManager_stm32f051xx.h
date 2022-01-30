
#ifndef _MICROHAL_PORTS_IOMANAGER_STM32F051XX_H_
#define _MICROHAL_PORTS_IOMANAGER_STM32F051XX_H_

#include <array>
#include "../gpio_stm32f0xx.h"
#include "ports/stmCommon/ioManager/ioPinOrUnused.h"
#include "ports/stmCommon/ioManager/pinOption.h"

namespace microhal {
namespace stm32f0xx {

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
        const std::array<const PinOption, 86> pin_options = {
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::SerialPort2, PinFunction::SerialPort_CTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 0}, Peripheral::TIM2, PinFunction::Timer_CH1_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::SerialPort2, PinFunction::SerialPort_RTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 1}, Peripheral::TIM2, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM15, PinFunction::Timer_CH1, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 2}, Peripheral::TIM2, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM15, PinFunction::Timer_CH2, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 3}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::I2S1, PinFunction::I2S_WS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::SerialPort2, PinFunction::SerialPort_CK, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 4}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::I2S1, PinFunction::I2S_CK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 5}, Peripheral::TIM2, PinFunction::Timer_CH1_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::I2S1, PinFunction::I2S_MCK, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 6}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::I2S1, PinFunction::I2S_SD, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF4},
            PinOption{IOPin{IOPin::PortA, 7}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF5},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::SerialPort1, PinFunction::SerialPort_CK, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 8}, Peripheral::TIM1, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM15, PinFunction::Timer_BKIN, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 9}, Peripheral::TIM1, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM17, PinFunction::Timer_BKIN, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 10}, Peripheral::TIM1, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::SerialPort1, PinFunction::SerialPort_CTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 11}, Peripheral::TIM1, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::SerialPort1, PinFunction::SerialPort_RTS, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 12}, Peripheral::TIM1, PinFunction::Timer_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortA, 14}, Peripheral::SerialPort2, PinFunction::SerialPort_TXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SPI1, PinFunction::SPI_NSS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::I2S1, PinFunction::I2S_WS, AF::AF0},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::SerialPort2, PinFunction::SerialPort_RXD, AF::AF1},
            PinOption{IOPin{IOPin::PortA, 15}, Peripheral::TIM2, PinFunction::Timer_CH1_ETR, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM3, PinFunction::Timer_CH3, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 0}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM14, PinFunction::Timer_CH1, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM3, PinFunction::Timer_CH4, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 1}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::SPI1, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::I2S1, PinFunction::I2S_CK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 3}, Peripheral::TIM2, PinFunction::Timer_CH2, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::SPI1, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::I2S1, PinFunction::I2S_MCK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 4}, Peripheral::TIM3, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::SPI1, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::I2S1, PinFunction::I2S_SD, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM3, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::TIM16, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 5}, Peripheral::I2C1, PinFunction::I2C_SMBA, AF::AF3},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::SerialPort1, PinFunction::SerialPort_TXD, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 6}, Peripheral::TIM16, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::SerialPort1, PinFunction::SerialPort_RXD, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 7}, Peripheral::TIM17, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::I2C1, PinFunction::I2C_SCL, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 8}, Peripheral::TIM16, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::I2C1, PinFunction::I2C_SDA, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 9}, Peripheral::TIM17, PinFunction::Timer_CH1, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::I2C2, PinFunction::I2C_SCL, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 10}, Peripheral::TIM2, PinFunction::Timer_CH3, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::I2C2, PinFunction::I2C_SDA, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 11}, Peripheral::TIM2, PinFunction::Timer_CH4, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::SPI2, PinFunction::SPI_NSS, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 12}, Peripheral::TIM1, PinFunction::Timer_BKIN, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::SPI2, PinFunction::SPI_SCK, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 13}, Peripheral::TIM1, PinFunction::Timer_CH1N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::SPI2, PinFunction::SPI_MISO, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM15, PinFunction::Timer_CH1, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 14}, Peripheral::TIM1, PinFunction::Timer_CH2N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::SPI2, PinFunction::SPI_MOSI, AF::AF0},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM15, PinFunction::Timer_CH2, AF::AF1},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM1, PinFunction::Timer_CH3N, AF::AF2},
            PinOption{IOPin{IOPin::PortB, 15}, Peripheral::TIM15, PinFunction::Timer_CH1N, AF::AF3},

        };
        for (const auto pin_option : pin_options) {
            if (pin_option.pin == pin && pin_option.peripheral == peripheral && pin_option.function == pin_function) {
                return pin_option.af;
            }
        }
        return AF::Invalid;
    }

 public:
    template <Peripheral peripheral, IOPin sda_pin, IOPin scl_pin>
    static void route_I2C(stm32f0xx::GPIO::PullType sda_pull = GPIO::PullType::NoPull, stm32f0xx::GPIO::PullType scl_pull = GPIO::PullType::NoPull) {
        static_assert(is_i2c(peripheral), "You need to pass I2C peripheral.");

        constexpr AlternateFunction sda_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SDA, sda_pin);
        constexpr AlternateFunction scl_alternate_function = get_alternate_function(peripheral, PinFunction::I2C_SCL, scl_pin);

        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SDA' can be connected to: PortB.7 or PortB.9");
        static_assert(sda_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C2,
                      "I2C2 pin 'SDA' can be connected only to: PortB.11");

        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C1,
                      "I2C1 pin 'SCL' can be connected to: PortB.6 or PortB.8");
        static_assert(scl_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::I2C2,
                      "I2C2 pin 'SCL' can be connected only to: PortB.10");

        GPIO gpio_sda(sda_pin);
        gpio_sda.setAlternateFunction(sda_alternate_function, sda_pull, GPIO::OutputType::OpenDrain);
        GPIO gpio_scl(scl_pin);
        gpio_scl.setAlternateFunction(scl_alternate_function, scl_pull, GPIO::OutputType::OpenDrain);
    }

    template <Peripheral peripheral, IOPinOrUnused rxd_pin, IOPinOrUnused txd_pin>
    static void route_SerialPort(stm32f0xx::GPIO::PullType rxd_pull = GPIO::PullType::NoPull,
                                 stm32f0xx::GPIO::OutputType txd_type = GPIO::OutputType::PushPull,
                                 stm32f0xx::GPIO::PullType txd_pull = GPIO::PullType::NoPull) {
        using AlternateFunction = GPIO::AlternateFunction;
        static_assert(is_serial_port(peripheral), "You need to pass SerialPort peripheral.");
        if constexpr (rxd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_RXD, rxd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'RX' can be connected to: PortA.10 or PortB.7");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'RX' can be connected to: PortA.3 or PortA.15");

            GPIO gpio_rxd(rxd_pin.getIOPin());
            gpio_rxd.setAlternateFunction(alternate_function, rxd_pull);
        }
        if constexpr (txd_pin != IOPinOrUnused::Unused) {
            constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, PinFunction::SerialPort_TXD, txd_pin.getIOPin());

            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort1,
                          "Serial Port 1, pin 'TX' can be connected to: PortA.9 or PortB.6");
            static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SerialPort2,
                          "Serial Port 2, pin 'TX' can be connected to: PortA.2 or PortA.14");

            GPIO gpio_txd(txd_pin.getIOPin());
            gpio_txd.setAlternateFunction(alternate_function, txd_pull, txd_type);
        }
    }

    template <Peripheral peripheral, IOPinOrUnused miso_pin, IOPinOrUnused mosi_pin, IOPin sck_pin>
    static void route_SPI(stm32f0xx::GPIO::PullType miso_pull = GPIO::PullType::NoPull,
                          stm32f0xx::GPIO::OutputType mosi_type = GPIO::OutputType::PushPull) {
        static_assert(is_spi(peripheral), "You need to pass SPI peripheral.");

        constexpr AlternateFunction miso_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MISO, miso_pin.getIOPin());
        constexpr AlternateFunction mosi_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_MOSI, mosi_pin.getIOPin());
        constexpr AlternateFunction sck_alternate_function = get_alternate_function(peripheral, PinFunction::SPI_SCK, sck_pin);

        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MISO' can be connected to: PortA.6 or PortB.4");
        static_assert(miso_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'MISO' can be connected only to: PortB.14");

        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'MOSI' can be connected to: PortA.7 or PortB.5");
        static_assert(mosi_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'MOSI' can be connected only to: PortB.15");

        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI1,
                      "SPI1 pin 'SCK' can be connected to: PortA.5 or PortB.3");
        static_assert(sck_alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::SPI2,
                      "SPI2 pin 'SCK' can be connected only to: PortB.13");

        GPIO gpio_miso(miso_pin.getIOPin());
        gpio_miso.setAlternateFunction(miso_alternate_function, miso_pull);
        GPIO gpio_mosi(mosi_pin.getIOPin());
        gpio_mosi.setAlternateFunction(mosi_alternate_function, GPIO::PullType::NoPull, mosi_type);
        GPIO gpio_sck(sck_pin);
        gpio_sck.setAlternateFunction(sck_alternate_function, GPIO::PullType::NoPull, GPIO::OutputType::PushPull);
    }

    template <Peripheral peripheral, TimerChannel channel, IOPin pin>
    static void route_timer(stm32f0xx::GPIO::PullType pull = GPIO::PullType::NoPull, stm32f0xx::GPIO::OutputType type = GPIO::OutputType::PushPull) {
        static_assert(is_timer(peripheral), "You need to pass Timer peripheral.");

        constexpr PinFunction pinFunction = static_cast<PinFunction>(channel);
        constexpr AlternateFunction alternate_function = get_alternate_function(peripheral, pinFunction, pin);

        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH1,
                      "TIM1_CH1 can be connected only to: PortA.8");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH2,
                      "TIM1_CH2 can be connected only to: PortA.9");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH3,
                      "TIM1_CH3 can be connected only to: PortA.10");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM1 || pinFunction != PinFunction::Timer_CH4,
                      "TIM1_CH4 can be connected only to: PortA.11");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH2,
                      "TIM2_CH2 can be connected to: PortA.1 or PortB.3");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH3,
                      "TIM2_CH3 can be connected to: PortA.2 or PortB.10");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM2 || pinFunction != PinFunction::Timer_CH4,
                      "TIM2_CH4 can be connected to: PortA.3 or PortB.11");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH1,
                      "TIM3_CH1 can be connected to: PortA.6 or PortB.4");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH2,
                      "TIM3_CH2 can be connected to: PortA.7 or PortB.5");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH3,
                      "TIM3_CH3 can be connected only to: PortB.0");
        static_assert(alternate_function != AlternateFunction::Invalid || peripheral != Peripheral::TIM3 || pinFunction != PinFunction::Timer_CH4,
                      "TIM3_CH4 can be connected only to: PortB.1");

        GPIO gpio(pin);
        gpio.setAlternateFunction(alternate_function, pull, type);
    }
};

}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_PORTS_IOMANAGER_STM32F051XX_H_
