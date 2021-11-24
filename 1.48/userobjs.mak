
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
# File userobjs.mak

all:: $(USER_OBJS)

depend::
	$(SD) $(USER_OBJS)

clean::
	-$(DEL) $(USER_OBJS)

DEPFILE = $(DEPFILE_)_$(DEPNAME)
