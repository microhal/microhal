pipeline {
    agent { node { label 'master' } }

    stages {
        stage('Build') {
            steps {
                def labels = ['stm32f4-discovery', 'NUCLEO-F411RE', 'NUCLEO-F334R8']
                for (target in labels) {
                    sh '/home/microide/microide_build.sh --launcher.suppressErrors -nosplash -data workspace -importAll "drivers/AVIA Semiconductor/HX711/example" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "hx711/${target}"'
                }
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


pipeline {
    agent { node { label 'master' } }

    stages {
        stage('Build') {
            steps {
                sh '/home/microide/microide_build.sh --launcher.suppressErrors -nosplash -data workspace -importAll examples/diagnostic -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "diagnostic/NUCLEO-F411RE"'
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
