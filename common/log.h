#pragma once

#include <unistd.h>
#include <stdio.h>

#ifdef _WIN32
#define LOG printf("\nmymagicbox|%8x|%32s|: ", GetCurrentProcessId(), __FUNCTION__); printf
#else
#define LOG printf("\nmymagicbox|%8x|%32s|: ", getpid(),              __FUNCTION__); printf
#endif
