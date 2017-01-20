#include "microhal.h"
#include "diagnostic/diagnostic.h"
#include "microhal_bsp.h"


using namespace microhal;
using namespace diagnostic;



int main(void) {
    debugPort.write("\n\r----------------- LIS2DH Demo -----------------\n\r");
    diagChannel.setOutputDevice(debugPort);



    while (1) {
    }

    return 0;
}
