#pragma once

#include <maya/MObject.h>
#include <maya/MString.h>

// this class holds the version data, and can be integreated into another node
class VersionHelper
{
public:
    VersionHelper();
    ~VersionHelper();

    MStatus	initialize();
	void	postConstructor(const MObject& mobjec);

    // the attribute which holds the version data
    static MObject aVersion;

protected:
    const MString m_long_name;
    const MString m_short_name;

protected:
    VersionHelper(const VersionHelper& o);
    VersionHelper& operator=(const VersionHelper& o);
};
