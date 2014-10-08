#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "mystring.h"

extern int main();

MStatus initializePlugin( MObject obj )
{

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any" );

    main();

	return( MS::kSuccess );
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	return MS::kSuccess;
}
