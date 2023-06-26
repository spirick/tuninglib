
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
# File tuning/subobjs.mak

STD_OBJS = \
 $(TSDIR_)stdstore$(_OBJ)

RND_OBJS = \
 $(TSDIR_)rndstore$(_OBJ)

CHN_OBJS = \
 $(TSDIR_)chnstore$(_OBJ)

SYS_OBJS = \
 $(TSDIR_)calloc$(_OBJ) \
 $(TSDIR_)cdir$(_OBJ) \
 $(TSDIR_)cfile$(_OBJ) \
 $(TSDIR_)cinfo$(_OBJ) \
 $(TSDIR_)cmemory$(_OBJ) \
 $(TSDIR_)cprocess$(_OBJ) \
 $(TSDIR_)cstring$(_OBJ) \
 $(TSDIR_)ctimedate$(_OBJ) \
 $(TSDIR_)cutf$(_OBJ)
