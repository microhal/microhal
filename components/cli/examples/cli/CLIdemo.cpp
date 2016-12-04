/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief	   Example of a project that incorporates CLI module.
 *
 * @authors    Kubaszek Mateusz <mathir.km.microhal@gmail.com>
 * created on: <23-01-2016>
 * last modification: <24-01-2016>
 *
 * @copyright Copyright (c) 2015, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *        2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *           documentation and/or other materials provided with the distribution.
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

/**
 * @defgroup microHAL
 * @{
 */

/**
 * @defgroup components
 * @{
 */

/**
 * @defgroup cli
 * @{
 */

/**
 * @defgroup example
 * @{
 */

#include "microhal.h"
#include "microhal_bsp.h"
#include "CLI.h"
#include "subMenu.h"
#include "cliParser.h"

#include <cstring>
#include <iostream>
#include <list>
#include <cstdlib>

using namespace microhal;

// The following classes are menu item objects containing executing static functions.

class MemorySave : public MenuItem {
 public:
  MemorySave(void) : MenuItem("save", "Saves current state to non-volatile memory.") {
  }
  static void memorySave(IODevice& port) {
    port.write("Memory saved!!!");
  }
 protected:
  int execute(std::list<char*> &words, IODevice& port) {
    memorySave(port);
    return 0;
  }
};

class MemoryRestore : public MenuItem {
 public:
  MemoryRestore(void)
      : MenuItem("restore", "Restores saved device state.") {
  }
  static void memoryRestore(IODevice& port) {
    port.write("Memory restored!!!");
  }
 protected:
  virtual int execute(std::list<char*> &words, IODevice& port) {
    memoryRestore(port);
    return 0;
  }
};

class Car : public MenuItem{
public:
	static char color[30];
	static float maxSpeed;
	static int gearsCnt;

	Car(const char* name, const char* help) :
			MenuItem(name, help) {}
protected:
	void setColor(char* color) {
		strcpy(this->color, color);
	}
};

char Car::color[30] = "undefined";
float Car::maxSpeed = -1.0;
int Car::gearsCnt = -1;


class CarSet : public Car{
public:
	CarSet(void) : Car("set",
			"Set car parameters:"
			"\n\tcolor:\t<""red"", ""yellow"", etc...> - color name as string"
			"\n\tspeed:\t<max speed> - max speed of car"
			"\n\tgears:\t<gears count> - gears count"){
	}
protected:
	virtual int execute(std::list<char*> &words, IODevice& port){
		char tmp[40];
		std::pair<char*,char*> color(tmp, (char*)"color");
		std::pair<float, char*> speed(0, (char*)"speed");
		std::pair<int, char*> gears(0, (char*)"gears");

		char txt[60];

		if(CliParser::ParserStatus::OK == CliParser::parseString(words, color, 40)){
			port.write("\tSet color to ");
			port.write(color.first);
			port.write(".\n");
			setColor(color.first);
		}
		CliParser::ParserStatus result = CliParser::parseNumerical(words, speed, 100.0, 50.0, 400.0, 10);
		if(CliParser::ParserStatus::OK == result){
			snprintf(txt, 60, "\tSet maxspeed to %f.\n", speed.first);
			port.write(txt);
			maxSpeed = speed.first;
		} else if ((CliParser::ParserStatus::MaxViolation | CliParser::ParserStatus::MinViolation)&result){
			snprintf(txt, 60, "\tMax or min value violation, set maxspeed to %f.\n", speed.first);
			port.write(txt);
			maxSpeed = speed.first;
		}

		result = CliParser::parseNumerical(words, gears, -1, 3, 20, 10);
		if(CliParser::ParserStatus::OK == result){
			snprintf(txt, 60, "\tSet gears count to %d.\n", gears.first);
			port.write(txt);
			gearsCnt = gears.first;
		} else if((CliParser::ParserStatus::MaxViolation | CliParser::ParserStatus::MinViolation)&result){
			snprintf(txt, 60, "\tMax or min violation, set gears count to %d.\n", gears.first);
			port.write(txt);
			gearsCnt = gears.first;
		}

		return 0;
	}
};

class CarPrint : public Car{
public:
	CarPrint(void) : Car("get", "Get car parameters."){
	}
protected:
	virtual int execute(std::list<char*> &words, IODevice& port){
		char txt[30];
		port.write("Your car is ");
		port.write(color);
		port.write(", its max speed is ");
		snprintf(txt, 30, "%f, and has %d gears.\n", Car::maxSpeed, Car::gearsCnt);
		port.write(txt);
		return 0;
	}
};

class Clock : public MenuItem {
public:
  Clock(const char* name, const char* help)
      : MenuItem(name, help) {}

  static int hours, minutes, seconds;
  static bool alarm;
};

int Clock::hours = 0;
int Clock::minutes = 0;
int Clock::seconds = 0;
bool Clock::alarm = false;

class AlarmOff : public Clock {
 public:
  AlarmOff(void)
      : Clock("off", "Restores saved device state.") {
  }
  static void alarmOff(IODevice& port) {
    alarm = false;
    port.write("AlarmOff");
  }
 protected:
  virtual int execute(std::list<char*> &words, IODevice& port) {
    alarmOff(port);
    return 0;
  }
};

class AlarmOn : public Clock {
 public:
  AlarmOn(void)
      : Clock("on", "Restores saved device state.") {
  }
  static void alarmOn(IODevice& port) {
    alarm = true;
    port.write("AlarmOn");
  }
 protected:
  virtual int execute(std::list<char*> &words, IODevice& port) {
    alarmOn(port);
    return 0;
  }
};

class ClockStatus : public Clock {
 public:
  ClockStatus(void)
      : Clock("status", "Gives current time.") {
  }
  static void clockStatus(IODevice& port) {
    char str[25];
    snprintf(str, 25, "%02d:%02d:%02d\n", hours, minutes, seconds);
    port.write(str);
    if (alarm)
      port.write("Alarm is on.");
    else
      port.write("Alarm is off.");
  }
 protected:
  virtual int execute(std::list<char*> &words, IODevice& port) {
    clockStatus(port);
    return 0;
  }
};

class ClockSet : public Clock {
 public:
  ClockSet(void)
      : Clock("set", "Sets current time\n\t-h: <hours>\n\t-m: <minutes>\n\t-s:[seconds]") {
  }
 protected:
	virtual int execute(std::list<char*> &words, IODevice& port) {

		std::pair<int, char*> sec(0, (char*) "-s");
		std::pair<int, char*> min(0, (char*) "-m");
		std::pair<int, char*> hrs(0, (char*) "-h");

		char txt[60];

		CliParser::ParserStatus result = CliParser::ParserStatus::Error;
		result = CliParser::parseNumerical(words, sec, 0, 0, 59, 10);
		if ((CliParser::ParserStatus::MaxViolation | CliParser::ParserStatus::MinViolation | CliParser::ParserStatus::OK) & result) {
			seconds = sec.first;
		}
		result = CliParser::parseNumerical(words, min, 0, 0, 59, 10);
		if ((CliParser::ParserStatus::MaxViolation | CliParser::ParserStatus::MinViolation | CliParser::ParserStatus::OK) & result) {
			minutes = min.first;
		}
		result = CliParser::parseNumerical(words, hrs, 0, 0, 23, 10);
		if ((CliParser::ParserStatus::MaxViolation | CliParser::ParserStatus::MinViolation | CliParser::ParserStatus::OK) & result) {
			hours = hrs.first;
		}

		snprintf(txt, 60, "\tCurrent time is %02d:%02d:%02d\n", hours, minutes, seconds);
		port.write(txt);

		return 0;
	}
};

/**
 * @brief   Main function of example CLI project. Consist of menu initialization and
 *          CLI chars reading loop.
 * @return  Maybe you are a golfer?
 */
int main(void) {
  debugPort.open(IODevice::ReadWrite);

  MainMenu _root(debugPort, "CLI example application.");

  SubMenu _clock("clock", "System clock management.");
  _root.addItem(_clock);

  SubMenu _car("car", "Set and check your car.");
  _root.addItem(_car);
  CarPrint carPrint;
  CarSet carSet;
  _car.addItem(carPrint);
  _car.addItem(carSet);

  ClockStatus cstat;
  _clock.addItem((MenuItem&) cstat);
  ClockSet cset;
  _clock.addItem((MenuItem&) cset);
  SubMenu _alarm("alarm", "Configure alarm.");
  _clock.addItem((MenuItem&) _alarm);

  SubMenu _emptySet("empty", "Just an empty set");
  _root.addItem(_emptySet);
  SubMenu _recursiveEmptySet("empty", "Recursive emtpy set");
  _emptySet.addItem(_recursiveEmptySet);

  AlarmOn aon;
  _alarm.addItem((MenuItem&) aon);
  AlarmOff aoff;
  _alarm.addItem((MenuItem&) aoff);

  SubMenu _memory("memory", "Store and read settings configuration.");
  _root.addItem(_memory);

  MemorySave ms;
  MemoryRestore mr;
  _memory.addItem(ms);
  _memory.addItem(mr);

  CLI cli(debugPort, _root, "\n\r---------------------------- CLI DEMO -----------------------------\n\r");

  std::chrono::milliseconds dura(100);
  while (1) {
    cli.readInput();
    std::this_thread::sleep_for(dura);
  }
  return 0;
}

/**
 * example
 * @}
 */

/**
 * cli
 * @}
 */

/**
 * components
 * @}
 */

/**
 * microHAL
 * @}
 */
