/*
 * ioPin_port.h
 *
 *  Created on: Oct 5, 2019
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_MANAGER_IOPIN_PORT_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_MANAGER_IOPIN_PORT_H_

/* ************************************************************************************************
 * INCLUDES
 */
/* clang-format off */
#if defined(VENDOR_ATMEL)
    #include "Atmel/ioPin_atmel.h"
#elif defined(VENDOR_NXP)

#elif defined(VENDOR_NORDIC)
    #include "Nordic/ioPin_nordic.h"
#elif defined(VENDOR_RENESAS)
    #include "Renesas/ioPin_renesas.h"
#elif defined(VENDOR_SILICON_LABS)
    #include "SiliconLabs/ioPin_siliconLabs.h"
#elif defined(VENDOR_STMICROELECTRONICS)
    #include "STMicroelectronics/ioPin_stm.h"
#elif defined(VENDOR_TEXAS_INSTRUMENTS)

#elif defined(LINUX_PORT)
    // #include "linux/ioPin_linux.h"
#elif defined(WINDOWS_PORT)
    // #include "windows/ioPin_windows.h"
#else
    #error "Microcontroller vendor undefined."
#endif
/* clang-format on */

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_MANAGER_IOPIN_PORT_H_ */
