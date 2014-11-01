#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "testProjectionNode.h"

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
MStatus initializePlugin( MObject obj )
{
    printf("\nplugin is loaded -----------------------\n");

	MFnPlugin plugin( obj, "yaoyu", "1.0.0", "Any" );

	CHECK_MSTATUS(
        plugin.registerShape( "quadricShape", quadricShape::id,
                                           &quadricShape::creator,
                                           &quadricShape::initialize,
                                           &quadricShapeUI::creator  )
    );

	return( MS::kSuccess );
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( quadricShape::id ) );

	return MS::kSuccess;
}
