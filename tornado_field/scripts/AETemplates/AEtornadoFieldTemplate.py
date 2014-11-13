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
import pymel.core			as pm
import mymagicbox.AETemplateBase	as AETemplateBase 
import mymagicbox.log			as log 

class AEtornadoFieldTemplate(AETemplateBase.mmbTemplateBase):
	def buildBody(self, nodeName):
		log.debug("building AETemplate for node: %s", nodeName)

		self.beginLayout("Transform Attributes",collapse=0)
		self.addControl("translate")
		self.addControl("rotate")
		self.addControl("scale")
		self.addControl("shear")
		self.addControl("rotateOrder")
		self.addControl("rotateAxis")
		self.addControl("inheritsTransform")
		self.endLayout()

		self.beginLayout("Torus Field Attributes", collapse=0);
		self.addControl("minDistance")
		self.addControl("attractDistance")
		self.addControl("repelDistance")
		self.addControl("drag")
		self.addControl("swarmAmplitude")
		self.addControl("swarmFrequency")
		self.addControl("swarmPhase")
		self.endLayout()

		pm.mel.AEdependNodeTemplate(self.nodeName)

		self.addExtraControls()
