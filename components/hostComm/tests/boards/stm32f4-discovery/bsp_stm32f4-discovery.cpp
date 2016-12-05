/* =========================================================================================== *//**
 @file        bsp_stm32f4-discovery.cpp
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       hardware definitions
 @hardware    STM32F4Discovery
 @copyright   $Copyright$
 @details

 *//* ============================================================================================ */
#include "microhal.h"
#include "microhal_bsp.h"
#include "SPIDevice/SPIDevice.h"
#include "microhal_bsp.h"

#include "FreeRTOS.h"
#include "task.h"

using namespace microhal;
using namespace diagnostic;

void main(void *);

void hardwareConfig(void) {
	stm32f4xx::Core::pll_start(8000000, 168000000);
	stm32f4xx::Core::fpu_enable();

    stm32f4xx::IOManager::routeSerial<1, Txd, stm32f4xx::GPIO::PortB, 6>();
    stm32f4xx::IOManager::routeSerial<1, Rxd, stm32f4xx::GPIO::PortB, 7>();

    stm32f4xx::IOManager::routeSerial<2, Txd, stm32f4xx::GPIO::PortA, 2>();
    stm32f4xx::IOManager::routeSerial<2, Rxd, stm32f4xx::GPIO::PortA, 3>();

    stm32f4xx::SerialPort::Serial1.clear();

	stm32f4xx::SerialPort::Serial1.setDataBits(stm32f4xx::SerialPort::Data8);
	stm32f4xx::SerialPort::Serial1.setStopBits(stm32f4xx::SerialPort::OneStop);
	stm32f4xx::SerialPort::Serial1.setParity(stm32f4xx::SerialPort::NoParity);
	stm32f4xx::SerialPort::Serial1.setBaudRate(stm32f4xx::SerialPort::Baud115200, stm32f4xx::SerialPort::AllDirections);


//    xTaskHandle mainHandle;
//    xTaskCreate(main, (const char* )"main", (10*1024), 0, tskIDLE_PRIORITY, &mainHandle);
//
//    vTaskStartScheduler();
}

bool BSP_Init(void) {

	bool result = false;
	do {
		// setup diagnostic port
		diagChannel.setOutputDevice(debugPort);

		result = true;
	} while(0);
	return result;
}

bool BSP_Deinit(void) {

	bool result = false;
	do {

		while(1) {
			;
		}
		result = true;
	} while(0);
	return result;
}

uint64_t SysTick_time = 0;
