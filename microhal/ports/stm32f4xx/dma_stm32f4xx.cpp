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

//Stream::Channel getChannelForDma1Stream0(SPI_TypeDef *devicePtr) {
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//		case reinterpret_cast<uint32_t>(SPI3):
//			return Stream::Channel::Channel0;
//		default:
//			while(1) {}
//	}
//}
//Stream::Channel getChannelForDma1Stream0(I2C_TypeDef *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//		case reinterpret_cast<uint32_t>(I2C1):
//			return Stream::Channel::Channel1;
//		default:
//			while(1) {}
//	}
//}
//Stream::Channel getChannelForDma1Stream0(TIM_TypeDef *devicePtr) {
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//		case reinterpret_cast<uint32_t>(TIM4):
//				return Stream::Channel::Channel2;
//		case reinterpret_cast<uint32_t>(TIM5):
//				return Stream::Channel::Channel6;
//		default:
//			while(1) {}
//	}
//}
//Stream::Channel getChannelForDma1Stream0(TIM_TypeDef *devicePtr) {
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//		case reinterpret_cast<uint32_t>(I2S3): return Stream::Channel::Channel3;
//		default:
//			while(1) {}
//	}
//}
//Stream::Channel getChannelForDma1Stream0(USART_TypeDef *devicePtr) {
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//		case reinterpret_cast<uint32_t>(UART5): return Stream::Channel::Channel4;
//		case reinterpret_cast<uint32_t>(UART8): return Stream::Channel::Channel5;
//		default:
//			while(1) {}
//	}
//}
//
//
//Stream::Channel DMA::getChannelForDma1Stream1(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(TIM2): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(USART3): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(UART7): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM5): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(TIM6): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma1Stream2(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SPI3): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(TIM7): return Stream::Channel::Channel1;
//	//case reinterpret_cast<uint32_t>(I2S3): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(I2C3): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(UART4): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(TIM3): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM5): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(I2C2): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma1Stream3(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SPI2): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(TIM4): return Stream::Channel::Channel2;
//	//case reinterpret_cast<uint32_t>(I2S2): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(USART3): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(UART7): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM5): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(I2C2): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma1Stream4(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SPI2): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(TIM7): return Stream::Channel::Channel1;
//	//case reinterpret_cast<uint32_t>(I2S2): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(I2C3): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(UART4): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(TIM3): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM5): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(USART3): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma1Stream5(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SPI3): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(I2C1): return Stream::Channel::Channel1;
//	//case reinterpret_cast<uint32_t>(I2S3): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(TIM2): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(USART2): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(TIM3): return Stream::Channel::Channel5;
//	//case reinterpret_cast<uint32_t>(DAC1): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma1Stream6(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(I2C1): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(TIM4): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(TIM2): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(USART2): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(UART8): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM5): return Stream::Channel::Channel6;
//	//case reinterpret_cast<uint32_t>(DAC2): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma1Stream7(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SPI3): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(I2C1): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(TIM4): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(TIM2): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(UART5): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(TIM3): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(I2C2): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
////------------------------------------------------ DMA2 -----------------------------------------//
//Stream::Channel DMA::getChannelForDma2Stream0(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(ADC1): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(ADC3): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(SPI1): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(SPI4): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel6;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma2Stream1(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SAI1): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(DCMI): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(ADC3): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(SPI4): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(USART6): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(TIM8): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma2Stream2(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(TIM8): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(ADC2): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(SPI1): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(USART1): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(USART6): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(TIM8): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma2Stream3(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SAI1): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(ADC2): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(SPI5): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(SPI1): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(SDIO): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(SPI4): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(TIM8): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma2Stream4(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(ADC1): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(SAI1): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(SPI5): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(SPI4): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(TIM8): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma2Stream5(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(SAI1): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(SPI6): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(CRYP): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(SPI1): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(USART1): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(SPI5): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma2Stream6(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel0;
//	case reinterpret_cast<uint32_t>(SPI6): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(CRYP): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(SPI1): return Stream::Channel::Channel3;
//	case reinterpret_cast<uint32_t>(USART1): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(TIM1): return Stream::Channel::Channel6;
//	case reinterpret_cast<uint32_t>(SPI5): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}
//Stream::Channel DMA::getChannelForDma2Stream7(void *devicePtr){
//	switch(reinterpret_cast<uint32_t>(devicePtr)){
//	case reinterpret_cast<uint32_t>(DCMI): return Stream::Channel::Channel1;
//	case reinterpret_cast<uint32_t>(HASH): return Stream::Channel::Channel2;
//	case reinterpret_cast<uint32_t>(USART1): return Stream::Channel::Channel4;
//	case reinterpret_cast<uint32_t>(USART6): return Stream::Channel::Channel5;
//	case reinterpret_cast<uint32_t>(TIM8): return Stream::Channel::Channel7;
//	//default : return 0xFFFFFFFF;
//	}
//}

} // namespace DMA
} // namespace stm32f4xx
} // namespace microhal
