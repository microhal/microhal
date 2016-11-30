/*
 * SPI_STM32f4xx.cpp
 *
 *  Created on: 13 mar 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_polling_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

#ifdef MICROHAL_USE_SPI1_POLLING
GPIO::IOPin spi1MisoPin(GPIO::PortB, 4);
SPI_polling SPI_polling::spi1(*SPI1, spi1MisoPin);
SPI &SPI::spi1 = SPI_polling::spi1;
#endif
#ifdef MICROHAL_USE_SPI2_POLLING
GPIO::IOPin spi2MisoPin(GPIO::PortB, 14);
SPI_polling SPI_polling::spi2(*SPI2, spi2MisoPin);
SPI &SPI::spi2 = SPI_polling::spi2;
#endif
#ifdef MICROHAL_USE_SPI3_POLLING
GPIO::IOPin spi3MisoPin(GPIO::PortC, 11);
SPI_polling SPI_polling::spi3(*SPI3, spi3MisoPin);
SPI &SPI::spi3 = SPI_polling::spi3;
#endif
#ifdef MICROHAL_USE_SPI4_POLLING
SPI_polling SPI_polling::spi4(*SPI4);
SPI &SPI::spi4 = SPI_polling::spi4;
#endif
#ifdef MICROHAL_USE_SPI5_POLLING
SPI_polling SPI_polling::spi5(*SPI5);
SPI &SPI::spi5 = SPI_polling::spi5;
#endif
#ifdef MICROHAL_USE_SPI6_POLLING
SPI_polling SPI_polling::spi6(*SPI6);
SPI &SPI::spi6 = SPI_polling::spi6;
#endif

} // namespace stm32f4xx
} // namespace microhal
