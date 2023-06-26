
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
