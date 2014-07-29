#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "dynamic_enum.h"

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
PLUGIN_EXPORT MStatus initializePlugin( MObject obj )
{

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any" );

	CHECK_MSTATUS( 
		plugin.registerNode( 
		DynamicEnum::typeName(), 
		DynamicEnum::typeId(), 
		DynamicEnum::creator, 
		DynamicEnum::initialize, 
		DynamicEnum::type(),
		&DynamicEnum::classification() 
		) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += DynamicEnum::classification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return( MS::kSuccess );
}

PLUGIN_EXPORT MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( DynamicEnum::typeId() ) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += DynamicEnum::classification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
