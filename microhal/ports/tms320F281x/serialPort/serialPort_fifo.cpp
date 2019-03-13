#include "serialPort_fifo.h"
#include "DSP281x_Device.h"     // DSP281x Headerfile Include File

namespace microhal {
namespace tms320f281x {

size_t min(size_t a, size_t b) {
    if (a > b)
        return b;
    else
        return a;
}

void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x204f;
    SciaRegs.SCIFFCT.all=0x0;
}

bool SerialPort_FIFO::open(microhal::IODevice::OpenMode a) {
    scia_fifo_init();

    SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all =0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA =1;
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;

    SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

size_t SerialPort_FIFO::read(char *data, size_t sizeToRead, std::chrono::milliseconds) {
    size_t toRead = min(sizeToRead, inputQueueSize());
    if (availableBytes() >= toRead) {
        for (size_t i = 0; i < toRead; i++) {
            data[i] = SciaRegs.SCIRXBUF.bit.RXDT;
        }
    }
    return toRead;
}

size_t SerialPort_FIFO::inputQueueSize() const {
    return 16; // RX FIFO depth
}

size_t SerialPort_FIFO::outputQueueSize() const {
    return 16; // TX FIFO depth
}

size_t SerialPort_FIFO::availableBytes() const {
 return SciaRegs.SCIFFRX.bit.RXFIFST;
}

bool SerialPort_FIFO::waitForWriteFinish(std::chrono::milliseconds timeout) const {
    while(SciaRegs.SCIFFTX.bit.TXFFST){
        // todo add timeout
    }
    return true;
}

size_t SerialPort_FIFO::write(const char *a, size_t size) {
    size_t toWrite = size;
    while (toWrite--){
        while (SciaRegs.SCIFFTX.bit.TXFFST == 16) {}
        SciaRegs.SCITXBUF=*a++;
    }
    return size - toWrite;
}

bool SerialPort_FIFO::clear(microhal::SerialPort::Direction direction) {
    if ((direction == SerialPort::Input) || (direction == SerialPort::AllDirections)) {
        SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;
        SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
    }
    if ((direction == SerialPort::Output) || (direction == SerialPort::AllDirections)) {
        SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 0;
        SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
    }
}

} // namespace microhal
} // namespace tms320f281x
