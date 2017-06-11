import unittest
import serial
import sys
import os
import random
import string
from tabulate import tabulate


def log(text):
	#print text
	return True

def runEclipse(project, target, defines):
	workspace = 'workspace_' + project.replace("\\s","_")    
	defs = ''
	for define in defines:
        	defs = defs + ' -D ' + define
	parameters = '--launcher.suppressErrors -nosplash -no-indexer' + defs + ' -data ' + workspace + ' -import "' + project + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + target + '"'
	#print parameters
	#subprocess.check_output("eclipse " + parameters, shell=True)
	return os.system("eclipse " + parameters)

def flash(boardType, binaryfile):
	parameters = '-f board/' + boardType + ' -c init -c targets -c reset -c halt -c "sleep 100" -c "flash write_image erase ' + binaryfile + '" -c "verify_image ' + binaryfile +'" -c "reset run" -c shutdown'
	#print 'openocd ' + parameters
	return os.system("openocd " + parameters)

def echoTest(controlSerial, testedSerials):
	controlSerial.write('-echo\r')
	log(controlSerial.read(100));
	testString = 'abcdefghij' #''.join(random.choice(string.lowercase) for i in range(101))
	
	for serial in testedSerials:
		serial.write(testString)
	result = [];
	status = True
	for serial in testedSerials:
		received = serial.read(1000)
		if received == testString:
			result.append(True)
			log('pass')
		else:
			result.append(False)
			status = False
			log('fail: ' + received)
			print 'fail: ' + serial.portstr + ' ' + received + ' != ' + testString
		
	controlSerial.write('end')
	return [status, result]

def baudrateTest(controlSerial, testedSerials, baudrates):
	result = []
	status = True
	for baudrate in baudrates:
		log("setting baudrate to: " + str(baudrate))
		for serial in testedSerials:
			serial.baudrate = baudrate
		controlSerial.write('-baudrate ' + str(baudrate) +'\r')
		log(controlSerial.read(100))
		[stat, res] = echoTest(controlSerial, testedSerials)
		result.append(res)
		if stat == False:
			status = False		
	return [status, result]

def parityTest(controlSerial, testedSerials, parities):
	parityList = {'none' : serial.PARITY_NONE, 'even': serial.PARITY_EVEN, 'odd': serial.PARITY_ODD}
	result = []
	status = True	
	for i, parity in enumerate(parities):
		log("setting parity to: " + parity)
		for ser in testedSerials:
			ser.parity = parityList[parity]
		controlSerial.write('-parity ' + parity +'\r')
		log(controlSerial.read(100))
		[stat, res] = echoTest(controlSerial, testedSerials)
		result.append(res)
		if stat == False:
			status = False
	return [status, result]

def stopBitsTest(controlSerial, testedSerials, stops):
	stopList = {'one' : serial.STOPBITS_ONE, 'onePointFive': serial.STOPBITS_ONE_POINT_FIVE, 'two': serial.STOPBITS_TWO}	
	result = []
	status = True	
	for stopBits in stops:
		log("setting stop bits to: " + stopBits)
		for ser in testedSerials:			
			ser.stopbits = stopList[stopBits]
		controlSerial.write('-stop ' + stopBits +'\r')
		log(controlSerial.read(100))
		[stat, res] = echoTest(controlSerial, testedSerials)
		result.append(res)
		if stat == False:	
			status = False
	return [status, result]


def statusToText(status):
	if status == True:
		return 'pass'
	return '\033[91m' + 'fail' + '\033[0m'


projdir = None

stopBitsResult = []

class SerialPortTest(unittest.TestCase): 
	@classmethod
	def setUpClass(self):	
       		print 'build project'
		runEclipse(projdir, 'serialPort_test/stm32f4-discovery', [])
		print 'falshing MCU'
		flash('stm32f4discovery_stlinkV2-1.cfg', projdir + '/stm32f4-discovery/serialPort_test.elf')
		
		self.serials = []
		self.serials.append(serial.Serial('/dev/ttyUSB0', 115200, timeout=0.5))
		self.serials.append(serial.Serial('/dev/ttyUSB1', 115200, timeout=0.5))
		self.serials.append(serial.Serial('/dev/ttyUSB2', 115200, timeout=0.5))
		
		self.controlSerial = self.serials[0]
		self.testedSerials = [self.serials[1], self.serials[2]]

	@classmethod
	def tearDownClass(self):
		stopStatus = []
		for i, value in enumerate(self.testedSerials):
			stopStatus.append([str(i), statusToText(stopBitsResult[0][i]), statusToText(stopBitsResult[1][i]), statusToText(stopBitsResult[2][i])])

		print '\n                   stop bits test'
		print tabulate(stopStatus, headers=['Serial Number', 'one', 'onePointFive', 'two'], tablefmt='orgtbl')  
	
	def test_BasicCommunication(self):
 		txt = "----------------------------- SerialPort Test -----------------------------\n"
		for port in self.serials:
			port.readline() # discard first line becouse may be incomplette			
			data = port.readline()
			self.assertEqual(txt, data)
	
		for i in range(1,2):
			for port in self.serials:
				port.readline() # discard first line becouse may be incomplette

	def test_baudrate(self):
		[status, baudrateResult]= baudrateTest(self.controlSerial, self.testedSerials, [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 576000, 921600, 1000000, 2000000])
	
		for ser in self.testedSerials:
			ser.baudrate = 115200
		self.controlSerial.write('-baudrate ' + str(115200) +'\r')

		baudrateStatus = []
		for i, value in enumerate(self.testedSerials):
			row = [str(i)]
			for j in range(0, len(baudrateResult)):
				row.append(statusToText(baudrateResult[j][i]))
			baudrateStatus.append(row)
		print '\n                     baudrate test'
		print tabulate(baudrateStatus, headers=['Serial Number', '1200', '2400', '4800', '9600', '19200', '38400', '57600', '115200', '230400', '460800', '576000', '921600', '1000000', '2000000'], tablefmt='orgtbl')
		self.assertTrue(status)

	def test_parity(self):
		[status, parityResult] = parityTest(self.controlSerial, self.testedSerials, ['none', 'even', 'odd'])

		for ser in self.testedSerials:
			ser.parity = serial.PARITY_NONE
		self.controlSerial.write('-parity none\r')
	
		parityStatus = []
		for i, value in enumerate(self.testedSerials):
			parityStatus.append([str(i), statusToText(parityResult[0][i]), statusToText(parityResult[1][i]), statusToText(parityResult[2][i])])

		print '\n                   parity test'
		print tabulate(parityStatus, headers=['Serial Number', 'none', 'even', 'odd'], tablefmt='orgtbl')
		self.assertTrue(status)

	def test_stopBits(self):
		stopBitsValues = ['one', 'onePointFive', 'two']
		[status, stopBitsResul] = stopBitsTest(self.controlSerial, self.testedSerials, stopBitsValues)
		global stopBitsResult
		stopBitsResult = stopBitsResul
		self.assertTrue(status)

	 


def main():
	global projdir
	projdir = sys.argv[1]	
	del sys.argv[1]
	unittest.main()

if __name__ == '__main__':
	main()
