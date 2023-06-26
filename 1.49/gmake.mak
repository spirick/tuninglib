
# Spirick Tuning
#
# A C++ class and template library
# for performance critical applications.
# Copyright (C) 1996-2023 Dietmar Deimling.
# All rights reserved.
# Internet  www.spirick.com
# E-Mail    info@spirick.com
#
# Version 1.49
# File gmake.mak

# Use gmake for this makefile
# Standard UNIX make reports an error in the next line
DUMMY += DUMMY

.PHONY: all depend clean

# 'all' is always the first target
all::

#---------------------------------------------------------------
# Global checks

ifndef TL_COMPILER
all depend clean::
	ERROR_UNKNOWN_COMPILER
endif

ifndef PROJECTDIR
all depend clean::
	ERROR_UNKNOWN_PROJECTDIR
endif

ifndef SUBDIR
all depend clean::
	ERROR_UNKNOWN_SUBDIR
endif

#---------------------------------------------------------------
# System wide variables

S     = /
_OBJ  = .o
_LIB  = .a
_EXE  = 

DEL   = rm -f 
MKDIR = mkdir 
RMDIR = rm -rf 

clean::
	-$(DEL) core
	-$(DEL) *~

#---------------------------------------------------------------
# Automatic selection of compiler
# If it doesn't work, comment out some lines

ifeq ($(findstring linux,$(OSTYPE)),linux)
 MAKE = make --no-print-directory -r -R -S
 MAKE_OS = LINUX
 include $(PROJECTDIR)/gmake_linux.mak
endif

ifndef MAKE_OS
all depend clean::
	ERROR_UNKNOWN_OS_TYPE
endif

#---------------------------------------------------------------
# Other includes

include $(PROJECTDIR)/project.mak

ifndef PROJECT_TARGETDIR
all depend clean::
	ERROR_UNKNOWN_TARGETDIR
endif

include $(PROJECTDIR)/common.mak

#---------------------------------------------------------------
# Subdirectories

ifdef SUBDIRS
 include $(PROJECTDIR)/subdirs.mak
endif

#---------------------------------------------------------------
# User objects

ifdef USER_OBJS
 include $(PROJECTDIR)/userobjs.mak
endif

#---------------------------------------------------------------
# User library

ifdef USER_LIB
 include $(PROJECTDIR)/userlib.mak
endif

#---------------------------------------------------------------
# User console program

ifdef USER_CON_PROG
 include $(PROJECTDIR)/userconprog.mak
endif

#---------------------------------------------------------------
# User GUI program

ifdef USER_GUI_PROG
 include $(PROJECTDIR)/userguiprog.mak
endif

#---------------------------------------------------------------

sinclude $(DEPFILE)

#---------------------------------------------------------------
# Compile

.SUFFIXES: .c .C .cpp $(_OBJ)

$(TSDIR_)%$(_OBJ): %.c
	$(ECHO1)
	$(COMPILE)
	$(SD) $<

$(TSDIR_)%$(_OBJ): %.C
	$(ECHO1)
	$(COMPILE)
	$(SD) $<

$(TSDIR_)%$(_OBJ): %.cpp
	$(ECHO1)
	$(COMPILE)
	$(SD) $<
