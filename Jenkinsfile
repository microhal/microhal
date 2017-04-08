pipeline {
    agent { node { label 'master' } }

    stages {
        stage('Build') {
            steps {
                sh /home/microide/microide_build.sh --launcher.suppressErrors -nosplash -data workspace -importAll "drivers/AVIA Semiconductor/HX711/example" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "hx711/stm32f4-discovery"
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
