
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
# File project.mak

PROJECT_TARGETDIR = $(PROJECTDIR)$Starget

#---------------------------------------------------------------

PROJECT_OPTS = $(EXCEPT_ON) $(RTTI_ON) $(THREAD_ON) $(SHORT_WCHAR)
PROJECT_INCS = -I$(PROJECTDIR)

#---------------------------------------------------------------

TUNINGLIB    = $(LIBDIR_)tuning$(_LIB)
PROJECTLIBS  = $(TUNINGLIB)
