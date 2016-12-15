/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 18-09-2016
 * last modification: 18-09-2016
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "spi_polling_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {

#ifdef MICROHAL_USE_SPI1_POLLING
GPIO::IOPin spi1MisoPin(GPIO::PortA, 6);
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

} // namespace stm32l4xx
} // namespace microhal


