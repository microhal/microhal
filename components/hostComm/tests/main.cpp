/*
 * hostComm_example.h
 *
 *  Created on: 20 gru 2014
 *      Author: Dell
 */

#include "microhal.h"
#include "hostComm/hostComm.h"
#include "microhal_bsp.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

using namespace microhal;
using namespace diagnostic;

int main(int argc, char* const argv[]) {
	int result = -1;

	if (BSP_Init()) {
	//diagChannel.setLogLevel(Debug);
		diagChannel << lock << INFORMATIONAL << "Starting unit tests." << endl << unlock;

		result = Catch::Session().run( argc, argv );
	} else {
		diagChannel << lock << EMERGENCY << "Unable to open communication ports." << endl << unlock;
	}

	BSP_Deinit();

	return result;
}
