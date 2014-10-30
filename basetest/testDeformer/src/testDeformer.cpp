
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

    if( initialized_mapping == 1 )
    {
        initVertMapping(data, iter, localToWorldMatrix, mIndex);

        MObject tObj  =  thisMObject();
        MPlug setInitMode = MPlug( tObj, initialized_data  );
        setInitMode.setShort( 2 );
        iter.reset();


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

        MDataHandle meshAttrHandle = data.inputValue( driver_mesh, &status );
        CHECK_MSTATUS( status );

        MObject meshMobj;

        meshMobj = meshAttrHandle.asMesh();

        _deform_on_one_mesh(data, iter, localToWorldMatrix, mIndex,
                            meshMobj,
                            envelopeHandle, vertMapArrayData );
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





    MDataHandle meshAttrHandle = data.inputValue( driver_mesh, &status );
    CHECK_MSTATUS( status );

    MObject meshMobj = meshAttrHandle.asMesh();

    {
        _initVertMapping_on_one_mesh(meshMobj, vertMapOutArrayBuilder, allPts);// Note: vertMapOutArrayBuilder is updated in this function!
        CHECK_MSTATUS(vertMapOutArrayData.set( vertMapOutArrayBuilder ));
    }





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
                                      MObject &meshMobj,
                                      const MDataHandle &envelopeHandle, MArrayDataHandle &vertMapArrayData)
{
    MStatus status;

    float env = envelopeHandle.asFloat();

    MItMeshVertex vertIter( meshMobj, &status );
    CHECK_MSTATUS( status );

    while( !iter.isDone(&status) )
    {
        CHECK_MSTATUS( status );

        float weight = weightValue( data, mIndex, iter.index() ); //painted weight
        float ww = weight * env;

        if ( fabs(ww) > FLT_EPSILON )//if ( ww != 0 )
        {
            CHECK_MSTATUS(vertMapArrayData.jumpToElement(iter.index()));

            int index_mapped = vertMapArrayData.inputValue(&status).asInt();
            CHECK_MSTATUS( status );

            if( index_mapped >= 0 )
            {
                int prevInt;
                CHECK_MSTATUS( vertIter.setIndex(index_mapped, prevInt) );

                MPoint mappedPt = vertIter.position( MSpace::kWorld, &status );
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
void TestDeformer::_initVertMapping_on_one_mesh( MObject &meshMobj, MArrayDataBuilder &vertMapOutArrayBuilder, const MPointArray& allPts)
{
    MStatus status;


    MItMeshVertex vertIter( meshMobj, &status );
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

