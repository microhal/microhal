/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic component
 *
 * @authors    Pawel Okas
 * created on: 12-03-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015, microHAL
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

#include "diagnostic.h"

namespace microhal {
namespace diagnostic {

const char *Diagnostic_base::levelName[9] = {nullptr, "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Informational", "Debug"};

/**
 *
 * @param logHeader
 */
void Diagnostic_base::printHeader(const LogLevelHeader_base &logHeader, const LogLevel level) {
    const char comaSeparator[] = {',', ' '};
    const char headerEndTxt[] = {':', '\t'};

    endl();  // always start header from new line
    if (headerDisplayMode & EnableTimestamp) {
        uint64_t milliseconds_since_epoch =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        write(milliseconds_since_epoch, 10);
        if (spaces == false) putChar(' ');
    }
    if (header) {
        writeText(header, strlen(header));
        writeText(comaSeparator, sizeof(comaSeparator));
    }
    if (headerDisplayMode & EnableFileName) {
        writeText(logHeader.fileLine, strlen(logHeader.fileLine));
    }
    if (headerDisplayMode & EnableLevelName) {
        writeText(comaSeparator, sizeof(comaSeparator));
        writeText(levelName[static_cast<unsigned int>(level)], strlen(levelName[static_cast<unsigned int>(level)]));  // print log level name
    }
    if (headerDisplayMode & EnableErrorCode) {
        writeText(comaSeparator, sizeof(comaSeparator));
        // log code have constant length
        writeText(logHeader.code, 9);  // print log code
    }
    writeText(headerEndTxt, sizeof(headerEndTxt));
}

/**
 *
 * @param data - pointer do data
 * @param size - size of data
 * @param radix
 */
void Diagnostic_base::write(const uint8_t *data, size_t size, uint8_t radix) {
    if (data == nullptr) return;
    if (size <= 0) return;

    char buffer[9];
    size_t i = 0;
    for (; i < size - 1; i++) {
        itoa(static_cast<uint32_t>(data[i]), buffer, radix);  // todo replece itoa function that will return length of string, this allow to increase
                                                              // execution time by eliminate strlen function
        writeText(buffer, strlen(buffer));
        putChar(':');
    }
    itoa(static_cast<uint32_t>(data[i]), buffer, radix);
    writeText(buffer, strlen(buffer));
}

/**
 *
 * @param data - pointer do data
 * @param size - size of data
 * @param radix -
 */
void Diagnostic_base::write(const uint16_t *data, size_t size, uint8_t radix) {
    if (data == nullptr) return;
    if (size <= 0) return;

    char buffer[33];
    size_t i = 0;
    for (; i < size - 1; i++) {
        itoa(static_cast<uint32_t>(data[i]), buffer, radix);
        writeText(buffer, strlen(buffer));
        putChar(':');
    }
    itoa(static_cast<uint32_t>(data[i]), buffer, radix);
    writeText(buffer, strlen(buffer));
}

/**
 *
 * @param data - pointer do data
 * @param size - size of data
 * @param radix -
 */
void Diagnostic_base::write(const uint32_t *data, size_t size, uint8_t radix) {
    if (data == nullptr) return;
    if (size <= 0) return;

    char buffer[33];
    size_t i = 0;
    for (; i < size - 1; i++) {
        itoa(data[i], buffer, radix);
        writeText(buffer, strlen(buffer));
        putChar(':');
    }
    itoa(data[i], buffer, radix);
    writeText(buffer, strlen(buffer));
}

}  // namespace diagnostic
}  // namespace microhal
