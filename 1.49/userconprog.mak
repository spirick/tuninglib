
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
# File userconprog.mak

all:: $(USER_CON_PROG)

depend::
	$(SD) $(USER_CON_PROG_OBJS)

clean::
	-$(DEL) $(USER_CON_PROG)
	-$(DEL) $(USER_CON_PROG_OBJS)

$(USER_CON_PROG):: $(USER_CON_PROG_OBJS) $(USER_SUB_OBJS) $(USER_CON_PROG_LIBS)
	$(ECHO2)
	$(LINK_CONSOLE)
