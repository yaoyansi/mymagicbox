
#include "testDeformer.h"


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
MObject  TestDeformer::aFilePath;
MObject  TestDeformer::aGridName;
MObject  TestDeformer::aOutColor;
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
TestDeformer::TestDeformer() { }
//
TestDeformer::~TestDeformer() { }
//
void* TestDeformer::creator()
{
	return new TestDeformer();
}
//
MStatus TestDeformer::initialize()
{
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnStringData sData;
	MStatus status; // Status will be used to hold the MStatus value

	// returned by each api function call. It is important
	// to check the status returned by a call to aid in
	// debugging. Failed API calls can result in subtle
	// errors that can be difficult to track down, you may
	// wish to use the CHECK_MSTATUS macro for any API
	// call where you do not need to provide your own
	// error handling.
	//

	// Attribute Initialization:
	aFilePath = tAttr.create( "filepath", "file", MFnData::kString, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( tAttr.setKeyable( true ) );
	CHECK_MSTATUS( tAttr.setStorable( true ) );
	CHECK_MSTATUS( tAttr.setDefault( sData.create("") ) );

	aGridName = tAttr.create( "grid", "grd", MFnData::kString, &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( tAttr.setKeyable( true ) );
	CHECK_MSTATUS( tAttr.setStorable( true ) );
	CHECK_MSTATUS( tAttr.setDefault( sData.create("") ) );

	aOutColor = nAttr.createColor( "outColor", "oc", &status );
	CHECK_MSTATUS( status );
	CHECK_MSTATUS( nAttr.setHidden( false ) );
	CHECK_MSTATUS( nAttr.setReadable( true ) );
	CHECK_MSTATUS( nAttr.setWritable( false ) );

	// Next we will add the attributes we have defined to the node
	//
	CHECK_MSTATUS( addAttribute( aFilePath ) );
	CHECK_MSTATUS( addAttribute( aGridName ) );
	CHECK_MSTATUS( addAttribute( aOutColor ) );

	// The attributeAffects() method is used to indicate when the input
	// attribute affects the output attribute. This knowledge allows Maya
	// to optimize dependencies in the graph in more complex nodes where
	// there may be several inputs and outputs, but not all the inputs
	// affect all the outputs.
	//
	CHECK_MSTATUS( attributeAffects( aFilePath, aOutColor ) );
	CHECK_MSTATUS( attributeAffects( aGridName, aOutColor ) );

	return( MS::kSuccess );
}
//
MStatus TestDeformer::deform(MDataBlock& block,
                          MItGeometry& iter,
                          const MMatrix& mat,
                          unsigned int multiIndex)
{
	return( MS::kSuccess );
}


