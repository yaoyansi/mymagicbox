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

///////////////////////////////////////////////////////////////////////////////
//
// quadricShape.cpp
//
// Description:
//    Registers a new type of shape with maya called "quadricShape".
//    This shape will display spheres, cylinders, disks, and partial disks
//    using the OpenGL gluQuadric functions.
//
//    There are no output attributes for this shape.
//    The following input attributes define the type of shape to draw.
//
//       shapeType  : 0=cylinder, 1=disk, 2=partialDisk, 3=sphere
//       radius1	: cylinder base radius, disk inner radius, sphere radius
//       radius2	: cylinder top radius, disk outer radius
//       height		: cylinder height
//       startAngle	: partial disk start angle
//       sweepAngle	: partial disk sweep angle
//       slices		: cylinder, disk, sphere slices
//       loops		: disk loops
//       stacks		: cylinder, sphere stacks
//
////////////////////////////////////////////////////////////////////////////////

#include "testProjectionNode.h"




// #if defined(OSMac_MachO_)
// #include <OpenGL/glu.h>
// #else
// #include <GL/glu.h>
// #end
//#include <maya/MIOStream.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MPoint.h>
#include <maya/MPlug.h>
#include <maya/MDrawData.h>
#include <maya/MDrawRequest.h>
#include <maya/MSelectionMask.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MMaterial.h>

#include "texture.h"
#include "NeHeGL.h"

/////////////////////////////////////////////////////////////////////

#define MCHECKERROR(STAT,MSG)       \
    if ( MS::kSuccess != STAT ) {   \
        cerr << MSG << endl;        \
        return MS::kFailure;        \
    }

#define MAKE_NUMERIC_ATTR( NAME, SHORTNAME, TYPE, DEFAULT, KEYABLE ) \
	MStatus NAME##_stat;                                             \
	MFnNumericAttribute NAME##_fn;                                   \
	NAME = NAME##_fn.create( #NAME, SHORTNAME, TYPE, DEFAULT );      \
	MCHECKERROR(NAME##_stat, "numeric attr create error");		     \
	NAME##_fn.setHidden( !KEYABLE );								 \
	NAME##_fn.setKeyable( KEYABLE );								 \
	NAME##_fn.setInternal( true );									 \
	NAME##_stat = addAttribute( NAME );                              \
	MCHECKERROR(NAME##_stat, "addAttribute error");

#define LEAD_COLOR				18	// green
#define ACTIVE_COLOR			15	// white
#define ACTIVE_AFFECTED_COLOR	8	// purple
#define DORMANT_COLOR			4	// blue
#define HILITE_COLOR			17	// pale blue

/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// SHAPE NODE IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

MObject quadricShape::shapeType;
MObject quadricShape::radius1;
MObject quadricShape::radius2;
MObject quadricShape::height;
MObject quadricShape::startAngle;
MObject quadricShape::sweepAngle;
MObject quadricShape::slices;
MObject quadricShape::loops;
MObject quadricShape::stacks;

MObject quadricShape::camTranslateZ;
MObject quadricShape::camRotateX;
MObject quadricShape::camRotateY;
MTypeId quadricShape::id( 0x80111 );

quadricShape::quadricShape()
{
	fGeometry = new quadricGeom;
	fGeometry->radius1		= 1.0;
	fGeometry->radius2		= 1.0;
	fGeometry->height		= 2.0;
	fGeometry->startAngle	= 0.0;
	fGeometry->sweepAngle	= 90.0;
	fGeometry->slices		= 8;
	fGeometry->loops		= 6;
	fGeometry->stacks		= 4;
    fGeometry->shapeType	= 0;

	fGeometry->camTranslateZ	= 0.0;
	fGeometry->camRotateX	= 0.0;
    fGeometry->camRotateY	= 0.0;
}

quadricShape::~quadricShape()
{
	delete fGeometry;
}

/* override */
void quadricShape::postConstructor()
//
// Description
//
//    When instances of this node are created internally, the MObject associated
//    with the instance is not created until after the constructor of this class
//    is called. This means that no member functions of MPxSurfaceShape can
//    be called in the constructor.
//    The postConstructor solves this problem. Maya will call this function
//    after the internal object has been created.
//    As a general rule do all of your initialization in the postConstructor.
//
{
	// This call allows the shape to have shading groups assigned
	//
	setRenderable( true );
}

/* override */
MStatus quadricShape::compute( const MPlug& /*plug*/, MDataBlock& /*datablock*/ )
//
// Since there are no output attributes this is not necessary but
// if we wanted to compute an output mesh for rendering it would
// be done here base on the inputs.
//
{
	return MS::kUnknownParameter;
}

/* override */
bool quadricShape::getInternalValue( const MPlug& plug,
									 MDataHandle& datahandle )
//
// Handle internal attributes.
// In order to impose limits on our attribute values we
// mark them internal and use the values in fGeometry intead.
//
{
	bool isOk = true;

	if ( plug == radius1 ) {
		datahandle.set( fGeometry->radius1 );
		isOk = true;
	}
	else if ( plug == radius2 ) {
		datahandle.set( fGeometry->radius2 );
		isOk = true;
	}
	else if ( plug == height ) {
		datahandle.set( fGeometry->height );
		isOk = true;
	}
	else if ( plug == startAngle ) {
		datahandle.set( fGeometry->startAngle );
		isOk = true;
	}
	else if ( plug == sweepAngle ) {
		datahandle.set( fGeometry->sweepAngle );
		isOk = true;
	}
	else if ( plug == slices ) {
		datahandle.set( fGeometry->slices );
		isOk = true;
	}
	else if ( plug == loops ) {
		datahandle.set( fGeometry->loops );
		isOk = true;
	}
	else if ( plug == stacks ) {
		datahandle.set( fGeometry->stacks );
		isOk = true;
	}
	else if ( plug == camTranslateZ ) {
		datahandle.set( fGeometry->camTranslateZ );
		isOk = true;
	}
	else if ( plug == camRotateX ) {
		datahandle.set( fGeometry->camRotateX );
		isOk = true;
	}
	else if ( plug == camRotateY ) {
		datahandle.set( fGeometry->camRotateY );
		isOk = true;
	}
	else {
		isOk = MPxSurfaceShape::getInternalValue( plug, datahandle );
	}

	return isOk;
}
/* override */
bool quadricShape::setInternalValue( const MPlug& plug,
									 const MDataHandle& datahandle )
//
// Handle internal attributes.
// In order to impose limits on our attribute values we
// mark them internal and use the values in fGeometry intead.
//
{
	bool isOk = true;

	// In the case of a disk or partial disk the inner radius must
	// never exceed the outer radius and the minimum radius is 0
	//
	if ( plug == radius1 ) {
		double innerRadius = datahandle.asDouble();
		double outerRadius = fGeometry->radius2;

		if ( innerRadius > outerRadius ) {
			outerRadius = innerRadius;
		}

		if ( innerRadius < 0 ) {
			innerRadius = 0;
		}

		fGeometry->radius1 = innerRadius;
		fGeometry->radius2 = outerRadius;
		isOk = true;
	}
	else if ( plug == radius2 ) {
		double outerRadius = datahandle.asDouble();
		double innerRadius = fGeometry->radius1;

		if ( outerRadius <= 0 ) {
			outerRadius = 0.1;
		}

		if ( innerRadius > outerRadius ) {
			innerRadius = outerRadius;
		}

		if ( innerRadius < 0 ) {
			innerRadius = 0;
		}

		fGeometry->radius1 = innerRadius;
		fGeometry->radius2 = outerRadius;
		isOk = true;
	}
	else if ( plug == height ) {
		double val = datahandle.asDouble();
		if ( val <= 0 ) {
			val = 0.1;
		}
		fGeometry->height = val;
	}
	else if ( plug == startAngle ) {
		double val = datahandle.asDouble();
		fGeometry->startAngle = val;
	}
	else if ( plug == sweepAngle ) {
		double val = datahandle.asDouble();
		fGeometry->sweepAngle = val;
	}
	else if ( plug == slices ) {
		short val = datahandle.asShort();
		if ( val < 3 ) {
			val = 3;
		}
		fGeometry->slices = val;
	}
	else if ( plug == loops ) {
		short val = datahandle.asShort();
		if ( val < 3 ) {
			val = 3;
		}
		fGeometry->loops = val;
	}
	else if ( plug == stacks ) {
		short val = datahandle.asShort();
		if ( val < 2 ) {
			val = 2;
		}
		fGeometry->stacks = val;
	}
	else if ( plug == camTranslateZ ) {
		double val = datahandle.asDouble();
		fGeometry->camTranslateZ = val;
	}
	else if ( plug == camRotateX ) {
		double val = datahandle.asDouble();
		fGeometry->camRotateX = val;
	}
	else if ( plug == camRotateY ) {
		double val = datahandle.asDouble();
		fGeometry->camRotateY = val;
	}
	else {
		isOk = MPxSurfaceShape::setInternalValue( plug, datahandle );
	}

	return isOk;
}

/* override */
bool quadricShape::isBounded() const { return true; }

/* override */
MBoundingBox quadricShape::boundingBox() const
//
// Returns the bounding box for the shape.
// In this case just use the radius and height attributes
// to determine the bounding box.
//
{
	MBoundingBox result;
	quadricShape* nonConstThis = const_cast <quadricShape*> (this);
	quadricGeom* geom = nonConstThis->geometry();

	double r = geom->radius1;
	result.expand( MPoint(r,r,r) );	result.expand( MPoint(-r,-r,-r) );
	r = geom->radius2;
	result.expand( MPoint(r,r,r) );	result.expand( MPoint(-r,-r,-r) );
	r = geom->height;
	result.expand( MPoint(r,r,r) );	result.expand( MPoint(-r,-r,-r) );

    return result;
}

void* quadricShape::creator()
{
	return new quadricShape();
}

MStatus quadricShape::initialize()
{
	MStatus				stat;
    MFnNumericAttribute	numericAttr;
    MFnEnumAttribute	enumAttr;

	// QUADRIC type enumerated attribute
	//
	shapeType = enumAttr.create( "shapeType", "st", 0, &stat );
	MCHECKERROR( stat, "create shapeType attribute" );
		enumAttr.addField( "cylinder", 0 );
		enumAttr.addField( "disk", 1 );
		enumAttr.addField( "partialDisk", 2 );
		enumAttr.addField( "sphere", 3 );
	enumAttr.setHidden( false );
	enumAttr.setKeyable( true );
	stat = addAttribute( shapeType );
	MCHECKERROR( stat, "Error adding shapeType attribute." );

	// QUADRIC ATTRIBUTES
	//
	MAKE_NUMERIC_ATTR( radius1, "r1", MFnNumericData::kDouble, 1.0, true );
	MAKE_NUMERIC_ATTR( radius2, "r2", MFnNumericData::kDouble, 1.0, true );
	MAKE_NUMERIC_ATTR( height, "ht", MFnNumericData::kDouble, 2.0, true );
	MAKE_NUMERIC_ATTR( startAngle, "sta", MFnNumericData::kDouble, 0.0, true );
	MAKE_NUMERIC_ATTR( sweepAngle, "swa", MFnNumericData::kDouble, 90.0, true );
	MAKE_NUMERIC_ATTR( slices, "sl", MFnNumericData::kShort, 8, true );
	MAKE_NUMERIC_ATTR( loops, "lp", MFnNumericData::kShort, 6, true );
	MAKE_NUMERIC_ATTR( stacks, "sk", MFnNumericData::kShort, 4, true );

	MAKE_NUMERIC_ATTR( camTranslateZ, "camTranslateZ", MFnNumericData::kDouble, 0.0, true );
	MAKE_NUMERIC_ATTR( camRotateX, "camRotateX", MFnNumericData::kDouble, 0.0, true );
	MAKE_NUMERIC_ATTR( camRotateY, "camRotateY", MFnNumericData::kDouble, 0.0, true );
	return stat;
}

quadricGeom* quadricShape::geometry()
//
// This function gets the values of all the attributes and
// assigns them to the fGeometry. Calling MPlug::getValue
// will ensure that the values are up-to-date.
//
{
	MObject this_object = thisMObject();
	MPlug plug( this_object, radius1 );	plug.getValue( fGeometry->radius1 );
	plug.setAttribute( radius2 );		plug.getValue( fGeometry->radius2 );
	plug.setAttribute( height );		plug.getValue( fGeometry->height );
	plug.setAttribute( startAngle );	plug.getValue( fGeometry->startAngle );
	plug.setAttribute( sweepAngle );	plug.getValue( fGeometry->sweepAngle );
	plug.setAttribute( slices );		plug.getValue( fGeometry->slices );
	plug.setAttribute( loops );			plug.getValue( fGeometry->loops );
	plug.setAttribute( stacks );		plug.getValue( fGeometry->stacks );
	plug.setAttribute( shapeType );		plug.getValue( fGeometry->shapeType );

	plug.setAttribute( camTranslateZ );	plug.getValue( fGeometry->camTranslateZ );
	plug.setAttribute( camRotateX );	plug.getValue( fGeometry->camRotateX );
	plug.setAttribute( camRotateY );	plug.getValue( fGeometry->camRotateY );
	return fGeometry;
}

/////////////////////////////////////////////////////////////////////
// UI IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

quadricShapeUI::quadricShapeUI() 
{
	g_Texture = 0;
	g_Texture = LoadTexture("texture.bmp");
}
//
quadricShapeUI::~quadricShapeUI() 
{

}

void* quadricShapeUI::creator()
{
	return new quadricShapeUI();
}

/* override */
void quadricShapeUI::getDrawRequests( const MDrawInfo & info,
							 bool /*objectAndActiveOnly*/,
							 MDrawRequestQueue & queue )
{
	// The draw data is used to pass geometry through the
	// draw queue. The data should hold all the information
	// needed to draw the shape.
	//
	MDrawData data;
	MDrawRequest request = info.getPrototype( *this );
	quadricShape* shapeNode = (quadricShape*)surfaceShape();
	quadricGeom* geom = shapeNode->geometry();
	getDrawData( geom, data );
	request.setDrawData( data );

	// Are we displaying meshes?
	if ( ! info.objectDisplayStatus( M3dView::kDisplayMeshes ) )
		return;

	// Use display status to determine what color to draw the object
	//
	switch ( info.displayStyle() )
	{
		case M3dView::kWireFrame :
			getDrawRequestsWireframe( request, info );
			queue.add( request );
			break;

		case M3dView::kGouraudShaded :
			request.setToken( kDrawSmoothShaded );
			getDrawRequestsShaded( request, info, queue, data );
			queue.add( request );
			break;

		case M3dView::kFlatShaded :
			request.setToken( kDrawFlatShaded );
 			getDrawRequestsShaded( request, info, queue, data );
			queue.add( request );
			break;
		default:
			break;
	}
}

/* override */
void quadricShapeUI::draw( const MDrawRequest & request, M3dView & view ) const
//
// From the given draw request, get the draw data and determine
// which quadric to draw and with what values.
//
{
	//

	MDrawData data = request.drawData();
	quadricGeom * geom = (quadricGeom*)data.geometry();
	int token = request.token();
	bool drawTexture = false;



	view.beginGL();

	if ( (token == kDrawSmoothShaded) || (token == kDrawFlatShaded) )
	{
#if		defined(SGI) || defined(MESA)
		glEnable( GL_POLYGON_OFFSET_EXT );
#else
		glEnable( GL_POLYGON_OFFSET_FILL );
#endif
		// Set up the material
		//
		MMaterial material = request.material();
		material.setMaterial( request.multiPath(), request.isTransparent() );

		// Enable texturing
		//
		drawTexture = material.materialIsTextured();
		if ( drawTexture ) glEnable(GL_TEXTURE_2D);

		// Apply the texture to the current view
		//
		if ( drawTexture ) {
			material.applyTexture( view, data );
		}
	}

	GLUquadricObj* qobj = gluNewQuadric();

	switch( token )
	{
		case kDrawWireframe :
		case kDrawWireframeOnShaded :
			gluQuadricDrawStyle( qobj, GLU_LINE );
			break;

		case kDrawSmoothShaded :
			gluQuadricNormals( qobj, GLU_SMOOTH );
			gluQuadricTexture( qobj, true );
			gluQuadricDrawStyle( qobj, GLU_FILL );
			break;

		case kDrawFlatShaded :
			gluQuadricNormals( qobj, GLU_FLAT );
			gluQuadricTexture( qobj, true );
			gluQuadricDrawStyle( qobj, GLU_FILL );
			break;
	}

	switch ( geom->shapeType )
	{
	case kDrawCylinder :
		gluCylinder( qobj, geom->radius1, geom->radius2, geom->height,
					 geom->slices, geom->stacks );
		break;
	case kDrawDisk :
		gluDisk( qobj, geom->radius1, geom->radius2, geom->slices, geom->loops );
		break;
	case kDrawPartialDisk :
		//gluPartialDisk( qobj, geom->radius1, geom->radius2, geom->slices,
		//				geom->loops, geom->startAngle, geom->sweepAngle );
		{
			//test1_manipulateUV(geom);
			test2_rtt(geom);
		}
		break;
	case kDrawSphere :
	default :
		gluSphere( qobj, geom->radius1, geom->slices, geom->stacks );
		break;
	}

	// Turn off texture mode
	//
	if ( drawTexture ) glDisable(GL_TEXTURE_2D);

	view.endGL();
}

/* override */
bool quadricShapeUI::select( MSelectInfo &selectInfo,
							 MSelectionList &selectionList,
							 MPointArray &worldSpaceSelectPts ) const
//
// Select function. Gets called when the bbox for the object is selected.
// This function just selects the object without doing any intersection tests.
//
{
	MSelectionMask priorityMask( MSelectionMask::kSelectObjectsMask );
	MSelectionList item;
	item.add( selectInfo.selectPath() );
	MPoint xformedPt;
	selectInfo.addSelection( item, xformedPt, selectionList,
							 worldSpaceSelectPts, priorityMask, false );
	return true;
}

void quadricShapeUI::getDrawRequestsWireframe( MDrawRequest& request,
											   const MDrawInfo& info )
{
	request.setToken( kDrawWireframe );

	M3dView::DisplayStatus displayStatus = info.displayStatus();
	M3dView::ColorTable activeColorTable = M3dView::kActiveColors;
	M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;
	switch ( displayStatus )
	{
		case M3dView::kLead :
			request.setColor( LEAD_COLOR, activeColorTable );
			break;
		case M3dView::kActive :
			request.setColor( ACTIVE_COLOR, activeColorTable );
			break;
		case M3dView::kActiveAffected :
			request.setColor( ACTIVE_AFFECTED_COLOR, activeColorTable );
			break;
		case M3dView::kDormant :
			request.setColor( DORMANT_COLOR, dormantColorTable );
			break;
		case M3dView::kHilite :
			request.setColor( HILITE_COLOR, activeColorTable );
			break;
		default:
			break;
	}
}

void quadricShapeUI::getDrawRequestsShaded( MDrawRequest& request,
											const MDrawInfo& info,
											MDrawRequestQueue& queue,
											MDrawData& data )
{
	// Need to get the material info
	//
	MDagPath path = info.multiPath();	// path to your dag object
	M3dView view = info.view();; 		// view to draw to
	MMaterial material = MPxSurfaceShapeUI::material( path );
	M3dView::DisplayStatus displayStatus = info.displayStatus();

	// Evaluate the material and if necessary, the texture.
	//
	if ( ! material.evaluateMaterial( view, path ) ) {
		cerr << "Couldnt evaluate\n";
	}

	bool drawTexture = true;
	if ( drawTexture && material.materialIsTextured() ) {
		material.evaluateTexture( data );
	}

	request.setMaterial( material );

	bool materialTransparent = false;
	material.getHasTransparency( materialTransparent );
	if ( materialTransparent ) {
		request.setIsTransparent( true );
	}

	// create a draw request for wireframe on shaded if
	// necessary.
	//
	if ( (displayStatus == M3dView::kActive) ||
		 (displayStatus == M3dView::kLead) ||
		 (displayStatus == M3dView::kHilite) )
	{
		MDrawRequest wireRequest = info.getPrototype( *this );
		wireRequest.setDrawData( data );
		getDrawRequestsWireframe( wireRequest, info );
		wireRequest.setToken( kDrawWireframeOnShaded );
		wireRequest.setDisplayStyle( M3dView::kWireFrame );
		queue.add( wireRequest );
	}
}

//
void quadricShapeUI::getPerspectiveMatrix(double fovy, double aspect, double zNear, GLdouble zFar, MMatrix &mm)const
{
	mm.setToIdentity();

    double sine, cotangent, deltaZ;
    double radians = fovy / 2.0f * M_PI / 180.0f;

    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
		return;
    }
    cotangent = cos(radians) / sine;


    mm[0][0] = cotangent / aspect;
    mm[1][1] = cotangent;
    mm[2][2] = -(zFar + zNear) / deltaZ;
    mm[2][3] = -1;
    mm[3][2] = -2 * zNear * zFar / deltaZ;
    mm[3][3] = 0;


}
//
double quadricShapeUI::zDepthFactor(double z, double _near, double _far)const
{
	return (z - _near)/(_far - _near);
}
//
void quadricShapeUI::test1_manipulateUV(const quadricGeom *geom)const
{
	double uScale = geom->camRotateX;
	double vScale = geom->camRotateY;


	double fovy = 90.0f;//degree;
	double aspect = 800.0/600; 
	double zNear = 0.1f;
	double zFar = 40.0f;
	MMatrix mPerspective;
	getPerspectiveMatrix(fovy, aspect, zNear, zFar, mPerspective);

	MPoint cam(0.0, 0.0, geom->camTranslateZ);

	// test plane vertex
	MPoint p0(  0.0f,  0.0f,  0.0);
	MPoint p1( 10.0f,  0.0f,  0.0);
	MPoint p2( 10.0f, 10.0f,  0.0);
	MPoint p3(  0.0f, 10.0f,  0.0);

	MMatrix camRotateIve;
	{
		MMatrix camRotate;

		MTransformationMatrix mRotX0;
		mRotX0.setToRotationAxis( MVector(1.0, 0, 0), uScale * M_PI/180.0f );// rotate around X axis
		MTransformationMatrix mRotY0;
		mRotY0.setToRotationAxis( MVector(0, 1.0, 0), vScale * M_PI/180.0f );// rotate around Y axis

		camRotate = mRotX0.asMatrix() * mRotY0.asMatrix();

		//MEulerRotation mRot1(geom->camRotateX, geom->camRotateY, 0.0);
		//camRotate = mRot1.asMatrix();

		camRotateIve = camRotate.inverse();
	}



	// p0 --> projected point:p0_p, ...
	MPoint p0_p = (p0 - cam) * camRotateIve * mPerspective;
	MPoint p1_p = (p1 - cam) * camRotateIve * mPerspective;
	MPoint p2_p = (p2 - cam) * camRotateIve * mPerspective;
	MPoint p3_p = (p3 - cam) * camRotateIve * mPerspective;

#ifdef _DEBUG
	std::cout<< "----------------------------------------"<< mPerspective <<std::endl;
	std::cout<< "p0_p="<< p0_p <<std::endl;
	std::cout<< "p1_p="<< p1_p <<std::endl;
	std::cout<< "p2_p="<< p2_p <<std::endl;
	std::cout<< "p3_p="<< p3_p <<std::endl;
#endif

	double zDepthFactor0 = zDepthFactor(p0_p.z, zNear, zFar);
	double zDepthFactor1 = zDepthFactor(p1_p.z, zNear, zFar);
	double zDepthFactor2 = zDepthFactor(p2_p.z, zNear, zFar);
	double zDepthFactor3 = zDepthFactor(p3_p.z, zNear, zFar);

	// p0_p --> screen point: p0_s
	const double screen_width  = p1.x - p0.x;
	const double screen_height = p2.y - p0.y;
	MPoint p0_s((p0_p.x-screen_width/2) * zDepthFactor0 , (p0_p.y-screen_height/2) * zDepthFactor0, 1.0f );
	MPoint p1_s((p1_p.x-screen_width/2) * zDepthFactor1 , (p1_p.y-screen_height/2) * zDepthFactor1, 1.0f );
	MPoint p2_s((p2_p.x-screen_width/2) * zDepthFactor2 , (p2_p.y-screen_height/2) * zDepthFactor2, 1.0f );
	MPoint p3_s((p3_p.x-screen_width/2) * zDepthFactor3 , (p3_p.y-screen_height/2) * zDepthFactor3, 1.0f );

	// draw the test plane
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(p0_s.x+0.5f,	p0_s.y+0.5f);	glVertex3d(p0.x, p0.y, p0.z);
	glTexCoord2d(p1_s.x+0.5f,	p1_s.y+0.5f);	glVertex3d(p1.x, p1.y, p1.z);
	glTexCoord2d(p2_s.x+0.5f,	p2_s.y+0.5f);	glVertex3d(p2.x, p2.y, p2.z);
	glTexCoord2d(p3_s.x+0.5f,	p3_s.y+0.5f);	glVertex3d(p3.x, p3.y, p3.z);
	glEnd();
}
//
void quadricShapeUI::test2_rtt(const quadricGeom *geom)const
{
	static bool rtt_inited = false;
	if( !rtt_inited )
	{
		if (GLEW_OK != glewInit()) {
			std::cout << "Couldn't initialize GLEW" << std::endl;
			//exit(0);
		}
		init();
		rtt_inited = true;
	}
	display();

}