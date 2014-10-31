#include "testuv.h"
#include "objectuv.h"

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
		MString objname( args.asString(0) );

		ObjectUV object(objname);
	} else {
		cerr << "No Object name specified\n";
	}

	return res;
}
