"""
1. I set PYTHONPATH :+= scripts in mod file, so this userSetup.py is loaded automatically
"""
import os
import sys
import maya.cmds as cmds

modulePath = cmds.getModulePath(moduleName='tornado_field')

# get fullpath of current py file
this_file_path = modulePath + '/scripts/userSetup.py';
this_file_path = this_file_path.replace('\\','/')
print("this is: "+this_file_path);

# add common module to python path
commonPath = modulePath+'/../common';
sys.path.insert(0, commonPath);

"""
2. When pymel is imported it will automatically import the module ``AETemplates``, if it exists.
So I have to import pymel manully to regist my AETemplates which are located at ./AETemplates
"""
import pymel.core
