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

#include <maya/MString.h>


class ObjectUV
{
public:
	ObjectUV(const MString& name);
	~ObjectUV();

protected:
    ObjectUV(const ObjectUV& o);
    ObjectUV& operator=(const ObjectUV& o);

protected:
    const MString m_objectname;
};

