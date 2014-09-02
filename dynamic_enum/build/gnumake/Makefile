#-
# ==========================================================================
# Copyright (c) 2011 Autodesk, Inc.
# All rights reserved.
# 
# These coded instructions, statements, and computer programs contain
# unpublished proprietary information written by Autodesk, Inc., and are
# protected by Federal copyright law. They may not be disclosed to third
# parties or copied or duplicated in any form, in whole or in part, without
# the prior written consent of Autodesk, Inc.
# ==========================================================================
#+

ifndef INCL_BUILDRULES

#
# Include platform specific build settings
#
PROJECT_DIR  := ../..
MAKEFILE_DIR := .
include $(MAKEFILE_DIR)/buildrules


#
# Always build the local plug-in when make is invoked from the
# directory.
#
all : plugins

endif

#
# Variable definitions
#

SRCDIR := $(PROJECT_DIR)/src
DSTDIR := $(PROJECT_DIR)/plug-ins
OBJDIR := $(PROJECT_DIR)/src

PROJECT_SOURCES  := $(SRCDIR)/dynamic_enum.cpp $(SRCDIR)/plugin.cpp
PROJECT_OBJECTS  := $(OBJDIR)/dynamic_enum.o   $(OBJDIR)/plugin.o
PROJECT_PLUGIN   := $(DSTDIR)/dynamic_enum.$(EXT)
PROJECT_MAKEFILE := $(SRCDIR)/Makefile

#
# Include the optional per-plugin Makefile.inc
#
#    The file can contain macro definitions such as:
#       {pluginName}_EXTRA_CFLAGS
#       {pluginName}_EXTRA_C++FLAGS
#       {pluginName}_EXTRA_INCLUDES
#       {pluginName}_EXTRA_LIBS
-include $(SRCDIR)/Makefile.inc


#
# Set target specific flags.
#

$(PROJECT_OBJECTS): CFLAGS   := $(CFLAGS)   $(PROJECT_EXTRA_CFLAGS)
$(PROJECT_OBJECTS): C++FLAGS := $(C++FLAGS) $(PROJECT_EXTRA_C++FLAGS)
$(PROJECT_OBJECTS): INCLUDES := $(INCLUDES) $(PROJECT_EXTRA_INCLUDES)

depend_PROJECT:     INCLUDES := $(INCLUDES) $(PROJECT_EXTRA_INCLUDES)

$(PROJECT_PLUGIN):  LFLAGS   := $(LFLAGS) $(PROJECT_EXTRA_LFLAGS) 
$(PROJECT_PLUGIN):  LIBS     := $(LIBS)   -lOpenMaya -lOpenMayaRender -lFoundation $(PROJECT_EXTRA_LIBS) 

#
# Rules definitions
#

.PHONY: depend_PROJECT clean_PROJECT Clean_PROJECT


$(PROJECT_PLUGIN): $(PROJECT_OBJECTS) 
	-rm -f $@
	$(LD) -o $@ $(LFLAGS) $^ $(LIBS)

depend_PROJECT :
	makedepend $(INCLUDES) $(MDFLAGS) -f$(DSTDIR)/Makefile $(PROJECT_SOURCES)

clean_PROJECT:
	-rm -f $(PROJECT_OBJECTS)

Clean_PROJECT:
	-rm -f $(PROJECT_MAKEFILE).bak $(PROJECT_OBJECTS) $(PROJECT_PLUGIN)


plugins: $(PROJECT_PLUGIN)
depend:	 depend_PROJECT
clean:	 clean_PROJECT
Clean:	 Clean_PROJECT

