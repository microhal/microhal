/*
 * dma_stm32f4xx.cpp
 *
 *  Created on: 12 kwi 2015
 *      Author: Pawel
 */



#include "dma_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
namespace DMA {

Stream::Channel DMA::getChannelForDma1Stream0(const void *devicePtr) const {
#if defined(SPI3)
	if (devicePtr == SPI3) return Stream::Channel::Channel0;
#endif
	if (devicePtr == I2C1) return Stream::Channel::Channel1;
#if defined(TIM4)
	if (devicePtr == TIM4) return Stream::Channel::Channel2;
#endif
#if defined(I2Sext)
	if (devicePtr == I2S3ext) return Stream::Channel::Channel3;
#endif
#if defined(UART5)
	if (devicePtr == UART5) return Stream::Channel::Channel4;
#endif
#if defined(UART8)
	if (devicePtr == UART8) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM5) return Stream::Channel::Channel6;
	//if (devicePtr == TIM6) return Stream::Channel::Channel7;
	//if (devicePtr == I2CFMP1) return Stream::Channel::Channel7; stm32f412
	std::terminate();
}

Stream::Channel DMA::getChannelForDma1Stream1(const void *devicePtr) const {
	//if (devicePtr == SPI1) return Stream::Channel::Channel0;
	//if (devicePtr == I2C3) return Stream::Channel::Channel1;
	//if (devicePtr == I2CFMP1) return Stream::Channel::Channel2; stm32f412
	//if (devicePtr == TIM4) return Stream::Channel::Channel2;
#if defined(TIM2)
	if (devicePtr == TIM2) return Stream::Channel::Channel3;
#endif
#if defined(USART3)
	if (devicePtr == USART3) return Stream::Channel::Channel4;
#endif
#if defined(UART7)
	if (devicePtr == UART7) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM5) return Stream::Channel::Channel6;
#if defined(TIM6)
	if (devicePtr == TIM6) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma1Stream2(const void *devicePtr) const {
#if defined(SPI3)
	if (devicePtr == SPI3) return Stream::Channel::Channel0;
#endif
#if defined(TIM7)
	if (devicePtr == TIM7) return Stream::Channel::Channel1;
#endif
#if defined(I2S3ext)
	if (devicePtr == I2S3ext) return Stream::Channel::Channel2;
#endif
#if defined(I2C3)
	if (devicePtr == I2C3) return Stream::Channel::Channel3;
#endif
#if defined(UART4)
	if (devicePtr == UART4) return Stream::Channel::Channel4;
#endif
#if defined(TIM3)
	if (devicePtr == TIM3) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM5) return Stream::Channel::Channel6;
	if (devicePtr == I2C2) return Stream::Channel::Channel7;
	std::terminate();
}
Stream::Channel DMA::getChannelForDma1Stream3(const void *devicePtr) const {
#if defined(SPI2)
	if (devicePtr == SPI2) return Stream::Channel::Channel0;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel1;
#if defined(TIM4)
	if (devicePtr == TIM4) return Stream::Channel::Channel2;
#endif
#if defined(I2S2ext)
	if (devicePtr == I2S2ext) return Stream::Channel::Channel3;
#endif
#if defined(USART3)
	if (devicePtr == USART3) return Stream::Channel::Channel4;
#endif
#if defined(UART7)
	if (devicePtr == UART7) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM5) return Stream::Channel::Channel6;
	if (devicePtr == I2C2) return Stream::Channel::Channel7;
	std::terminate();
}
Stream::Channel DMA::getChannelForDma1Stream4(const void *devicePtr) const {
#if defined(SPI2)
	if (devicePtr == SPI2) return Stream::Channel::Channel0;
#endif
#if defined(TIM7)
	if (devicePtr == TIM7) return Stream::Channel::Channel1;
#endif
#if defined(I2S2ext)
	if (devicePtr == I2S2ext) return Stream::Channel::Channel2;
#endif
#if defined(I2C3)
	if (devicePtr == I2C3) return Stream::Channel::Channel3;
#endif
#if defined(UART4)
	if (devicePtr == UART4) return Stream::Channel::Channel4;
#endif
#if defined(TIM3)
	if (devicePtr == TIM3) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM5) return Stream::Channel::Channel6;
#if defined(USART3)
	if (devicePtr == USART3) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma1Stream5(const void *devicePtr) const {
#if defined(SPI3)
	if (devicePtr == SPI3) return Stream::Channel::Channel0;
#endif
#if defined(I2C1)
	if (devicePtr == I2C1) return Stream::Channel::Channel1;
#endif
#if defined(I2S3ext)
	if (devicePtr == I2S3ext) return Stream::Channel::Channel2;
#endif
#if defined(TIM2)
	if (devicePtr == TIM2) return Stream::Channel::Channel3;
#endif
	if (devicePtr == USART2) return Stream::Channel::Channel4;
#if defined(TIM3)
	if (devicePtr == TIM3) return Stream::Channel::Channel5;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel6;
#if defined(DAC1)
	if (devicePtr == DAC1) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma1Stream6(const void *devicePtr) const {
	//if (devicePtr == ) return Stream::Channel::Channel0;
	if (devicePtr == I2C1) return Stream::Channel::Channel1;
#if defined(TIM4)
	if (devicePtr == TIM4) return Stream::Channel::Channel2;
#endif
#if defined(TIM2)
	if (devicePtr == TIM2) return Stream::Channel::Channel3;
#endif
	if (devicePtr == USART2) return Stream::Channel::Channel4;
#if defined(UART8)
	if (devicePtr == UART8) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM5) return Stream::Channel::Channel6;
#if defined(DAC2)
	if (devicePtr == DAC2) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma1Stream7(const void *devicePtr) const {
#if defined(SPI3)
	if (devicePtr == SPI3) return Stream::Channel::Channel0;
#endif
	if (devicePtr == I2C1) return Stream::Channel::Channel1;
#if defined(TIM4)
	if (devicePtr == TIM4) return Stream::Channel::Channel2;
#endif
#if defined(TIM2)
	if (devicePtr == TIM2) return Stream::Channel::Channel3;
#endif
#if defined(UART5)
	if (devicePtr == UART5) return Stream::Channel::Channel4;
#endif
#if defined(TIM3)
	if (devicePtr == TIM3) return Stream::Channel::Channel5;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel6;
	if (devicePtr == I2C2) return Stream::Channel::Channel7;
	std::terminate();
}
//------------------------------------------------ DMA2 -----------------------------------------//
Stream::Channel DMA::getChannelForDma2Stream0(const void *devicePtr) const {
	if (devicePtr == ADC1) return Stream::Channel::Channel0;
	//if (devicePtr == ) return Stream::Channel::Channel1;
#if defined(ADC3)
	if (devicePtr == ADC3) return Stream::Channel::Channel2;
#endif
	if (devicePtr == SPI1) return Stream::Channel::Channel3;
#if defined(SPI4)
	if (devicePtr == SPI4) return Stream::Channel::Channel4;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel5;
	if (devicePtr == TIM1) return Stream::Channel::Channel6;
	//if (devicePtr == ) return Stream::Channel::Channel7;
	std::terminate();
}
Stream::Channel DMA::getChannelForDma2Stream1(const void *devicePtr) const {
#if defined(SAI1)
	if (devicePtr == SAI1) return Stream::Channel::Channel0;
#endif
#if defined(DCMI)
	if (devicePtr == DCMI) return Stream::Channel::Channel1;
#endif
#if defined(ADC3)
	if (devicePtr == ADC3) return Stream::Channel::Channel2;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel3;
#if defined(SPI4)
	if (devicePtr == SPI4) return Stream::Channel::Channel4;
#endif
	if (devicePtr == USART6) return Stream::Channel::Channel5;
	if (devicePtr == TIM1) return Stream::Channel::Channel6;
#if defined(TIM8)
	if (devicePtr == TIM8) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma2Stream2(const void *devicePtr) const {
#if defined(TIM8)
	if (devicePtr == TIM8) return Stream::Channel::Channel0;
#endif
#if defined(ADC2)
	if (devicePtr == ADC2) return Stream::Channel::Channel1;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel2;
	if (devicePtr == SPI1) return Stream::Channel::Channel3;
	if (devicePtr == USART1) return Stream::Channel::Channel4;
	if (devicePtr == USART6) return Stream::Channel::Channel5;
	if (devicePtr == TIM1) return Stream::Channel::Channel6;
#if defined(TIM8)
	if (devicePtr == TIM8) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma2Stream3(const void *devicePtr) const {
#if defined(SAI1)
	if (devicePtr == SAI1) return Stream::Channel::Channel0;
#endif
#if defined(ADC2)
	if (devicePtr == ADC2) return Stream::Channel::Channel1;
#endif
#if defined(SPI5)
	if (devicePtr == SPI5) return Stream::Channel::Channel2;
#endif
	if (devicePtr == SPI1) return Stream::Channel::Channel3;
#if defined(SDIO)
	if (devicePtr == SDIO) return Stream::Channel::Channel4;
#endif
#if defined(SPI4)
	if (devicePtr == SPI4) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM1) return Stream::Channel::Channel6;
#if defined(TIM8)
	if (devicePtr == TIM8) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma2Stream4(const void *devicePtr) const {
	if (devicePtr == ADC1) return Stream::Channel::Channel0;
#if defined(SAI1)
	if (devicePtr == SAI1) return Stream::Channel::Channel1;
#endif
#if defined(SPI5)
	if (devicePtr == SPI5) return Stream::Channel::Channel2;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel3;
	//if (devicePtr == ) return Stream::Channel::Channel4;
#if defined(SPI4)
	if (devicePtr == SPI4) return Stream::Channel::Channel5;
#endif
	if (devicePtr == TIM1) return Stream::Channel::Channel6;
#if defined(TIM8)
	if (devicePtr == TIM8) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma2Stream5(const void *devicePtr) const {
#if defined(SAI1)
	if (devicePtr == SAI1) return Stream::Channel::Channel0;
#endif
#if defined(SPI6)
	if (devicePtr == SPI6) return Stream::Channel::Channel1;
#endif
#if defined(CRYP)
	if (devicePtr == CRYP) return Stream::Channel::Channel2;
#endif
	if (devicePtr == SPI1) return Stream::Channel::Channel3;
	if (devicePtr == USART1) return Stream::Channel::Channel4;
	//if (devicePtr == ) return Stream::Channel::Channel5;
	if (devicePtr == TIM1) return Stream::Channel::Channel6;
#if defined(SPI5)
	if (devicePtr == SPI5) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma2Stream6(const void *devicePtr) const {
	if (devicePtr == TIM1) return Stream::Channel::Channel0;
#if defined(SPI6)
	if (devicePtr == SPI6) return Stream::Channel::Channel1;
#endif
#if defined(CRYP)
	if (devicePtr == CRYP) return Stream::Channel::Channel2;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel3;
#if defined(SDIO)
	if (devicePtr == SDIO) return Stream::Channel::Channel4;
#endif
	if (devicePtr == USART6) return Stream::Channel::Channel5;
	if (devicePtr == TIM1) return Stream::Channel::Channel6;
#if defined(SPI5)
	if (devicePtr == SPI5) return Stream::Channel::Channel7;
#endif
	std::terminate();
}
Stream::Channel DMA::getChannelForDma2Stream7(const void *devicePtr) const {
	//if (devicePtr == ) return Stream::Channel::Channel0;
#if defined(DCMI)
	if (devicePtr == DCMI) return Stream::Channel::Channel1;
#endif
#if defined(HASH)
	if (devicePtr == HASH) return Stream::Channel::Channel2;
#endif
	//if (devicePtr == ) return Stream::Channel::Channel3;
	if (devicePtr == USART1) return Stream::Channel::Channel4;
	if (devicePtr == USART6) return Stream::Channel::Channel5;
	//if (devicePtr == ) return Stream::Channel::Channel6;
#if defined(TIM8)
	if (devicePtr == TIM8) return Stream::Channel::Channel7;
#endif
	std::terminate();
}

} // namespace DMA
} // namespace stm32f4xx
} // namespace microhal
