
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
# File userlib.mak

all:: $(USER_LIB)

depend::
	$(SD) $(USER_LIB_OBJS)

clean::
	-$(DEL) $(USER_LIB)
	-$(DEL) $(USER_LIB_OBJS)

$(USER_LIB):: $(USER_LIB_OBJS) $(USER_SUB_OBJS)
	$(ECHO2)
	-$(DEL) $(USER_LIB)
	$(LINK_LIB)
