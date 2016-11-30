/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic example main file
 *
 * @authors    Pawel Okas
 * created on: 2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014, microHAL
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

#include <string.h>

#include "microhal.h"
#include "microhal_bsp.h"
#include "diagnostic/diagnostic.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

int main(void) {
    // to print some diagnostic logs we need debug IODevice, to see declaration of this device go to 'boards' folder and find your active configuration (i.e. Windows).
    // In this example IODevice instance is called 'debugPort', on Windows it may be connected to console, on STM32 to UART
    // open diagnostic device
    debugPort.open(IODevice::ReadWrite);
    // show hello text
    debugPort.write("\n\r------------------- Diagnostic Demo -------------------------\n\r");

    // lets create diagnosticChannel
    // 'LogLevel::Notice' that template parameter sets log level, it mean that every information with lover log level will be skipped during compilation and not included in binary.
    // we can name our diagnostic channel, in this case the name is set to: "First diagnostic channel : "
    // last thing to do is to pass to which output IODevice print the logs
    Diagnostic<LogLevel::Notice> firstDiagnosticChannel("First log channel", debugPort, EnableTimestamp | EnableFileName | EnableLevelName | EnableErrorCode);

    // because diagnostic module is designed to work in multithread environment we should 'lock' channel before writing any information on it. After writing, we
    // have to 'unlock' diagnostic channel.
    // the line below prints two messages, the first is warning, next is notice.
    firstDiagnosticChannel << lock << MICROHAL_WARNING << "This is a warning" << MICROHAL_NOTICE << "Notice:" << "you can ignore it" << unlock;

    // lets see same log as above but with enabled automatics space insertion
    firstDiagnosticChannel.autoInsertSpaces(true);
    firstDiagnosticChannel << lock << MICROHAL_WARNING << "This is a warning" << MICROHAL_NOTICE << "Notice:" << "you can ignore it" << unlock;

    // lets check compile time log removal
    // firstDiagnosticChannel have compile time log level set to Notice it means every log with less priority will be removed. Bellow you can see available priority
    //
    // Emergency -> highest log priority
    // Alert
    // Critical
    // Error
    // Warning
    // Notice
    // Informational
    // Debug  -> lowest log priority
    //
    // lets print some logs
    firstDiagnosticChannel << lock << MICROHAL_NOTICE << "This is notice, after this communicate we try to print some log with 'Informational' priority,"
    		"it shouldn't be visible" << MICROHAL_INFORMATIONAL << "-----> If you see these line it means 'firstDiagnosticChannel' have compile time log"
			"level set at least to 'Informational'or an error occurred. <-----" << unlock;

    // to increase flexibility of diagnostic channels usage you can change logging level at runtime, but that have some limitations, you can change your logging level
    // only to higher than set during compiling.
    // For example our 'firstDiagnosticChannel' have compile time log level set to 'Notice', so our runtime log level can't be set to lower priority than Notice.
    // Let see what will happen when we try to set lower priority
    firstDiagnosticChannel.setLogLevel(Informational);

    //OK, let set runtime priority to a valid value, and again print first log
    firstDiagnosticChannel.setLogLevel(Warning);
    firstDiagnosticChannel << lock << MICROHAL_WARNING << "This is a warning" << MICROHAL_NOTICE << "Notice: you can ignore it" << unlock;

    // our diagnostic channel can print variables, so create one and print it
    uint8_t variable = 0x0A;

    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "variable value is printed in decimal by default: " << variable << unlock;
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "but we can print it in binary form: " << toBin(variable) << unlock;
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "octal: " << toOct(variable) << unlock;
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "or hexadecimal: " << toHex(variable) << unlock;

    // another nice feature is array printing, so lets create one
    uint32_t array[] = {0x01, 0x02, 0x03, 0x09, 0x0F};

    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "lets print our array in hex: " << toHex(array, sizeof(array)) << unlock;
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "now in binary form: " << toBin(array, sizeof(array)) << unlock;
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "and octal: " << toOct(array, sizeof(array)) << unlock;
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "boring..." << unlock;

    // our diagnostic channel can log time, you probably noticed that
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "After this text we will wait 500ms" << unlock;
    std::this_thread::sleep_for(std::chrono::milliseconds {500});
    firstDiagnosticChannel << lock << MICROHAL_EMERGENCY << "done, this log is printed 500ms later." << unlock;

    // what if we have limited time to print log, our 'lock' don't have timeout parameter so we can do something like this:
    if (firstDiagnosticChannel.tryLock(1000ms)) {
        firstDiagnosticChannel << MICROHAL_EMERGENCY << "Locked successfully." << unlock;
    }

    // OK, what if we want to start our log and finish it with different communicate, depending on if statement
    // as you can see below we have log level 'WARNING', that is a macro which generates header with file name and log line position in file,
    // but 'Warning' will set log level to 'Warning' without header
    firstDiagnosticChannel << lock << MICROHAL_WARNING << "Trying initialize some important thing...";
    if (true) {
        firstDiagnosticChannel << Warning << "OK. Important thing was initialized successfully." << unlock; // continue text log without new header and changing log level
    } else {
        firstDiagnosticChannel << MICROHAL_EMERGENCY << "BAD, our important thing wasn't initialized successfully, this is Emergency situation." << unlock; // add new header
    }

    // last thing is that microHAL library have embedded diagnostic channel called diagChannel. It is used to debug microHAL library. To make 'diagChannel' useful
    // you have to set diagnostic IODevice to it. Below we set diagnostic IODevice to our debugPort.
    // compile time log level of diagChannel can be set by MICROHAL_DIAGNOSTIC_LOG_LEVEL macro.
    diagChannel.setOutputDevice(debugPort);

    // lets check if diagChannal is working
    diagChannel << lock << MICROHAL_EMERGENCY << "microHAL embedded diagChannel is working:)" << unlock;
    diagChannel << lock << MICROHAL_EMERGENCY << "end of example, I hope it was useful." << unlock;

    return 0;
}
