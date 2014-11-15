#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "msg_cb_mgr.h"
#include "test_node_a.h"
#include "test_node_b.h"
#include "version.h"


static MessageCallbackMgr g_msg_cb_mgr;

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
PLUGIN_EXPORT MStatus initializePlugin( MObject obj )
{

	MFnPlugin plugin( obj, PLUGIN_COMPANY, PLUGIN_VERSION_STRING, "Any" );

	CHECK_MSTATUS(
		plugin.registerNode(
		TestNodeA::cTypeName(),
		TestNodeA::cTypeId(),
		TestNodeA::creator,
		TestNodeA::initialize,
		TestNodeA::cType(),
		&TestNodeA::classification()
		) );

	CHECK_MSTATUS(
		plugin.registerNode(
		TestNodeB::cTypeName(),
		TestNodeB::cTypeId(),
		TestNodeB::creator,
		TestNodeB::initialize,
		TestNodeB::cType(),
		&TestNodeB::classification()
		) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += TestNodeA::classification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	CHECK_MSTATUS(g_msg_cb_mgr.regist());

	return( MS::kSuccess );
}

PLUGIN_EXPORT MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS(g_msg_cb_mgr.deregist());

	CHECK_MSTATUS( plugin.deregisterNode( TestNodeB::cTypeId() ) );
	CHECK_MSTATUS( plugin.deregisterNode( TestNodeA::cTypeId() ) );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	command += TestNodeA::classification();
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
