
////////////////////////////////////////////////////////////////////////
//
// peltOverlapCmd.cpp
//
// This plugin checks and returns the overlapping faces in pairs for a given
// list of shading groups. The faces can be from the same mesh or different meshes, i.e.
//
// 1) Return the first overlapping faces associated to "shadingGroupName1"
// peltOverlapCmd "shadingGroupName1";
//
// 2) Return the first 100 overlapping faces associated to "shadingGroupName1"
//       and the first 100 overlapping faces associated to "shadingGroupName2".
// peltOverlap -exitAfterNthError 100 "shadingGroupName1" "shadingGroupName2";
//
////////////////////////////////////////////////////////////////////////

#include <maya/MFloatArray.h>
#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

class peltOverlap : public MPxCommand
{
public:
    peltOverlap();
    ~peltOverlap();

    virtual MStatus		doIt( const MArgList& args );

	static MSyntax	newSyntax();
	static void*	creator();
	static MString cCmdName();

private:

    MStatus		    parseArgs( const MArgList &args );
    void            createBoundingCircle(const MStringArray &flattenFaces, MFloatArray &center, MFloatArray &radius);
    bool            createRayGivenFace(const MString &face, MFloatArray &orig, MFloatArray &vec);
    unsigned int    checkCrossingEdges(MFloatArray &face1Orig,
                                       MFloatArray &face1Vec,
                                       MFloatArray &face2Orig,
                                       MFloatArray &face2Vec);
    void            numOverlapUVFaces(const MString &shadingGroup, MStringArray &flattenFaces);

private:
    unsigned int	fNthPairs;
    MStringArray    fShadingGroups;
};
