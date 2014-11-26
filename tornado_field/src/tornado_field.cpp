
#include "tornado_field.h"
#include <cassert>
#include <cmath>

#include <maya/MArrayDataBuilder.h>
#include <maya/MDagPath.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MTime.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>

#include <common/node_ids.h>
#include <common/log.h>

MObject tornadoField::aMinDistance;
MObject tornadoField::aAttractDistance;
MObject tornadoField::aRepelDistance;
MObject tornadoField::aDrag;
MObject tornadoField::aSwarmAmplitude;
MObject tornadoField::aSwarmFrequency;
MObject tornadoField::aSwarmPhase;

MTypeId tornadoField::m_id( NodeID_tornado_field );
MString tornadoField::m_classification("utility/general");

//
tornadoField::tornadoField()
{
}
//
tornadoField::~tornadoField()
{
}
//
void tornadoField::postConstructor()
{
    setMPSafe(true);

    MStatus status;

    MFnDependencyNode fnDepNode(thisMObject(), &status);
    CHECK_MSTATUS(status);

    // If the new name conflicts with the name of an existing node then
    // the object will be given a unique name based on the supplied name.
    // If the new name ends in a single '#' it will be replaced with a number
    // that ensures the new name is unique.
    fnDepNode.setName(cTypeName()+"#", &status);
    CHECK_MSTATUS(status);
}
//
MString tornadoField::cTypeName()
{
	return "tornadoField";
}
//
MTypeId tornadoField::cTypeId()
{
	return tornadoField::m_id;
}
//
MPxNode::Type tornadoField::cType()
{
	return MPxNode::kFieldNode;
}
//
const MString& tornadoField::cClassification()
{
	return m_classification;
}
//
void *tornadoField::creator()
{
    return new tornadoField;
}

MStatus tornadoField::initialize()
//
//	Descriptions:
//		Initialize the node, attributes.
//
{
	MStatus status;

	MFnNumericAttribute numAttr;

	// create the field basic attributes.
	//
	aMinDistance = numAttr.create("minDistance","mnd",MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aMinDistance );
	McheckErr(status, "ERROR adding aMinDistance attribute.\n");

	aAttractDistance = numAttr.create("attractDistance","ad",
										MFnNumericData::kDouble);
	numAttr.setDefault( 20.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aAttractDistance );
	McheckErr(status, "ERROR adding aAttractDistance attribute.\n");

	aRepelDistance = numAttr.create("repelDistance","rd",
										MFnNumericData::kDouble);
	numAttr.setDefault( 10.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aRepelDistance );
	McheckErr(status, "ERROR adding aRepelDistance attribute.\n");

	aDrag = numAttr.create("drag", "d", MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aDrag );
	McheckErr(status, "ERROR adding aDrag attribute.\n");

	aSwarmAmplitude = numAttr.create("swarmAmplitude", "samp",
										MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aSwarmAmplitude );
	McheckErr(status, "ERROR adding aSwarmAmplitude attribute.\n");

	aSwarmFrequency = numAttr.create("swarmFrequency", "sfrq",
										MFnNumericData::kDouble);
	numAttr.setDefault( 1.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aSwarmFrequency );
	McheckErr(status, "ERROR adding aSwarmFrequency attribute.\n");

	aSwarmPhase = numAttr.create("swarmPhase", "sa", MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aSwarmPhase );
	McheckErr(status, "ERROR adding aSwarmPhase attribute.\n");

	// the new attribute will affect output force.
	//
	//status = attributeAffects( aMinDistance, mOutputForce );
	//McheckErr(status, "ERROR in attributeAffects(aMinDistance,mOutputForce).\n");
	//status = attributeAffects( aAttractDistance, mOutputForce );
	//McheckErr(status, "ERROR in attributeAffects(aAttractDistance,mOutputForce).\n");
	//status = attributeAffects( aRepelDistance, mOutputForce );
	//McheckErr(status, "ERROR in attributeAffects(aRepelDistance,mOutputForce).\n");
	//status = attributeAffects( aDrag, mOutputForce );
	//McheckErr(status, "ERROR in attributeAffects(aDrag,mOutputForce).\n");

	return( MS::kSuccess );
}


MStatus tornadoField::compute(const MPlug& plug, MDataBlock& block)
//
//	Descriptions:
//		compute output force.
//
{
	MStatus status;

	if( !(plug == mOutputForce) )
        return( MS::kUnknownParameter );

	// get the logical index of the element this plug refers to.
	//
	int multiIndex = plug.logicalIndex( &status );
	McheckErr(status, "ERROR in plug.logicalIndex.\n");

	// Get input data handle, use outputArrayValue since we do not
	// want to evaluate both inputs, only the one related to the
	// requested multiIndex. Evaluating both inputs at once would cause
	// a dependency graph loop.
	//
	MArrayDataHandle hInputArray = block.outputArrayValue( mInputData, &status );
	McheckErr(status,"ERROR in hInputArray = block.outputArrayValue().\n");

	status = hInputArray.jumpToElement( multiIndex );
	McheckErr(status, "ERROR: hInputArray.jumpToElement failed.\n");

	// get children of aInputData.
	//
	MDataHandle hCompond = hInputArray.inputValue( &status );
	McheckErr(status, "ERROR in hCompond=hInputArray.inputValue\n");

	MDataHandle hPosition = hCompond.child( mInputPositions );
	MObject dPosition = hPosition.data();
	MFnVectorArrayData fnPosition( dPosition );
	MVectorArray points = fnPosition.array( &status );
	McheckErr(status, "ERROR in fnPosition.array(), not find points.\n");

	MDataHandle hVelocity = hCompond.child( mInputVelocities );
	MObject dVelocity = hVelocity.data();
	MFnVectorArrayData fnVelocity( dVelocity );
	MVectorArray velocities = fnVelocity.array( &status );
	McheckErr(status, "ERROR in fnVelocity.array(), not find velocities.\n");

	MDataHandle hMass = hCompond.child( mInputMass );
	MObject dMass = hMass.data();
	MFnDoubleArrayData fnMass( dMass );
	MDoubleArray masses = fnMass.array( &status );
	McheckErr(status, "ERROR in fnMass.array(), not find masses.\n");


	// points and velocities should have the same length. If not return.
	//
	if( points.length() != velocities.length() )
    {
        LWrn("points.length() != velocities.length(), %d, %d", points.length(), velocities.length());
		return MS::kInvalidParameter;
    }

	// Compute the output force.
	//
	MVectorArray forceArray(points.length(), MVector::zero);
	status = _getForce(
           block,
           points,
           velocities,
           masses,
           forceArray,
           -1.0// deltaTime is not provided in compute()
    );
    CHECK_MSTATUS(status);

	// get output data handle
	//
	MArrayDataHandle hOutArray = block.outputArrayValue( mOutputForce, &status);
	McheckErr(status, "ERROR in hOutArray = block.outputArrayValue.\n");
	MArrayDataBuilder bOutArray = hOutArray.builder( &status );
	McheckErr(status, "ERROR in bOutArray = hOutArray.builder.\n");

	// get output force array from block.
	//
	MDataHandle hOut = bOutArray.addElement(multiIndex, &status);
	McheckErr(status, "ERROR in hOut = bOutArray.addElement.\n");

	MFnVectorArrayData fnOutputForce;
	MObject dOutputForce = fnOutputForce.create( forceArray, &status );
	McheckErr(status, "ERROR in dOutputForce = fnOutputForce.create\n");

	// update data block with new output force data.
	//
	hOut.set( dOutputForce );
	block.setClean( plug );

	return( MS::kSuccess );
}


void tornadoField::applyNoMaxDist
	(
		MDataBlock &block,				// get field param from this block
		const MVectorArray &points,		// current position of Object
		const MVectorArray &velocities,	// current velocity of Object
		const MDoubleArray &masses,		// mass of Object
		MVectorArray &outputForce		// output force
	)
//
//	Descriptions:
//		Compute output force in the case that the useMaxDistance is not set.
//
{
    MStatus status;

    assert(outputForce.length() == points.length());

	// get field parameters.
	//
	double magValue = magnitudeValue( block );
	// double attenValue = attenuationValue( block );
	double minDist = minDistanceValue( block );
	double attractDist = attractDistanceValue( block );
	double repelDist = repelDistanceValue( block );
	double dragMag = dragValue( block );
	double swarmAmp = swarmAmplitudeValue( block );

	//printMObjectInfo("", thisMObject());
	MMatrix worldMatrix;// field's world matrix
	CHECK_MSTATUS(worldMatrixValue(block, worldMatrix));
	MTransformationMatrix transformMatrix(worldMatrix);

    //MQuaternion rotation(transformMatrix.rotation());
    //COUT("rotation=", rotation);

    // direction
    MVector UP(0.0, 1.0, 0.0);// the original up direction is (0, 1, 0),
    UP = UP * worldMatrix;    // the new up direction after rotation
    const MVector UPdir(UP.normal());
    //COUT("up=", up);

    MVector ORIGINAL(transformMatrix.getTranslation(MSpace::kWorld, &status)); CHECK_MSTATUS(status);
    //COUT("translation=", translation);

	// get owner's data. posArray may have only one point which is the centroid
	// (if this has owner) or field position(if without owner). Or it may have
	// a list of points if with owner and applyPerVertex.
	//
	MVectorArray posArray;
	posArray.clear();
	ownerPosition( block, posArray );

	int fieldPosCount = posArray.length();
	int receptorSize = points.length();

	// With this model,if max distance isn't set then we
	// also don't attenuate, because 1 - dist/maxDist isn't
	// meaningful. No max distance and no attenuation.
	//
    for (int i = 0; i < receptorSize; i ++ )
    {

/*
             UPdir ^
                   |
                Pp |-----R--->o P
                   |         /
                   |        /
                   |       /
                   |      /
                   |     /
                   |    /
                   |   /
                   |  /
                   | /
                   |/
                   o
                ORIGINAL

*/      const MVector &P(points[i]);
        const double  &M(masses[i]);
        const MVector &V(velocities[i]);

        float l = UPdir * (P - ORIGINAL);
        MVector Pp = ORIGINAL + UP * l;// Pp is the projection of P onto UP
        MVector R  = P - Pp;
        MVector Rdir = R.normal();
        MVector Fdir = (UPdir ^ V).normal();// direction of F



        // F = m × (v²/r)
        double f = 1.0;
        const double minRadius = 0.001;// to avoid dividing Zero
        if(R.length() > minRadius)
        {
            f = M * (V.length() * V.length() / R.length());
        }else{
            f = M * (V.length() * V.length() / minRadius);
        }

        const MVector F(Fdir * f);

        outputForce[i] += F ;// accumulate the force for particle i
    }

}


void tornadoField::applyMaxDist
	(
		MDataBlock& block,				// get field param from this block
		const MVectorArray &points,		// current position of Object
		const MVectorArray &velocities,	// current velocity of Object
		const MDoubleArray &/*masses*/,		// mass of Object
		MVectorArray &outputForce		// output force
	)
//
//	Descriptions:
//		Compute output force in the case that the useMaxDistance is set.
//
{
	// points and velocities should have the same length. If not return.
	//
	if( points.length() != velocities.length() )
		return;

	// clear the output force array.
	//
	outputForce.clear();

	// get field parameters.
	//
	double magValue = magnitudeValue( block );
	double attenValue = attenuationValue( block );
	double maxDist = maxDistanceValue( block );
	double minDist = minDistanceValue( block );
	double attractDist = attractDistanceValue( block );
	double repelDist = repelDistanceValue( block );
	double dragMag = dragValue( block );
	double swarmAmp = swarmAmplitudeValue( block );

	// get owner's data. posArray may have only one point which is the centroid
	// (if this has owner) or field position(if without owner). Or it may have
	// a list of points if with owner and applyPerVertex.
	//
	MVectorArray posArray;
	posArray.clear();
	ownerPosition( block, posArray );

	int fieldPosCount = posArray.length();
	int receptorSize = points.length();

	if (attenValue > 0.0)
	{
		// Max distance applies and so does attenuation.
		//
		for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
		{
			const MVector &receptorPoint = points[ptIndex];

			// Apply from every field position to every receptor position.
			//
			MVector forceV(0,0,0);
			MVector sumForceV(0,0,0);
			for(int i = fieldPosCount; --i>=0; )
			{
				MVector difference = receptorPoint-posArray[i];
				double distance  = difference.length();
				if (distance <= maxDist && distance >= minDist )
				{
					double force = magValue *
									(pow((1.0-(distance/maxDist)),attenValue));
					forceV = difference * force;

					// Apply drag and swarm if the object is inside
					// the zone the repulsion-attraction is pushing the
					// object to, and if they are set.
					//
					if ( distance >= repelDist && distance <= attractDist)
					{
						if (fieldPosCount > 0 && dragMag > 0)
						{
							MVector dragForceV;
							dragForceV = velocities[ptIndex] *
											(-dragMag) * fieldPosCount;
							forceV += dragForceV;
						}

						// Add swarm if swarm aplitude is set.
						//
						if (swarmAmp > 0)
						{
							double frequency = swarmFrequencyValue( block );
							MVector phase( 0.0, 0.0, swarmPhaseValue(block) );

							difference = receptorPoint - posArray[i];
							difference = (difference + phase) * frequency;
							double *noiseEffect = &difference.x;
							if( (noiseEffect[0] < -2147483647.0) ||
								(noiseEffect[0] >  2147483647.0) ||
								(noiseEffect[1] < -2147483647.0) ||
								(noiseEffect[1] >  2147483647.0) ||
								(noiseEffect[2] < -2147483647.0) ||
								(noiseEffect[2] >  2147483647.0) )
								continue;

							double noiseOut[4];
							noiseFunction( noiseEffect, noiseOut );
							MVector swarmForce( noiseOut[0] * swarmAmp,
												noiseOut[1] * swarmAmp,
												noiseOut[2] * swarmAmp );
							forceV += swarmForce;
						}
					}
				}
				if (maxDist > 0.0) forceV *= falloffCurve(distance/maxDist);
				sumForceV += forceV;
			}
			outputForce.append( sumForceV );
		}
	}
	else
	{
		// Max dist applies, but not attenuation.
		//
		for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
		{
			const MVector & receptorPoint = points[ptIndex];

			// Apply from every field position to every receptor position.
			//
			MVector forceV(0,0,0);
			MVector sumForceV(0,0,0);
			int i;
			for(i = fieldPosCount; --i>=0; )
			{
				MVector difference = (receptorPoint-posArray[i]);
				double distance = difference.length();
				if (distance < minDist || distance > maxDist) continue;

				if (distance <= repelDist)
					forceV = difference * magValue;
				else if (distance >= attractDist)
					forceV = -difference * magValue;

				// Apply drag and swarm if the object is inside
				// the zone the repulsion-attraction is pushing the
				// object to, and if they are set.
				//
				if ( distance >= repelDist && distance <= attractDist)
				{
					if (fieldPosCount > 0 && dragMag > 0)
					{
						MVector dragForceV;
						dragForceV = velocities[ptIndex] *
											(-dragMag) * fieldPosCount;
						forceV += dragForceV;
					}

					// Add swarm if swarm aplitude is set.
					//
					if (swarmAmp > 0)
					{
						double frequency = swarmFrequencyValue( block );
						MVector phase( 0.0, 0.0, swarmPhaseValue(block) );

						for(i = fieldPosCount; --i >= 0;)
						{
							difference = receptorPoint - posArray[i];
							difference = (difference + phase) * frequency;
							double *noiseEffect = &difference.x;
							if( (noiseEffect[0] < -2147483647.0) ||
								(noiseEffect[0] >  2147483647.0) ||
								(noiseEffect[1] < -2147483647.0) ||
								(noiseEffect[1] >  2147483647.0) ||
								(noiseEffect[2] < -2147483647.0) ||
								(noiseEffect[2] >  2147483647.0) )
								continue;

							double noiseOut[4];
							noiseFunction( noiseEffect, noiseOut );
							MVector swarmForce( noiseOut[0] * swarmAmp,
												noiseOut[1] * swarmAmp,
												noiseOut[2] * swarmAmp );

							forceV += swarmForce;
						}
					}
				}
				if (maxDist > 0.0) forceV *= falloffCurve(distance/maxDist);
				sumForceV += forceV;
			}
			outputForce.append( sumForceV );
		}
	}
}


void tornadoField::ownerPosition
	(
		MDataBlock& block,
		MVectorArray &ownerPosArray
	)
//
//	Descriptions:
//		If this field has an owner, get the owner's position array or
//		centroid, then assign it to the ownerPosArray.
//		If it does not have owner, get the field position in the world
//		space, and assign it to the given array, ownerPosArray.
//
{
	MStatus status;

	if( applyPerVertexValue(block) )
	{
		MDataHandle hOwnerPos = block.inputValue( mOwnerPosData, &status );
		if( status == MS::kSuccess )
		{
			MObject dOwnerPos = hOwnerPos.data();
			MFnVectorArrayData fnOwnerPos( dOwnerPos );
			MVectorArray posArray = fnOwnerPos.array( &status );
			if( status == MS::kSuccess )
			{
				// assign vectors from block to ownerPosArray.
				//
				for( unsigned int i = 0; i < posArray.length(); i ++ )
					ownerPosArray.append( posArray[i] );
			}
			else
			{
				MVector worldPos(0.0, 0.0, 0.0);
				//status = getWorldPosition( block, worldPos );
				status = getWorldPosition( worldPos );
				ownerPosArray.append( worldPos );
			}
		}
		else
		{
			// get the field position in the world space
			// and add it into ownerPosArray.
			//
			MVector worldPos(0.0, 0.0, 0.0);
			//status = getWorldPosition( block, worldPos );
			status = getWorldPosition( worldPos );
			ownerPosArray.append( worldPos );
		}
	}
	else
	{
		MVector centroidV(0.0, 0.0, 0.0);
		status = ownerCentroidValue( block, centroidV );
		if( status == MS::kSuccess )
		{
			// assign centroid vector to ownerPosArray.
			//
			ownerPosArray.append( centroidV );
		}
		else
		{
			// get the field position in the world space.
			//
			MVector worldPos(0.0, 0.0, 0.0);
			//status = getWorldPosition( block, worldPos );
			status = getWorldPosition( worldPos );
			ownerPosArray.append( worldPos );
		}
	}
}


MStatus tornadoField::getWorldPosition( MVector &vector )
//
//	Descriptions:
//		get the field position in the world space.
//		The position value is from inherited attribute, aWorldMatrix.
//
{
	MStatus status;

	MObject thisNode = thisMObject();
	MFnDependencyNode fnThisNode( thisNode );

	// get worldMatrix attribute.
	//
	MObject worldMatrixAttr = fnThisNode.attribute( "worldMatrix" );

	// build worldMatrix plug, and specify which element the plug refers to.
	// We use the first element(the first dagPath of this field).
	//
	MPlug matrixPlug( thisNode, worldMatrixAttr );
	matrixPlug = matrixPlug.elementByLogicalIndex( 0 );

	// Get the value of the 'worldMatrix' attribute
	//
	MObject matrixObject;
	status = matrixPlug.getValue( matrixObject );
	if( !status )
	{
		status.perror("tornadoField::getWorldPosition: get matrixObject");
		return( status );
	}

	MFnMatrixData worldMatrixData( matrixObject, &status );
	if( !status )
	{
		status.perror("tornadoField::getWorldPosition: get worldMatrixData");
		return( status );
	}

	MMatrix worldMatrix = worldMatrixData.matrix( &status );
	if( !status )
	{
		status.perror("tornadoField::getWorldPosition: get worldMatrix");
		return( status );
	}

	// assign the translate to the given vector.
	//
	vector[0] = worldMatrix( 3, 0 );
	vector[1] = worldMatrix( 3, 1 );
	vector[2] = worldMatrix( 3, 2 );

    return( status );
}


MStatus tornadoField::getWorldPosition( MDataBlock& block, MVector &vector )
//
//	Descriptions:
//		Find the field position in the world space.
//
{
    MStatus status;

	MObject thisNode = thisMObject();
	MFnDependencyNode fnThisNode( thisNode );

	// get worldMatrix attribute.
	//
	MObject worldMatrixAttr = fnThisNode.attribute( "worldMatrix" );

	// build worldMatrix plug, and specify which element the plug refers to.
	// We use the first element(the first dagPath of this field).
	//
	MPlug matrixPlug( thisNode, worldMatrixAttr );
	matrixPlug = matrixPlug.elementByLogicalIndex( 0 );

    //MDataHandle hWMatrix = block.inputValue( worldMatrix, &status );
    MDataHandle hWMatrix = block.inputValue( matrixPlug, &status );

	McheckErr(status, "ERROR getting hWMatrix from dataBlock.\n");

    if( status == MS::kSuccess )
    {
        MMatrix wMatrix = hWMatrix.asMatrix();
        vector[0] = wMatrix(3, 0);
        vector[1] = wMatrix(3, 1);
        vector[2] = wMatrix(3, 2);
    }
    return( status );
}

MStatus tornadoField::getForceAtPoint(const MVectorArray&	points,
                                  const MVectorArray&	velocities,
                                  const MDoubleArray&	masses,
                                  MVectorArray&	forceArray,
                                  double	deltaTime)
//
//    This method is not required to be overridden, it is only necessary
//    for compatibility with the MFnField function set.
//
{
	MDataBlock block = forceCache();

    return _getForce( block, points, velocities, masses, forceArray, deltaTime);
}
//
MStatus tornadoField::_getForce(
    MDataBlock& block,
    const MVectorArray& point,
    const MVectorArray& velocity,
    const MDoubleArray& mass,
    MVectorArray& force,
    double deltaTime
)
{
	bool useMaxDistSet = useMaxDistanceValue( block );
	if( useMaxDistSet )
	{
		applyMaxDist( block, point, velocity, mass, force );
	}
	else
	{
		applyNoMaxDist( block, point, velocity, mass, force );
	}

    return MS::kSuccess;
}
//
MStatus tornadoField::iconSizeAndOrigin(	GLuint& width,
					GLuint& height,
					GLuint& xbo,
					GLuint& ybo   )
//
//	This method is not required to be overridden.  It should be overridden
//	if the plug-in has custom icon.
//
//	The width and height have to be a multiple of 32 on Windows and 16 on
//	other platform.
//
//	Define an 8x8 icon at the lower left corner of the 32x32 grid.
//	(xbo, ybo) of (4,4) makes sure the icon is center at origin.
//
{
	width = 32;
	height = 32;
	xbo = 4;
	ybo = 4;
	return MS::kSuccess;
}

MStatus tornadoField::iconBitmap(GLubyte* bitmap)
//
//	This method is not required to be overridden.  It should be overridden
//	if the plug-in has custom icon.
//
//	Define an 8x8 icon at the lower left corner of the 32x32 grid.
//	(xbo, ybo) of (4,4) makes sure the icon is center at origin.
{
	bitmap[0] = 0x18;
	bitmap[4] = 0x66;
	bitmap[8] = 0xC3;
	bitmap[12] = 0x81;
	bitmap[16] = 0x81;
	bitmap[20] = 0xC3;
	bitmap[24] = 0x66;
	bitmap[28] = 0x18;

	return MS::kSuccess;
}

#define rand3a(x,y,z)	frand(67*(x)+59*(y)+71*(z))
#define rand3b(x,y,z)	frand(73*(x)+79*(y)+83*(z))
#define rand3c(x,y,z)	frand(89*(x)+97*(y)+101*(z))
#define rand3d(x,y,z)	frand(103*(x)+107*(y)+109*(z))

int		xlim[3][2];		// integer bound for point
double	xarg[3];		// fractional part

double frand( register int s )   // get random number from seed
{
	s = s << 13^s;
	return(1. - ((s*(s*s*15731+789221)+1376312589)&0x7fffffff)/1073741824.);
}

double hermite( double p0, double p1, double r0, double r1, double t )
{
	register double t2, t3, _3t2, _2t3 ;
	t2 = t * t;
	t3 = t2 * t;
	_3t2 = 3. * t2;
	_2t3 = 2. * t3 ;

	return(p0*(_2t3-_3t2+1) + p1*(-_2t3+_3t2) + r0*(t3-2.*t2+t) + r1*(t3-t2));
}

void interpolate( double f[4], register int i, register int n )
//
//	f[] returned tangent and value *
//	i   location ?
//	n   order
//
{
	double f0[4], f1[4] ;  //results for first and second halves

	if( n == 0 )	// at 0, return lattice value
	{
		f[0] = rand3a( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		f[1] = rand3b( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		f[2] = rand3c( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		f[3] = rand3d( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		return;
	}

	n--;
	interpolate( f0, i, n );		// compute first half
	interpolate( f1, i| 1<<n, n );	// compute second half

	// use linear interpolation for slopes
	//
	f[0] = (1. - xarg[n]) * f0[0] + xarg[n] * f1[0];
	f[1] = (1. - xarg[n]) * f0[1] + xarg[n] * f1[1];
	f[2] = (1. - xarg[n]) * f0[2] + xarg[n] * f1[2];

	// use hermite interpolation for values
	//
	f[3] = hermite( f0[3], f1[3], f0[n], f1[n], xarg[n] );
}

void tornadoField::noiseFunction( double *inNoise, double *out )
//
//	Descriptions:
//		A noise function.
//
{
	xlim[0][0] = (int)floor( inNoise[0] );
	xlim[0][1] = xlim[0][0] + 1;
	xlim[1][0] = (int)floor( inNoise[1] );
	xlim[1][1] = xlim[1][0] + 1;
	xlim[2][0] = (int)floor( inNoise[2] );
	xlim[2][1] = xlim[2][0] + 1;

	xarg[0] = inNoise[0] - xlim[0][0];
	xarg[1] = inNoise[1] - xlim[1][0];
	xarg[2] = inNoise[2] - xlim[2][0];

	interpolate( out, 0, 3 ) ;
}

#define TORUS_PI 3.14159265
#define TORUS_2PI 2*TORUS_PI
#define EDGES 30
#define SEGMENTS 20

//
//	Descriptions:
//		Draw a set of rings to symbolie the field. This does not override default icon, you can do that by implementing the iconBitmap() function
//
void tornadoField::draw( M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView:: DisplayStatus )
{
	 view.beginGL();
	 for (int j = 0; j < SEGMENTS; j++ )
	 {
		glPushMatrix();
		glRotatef( GLfloat(360 * j / SEGMENTS), 0.0, 1.0, 0.0 );
		glTranslatef( 1.5, 0.0, 0.0 );

		 for (int i = 0; i < EDGES; i++ )
		 {
			glBegin(GL_LINE_STRIP);
			float p0 = float( TORUS_2PI * i / EDGES );
			float p1 = float( TORUS_2PI * (i+1) / EDGES );
			glVertex2f( cos(p0), sin(p0) );
			glVertex2f( cos(p1), sin(p1) );
			glEnd();
		 }
		glPopMatrix();
	 }
	 view.endGL ();
}
//
MStatus tornadoField::worldMatrixValue(MDataBlock& block, MMatrix &m)
{
	MStatus status;
#if 0
    MFnDependencyNode fnDNode(thisMObject(), &status); CHECK_MSTATUS(status);
    MPlug pWorldMatrix(fnDNode.findPlug("worldMatrix", true, &status));

    //MObject oMat ;
	//CHECK_MSTATUS(pWorldMatrix.getValue(oMat));

	MFnMatrixData fnMat(pWorldMatrix.asMObject(), &status); CHECK_MSTATUS(status);//(kInvalidParameter): Object is incompatible with this method
	m = fnMat.matrix(&status);                              CHECK_MSTATUS(status);
#else
    MFnDagNode fnDagNode(thisMObject(), &status);
    CHECK_MSTATUS(status);
    MDagPath path2; CHECK_MSTATUS(fnDagNode.getPath(path2));

    MDoubleArray ret;
    CHECK_MSTATUS(MGlobal::executeCommand("getAttr \""+path2.fullPathName()+".worldMatrix\"", ret));
    //COUT(path2.fullPathName()+".worldMatrix=", ret);

    m.matrix[0][0] = ret[0];     m.matrix[0][1] = ret[1];     m.matrix[0][2] = ret[2];      m.matrix[0][3] = ret[3];
    m.matrix[1][0] = ret[4];     m.matrix[1][1] = ret[5];     m.matrix[1][2] = ret[6];      m.matrix[1][3] = ret[7];
    m.matrix[2][0] = ret[8];     m.matrix[2][1] = ret[9];     m.matrix[2][2] = ret[10];     m.matrix[2][3] = ret[11];
    m.matrix[3][0] = ret[12];    m.matrix[3][1] = ret[13];    m.matrix[3][2] = ret[14];     m.matrix[3][3] = ret[15];
#endif
	return MS::kSuccess;
}
