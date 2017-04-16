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
        'm24c16' : 'drivers/STMicroelectronics/M24C16/example',
        'mrf89xa' : 'drivers/Microchip/MRF89XA/example/mrf89xa',
        'pcf8563' : 'drivers/NXP/PCF8563/example',
        'rfm70' : 'drivers/Hoperf Electronic/RFM70/example/packet_send',
        
    ]
    
    //sh '/home/microide/microide_build.sh --launcher.suppressErrors -nosplash -data workspace -importAll "' + projDirMap[projName] + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + projName + '/' + target + '"'
     withEnv(['PATH+WHATEVER=/var/jenkins/tools/microide/toolchains/gcc-arm-none-eabi/microhal/gcc-arm-none-eabi-5_3-2016q1/bin:/var/jenkins/tools/microide/eclipse']) {
         sh 'eclipse --launcher.suppressErrors -nosplash -data workspace -importAll "' + projDirMap[projName] + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + projName + '/' + target + '"'
     }
}

def buildAllDevExamples() {
    def projects = [ 'bmp180', 'ds2782', 'ds2786', 'hx711', 'isl29023', 'lepton', 'lis302', 'm24c16', 'mrf89xa', 'pcf8563', 'rfm70']
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
               eclipseBuild('diagnostic', 'stm32f4-discovery')
               eclipseBuild('diagnostic', 'NUCLEO-F411RE')
               eclipseBuild('diagnostic', 'NUCLEO-F334R8')
                        
               eclipseBuild('externalInterrupt', 'stm32f4-discovery')
            }
        }
        stage('Build components examples') {
            steps {                
               eclipseBuild('cli', 'stm32f4-discovery')
                           
               eclipseBuild('hostComm', 'stm32f4-discovery')
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
