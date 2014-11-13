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
#pragma once

#include <maya/MIOStream.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MGlobal.h>

#include "version_helper.h"

/////////////////////////////////
// Plugin Lambert Shader Class //
/////////////////////////////////


// This class will create a new shader. Shaders are custom dependency
// graph objects so we will derive this class from the basic DG node
// type MPxNode
//

class node_template : public MPxNode
{
public:
	node_template();
	virtual			~node_template();

	static  void *	creator();
	virtual MStatus	compute( const MPlug&, MDataBlock& );
	static  MStatus	initialize();


	// postConstructor:
	// The postConstructor method allows us to call MPxNode member
	// functions during initialization. Internally maya creates two
	// objects when a user defined node is created, the internal MObject
	// and the user derived object. The association between the these
	// two objects is not made until after the MPxNode constructor is
	// called. This implies that no MPxNode member function can be called
	// from the MPxNode constructor. The postConstructor will get called
	// immediately after the constructor when it is safe to call any
	// MPxNode member function.
	//

	virtual void	postConstructor();

	static MString cTypeName();
	static MTypeId cTypeId();
	static MPxNode::Type cType();
	static const MString& classification();


protected:
	static MTypeId   m_id;  // The IFF type id
	static MString   m_classification;
	static VersionHelper    m_version_helper;

	// Surface color
	static MObject  aColor;

	// Output color
	static MObject  aOutColor;

};
