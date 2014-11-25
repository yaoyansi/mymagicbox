#pragma once

#include <unistd.h>
#include <stdio.h>

#include <maya/MObject.h>

#ifdef _WIN32
#define LOG          printf("\nmymagicbox|%8x|%32s|: ", GetCurrentProcessId(), __FUNCTION__); printf
#define COUT(msg, v) printf("\nmymagicbox|%8x|%32s|: ", GetCurrentProcessId(), __FUNCTION__); std::cout<< msg; std::cout<< v;
#else
#define LOG          printf("\nmymagicbox|%8x|%32s|: ", getpid(),              __FUNCTION__); printf
#define COUT(msg, v) printf("\nmymagicbox|%8x|%32s|: ", getpid(),              __FUNCTION__); std::cout<< msg; std::cout<< v;
#endif


void printMObjectInfo(const char* msg, MObject mobj);
void printMeshInfo(const char* msg, MObject mobj);
