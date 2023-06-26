
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
# File tuning/makefile

PROJECTDIR    = ..
SUBDIR        = tuning

SUBDIRS       = std/ rnd/ chn/ sys/

USER_LIB      = $(TUNINGLIB)

USER_LIB_OBJS = \
 $(TSDIR_)array$(_OBJ) \
 $(TSDIR_)blockdlist$(_OBJ) \
 $(TSDIR_)blockrefdlist$(_OBJ) \
 $(TSDIR_)dir$(_OBJ) \
 $(TSDIR_)dirscan$(_OBJ) \
 $(TSDIR_)dlist$(_OBJ) \
 $(TSDIR_)file$(_OBJ) \
 $(TSDIR_)filename$(_OBJ) \
 $(TSDIR_)md5$(_OBJ) \
 $(TSDIR_)object$(_OBJ) \
 $(TSDIR_)packstore$(_OBJ) \
 $(TSDIR_)pageblock$(_OBJ) \
 $(TSDIR_)printf$(_OBJ) \
 $(TSDIR_)refdlist$(_OBJ) \
 $(TSDIR_)sortedarray$(_OBJ) \
 $(TSDIR_)stringsort$(_OBJ) \
 $(TSDIR_)timedate$(_OBJ) \
 $(TSDIR_)uuid$(_OBJ)

include ./subobjs.mak

USER_SUB_OBJS = \
 $(STD_OBJS) \
 $(RND_OBJS) \
 $(CHN_OBJS) \
 $(SYS_OBJS)

include ../system.mak
