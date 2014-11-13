//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

// Example Plugin: lambertShader.cpp
//
// Produces dependency graph node LambertShader
// This node is an example of a Lambert shader and how to build a
// dependency node as a surface shader in Maya. The inputs for this node
// are many, and can be found in the Maya UI on the Attribute Editor for
// the node. The output attributes for the node are "outColor" and
// "outTransparency". To use this shader, create a lambertShader with
// Shading Group or connect the outputs to a Shading Group's
// "SurfaceShader" attribute.
//

#include "version_control_node.h"
#include <common/node_ids.h>

/////////////////////////////////
// Plugin Lambert Shader Class //
/////////////////////////////////


// This class will create a new shader. Shaders are custom dependency
// graph objects so we will derive this class from the basic DG node
// type MPxNode
//


// IFF type ID
// Each node requires a unique identifier which is used by
// MFnDependencyNode::create() to identify which node to create, and by
// the Maya file format.
//
// For local testing of nodes you can use any identifier between
// 0x00000000 and 0x0007ffff, but for any node that you plan to use for
// more permanent purposes, you should get a universally unique id from
// Autodesk Support. You will be assigned a unique range that you
// can manage on your own.
//
MTypeId node_template::m_id( NodeID_version_control );
MString node_template::m_classification("shader/surface");
VersionHelper node_template::m_version_helper;
///////////////////////////////////////////////////////
// DESCRIPTION: attribute information
///////////////////////////////////////////////////////
//
MObject  node_template::aColor;
MObject  node_template::aOutColor;


// the postConstructor() function is called immediately after the objects
// constructor. It is not safe to call MPxNode member functions from the
// constructor, instead they should be called here.
//
void node_template::postConstructor( )
{
	// setMPSafe indicates that this shader can be used for multiprocessor
	// rendering. For a shading node to be MP safe, it cannot access any
	// shared global data and should only use attributes in the datablock
	// to get input data and store output data.
	//
	setMPSafe( true );

	m_version_helper.postConstructor(thisMObject());
}

MString node_template::cTypeName()
{
	return "version_control";
}

MTypeId node_template::cTypeId()
{
	return node_template::m_id;
}

MPxNode::Type node_template::cType()
{
	return MPxNode::kDependNode;
}

const MString& node_template::classification()
{
	return m_classification;
}


// This node does not need to perform any special actions on creation or
// destruction
//

node_template::node_template() { }
node_template::~node_template() { }


// The creator() method allows Maya to instantiate instances of this node.
// It is called every time a new instance of the node is requested by
// either the createNode command or the MFnDependencyNode::create()
// method.
//
// In this case creator simply returns a new node_template object.
//

void* node_template::creator()
{
	return new node_template();
}


// The initialize method is called only once when the node is first
// registered with Maya. In this method you define the attributes of the
// node, what data comes in and goes out of the node that other nodes may
// want to connect to.
//

MStatus node_template::initialize()
{
	MFnNumericAttribute nAttr;
	MFnLightDataAttribute lAttr;

	MStatus status; // Status will be used to hold the MStatus value

	// Input Attributes
	//
	aColor = nAttr.createColor( "color", "c", &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f, 0.58824f, 0.644f ) );
	CHECK_MSTATUS( nAttr.setUsedAsColor( true ) );

	// Output Attributes
	//
	aOutColor = nAttr.createColor( "outColor", "oc", &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setHidden( false ) );
	CHECK_MSTATUS( nAttr.setReadable( true ) );
	CHECK_MSTATUS( nAttr.setWritable( false ) );


	CHECK_MSTATUS( addAttribute( aColor ) );
	CHECK_MSTATUS( addAttribute( aOutColor ) );

	CHECK_MSTATUS( attributeAffects( aColor, aOutColor ) );


	m_version_helper.initialize();

	return( MS::kSuccess );
}


// The compute() method does the actual work of the node using the inputs
// of the node to generate its output.
//
// Compute takes two parameters: plug and data.
// - Plug is the the data value that needs to be recomputed
// - Data provides handles to all of the nodes attributes, only these
//   handles should be used when performing computations.
//
MStatus node_template::compute( const MPlug& plug, MDataBlock& block )
{
	// The plug parameter will allow us to determine which output attribute
	// needs to be calculated.
	//
	if( plug == aOutColor)
	{
		MStatus status;
		MFloatVector resultColor( 0.0, 0.0, 0.0 );


		// Get surface shading parameters from input block
		//
		MFloatVector& surfaceColor = block.inputValue( aColor, &status ).asFloatVector();
		CHECK_MSTATUS( status );

		// Factor incident light with surface color and add incandescence
		//
		resultColor = surfaceColor;

		// Set ouput color attribute
		//
		if ( plug == aOutColor )
		{
			// Get the handle to the attribute
			//
			MDataHandle outColorHandle = block.outputValue( aOutColor, &status );
			CHECK_MSTATUS( status );
			MFloatVector& outColor = outColorHandle.asFloatVector();

			outColor = resultColor;		// Set the output value
			outColorHandle.setClean(); // Mark the output value as clean
		}
	}
	else
	{
		return( MS::kUnknownParameter ); // We got an unexpected plug
	}

	return( MS::kSuccess );
}


