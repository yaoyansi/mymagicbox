#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "testDeformer.h"

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
MStatus initializePlugin( MObject obj )
{

	MFnPlugin plugin( obj, "yaoyu", "1.0.1", "Any" );

	CHECK_MSTATUS(
		plugin.registerNode(
            TestDeformer::cTypeName(),
            TestDeformer::cTypeId(),
            TestDeformer::creator,
            TestDeformer::initialize,
            TestDeformer::cType(),
            &TestDeformer::cClassification()
		) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += TestDeformer::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return( MS::kSuccess );
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( TestDeformer::cTypeId() ) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += TestDeformer::cClassification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
