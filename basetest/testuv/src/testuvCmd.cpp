#include "testuvCmd.h"
#include "objectuv.h"

TestUVCmd::TestUVCmd()
{
}
//
TestUVCmd::~TestUVCmd()
{
}
//
void* TestUVCmd::creator()
{
	return new TestUVCmd();
}
//
MString TestUVCmd::cCmdName()
{
    return "testuvCmd";
}
//
MStatus TestUVCmd::doIt( const MArgList& args )
{
	MStatus res = MS::kSuccess;

	unsigned len = args.length();
	if ( len > 0 ) {
		MString objname( args.asString(0) );

		ObjectUV object(objname);
		object.testUV();
	} else {
		std::cerr << "No Object name specified\n";
	}

	return res;
}
