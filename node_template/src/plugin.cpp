#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "node_template.h"

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
PLUGIN_EXPORT MStatus initializePlugin( MObject obj )
{

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any" );

	CHECK_MSTATUS(
		plugin.registerNode(
		node_template::cTypeName(),
		node_template::cTypeId(),
		node_template::creator,
		node_template::initialize,
		node_template::cType(),
		&node_template::classification()
		) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += node_template::classification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return( MS::kSuccess );
}

PLUGIN_EXPORT MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( node_template::cTypeId() ) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += node_template::classification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
