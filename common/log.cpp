#include "log.h"

#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>

void printMObjectInfo(const char* msg, MObject mobj)
{
    MStatus status;

    MFnDependencyNode fnDNode(mobj, &status); CHECK_MSTATUS(status);//
    LDbg("%s(), name=%s", msg, fnDNode.name().asChar());

    MFnDagNode fnDagNode(mobj, &status);
    CHECK_MSTATUS(status);//

    MDagPath path2; CHECK_MSTATUS(fnDagNode.getPath(path2));//
    LDbg("%s(), path2=%s", msg, path2.fullPathName().asChar());

    MDagPath dagpath2 = fnDagNode.dagPath(&status); CHECK_MSTATUS(status);//
    LDbg("%s(), dagpath2=%s", msg, dagpath2.fullPathName().asChar());
}
//
void printMeshInfo(const char* msg, MObject mobj)
{
    printMObjectInfo(msg, mobj);

    MStatus status;

    MFnMesh fnMesh(mobj, &status);
    CHECK_MSTATUS(status);

    LDbg("%s(), fnMesh.fullPathName=%s", msg, fnMesh.fullPathName().asChar());
    LDbg("%s(), fnMesh.name=%s", msg, fnMesh.name().asChar());

    MDagPath path; CHECK_MSTATUS(fnMesh.getPath(path));
    LDbg("%s(), path=%s", msg, path.fullPathName().asChar());

    MDagPath dagpath = fnMesh.dagPath(&status); CHECK_MSTATUS(status);
    LDbg("%s(), dagpath=%s", msg, dagpath.fullPathName().asChar());


}
