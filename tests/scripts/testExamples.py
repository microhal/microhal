from common import flash
from common import buildTool
from common import resultPublisher
from common import hardwareTesterController
import devicesConfig as interface
import sys
import serial
import time
import os
import fnmatch

import imp

disableColor = False

def colour(text, colourName):
    colourDict = {"Red": '\033[91m', 'Default' : '\033[0m'}
    global disableColor
    
    if disableColor == False:
        return colourDict[colourName] + text + colourDict['Default']
    else:
        return text


def findTestconfigFiles(projectDir):
    projectsToTest = []
    for root, subdirs, files in os.walk(projectDir):
        for filename in fnmatch.filter(files, '*.testconfig'):
            projectsToTest.append(os.path.join(root, filename))    
    return projectsToTest

def statusToText(status):   
	if status == True:
		return 'pass'
	return colour('fail', 'Red')#'\033[91m' + 'fail' + '\033[0m'
	
def createProjectsList(projectsToTest):
    projects = []
    for i, projectDir in enumerate(projectsToTest):
        projects.append(imp.load_source('test' + str(i), projectDir))
    return projects  
    
def flashAndRunApp(test, binary, target):    
    if test.requireDevice == True:
        if hardwareTesterController.select(target, test.deviceName) == False:
            return {'testName':test.projectName(), 'target': target, 'status': '\033[91m' + 'Missing device' + '\033[0m'}           
            
    result = None
    print '\tFlashing target.'
    if flash.program(binary, target):
        print '\tHalting target.'
        flash.resetHalt(target)
       
        testPort = serial.Serial(interface.getSerialPort(target), 115200, timeout=0.5)
        testPort.readall()
        print '\tRunning target.'
        flash.resetRun(target)
        time.sleep(test.executionTime) 
        dataLen = testPort.in_waiting
        portData = testPort.read(dataLen) # don't use readall(), it may hang this script when board will transmit data wery fast. Python is unable to read all data so readall is blocking all application.
        testPort.close()
        status = test.passed(portData)
        
        result = {'testName':test.projectName(), 'target': target, 'status': statusToText(status)}
    else:
        result = {'testName':test.projectName(), 'target': target, 'status': '\033[91m' + 'Flashing failed' + '\033[0m'}
        print colour('Flashing failed', 'Red')#'\033[91m' + 'Flashing failed' + '\033[0m'  
    return result
        
        
def runTests(projectsToTest, testSpecificProjects, targets, buildOnly):
    result = []
    for i, project in enumerate(projectsToTest):
        test = imp.load_source('test' + str(i), project)
        projectDir = os.path.dirname(project)
        for target in targets:
            if target in test.targets and (test.projectName() in testSpecificProjects or testSpecificProjects == []):
                print 'building project ' + test.projectName() + ', configuration ' + target
                if test.testType == 'nonstandard':
                    result.append(test.run(projectDir, target, buildTool))
                else:                   
                    buildStatus = buildTool.build(projectDir, test.projectName() + '/' + target, [])
                    if buildStatus == False or buildOnly == True:
                        if buildStatus == False:
                            result.append({'testName':test.projectName(), 'target': target, 'status': colour('build fail' , 'Red')})#'\033[91m' + 'build fail' + '\033[0m'})
                            print colour('build fail' , 'Red')#'\033[91m' + 'build fail' + '\033[0m'
                        else:
                            result.append({'testName':test.projectName(), 'target': target, 'status': 'build pass'})
                    else:                            
                        if target == 'linux' or test.requireHardware == False:
                            result.append({'testName':test.projectName(), 'target': target, 'status': 'build pass'})
                        else:
                            result.append(flashAndRunApp(test, projectDir + '/' + target + '/' + test.binaryName(target), target))
    return result                    

def envinronmentCheck():
    try:
        subprocess.Popen("eclipse")
    except OSError:
	print "Eclipse not fount"
        return False;
    
       
def main():    
    import argparse
    import datetime
    startTime = datetime.datetime.now()
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--searchDir", required=True, help="Path where script will be loking for *.testconfig files")
    parser.add_argument("--searchOnly", help="Find and display all projects to test but do not build or test them. When this option is used all other options will be ignored.", nargs='?', type=bool, const=True)
    parser.add_argument("--buildOnly", help="This command will skip running all examples. All examples will be tested only for build correctnes.", nargs='?', type=bool, const=True)
    parser.add_argument("--selectTargets", help="This command will run tests for targets that will be selected by this command.", nargs='+')
    parser.add_argument("--selectProjects", help="This command will run tests for projects that will be selected by this command.", nargs='+')
    parser.add_argument("--targetInfo", help="This command will show information about target.", nargs='?')
    parser.add_argument("--targetSelect", help="This command will connect target with device specified by --deviceSelect command.", nargs='?')
    parser.add_argument("--deviceSelect", help="This command will connect device with target specified by --targetSelect command.", nargs='?')
    parser.add_argument("--disableColor", help="This command will disable coloring text in terminal", nargs='?', type=bool, const=True)
    args = parser.parse_args()
    

    if args.disableColor:
        global disableColor
        disableColor = True

    if args.targetInfo != None:
        hardwareTesterController.targetInfo(args.targetInfo)
        return
        
    if args.targetSelect != None or args.deviceSelect != None:
        if args.targetSelect == None or args.deviceSelect == None:
            print "Argument missing both --targetSelect and --deviceSelect have to be passed."
            return
        if hardwareTesterController.select(args.targetSelect, args.deviceSelect) == False:
            print "Unable to connect target with device, target or device isn't available in hardware tester."
            print "Available targets: " + str(hardwareTesterController.targets)
            print "Available devices: " + str(hardwareTesterController.devices)
        return
    
    # lets find *.testconfig files in subdirectorys, if file is found lets add them into projectToTest list
    projectsToTest = findTestconfigFiles(args.searchDir)
    projects = createProjectsList(projectsToTest)
  
    # create sets that contain all possibles test names and targets
    testSpecificProjects = set()
    targets = set()    
    for project in projects:
        targets.update(project.targets)
        testSpecificProjects.add(project.projectName())    

    if args.selectProjects != None:
        testSpecificProjects = args.selectProjects            
    if args.selectTargets != None:
        targets = args.selectTargets

    if args.searchOnly == True:
        resultPublisher.showFoundProjects(projects)
    else:                    
        result = runTests(projectsToTest, testSpecificProjects, targets, args.buildOnly)
        resultPublisher.showResult(result)
    
    print "execution time: " + str(datetime.datetime.now() - startTime)
            
if __name__ == "__main__":
    main()
        





