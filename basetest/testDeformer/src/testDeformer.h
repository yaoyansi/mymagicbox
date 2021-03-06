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
#include <maya/MPxDeformerNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItGeometry.h>
#include <maya/MMatrix.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>

//#define _DEBUG
/////////////////////////////////
// Plugin Lambert Shader Class //
/////////////////////////////////



class TestDeformer : public MPxDeformerNode
{
public:
	TestDeformer();
	virtual			~TestDeformer();

	static  void *	creator();

	static  MStatus	initialize();

	virtual void	postConstructor();
    virtual MStatus deform(MDataBlock& data,
                          MItGeometry& iter,
                          const MMatrix& localToWorldMatrix,
                          unsigned int mIndex);

	static MString          cTypeName();
	static MTypeId          cTypeId();
	static MPxNode::Type    cType();
	static const MString&   cClassification();

private:
    void initVertMapping(MDataBlock& data,
                          MItGeometry& iter,
                          const MMatrix& localToWorldMatrix,
                          unsigned int mIndex);
    int getClosestPt(const MPoint &pt, const MPointArray &points);

    // accumulate the deform values of one driver mesh to outputPtr
    void _deform_on_one_mesh(MDataBlock& data,
                              MItGeometry& iter,
                              const MMatrix& localToWorldMatrix,
                              unsigned int mIndex,
                              MObject &meshMobj,
                              const MDataHandle &envelopeHandle, MArrayDataHandle &vertMapArrayData, MPointArray &outputPtr);
    // initilize the data from one driver mesh
    void _initVertMapping_on_one_mesh( MObject &meshMobj, MArrayDataBuilder &vertMapOutArrayBuilder, const MPointArray& allPts);

    // utility functions
    static void __debug(const char* format, ...);
    static void __debugMeshInfo(const char* msg, MObject &object);

protected:
	static MTypeId   m_id;
	static MString   m_classification;

	static MObject  driver_mesh;
	static MObject  initialized_data;
	static MObject  vert_map;
};
