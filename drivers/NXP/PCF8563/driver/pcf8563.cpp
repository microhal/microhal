/* =========================================================================================== *//**
 @file        pcf8563.cpp
 @authors     Paweł Okas
 @version     $Id$
 @package
 @brief       PCF8563 driver
 @hardware    none
 @copyright   $Copyright$
 @details

 *//* ============================================================================================ */

/*****************************************************************************
 * INCLUDES
 */
#include "pcf8563.h"

uint8_t BCDtoUnt8_t(uint8_t bcd);
uint8_t uint8_tToBCD(uint8_t i);

/**
 * @brief    This function sets timer value and enable PCF8563 timer
 *
 * @param	souceClock  select clock source
 * @param	timeout
 *
 * @retval  SUCCES if timer was set
 * @retval  ERROR if an error occurred
 */
PCF8563::Result PCF8563::setTimer(PCF8563::TimerSourceClock sourceClock, uint8_t timeout)
{
    uint8_t buff[] = { TIMER_ENABLE_FLAG | sourceClock,     // set clock source and enable timer
    timeout };                                  // set timeout
    //write buffer to device and return write status
    return (Result) writeRegisters(TIMER_CONTROL, buff, sizeof(buff));
}
/**
 * @brief   This function disable PCD8563 timer
 *
 * @param   none
 *
 * @retval  SUCCES if timer was disabled
 * @retval  ERROR if error occurred
 */
PCF8563::Result PCF8563::disableTimer()
{
    uint8_t tmp;

    if (readRegister(TIMER_CONTROL, tmp) == false) {
        return ERROR;   //read control register failed, return error
    }

    tmp &= ~TIMER_ENABLE_FLAG;  //clear timer enable flag
    return (Result) writeRegister(TIMER_CONTROL, tmp);  //applay settings
}
/**
 * @brief   This function enable PCF8563 timer
 *
 * @param   none
 *
 * @retval  SUCCES if timer was enabled
 * @retval  ERROR if an error occurred
 */
PCF8563::Result PCF8563::enableTimer()
{
    uint8_t tmp;

    if (readRegister(TIMER_CONTROL, tmp) == false) {
        return ERROR;   //read control register failed, return error
    }

    tmp |= TIMER_ENABLE_FLAG;  //set timer enable flag
    return (Result) writeRegister(TIMER_CONTROL, tmp);  //applay settings
}
/**
 * @brief    This function sets alarm stored in parameters
 *
 * @param	min -
 * @param	hour
 * @param	day
 * @param	week_day
 *
 * @retval  SUCCES if alarm was set
 * @retval  ERROR if an error occurred
 */
PCF8563::Result PCF8563::setAlarm(uint8_t min, uint8_t hour, uint16_t day, uint8_t weekday)
{
    uint8_t buffer[] = { uint8_tToBCD(min), //convert minutes to BCD and storage in buffer
    uint8_tToBCD(hour), //convert hours to BCD and storage in buffer
    uint8_tToBCD(day), //convert day to BCD and storage in buffer
    uint8_tToBCD(weekday) }; //convert weekday to BCD and storage in buffer

    //write alarm to device and return write status
    return (Result) writeRegisters(MINUTE_ALARM, buffer, sizeof(buffer));
}
/**
 * @brief    This function gets actual alarm set.
 *
 * @param    alarm
 *
 * @retval  SUCCES if alarm was read
 * @retval  ERROR if an error occurred
 */
PCF8563::Result PCF8563::getAlarm(PCF8563::Time &alarm)
{
    uint8_t buffer[4];
    //read actual alarm
    if (readRegisters(MINUTE_ALARM, buffer, sizeof(buffer)) == false) {
        return ERROR;
    }
    alarm.sec = 0;  //this value isn't used for alarm generation
    alarm.min = BCDtoUnt8_t(buffer[0] & 0x7F); //convert minutes form BCD to uint8_t and storage in Time structure
    alarm.hour = BCDtoUnt8_t(buffer[1] & 0x3F); //convert hours form BCD to uint8_t and storage in Time structure
    alarm.day = BCDtoUnt8_t(buffer[2] & 0x7F); //convert day form BCD to uint8_t and storage in Time structure
    //alarm.weekends in buffer[3];    //convert weekday form BCD to uint8_t and storage in Time structure
    alarm.month = 0;    //this value isn't used for alarm generation
    alarm.year = 0;     //this value isn't used for alarm generation

    return SUCCESS;
}
/**
 *@brief    This function sends time from parameter to PCF8563
 *
 *@param	time
 *
 *@return	PCF8563_Result
 */
PCF8563::Result PCF8563::setTime(PCF8563::Time &time)
{
    uint8_t buffer[] = { uint8_tToBCD(time.sec), uint8_tToBCD(time.min), uint8_tToBCD(time.hour),
            uint8_tToBCD(time.day), uint8_tToBCD(0x00), //weekends
            uint8_tToBCD(time.month), uint8_tToBCD(time.year) };

    //write actual time and return writing status
    return (Result) writeRegisters(VL_SECONDS, buffer, sizeof(buffer));
}
/**
 * @brief    This function read time from PCF8563 device and fill time structure actual data
 *
 * @param	time
 *
 * @retval  SUCCES if time was read
 * @retval  ERROR if error occurred
 */
PCF8563::Result PCF8563::getTime(PCF8563::Time& time)
{
    uint8_t buffer[7];
    //read time from PCF8563
    if (readRegisters(VL_SECONDS, buffer, 0x07) == false) {
        return ERROR;   //is reading failed then return ERROR
    }

    time.sec = BCDtoUnt8_t(buffer[0] & 0x7F); //convert seconds form BCD to uint8_t and storage in Time structure
    time.min = BCDtoUnt8_t(buffer[1] & 0x7F); //convert minutes form BCD to uint8_t and storage in Time structure
    time.hour = BCDtoUnt8_t(buffer[2] & 0x3F); //convert hours form BCD to uint8_t and storage in Time structure
    time.day = BCDtoUnt8_t(buffer[3] & 0x7F); //convert days form BCD to uint8_t and storage in Time structure
    //alarm.weekends in buffer[4];    //convert weekday form BCD to uint8_t and storage in Time structure
    time.month = BCDtoUnt8_t(buffer[5] & 0x7F); //convert month form BCD to uint8_t and storage in Time structure
    time.year = BCDtoUnt8_t(buffer[6] & 0x7F); //convert year form BCD to uint8_t and storage in Time structure

   // hal::Debug::debug() << "\n\rPCF: odczytano czas: ";
   // hal::Debug::debug() << time.hour << ":" << time.min << ":" << time.sec;

    return SUCCESS;
}
/**
 * @brief    this function should be connected to external interrupt generated by pcf8563 INT pin
 *
 * @param    none
 *
 * @return   none
 */
void PCF8563::interruptFunction(void)
{
    uint8_t controlStatus2, newControlStatus2;
    //get interrupt source information
    if (readRegister(CONTROL_STATUS_2, controlStatus2) == false) {
        return;
    }

    newControlStatus2 = controlStatus2 | TIMER_INTERRUPT_FLAG | ALARM_INTERRUPT_FLAG;
    if (controlStatus2 & TIMER_INTERRUPT_FLAG) {
        //clear timer interrupt flag
        newControlStatus2 &= ~TIMER_INTERRUPT_FLAG;
        //todo emit
    }
    if (controlStatus2 & ALARM_INTERRUPT_FLAG) {
        //clear alarm interrupt flag
        newControlStatus2 &= ~ALARM_INTERRUPT_FLAG;
        //todo emit
    }
    //set new settings
    writeRegister(CONTROL_STATUS_2, newControlStatus2);
}
/**
 * @brief       function provides conversion between BCD and uint8_t format
 *
 * @param		bcd number in BCD format
 *
 * @return      number converted to uint8_t
 */
uint8_t BCDtoUnt8_t(uint8_t bcd)
{
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}
/**
 * @brief       function provides conversion between uint8_t and BCD format
 *
 * @param		num value in uint8_t format
 *
 * @return      value in BCD format
 */
uint8_t uint8_tToBCD(uint8_t num)
{
    return (((num / 10) << 4) | (num % 10));
}
