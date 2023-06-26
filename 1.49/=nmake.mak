
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
# File =nmake.mak

# 'all' is always the first target
all::

#---------------------------------------------------------------
# Global checks

!ifndef TL_COMPILER
! error ERROR_UNKNOWN_COMPILER
!endif

!ifndef PROJECTDIR
! error ERROR_UNKNOWN_PROJECTDIR
!endif

!ifndef SUBDIR
! error ERROR_UNKNOWN_SUBDIR
!endif

#---------------------------------------------------------------
# System wide variables

S     = \ 
_OBJ  = .obj
_LIB  = .lib
_EXE  = .exe

DEL   = cmd /C del /Q 
MKDIR = cmd /C mkdir 
RMDIR = cmd /C rmdir /S /Q 
MAKE  = nmake -nologo

PROJECTDIR = $(PROJECTDIR:/=\)
SUBDIR     = $(SUBDIR:/=\)

#---------------------------------------------------------------
# Automatic selection of compiler
# If it doesn't work, comment out some lines

!ifdef MSDEVDIR
! include nmake_msvc.mak
!else
! error ERROR_UNKNOWN_COMPILER
!endif

#---------------------------------------------------------------
# Other includes

!include project.mak

!ifdef TL_PROJECT_TARGETDIR
PROJECT_TARGETDIR = $(TL_PROJECT_TARGETDIR)
!endif

!ifndef PROJECT_TARGETDIR
! error ERROR_UNKNOWN_TARGETDIR
!endif

!include common.mak

#---------------------------------------------------------------
# Subdirectories

!ifdef SUBDIRS
! include subdirs.mak
!endif

#---------------------------------------------------------------
# User objects

!ifdef USER_OBJS
! include userobjs.mak
!endif

#---------------------------------------------------------------
# User library

!ifdef USER_LIB
! include userlib.mak
!endif

#---------------------------------------------------------------
# User console program

!ifdef USER_CON_PROG
! include userconprog.mak
!endif

#---------------------------------------------------------------
# User GUI program

!ifdef USER_GUI_PROG
! include userguiprog.mak
!endif

#---------------------------------------------------------------

!if exist ($(DEPFILE))
! include $(DEPFILE)
!endif

#---------------------------------------------------------------
# Compile

.SUFFIXES: .c .C .cpp $(_OBJ)

{.}.c{$(TSDIR)}$(_OBJ):
	$(ECHO1)
	$(COMPILE)
	$(SD) $<

{.}.C{$(TSDIR)}$(_OBJ):
	$(ECHO1)
	$(COMPILE)
	$(SD) $<

{.}.cpp{$(TSDIR)}$(_OBJ):
	$(ECHO1)
	$(COMPILE)
	$(SD) $<
