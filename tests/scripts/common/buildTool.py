#!/usr/bin/env python2

import os

def build(project, target, defines):
	workspace = 'workspace_' + project.replace("\\s","_")    
	defs = ''
	for define in defines:
        	defs = defs + ' -D ' + define
	parameters = '--launcher.suppressErrors -nosplash -no-indexer' + defs + ' -data ' + workspace + ' -import "' + project + '" -application org.eclipse.cdt.managedbuilder.core.headlessbuild -cleanBuild "' + target + '"'
	#print parameters
	#subprocess.check_output("eclipse " + parameters, shell=True)
	return os.system("eclipse " + parameters)
