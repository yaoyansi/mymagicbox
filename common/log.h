#pragma once

#include <unistd.h>
#include <stdio.h>

#include <maya/MObject.h>

#ifdef _WIN32
#define _LOG_Prefix(level)      printf("\nmymagicbox|%6x|%20s| %s|: ", GetCurrentProcessId(), __FUNCTION__, level);
#else
#define _LOG_Prefix(level)      printf("\nmymagicbox|%6x|%20s| %s|: ", getpid(), __FUNCTION__, level);
#endif

#define LFat           _LOG_Prefix("Fat"); printf
#define LErr           _LOG_Prefix("Err"); printf
#define LWrn           _LOG_Prefix("Wrn"); printf
#define LInf           _LOG_Prefix("Inf"); printf
#define LDbg           _LOG_Prefix("Dbg"); printf

// define osLFAT for the classes which implement the operator<<(os, ...),
// otherwise, use LFAT instead.
#define osLFAT(msg, v) _LOG_Prefix("Fat"); std::cout<< msg; std::cout<< v;
#define osLErr(msg, v) _LOG_Prefix("Err"); std::cout<< msg; std::cout<< v;
#define osLWrn(msg, v) _LOG_Prefix("Wrn"); std::cout<< msg; std::cout<< v;
#define osLInf(msg, v) _LOG_Prefix("Inf"); std::cout<< msg; std::cout<< v;
#define osLDbg(msg, v) _LOG_Prefix("Dbg"); std::cout<< msg; std::cout<< v;


//
void printMObjectInfo(const char* msg, MObject mobj);
void printMeshInfo(const char* msg, MObject mobj);
