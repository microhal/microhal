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
         for (target in targets) {
            sh 'eclipse --launcher.suppressErrors -nosplash -data workspace_' + projName.replaceAll("\\s","_") + ' -importAll "' + projDirMap[projName] + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + projName + '/' + target + '"'
         }
     }
}

def buildAllDevExamples() {
    def projects = ['bmp180', 'ds2782', 'ds2786', 'hx711', 'isl29023', 'lepton', 'lis302', 'lsm330dl', 'm24c16', 'mpl115a1',
                    'mpl115a2', 'mrf89xa', 'pcf8563', 'rfm70', 'sht21', 'tmp006']
    def targets = ['stm32f4-discovery', 'NUCLEO-F411RE', 'NUCLEO-F334R8']
    for (project in projects) {        
        eclipseBuild(project, targets)        
    }
}
def targets = ['stm32f4-discovery', 'NUCLEO-F411RE', 'NUCLEO-F334R8']
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
                parallel(
                    diagnostic :        { eclipseBuild('diagnostic', targets) },                                  
                    externalInterrupt : { eclipseBuild('externalInterrupt', targets) },
                    //gpio :              { eclipseBuild('gpio', targets) },
                    //os :                { eclipseBuild('os', targets) },                
                   // serialPort :        { eclipseBuild('serialPort', targets) },              
                   // signalSlot :        { eclipseBuild('signal slot', targets) },             
                   // ticToc :            { eclipseBuild('ticToc', targets) },                 
                )
            }
        }
        stage('Build components examples') {
            steps {  
                parallel(
                    cli : { eclipseBuild('cli', ['stm32f4-discovery']) },
                    hostComm : { eclipseBuild('hostComm', ['stm32f4-discovery']) }
                )
            }
        }
        stage('Build devices examples') {
            steps {
                parallel(
                    bmp180 : {  eclipseBuild('bmp180', targets) },
                    ds2782 : {  eclipseBuild('ds2782', targets) },
                  //  ds2786 : {  eclipseBuild('ds2786', targets) },
                  //  hx711 : {  eclipseBuild('hx711', targets) },
                  //  isl29023 : {  eclipseBuild('isl29023', targets) },
                  //  lis302 : {  eclipseBuild('lis302', targets) },
                 //   lsm330dl : {  eclipseBuild('lsm330dl', targets) },
                   // m24c16 : {  eclipseBuild('m24c16', targets) },
                 //   mpl115a1 : {  eclipseBuild('mpl115a1', targets) },
                  //  mrf89xa : {  eclipseBuild('mrf89xa', targets) },
                  //  pcf8563 : {  eclipseBuild('pcf8563', targets) },
                 //   rfm70 : {  eclipseBuild('rfm70', targets) },
                 //   sht21 : {  eclipseBuild('sht21', targets) },
                 //   tmp006 : {  eclipseBuild('tmp006', targets) },
                )
               //buildAllDevExamples()
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
