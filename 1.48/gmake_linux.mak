
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
# File gmake_linux.mak

# Some GNU compiler options
#
# -g                Generate debugging information
# -O | -O1          Optimize
# -O2               Optimize even more
# -O3               Optimize yet more (Like -O2, and also turns on the inline-functions)
# -fexceptions      Enable exception handling (default)
# -fno-exceptions   Disable exception handling
# -frtti            Generate run time type descriptor information (default)
# -fno-rtti         Do not generate run time type descriptor information
# -fsigned-char     Make 'char' be signed by default (machine dependend default)
# -funsigned-char   Make 'char' be unsigned by default (machine dependend default)
# -fstack-check     Insert stack checking code into the program (for multiple threads)
# -fno-stack-check  Do not insert stack checking code into the program (default)
# -fpack-struct     Pack structure members together without holes
# -fno-pack-struct  Do not pack structure members together without holes (default)
# -fshort-enums     Use the smallest fitting integer to hold enums
# -fno-short-enums  Do not use the smallest fitting integer to hold enums (default)

# Some GNU linker options
#
# -s                Remove all symbol table and relocation information from the executable

# g++ 4.4.5 or newer

sinclude $(PROJECTDIR)/project_linux.mak

EXCEPT_ON     = -fexceptions
EXCEPT_OFF    = -fno-exceptions

RTTI_ON       = -frtti
RTTI_OFF      = -fno-rtti

# Macros for Pthreads
THREAD_ON     = -DTL_MULTI -D_REENTRANT
THREAD_OFF    =

UNSIGNED_CHAR = -funsigned-char
SHORT_WCHAR   = -fshort-wchar

# in case of __sync_add_and_fetch linker error: -march=pentium4
COMPILE       = g++ $(OPTIONS) $(INCLUDES) -Wall -pedantic -c $< -o $@
LINK_PTHREAD  = $(subst -DTL_MULTI,-lpthread,$(findstring -DTL_MULTI,$(OPTIONS)))
LINUX_LIBS    = -lrt -luuid
LINK_LIB      = ar -cq $@ $+

ifdef TL_RELEASE

COMPILERDIR   = $(TL_COMPILER)R

SYSTEM_OPTS   = -DNDEBUG -O3 -fno-strict-aliasing
LINK_CONSOLE  = g++ -s -o $@ $+ $(LINK_PTHREAD) $(LINUX_LIBS)
LINK_GUI      = g++ -s -o $@ $+ $(LINK_PTHREAD) $(LINUX_LIBS)

else # DEBUG

COMPILERDIR   = $(TL_COMPILER)D

SYSTEM_OPTS   = -g
LINK_CONSOLE  = g++ -g -o $@ $+ $(LINK_PTHREAD) $(LINUX_LIBS)
LINK_GUI      = g++ -g -o $@ $+ $(LINK_PTHREAD) $(LINUX_LIBS)

endif
