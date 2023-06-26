
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
# File userobjs.mak

all:: $(USER_OBJS)

depend::
	$(SD) $(USER_OBJS)

clean::
	-$(DEL) $(USER_OBJS)

DEPFILE = $(DEPFILE_)_$(DEPNAME)
