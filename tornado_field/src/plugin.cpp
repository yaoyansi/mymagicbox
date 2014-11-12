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
            tornadoField::cTypeName(),
            tornadoField::cTypeId(),
            tornadoField::creator,
            tornadoField::initialize,
            tornadoField::cType(),
            &tornadoField::cClassification()
		) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += tornadoField::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return( MS::kSuccess );
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( tornadoField::cTypeId() ) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += tornadoField::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
