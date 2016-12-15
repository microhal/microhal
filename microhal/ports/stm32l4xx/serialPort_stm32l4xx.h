/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 11-09-2016
 * last modification: 11-09-2016
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

#ifndef SERIALPORT_STM32L4XX_H_
#define SERIALPORT_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "device/stm32l4xx.h"
#include "interfaces/serialPort_interface.h"
#include "microhalPortConfig_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup stm32f4xx_port
 * @{
 * @class SerialPort
 * @}
 * \brief   This class implements SerialPort functions.
 */
class SerialPort: public microhal::SerialPort {
public:
#if (defined MICROHAL_USE_SERIAL_PORT1_POLLING) || (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT)
    static SerialPort &Serial1;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT2_POLLING) || (defined MICROHAL_USE_SERIAL_PORT2_INTERRUPT)
    static SerialPort &Serial2;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT3_POLLING) || (defined MICROHAL_USE_SERIAL_PORT3_INTERRUPT)
    static SerialPort &Serial3;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT4_POLLING) || (defined MICROHAL_USE_SERIAL_PORT4_INTERRUPT)
    static SerialPort &Serial4;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT5_POLLING) || (defined MICROHAL_USE_SERIAL_PORT5_INTERRUPT)
    static SerialPort &Serial5;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT6_POLLING) || (defined MICROHAL_USE_SERIAL_PORT6_INTERRUPT)
    static SerialPort &Serial6;
#endif

    virtual bool open(OpenMode mode) noexcept = 0;
    bool isOpen(void) const noexcept final {
        return ((usart.CR1 & USART_CR1_UE) == USART_CR1_UE);
    }
    void close() noexcept final {
        usart.CR1 = 0;
    }

    bool setBaudRate(uint32_t baudRate) noexcept final;
    uint32_t getBaudRate() const noexcept final;
    bool setParity(SerialPort::Parity parity) noexcept final;
    bool setStopBits(SerialPort::StopBits stopBits) noexcept final;
    bool setDataBits(SerialPort::DataBits dataBits) noexcept final;
protected:
//------------------------------------------- variables -----------------------------------------//
    USART_TypeDef &usart;
//------------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
    SerialPort(USART_TypeDef &usart) :
            usart(usart) {
    }

    uint16_t maskComputation()
    {
    	if(usart.CR1 & USART_CR1_M) // 9 bits data
    	{
    		if(!(usart.CR1 & USART_CR1_PCE)) // no parity
    		{
    			return 0x01FF;
    		}
    		else //parity
    		{
    			return 0x00FF;
    		}
    	}
    	else // 8 bits data
    	{
    		if(!(usart.CR1 & USART_CR1_PCE)) // no parity
    		{
    			return 0x00FF;
    		}
    		else //parity
    		{
    			return 0x007F;
    		}
    	}
    }

   // virtual ~SerialPort(){
   // }
//------------------------------------------- friends -------------------------------------------//
};
//*************************************************************************************************
// INLINE FUNCTIONS
//*****************

} // namespace stm32l4xx
} // namespace microhal

#endif  // SERIALPORT_STM32L4XX_H_
