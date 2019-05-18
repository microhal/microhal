/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 06-04-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "l86.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::literals;

void demo() {
    bsp::gpsReset.reset();
    bsp::gpsForceOn.set();
    L86 gps;
    while (1) {
        if (bsp::gpsPort.availableBytes() > 90) {
            char buffer[100];
            size_t length = bsp::gpsPort.read(buffer, sizeof(buffer));
            bsp::debugPort.write(buffer, length);
            bsp::debugPort.waitForWriteFinish(500ms);

            std::string_view message(buffer);
            message.remove_prefix(message.find_first_of("$"));
            if (auto [status, gga] = gps.decodeGGA(message); status == 1) {
                diagChannel << lock << Debug << "GGA: " << endl
                            << "\tAltitude: " << gga.altitude << endl
                            << "\tLatitude: " << gga.latitude << endl
                            << "\tLongitude: " << gga.longitude << endl
                            << "\tHDOP: " << gga.hdop << endl
                            << "\tSatelites in use: " << gga.numberOfSatelitesUsed << endl
                            << unlock;
            }
        }
    }
}

int main(int argc, char* const argv[]) {
    int result = -1;

    if (bsp::init()) {
        bsp::debugPort.write("\n\r------------------- L86 GPS Demo -------------------------\n\r");
        diagChannel.setOutputDevice(bsp::debugPort);

        diagChannel << lock << MICROHAL_INFORMATIONAL << "Starting unit tests." << endl << unlock;

        doctest::Context context(argc, argv);
        result = context.run();
        if (context.shouldExit()) {  // important - query flags (and --exit) rely on the user doing this
            bsp::deinit();
            return result;  // propagate the result of the tests
        }

    } else {
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to open communication ports." << endl << unlock;
    }
    demo();
    return result;
}
