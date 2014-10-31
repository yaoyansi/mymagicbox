#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "testuv.h"
#include "peltOverlapCmd.h"

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
MStatus initializePlugin( MObject obj )
{
    printf("\nplugin %s is loaded -----------------------\n", TestUVCmd::cCmdName().asChar() );

	MFnPlugin plugin( obj, "yaoyu", "1.0.0", "Any" );

	CHECK_MSTATUS(
		plugin.registerCommand(
            TestUVCmd::cCmdName(),
            TestUVCmd::creator
		) );

    CHECK_MSTATUS(plugin.registerCommand( "peltOverlap",
                 peltOverlap::creator,
                 peltOverlap::newSyntax));

	return( MS::kSuccess );
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

    CHECK_MSTATUS(plugin.deregisterCommand( "peltOverlap" ));

	CHECK_MSTATUS( plugin.deregisterCommand( TestUVCmd::cCmdName() ) );

	return MS::kSuccess;
}
