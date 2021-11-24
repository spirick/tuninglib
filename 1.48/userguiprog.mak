
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
# File userguiprog.mak

all:: $(USER_GUI_PROG)

depend::
	$(SD) $(USER_GUI_PROG_OBJS)

clean::
	-$(DEL) $(USER_GUI_PROG)
	-$(DEL) $(USER_GUI_PROG_OBJS)

$(USER_GUI_PROG):: $(USER_GUI_PROG_OBJS) $(USER_SUB_OBJS) $(USER_GUI_PROG_LIBS)
	$(ECHO2)
	$(LINK_GUI)
