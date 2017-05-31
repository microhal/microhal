from subprocess import call
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
	os.system("eclipse " + parameters)
	return os.popen("arm-none-eabi-size " + project + '/jenkins/stm32f4xx_stat_serialPort.elf').read()

def createDefinesList(configuration, numberOfserialPorts):
	definesList = []
	definesList.append([])
	for i in range(1,numberOfserialPorts+1):
		defines = []
		for j in range(1, i+1): 
			defines.append('MICROHAL_USE_SERIAL_PORT' + str(j) + '_' + configuration)
		definesList.append(defines)
	return definesList

def buildConfiguration(config, numberOfserialPorts):
	definesList = createDefinesList(config, numberOfserialPorts)

	sizeOutput = []
	for defines in definesList:
		sizeOutput.append(runEclipse(projectDir, 'stm32f4xx_stat_serialPort/jenkins', defines))
       
	enabledSize = []
	for i in sizeOutput:
		(text, data, bss, dec, hex , filename) = i.splitlines()[1].split()
		enabledSize.append({'text' : int(text), 'data' : int(data), 'bss' : int(bss), 'dec' : int(dec), 'hex' : hex})
	return enabledSize



def saveDiffFile(filename, data, section):
	diffSize = []	
	for i, size in enumerate(data):
		if i == 0:
			diffSize.append(size[section])
		else:
			diffSize.append(size[section] - diffSize[i-1])

	with open(filename, 'w') as f:
		firstLine = ''
		secondLine = ''		
		i = 0
		for size in data:
			if i == 0:
				firstLine = 'no serial enabled'
				secondLine = str(diffSize[i])
			else:
				firstLine = firstLine + ', enabled ' + str(i)
				secondLine = secondLine + ', ' + str(diffSize[i])
			i = i + 1
		firstLine = firstLine + '\n'
		f.write(firstLine)
		f.write(secondLine)

def saveFile(filename, data, section):
	with open(filename, 'w') as f:
		firstLine = ''
		secondLine = ''		
		i = 0
		for size in data:
			if i == 0:
				firstLine = 'no serial enabled'
				secondLine = str(size[section])
			else:
				firstLine = firstLine + ', enabled ' + str(i)
				secondLine = secondLine + ', ' + str(size[section])
			i = i + 1
		firstLine = firstLine + '\n'
		f.write(firstLine)
		f.write(secondLine)	

pollingSize = buildConfiguration('POLLING', 8)
saveFile('polling_text_size.txt', pollingSize, 'text')
saveFile('polling_data_size.txt', pollingSize, 'data')
saveFile('polling_bss_size.txt', pollingSize, 'bss')

saveDiffFile('output_Diff_text.txt', pollingSize, 'text')

interruptSize = buildConfiguration('INTERRUPT', 8)
saveFile('interrupt_text_size.txt', interruptSize, 'text')
saveFile('interrupt_data_size.txt', interruptSize, 'data')
saveFile('interrupt_bss_size.txt', interruptSize, 'bss')

dmaSize = buildConfiguration('DMA', 6)
saveFile('dma_text_size.txt', dmaSize, 'text')
saveFile('dma_data_size.txt', dmaSize, 'data')
saveFile('dma_bss_size.txt', dmaSize, 'bss')
