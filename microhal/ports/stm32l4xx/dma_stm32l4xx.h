/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 08-10-2016
 * last modification: 08-10-2016
 *
 * @copyright Copyright (c) 2016, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_DMA_STM32L4XX_H_
#define _MICROHAL_DMA_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <stddef.h>
#include "device/stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
namespace DMA {
/* **************************************************************************************************************************************************
 * CLASS
 */
class Channel{
	DMA_Channel_TypeDef    channel;
public:

	enum class Request : uint8_t {
		Request0 = 0,
		Request1 = 1,
		Request2 = 2,
		Request3 = 3,
		Request4 = 4,
		Request5 = 5,
		Request6 = 6,
		Request7 = 7
	};

	enum class Priority : uint32_t {
		Low = 0x00000000,
		Medium = DMA_CCR_PL_0,
		High = DMA_CCR_PL_1,
		VeryHigh = DMA_CCR_PL_0 | DMA_CCR_PL_1
	};

	enum class MemoryDataSize : uint32_t {
		Byte = 0x00000000,
		HalfWord = DMA_CCR_MSIZE_0,
		Word = DMA_CCR_MSIZE_1
	};

	enum class PeripheralDataSize: uint32_t {
		Byte = 0x00,
		HalfWord = DMA_CCR_PSIZE_0,
		Word = DMA_CCR_PSIZE_1
	};

	enum class MemoryIncrementMode : uint32_t {
		PointerFixed = 0x00000000,
		PointerIncremented = DMA_CCR_MINC
	};

	enum class PeripheralIncrementMode : uint32_t {
		PointerFixed = 0x00000000,
		PointerIncremented = DMA_CCR_PINC
	};

	enum class TransmisionDirection : uint32_t {
		PerToMem = 0x00,
		MemToPer = DMA_CCR_DIR,
		MemToMem = DMA_CCR_MEM2MEM
	};

	enum class Interrupt : uint32_t {
		TransferComplete = DMA_CCR_TCIE,
		HalfTransferComplete = DMA_CCR_HTIE,
		TransferError = DMA_CCR_TEIE,
	};

	void init(MemoryDataSize memSize, PeripheralDataSize peripheralSize, MemoryIncrementMode memoryInc,
			  PeripheralIncrementMode peripheralInc, TransmisionDirection direction)
	{
		channel.CCR =  static_cast<uint32_t>(memSize) | static_cast<uint32_t>(peripheralSize)
				| static_cast<uint32_t>(memoryInc) | static_cast<uint32_t>(peripheralInc) |	static_cast<uint32_t>(direction);
	}

	void deinit(){
		channel.CCR = 0;
	}

	void setMemoryIncrement(MemoryIncrementMode memoryInc) {
		channel.CCR = (channel.CCR & ~static_cast<uint32_t>(MemoryIncrementMode::PointerIncremented)) | static_cast<uint32_t>(memoryInc);

	}

	void setPriority(Priority priority){
		channel.CCR = (channel.CCR & ~DMA_CCR_PL) | static_cast<uint32_t>(priority);
	}

	void setPeripheralAddress(volatile void *addr){
		channel.CPAR = reinterpret_cast<uint32_t>(addr);
	}
	void setMemory(const void *memoryAddr){
		channel.CMAR = reinterpret_cast<uint32_t>(memoryAddr);
	}

	void setNumberOfItemsToTransfer(size_t len){
		channel.CNDTR = len;
	}
	void enableInterrupt(Interrupt interrupts){
		channel.CCR |= static_cast<uint32_t>(interrupts);
	}
	void disableInterrupt(Interrupt interrupts){
		channel.CCR &= ~(static_cast<uint32_t>(interrupts));
	}
	void enable(){
		channel.CCR |= DMA_CCR_EN;
	}
	void disable(){
		channel.CCR &= ~DMA_CCR_EN;
	}
};

class DMA{
	DMA_TypeDef dma;
public:
	Channel channel[7];
	void clearInterruptFlag(Channel &channel, Channel::Interrupt interrupt){
		uint8_t channelNumber = calculateChannelNumber(channel);
		uint32_t flag;

		switch(interrupt)
		{
		case Channel::Interrupt::TransferComplete:
			flag = 1;
			break;
		case Channel::Interrupt::HalfTransferComplete:
			flag = 2;
			break;
		case Channel::Interrupt::TransferError:
			flag = 3;
			break;
		}

		flag += (4 * channelNumber);
		dma.IFCR = flag;
	}

	void init(Channel &channel, Channel::Request request)
	{
		DMA_request_TypeDef * req = (DMA_request_TypeDef *)(this + 0x00A8U);         // todo add bitband
		req->CSELR &=~ DMA_CSELR_C1S << (4 * calculateChannelNumber(channel));      // todo add bitbang
		req->CSELR |= (uint32_t)((uint32_t)request << (4 * calculateChannelNumber(channel))); // todo add bitband
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
	uint8_t calculateChannelNumber(Channel &channel){
		return (&channel - &this->channel[0]) / sizeof(Channel);
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
} // namespace stm32l4xx
} // namespace microhal

#endif  // _MICROHAL_DMA_STM32L4XX_H_
