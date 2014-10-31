
////////////////////////////////////////////////////////////////////////
//
// peltOverlapCmd.cpp
//
// This plugin checks and returns the overlapping faces in pairs for a given
// list of shading groups. The faces can be from the same mesh or different meshes, i.e.
//
// 1) Return the first overlapping faces associated to "shadingGroupName1"
// peltOverlap "shadingGroupName1";
//
// 2) Return the first 100 overlapping faces associated to "shadingGroupName1"
//       and the first 100 overlapping faces associated to "shadingGroupName2".
// peltOverlap -exitAfterNthError 100 "shadingGroupName1" "shadingGroupName2";
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MFloatArray.h>
#include <maya/MArgList.h>

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshPolygon.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnSet.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>


class peltOverlap : public MPxCommand
{
public:
                         peltOverlap();
        virtual		~peltOverlap();

        MStatus		doIt( const MArgList& args );

	static MSyntax	newSyntax();
	static void*	creator();

private:

	MStatus		parseArgs( const MArgList &args );
        void            createBoundingCircle(const MStringArray &flattenFaces, MFloatArray &center, MFloatArray &radius);
        bool            createRayGivenFace(const MString &face, MFloatArray &orig, MFloatArray &vec);
        unsigned int    checkCrossingEdges(MFloatArray &face1Orig,
					   MFloatArray &face1Vec,
					   MFloatArray &face2Orig,
					   MFloatArray &face2Vec);
        void            numOverlapUVFaces(const MString &shadingGroup, MStringArray &flattenFaces);

	unsigned int	fNthPairs;
        MStringArray    fShadingGroups;
};
