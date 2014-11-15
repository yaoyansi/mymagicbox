#pragma once

// Version of this project:
#define PLUGIN_VERSION_MAJOR 0
#define PLUGIN_VERSION_MINOR 0
#define PLUGIN_VERSION_PATCH 5

#define PLUGIN_VERSION_CODE (10000 * PLUGIN_VERSION_MAJOR + \
                               100 * PLUGIN_VERSION_MINOR + \
                                     PLUGIN_VERSION_PATCH)

// Magic macros to make VERSION_STRING that looks like "1.2.3"
#define MAKE_VERSION_STRING2(a,b,c) #a "." #b "." #c
#define MAKE_VERSION_STRING(a,b,c) MAKE_VERSION_STRING2(a,b,c)

#define PLUGIN_VERSION_STRING \
        MAKE_VERSION_STRING(PLUGIN_VERSION_MAJOR, \
                            PLUGIN_VERSION_MINOR, \
                            PLUGIN_VERSION_PATCH)
