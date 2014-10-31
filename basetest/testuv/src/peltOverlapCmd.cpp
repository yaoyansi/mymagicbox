
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

#include "peltOverlapCmd.h"


#define kExitFlag	"-ea"
#define kExitFlagLong	"-exitAfterNthPairs"

///////////////////////////////////////////////////
//
// Command class declaration
//
///////////////////////////////////////////////////

peltOverlap::peltOverlap()
	: fNthPairs (1)
{}

peltOverlap::~peltOverlap() {}

void* peltOverlap::creator()
{
	return new peltOverlap;
}

MSyntax peltOverlap::newSyntax()
{
	 MSyntax syntax;
	 syntax.addFlag(kExitFlag, kExitFlagLong, MSyntax::kUnsigned);
	 syntax.setObjectType(MSyntax::kStringObjects);
	 return syntax;
}

MStatus peltOverlap::parseArgs( const MArgList& args )
{
	MStatus		status = MS::kSuccess;
	MArgDatabase	argData(syntax(), args);

	if (argData.isFlagSet(kExitFlag)) {
	    status = argData.getFlagArgument(kExitFlag, 0, fNthPairs);
	    if (status != MS::kSuccess) {
	        MGlobal::displayError("-ea/exitAfterNthPairs is missing an int argument");
		return status;
	    }
	}

	status = argData.getObjects(fShadingGroups);
	if (status != MS::kSuccess || fShadingGroups.length() < 1) {
	        MGlobal::displayError("Missing shading group(s) input");
		status = MS::kFailure;
	}

	return status;
}

MStatus peltOverlap::doIt( const MArgList& args )
//
// Description
//     Return the overlapping faces in pairs for given shading groups.
//
{
	MStatus status = parseArgs ( args );
	if ( !status ) return status;

	// Iterate over shading groups
	for (unsigned int i = 0; i < fShadingGroups.length(); i++ )
	{
	    // Get a list of faces through mel because it's so much easier this way.
	    // It should be fine because this is not performance critical.
	    MStringArray faces, flattenFaces;
	    MGlobal::executeCommand("sets -q " + fShadingGroups[i], faces, false, false);
	    MGlobal::clearSelectionList();
	    for(unsigned int j = 0; j < faces.length(); j++)
	    {
		MString nodeType = MGlobal::executeCommandStringResult("nodeType " + faces[j]);
		if (nodeType == "mesh") {
		    MGlobal::selectByName(faces[j]); // add to list
		}
	    }
	    MGlobal::executeCommand("ConvertSelectionToFaces");
	    MGlobal::executeCommand("ls -sl -flatten", flattenFaces, false, false);

	    // return overlap faces from shading group
	    numOverlapUVFaces(fShadingGroups[i], flattenFaces);
	}
	return MS::kSuccess;
}

void peltOverlap::createBoundingCircle(const MStringArray &flattenFaces, MFloatArray &center, MFloatArray &radius)
//
// Description
//     Represent a face by a center and radius, i.e.
//     center = {center1u, center1v, center2u, center2v, ... }
//     radius = {radius1, radius2,  ... }
//
{
    center.setLength(2 * flattenFaces.length());
    radius.setLength(flattenFaces.length());
	for(unsigned int i = 0; i < flattenFaces.length(); i++)
    {
		MSelectionList selList;
		selList.add(flattenFaces[i]);
		MDagPath dagPath;
		MObject  comp;
		selList.getDagPath(0, dagPath, comp);
		MItMeshPolygon iter(dagPath, comp);

		MFloatArray uArray, vArray;
		iter.getUVs(uArray, vArray);
		// Loop through all vertices to construct edges/rays
		float cu = 0.f;
		float cv = 0.f;
		unsigned int j;
		for(j = 0; j < uArray.length(); j++) {
		        cu += uArray[j];
			cv += vArray[j];
		}
		cu = cu / uArray.length();
		cv = cv / vArray.length();
		float rsqr = 0.f;
		for(j = 0; j < uArray.length(); j++) {
		        float du = uArray[j] - cu;
			float dv = vArray[j] - cv;
			float dsqr = du*du + dv*dv;
			rsqr = dsqr > rsqr ? dsqr : rsqr;
		}
		center[2*i]   = cu;
		center[2*i+1] = cv;
		radius[i]  = sqrt(rsqr);
	}
}
//     Represent a face by a series of edges(rays), i.e.
//     orig = {orig1u, orig1v, orig2u, orig2v, ... }
//     vec  = {vec1u,  vec1v,  vec2u,  vec2v,  ... }
//     return false if no valid uv's.
bool peltOverlap::createRayGivenFace(const MString &face, MFloatArray &orig, MFloatArray &vec)
{
    MSelectionList selList;
	selList.add(face);
	MDagPath dagPath;
	MObject  comp;
	selList.getDagPath(0, dagPath, comp);
	MItMeshPolygon iter(dagPath, comp);

	MFloatArray uArray, vArray;
	iter.getUVs(uArray, vArray);

	if (uArray.length() == 0 || vArray.length() == 0) return false;

	orig.setLength(2 * uArray.length());
	vec.setLength( 2 * uArray.length());

	// Loop through all vertices to construct edges/rays
	float u = uArray[uArray.length() - 1];
	float v = vArray[vArray.length() - 1];
	for(unsigned int j = 0; j < uArray.length(); j++) {
		orig[2*j]   = uArray[j];
		orig[2*j+1] = vArray[j];
		vec[2*j]    = u - uArray[j];
		vec[2*j+1]  = v - vArray[j];
		u = uArray[j];
		v = vArray[j];
	}
	return true;
}
// Check if there are crossing edges between two faces. Return true
// if there are crossing edges and false otherwise. A face is represented
// by a series of edges(rays), i.e.
// faceOrig[] = {orig1u, orig1v, orig2u, orig2v, ... }
// faceVec[]  = {vec1u,  vec1v,  vec2u,  vec2v,  ... }
unsigned int peltOverlap::checkCrossingEdges(
    MFloatArray &face1Orig,
    MFloatArray &face1Vec,
    MFloatArray &face2Orig,
    MFloatArray &face2Vec
)
{
	unsigned int face1Size = face1Orig.length();
	unsigned int face2Size = face2Orig.length();

	for (unsigned int i = 0; i < face1Size; i += 2)
    {
		float o1x = face1Orig[i];
		float o1y = face1Orig[i+1];
		float v1x = face1Vec[i];
		float v1y = face1Vec[i+1];
		float n1x =  v1y;
		float n1y = -v1x;
		for (unsigned int j = 0; j < face2Size; j += 2)
        {
			// Given ray1(O1, V1) and ray2(O2, V2)
			// Normal of ray1 is (V1.y, V1.x)
			float o2x = face2Orig[j];
			float o2y = face2Orig[j+1];
			float v2x = face2Vec[j];
			float v2y = face2Vec[j+1];
			float n2x =  v2y;
			float n2y = -v2x;

			///////////////////////////////////////////////////////////////////
			////        check if the two edges cross with each other        ///
			///////////////////////////////////////////////////////////////////
			// Find t for ray2
			// t = [(o1x-o2x)n1x + (o1y-o2y)n1y] / (v2x * n1x + v2y * n1y)
			float denum = v2x * n1x + v2y * n1y;
			// Edges are parallel if denum is close to 0.
			if (fabs(denum) < 0.000001f)
                continue;
			float t2 = ((o1x-o2x)* n1x + (o1y-o2y) * n1y) / denum;

			if (t2 < 0.00001f || t2 > 0.99999f)
                continue;

			// Find t for ray1
			// t = [(o2x-o1x)n2x + (o2y-o1y)n2y] / (v1x * n2x + v1y * n2y)
			denum = v1x * n2x + v1y * n2y;
			// Edges are parallel if denum is close to 0.
			if (fabs(denum) < 0.000001f)
                continue;

			float t1 = ((o2x-o1x)* n2x + (o2y-o1y) * n2y) / denum;

			// Edges intersect
			if (t1 > 0.00001f && t1 < 0.99999f)
                return 1;
		}
	}
	return 0;
}
// Return overlapping faces in pairs for given a shading group and its associated faces.
void peltOverlap::numOverlapUVFaces(const MString& shadingGroup, MStringArray& flattenFaces)
{
	MFloatArray  face1Orig, face1Vec, face2Orig, face2Vec, center, radius;

	// Loop through face i
	unsigned int numOverlap = 0;
	createBoundingCircle(flattenFaces, center, radius);

	for(unsigned int i = 0; i < flattenFaces.length() && numOverlap < fNthPairs; i++)
    {
        // for face[i]
        //construct point and edge(line section) for face[i]
        if(!createRayGivenFace(flattenFaces[i], face1Orig, face1Vec))
            continue;

        // center point of face[i]
		const float cui  = center[2*i];
		const float cvi  = center[2*i+1];

		//radius of face[i]
		const float ri  = radius[i];

		//////////////////////////////////////////////
		//////// test face[i] and other faces  ///////
		//////////////////////////////////////////////
		// Loop through face j where j != i
		for(unsigned int j = i+1; j < flattenFaces.length() && numOverlap < fNthPairs; j++)
		{
            // center point of face[j]
			const float &cuj = center[2*j];
			const float &cvj = center[2*j+1];

            //radius of face[j]
			const float &rj  = radius[j];

			// Quick rejection if bounding circles don't overlap
			// 如果两个线段的中心点的距离大于它们的外接圆的半径和, 则这两个线段不可能相交
			float du = cuj - cui;
			float dv = cvj - cvi;
			float dsqr = du*du + dv*dv;
			if (dsqr >= (ri+rj)*(ri+rj))
                continue;

            //construct point and edge(line section) for face[j]
			if(!createRayGivenFace(flattenFaces[j], face2Orig, face2Vec))
                continue;

            // test if the two line sections cross with eachother
			if (checkCrossingEdges(face1Orig, face1Vec, face2Orig, face2Vec)) {
				numOverlap++;
				appendToResult(flattenFaces[i]);
				appendToResult(flattenFaces[j]);
				continue;
			}
		}
	}
}

