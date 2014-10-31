
#include "testDeformer.h"

#include <cfloat>

// For local testing of nodes you can use any identifier between
// 0x00000000 and 0x0007ffff, but for any node that you plan to use for
// more permanent purposes, you should get a universally unique id from
// Autodesk Support. You will be assigned a unique range that you
// can manage on your own.
//
MTypeId TestDeformer::m_id( 0x7269b );
MString TestDeformer::m_classification("utility/general");

///////////////////////////////////////////////////////
// DESCRIPTION: attribute information
///////////////////////////////////////////////////////
//
MObject  TestDeformer::driver_mesh;
MObject  TestDeformer::initialized_data;
MObject  TestDeformer::vert_map;
//
void TestDeformer::postConstructor( )
{
	// setMPSafe indicates that this shader can be used for multiprocessor
	// rendering. For a shading node to be MP safe, it cannot access any
	// shared global data and should only use attributes in the datablock
	// to get input data and store output data.
	//
	setMPSafe( true );
}
//
MString TestDeformer::cTypeName()
{
	return "testDeformer";
}
//
MTypeId TestDeformer::cTypeId()
{
	return TestDeformer::m_id;
}
//
MPxNode::Type TestDeformer::cType()
{
	return MPxNode::kDeformerNode;
}
//
const MString& TestDeformer::cClassification()
{
	return m_classification;
}
//
TestDeformer::TestDeformer()
: MPxDeformerNode()
{
}
//
TestDeformer::~TestDeformer()
{
}
//
void* TestDeformer::creator()
{
	return new TestDeformer();
}
//
MStatus TestDeformer::initialize()
{
	MFnNumericAttribute numericAttr;
	MFnTypedAttribute polyMeshAttr;
	MFnEnumAttribute enumAttr;

	MStatus status; // Status will be used to hold the MStatus value

	// returned by each api function call. It is important
	// to check the status returned by a call to aid in
	// debugging. Failed API calls can result in subtle
	// errors that can be difficult to track down, you may
	// wish to use the CHECK_MSTATUS macro for any API
	// call where you do not need to provide your own
	// error handling.
	//

	// vertSnapInput
	driver_mesh = polyMeshAttr.create( "vertSnapInput", "vsnpin", MFnData::kMesh, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( polyMeshAttr.setStorable( false ) );
	CHECK_MSTATUS( polyMeshAttr.setArray(true) );
	CHECK_MSTATUS( polyMeshAttr.setConnectable( true ) );
	CHECK_MSTATUS( addAttribute(driver_mesh) );
	CHECK_MSTATUS( attributeAffects(driver_mesh, outputGeom) );

	//
	initialized_data = enumAttr.create( "initialize", "inl", 0/*default*/, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( enumAttr.addField(	"Off", 0) );
	CHECK_MSTATUS( enumAttr.addField(	"Re-Set Bind", 1) );
	CHECK_MSTATUS( enumAttr.addField(	"Bound", 2) );
	CHECK_MSTATUS( enumAttr.setKeyable(true) );
	CHECK_MSTATUS( enumAttr.setStorable(true) );
	CHECK_MSTATUS( enumAttr.setReadable(true) );
	CHECK_MSTATUS( enumAttr.setWritable(true) );
	CHECK_MSTATUS( enumAttr.setDefault(0) );
	CHECK_MSTATUS( addAttribute( initialized_data ) );
	CHECK_MSTATUS( attributeAffects( initialized_data, outputGeom ) );


	vert_map = numericAttr.create( "vtxIndexMap", "vtximp", MFnNumericData::kLong, 0/*default*/, &status );
	CHECK_MSTATUS( status );
    CHECK_MSTATUS( numericAttr.setKeyable(false) );
	CHECK_MSTATUS( numericAttr.setArray(true) );
	CHECK_MSTATUS( numericAttr.setStorable(true) );
	CHECK_MSTATUS( numericAttr.setReadable(true) );
	CHECK_MSTATUS( numericAttr.setWritable(true) );
	CHECK_MSTATUS( addAttribute( vert_map ) );
	CHECK_MSTATUS( attributeAffects( vert_map, outputGeom ) );

	CHECK_MSTATUS( MGlobal::executePythonCommand("import maya.cmds; maya.cmds.makePaintable('"+TestDeformer::cTypeName()+"', 'weights', attrType='multiFloat')") );

	return( MS::kSuccess );
}
//
MStatus TestDeformer::deform(MDataBlock& data,
                          MItGeometry& iter,
                          const MMatrix& localToWorldMatrix,
                          unsigned int mIndex)
{
    MStatus status;

    short initialized_mapping = data.inputValue( initialized_data, &status).asShort();
    CHECK_MSTATUS(status);
    __debug("%s(), initialized_mapping=%d", __FUNCTION__, initialized_mapping);

    if( initialized_mapping == 1 )
    {
        initVertMapping(data, iter, localToWorldMatrix, mIndex);

//        MObject tObj  =  thisMObject();
//        MPlug setInitMode = MPlug( tObj, initialized_data  );
//        setInitMode.setShort( 2 );


        initialized_mapping = data.inputValue( initialized_data, &status ).asShort();
        CHECK_MSTATUS(status);
    }

    if( initialized_mapping == 2 )
    {
        envelope = MPxDeformerNode::envelope;
        MDataHandle envelopeHandle = data.inputValue( envelope, &status );
        CHECK_MSTATUS( status );

        MArrayDataHandle vertMapArrayData  = data.inputArrayValue( vert_map, &status  );
        CHECK_MSTATUS( status );

        MArrayDataHandle meshAttrHandle = data.inputArrayValue( driver_mesh, &status );
        CHECK_MSTATUS( status );

        int numMeshes = meshAttrHandle.elementCount();
        __debug("%s(), numMeshes=%d", __FUNCTION__, numMeshes);

        CHECK_MSTATUS(meshAttrHandle.jumpToElement(0));
        for( int count=0; count < numMeshes; ++count )
        {
            __debug("%s(), count=%d", __FUNCTION__, count);

            MDataHandle currentMesh = meshAttrHandle.inputValue(&status);
            CHECK_MSTATUS( status );


            MObject meshMobj;

            meshMobj = currentMesh.asMesh();
            __debugMeshInfo(__FUNCTION__, meshMobj);

            _deform_on_one_mesh(data, iter, localToWorldMatrix, mIndex,
                                meshMobj,
                                envelopeHandle, vertMapArrayData );


            if( !meshAttrHandle.next() )
            {
                break;
            }

        }
    }// if

	return( MS::kSuccess );
}
//
//
void TestDeformer::initVertMapping(MDataBlock& data,
                          MItGeometry& iter,
                          const MMatrix& localToWorldMatrix,
                          unsigned int mIndex)
{
    MStatus status;


    MArrayDataHandle vertMapOutArrayData = data.outputArrayValue( vert_map, &status );
    CHECK_MSTATUS( status );

    iter.reset();
    int count = iter.count();
    MArrayDataBuilder vertMapOutArrayBuilder( vert_map, count, &status );
    CHECK_MSTATUS( status );




    MPointArray allPts;// world vertex position of the driven mesh
    allPts.clear();
    // walk through the driven mesh
    int i = 0;
    while( !iter.isDone(&status) )
    {
        CHECK_MSTATUS( status );

        MDataHandle initIndexDataHnd = vertMapOutArrayBuilder.addElement( i, &status );
        CHECK_MSTATUS( status );

        int negIndex = -1;

        initIndexDataHnd.setInt( negIndex );
        initIndexDataHnd.setClean();

        CHECK_MSTATUS(allPts.append( iter.position() * localToWorldMatrix ));
        i = i+1;
        iter.next();
    }
    CHECK_MSTATUS(vertMapOutArrayData.set( vertMapOutArrayBuilder ));





    MArrayDataHandle meshAttrHandle = data.inputArrayValue( driver_mesh, &status );
    CHECK_MSTATUS( status );

    int numMeshes = meshAttrHandle.elementCount();
    __debug("%s(), numMeshes=%d", __FUNCTION__, numMeshes);

    CHECK_MSTATUS(meshAttrHandle.jumpToElement(0));
    for( int meshIndex=0; meshIndex < numMeshes; ++meshIndex )
    {
        __debug("%s(), meshIndex=%d", __FUNCTION__, meshIndex);

        MDataHandle currentMesh = meshAttrHandle.inputValue(&status);
        CHECK_MSTATUS(status);

        MObject meshMobj = currentMesh.asMesh();
        __debug("%s(), meshMobj.apiTypeStr()=%s", __FUNCTION__, meshMobj.apiTypeStr());

        __debugMeshInfo(__FUNCTION__, meshMobj);
        {
            _initVertMapping_on_one_mesh(meshMobj, vertMapOutArrayBuilder, allPts);// Note: vertMapOutArrayBuilder is updated in this function!
            CHECK_MSTATUS(vertMapOutArrayData.set( vertMapOutArrayBuilder ));
        }

        if( !meshAttrHandle.next() )
        {
            break;
        }
    }// for (mesh




}
// which one is the closest point(in points array) to pt
int TestDeformer::getClosestPt(const MPoint &pt, const MPointArray &points)
{
    int ptIndex=0;
	float currentDistance = 9e99;
	float furthestDistanceSoFar = 9e99;

	for(int i = 0; i < points.length(); ++i)
    {
        currentDistance = pt.distanceTo( points[i] );
        if(currentDistance < furthestDistanceSoFar )
        {
            ptIndex = i;
            furthestDistanceSoFar = currentDistance;
        }
    }

	return ptIndex;
}

void TestDeformer::_deform_on_one_mesh(MDataBlock& data,
                                      MItGeometry& iter,
                                      const MMatrix& localToWorldMatrix,
                                      unsigned int mIndex,
                                      MObject &driver_mesh,
                                      const MDataHandle &envelopeHandle, MArrayDataHandle &vertMapArrayData)
{
    MStatus status;

    float env = envelopeHandle.asFloat();

    MItMeshVertex driver_meshVertIter( driver_mesh, &status );
    CHECK_MSTATUS( status );

    iter.reset();
    while( !iter.isDone(&status) )
    {
        CHECK_MSTATUS( status );

        float weight = weightValue( data, mIndex, iter.index() ); //painted weight
        float ww = weight * env;

        if ( fabs(ww) > FLT_EPSILON )//if ( ww != 0 )
        {
            __debug("%s(), vertMapArrayData.elementCount()=%d, iter.index()=%d",
                    __FUNCTION__, vertMapArrayData.elementCount(), iter.index());

            CHECK_MSTATUS(vertMapArrayData.jumpToElement(iter.index()));

            int index_mapped = vertMapArrayData.inputValue(&status).asInt();
            CHECK_MSTATUS( status );

            if( index_mapped >= 0 )
            {
                __debug("index_mapped=%d", index_mapped);

                int prevInt;
                CHECK_MSTATUS( driver_meshVertIter.setIndex(index_mapped, prevInt) );

                MPoint mappedPt = driver_meshVertIter.position( MSpace::kWorld, &status );
                CHECK_MSTATUS( status );

                MPoint iterPt = iter.position(MSpace::kObject, &status) * localToWorldMatrix;
                CHECK_MSTATUS( status );

                MPoint pt = iterPt + ((mappedPt - iterPt) * ww );
                pt = pt * localToWorldMatrix.inverse();
                CHECK_MSTATUS(iter.setPosition( pt ));
            }
        }//if
        CHECK_MSTATUS(iter.next());
    }//while
}

//
void TestDeformer::_initVertMapping_on_one_mesh( MObject &driver_mesh, MArrayDataBuilder &vertMapOutArrayBuilder, const MPointArray& allPts)
{
    MStatus status;


    MItMeshVertex vertIter( driver_mesh, &status );
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(vertIter.reset());


    // for each vertex of the driver mesh
    while( !vertIter.isDone(&status) )
    {
        CHECK_MSTATUS(status);

        // get vertex position
        MPoint driver_pt;
        driver_pt = vertIter.position( MSpace::kWorld, &status );
        CHECK_MSTATUS(status);

        //get the closest driven point
        int closest_pt_index = getClosestPt( driver_pt, allPts );//which one is the closest point(in allPts array) to driver_pt

        //add the closest driven point
        MDataHandle snapDataHnd = vertMapOutArrayBuilder.addElement( closest_pt_index, &status );
        CHECK_MSTATUS( status );
        snapDataHnd.setInt( vertIter.index() );
        snapDataHnd.setClean();

        CHECK_MSTATUS(vertIter.next());
    }
}
//
void TestDeformer::__debug(const char* format, ...) const
{
	va_list args;
	char msg[ 1024 ];

	va_start(args, format);

	vsnprintf(msg, 1024, format, args);

	fprintf(stdout, "Debug> %s\n", msg);

	va_end(args);
}
//
void TestDeformer::__debugMeshInfo(const char* msg, MObject &meshMobj)const
{
#ifdef _DEBUG

        MStatus status;

        MFnMesh fnMesh(meshMobj, &status);
        CHECK_MSTATUS(status);

        __debug("%s(), fnMesh.fullPathName=%s", msg, fnMesh.fullPathName().asChar());
        __debug("%s(), fnMesh.name=%s", msg, fnMesh.name().asChar());

        MDagPath path; CHECK_MSTATUS(fnMesh.getPath(path));
        __debug("%s(), path=%s", msg, path.fullPathName().asChar());

        MDagPath dagpath = fnMesh.dagPath(&status); CHECK_MSTATUS(status);
        __debug("%s(), dagpath=%s", msg, dagpath.fullPathName().asChar());

        MFnDependencyNode fnDNode(meshMobj, &status); CHECK_MSTATUS(status);//
        __debug("%s(), name=%s", msg, fnDNode.name().asChar());

        MFnDagNode fnDagNode(meshMobj, &status);
        CHECK_MSTATUS(status);//

        MDagPath path2; CHECK_MSTATUS(fnDagNode.getPath(path2));//
        __debug("%s(), path2=%s", msg, path2.fullPathName().asChar());

        MDagPath dagpath2 = fnDagNode.dagPath(&status); CHECK_MSTATUS(status);//
        __debug("%s(), dagpath2=%s", msg, dagpath2.fullPathName().asChar());
#endif
}
