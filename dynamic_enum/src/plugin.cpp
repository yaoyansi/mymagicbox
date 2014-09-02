#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "dynamic_enum.h"

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
MStatus initializePlugin( MObject obj )
{

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any" );

	CHECK_MSTATUS(
		plugin.registerNode(
            DynamicEnum::cTypeName(),
            DynamicEnum::cTypeId(),
            DynamicEnum::creator,
            DynamicEnum::initialize,
            DynamicEnum::cType(),
            &DynamicEnum::cClassification()
		) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += DynamicEnum::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return( MS::kSuccess );
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( DynamicEnum::cTypeId() ) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += DynamicEnum::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
