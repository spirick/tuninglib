
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
# File system.mak

# Automatic selection between NMake and GNU Make
# If it doesn't work, comment out some lines

# NMake:    Include file "=nmake.mak"
# GNU Make: Define variable "!include"
!include =nmake.mak

# NMake:    "!ifdef =0" is always false
# GNU Make: Define variables "!ifdef" and "!endif"
!ifdef =0
include $(PROJECTDIR)/gmake.mak
!endif =0
