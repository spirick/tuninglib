
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
# File subdirs.mak

$(SUBDIRS:/=/all)::
	@echo +++ Entering directory '$(@D)'
	@cd $(@D) && $(MAKE) all
	@echo --- Leaving directory '$(@D)'

$(SUBDIRS:/=/depend)::
	@echo +++ Entering directory '$(@D)'
	@cd $(@D) && $(MAKE) depend
	@echo --- Leaving directory '$(@D)'

$(SUBDIRS:/=/clean)::
	@echo +++ Entering directory '$(@D)'
	@cd $(@D) && $(MAKE) clean
	@echo --- Leaving directory '$(@D)'

all:: $(SUBDIRS:/=/all)

depend:: $(SUBDIRS:/=/depend)

clean:: $(SUBDIRS:/=/clean)
