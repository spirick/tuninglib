
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
# File nmake_msvc.mak

# Some MSVC compiler options:
#
# -EH{s|a}[c][-] Specifies the model of exception handling
# -EHs           Synchronous exception handling (only throw statements, smaller code)
# -EHa           Asynchronous exception handling (any instruction may generate an exception)
# -EHc           Assume that extern C functions never throw an exception
# -GX[-]         Enables synchronous exception handling (equivalent to -EHsc)
# -F number      Sets stack size
# -Fofilename    Creates an object file
# -G5            Optimizes code to favor the Pentium processor
# -G6            Optimizes code to favor the Pentium Pro processor
# -GA            Optimizes code for Windows application
# -GB            Optimizes code to favor the Pentium processor. (blend)
# -GD            Optimizes code for Windows DLL
# -Gf            Eliminate Duplicate Strings
# -GR            Enables run-time type information (RTTI)
# -Gy            Enables function-level linking
# -J             Changes the default char type
# -LD            Creates a dynamic-link library
# -LDd           Creates a debug dynamic-link library
# -MD            Creates a multithreaded DLL, using MSVCRT.LIB
# -MDd           Creates a debug multithreaded DLL, using MSVCRTD.LIB
# -ML            Creates a single-threaded executable file, using LIBC.LIB
# -MLd           Creates a debug single-threaded executable file, using LIBCD.LIB
# -MT            Creates a multithreaded executable file, using LIBCMT.LIB
# -MTd           Creates a debug multithreaded executable file, using LIBCMTD.LIB
# -nologo        Suppresses display of sign-on banner
# -O1            Creates small code
# -O2            Creates fast code
# -Od            Disables optimization
# -Ox            Uses maximum optimization (/Ob1gity /Gs)
# -TP            Specifies a C++ source file
# -Wlevel        Sets warning level
# -Z7            Generates C 7.0-compatible debugging information
# -Zd            Generates line numbers debugging information only
# -Zi            Generates complete debugging information (program database)
# -Zmnumber      Sets the compiler's memory allocation limit
# -Zpn           Packs structure members

# Some MSVC linker options:
#
# -DEBUG          Creates debugging information
# -DEBUGTYPE:CV   Microsoft Format
# -DEBUGTYPE:COFF COFF Format
# -DEBUGTYPE:BOTH Both Formats
# -DEF:filename   Passes a module-definition (.DEF) file to the linker
# -DELAY          Controls the delayed loading of DLLs
# -DELAYLOAD      Causes the delayed loading of the specified DLL
# -DLL            Builds a DLL
# -LIBPATH:path   Allows the user to override the environmental library path
# -NOLOGO         Suppresses startup banner
# -OPT:REF        Eliminate functions and/or data that is never referenced
# -OPT:NOREF      Keep functions and/or data that is never referenced
# -OPT:ICF[,iterations] Perform identical COMDAT folding
# -OUT:filename   Specifies the output file name
# -PDB:filename   Creates a program database (.PDB) file
# -SUBSYSTEM:{CONSOLE|WINDOWS|NATIVE|POSIX|WINDOWSCE} [,major[.minor] ]
#                 Tells the operating system how to run the .EXE file

# Use MSVC 2005 or newer

!if exist (project_msvc.mak)
! include project_msvc.mak
!endif

EXCEPT_ON     = -EHsc
EXCEPT_OFF    = -EHs-c-

RTTI_ON       = -GR
RTTI_OFF      = -GR-

UNSIGNED_CHAR = -J
SHORT_WCHAR   = 

# -D_UNICODE -DUNICODE
COMPILE       = cl -nologo -DWIN32 -D_WIN32_WINNT=0x0501 -W4 $(OPTIONS) $(INCLUDES) -c $< -Fo$@
WIN_API_LIBS  = user32.lib Rpcrt4.lib Advapi32.lib Psapi.lib
WIN_GUI_LIBS  = 
LINK_LIB      = link -lib -nologo -out:$@ $**
LINK_EXE      = link -nologo -largeaddressaware -incremental:no -out:$@ $** $(WIN_API_LIBS)

!ifdef TL_RELEASE

COMPILERDIR   = $(TL_COMPILER)R

#THREAD_ON     = -MD
THREAD_ON     = -MT
THREAD_OFF    = -ML

SYSTEM_OPTS   = -DNDEBUG -O2
LINK_CONSOLE  = $(LINK_EXE) -subsystem:console -pdb:none -opt:ref
LINK_GUI      = $(LINK_EXE) -subsystem:windows -pdb:none -opt:ref $(WIN_GUI_LIBS)

!else # DEBUG

COMPILERDIR   = $(TL_COMPILER)D

#THREAD_ON     = -MDd
THREAD_ON     = -MTd
THREAD_OFF    = -MLd

SYSTEM_OPTS   = -DDEBUG -Od -Zi -Fd$(TSDIR_)
LINK_CONSOLE  = $(LINK_EXE) -subsystem:console -pdb:$(@:.exe=.pdb) -debug
LINK_GUI      = $(LINK_EXE) -subsystem:windows -pdb:$(@:.exe=.pdb) -debug $(WIN_GUI_LIBS)

!endif
