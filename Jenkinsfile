#!/usr/bin/env groovy

def eclipseBuild(projName, targets) {
    def projDirMap = [
        'diagnostic' : 'examples/diagnostic',
        'externalInterrupt' : 'examples/externalInterrupt',
        'gpio' : 'examples/gpio',
        'os' : 'examples/os',
        'serialPort' : 'examples/serialPort',
        'signal slot' : 'examples/signalSlot',
        'ticToc' : 'examples/ticToc',
        'cli' : 'components/cli/examples',
        'hostComm' : 'components/hostComm/examples',
        'at45db' : 'drivers/Atmel/AT45DB041D/example',        
        'bmp180' : 'drivers/Bosch/BMP180/example',
        'ds2782' : 'drivers/MaximDallas/DS2782/example',
        'ds2786' : 'drivers/MaximDallas/DS2786/example',        
        'hx711' : 'drivers/AVIA Semiconductor/HX711/example',
        'isl29023' : 'drivers/Intersil/ISL29023/example',        
        'leptonPCSerialReceiver' : 'drivers/FLIR/Lepton/example/lepton_PC_Serial_Receiver',
        'leptonSDCardWrite' : 'drivers/FLIR/Lepton/example/lepton_SDCard_write',
        'leptonSerialTransmitter' : 'drivers/FLIR/Lepton/example/lepton_Serial_Transmitter',
        'lis302' : 'drivers/STMicroelectronics/LIS302/example',
        'lsm330dl' : 'drivers/STMicroelectronics/LSM330DL/example/basic',
        'm24c16' : 'drivers/STMicroelectronics/M24C16/example',
        'mpl115a1' : 'drivers/Freescale Semiconductor/MPL115A1/example',
        'mpl115a2' : 'drivers/Freescale Semiconductor/MPL115A2/example',
        'mrf89xa' : 'drivers/Microchip/MRF89XA/example/mrf89xa',
        'pcf8563' : 'drivers/NXP/PCF8563/example',
        'rfm70' : 'drivers/Hoperf Electronic/RFM70/example/packet_send',
        'sht21' : 'drivers/Sensirion/SHT21/example/basic',
        'tmp006' : 'drivers/Texas Instruments/TMP006/example',
        'uCAM-II' : 'drivers/4D Systems/uCAM-II/example',
        'ws2812' : 'drivers/Worldsemi/WS2812/example',
        'serialPort_test' : 'tests/serialPort',
    ]
     
    echo "Building on ${env.NODE_NAME}"
    if (env.NODE_NAME == 'master') {
         lock('eclipseBuild_master') {
             withEnv(['PATH+WHATEVER=/home/microide/microide/toolchains/arm-none-eabi-gcc/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:/home/microide/microide/eclipse']) {
                 for (target in targets) {
                     retry(2) {
                        timeout(time:10, unit:'MINUTES') {
                            sh 'eclipse -configuration /srv/jenkins --launcher.suppressErrors -nosplash -data workspace_' + projName.replaceAll("\\s","_") + ' -importAll "' + projDirMap[projName] + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + projName + '/' + target + '"'
                        }
                    }
                }
            }
        }
    }
    if (env.NODE_NAME == 'FX160_HardwareTester') {
        lock('eclipseBuild_FX160') {
            withEnv(['PATH+WHATEVER=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:/var/jenkins/tools/microide/eclipse']) {
                for (target in targets) {
                    retry(2) {
                        timeout(time:10, unit:'MINUTES') {
                            sh 'eclipse --launcher.suppressErrors -nosplash -no-indexer -data workspace_' + projName.replaceAll("\\s","_") + ' -importAll "' + projDirMap[projName] + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + projName + '/' + target + '"'
                        }
                    }
                }
            }
        }
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
        'cli' : 'components/cli/examples',
        'hostComm' : 'components/hostComm/examples',
        'at45db' : 'drivers/Atmel/AT45DB041D/example',        
        'bmp180' : 'drivers/Bosch/BMP180/example',
        'ds2782' : 'drivers/MaximDallas/DS2782/example',
        'ds2786' : 'drivers/MaximDallas/DS2786/example',        
        'hx711' : 'drivers/AVIA Semiconductor/HX711/example',
        'isl29023' : 'drivers/Intersil/ISL29023/example',        
        'leptonPCSerialReceiver' : 'drivers/FLIR/Lepton/example/lepton_PC_Serial_Receiver',
        'leptonSDCardWrite' : 'drivers/FLIR/Lepton/example/lepton_SDCard_write',
        'leptonSerialTransmitter' : 'drivers/FLIR/Lepton/example/lepton_Serial_Transmitter',
        'lis302' : 'drivers/STMicroelectronics/LIS302/example',
        'lsm330dl' : 'drivers/STMicroelectronics/LSM330DL/example/basic',
        'm24c16' : 'drivers/STMicroelectronics/M24C16/example',
        'mpl115a1' : 'drivers/Freescale Semiconductor/MPL115A1/example',
        'mpl115a2' : 'drivers/Freescale Semiconductor/MPL115A2/example',
        'mrf89xa' : 'drivers/Microchip/MRF89XA/example/mrf89xa',
        'pcf8563' : 'drivers/NXP/PCF8563/example',
        'rfm70' : 'drivers/Hoperf Electronic/RFM70/example/packet_send',
        'sht21' : 'drivers/Sensirion/SHT21/example/basic',
        'tmp006' : 'drivers/Texas Instruments/TMP006/example',
        'uCAM-II' : 'drivers/4D Systems/uCAM-II/example',
        'ws2812' : 'drivers/Worldsemi/WS2812/example',
        'serialPort_test' : 'tests/serialPort',
    ]
     
    lock('eclipseBuild_FX160') {
        withEnv(['PATH+WHATEVER=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:/var/jenkins/tools/microide/eclipse']) {
            for (target in targets) {               
                timeout(time:10, unit:'MINUTES') {
                    sh '''#!/bin/bash
                    source /var/jenkins/tools/codechecker/venv/bin/activate 
                    export PATH=/var/jenkins/tools/codechecker/build/CodeChecker/bin:$PATH 
                    export PATH=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:$PATH
                    CodeChecker check -e alpha -e llvm -n ''' + projName.replaceAll("\\s","_") + '_' + target + ' -b "cd ' + projDirMap[projName] + '/' + target +' && make clean && make all"'                    
                }        
            }
        }
    }
}

def targets = ['stm32f4-discovery', 'NUCLEO-F334R8']
pipeline {   
    agent {
        node {
            label 'FX160_HardwareTester'
        }    
    }   

    stages {
        stage('Prepare') {
            steps {  
               //node('master') {    
                   checkout scm
                   sh 'git submodule update --init'
               //}
            }
        }
        stage('Build microhal examples') {
            steps {
                parallel(
                    diagnostic :        { eclipseBuild('diagnostic', targets) },
                    externalInterrupt : { eclipseBuild('externalInterrupt', targets) },
                    gpio :              { eclipseBuild('gpio', targets) },
                    os :                { eclipseBuild('os', targets) },
                    serialPort :        { eclipseBuild('serialPort', targets) },
                    signalSlot :        { eclipseBuild('signal slot', targets) },
                   // ticToc :            { eclipseBuild('ticToc', targets) },
                )
            }
        }
        stage('Analyze microhal examples') {
            steps {
                parallel(
                    diagnostic :        { sa('diagnostic', targets) },
                    externalInterrupt : { sa('externalInterrupt', targets) },
                    gpio :              { sa('gpio', targets) },
                    os :                { sa('os', targets) },
                    serialPort :        { sa('serialPort', targets) },
                    signalSlot :        { sa('signal slot', targets) },
                   // ticToc :            { sa('ticToc', targets) },
                )
            }
        }
    } 
    post {
        always {
            deleteDir()
        }
    }
}
