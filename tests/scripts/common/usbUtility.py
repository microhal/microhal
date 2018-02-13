#!/usr/bin/env python2

import pyudev

def getSerialPortNameFromSerialNumber(serialNumber):
    context = pyudev.Context()
    for device in context.list_devices(subsystem='tty', ID_BUS='usb'):
        if device.get('ID_SERIAL_SHORT') == serialNumber:
            return device.get('DEVNAME')
