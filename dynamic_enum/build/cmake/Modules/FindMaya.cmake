# - Maya finder module
# This module searches for a valid Maya instalation. 
# It searches for Maya's devkit, libraries, executables
# and related paths (scripts)
# 
# Variables that will be defined: 
# MAYA_FOUND          Defined if a Maya installation has been detected
# MAYA_EXECUTABLE     Path to Maya's executable
# MAYA_<lib>_FOUND    Defined if <lib> has been found
# MAYA_<lib>_LIBRARY  Path to <lib> library
# MAYA_INCLUDE_DIRS   Path to the devkit's include directories
# 
# IMPORTANT: Currently, there's only support for OSX platform and Maya version 2012. 

#=============================================================================
# Copyright 2011-2012 Francisco Requena <frarees@gmail.com>
# 
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
# 
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

SET(MAYA_VERSION_2012 TRUE)

## add one to this list to match your install if none match

IF(APPLE)
  FIND_PATH(MAYA_BASE_DIR ../../devkit/include/maya/MFn.h PATH
    ENV MAYA_LOCATION
    "/Applications/Autodesk/maya2013/Maya.app/Contents"
    "/Applications/Autodesk/maya2012.17/Maya.app/Contents"
    "/Applications/Autodesk/maya2012/Maya.app/Contents"
    "/Applications/Autodesk/maya2011/Maya.app/Contents"
    "/Applications/Autodesk/maya2010/Maya.app/Contents"
  )
  FIND_PATH(MAYA_LIBRARY_DIR libOpenMaya.dylib
    PATHS
      ENV MAYA_LOCATION
      ${MAYA_BASE_DIR}
    PATH_SUFFIXES
      Maya.app/Contents/MacOS/
    DOC "Maya's libraries path"
  )
ENDIF(APPLE)

IF(UNIX)
  FIND_PATH(MAYA_BASE_DIR include/maya/MFn.h
    PATH
      ENV MAYA_LOCATION
      "/usr/autodesk/maya2012.17-x64"
      "/usr/autodesk/maya2012-x64"
      "/usr/autodesk/maya2011-x64"
      "/usr/autodesk/maya2010-x64"
  )
  FIND_PATH(MAYA_LIBRARY_DIR libOpenMaya.so
    PATHS
      ENV MAYA_LOCATION
      ${MAYA_BASE_DIR}
    PATH_SUFFIXES
      lib/
    DOC "Maya's libraries path"
  )
ENDIF(UNIX)
 
IF(WIN32)
  FIND_PATH(MAYA_BASE_DIR include/maya/MFn.h
    PATH
      ENV MAYA_LOCATION
        "C:/Program Files/Autodesk/Maya2012-x64"
        "C:/Program Files/Autodesk/Maya2012"
        "C:/Program Files (x86)/Autodesk/Maya2012"
        "C:/Autodesk/maya-2012x64"
        "C:/Program Files/Autodesk/Maya2011-x64"
        "C:/Program Files/Autodesk/Maya2011"
        "C:/Program Files (x86)/Autodesk/Maya2011"
        "C:/Autodesk/maya-2011x64"
        "C:/Program Files/Autodesk/Maya2010-x64"
        "C:/Program Files/Autodesk/Maya2010"
        "C:/Program Files (x86)/Autodesk/Maya2010"
        "C:/Autodesk/maya-2010x64"
  )
  FIND_PATH(MAYA_LIBRARY_DIR OpenMaya.lib
    PATHS
      ENV MAYA_LOCATION
      ${MAYA_BASE_DIR}
    PATH_SUFFIXES
      lib/
    DOC "Maya's libraries path"
  )
ENDIF(WIN32)

FIND_PATH(MAYA_INCLUDE_DIR maya/MFn.h
  PATHS
    ENV MAYA_LOCATION
    ${MAYA_BASE_DIR}
  PATH_SUFFIXES
    ../../devkit/include/
    include/
  DOC "Maya's devkit headers path"
)

FIND_PATH(MAYA_LIBRARY_DIR OpenMaya
  PATHS
    ENV MAYA_LOCATION
    ${MAYA_BASE_DIR}
  PATH_SUFFIXES
    ../../devkit/include/
    include/
  DOC "Maya's devkit headers path"
)

LIST(APPEND MAYA_INCLUDE_DIRS ${MAYA_INCLUDE_DIR})

FIND_PATH(MAYA_DEVKIT_INC_DIR GL/glext.h
  PATHS
    ENV MAYA_LOCATION
    ${MAYA_BASE_DIR}
  PATH_SUFFIXES
    ../../devkit/plug-ins/
    /devkit/plug-ins/
  DOC "Maya's devkit headers path"
)
LIST(APPEND MAYA_INCLUDE_DIRS ${MAYA_DEVKIT_INC_DIR})

FOREACH(MAYA_LIB
  OpenMaya
  OpenMayaAnim
  OpenMayaFX
  OpenMayaRender
  OpenMayaUI  
  Image
  Foundation
  IMFbase
  tbb
  cg
  cgGL
)
  FIND_LIBRARY(MAYA_${MAYA_LIB}_LIBRARY ${MAYA_LIB}
    PATHS
      ENV MAYA_LOCATION
      ${MAYA_BASE_DIR}
    PATH_SUFFIXES
      MacOS/
      lib/
    DOC "Maya's ${MAYA_LIB} library path"
  )
  
  LIST(APPEND ${MAYA_LIBRARIES} MAYA_${MAYA_LIB}_LIBRARY)
ENDFOREACH(MAYA_LIB)

FIND_PROGRAM(MAYA_EXECUTABLE Maya
  PATHS
    ENV MAYA_LOCATION
    ${MAYA_BASE_DIR}
  PATH_SUFFIXES
    MacOS/
    bin/
  DOC "Maya's executable path"
)

# handle the QUIETLY and REQUIRED arguments and set MAYA_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Maya DEFAULT_MSG ${MAYA_LIBRARIES} MAYA_EXECUTABLE MAYA_INCLUDE_DIRS)

IF(NOT MAYA_FOUND AND NOT MAYA_FIND_QUIETLY)
  MESSAGE("Remember that currently there's support only for Maya 2013 on OS X. ")
ENDIF()

#=============================================================================
# Macros
#=============================================================================

# Macro for setting up typical plugin properties. These include:
#  - OS-specific plugin suffix (.mll, .so, .bundle)
#  - Removal of 'lib' prefix on osx/linux
#  - OS-specific defines
#  - Post-commnad for correcting Qt library linking on osx
#  - Windows link flags for exporting initializePlugin/uninitializePlugin
macro(MAYA_SET_PLUGIN_PROPERTIES target)

    if(APPLE)
        set(MAYA_EXTENSION ".bundle")
        set(MAYA_COMPILE_DEFINITIONS AW_NEW_IOSTREAMS REQUIRE_IOSTREAM _BOOL _DARWIN MAC_PLUGIN
            OSMac_ OSMac_MachO OSMacOSX_ CC_GNU_ _LANGUAGE_C_PLUS_PLUS)
        set(MAYA_COMPILE_FLAGS
          "-fno-gnu-keywords -include ${MAYA_INCLUDE_PATH}/maya/OpenMayaMac.h" )

        set(MAYA_LINK_FLAGS
          #"-dynamic -g -fPIC "
          #"-shared -g -fPIC "
          "-fno-gnu-keywords -framework System  -framework SystemConfiguration -framework CoreServices -framework Carbon -framework Cocoa -framework ApplicationServices -framework Quicktime -framework IOKit -bundle -fPIC -L${ALEMBIC_MAYA_LIB_ROOT} -Wl,-executable_path,${ALEMBIC_MAYA_LIB_ROOT}" )

        if(QT_LIBRARIES)
            set(_changes "")
            foreach(_lib ${QT_LIBRARIES})
                if("${_lib}" MATCHES ".*framework.*")
                    get_filename_component(_shortname ${_lib} NAME)
                    string(REPLACE ".framework" "" _shortname ${_shortname})
                    # FIXME: QT_LIBRARIES does not provide the entire path to the lib.
                    #  it provides /usr/local/qt/4.7.2/lib/QtGui.framework
                    #  but we need /usr/local/qt/4.7.2/lib/QtGui.framework/Versions/4/QtGui
                    # below is a hack, likely to break on other configurations
                    set(_changes ${_changes} "-change" "${_lib}/Versions/4/${_shortname}" "@executable_path/${_shortname}")
                endif()
            endforeach()

            add_custom_command(TARGET ${target}
                POST_BUILD
                COMMAND install_name_tool ${_changes} $<TARGET_FILE:${target}>)
        endif()

    elseif(WIN32)
        set(MAYA_EXTENSION ".mll")
        set(MAYA_COMPILE_DEFINITIONS REQUIRE_IOSTREAM _BOOL _AFXDLL _MBCS NT_PLUGIN)
        set(MAYA_LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin")
        set( MAYA_COMPILE_FLAGS "/MD")
    else()
        set(MAYA_EXTENSION ".so")
        set(MAYA_COMPILE_DEFINITIONS REQUIRE_IOSTREAM _BOOL LINUX _LINUX LINUX_64)
        set(MAYA_COMPILE_FLAGS
          "-m64 -g -pthread -pipe -fPIC -Wno-deprecated -fno-gnu-keywords" )

        set(MAYA_LINK_FLAGS
          "-shared -m64 -g -pthread -pipe -fPIC -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic" )
    endif()

    set_target_properties( ${target} PROPERTIES
        COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS}"
        COMPILE_FLAGS "${MAYA_COMPILE_FLAGS}"
        LINK_FLAGS "${MAYA_LINK_FLAGS}"
        PREFIX ""
        SUFFIX ${MAYA_EXTENSION}
    )

endmacro(MAYA_SET_PLUGIN_PROPERTIES)
