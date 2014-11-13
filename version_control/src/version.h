#pragma once

// Version of this project:
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define VERSION_CODE (10000 * VERSION_MAJOR + \
                        100 * VERSION_MINOR + \
                              VERSION_PATCH)

// Magic macros to make VERSION_STRING that looks like "1.2.3"
#define MAKE_VERSION_STRING2(a,b,c) #a "." #b "." #c
#define MAKE_VERSION_STRING(a,b,c) MAKE_VERSION_STRING2(a,b,c)

#define VERSION_STRING MAKE_VERSION_STRING(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH)
