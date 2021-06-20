/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 01-01-2018
 * last modification: 01-01-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_MICROHALCONFIG_H_
#define _MICROHAL_MICROHALCONFIG_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */

// clang-format off
//***********************************************************************************************//
//                                    Diagnostic configuration                                   //
//***********************************************************************************************//
#define MICROHAL_DIAGNOSTIC_TEXT_VISIBLE		// when defined message text is printed in diagnostic channel messages
#define MICROHAL_DIAGNOSTIC_LOG_LEVEL Debug		// Set compile time log level for embedded diagnostic channel (diagChannel)
												// Emergency -> highest log priority
												// Alert
												// Critical
												// Error
												// Warning
												// Notice
												// Informational
//***********************************************************************************************//
//                                    FatFs module configuration                                 //
//***********************************************************************************************//
#define MICROHAL_FATFS_MAXFILES_IN_USE 10
#define MICROHAL_FATFS_RAMDISK_SIZE 0               // When RAM disk size is set to 0 RAM disk functionality is disabled.
                                                    // RAM diski size is expressed in bytes									// Debug  -> lowest log priority
// clang-format on
#endif  // _MICROHAL_MICROHALCONFIG_H_
