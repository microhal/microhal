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

targets=['NUCLEO-F411RE', 'NUCLEO-F334R8']

# lets find *.testconfig files in subdirectorys, if file is found lets add them into projectToTest list
projectsToTest = []
for root, subdirs, files in os.walk(projectDir):
    for filename in fnmatch.filter(files, '*.testconfig'):
        projectsToTest.append(os.path.join(root, filename))

print projectsToTest
result = []
for i, project in enumerate(projectsToTest):
    test = imp.load_source('test.' + str(i), project)
    projectDir = os.path.dirname(project)
    for target in targets:
        if target in test.targets:
            buildTool.build(projectDir, test.projectName() + '/' + target, [])
            flash.program(projectDir + '/' + target + '/' + test.binaryName(target), target)
            flash.resetHalt(target)
            testPort = serial.Serial(interface.getSerialPort(target), 115200, timeout=0.5)
            testPort.readall()
            flash.resetRun(target)
            time.sleep(test.executionTime)
            status = test.passed(testPort.readall())
            testPort.close()
            result.append({'testName':test.projectName(), 'target': target, 'status': status})
       
resultPublisher.showResult(result)
print result




