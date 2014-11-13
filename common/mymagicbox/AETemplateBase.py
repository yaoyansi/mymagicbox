"""
To create an Attribute Editor template using python, do the following:
 	1. create a subclass of `uitypes.AETemplate`
	2. set its ``_nodeType`` class attribute to the name of the desired node type, or name the class using the
convention ``AE<nodeType>Template``
	3. import the module

AETemplates which do not meet one of the two requirements listed in step 2 will be ignored.  To ensure that your
Template's node type is being detected correctly, use the ``AETemplate.nodeType()`` class method::

    import AETemplates
    AETemplates.AEmib_amb_occlusionTemplate.nodeType()

As a convenience, when pymel is imported it will automatically import the module ``AETemplates``, if it exists,
thereby causing any AETemplates within it or its sub-modules to be registered. Be sure to import pymel
or modules containing your ``AETemplate`` classes before opening the Atrribute Editor for the node types in question.

To check which python templates are loaded::

	from pymel.core.uitypes import AELoader
	print AELoader.loadedTemplates()

The example below demonstrates the simplest case, which is the first. It provides a layout for the mib_amb_occlusion
mental ray shader.
"""

from pymel.core import *

class LocalizedTemplate(ui.AETemplate):
    "automatically apply language localizations to template arguments"
    def _applyLocalization(self, name):
        if name is not None and len(name)>2 and name[0] == 'k' and name[1].isupper():
            return mel.uiRes('m_' + self.__class__.__name__ + '.' + name)
        return name

    def addControl(self, control, label=None, **kwargs):
        label = self._applyLocalization(label)
        ui.AETemplate.addControl(self, control, label=label, **kwargs)

    def beginLayout(self, name, collapse=True):
        name =  self._applyLocalization(name)
        ui.AETemplate.beginLayout(self, name, collapse=collapse)

class mmbTemplateBase(LocalizedTemplate):
    def __init__(self, nodeName):
        LocalizedTemplate.__init__(self,nodeName)
        #mel.AEswatchDisplay(nodeName)
        self.beginScrollLayout()
        self.buildBody(nodeName)
        self.endScrollLayout()


