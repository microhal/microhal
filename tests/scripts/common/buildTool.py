#!/usr/bin/env python2

import os

def createDirectoryIfNeeded(directory):
    import os
    if not os.path.exists(directory):
        os.makedirs(directory)

def build(project, target, defines):
    workspace = 'workspace_' + project.replace('../', "").replace("\\s","_")
    workspace = workspace.replace(" ","_")

    logDirectory = "logs"
    createDirectoryIfNeeded(logDirectory)
    logFileName = workspace.replace('/','-') + "_" + target.replace('/','-').replace(" ","_")
    logFile = logDirectory + '/' + logFileName + '.log'
    errorFile = logDirectory + '/' + logFileName + '.err'

    defs = ''
    for define in defines:
            defs = defs + ' -D ' + define
    parameters = '--launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -no-indexer' + defs + ' -data ' + workspace + ' -import "' + project + '" -cleanBuild "' + target + '"'
    #print parameters
	#subprocess.check_output("eclipse " + parameters, shell=True)
    return os.system("eclipse " + parameters + " >> " + logFile + " 2> " + errorFile) == 0
