/* =========================================================================================== *//**
 @file        pcf8563.h
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       PCF8563 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Implements function for PCF8563

 *//* ============================================================================================ */

#ifndef __PCF8563_H__
#define __PCF8563_H__
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "microhal.h"
/**
 * @addtogroup Devices
 * @{
 * @class PCF8563
 * @}
 */
class PCF8563: public microhal::I2CDevice {
    enum Registers {
        //control registers
        CONTROL_STATUS_1 = 0x00,
        CONTROL_STATUS_2 = 0x01,
        //time registers
        VL_SECONDS = 0x02,
        MINUTES = 0x03,
        HOURS = 0x04,
        DAYS = 0x05,
        WEEKDAYS = 0x06,
        CENTURY_MONTHS = 0x07,
        YEARS = 0x08,
        //alarm registers
        MINUTE_ALARM = 0x09,
        HOUR_ALARM = 0x0A,
        DAY_ALARM = 0x0B,
        WEEKDAY_ALARM = 0x0C,
        //clk out register
        CLKOUT_CONTROL = 0x0D,
        //timer registers
        TIMER_CONTROL = 0x0E,
        TIMER = 0x0F
    };

    enum Flags {
        TIMER_INTERRUPT_ENABLE = 0x01,
        ALARM_INTERRUPT_ENABLE = 0x02,
        TIMER_INTERRUPT_FLAG = 0x04,
        ALARM_INTERRUPT_FLAG = 0x08,
        TIMER_ENABLE_FLAG = 0x80,

    };
public:
    typedef enum {
        SUCCESS = true, ///< value returned if function funished witch succes
        ERROR = false   ///< value returned by function if somethings goes wrong
    } Result;
    /**
     * @brief enumerator type for select timer source
     */
    typedef enum {
        TIMER_SOURCE_CLOCK_4096kHz = 0, ///< source clock set to 4096kHz
        TIMER_SOURCE_CLOCK_64Hz = 1, ///< source clock set to 64Hz
        TIMER_SOURCE_CLOCK_1Sec = 2, ///< source clock set to 1 Hz
        TIMER_SOURCE_CLOCK_1Min = 3, ///< source clock set to 1/60 Hz
    } TimerSourceClock;
    /**
     * @brief This structure is used to stored time read from PCF8563, or to set parameters of PCF8563, example: set alarm.
     */
    typedef struct {
        uint8_t sec;    ///< storage seconds
        uint8_t min;    ///< storage minutes
        uint8_t hour;   ///< storage hours
        uint8_t day;    ///< storage days
        uint8_t month;  ///< storage months
        uint8_t year;   ///< storage years
    } Time;

public:
    /**
     * @brief Default constructor for PCF8563 class
     * @param   i2cNr - numer of used I2C driver
     */
    PCF8563(microhal::I2C &i2c) :
            I2CDevice(i2c, 0xA2) {
    }
    Result setTimer(TimerSourceClock souceClock, uint8_t timeout);
    Result disableTimer();
    Result enableTimer();
    Result setAlarm(uint8_t min, uint8_t hour, uint16_t day, uint8_t week_day);
    Result getAlarm(Time &alarm);

    Result setTime(Time &time);
    Result getTime(Time &time);
private:
    void interruptFunction(void);
};
#endif
