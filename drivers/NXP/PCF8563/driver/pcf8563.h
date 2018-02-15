/* =========================================================================================== */ /**
 @file        pcf8563.h
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       PCF8563 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Implements function for PCF8563

 */ /* ============================================================================================ */

#ifndef __PCF8563_H__
#define __PCF8563_H__
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "I2CDevice/I2CDevice.h"
#include "microhal.h"
/**
 * @addtogroup Devices
 * @{
 * @class PCF8563
 * @}
 */
class PCF8563 : public microhal::I2CDevice {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;

    // control registers
    static constexpr auto CONTROL_STATUS_1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x00>{});
    static constexpr auto CONTROL_STATUS_2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x01>{});
    // time registers
    static constexpr auto VL_SECONDS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x02>{});
    static constexpr auto MINUTES = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x03>{});
    static constexpr auto HOURS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x04>{});
    static constexpr auto DAYS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x05>{});
    static constexpr auto WEEKDAYS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x06>{});
    static constexpr auto CENTURY_MONTHS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x07>{});
    static constexpr auto YEARS = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x08>{});
    // alarm registers
    static constexpr auto MINUTE_ALARM = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x09>{});
    static constexpr auto HOUR_ALARM = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x0A>{});
    static constexpr auto DAY_ALARM = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x0B>{});
    static constexpr auto WEEKDAY_ALARM = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x0C>{});
    // clk out register
    static constexpr auto CLKOUT_CONTROL = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x0D>{});
    // timer registers
    static constexpr auto TIMER_CONTROL = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x0E>{});
    static constexpr auto TIMER = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x0F>{});

    enum Flags {
        TIMER_INTERRUPT_ENABLE = 0x01,
        ALARM_INTERRUPT_ENABLE = 0x02,
        TIMER_INTERRUPT_FLAG = 0x04,
        ALARM_INTERRUPT_FLAG = 0x08,
        TIMER_ENABLE_FLAG = 0x80,

    };

 public:
    typedef enum {
        SUCCESS = true,  ///< value returned if function finished witch success
        ERROR = false    ///< value returned by function if something goes wrong
    } Result;
    /**
     * @brief enumerator type for select timer source
     */
    typedef enum {
        TIMER_SOURCE_CLOCK_4096kHz = 0,  ///< source clock set to 4096kHz
        TIMER_SOURCE_CLOCK_64Hz = 1,     ///< source clock set to 64Hz
        TIMER_SOURCE_CLOCK_1Sec = 2,     ///< source clock set to 1 Hz
        TIMER_SOURCE_CLOCK_1Min = 3,     ///< source clock set to 1/60 Hz
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
    typedef std::chrono::seconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<PCF8563, duration> time_point;

    static constexpr bool is_steady = true;
    /**
     * @brief Default constructor for PCF8563 class
     * @param   i2cNr - number of used I2C driver
     */
    PCF8563(microhal::I2C &i2c) : I2CDevice(i2c, 0xA2) {}
    Result setTimer(TimerSourceClock souceClock, uint8_t timeout);
    Result disableTimer();
    Result enableTimer();
    Result setAlarm(uint8_t min, uint8_t hour, uint16_t day, uint8_t week_day);
    Result getAlarm(Time &alarm);

    Result setTime(Time &time);
    Result getTime(Time &time);

    time_point now() {}

 private:
    void interruptFunction(void);
};
#endif
