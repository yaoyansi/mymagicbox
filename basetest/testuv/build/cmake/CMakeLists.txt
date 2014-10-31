
cmake_minimum_required(VERSION 2.6)

set(MY_PROJECT_NAME testuv)

project(${MY_PROJECT_NAME})



if(UNIX)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Modules/")

	  set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules/")

	else ()
	  message(SEND_ERROR "Failed to find module path: ${CMAKE_CURRENT_SOURCE_DIR}/Modules/")
	endif()
endif(UNIX)
 
if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "Debug" 
		CACHE STRING 
		"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." 
		FORCE
	)
endif ()

#set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")
 
find_package(Maya REQUIRED)
 
#if(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
#  message(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
#endif()

# Find Boost
 
set(HDRS
	../../src/testuv.h
	../../src/objectuv.h
	../../src/peltOverlapCmd.h
)

set(SRCS
	../../src/testuv.cpp
	../../src/objectuv.cpp
	../../src/peltOverlapCmd.cpp
	../../src/plugin.cpp
)

include_directories( 
	${MAYA_INCLUDE_DIR} 
)

ADD_LIBRARY( ${MY_PROJECT_NAME} 
	SHARED 
		${HDRS} 
		${SRCS} 
)



TARGET_LINK_LIBRARIES( ${MY_PROJECT_NAME}
	${MAYA_Foundation_LIBRARY} 
	${MAYA_OpenMaya_LIBRARY} 
	${MAYA_OpenMayaUI_LIBRARY}
	${MAYA_OpenMayaAnim_LIBRARY}
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
		LIBRARY_OUTPUT_DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR}/../../plug-ins
#		RUNTIME_OUTPUT_DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR}/../../plug-ins
#		EXECUTABLE_OUTPUT_PATH		${CMAKE_CURRENT_SOURCE_DIR}/../../plug-ins
)
