/*
 * serialPort_dma_stm32f3xx.h
 *
 *  Created on: Aug 20, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STM32F3XX_SERIALPORT_DMA_STM32F3XX_H_
#define _MICROHAL_PORTS_STM32F3XX_SERIALPORT_DMA_STM32F3XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "../dma_stm32f3xx.h"
#include "../serialPort_stm32f3xx.h"
#include "buffers/cyclicBuffer.h"
#include "microhal_semaphore.h"
#include "ports/common/serialPort_bufferedBase.h"

namespace microhal {
namespace stm32f3xx {
extern "C" {
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class SerialPort_Dma : public common::SerialPort_BufferedBase<SerialPort_Dma> {
 public:
#ifdef MICROHAL_USE_SERIAL_PORT1_DMA
    static SerialPort_Dma Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_DMA
    static SerialPort_Dma Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_DMA
    static SerialPort_Dma Serial3;
#endif
    //--------------------------------------------- functions ---------------------------------------//
    bool open(OpenMode mode) noexcept;
    /**
     *
     * @return
     */
    size_t availableBytes() const noexcept final {
        size_t receivedBytes = rxTransferInProgress - rxStream.getNumberOfItemsInTransfer();
        return rxBuffer.getLength() + receivedBytes;
    }

 private:
    //------------------------------------------- variables -----------------------------------------//
    size_t transferInProgress = 0;
    size_t rxTransferInProgress = 0;
    DMA::Channel &txStream;
    DMA::Channel &rxStream;
    //------------------------------------------- constructors --------------------------------------//
    inline SerialPort_Dma(USART_TypeDef &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize, DMA::Channel &txStream,
                          DMA::Channel &rxStream);

    //--------------------------------------------- functions ---------------------------------------//
    void updateRxBuffer_impl() {
        __disable_irq();
        size_t receivedBytes = rxTransferInProgress - rxStream.getNumberOfItemsInTransfer();
        rxTransferInProgress -= receivedBytes;
        rxBuffer.updateWritePointer(receivedBytes);
        __enable_irq();
    }

    void startTransmission_impl() {
        if (transferInProgress == 0) {
            prepareDmaTransfer();
        }
    }
    void configureRxWait_impl(size_t bytesToWait) {
        waitForBytes = bytesToWait;
        if (rxTransferInProgress > bytesToWait) {
            rxStream.disable();
        }
    }

    size_t prepareDmaTransfer();
    void prepareRxDmaTransfer(size_t bytesToReceive = 0x10000);
    //------------------------------------------- friends -------------------------------------------//
    friend SerialPort_BufferedBase<SerialPort_Dma>;

    friend inline void serialPort_interruptFunction(USART_TypeDef *const usart, SerialPort_Dma &serial);
    friend inline void DMA_rx_function(SerialPort_Dma &serial, uint32_t DMA_Stream_NDTR);
    friend inline void DMA_tx_function(SerialPort_Dma &serial);
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STM32F3XX_SERIALPORT_DMA_STM32F3XX_H_ */
