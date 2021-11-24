
# Spirick Tuning
#
# The C++ class and template library
# for performance critical applications.
# Copyright (C) 1996-2021 Dietmar Deimling.
# All rights reserved.
# Internet  www.spirick.com
# E-Mail    info@spirick.com
#
# Version 1.48
# Date 11-20-2021
# File sourcedep/makefile

PROJECTDIR         = ..
SUBDIR             = sourcedep

USER_CON_PROG      = $(BINDIR_)sd$(_EXE)

USER_CON_PROG_OBJS = \
 $(TSDIR_)filesem$(_OBJ) \
 $(TSDIR_)parser$(_OBJ) \
 $(TSDIR_)scanner$(_OBJ) \
 $(TSDIR_)dirmap$(_OBJ) \
 $(TSDIR_)dirmaps$(_OBJ) \
 $(TSDIR_)filemap$(_OBJ) \
 $(TSDIR_)filemaps$(_OBJ) \
 $(TSDIR_)project$(_OBJ) \
 $(TSDIR_)sd$(_OBJ)

USER_CON_PROG_LIBS = $(TUNINGLIB)

include ../system.mak
