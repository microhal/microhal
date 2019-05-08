/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 01-05-2019
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

using namespace microhal;
using namespace diagnostic;
using namespace std::literals;

void logger() {
    bsp::gpsReset.reset();
    bsp::gpsForceOn.set();
    L86 gps;
    diagChannel << lock << MICROHAL_DEBUG << "Logging started." << endl << unlock;
    if (FILE *f = fopen("log.txt", "wb")) {
        while (1) {
            if (bsp::gpsPort.availableBytes() > 90) {
                char buffer[100];
                size_t length = bsp::gpsPort.read(buffer, sizeof(buffer));
                fwrite(buffer, 1, length, f);
                bsp::debugPort.write(buffer, length);

                // bsp::debugPort.waitForWriteFinish(500ms);

                //            std::string_view message(buffer);
                //            message.remove_prefix(message.find_first_of("$"));
                //            if (auto [status, gga] = gps.decodeGGA(message); status == 1) {
                //                diagChannel << lock << Debug << "GGA: " << endl
                //                            << "\tAltitude: " << gga.altitude << endl
                //                            << "\tLatitude: " << gga.latitude << endl
                //                            << "\tLongitude: " << gga.longitude << endl
                //                            << "\tHDOP: " << gga.hdop << endl
                //                            << "\tSatelites in use: " << gga.numberOfSatelitesUsed << endl
                //                            << unlock;
                //            }
            }
            if (bsp::button2.isReset()) break;
        }
        fclose(f);
        diagChannel << lock << MICROHAL_DEBUG << "Logging stoped." << endl << unlock;
    } else {
        diagChannel << lock << MICROHAL_DEBUG << "Unable to open log file." << endl << unlock;
    }
}

int main(int argc, char *const argv[]) {
    int result = -1;
    if (bsp::init()) {
        bsp::debugPort.write("\n\r------------------- GPS Logger -------------------------\n\r");
        diagChannel.setOutputDevice(bsp::debugPort);

        while (1) {
            if (bsp::button1.isReset()) {
                logger();
            }
        }
    } else {
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to open communication ports." << endl << unlock;
    }

    return result;
}
