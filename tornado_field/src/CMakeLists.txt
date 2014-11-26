set(HDRS
	../../common/log.h
	tornado_field.h
	particle.h
)

set(SRCS
	../../common/log.cpp
	plugin.cpp
	tornado_field.cpp
	particle.cpp
)

ADD_LIBRARY( ${MY_PROJECT_NAME} 
	SHARED 
		${HDRS} 
		${SRCS} 
)

include_directories( 
	${MAYA_INCLUDE_DIR} 
	"../../"
)

TARGET_LINK_LIBRARIES( ${MY_PROJECT_NAME}
	${MAYA_Foundation_LIBRARY}
	${MAYA_OpenMaya_LIBRARY} 
	${MAYA_OpenMayaAnim_LIBRARY}
	${MAYA_OpenMayaFX_LIBRARY}
	${MAYA_OpenMayaRender_LIBRARY}
	${MAYA_OpenMayaUI_LIBRARY}
	${MAYA_Image_LIBRARY}
	${MAYA_IMFbase_LIBRARY}
)

set(MAYA_EXTENSION ".so")

set(MAYA_COMPILE_DEFINITIONS 
	REQUIRE_IOSTREAM _BOOL LINUX _LINUX LINUX_64)

set(MAYA_COMPILE_FLAGS
	"-m64 -g -pthread -pipe -fPIC -Wno-deprecated -fno-gnu-keywords" )

set(MAYA_LINK_FLAGS
	"-shared -m64 -g -pthread -pipe -fPIC -Wno-deprecated -fno-gnu-keywords -Wl,-Bsymbolic" )

SET_TARGET_PROPERTIES( ${MY_PROJECT_NAME}
	PROPERTIES 
		COMPILE_DEFINITIONS		"${MAYA_COMPILE_DEFINITIONS}"
		COMPILE_FLAGS			"${MAYA_COMPILE_FLAGS}"
		LINK_FLAGS			"${MAYA_LINK_FLAGS}"
		PREFIX				""
		SUFFIX				${MAYA_EXTENSION}

		OUTPUT_NAME			${MY_PROJECT_NAME}
		LIBRARY_OUTPUT_DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR}/../plug-ins
#		RUNTIME_OUTPUT_DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR}/../plug-ins
#		EXECUTABLE_OUTPUT_PATH		${CMAKE_CURRENT_SOURCE_DIR}/../plug-ins
)
