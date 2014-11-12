#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>
#include <maya/MGlobal.h>

#include <common/copyright.h>
#include "tornado_field.h"


// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
MStatus initializePlugin( MObject obj )
{

	MFnPlugin plugin( obj, PLUGIN_AUTHOR, "0.1", "Any" );

	CHECK_MSTATUS(
		plugin.registerNode(
            torusField::cTypeName(),
            torusField::cTypeId(),
            torusField::creator,
            torusField::initialize,
            torusField::cType(),
            &torusField::cClassification()
		) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += torusField::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return( MS::kSuccess );
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( torusField::cTypeId() ) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += torusField::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
