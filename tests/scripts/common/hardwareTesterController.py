#!/usr/bin/env python2

from common import usbUtility as usb
import serial




hardwareSerial = serial.Serial(usb.getSerialPortNameFromSerialNumber('0677FF494851877267034154'), 115200, timeout=0.5)

def select(deviceName):
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
        
        
        
        
