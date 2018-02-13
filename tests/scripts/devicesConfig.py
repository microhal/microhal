#!/usr/bin/env python2

from common import usbUtility as usb

def getSerialPort(target):
    serialNumber = 0
    if target == 'stm32f4-discovery':
        serialNumber = 'A50285BI'
    else:
        serialNumber = getInterfaceSerialNumber(target)
    return usb.getSerialPortNameFromSerialNumber(serialNumber)

def getInterfaceSerialNumber(target):
    if target == 'NUCLEO-F411RE':
        return '0677FF494851877267034154'
    if target == 'NUCLEO-F334R8':
        return '0668FF545150656767231018'
    if target == 'stm32f4-discovery':
        return '0670FF485550755187151032'

def getBoardType(target):
    if target == 'NUCLEO-F411RE':
        return 'st_nucleo_f4.cfg'
    if target == 'NUCLEO-F334R8':
        return 'st_nucleo_f3.cfg'
    if target == 'stm32f4-discovery':
        return 'stm32f4discovery_stlinkV2-1.cfg'

