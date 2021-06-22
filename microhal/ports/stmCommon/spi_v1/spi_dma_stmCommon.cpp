/*
 * SPI_dma_stm32f4xx.cpp
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_dma_stmCommon.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SPI(DMA)                        // Check if driver is enabled in microhal port config

#include <cassert>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
//***********************************************************************************************//
//                                         SPI objects //
//***********************************************************************************************//
#if defined(MICROHAL_USE_SPI1_DMA) && MICROHAL_USE_SPI1_DMA == 1
SPI_dma *SPI_dma::spi1;
#endif
#if defined(MICROHAL_USE_SPI2_DMA) && MICROHAL_USE_SPI2_DMA == 1
SPI_dma *SPI_dma::spi2;
#endif
#if defined(MICROHAL_USE_SPI3_DMA) && MICROHAL_USE_SPI3_DMA == 1
SPI_dma *SPI_dma::spi3;
#endif
#if defined(MICROHAL_USE_SPI4_DMA) && MICROHAL_USE_SPI4_DMA == 1
SPI_dma *SPI_dma::spi4;
#endif
#if defined(MICROHAL_USE_SPI5_DMA) && MICROHAL_USE_SPI5_DMA == 1
SPI_dma *SPI_dma::spi5;
#endif
#if defined(MICROHAL_USE_SPI6_DMA) && MICROHAL_USE_SPI6_DMA == 1
SPI_dma *SPI_dma::spi6;
#endif

//***********************************************************************************************//
//                                         Functions
//***********************************************************************************************//
SPI::Error SPI_dma::write(const void *data, size_t len, bool last) {
    txStream.setMemoryIncrement(DMAMemoryIncrementMode::PointerIncremented);
    txStream.setNumberOfItemsToTransfer(len);
#ifdef _MICROHAL_DMA_HAS_BANKS
    txStream.setMemoryBank0(data);
#else
    txStream.setMemoryAddress(const_cast<void *>(data));
#endif
    txStream.enableInterrupt(DMAInterrupt::TransferComplete);
    txStream.enable();

    auto cr2 = spi.cr2.volatileLoad();
    cr2.TXDMAEN.set();
    spi.cr2.volatileStore(cr2);

    if (semaphore.wait(std::chrono::milliseconds::max())) {
        if (last) {
            busyWait();
        }
        return Error::None;
    }

    return Error::Timeout;
}

SPI::Error SPI_dma::writeRead(const void *writePtr, void *readPtr, size_t writeLen, size_t readLen) {
    // configure tx
    txStream.disableInterrupt(DMAInterrupt::TransferComplete);
    if (writeLen < readLen) {
        writeLen = readLen;
        txStream.setMemoryIncrement(DMAMemoryIncrementMode::PointerFixed);
    } else {
        txStream.setMemoryIncrement(DMAMemoryIncrementMode::PointerIncremented);
    }
    txStream.setNumberOfItemsToTransfer(writeLen);
#ifdef _MICROHAL_DMA_HAS_BANKS
    txStream.setMemoryBank0(writePtr);
#else
    txStream.setMemoryAddress(const_cast<void *>(writePtr));
#endif

    // configure rx
    rxStream.setNumberOfItemsToTransfer(readLen);
#ifdef _MICROHAL_DMA_HAS_BANKS
    rxStream.setMemoryBank0(readPtr);
#else
    rxStream.setMemoryAddress(readPtr);
#endif
    rxStream.enableInterrupt(DMAInterrupt::TransferComplete);

    // clear RX FIFO
    busyWait();
// drop all previously received data
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    while (spi.sr.volatileLoad().FRLVL.get()) {
        [[maybe_unused]] volatile uint32_t unused = spi.dr.volatileLoad_8bit();
    }
#else
    spi.dr.volatileLoad();
#endif

    // 1 enable DMA Rx buffer in CR2 register
    rxStream.enable();
    // 2 enable DMA streams
    {
        auto cr2 = spi.cr2.volatileLoad();
        cr2.RXDMAEN.set();
        spi.cr2.volatileStore(cr2);
    }
    txStream.enable();

    // 3 enable DMA Tx buffer in CR2 register
    {
        auto cr2 = spi.cr2.volatileLoad();
        cr2.TXDMAEN.set();
        spi.cr2.volatileStore(cr2);
    }

    if (semaphore.wait(std::chrono::milliseconds::max())) {
        rxStream.disableInterrupt(DMAInterrupt::TransferComplete);
        return Error::None;
    }
    rxStream.disableInterrupt(DMAInterrupt::TransferComplete);
    return Error::Timeout;
}

void SPI_dma::initialize() {
    dma.clockEnable();
    // rx
    rxStream.deinit();
#ifdef _MICROHAL_DMA_HAS_CHANNELS
    rxStream.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                  DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
#else
    rxStream.init(dma.channel(rxStream, &spi), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMAMemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::PerToMem);
#endif
    rxStream.setPeripheralAddress(&spi.dr);
    // tx
    txStream.deinit();

#ifdef _MICROHAL_DMA_HAS_CHANNELS
    txStream.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                  DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::MemToPer);
#else
    txStream.init(dma.channel(txStream, &spi), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMAMemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::MemToPer);
#endif
    txStream.setPeripheralAddress(&spi.dr);

    dma.clearInterruptFlag(rxStream, DMAInterrupt::TransferComplete);
    dma.enableInterrupt(rxStream, 6);

    dma.clearInterruptFlag(txStream, DMAInterrupt::TransferComplete);
    dma.enableInterrupt(txStream, 6);
}

//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void SPI_dma::IRQfunction(SPI_dma &object, registers::SPI *spi) {
    const SPI::Error error = SPI::errorCheck(spi->sr.volatileLoad());

    if (error != SPI::Error::None) {
        bool shouldYeld = object.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR(shouldYeld);
#else
        (void)shouldYeld;
#endif
    }
}

void SPI_dma::DMA_rxFunction(SPI_dma &object) {
    object.txStream.disable();
    object.rxStream.disable();
    auto cr2 = object.spi.cr2.volatileLoad();
    cr2.RXDMAEN.clear();
    cr2.TXDMAEN.clear();
    object.spi.cr2.volatileStore(cr2);
    [[maybe_unused]] bool shouldYeld = object.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}

void SPI_dma::DMA_txFunction(SPI_dma &object) {
    object.txStream.disable();
    auto cr2 = object.spi.cr2.volatileLoad();
    cr2.TXDMAEN.clear();
    object.spi.cr2.volatileStore(cr2);
    [[maybe_unused]] bool shouldYeld = object.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}

//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#define GLUE3(x, y, z) x##y##z
#define GLUE(x, y) x##y

#define ENABLE_IF_LESS_4(c, v) ENABLE_IF_L_##c(v)
#define ENABLE_IF_MORE_4(c, v) ENABLE_IF_M_##c(v)
#define ENABLE_IF_L_0(x) x
#define ENABLE_IF_L_1(x) x
#define ENABLE_IF_L_2(x) x
#define ENABLE_IF_L_3(x) x
#define ENABLE_IF_L_4(x)
#define ENABLE_IF_L_5(x)
#define ENABLE_IF_L_6(x)
#define ENABLE_IF_L_7(x)

#define ENABLE_IF_M_0(x)
#define ENABLE_IF_M_1(x)
#define ENABLE_IF_M_2(x)
#define ENABLE_IF_M_3(x)
#define ENABLE_IF_M_4(x) x
#define ENABLE_IF_M_5(x) x
#define ENABLE_IF_M_6(x) x
#define ENABLE_IF_M_7(x) x

#define DMA1_RX_STREAM_IRQHANDLER(s, n)                              \
    void GLUE3(DMA1_Stream, n, _IRQHandler)(void) {                  \
        /* clear DMA Streamx interrupt flag*/                        \
        ENABLE_IF_LESS_4(n, DMA1->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);) \
        ENABLE_IF_MORE_4(n, DMA1->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);) \
        /* disable SPI DMA request*/                                 \
        SPI_dma::DMA_txFunction(*SPI_dma::s);                        \
    }

#define DMA1_TX_STREAM_IRQHANDLER(s, n)                              \
    void GLUE3(DMA1_Stream, n, _IRQHandler)(void) {                  \
        /* clear DMA Streamx interrupt flag*/                        \
        ENABLE_IF_LESS_4(n, DMA1->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);) \
        ENABLE_IF_MORE_4(n, DMA1->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);) \
        /* disable SPI DMA request*/                                 \
        SPI_dma::DMA_txFunction(*SPI_dma::s);                        \
    }

#define DMA2_RX_STREAM_IRQHANDLER(s, n, txStreamNumber)                                        \
    void GLUE3(DMA2_Stream, n, _IRQHandler)(void) {                                            \
        /* clear DMA TX Streamx interrupt flag*/                                               \
        ENABLE_IF_LESS_4(txStreamNumber, DMA2->LIFCR = GLUE(DMA_LIFCR_CTCIF, txStreamNumber);) \
        ENABLE_IF_MORE_4(txStreamNumber, DMA2->HIFCR = GLUE(DMA_HIFCR_CTCIF, txStreamNumber);) \
        /* clear DMA Streamx interrupt flag*/                                                  \
        ENABLE_IF_LESS_4(n, DMA2->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);)                           \
        ENABLE_IF_MORE_4(n, DMA2->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);)                           \
        /* disable SPI DMA request*/                                                           \
        SPI_dma::DMA_txFunction(*SPI_dma::s);                                                  \
    }

#define DMA2_TX_STREAM_IRQHANDLER(s, n)                              \
    void GLUE3(DMA2_Stream, n, _IRQHandler)(void) {                  \
        /* clear DMA Streamx interrupt flag*/                        \
        ENABLE_IF_LESS_4(n, DMA2->LIFCR = GLUE(DMA_LIFCR_CTCIF, n);) \
        ENABLE_IF_MORE_4(n, DMA2->HIFCR = GLUE(DMA_HIFCR_CTCIF, n);) \
        /* disable SPI DMA request*/                                 \
        SPI_dma::DMA_txFunction(*SPI_dma::s);                        \
    }
//------
#define DMA1_RX_CHANNEL_IRQHANDLER(s, rxChannelNum, txChannelNum)                                  \
    void GLUE3(DMA1_Channel, rxChannelNum, _IRQHandler)(void) {                                    \
        /* clear DMA channel interrupt flag*/                                                      \
        DMA::dma1->clearInterruptFlag(rxChannelNum - 1, DMA::Channel::Interrupt::GlobalInterrupt); \
        DMA::dma1->clearInterruptFlag(txChannelNum - 1, DMA::Channel::Interrupt::GlobalInterrupt); \
        /* disable SPI DMA request*/                                                               \
        SPI_dma::DMA_rxFunction(*SPI_dma::s);                                                      \
    }

#define DMA1_TX_CHANNEL_IRQHANDLER(s, n)                                                \
    void GLUE3(DMA1_Channel, n, _IRQHandler)(void) {                                    \
        /* clear DMA Streamx interrupt flag*/                                           \
        DMA::dma1->clearInterruptFlag(n - 1, DMA::Channel::Interrupt::GlobalInterrupt); \
        /* disable SPI DMA request*/                                                    \
        SPI_dma::DMA_txFunction(*SPI_dma::s);                                           \
    }

#define DMA2_RX_CHANNEL_IRQHANDLER(s, n)                                            \
    void GLUE3(DMA2_Channel, n, _IRQHandler)(void) {                                \
        /* clear DMA Streamx interrupt flag*/                                       \
        DMA::dma2->clearInterruptFlag(n, DMA::Channel::Interrupt::GlobalInterrupt); \
        /* disable SPI DMA request*/                                                \
        SPI_dma::DMA_txFunction(*SPI_dma::s);                                       \
    }

#define DMA2_TX_CHANNEL_IRQHANDLER(s, n)                                            \
    void GLUE3(DMA2_Channel, n, _IRQHandler)(void) {                                \
        /* clear DMA Streamx interrupt flag*/                                       \
        DMA::dma2->clearInterruptFlag(n, DMA::Channel::Interrupt::GlobalInterrupt); \
        /* disable SPI DMA request*/                                                \
        SPI_dma::DMA_txFunction(*SPI_dma::s);                                       \
    }
// ---------------------------- spi 1
#if defined(MICROHAL_USE_SPI1_DMA) && MICROHAL_USE_SPI1_DMA == 1
extern "C" void SPI1_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi1, registers::spi1);
}
#ifdef _MICROHAL_DMA2_BASE_ADDRESS
DMA2_RX_STREAM_IRQHANDLER(spi1, MICROHAL_SPI1_DMA_RX_STREAM, MICROHAL_SPI1_DMA_TX_STREAM)
DMA2_TX_STREAM_IRQHANDLER(spi1, MICROHAL_SPI1_DMA_TX_STREAM)
#else   // _MICROHAL_DMA2_BASE_ADDRESS
DMA1_RX_CHANNEL_IRQHANDLER(spi1, MICROHAL_SPI1_DMA_RX_STREAM, MICROHAL_SPI1_DMA_TX_STREAM)
DMA1_TX_CHANNEL_IRQHANDLER(spi1, MICROHAL_SPI1_DMA_TX_STREAM)
#endif  // _MICROHAL_DMA2_BASE_ADDRESS
#endif  // defined(MICROHAL_USE_SPI1_DMA) && MICROHAL_USE_SPI1_DMA == 1
// ---------------------------- spi 2
#if defined(MICROHAL_USE_SPI2_DMA) && MICROHAL_USE_SPI2_DMA == 1
void SPI2_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi2, registers::spi2);
}
#ifdef _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_CHANNEL_IRQHANDLER(spi2, MICROHAL_SPI2_DMA_RX_STREAM, MICROHAL_SPI2_DMA_TX_STREAM)
DMA1_TX_CHANNEL_IRQHANDLER(spi2, MICROHAL_SPI2_DMA_TX_STREAM)
#else   // _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_STREAM_IRQHANDLER(spi2, MICROHAL_SPI2_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(spi2, MICROHAL_SPI2_DMA_TX_STREAM)
#endif  //_MICROHAL_DMA_HAS_CHANNELS
#endif  // defined(MICROHAL_USE_SPI2_DMA) && MICROHAL_USE_SPI2_DMA == 1
// ---------------------------- spi 3
#if defined(MICROHAL_USE_SPI3_DMA) && MICROHAL_USE_SPI3_DMA == 1
void SPI3_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi3, registers::spi3);
}
#ifdef _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_CHANNEL_IRQHANDLER(spi3, MICROHAL_SPI3_DMA_RX_STREAM, MICROHAL_SPI3_DMA_TX_STREAM)
DMA1_TX_CHANNEL_IRQHANDLER(spi3, MICROHAL_SPI3_DMA_TX_STREAM)
#else   // _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_STREAM_IRQHANDLER(spi3, MICROHAL_SPI3_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(spi3, MICROHAL_SPI3_DMA_TX_STREAM)
#endif  // _MICROHAL_DMA_HAS_CHANNELS
#endif
// ---------------------------- spi 4
#if defined(MICROHAL_USE_SPI4_DMA) && MICROHAL_USE_SPI4_DMA == 1
void SPI4_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi4, registers::spi4);
}
#ifdef _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_CHANNEL_IRQHANDLER(spi4, MICROHAL_SPI4_DMA_RX_STREAM, MICROHAL_SPI4_DMA_TX_STREAM)
DMA1_TX_CHANNEL_IRQHANDLER(spi4, MICROHAL_SPI4_DMA_TX_STREAM)
#else   // _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_STREAM_IRQHANDLER(spi4, MICROHAL_SPI4_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(spi4, MICROHAL_SPI4_DMA_TX_STREAM)
#endif  // _MICROHAL_DMA_HAS_CHANNELS
#endif
// ---------------------------- spi 5
#if defined(MICROHAL_USE_SPI5_DMA) && MICROHAL_USE_SPI5_DMA == 1
void SPI5_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi5, registers::spi5);
}
#ifdef _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_CHANNEL_IRQHANDLER(spi5, MICROHAL_SPI5_DMA_RX_STREAM, MICROHAL_SPI5_DMA_TX_STREAM)
DMA1_TX_CHANNEL_IRQHANDLER(spi5, MICROHAL_SPI5_DMA_TX_STREAM)
#else   // _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_STREAM_IRQHANDLER(spi5, MICROHAL_SPI5_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(spi5, MICROHAL_SPI5_DMA_TX_STREAM)
#endif  // _MICROHAL_DMA_HAS_CHANNELS
#endif
// ---------------------------- spi 6
#if defined(MICROHAL_USE_SPI6_DMA) && MICROHAL_USE_SPI6_DMA == 1
void SPI6_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi6, registers::spi6);
}
#ifdef _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_CHANNEL_IRQHANDLER(spi6, MICROHAL_SPI6_DMA_RX_STREAM, MICROHAL_SPI6_DMA_TX_STREAM)
DMA1_TX_CHANNEL_IRQHANDLER(spi6, MICROHAL_SPI6_DMA_TX_STREAM)
#else   // _MICROHAL_DMA_HAS_CHANNELS
DMA1_RX_STREAM_IRQHANDLER(spi6, MICROHAL_SPI6_DMA_RX_STREAM)
DMA1_TX_STREAM_IRQHANDLER(spi6, MICROHAL_SPI6_DMA_TX_STREAM)
#endif  // _MICROHAL_DMA_HAS_CHANNELS
#endif
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SPI(INTERRUPT)
#endif  // _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1
