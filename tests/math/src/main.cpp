/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
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

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"

#if defined(USE_DOCTEST) && USE_DOCTEST == 1
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

using namespace std::literals::chrono_literals;

using namespace microhal;
using namespace diagnostic;

void showNativeRepresentation();

int main(int argc, char* const argv[]) {
    int result = -1;
    if (bsp::init()) {
        bsp::debugPort.write("\n\r------------------- CAN Demo -------------------------\n\r");
        diagChannel.setOutputDevice(bsp::debugPort);

        diagChannel << lock << MICROHAL_INFORMATIONAL << "Starting unit tests." << endl << unlock;
        showNativeRepresentation();

#if defined(USE_DOCTEST) && USE_DOCTEST == 1
        doctest::Context context(argc, argv);
        result = context.run();
        if (context.shouldExit()) {  // important - query flags (and --exit) rely on the user doing this
#endif
            bsp::deinit();
            return result;  // propagate the result of the tests
#if defined(USE_DOCTEST) && USE_DOCTEST == 1
        }
#endif

    } else {
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to open communication ports." << endl << unlock;
    }

    return result;
}
