
#include <maya/MIOStream.h>
#include <maya/MVector.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MMatrix.h>
#include <maya/MPxFieldNode.h>

#if defined(OSMac_MachO_)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define McheckErr(stat, msg)		\
	if ( MS::kSuccess != stat )		\
	{								\
		cerr << msg;				\
		return MS::kFailure;		\
	}

class tornadoField: public MPxFieldNode
{
public:
	tornadoField();
	virtual ~tornadoField();
    virtual void postConstructor();

	static void		*creator();
	static MStatus	initialize();

	static MString cTypeName();
	static MTypeId cTypeId();
	static MPxNode::Type cType();
	static const MString& cClassification();
	// will compute output force.
	//
	virtual MStatus	compute( const MPlug& plug, MDataBlock& block );

	virtual void 	draw (  M3dView  & view, const  MDagPath  & path,  M3dView::DisplayStyle  style, M3dView:: DisplayStatus );

    ///
    virtual MStatus getForceAtPoint(const MVectorArray& point,
                            const MVectorArray& velocity,
                            const MDoubleArray& mass,
                            MVectorArray& force,
                            double deltaTime);

	///
	virtual MStatus iconSizeAndOrigin(	GLuint& width,
						GLuint& height,
						GLuint& xbo,
						GLuint& ybo   );
	///
	virtual MStatus iconBitmap(GLubyte* bitmap);

	//
	// attributes.
	//

	// minimum distance from field at which repel is applied
	//
	static MObject	aMinDistance;

	// min distance from field at which the force attracts
	//
	static MObject	aAttractDistance;

	// max distance from field at which the force repels.
	//
	static MObject	aRepelDistance;

	// drag exerted on the attractRepel force.
	//
	static MObject	aDrag;

	// amplitude/magnitude of the swarm force.
	//
	static MObject	aSwarmAmplitude;

	// frequency of the swarm force.
	//
	static MObject	aSwarmFrequency;

	// phase of the swarm force.
	//
	static MObject	aSwarmPhase;

	// Other data members
	//
	static MTypeId	m_id;
	static MString  m_classification;
private:

	// methods to compute output force.
	//
	void	addCentripetalForce( MDataBlock& block,
							const MVectorArray &points,
							const MVectorArray &velocities,
							const MDoubleArray &masses,
							MVectorArray &outputForce );

	void	ownerPosition( MDataBlock& block, MVectorArray &vArray );
	MStatus	getWorldPosition( MVector &vector );
	MStatus	getWorldPosition( MDataBlock& block, MVector &vector );
	void	noiseFunction( double *inputNoise, double *out );

	// methods to get attribute value.
	//
	double	magnitudeValue( MDataBlock& block );
	double	attenuationValue( MDataBlock& block );
	double	maxDistanceValue( MDataBlock& block );
	bool	useMaxDistanceValue( MDataBlock& block );
	bool	applyPerVertexValue( MDataBlock& block );

	// methods to get attribute value of local attributes.
	//
	double	minDistanceValue( MDataBlock& block );
	double	attractDistanceValue( MDataBlock& block );
	double	repelDistanceValue( MDataBlock& block );
	double	dragValue( MDataBlock& block );
	double	swarmAmplitudeValue( MDataBlock& block );
	double	swarmFrequencyValue( MDataBlock& block );
	double	swarmPhaseValue( MDataBlock& block );

	MStatus	ownerCentroidValue( MDataBlock& block, MVector &vector );
	MStatus worldMatrixValue(MDataBlock& block, MMatrix &matrix);

	MStatus _getForce(
        MDataBlock& block,
        const MVectorArray& point,
        const MVectorArray& velocity,
        const MDoubleArray& mass,
        MVectorArray& force,
        double deltaTime
    );
};

// inlines
//
inline double tornadoField::magnitudeValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mMagnitude, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::attenuationValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mAttenuation, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::maxDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mMaxDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline bool tornadoField::useMaxDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mUseMaxDistance, &status );

	bool value = false;
	if( status == MS::kSuccess )
		value = hValue.asBool();

	return( value );
}

inline bool tornadoField::applyPerVertexValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mApplyPerVertex, &status );

	bool value = false;
	if( status == MS::kSuccess )
		value = hValue.asBool();

	return( value );
}

inline double tornadoField::minDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aMinDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::attractDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aAttractDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::repelDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aRepelDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::dragValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aDrag, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::swarmAmplitudeValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aSwarmAmplitude, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::swarmFrequencyValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aSwarmFrequency, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double tornadoField::swarmPhaseValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aSwarmPhase, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline MStatus tornadoField::ownerCentroidValue(MDataBlock& block, MVector &vector)
{
	MStatus status;

	MDataHandle hValueX = block.inputValue( mOwnerCentroidX, &status );
	MDataHandle hValueY = block.inputValue( mOwnerCentroidY, &status );
	MDataHandle hValueZ = block.inputValue( mOwnerCentroidZ, &status );

	if( status == MS::kSuccess )
	{
		vector[0] = hValueX.asDouble();
		vector[1] = hValueY.asDouble();
		vector[2] = hValueZ.asDouble();
	}

	return( status );
}


