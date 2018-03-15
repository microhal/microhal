#!/usr/bin/env python2

from common import usbUtility as usb
import devicesConfig as interface
import serial

hardwareSerial = serial.Serial(usb.getSerialPortNameFromSerialNumber('0677FF494851877267034154'), 115200, timeout=0.5)

devices = ['sht21', 'pcf8563', 'mpl115a2', 'mag3110', 'mpl3115a2', 'm24c16', 'tmp006', 'isl29023']
targets = ['NUCLEO-F411RE', 'NUCLEO-F334R8', 'stm32f4-discovery']


def targetInfo(target):
    serialNumber = interface.getInterfaceSerialNumber(target)
    boardType = interface.getBoardType(target)
    serialName = usb.getSerialPortNameFromSerialNumber(serialNumber)
    
    print "Information about " + target
    print "\tOpenOCD configuration file: " + boardType
    print "\tSerial Number: " + str(serialNumber)
    print "\tSerial Port name: " + str(serialName)


def select(target, deviceName):    
    if (deviceName not in devices) or (target not in targets):
        return False     
        
    if target == 'NUCLEO-F411RE':
        hardwareSerial.write('1')        
    if target == 'NUCLEO-F334R8':
        hardwareSerial.write('2')
    if target == 'stm32f4-discovery':
        hardwareSerial.write('3')

    if deviceName == 'sht21':
        hardwareSerial.write('a')
    if deviceName == 'pcf8563':
        hardwareSerial.write('b')
    if deviceName == 'mpl115a2':
        hardwareSerial.write('c')
    if deviceName == 'mag3110':
        hardwareSerial.write('d')
    if deviceName == 'mpl3115a2':
        hardwareSerial.write('e')
    if deviceName == 'm24c16':
        hardwareSerial.write('f')
    if deviceName == 'tmp006':
        hardwareSerial.write('g')
    if deviceName == 'isl29023':
        hardwareSerial.write('h')
        
    return True  

        
        
        
