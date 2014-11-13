
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
As a convenience, when pymel is imported it will automatically import the module ``AETemplates``, if it exists,
thereby causing any AETemplates within it or its sub-modules to be registered. Be sure to import pymel
or modules containing your ``AETemplate`` classes before opening the Atrribute Editor for the node types in question.
"""
import pymel.core
