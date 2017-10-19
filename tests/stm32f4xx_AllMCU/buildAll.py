from subprocess import call
from tabulate import tabulate
import subprocess
import sys
import os

projectDir = sys.argv[1]
print projectDir

def runEclipse(project, target, defines):
	workspace = 'workspace_' + project.replace("\\s","_")    
	defs = ''
	for define in defines:
        	defs = defs + ' -D ' + define
	parameters = '--launcher.suppressErrors -nosplash -no-indexer' + defs + ' -data ' + workspace + ' -import "' + project + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + target + '"'
	print parameters
	#subprocess.check_output("eclipse " + parameters, shell=True)
	return os.system("eclipse " + parameters)



def buildImplementation(impl, targets):
	buildStatus = []
	for target in targets:
 		buildStatus.append(runEclipse(projectDir, 'stm32f4xx_allMCU/' + impl, [target]))		
	return buildStatus

def statusToText(status):
	if status == 0:
		return 'pass'
	return 'fail'


targets = ['STM32F405xx', 'STM32F415xx', 'STM32F407xx', 'STM32F417xx', 'STM32F427xx', 'STM32F437xx', 'STM32F429xx', 'STM32F439xx', 'STM32F401xC', 'STM32F401xE', 'STM32F410Tx', 'STM32F410Cx', 'STM32F410Rx', 'STM32F411xE', 'STM32F446xx', 'STM32F469xx', 'STM32F479xx', 'STM32F412Cx', 'STM32F412Zx', 'STM32F412Vx', 'STM32F412Rx']
#targets = ['STM32F429xx']

pollingBuildStatus = buildImplementation('polling', targets)
interruptBuildStatus = buildImplementation('interrupt', targets)
dmaBuildStatus = buildImplementation('dma', targets)


buildStatus = []
for i, target in enumerate(targets):
	buildStatus.append([target, statusToText(pollingBuildStatus[i]), statusToText(interruptBuildStatus[i]), statusToText(dmaBuildStatus[i])])

print tabulate(buildStatus, headers=['Target', 'Polling', 'Interrupt', 'DMA'], tablefmt='orgtbl')
print '\n'
