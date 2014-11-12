
cmake_minimum_required(VERSION 2.6)

set(MY_PROJECT_NAME dynamic_enum)

project(${MY_PROJECT_NAME})



if(UNIX)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/Modules/")

	  set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/Modules/")

	else ()
	  message(SEND_ERROR "Failed to find module path: ${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/Modules/")
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

#
add_subdirectory(src)


