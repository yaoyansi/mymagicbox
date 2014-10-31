#include "testuv.h"

TestUVCmd::~TestUVCmd() {}

void* TestUVCmd::creator()
{
	return new TestUVCmd();
}

MString TestUVCmd::cCmdName()
{
    return "testuv";
}
MStatus TestUVCmd::doIt( const MArgList& args )
{
	MStatus res = MS::kSuccess;

	unsigned len = args.length();
	if ( len > 0 ) {
		MString object_name( args.asString(0) );

		if ( MS::kSuccess != MGlobal::selectByName( object_name ) )
			cerr << "Object " << object_name.asChar() << " not found\n";
	} else {
		cerr << "No Object name specified\n";
	}

	return res;
}
