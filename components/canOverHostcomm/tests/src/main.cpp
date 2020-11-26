/*
 * hostComm_example.h
 *
 *  Created on: 20 gru 2014
 *      Author: Dell
 */

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "hostComm.h"
#include "microhal.h"

using namespace microhal;
using namespace diagnostic;

int main(int argc, char* const argv[]) {
    int result = -1;
    doctest::Context context;

    if (bsp::init()) {
        // diagChannel.setLogLevel(Debug);
        diagChannel << lock << MICROHAL_INFORMATIONAL << "Starting unit tests." << endl << unlock;

        context.applyCommandLine(argc, argv);
        int res = context.run();
        if (context.shouldExit())  // important - query flags (and --exit) rely on the user doing this
            return res;            // propagate the result of the tests

        context.clearFilters();  // removes all filters added up to this point
    } else {
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to open communication ports." << endl << unlock;
    }

    bsp::deinit();

    return result;
}
