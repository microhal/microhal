/*
 * dataBus.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef DATABUS_H_
#define DATABUS_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "gpio/gpio_interface.h"
#include "interfaces/dataBus_interface.h"

namespace microhal {

class GenericDataBus : public DataBus {
 public:
    GenericDataBus(microhal::GPIO &wr, microhal::GPIO &rd, microhal::GPIO &data0, microhal::GPIO &data1, microhal::GPIO &data2, microhal::GPIO &data3,
                   microhal::GPIO &data4, microhal::GPIO &data5, microhal::GPIO &data6, microhal::GPIO &data7)
        : wr(wr), rd(rd), data0(data0), data1(data1), data2(data2), data3(data3), data4(data4), data5(data5), data6(data6), data7(data7) {
        wr.set();
        rd.set();
    }

    void write(uint16_t data) final {
        setDirectionOutput();
        write8(data >> 8);
        write8(data & 0xFF);
    }

    uint16_t read() final {
        setDirectionInput();
        uint16_t data = read8() << 8;
        data |= read8();
        return data;
    }

 private:
    microhal::GPIO &wr;  // write strobe
    microhal::GPIO &rd;  // read strobe
    microhal::GPIO &data0;
    microhal::GPIO &data1;
    microhal::GPIO &data2;
    microhal::GPIO &data3;
    microhal::GPIO &data4;
    microhal::GPIO &data5;
    microhal::GPIO &data6;
    microhal::GPIO &data7;

    void delay() {
        volatile uint8_t i = 20;
        while (i--) {
        }
    }

    void writeStrobe() {
        wr.reset();
        delay();
        wr.set();
    }

    void setDirectionOutput() {
        data0.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
        data1.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
        data2.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
        data3.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
        data4.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
        data5.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
        data6.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
        data7.configureAsOutput(microhal::GPIO::OutputType::PushPull, microhal::GPIO::NoPull);
    }

    void setDirectionInput() {
        data0.configureAsInput(microhal::GPIO::NoPull);
        data1.configureAsInput(microhal::GPIO::NoPull);
        data2.configureAsInput(microhal::GPIO::NoPull);
        data3.configureAsInput(microhal::GPIO::NoPull);
        data4.configureAsInput(microhal::GPIO::NoPull);
        data5.configureAsInput(microhal::GPIO::NoPull);
        data6.configureAsInput(microhal::GPIO::NoPull);
        data7.configureAsInput(microhal::GPIO::NoPull);
    }

    void write8(uint8_t data) {
        data0 = (data & (1 << 0));
        data1 = (data & (1 << 1));
        data2 = (data & (1 << 2));
        data3 = (data & (1 << 3));
        data4 = (data & (1 << 4));
        data5 = (data & (1 << 5));
        data6 = (data & (1 << 6));
        data7 = (data & (1 << 7));
        writeStrobe();
    }

    uint8_t read8() {
        rd.reset();
        std::this_thread::sleep_for(std::chrono::milliseconds{1});

        uint8_t data = data0 ? 1 << 0 : 0;
        data |= data1 ? 1 << 1 : 0;
        data |= data2 ? 1 << 2 : 0;
        data |= data3 ? 1 << 3 : 0;
        data |= data4 ? 1 << 4 : 0;
        data |= data5 ? 1 << 5 : 0;
        data |= data6 ? 1 << 6 : 0;
        data |= data7 ? 1 << 7 : 0;
        rd.set();
        return data;
    }
};
}  // namespace microhal

#endif /* DATABUS_H_ */
