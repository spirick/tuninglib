
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
# File common.mak

#---------------------------------------------------------------
# Directories

$(PROJECT_TARGETDIR):
	$(MKDIR) $(PROJECT_TARGETDIR)

all depend:: $(PROJECT_TARGETDIR)

#--------------------------------

TARGETDIR = $(PROJECT_TARGETDIR)$S$(COMPILERDIR)
LIBDIR    = $(TARGETDIR)$Slib
LIBDIR_   = $(LIBDIR)$S
BINDIR    = $(TARGETDIR)$Sbin
BINDIR_   = $(BINDIR)$S

$(TARGETDIR):
	$(MKDIR) $(TARGETDIR)
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(BINDIR)

all depend:: $(TARGETDIR)

#--------------------------------

TSDIR  = $(TARGETDIR)$S$(SUBDIR)
TSDIR_ = $(TSDIR)$S

$(TSDIR):
	$(MKDIR) $(TSDIR)

all depend:: $(TSDIR)

#---------------------------------------------------------------
# Options & includes

OPTIONS   = $(SYSTEM_OPTS) $(PROJECT_OPTS) $(USER_OPTS)
INCLUDES  = $(SYSTEM_INCS) $(PROJECT_INCS) $(USER_INCS)

#---------------------------------------------------------------
# Dependencies

DEPFILE_  = $(TSDIR_)depend.mak
DEPFILE   = $(DEPFILE_)
SD        = sd -nologo -file $(DEPFILE) $(INCLUDES) -prefix $(TSDIR_)

#---------------------------------------------------------------
# Output delimiter

ECHO1 = @echo ----------------------------------------------------
ECHO2 = @echo ++++++++++++++++++++++++++++++++++++++++++++++++++++

#---------------------------------------------------------------
# Common depend & clean

depend::
	-$(DEL) $(DEPFILE)
