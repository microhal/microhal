from common import flash
from common import buildTool
from common import resultPublisher
import devicesConfig as interface
import sys
import serial
import time
import os
import fnmatch

projectDir = sys.argv[1]
print projectDir

import imp

targets=['NUCLEO-F411RE', 'NUCLEO-F334R8', 'stm32f4-discovery', 'linux']
#targets=['NUCLEO-F411RE']
testSpecificProjects=[]
# lets find *.testconfig files in subdirectorys, if file is found lets add them into projectToTest list
projectsToTest = []
for root, subdirs, files in os.walk(projectDir):
    for filename in fnmatch.filter(files, '*.testconfig'):
        projectsToTest.append(os.path.join(root, filename))

print projectsToTest
result = []


def statusToText(status):   
	if status == True:
		return 'pass'
	return '\033[91m' + 'fail' + '\033[0m'

for i, project in enumerate(projectsToTest):
    test = imp.load_source('test.' + str(i), project)
    projectDir = os.path.dirname(project)
    for target in targets:
        if target in test.targets and (test.projectName() in testSpecificProjects or testSpecificProjects == []):
            print 'building project ' + test.projectName() + ', configuration ' + target
            if buildTool.build(projectDir, test.projectName() + '/' + target, []):
                if target != 'linux':
                    print '\tFlashing target.'
                    if flash.program(projectDir + '/' + target + '/' + test.binaryName(target), target):
                        print '\tHalting target.'
                        flash.resetHalt(target)
                        testPort = serial.Serial(interface.getSerialPort(target), 115200, timeout=0.5)
                        testPort.readall()
                        print '\tRunning target.'
                        flash.resetRun(target)
                        time.sleep(test.executionTime)
                        status = test.passed(testPort.readall())
                        testPort.close()
                        result.append({'testName':test.projectName(), 'target': target, 'status': statusToText(status)})
                    else:
                        result.append({'testName':test.projectName(), 'target': target, 'status': '\033[91m' + 'Flashing failed' + '\033[0m'})
                        print '\033[91m' + 'Flashing failed' + '\033[0m'
                else:                    
                    result.append({'testName':test.projectName(), 'target': target, 'status': 'build pass'})
                    #todo add running example
            else:
                result.append({'testName':test.projectName(), 'target': target, 'status': '\033[91m' + 'build fail' + '\033[0m'})
                print '\033[91m' + 'build fail' + '\033[0m'
       
resultPublisher.showResult(result)





