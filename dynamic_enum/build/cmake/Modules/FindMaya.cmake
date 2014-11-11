

#=============================================================================

# Get path, convert backslashes as ${ENV_${var}}
IF(UNIX)
  FIND_PATH(MAYA_BASE_DIR include/maya/MFn.h
    PATH
      ENV MAYA_LOCATION
  )
ENDIF(UNIX)



FIND_PATH(MAYA_INCLUDE_DIR maya/MFn.h
  PATHS
    ENV MAYA_LOCATION
    ${MAYA_BASE_DIR}
  PATH_SUFFIXES
    ../../devkit/include/
    include/
  DOC "Maya's devkit headers path"
)
LIST(APPEND MAYA_INCLUDE_DIRS ${MAYA_INCLUDE_DIR})



FIND_PATH(MAYA_LIBRARY_DIR libOpenMaya.so
    PATHS
      ENV MAYA_LOCATION
      ${MAYA_BASE_DIR}
    PATH_SUFFIXES
      lib/
    DOC "Maya's libraries path"
)



#=============================================================================
#MESSAGE(STATUS "-MAYA_INCLUDE_DIR= " ${MAYA_INCLUDE_DIR})


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

#MESSAGE(STATUS "-MAYA_LIBRARIES= " ${MAYA_LIBRARIES})

