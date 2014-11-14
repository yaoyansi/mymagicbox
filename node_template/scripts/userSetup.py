"""
1. I set PYTHONPATH :+= scripts in mod file, so this userSetup.py is loaded automatically
"""
import os
import sys
import maya.cmds as cmds

modulePath = cmds.getModulePath(moduleName='node_template')
modulePath = modulePath.replace('\\','/')

# get fullpath of current py file
this_file_path = modulePath + '/scripts/userSetup.py';
print("this is: "+this_file_path);

# add common module to python path
commonPath = modulePath+'/../common';
sys.path.insert(0, commonPath);

"""
2. There are so many ``AETemplates`` directories, so I have to load the modules with the full path.
"""
import imp
imp.load_source('AETemplates.AEnode_templateTemplate', modulePath+'/scripts/AETemplates/AEnode_templateTemplate.py')
