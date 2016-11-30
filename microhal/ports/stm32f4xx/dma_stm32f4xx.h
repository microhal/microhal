/*
 * dma_stm32f4xx.h
 *
 *  Created on: 4 kwi 2015
 *      Author: Pawel
 */

#ifndef DMA_STM32F4XX_H_
#define DMA_STM32F4XX_H_

#include <stddef.h>
#include "stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
namespace DMA {
class Stream {
	DMA_Stream_TypeDef stream;
public:
	typedef enum {
		Channel0 = 0,
		Channel1 = DMA_SxCR_CHSEL_0,
		Channel2 = DMA_SxCR_CHSEL_1,
		Channel3 = DMA_SxCR_CHSEL_1 | DMA_SxCR_CHSEL_0,
		Channel4 = DMA_SxCR_CHSEL_2,
		Channel5 = DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_0,
		Channel6 = DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_1,
		Channel7 = DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_1 | DMA_SxCR_CHSEL_0,
	}Channel;

	enum class MemoryBurst : uint32_t {
		SingleTransfer = 0x00,
		IncrementalOf_4Beats = DMA_SxCR_MBURST_0,
		IncrementalOf_8Beats = DMA_SxCR_MBURST_1,
		IncrementalOf_16Beats = DMA_SxCR_MBURST_1 | DMA_SxCR_MBURST_0
	};

	enum class PeripheralBurst : uint32_t {
		SingleTransfer = 0x00,
		IncrementalOf_4Beats = DMA_SxCR_PBURST_0,
		IncrementalOf_8Beats = DMA_SxCR_PBURST_1,
		IncrementalOf_16Beats = DMA_SxCR_PBURST_1 | DMA_SxCR_PBURST_0
	};

	enum class MemoryTarget : uint32_t {
		Memory0 = 0x0000,
		Memory1 = DMA_SxCR_CT
	};

	enum class Priority : uint32_t {
		Low = 0x00000000,
		Medium = DMA_SxCR_PL_0,
		High = DMA_SxCR_PL_1,
		VeryHigh = DMA_SxCR_PL_1 | DMA_SxCR_PL_0
	};

	enum class MemoryDataSize : uint32_t {
		Byte = 0x00000000,
		HalfWord = DMA_SxCR_MSIZE_0,
		Word = DMA_SxCR_MSIZE_1
	};

	enum class PeripheralDataSize: uint32_t {
		Byte = 0x00,
		HalfWord = DMA_SxCR_PSIZE_0,
		Word = DMA_SxCR_PSIZE_1
	};

	enum class MemoryIncrementMode : uint32_t {
		PointerFixed = 0x00000000,
		PointerIncremented = DMA_SxCR_MINC
	};

	enum class PeripheralIncrementMode : uint32_t {
		PointerFixed = 0x00000000,
		PointerIncremented = DMA_SxCR_PINC
	};

	enum class TransmisionDirection : uint32_t {
		PerToMem = 0x00,
		MemToPer = DMA_SxCR_DIR_0,
		MemToMem = DMA_SxCR_DIR_0 | DMA_SxCR_DIR_1
	};

	enum class Interrupt : uint32_t {
		TransferComplete = DMA_SxCR_TCIE,
		HalfTransferComplete = DMA_SxCR_HTIE,
		TransferError = DMA_SxCR_TEIE,
		DirectModeError = DMA_SxCR_DMEIE
	};

	void init(Channel channel, MemoryBurst memBurst, PeripheralBurst peripheralBurst, MemoryDataSize memSize, PeripheralDataSize peripheralSize,
			MemoryIncrementMode memoryInc, PeripheralIncrementMode peripheralInc, TransmisionDirection direction){
		stream.CR = static_cast<uint32_t>(channel) | static_cast<uint32_t>(memBurst) | static_cast<uint32_t>(peripheralBurst) |
				static_cast<uint32_t>(memSize) | static_cast<uint32_t>(peripheralSize) | static_cast<uint32_t>(memoryInc) |
				static_cast<uint32_t>(peripheralInc) |	static_cast<uint32_t>(direction);
	}

	void deinit(){
		stream.CR = 0;
	}

	void setMemoryIncrement(MemoryIncrementMode memoryInc) {
		stream.CR = (stream.CR & ~static_cast<uint32_t>(MemoryIncrementMode::PointerIncremented)) | static_cast<uint32_t>(memoryInc);

	}

	void setPriority(Priority priority){
		stream.CR = (stream.CR & ~DMA_SxCR_PL) | static_cast<uint32_t>(priority);
	}

	void setPeripheralAddress(volatile void *addr){
		stream.PAR = reinterpret_cast<uint32_t>(addr);
	}
	void setMemoryBank0(const void *memoryAddr){
		stream.M0AR = reinterpret_cast<uint32_t>(memoryAddr);
	}
	void setMemoryBank1(const void *memoryAddr){
		stream.M1AR = reinterpret_cast<uint32_t>(memoryAddr);
	}

	void setNumberOfItemsToTransfer(size_t len){
		stream.NDTR = len;
	}
	size_t getNumberOfItemsInTransfer() {
		return stream.NDTR;
	}
	void enableInterrupt(Interrupt interrupts){
		stream.CR |= static_cast<uint32_t>(interrupts);
	}
	void disableInterrupt(Interrupt interrupts){
		stream.CR &= ~(static_cast<uint32_t>(interrupts));
	}
	void enable(){
		stream.CR |= DMA_SxCR_EN;
	}
	void disable(){
		stream.CR &= ~DMA_SxCR_EN;
	}
};

class DMA {
	DMA_TypeDef dma;
public:
	Stream stream[8];
	void clearInterruptFlag(Stream &stream, Stream::Interrupt interrupt){
		auto streamNumber = calculateStreamNumber(stream);
		uint32_t flag;
		if(streamNumber % 2 == 0){
			flag = static_cast<uint32_t>(interrupt) << 1;
		} else {
			flag = static_cast<uint32_t>(interrupt) << 7;
		}

		if(streamNumber <= 3){
			dma.LIFCR = flag;
		}else{
			dma.HIFCR = flag;
		}
	}

	void enableInterrupt(const Stream& stream, uint32_t priority) noexcept {
		IRQn irq[2][8] = { { DMA1_Stream0_IRQn, DMA1_Stream1_IRQn, DMA1_Stream2_IRQn, DMA1_Stream3_IRQn, DMA1_Stream4_IRQn, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn, DMA1_Stream7_IRQn},
					       { DMA2_Stream0_IRQn, DMA2_Stream1_IRQn, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn, DMA2_Stream4_IRQn, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn, DMA2_Stream7_IRQn}};
		IRQn interruptNumber = irq[dmaNumber() - 1][calculateStreamNumber(stream)];

		NVIC_EnableIRQ(interruptNumber);
		NVIC_SetPriority(interruptNumber, priority);
	}

	void disableInterrupt(const Stream& stream) {
		IRQn irq[2][8] = { { DMA1_Stream0_IRQn, DMA1_Stream1_IRQn, DMA1_Stream2_IRQn, DMA1_Stream3_IRQn, DMA1_Stream4_IRQn, DMA1_Stream5_IRQn, DMA1_Stream6_IRQn, DMA1_Stream7_IRQn},
					       { DMA2_Stream0_IRQn, DMA2_Stream1_IRQn, DMA2_Stream2_IRQn, DMA2_Stream3_IRQn, DMA2_Stream4_IRQn, DMA2_Stream5_IRQn, DMA2_Stream6_IRQn, DMA2_Stream7_IRQn}};
		IRQn interruptNumber = irq[dmaNumber() - 1][calculateStreamNumber(stream)];

		NVIC_DisableIRQ(interruptNumber);
	}

	void clockEnable(){
		if(this == reinterpret_cast<DMA*>(DMA1))
			RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		else
			RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	}
	void clockDisable(){
		if(this == reinterpret_cast<DMA*>(DMA1))
			RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
		else
			RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA2EN;
	}

	Stream::Channel channel(const Stream &stream, const USART_TypeDef &devicePtr) {
		if(this == reinterpret_cast<DMA*>(DMA1)) return DMA1Channel(stream, devicePtr);
		if(this == reinterpret_cast<DMA*>(DMA2)) return DMA2Channel(stream, devicePtr);
		while(1);
	}

//	Stream::Channel getChannel(Stream &stream, void *devicePtr){
//		if(this == reinterpret_cast<DMA*>(DMA1)){
//			switch(calculateStreamNumber(stream)){
//			case 0: return getChannelForDma1Stream0(devicePtr);
//			case 1: return getChannelForDma1Stream1(devicePtr);
//			case 2: return getChannelForDma1Stream2(devicePtr);
//			case 3: return getChannelForDma1Stream3(devicePtr);
//			case 4: return getChannelForDma1Stream4(devicePtr);
//			case 5: return getChannelForDma1Stream5(devicePtr);
//			case 6: return getChannelForDma1Stream6(devicePtr);
//			case 7: return getChannelForDma1Stream7(devicePtr);
//			}
//		} else {
//			switch(calculateStreamNumber(stream)){
//			case 0: return getChannelForDma2Stream0(devicePtr);
//			case 1: return getChannelForDma2Stream1(devicePtr);
//			case 2: return getChannelForDma2Stream2(devicePtr);
//			case 3: return getChannelForDma2Stream3(devicePtr);
//			case 4: return getChannelForDma2Stream4(devicePtr);
//			case 5: return getChannelForDma2Stream5(devicePtr);
//			case 6: return getChannelForDma2Stream6(devicePtr);
//			case 7: return getChannelForDma2Stream7(devicePtr);
//			}
//		}
//	}

private:
	uint32_t calculateStreamNumber(const Stream &stream) const noexcept {
		return &stream - &this->stream[0];
	}

	uint32_t dmaNumber() const noexcept {
		if (this == reinterpret_cast<DMA*>(DMA1)) return 1;
		if (this == reinterpret_cast<DMA*>(DMA2)) return 2;
		while(1);
		return 0;
	}

	Stream::Channel DMA1Channel(const Stream &stream, const USART_TypeDef &devicePtr) noexcept {
		const uint32_t streamNumber = calculateStreamNumber(stream);
		switch (streamNumber) {
		case 0:
			if (&devicePtr == UART5) return Stream::Channel::Channel4;
			if (&devicePtr == UART8) return Stream::Channel::Channel5;
			break;
		case 1:
			if (&devicePtr == USART3) return Stream::Channel::Channel4;
			if (&devicePtr == UART7) return Stream::Channel::Channel5;
			break;
		case 2:
			if (&devicePtr == UART4) return Stream::Channel::Channel4;
			break;
		case 3:
			if (&devicePtr == USART3) return Stream::Channel::Channel4;
			if (&devicePtr == UART7) return Stream::Channel::Channel5;
			break;
		case 4:
			if (&devicePtr == UART4) return Stream::Channel::Channel4;
			if (&devicePtr == USART3) return Stream::Channel::Channel7;
			break;
		case 5:
			if (&devicePtr == USART2) return Stream::Channel::Channel4;
			break;
		case 6:
			if (&devicePtr == USART2) return Stream::Channel::Channel4;
			if (&devicePtr == UART8) return Stream::Channel::Channel5;
			break;
		case 7:
			if (&devicePtr == UART5) return Stream::Channel::Channel4;
			break;
		}
		while(1) {
			volatile auto dbg = streamNumber;
		}
	}

	Stream::Channel DMA2Channel(const Stream &stream, const USART_TypeDef &devicePtr) {
		auto stremNumber = calculateStreamNumber(stream);
		switch (stremNumber) {
		case 1:
			if (&devicePtr == USART6) return Stream::Channel::Channel5;
			break;
		case 2:
			if (&devicePtr == USART1) return Stream::Channel::Channel4;
			if (&devicePtr == USART6) return Stream::Channel::Channel5;
			break;
		case 5:
			if (&devicePtr == USART1) return Stream::Channel::Channel4;
			break;
		case 6:
			if (&devicePtr == USART6) return Stream::Channel::Channel5;
			break;
		case 7:
			if (&devicePtr == USART1) return Stream::Channel::Channel4;
			if (&devicePtr == USART6) return Stream::Channel::Channel5;
			break;
		}
		while(1) {
			volatile auto dbg = stremNumber;
		}
	}

//	Stream::Channel getChannelForDma1Stream0(void *devicePtr);
//	Stream::Channel getChannelForDma1Stream1(void *devicePtr);
//	Stream::Channel getChannelForDma1Stream2(void *devicePtr);
//	Stream::Channel getChannelForDma1Stream3(void *devicePtr);
//	Stream::Channel getChannelForDma1Stream4(void *devicePtr);
//	Stream::Channel getChannelForDma1Stream5(void *devicePtr);
//	Stream::Channel getChannelForDma1Stream6(void *devicePtr);
//	Stream::Channel getChannelForDma1Stream7(void *devicePtr);
//
//	Stream::Channel getChannelForDma2Stream0(void *devicePtr);
//	Stream::Channel getChannelForDma2Stream1(void *devicePtr);
//	Stream::Channel getChannelForDma2Stream2(void *devicePtr);
//	Stream::Channel getChannelForDma2Stream3(void *devicePtr);
//	Stream::Channel getChannelForDma2Stream4(void *devicePtr);
//	Stream::Channel getChannelForDma2Stream5(void *devicePtr);
//	Stream::Channel getChannelForDma2Stream6(void *devicePtr);
//	Stream::Channel getChannelForDma2Stream7(void *devicePtr);
};

constexpr DMA *dma1 = reinterpret_cast<DMA*>(DMA1);
constexpr DMA *dma2 = reinterpret_cast<DMA*>(DMA2);
} // namespace DMA
} // namespace stm32f4xx
} // namespace microhal

#endif /* DMA_STM32F4XX_H_ */
