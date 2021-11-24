
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
# File samples/makefile

PROJECTDIR  = ..
SUBDIR      = samples

SAMPLE_OBJS = \
 $(TSDIR_)exechelper$(_OBJ) \
 $(TSDIR_)exechelpermain$(_OBJ) \
 $(TSDIR_)int$(_OBJ) \
 $(TSDIR_)taccess$(_OBJ) \
 $(TSDIR_)talign$(_OBJ) \
 $(TSDIR_)talloc$(_OBJ) \
 $(TSDIR_)tblock$(_OBJ) \
 $(TSDIR_)tblockstore$(_OBJ) \
 $(TSDIR_)tchain$(_OBJ) \
 $(TSDIR_)tcollection$(_OBJ) \
 $(TSDIR_)tcontainer$(_OBJ) \
 $(TSDIR_)tcurrdir$(_OBJ) \
 $(TSDIR_)tdir$(_OBJ) \
 $(TSDIR_)tdirscan$(_OBJ) \
 $(TSDIR_)texception$(_OBJ) \
 $(TSDIR_)texec$(_OBJ) \
 $(TSDIR_)texechelper$(_OBJ) \
 $(TSDIR_)tfile$(_OBJ) \
 $(TSDIR_)tfilename$(_OBJ) \
 $(TSDIR_)tinfo$(_OBJ) \
 $(TSDIR_)tinterlocked$(_OBJ) \
 $(TSDIR_)tmap$(_OBJ) \
 $(TSDIR_)tmd5$(_OBJ) \
 $(TSDIR_)tptrmap$(_OBJ) \
 $(TSDIR_)tsemaphore$(_OBJ) \
 $(TSDIR_)tshared$(_OBJ) \
 $(TSDIR_)tsort$(_OBJ) \
 $(TSDIR_)tstore$(_OBJ) \
 $(TSDIR_)tstring$(_OBJ) \
 $(TSDIR_)tthread$(_OBJ) \
 $(TSDIR_)ttimedate$(_OBJ) \
 $(TSDIR_)ttree$(_OBJ) \
 $(TSDIR_)tutf$(_OBJ) \
 $(TSDIR_)tuuid$(_OBJ) \
 $(TSDIR_)tall$(_OBJ)

SAMPLE_PROGS = \
 $(BINDIR_)exechelpermain$(_EXE) \
 $(BINDIR_)taccess$(_EXE) \
 $(BINDIR_)talign$(_EXE) \
 $(BINDIR_)talloc$(_EXE) \
 $(BINDIR_)tblock$(_EXE) \
 $(BINDIR_)tblockstore$(_EXE) \
 $(BINDIR_)tchain$(_EXE) \
 $(BINDIR_)tcollection$(_EXE) \
 $(BINDIR_)tcontainer$(_EXE) \
 $(BINDIR_)tcurrdir$(_EXE) \
 $(BINDIR_)tdir$(_EXE) \
 $(BINDIR_)tdirscan$(_EXE) \
 $(BINDIR_)texception$(_EXE) \
 $(BINDIR_)texec$(_EXE) \
 $(BINDIR_)texechelper$(_EXE) \
 $(BINDIR_)tfile$(_EXE) \
 $(BINDIR_)tfilename$(_EXE) \
 $(BINDIR_)tinfo$(_EXE) \
 $(BINDIR_)tinterlocked$(_EXE) \
 $(BINDIR_)tmap$(_EXE) \
 $(BINDIR_)tmd5$(_EXE) \
 $(BINDIR_)tptrmap$(_EXE) \
 $(BINDIR_)tsemaphore$(_EXE) \
 $(BINDIR_)tshared$(_EXE) \
 $(BINDIR_)tsort$(_EXE) \
 $(BINDIR_)tstore$(_EXE) \
 $(BINDIR_)tstring$(_EXE) \
 $(BINDIR_)tthread$(_EXE) \
 $(BINDIR_)ttimedate$(_EXE) \
 $(BINDIR_)ttree$(_EXE) \
 $(BINDIR_)tutf$(_EXE) \
 $(BINDIR_)tuuid$(_EXE) \
 $(BINDIR_)tall$(_EXE)

include ../system.mak

$(BINDIR_)exechelpermain$(_EXE): $(TSDIR_)exechelpermain$(_OBJ) $(TSDIR_)exechelper$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)taccess$(_EXE): $(TSDIR_)taccess$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)talign$(_EXE): $(TSDIR_)talign$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)talloc$(_EXE): $(TSDIR_)talloc$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tblock$(_EXE): $(TSDIR_)tblock$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tblockstore$(_EXE): $(TSDIR_)tblockstore$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tchain$(_EXE): $(TSDIR_)tchain$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tcollection$(_EXE): $(TSDIR_)tcollection$(_OBJ) $(TSDIR_)int$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tcontainer$(_EXE): $(TSDIR_)tcontainer$(_OBJ) $(TSDIR_)int$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tcurrdir$(_EXE): $(TSDIR_)tcurrdir$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tdir$(_EXE): $(TSDIR_)tdir$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tdirscan$(_EXE): $(TSDIR_)tdirscan$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)texception$(_EXE): $(TSDIR_)texception$(_OBJ) $(TSDIR_)int$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)texec$(_EXE): $(TSDIR_)texec$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)texechelper$(_EXE): $(TSDIR_)texechelper$(_OBJ) $(TSDIR_)exechelper$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tfile$(_EXE): $(TSDIR_)tfile$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tfilename$(_EXE): $(TSDIR_)tfilename$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tinfo$(_EXE): $(TSDIR_)tinfo$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tinterlocked$(_EXE): $(TSDIR_)tinterlocked$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tmap$(_EXE): $(TSDIR_)tmap$(_OBJ) $(TSDIR_)int$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tmd5$(_EXE): $(TSDIR_)tmd5$(_OBJ) $(TSDIR_)int$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tptrmap$(_EXE): $(TSDIR_)tptrmap$(_OBJ) $(TSDIR_)int$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tsemaphore$(_EXE): $(TSDIR_)tsemaphore$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tshared$(_EXE): $(TSDIR_)tshared$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tsort$(_EXE): $(TSDIR_)tsort$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tstore$(_EXE): $(TSDIR_)tstore$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tstring$(_EXE): $(TSDIR_)tstring$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tthread$(_EXE): $(TSDIR_)tthread$(_OBJ) $(TSDIR_)int$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)ttimedate$(_EXE): $(TSDIR_)ttimedate$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)ttree$(_EXE): $(TSDIR_)ttree$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tutf$(_EXE): $(TSDIR_)tutf$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tuuid$(_EXE): $(TSDIR_)tuuid$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

$(BINDIR_)tall$(_EXE): $(TSDIR_)tall$(_OBJ) $(TUNINGLIB)
	$(ECHO2)
	$(LINK_CONSOLE)

all:: $(SAMPLE_PROGS)

depend::
	$(SD) $(SAMPLE_OBJS)

clean::
	-$(DEL) $(SAMPLE_OBJS)
	-$(DEL) $(SAMPLE_PROGS)
