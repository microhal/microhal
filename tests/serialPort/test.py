import unittest
import subprocess
import shlex
import serial

class SerialPortTest(unittest.TestCase):
	def setUp(self):
		self.serial = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

	def testOne(self):
		subprocess.call(shlex.split('./flash.sh stm32f4-discovery'))

		txt = "----------------------------- SerialPort Test -----------------------------\n"
		self.serial.readline() # discard first line becouse may be incomplette

		line = self.serial.readline()
		self.assertEqual(txt, line)

def main():
	unittest.main()

if __name__ == '__main__':
	main()
