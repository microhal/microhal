import os
import devicesConfig as interface

def program(fullPathToBinary, board):  
    fullPathToBinary = fullPathToBinary.replace(' ', '\ ')  
    boardType = interface.getBoardType(board)
    serial = interface.getInterfaceSerialNumber(board)

    parameters = '-f board/' + boardType + ' -c "hla_serial ' + serial + '" -c init -c targets -c "reset halt" -c "sleep 100" -c "flash write_image erase ' + fullPathToBinary + '" -c "verify_image ' + fullPathToBinary +'" -c "reset run" -c shutdown'	
    return os.system("/home/pawel/bin/microide/tools/openocd/0.10.0/bin/openocd " + parameters + " 2>> log.log") == 0

def resetHalt(board):
    boardType = interface.getBoardType(board)
    serial = interface.getInterfaceSerialNumber(board)

    parameters = '-f board/' + boardType + ' -c "hla_serial ' + serial + '" -c init -c targets -c "reset halt" -c shutdown'	
    return os.system("/home/pawel/bin/microide/tools/openocd/0.10.0/bin/openocd " + parameters + " 2>> log.log") == 0

def resetRun(board):
    boardType = interface.getBoardType(board)
    serial = interface.getInterfaceSerialNumber(board)

    parameters = '-f board/' + boardType + ' -c "hla_serial ' + serial + '" -c init -c targets -c "reset run" -c shutdown'	
    return os.system("/home/pawel/bin/microide/tools/openocd/0.10.0/bin/openocd " + parameters + " 2>> log.log") == 0
