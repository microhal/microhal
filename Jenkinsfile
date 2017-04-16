def eclipseBuild(projName, target) {
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
        'lepton' : 'drivers/FLIR/Lepton/example',
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
    ]
    
    //sh '/home/microide/microide_build.sh --launcher.suppressErrors -nosplash -data workspace -importAll "' + projDirMap[projName] + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + projName + '/' + target + '"'
     withEnv(['PATH+WHATEVER=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:/var/jenkins/tools/microide/eclipse']) {
         sh 'eclipse --launcher.suppressErrors -nosplash -data "workspace/' + projName + '" -importAll "' + projDirMap[projName] + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + projName + '/' + target + '"'
     }
}

def buildAllDevExamples() {
    def projects = ['bmp180', 'ds2782', 'ds2786', 'hx711', 'isl29023', 'lepton', 'lis302', 'lsm330dl', 'm24c16', 'mpl115a1',
                    'mpl115a2', 'mrf89xa', 'pcf8563', 'rfm70', 'sht21', 'tmp006']
    def targets = ['stm32f4-discovery', 'NUCLEO-F411RE', 'NUCLEO-F334R8']
    for (project in projects) {
        for (target in targets) {
            eclipseBuild(project, target)
        }
    }
}

pipeline {   
    agent {
        node {
            label 'FX160_HardwareTester'           
        }    
    }   

    stages {
        stage('Prepare') {
            steps { 
                checkout scm
                sh 'git submodule update --init'
            }
        }
        stage('Build microhal examples') {
            steps {
              //  parallel(
                  //  diagnostic : {
                        eclipseBuild('diagnostic', 'stm32f4-discovery')
                                    eclipseBuild('diagnostic', 'NUCLEO-F411RE')
                                    eclipseBuild('diagnostic', 'NUCLEO-F334R8')// },
                        
                   // externalInterrupt : {
                        eclipseBuild('externalInterrupt', 'stm32f4-discovery')
                                            eclipseBuild('externalInterrupt', 'NUCLEO-F411RE')
                                            eclipseBuild('externalInterrupt', 'NUCLEO-F334R8')// },
                
                   // gpio : { eclipseBuild('gpio', 'stm32f4-discovery')
                   //         eclipseBuild('gpio', 'NUCLEO-F411RE')
                   //         eclipseBuild('gpio', 'NUCLEO-F334R8')// },
                
                   // os : { 
                   //     eclipseBuild('os', 'stm32f4-discovery')
                   //         eclipseBuild('os', 'NUCLEO-F411RE')
                    //        eclipseBuild('os', 'NUCLEO-F334R8')// },
                
                   // serialPort : {
                   //     eclipseBuild('serialPort', 'stm32f4-discovery')
                   //                 eclipseBuild('serialPort', 'NUCLEO-F411RE')
                   //                 eclipseBuild('serialPort', 'NUCLEO-F334R8')// },
                
                    //signalSlot : {
                   //     eclipseBuild('signal slot', 'stm32f4-discovery') 
                    //                eclipseBuild('signal slot', 'NUCLEO-F411RE')
                    //                eclipseBuild('signal slot', 'NUCLEO-F334R8')// },
                
                   // ticToc : {  
                    //    eclipseBuild('ticToc', 'stm32f4-discovery')
                     //           eclipseBuild('ticToc', 'NUCLEO-F411RE')
                     //           eclipseBuild('ticToc', 'NUCLEO-F334R8')// }
              //  )
            }
        }
        stage('Build components examples') {
            steps {  
                parallel( cli : { eclipseBuild('cli', 'stm32f4-discovery') },
                          hostComm : { eclipseBuild('hostComm', 'stm32f4-discovery') }
                         )
            }
        }
        stage('Build devices examples') {
            steps {                
               buildAllDevExamples()
            }
        }
        stage('Test') {
            steps {
                    echo 'Testing..'
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying....'
            }       
        }
    }
}
