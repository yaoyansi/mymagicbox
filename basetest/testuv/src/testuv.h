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

////////////////////////////////////////////////////////////////////////
//
// Pick.cc
//		- Pick objects by name
//
//     EGs:  doPick curveShape1
//           doPick "curveShape*"
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>

#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>

class TestUVCmd : public MPxCommand
{
public:
	TestUVCmd() {};
	virtual			~TestUVCmd();

	MStatus     	doIt( const MArgList& args );

	static void*	creator();

	static MString cCmdName();
};

