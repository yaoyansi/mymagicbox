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

#include "dynamic_enum.h"


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
MTypeId DynamicEnum::m_id( 0x81001 );
MString DynamicEnum::m_classification("shader/surface");

///////////////////////////////////////////////////////
// DESCRIPTION: attribute information
///////////////////////////////////////////////////////
//
MObject  DynamicEnum::aFilePath;
MObject  DynamicEnum::aGridName;
MObject  DynamicEnum::aOutColor;

// the postConstructor() function is called immediately after the objects
// constructor. It is not safe to call MPxNode member functions from the
// constructor, instead they should be called here.
//
void DynamicEnum::postConstructor( )
{
	// setMPSafe indicates that this shader can be used for multiprocessor
	// rendering. For a shading node to be MP safe, it cannot access any
	// shared global data and should only use attributes in the datablock
	// to get input data and store output data.
	//
	setMPSafe( true );
}

MString DynamicEnum::typeName()
{
	return "dynamic_enum";
}

MTypeId DynamicEnum::typeId()
{
	return DynamicEnum::m_id;
}

MPxNode::Type DynamicEnum::type()
{
	return MPxNode::kDependNode;
}

const MString& DynamicEnum::classification()
{
	return m_classification;
}


// This node does not need to perform any special actions on creation or
// destruction
//

DynamicEnum::DynamicEnum() { }
DynamicEnum::~DynamicEnum() { }


// The creator() method allows Maya to instantiate instances of this node.
// It is called every time a new instance of the node is requested by
// either the createNode command or the MFnDependencyNode::create()
// method. 
//
// In this case creator simply returns a new DynamicEnum object.
//

void* DynamicEnum::creator()
{
	return new DynamicEnum();
}


// The initialize method is called only once when the node is first
// registered with Maya. In this method you define the attributes of the
// node, what data comes in and goes out of the node that other nodes may
// want to connect to. 
//

MStatus DynamicEnum::initialize()
{
	MFnNumericAttribute nAttr; 
	MFnTypedAttribute tAttr;
	MFnStringData sData;
	MStatus status; // Status will be used to hold the MStatus value

	// returned by each api function call. It is important
	// to check the status returned by a call to aid in
	// debugging. Failed API calls can result in subtle 
	// errors that can be difficult to track down, you may
	// wish to use the CHECK_MSTATUS macro for any API
	// call where you do not need to provide your own
	// error handling.
	//

	// Attribute Initialization:
	aFilePath = tAttr.create( "filepath", "file", MFnData::kString, &status );
	CHECK_MSTATUS( status );	
	CHECK_MSTATUS( tAttr.setKeyable( true ) );
	CHECK_MSTATUS( tAttr.setStorable( true ) );
	CHECK_MSTATUS( tAttr.setDefault( sData.create("") ) );

	aGridName = tAttr.create( "grid", "grd", MFnData::kString, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( tAttr.setKeyable( true ) );
	CHECK_MSTATUS( tAttr.setStorable( true ) );
	CHECK_MSTATUS( tAttr.setDefault( sData.create("") ) );

	aOutColor = nAttr.createColor( "outColor", "oc", &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setHidden( false ) );
	CHECK_MSTATUS( nAttr.setReadable( true ) );
	CHECK_MSTATUS( nAttr.setWritable( false ) );

	// Next we will add the attributes we have defined to the node
	//
	CHECK_MSTATUS( addAttribute( aFilePath ) );
	CHECK_MSTATUS( addAttribute( aGridName ) );
	CHECK_MSTATUS( addAttribute( aOutColor ) );

	// The attributeAffects() method is used to indicate when the input
	// attribute affects the output attribute. This knowledge allows Maya
	// to optimize dependencies in the graph in more complex nodes where
	// there may be several inputs and outputs, but not all the inputs
	// affect all the outputs.
	//
	CHECK_MSTATUS( attributeAffects( aFilePath, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aGridName, aOutColor ) );

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
MStatus DynamicEnum::compute( const MPlug& plug, MDataBlock& block ) 
{ 
	// The plug parameter will allow us to determine which output attribute
	// needs to be calculated.
	//
	if( plug == aOutColor )
	{
		MStatus status;
		MFloatVector resultColor( 0.0, 0.0, 0.0 ); 

	} 
	else
	{
		return( MS::kUnknownParameter ); // We got an unexpected plug
	}

	return( MS::kSuccess );
}


