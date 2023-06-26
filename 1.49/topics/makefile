
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
# File topics/makefile

PROJECTDIR  = ..
SUBDIR      = topics

SAMPLE_OBJS = \
 $(TSDIR_)01_vector1$(_OBJ) \
 $(TSDIR_)01_vector2$(_OBJ) \
 $(TSDIR_)01_vector3$(_OBJ) \
 $(TSDIR_)02_vector$(_OBJ) \
 $(TSDIR_)03_vector$(_OBJ) \
 $(TSDIR_)04_vector$(_OBJ)

SAMPLE_PROGS = \
 $(BINDIR_)01_vector1$(_EXE) \
 $(BINDIR_)01_vector2$(_EXE) \
 $(BINDIR_)01_vector3$(_EXE) \
 $(BINDIR_)02_vector$(_EXE) \
 $(BINDIR_)03_vector$(_EXE) \
 $(BINDIR_)04_vector$(_EXE)

include ../system.mak

$(BINDIR_)01_vector1$(_EXE): $(TSDIR_)01_vector1$(_OBJ)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)01_vector2$(_EXE): $(TSDIR_)01_vector2$(_OBJ)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)01_vector3$(_EXE): $(TSDIR_)01_vector3$(_OBJ)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)02_vector$(_EXE): $(TSDIR_)02_vector$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)03_vector$(_EXE): $(TSDIR_)03_vector$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)04_vector$(_EXE): $(TSDIR_)04_vector$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

all:: $(SAMPLE_PROGS)

depend::
	$(SD) $(SAMPLE_OBJS)

clean::
	-$(DEL) $(SAMPLE_OBJS)
	-$(DEL) $(SAMPLE_PROGS)
