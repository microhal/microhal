/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      DS2786 driver
 *
 * @authors    Pawel Okas
 * created on: 2015
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015-2017, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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
#include "ds2786.h"

bool DS2786::init(const CellModel &cellModel, uint32_t initialCellCapacity_mAh, int32_t currentOffsetBias_uA, uint16_t OCVThreshold_uA,
                  uint16_t OCVdVdtThreshold_uV) {
    if (!cellModel.isValid()) return false;
    if (OCVThreshold_uA > 6375 / Rsns) return false;

    ParameterEEPROMMemoryBlock parameter;
    parameter.currentOffsetBiasRegister = (currentOffsetBias_uA * Rsns) / 25000;
    parameter.capacity[0] = cellModel.capacity1;
    parameter.capacity[1] = cellModel.capacity2;
    parameter.capacity[2] = cellModel.capacity3;
    parameter.capacity[3] = cellModel.capacity4;
    parameter.capacity[4] = cellModel.capacity5;
    parameter.capacity[5] = cellModel.capacity6;
    parameter.capacity[6] = cellModel.capacity7;
    parameter.voltageBreakpoint[0] = (cellModel.voltageBreakpoint0 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[1] = (cellModel.voltageBreakpoint1 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[2] = (cellModel.voltageBreakpoint2 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[3] = (cellModel.voltageBreakpoint3 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[4] = (cellModel.voltageBreakpoint4 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[5] = (cellModel.voltageBreakpoint5 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[6] = (cellModel.voltageBreakpoint6 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[7] = (cellModel.voltageBreakpoint7 / 1220) << 4;  // in [uV]
    parameter.voltageBreakpoint[8] = (cellModel.voltageBreakpoint8 / 1220) << 4;  // in [uV]
    parameter.initialCapacityScallingFactor = calculateInitialCapacity(initialCellCapacity_mAh);
    parameter.blankingOCVCurrentTreshold = static_cast<uint32_t>(OCVThreshold_uA) * Rsns / 25;
    parameter.OCVdVdtThreshold = OCVdVdtThreshold_uV / 1220;

    if (write(Command::Rcall)) {
        ParameterEEPROMMemoryBlock tmp;
        readRegister(EEPROM_CURRENT_OFFSET, tmp.currentOffsetBiasRegister);
        readMultipleRegisters(tmp.capacity, EEPROM_CAPACITY_1, EEPROM_CAPACITY_2, EEPROM_CAPACITY_3, EEPROM_CAPACITY_4, EEPROM_CAPACITY_5,
                              EEPROM_CAPACITY_6, EEPROM_CAPACITY_7);
        readMultipleRegisters(tmp.voltageBreakpoint, EEPROM_VOLTAGE_BREAKPOINT_MSB_0, EEPROM_VOLTAGE_BREAKPOINT_MSB_1,
                              EEPROM_VOLTAGE_BREAKPOINT_MSB_2, EEPROM_VOLTAGE_BREAKPOINT_MSB_3, EEPROM_VOLTAGE_BREAKPOINT_MSB_4,
                              EEPROM_VOLTAGE_BREAKPOINT_MSB_5, EEPROM_VOLTAGE_BREAKPOINT_MSB_6, EEPROM_VOLTAGE_BREAKPOINT_MSB_7,
                              EEPROM_VOLTAGE_BREAKPOINT_MSB_8);
        readRegister(EEPROM_INITIAL_CAPACITY_SCALING, tmp.initialCapacityScallingFactor);
        readRegister(EEPROM_CURRENT_THRESHOLD, tmp.blankingOCVCurrentTreshold);
        readRegister(EEPROM_DV_DT_THRESHOLD, tmp.OCVdVdtThreshold);
        readRegister(EEPROM_I2C_ADDRESS, tmp.i2cAddressConfiguration);
        readRegister(EEPROM_LEARN_THRESHOLD, tmp.learnThreshold);
        readRegister(EEPROM_USER_VALUE, tmp.userEeprom);

        parameter.OCVdVdtThreshold |= (tmp.OCVdVdtThreshold & 0xF0);
        parameter.i2cAddressConfiguration = tmp.i2cAddressConfiguration;
        parameter.learnThreshold = tmp.learnThreshold;
        parameter.userEeprom = tmp.userEeprom;

        if (tmp == parameter) return true;

        writeRegister(EEPROM_CURRENT_OFFSET, parameter.currentOffsetBiasRegister);
        writeMultipleRegisters(parameter.capacity, EEPROM_CAPACITY_1, EEPROM_CAPACITY_2, EEPROM_CAPACITY_3, EEPROM_CAPACITY_4, EEPROM_CAPACITY_5,
                               EEPROM_CAPACITY_6, EEPROM_CAPACITY_7);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_0, parameter.voltageBreakpoint[0]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_1, parameter.voltageBreakpoint[1]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_2, parameter.voltageBreakpoint[2]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_3, parameter.voltageBreakpoint[3]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_4, parameter.voltageBreakpoint[4]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_5, parameter.voltageBreakpoint[5]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_6, parameter.voltageBreakpoint[6]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_7, parameter.voltageBreakpoint[7]);
        writeRegister(EEPROM_VOLTAGE_BREAKPOINT_MSB_8, parameter.voltageBreakpoint[8]);
        writeRegister(EEPROM_INITIAL_CAPACITY_SCALING, parameter.initialCapacityScallingFactor);
        writeRegister(EEPROM_CURRENT_THRESHOLD, parameter.blankingOCVCurrentTreshold);
        writeRegister(EEPROM_DV_DT_THRESHOLD, parameter.OCVdVdtThreshold);

        write(Command::Copy);
        std::this_thread::sleep_for(std::chrono::milliseconds{14});
        return clear(Command::Copy);
    }
    return false;
}
