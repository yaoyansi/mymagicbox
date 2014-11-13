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

///////////////////////////////////////////////////////
// DESCRIPTION: attribute information
///////////////////////////////////////////////////////
//
MObject  node_template::aTranslucenceCoeff;
MObject  node_template::aDiffuseReflectivity;
MObject  node_template::aInTransparency;
MObject  node_template::aInTransR;
MObject  node_template::aInTransG;
MObject  node_template::aInTransB;
MObject  node_template::aColor;
MObject  node_template::aColorR;
MObject  node_template::aColorG;
MObject  node_template::aColorB;
MObject  node_template::aIncandescence;
MObject  node_template::aIncandescenceR;
MObject  node_template::aIncandescenceG;
MObject  node_template::aIncandescenceB;
MObject  node_template::aOutColor;
MObject  node_template::aOutColorR;
MObject  node_template::aOutColorG;
MObject  node_template::aOutColorB;
MObject  node_template::aOutTransparency;
MObject  node_template::aOutTransR;
MObject  node_template::aOutTransG;
MObject  node_template::aOutTransB;
MObject  node_template::aNormalCamera;
MObject  node_template::aNormalCameraX;
MObject  node_template::aNormalCameraY;
MObject  node_template::aNormalCameraZ;
MObject  node_template::aLightData;
MObject  node_template::aLightDirection;
MObject  node_template::aLightDirectionX;
MObject  node_template::aLightDirectionY;
MObject  node_template::aLightDirectionZ;
MObject  node_template::aLightIntensity;
MObject  node_template::aLightIntensityR;
MObject  node_template::aLightIntensityG;
MObject  node_template::aLightIntensityB;
MObject  node_template::aLightAmbient;
MObject  node_template::aLightDiffuse;
MObject  node_template::aLightSpecular;
MObject  node_template::aLightShadowFraction;
MObject  node_template::aPreShadowIntensity;
MObject  node_template::aLightBlindData;


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
}

MString node_template::cTypeName()
{
	return "node_template";
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
	// returned by each api function call. It is important
	// to check the status returned by a call to aid in
	// debugging. Failed API calls can result in subtle
	// errors that can be difficult to track down, you may
	// wish to use the CHECK_MSTATUS macro for any API
	// call where you do not need to provide your own
	// error handling.
	//

	// Attribute Initialization:
	//
	// create      - The create function creates a new attribute for the
	//				 node, it takes a long name for the attribute, a short
	//				 name for the attribute, the type of the attribute,
	//				 and a status object to determine if the api call was
	//				 successful.
	//
	// setKeyable  - Sets whether this attribute should accept keyframe
	//				 data, Attributes are not keyable by default.
	//
	// setStorable - Sets whether this attribute should be storable. If an
	//				 attribute is storable, then it will be writen out
	//				 when the node is stored to a file. Attributes are
	//               storable by default.
	//
	// setDefault  - Sets the default value for this attribute.
	//
	// setUsedAsColor - Sets whether this attribute should be presented as
	//				 a color in the UI.
	//
	// setHidden   - Sets whether this attribute should be hidden from the
	//				 UI. This is useful if the attribute is being used for
	//				 blind data, or if it is being used as scratch space
	//				 for a geometry calculation (should also be marked
	//				 non-connectable in that case). Attributes are not
	//				 hidden by default.
	//
	// setReadable - Sets whether this attribute should be readable. If an
	//				 attribute is readable, then it can be used as the
	//				 source in a dependency graph connection. Attributes
	//				 are readable by default.
	//
	// setWritable - Sets whether this attribute should be readable. If an
	//				 attribute is writable, then it can be used as the
	//				 destination in a dependency graph connection. If an
	//			     attribute is not writable then setAttr commands will
	//				 fail to change the attribute. If both keyable and
	//				 writable for an attribute are set to true it will be
	//				 displayed in the channel box when the node is
	//				 selected. Attributes are writable by default.
	//
	// setArray    - Sets whether this attribute should have an array of
	//				 data. This should be set to true if the attribute
	//				 needs to accept multiple incoming connections.
	//				 Attributes are single elements by default.
	//


	// Input Attributes
	//
	aTranslucenceCoeff = nAttr.create( "translucenceCoeff", "tc",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f ) );

	aDiffuseReflectivity = nAttr.create( "diffuseReflectivity", "drfl",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.8f ) );

	aColorR = nAttr.create( "colorR", "cr",MFnNumericData::kFloat, 0,
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f ) );

	aColorG = nAttr.create( "colorG", "cg", MFnNumericData::kFloat, 0,
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.58824f ) );

	aColorB = nAttr.create( "colorB", "cb",MFnNumericData::kFloat, 0,
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.644f ) );

	aColor = nAttr.create( "color", "c", aColorR, aColorG, aColorB,
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f, 0.58824f, 0.644f ) );
	CHECK_MSTATUS( nAttr.setUsedAsColor( true ) );

	aIncandescenceR = nAttr.create( "incandescenceR", "ir",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f ) );

	aIncandescenceG = nAttr.create( "incandescenceG", "ig",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f ) );

	aIncandescenceB = nAttr.create( "incandescenceB", "ib",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status);
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f ) );

	aIncandescence = nAttr.create( "incandescence", "ic", aIncandescenceR,
		aIncandescenceG, aIncandescenceB, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f, 0.0f, 0.0f ) );
	CHECK_MSTATUS( nAttr.setUsedAsColor( true ) );

	aInTransR = nAttr.create( "transparencyR", "itr",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );

	aInTransG = nAttr.create( "transparencyG", "itg",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );

	aInTransB = nAttr.create( "transparencyB", "itb",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status);
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );

	aInTransparency = nAttr.create( "transparency", "it", aInTransR,
		aInTransG, aInTransB, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setKeyable( true ) );
	CHECK_MSTATUS( nAttr.setStorable( true ) );
	CHECK_MSTATUS( nAttr.setDefault( 0.0f, 0.0f, 0.0f ) );
	CHECK_MSTATUS( nAttr.setUsedAsColor( true ) );


	// Output Attributes
	//

	// Color Output
	//
	aOutColorR = nAttr.create( "outColorR", "ocr", MFnNumericData::kFloat,
		0, &status );
	CHECK_MSTATUS( status );

	aOutColorG = nAttr.create( "outColorG", "ocg", MFnNumericData::kFloat,
		0, &status );
	CHECK_MSTATUS( status );

	aOutColorB = nAttr.create( "outColorB", "ocb", MFnNumericData::kFloat,
		0, &status );
	CHECK_MSTATUS( status );

	aOutColor = nAttr.create( "outColor", "oc", aOutColorR, aOutColorG,
		aOutColorB, &status );
	CHECK_MSTATUS( status );

	CHECK_MSTATUS( nAttr.setHidden( false ) );
	CHECK_MSTATUS( nAttr.setReadable( true ) );
	CHECK_MSTATUS( nAttr.setWritable( false ) );


	// Transparency Output
	//
	aOutTransR = nAttr.create( "outTransparencyR", "otr",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );

	aOutTransG = nAttr.create( "outTransparencyG", "otg",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );

	aOutTransB = nAttr.create( "outTransparencyB", "otb",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );

	aOutTransparency = nAttr.create( "outTransparency", "ot",
		aOutTransR,aOutTransG,aOutTransB, &status );
	CHECK_MSTATUS( status );

	CHECK_MSTATUS( nAttr.setHidden( false ) );
	CHECK_MSTATUS( nAttr.setReadable( true ) );
	CHECK_MSTATUS( nAttr.setWritable( false ) );


	// Camera Normals
	//
	aNormalCameraX = nAttr.create( "normalCameraX", "nx",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setStorable( false ) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aNormalCameraY = nAttr.create( "normalCameraY", "ny",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setStorable( false ) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aNormalCameraZ = nAttr.create( "normalCameraZ", "nz",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setStorable( false ) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aNormalCamera = nAttr.create( "normalCamera","n", aNormalCameraX,
		aNormalCameraY, aNormalCameraZ, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setStorable( false ) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f, 1.0f, 1.0f ) );
	CHECK_MSTATUS( nAttr.setHidden( true ) );


	// Light Direction
	//
	aLightDirectionX = nAttr.create( "lightDirectionX", "ldx",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aLightDirectionY = nAttr.create( "lightDirectionY", "ldy",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aLightDirectionZ = nAttr.create( "lightDirectionZ", "ldz",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aLightDirection = nAttr.create( "lightDirection", "ld",
		aLightDirectionX, aLightDirectionY, aLightDirectionZ,
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f, 1.0f, 1.0f ) );


	// Light Intensity
	//
	aLightIntensityR = nAttr.create( "lightIntensityR", "lir",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aLightIntensityG = nAttr.create( "lightIntensityG", "lig",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aLightIntensityB = nAttr.create( "lightIntensityB", "lib",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aLightIntensity = nAttr.create( "lightIntensity", "li",
		aLightIntensityR, aLightIntensityG, aLightIntensityB,
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f, 1.0f, 1.0f ) );


	// Light
	//
	aLightAmbient = nAttr.create( "lightAmbient", "la",
		MFnNumericData::kBoolean, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( true ) );

	aLightDiffuse = nAttr.create( "lightDiffuse", "ldf",
		MFnNumericData::kBoolean, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( true ) );

	aLightSpecular = nAttr.create( "lightSpecular", "ls",
		MFnNumericData::kBoolean, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( false ) );

	aLightShadowFraction = nAttr.create( "lightShadowFraction", "lsf",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aPreShadowIntensity = nAttr.create( "preShadowIntensity", "psi",
		MFnNumericData::kFloat, 0, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );
	CHECK_MSTATUS( nAttr.setDefault( 1.0f ) );

	aLightBlindData = nAttr.createAddr( "lightBlindData", "lbld",
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS ( nAttr.setStorable(false) );
	CHECK_MSTATUS ( nAttr.setHidden(true) );
	CHECK_MSTATUS ( nAttr.setReadable(true) );
	CHECK_MSTATUS ( nAttr.setWritable(false) );

	aLightData = lAttr.create( "lightDataArray", "ltd", aLightDirection,
		aLightIntensity, aLightAmbient, aLightDiffuse, aLightSpecular,
		aLightShadowFraction, aPreShadowIntensity, aLightBlindData,
		&status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( lAttr.setArray( true ) );
	CHECK_MSTATUS( lAttr.setStorable( false ) );
	CHECK_MSTATUS( lAttr.setHidden( true ) );
	CHECK_MSTATUS( lAttr.setDefault( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		true, true, false, 1.0f, 1.0f, NULL ) );


	// Next we will add the attributes we have defined to the node
	//
	CHECK_MSTATUS( addAttribute( aTranslucenceCoeff ) );
	CHECK_MSTATUS( addAttribute( aDiffuseReflectivity ) );
	CHECK_MSTATUS( addAttribute( aColor ) );
	CHECK_MSTATUS( addAttribute( aIncandescence ) );
	CHECK_MSTATUS( addAttribute( aInTransparency ) );
	CHECK_MSTATUS( addAttribute( aOutColor ) );
	CHECK_MSTATUS( addAttribute( aOutTransparency ) );
	CHECK_MSTATUS( addAttribute( aNormalCamera ) );

	// Only add the parent of the compound
	CHECK_MSTATUS( addAttribute( aLightData ) );

	// The attributeAffects() method is used to indicate when the input
	// attribute affects the output attribute. This knowledge allows Maya
	// to optimize dependencies in the graph in more complex nodes where
	// there may be several inputs and outputs, but not all the inputs
	// affect all the outputs.
	//
	CHECK_MSTATUS( attributeAffects( aTranslucenceCoeff, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aDiffuseReflectivity, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aColorR, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aColorG, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aColorB, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aColor, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aInTransR, aOutTransparency ) );
	CHECK_MSTATUS( attributeAffects( aInTransG, aOutTransparency ) );
	CHECK_MSTATUS( attributeAffects( aInTransB, aOutTransparency ) );
	CHECK_MSTATUS( attributeAffects( aInTransparency, aOutTransparency ) );
	CHECK_MSTATUS( attributeAffects( aInTransparency, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aIncandescenceR, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aIncandescenceG, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aIncandescenceB, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aIncandescence, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightIntensityR, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightIntensityB, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightIntensityG, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightIntensity, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aNormalCameraX, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aNormalCameraY, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aNormalCameraZ, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aNormalCamera, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightDirectionX, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightDirectionY, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightDirectionZ, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightDirection, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightAmbient, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightSpecular, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightDiffuse, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightShadowFraction, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aPreShadowIntensity, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightBlindData, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aLightData, aOutColor ) );

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
	if( plug == aOutColor
		|| plug == aOutColorR
		|| plug == aOutColorG
		|| plug == aOutColorB
		|| plug == aOutTransparency
		|| plug == aOutTransR
		|| plug == aOutTransG
		|| plug == aOutTransB )
	{
		MStatus status;
		MFloatVector resultColor( 0.0, 0.0, 0.0 );


		// Get surface shading parameters from input block
		//
		MFloatVector& surfaceNormal = block.inputValue( aNormalCamera,
			&status ).asFloatVector();
		CHECK_MSTATUS( status );

		MFloatVector& surfaceColor = block.inputValue( aColor,
			&status ).asFloatVector();
		CHECK_MSTATUS( status );

		MFloatVector& incandescence = block.inputValue( aIncandescence,
			&status ).asFloatVector();
		CHECK_MSTATUS( status );

		float diffuseReflectivity = block.inputValue(
			aDiffuseReflectivity, &status ).asFloat();
		CHECK_MSTATUS( status );

		// 		float translucenceCoeff = block.inputValue( aTranslucenceCoeff,
		// 				&status ).asFloat();
		// 		CHECK_MSTATUS( status );


		// Get light list
		//
		MArrayDataHandle lightData = block.inputArrayValue( aLightData,
			&status );
		CHECK_MSTATUS( status );

		int numLights = lightData.elementCount( &status );
		CHECK_MSTATUS( status );


		// Calculate the effect of the lights in the scene on the color
		//

		// Iterate through light list and get ambient/diffuse values
		//
		for( int count=1; count <= numLights; count++ )
		{
			// Get the current light out of the array
			//
			MDataHandle currentLight = lightData.inputValue( &status );
			CHECK_MSTATUS( status );


			// Get the intensity of that light
			//
			MFloatVector& lightIntensity = currentLight.child(
				aLightIntensity ).asFloatVector();


			// Find ambient component
			//
			if ( currentLight.child( aLightAmbient ).asBool() )
			{
				resultColor += lightIntensity;
			}


			// Find diffuse component
			//
			if ( currentLight.child( aLightDiffuse ).asBool() )
			{
				MFloatVector& lightDirection = currentLight.child(
					aLightDirection ).asFloatVector();
				float cosln = lightDirection * surfaceNormal;

				if ( cosln > 0.0f ) {
					resultColor += lightIntensity
						* ( cosln * diffuseReflectivity );
				}
			}


			// Advance to the next light.
			//
			if ( count < numLights ) {
				status = lightData.next();
				CHECK_MSTATUS( status );
			}
		}


		// Factor incident light with surface color and add incandescence
		//
		resultColor[0] = resultColor[0] * surfaceColor[0] + incandescence[0];
		resultColor[1] = resultColor[1] * surfaceColor[1] + incandescence[1];
		resultColor[2] = resultColor[2] * surfaceColor[2] + incandescence[2];


		// Set ouput color attribute
		//
		if ( plug == aOutColor || plug == aOutColorR || plug == aOutColorG
			|| plug == aOutColorB)
		{
			// Get the handle to the attribute
			//
			MDataHandle outColorHandle = block.outputValue( aOutColor,
				&status );
			CHECK_MSTATUS( status );
			MFloatVector& outColor = outColorHandle.asFloatVector();

			outColor = resultColor;		// Set the output value
			outColorHandle.setClean(); // Mark the output value as clean
		}


		// Set ouput transparency
		//
		if ( plug == aOutTransparency || plug == aOutTransR
			|| plug == aOutTransG || plug == aOutTransB )
		{
			MFloatVector& transparency = block.inputValue(
				aInTransparency, &status ).asFloatVector();
			CHECK_MSTATUS( status );


			// Get the handle to the attribute
			//
			MDataHandle outTransHandle = block.outputValue(
				aOutTransparency, &status );
			CHECK_MSTATUS( status );
			MFloatVector& outTrans = outTransHandle.asFloatVector();

			outTrans = transparency;   // Set the output value
			outTransHandle.setClean(); // Mark the output value as clean
		}
	}
	else
	{
		return( MS::kUnknownParameter ); // We got an unexpected plug
	}

	return( MS::kSuccess );
}


