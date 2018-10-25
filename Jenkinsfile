#!/usr/bin/env groovy

def projDirMap = [
	'diagnostic' : 'examples/diagnostic',
	'externalInterrupt' : 'examples/externalInterrupt',
	'gpio' : 'examples/gpio',
	'os' : 'examples/os',
	'serialPort' : 'examples/serialPort',
	'signal slot' : 'examples/signalSlot',
	'ticToc' : 'examples/ticToc',
	'cli' : 'components/cli/examples/cli',
	'hostComm' : 'components/hostComm/examples/simpeCommunication',
	'at45db' : 'drivers/Atmel/AT45DB041D/example',        
	'bmp180' : 'drivers/Bosch/BMP180/example/basic',
	'ds2782' : 'drivers/MaximDallas/DS2782/example/basic',
	'ds2786' : 'drivers/MaximDallas/DS2786/example/basic',        
	'hx711' : 'drivers/AVIA Semiconductor/HX711/example',
	'isl29023' : 'drivers/Intersil/ISL29023/example/basic',        
	'leptonPCSerialReceiver' : 'drivers/FLIR/Lepton/example/lepton_PC_Serial_Receiver',
	'leptonSDCardWrite' : 'drivers/FLIR/Lepton/example/lepton_SDCard_write',
	'leptonSerialTransmitter' : 'drivers/FLIR/Lepton/example/lepton_Serial_Transmitter',
	'lis302' : 'drivers/STMicroelectronics/LIS302/example/lis302',
	'lsm330dl' : 'drivers/STMicroelectronics/LSM330DL/example/basic',
	'm24c16' : 'drivers/STMicroelectronics/M24C16/example',
	'mcp9800' : 'drivers/Microchip/MCP9800/example',
	'mpl115a1' : 'drivers/Freescale Semiconductor/MPL115A1/example',
	'mpl115a2' : 'drivers/Freescale Semiconductor/MPL115A2/example',
	'mrf89xa' : 'drivers/Microchip/MRF89XA/example/mrf89xa',
	'pcf8563' : 'drivers/NXP/PCF8563/example',
	'rfm70' : 'drivers/Hoperf Electronic/RFM70/example/packet_send',
	'sht21' : 'drivers/Sensirion/SHT21/example/basic',
	'tmp006' : 'drivers/Texas Instruments/TMP006/example/temperature_show',
	'uCAM-II' : 'drivers/4D Systems/uCAM-II/example',
	'ws2812' : 'drivers/Worldsemi/WS2812/example',
	'serialPort_test' : 'tests/serialPort',
	'stm32f3xx_allMCU' : 'tests/stm32f3xx_AllMCU',
	'stm32f4xx_allMCU' : 'tests/stm32f4xx_AllMCU',
        'stm32f4xx_stat_serialPort' : 'tests/statistics/stm32f4xx/serialPort',
        'stm32f4xx_stat_i2c' : 'tests/statistics/stm32f4xx/i2c',
        'stm32f4xx_stat_spi' : 'tests/statistics/stm32f4xx/spi',
]

def eclipseRun(project, target, defines) {
    def workspace = 'workspace_' + project.replaceAll("\\s","_")    
    def defs = ''
    for (define in defines) {
        defs = defs + ' -D ' + define
    }	    
    if (env.NODE_NAME == 'master') {
        lock(label: 'master_core', quantity: 1) {
             withEnv(['PATH+WHATEVER=/srv/jenkins/tools/microide:/srv/jenkins/tools/microide/toolchains/arm-none-eabi-gcc/microhal/gcc-arm-none-eabi-5_3-2016q1/bin']) {
                retry(2) {
                    timeout(time:10, unit:'MINUTES') {			
                        sh 'eclipse -configuration /srv/jenkins --launcher.suppressErrors -nosplash -no-indexer' + defs + ' -data ' + workspace + ' -import "' + project + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + target + '"'
                    }
                }  
            }	
        }
    }
    if (env.NODE_NAME == 'FX160_HardwareTester') {
        lock(label: 'hwTester_core', quantity: 1) {
            withEnv(['PATH+WHATEVER=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:/var/jenkins/tools/microide/eclipse']) {
                retry(2) {
                    timeout(time:10, unit:'MINUTES') {
                        sh 'eclipse --launcher.suppressErrors -nosplash -no-indexer' + defs + ' -data ' + workspace + ' -import "' + project + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + target + '"'                        
                    }
                }
            }
        }
    }
}

def eclipseBuild(projName, targets) {     
	def projDirMap = [
	'diagnostic' : 'examples/diagnostic',
	'externalInterrupt' : 'examples/externalInterrupt',
	'gpio' : 'examples/gpio',
	'os' : 'examples/os',
	'serialPort' : 'examples/serialPort',
	'signal slot' : 'examples/signalSlot',
	'ticToc' : 'examples/ticToc',
	'cli' : 'components/cli/examples/cli',
	'hostComm' : 'components/hostComm/examples/simpeCommunication',
	'at45db' : 'drivers/Atmel/AT45DB041D/example',        
	'bmp180' : 'drivers/Bosch/BMP180/example/basic',
	'ds2782' : 'drivers/MaximDallas/DS2782/example/basic',
	'ds2786' : 'drivers/MaximDallas/DS2786/example/basic',        
	'hx711' : 'drivers/AVIA Semiconductor/HX711/example',
	'isl29023' : 'drivers/Intersil/ISL29023/example/basic',        
	'leptonPCSerialReceiver' : 'drivers/FLIR/Lepton/example/lepton_PC_Serial_Receiver',
	'leptonSDCardWrite' : 'drivers/FLIR/Lepton/example/lepton_SDCard_write',
	'leptonSerialTransmitter' : 'drivers/FLIR/Lepton/example/lepton_Serial_Transmitter',
	'lis302' : 'drivers/STMicroelectronics/LIS302/example/lis302',
	'lsm330dl' : 'drivers/STMicroelectronics/LSM330DL/example/basic',
	'm24c16' : 'drivers/STMicroelectronics/M24C16/example',
	'mcp9800' : 'drivers/Microchip/MCP9800/example',
	'mpl115a1' : 'drivers/Freescale Semiconductor/MPL115A1/example',
	'mpl115a2' : 'drivers/Freescale Semiconductor/MPL115A2/example',
	'mrf89xa' : 'drivers/Microchip/MRF89XA/example/mrf89xa',
	'pcf8563' : 'drivers/NXP/PCF8563/example',
	'rfm70' : 'drivers/Hoperf Electronic/RFM70/example/packet_send',
	'sht21' : 'drivers/Sensirion/SHT21/example/basic',
	'tmp006' : 'drivers/Texas Instruments/TMP006/example/temperature_show',
	'uCAM-II' : 'drivers/4D Systems/uCAM-II/example',
	'ws2812' : 'drivers/Worldsemi/WS2812/example',
	'serialPort_test' : 'tests/serialPort',
	'stm32f3xx_allMCU' : 'tests/stm32f3xx_AllMCU',
	'stm32f4xx_allMCU' : 'tests/stm32f4xx_AllMCU',
]
    echo "Building on ${env.NODE_NAME}"
    for (target in targets) {
        def buildTarget = projName + '/' + target
	if (target == 'all') {
	    buildTarget = 'all'
        }
        eclipseRun(projDirMap[projName], buildTarget, [])
    }	
}

def sa(projName, targets) {
    def projDirMap = [
        'diagnostic' : 'examples/diagnostic',
        'externalInterrupt' : 'examples/externalInterrupt',
        'gpio' : 'examples/gpio',
        'os' : 'examples/os',
        'serialPort' : 'examples/serialPort',
        'signal slot' : 'examples/signalSlot',
        'ticToc' : 'examples/ticToc',
        'cli' : 'components/cli/examples/cli',
        'hostComm' : 'components/hostComm/examples/simpeCommunication',
        'at45db' : 'drivers/Atmel/AT45DB041D/example',        
        'bmp180' : 'drivers/Bosch/BMP180/example/basic',
        'ds2782' : 'drivers/MaximDallas/DS2782/example/basic',
        'ds2786' : 'drivers/MaximDallas/DS2786/example/basic',        
        'hx711' : 'drivers/AVIA Semiconductor/HX711/example',
        'isl29023' : 'drivers/Intersil/ISL29023/example/basic',        
        'leptonPCSerialReceiver' : 'drivers/FLIR/Lepton/example/lepton_PC_Serial_Receiver',
        'leptonSDCardWrite' : 'drivers/FLIR/Lepton/example/lepton_SDCard_write',
        'leptonSerialTransmitter' : 'drivers/FLIR/Lepton/example/lepton_Serial_Transmitter',
        'lis302' : 'drivers/STMicroelectronics/LIS302/example/lis302',
        'lsm330dl' : 'drivers/STMicroelectronics/LSM330DL/example/basic',
        'm24c16' : 'drivers/STMicroelectronics/M24C16/example',
	'mcp9800' : 'drivers/Microchip/MCP9800/example',
        'mpl115a1' : 'drivers/Freescale Semiconductor/MPL115A1/example',
        'mpl115a2' : 'drivers/Freescale Semiconductor/MPL115A2/example',
        'mrf89xa' : 'drivers/Microchip/MRF89XA/example/mrf89xa',
        'pcf8563' : 'drivers/NXP/PCF8563/example',
        'rfm70' : 'drivers/Hoperf Electronic/RFM70/example/packet_send',
        'sht21' : 'drivers/Sensirion/SHT21/example/basic',
        'tmp006' : 'drivers/Texas Instruments/TMP006/example/temperature_show',
        'uCAM-II' : 'drivers/4D Systems/uCAM-II/example',
        'ws2812' : 'drivers/Worldsemi/WS2812/example',
        'serialPort_test' : 'tests/serialPort',
	'stm32f3xx_allMCU' : 'tests/stm32f3xx_AllMCU',
    ]
    if (env.NODE_NAME == 'master') {
        lock(label: 'master_core', quantity: 1) {
            withEnv(['PATH+WHATEVER=/srv/jenkins/tools/microide:/srv/jenkins/tools/microide/toolchains/arm-none-eabi-gcc/microhal/gcc-arm-none-eabi-5_3-2016q1/bin']) {
                for (target in targets) {               
                    timeout(time:15, unit:'MINUTES') {
                        sh '''#!/bin/bash
                        echo -target arm-none-eabi > extra_clang_options
                        echo -/third-party* > skipfile
                        source /srv/jenkins/tools/codechecker/venv/bin/activate 
                        export PATH=/srv/jenkins/tools/codechecker/build/CodeChecker/bin:$PATH 
                        export PATH=/srv/jenkins/tools/microide/toolchains/arm-none-eabi-gcc/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:$PATH
                        CodeChecker check --saargs extra_clang_options --skip skipfile -j 2 -e alpha -e llvm -n ''' + projName.replaceAll("\\s","_") + '_' + target + ' -b "cd ' + projDirMap[projName] + '/' + target +' && make clean && make all"'                    
		    }
                }        
            }
        }
    }
   if (env.NODE_NAME == 'FX160_HardwareTester') {
       lock(label: 'hwTester_core', quantity: 1) {
           withEnv(['PATH+WHATEVER=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:/var/jenkins/tools/microide/eclipse']) {
               for (target in targets) {               
                   timeout(time:10, unit:'MINUTES') {
                       sh '''#!/bin/bash
                       echo -target arm-none-eabi > extra_clang_options
                       echo -/third-party* > skipfile
                       source /var/jenkins/tools/codechecker/venv/bin/activate 
                       export PATH=/var/jenkins/tools/codechecker/build/CodeChecker/bin:$PATH 
                       export PATH=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:$PATH
                       CodeChecker check --saargs extra_clang_options --skip skipfile -j 2 -e alpha -e llvm -n ''' + projName.replaceAll("\\s","_") + '_' + target + ' -b "cd ' + projDirMap[projName] + '/' + target +' && make clean && make all"'                    
		   }
                }        
            }
        }
    }
}

def targets = ['stm32f4-discovery', 'NUCLEO-F411RE', 'NUCLEO-F334R8']
pipeline {   
    agent {
        node {
            label 'microhal'
        }    
    }
    environment {
        PATH = "/var/jenkins/tools/microide-0.3.4/eclipse:/var/jenkins/tools/microide-0.3.4/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-7-2018-q2-update/bin:$PATH"
    }
    stages {
        stage('Checkout') {
            steps {
               	checkout scm
                sh 'git submodule update --init'               
            }
        }
        stage('Build microhal examples') {
            steps {
                sh 'python tests/scripts/testExamples.py -d examples/ --buildOnly'
            }
        }
        stage('Build components examples') {
            steps {
                sh 'python tests/scripts/testExamples.py -d components/ --buildOnly'
            }
        }
        stage('Build devices examples') {
            steps {            
                sh 'python tests/scripts/testExamples.py -d drivers/ --buildOnly'
            }             
        }
        stage('Test - build all targets') {
            steps {
		eclipseBuild('stm32f3xx_allMCU', ['all'])	
                sh 'python ' + projDirMap['stm32f4xx_allMCU'] +'/buildAll.py ' + projDirMap['stm32f4xx_allMCU']            
            }
        }  
        stage('Test - code size statistics') {
	    steps {
                sh 'python ' + projDirMap['stm32f4xx_stat_serialPort'] +'/generateStats.py ' + projDirMap['stm32f4xx_stat_serialPort']
                sh 'python ' + projDirMap['stm32f4xx_stat_i2c'] +'/generateStats.py ' + projDirMap['stm32f4xx_stat_i2c']
                sh 'python ' + projDirMap['stm32f4xx_stat_spi'] +'/generateStats.py ' + projDirMap['stm32f4xx_stat_spi']
            }	
            post {
                 success {
                     build job: 'plots_stm32f4xx', parameters: [string(name: 'serialPort_interrupt_size', value: readFile('interrupt_text_size.txt')), string(name: 'serialPort_interrupt_diff_size', value: readFile('outputDiff_interrupt.txt')), string(name: 'serialPort_polling_diff_size', value: readFile('output_Diff_text.txt')), string(name: 'serialPort_interrupt_text_size', value: readFile('interrupt_text_size.txt')), string(name: 'serialPort_interrupt_data_size', value: readFile('interrupt_data_size.txt')), string(name: 'serialPort_interrupt_bss_size', value: readFile('interrupt_bss_size.txt')), string(name: 'serialPort_polling_text_size', value: readFile('polling_text_size.txt')), string(name: 'serialPort_polling_data_size', value: readFile('polling_data_size.txt')), string(name: 'serialPort_polling_bss_size', value: readFile('polling_bss_size.txt')), string(name: 'serialPort_dma_text_size', value: readFile('dma_text_size.txt')), string(name: 'serialPort_dma_data_size', value: readFile('dma_data_size.txt')), string(name: 'serialPort_dma_bss_size', value: readFile('dma_bss_size.txt')), string(name: 'i2c_interrupt_text_size', value: readFile('i2c_interrupt_text_size.txt')), string(name: 'i2c_interrupt_data_size', value: readFile('i2c_interrupt_data_size.txt')), string(name: 'i2c_interrupt_bss_size', value: readFile('i2c_interrupt_bss_size.txt')), string(name: 'i2c_polling_text_size', value: readFile('i2c_polling_text_size.txt')), string(name: 'i2c_polling_data_size', value: readFile('i2c_polling_data_size.txt')), string(name: 'i2c_polling_bss_size', value: readFile('i2c_polling_bss_size.txt')), string(name: 'i2c_dma_text_size', value: readFile('i2c_dma_text_size.txt')), string(name: 'i2c_dma_data_size', value: readFile('i2c_dma_data_size.txt')), string(name: 'i2c_dma_bss_size', value: readFile('i2c_dma_bss_size.txt')), string(name: 'spi_interrupt_text_size', value: readFile('spi_interrupt_text_size.txt')), string(name: 'spi_interrupt_data_size', value: readFile('spi_interrupt_data_size.txt')), string(name: 'spi_interrupt_bss_size', value: readFile('spi_interrupt_bss_size.txt')), string(name: 'spi_polling_text_size', value: readFile('spi_polling_text_size.txt')), string(name: 'spi_polling_data_size', value: readFile('spi_polling_data_size.txt')), string(name: 'spi_polling_bss_size', value: readFile('spi_polling_bss_size.txt')), string(name: 'spi_dma_text_size', value: readFile('spi_dma_text_size.txt')), string(name: 'spi_dma_data_size', value: readFile('spi_dma_data_size.txt')), string(name: 'spi_dma_bss_size', value: readFile('spi_dma_bss_size.txt'))]
                 }
            }    
        }
//        stage('Checkout tester') {
//	    agent { 
//                label 'FX160_HardwareTester'
//            }
//            steps {
//                sh 'git submodule update --init'
//                unstash 'makefiles'		                
//            }
//	}
//        stage('Build on tester') {
//	    agent { 
//                label 'FX160_HardwareTester'
//            }
//            steps {
//                parallel(
//                    diagnostic :        { eclipseBuild('diagnostic', targets) },
//                    externalInterrupt : { eclipseBuild('externalInterrupt', targets) },
//                    gpio :              { eclipseBuild('gpio', targets) },
//                    os :                { eclipseBuild('os', targets) },
//                    serialPort :        { eclipseBuild('serialPort', targets) },
//                    signalSlot :        { eclipseBuild('signal slot', targets) },
//                    ticToc :            { eclipseBuild('ticToc', targets) },
//                )
//            }
//        }
	stage('Analyze microhal examples') {
	    when {
                expression { env.BRANCH_NAME == 'devel'}
            }
	    steps {		
                parallel(
                    diagnostic :        { sa('diagnostic', targets) },
                    externalInterrupt : { sa('externalInterrupt', targets) },
                    gpio :              { sa('gpio', targets) },
                    os :                { sa('os', targets) },
                    serialPort :        { sa('serialPort', targets) },
                    signalSlot :        { sa('signal slot', targets) },
                    ticToc :            { sa('ticToc', targets) },
                )
            }	    
        }
        stage('Create documentation') {
            when {
                expression { env.BRANCH_NAME == 'master'}
            }
            steps {
                sh 'doxygen microhal/core/HAL_DOC.doxyfile'
            }
            post {
                success {
                    echo "Documentation is published."
                    // todo(pokas) publish documentation
                }
            }
        }	
    }
    post {
        always {
            deleteDir()
        }
    }
}

