
#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>

class TestUVCmd : public MPxCommand
{
public:
	TestUVCmd();
	~TestUVCmd();

	virtual MStatus doIt(const MArgList& args);

	static void*	creator();

	static MString cCmdName();
};

