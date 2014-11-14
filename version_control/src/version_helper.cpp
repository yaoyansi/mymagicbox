#include "version_helper.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>
#include "version.h"

MObject  VersionHelper::aVersion;
//
VersionHelper::VersionHelper()
:m_long_name("mmbversion"), m_short_name("mmbv")
{

}
//
VersionHelper::~VersionHelper()
{

}
//
MStatus	VersionHelper::initialize()
{
    MStatus status;

	MFnNumericAttribute nAttr;

	aVersion = nAttr.create( m_long_name, m_short_name, MFnNumericData::kInt, 0, &status );
	CHECK_MSTATUS( status);
	/*
	By default, attributes are:
        Readable.
        Writable.
        Connectable.
        Storable.
        Cached.
        Not arrays.
        Have indices that matter.
        Do not use an array builder.
        Not keyable.
        Not hidden.
        Not used as colors.
        Not indeterminant.
        Set to disconnect behavior kNothing.
	*/
	CHECK_MSTATUS(MPxNode::addAttribute(aVersion));
}
//
void VersionHelper::postConstructor(const MObject& mobject)
{
    MStatus status;
    MFnDependencyNode fnDepNode(mobject, &status);
    CHECK_MSTATUS(status);

    MPlug versionPlug(fnDepNode.findPlug(m_long_name, false, &status));
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(versionPlug.setValue(PLUGIN_VERSION_CODE));
}
