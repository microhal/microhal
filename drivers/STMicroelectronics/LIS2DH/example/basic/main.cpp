#include "diagnostic/diagnostic.h"
#include "lis2dh.h"
#include "microhal.h"
#include "microhal_bsp.h"

using namespace microhal;
using namespace diagnostic;

LIS2DH lis2dh(sensorI2C, LIS2DH::I2C_ADDRESS_0);

void lisInterrupt1() {}

void lisInterrupt2() {}

void test1() {
  diagChannel << lock << Informational << "======Test1======" << endl << unlock;
  diagChannel << lock << Informational << "Simple raw accelerates read" << endl
              << unlock;
  bool flag = true;

  do {
    flag = lis2dh.enableAxis(true, true, true);
    if (false == flag) break;
    flag = lis2dh.setDataRate(LIS2DH::DataRate::Mode1Hz);
    if (false == flag) break;

    break;
  } while (1);

  int16_t x, y, z;
  while (flag == true) {
    flag = lis2dh.getLastData(x, y, z);
    diagChannel << lock << Informational << "Raw X:" << x << " Raw Y:" << y
                << " Raw Z:" << z << endl
                << unlock;
    flag = lis2dh.getLastRawX(x);
    diagChannel << lock << Informational << "Only raw X:" << x << endl
                << unlock;
    flag = lis2dh.getLastRawY(y);
    diagChannel << lock << Informational << "Only raw Y:" << y << endl
                << unlock;
    flag = lis2dh.getLastRawZ(z);
    diagChannel << lock << Informational << "Only raw Z:" << z << endl
                << unlock;
    LIS2DH::AccelerationVector v;
    flag = lis2dh.getLastData(v);
    diagChannel << lock << Informational << "X:" << v[0] << " Y:" << v[1]
                << " Z:" << v[2] << endl
                << unlock;
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
  }
  diagChannel << lock << MICROHAL_ERROR << "Test1 error" << endl << unlock;
}
void test2() {}
int main(void) {
  ExternalInterrupt::init();
  debugPort.write("\n\r----------------- LIS2DH Demo -----------------\n\r");
  diagChannel.setOutputDevice(debugPort);

  GPIO CS(lisCS, GPIO::Direction::Output);
  CS.set();
  GPIO SA0(lisSA0, GPIO::Direction::Output);
  SA0.reset();

  ExternalInterrupt::connect(
      lisInterrupt1, ExternalInterrupt::Trigger::OnFallingEdge, lisINT1);
  ExternalInterrupt::enable(lisINT1);

  //  ExternalInterrupt::connect(
  //      lisInterrupt2, ExternalInterrupt::Trigger::OnFallingEdge, lisINT2);
  //  ExternalInterrupt::enable(lisINT2);

  if (false == lis2dh.init()) {
    diagChannel << lock << MICROHAL_ERROR << "Cannot initialize LIS2DH" << endl
                << unlock;
  }

  test1();
  test2();
  while (1) {
  }

  return 0;
}
