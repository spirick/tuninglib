
// Spirick Tuning
//
// A C++ class and template library
// for performance critical applications.
// Copyright (C) 1996-2023 Dietmar Deimling.
// All rights reserved.
// Internet  www.spirick.com
// E-Mail    info@spirick.com
//
// Version 1.49
// File samples/tcurrdir.cpp

#include <stdio.h>
#include "tuning/dir.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

int main ()
  {
    {
    ct_Directory co_dir;

    if (co_dir. QueryCurrentDrive ())
      printf ("CurrentDrive: \"%s\"\n", co_dir. GetAllStr ());
    }

    {
    ct_Directory co_dir;

    if (co_dir. QueryCurrentDirectory ())
      printf ("CurrentDirectory: \"%s\"\n", co_dir. GetAllStr ());
    }

    {
    ct_Directory co_dir;

    if (co_dir. QueryCurrentDriveDirectory ())
      printf ("CurrentDriveDirectory: \"%s\"\n", co_dir. GetAllStr ());
    }

  #if defined TL_WINDOWS

    {
    ct_Directory co_dir ("C:");

    if (co_dir. QueryCurrentDrive ())
      printf ("CurrentDrive (C): \"%s\"\n", co_dir. GetAllStr ());
    }

    {
    ct_Directory co_dir ("C:");

    if (co_dir. QueryCurrentDirectory ())
      printf ("CurrentDirectory (C): \"%s\"\n", co_dir. GetAllStr ());
    }

    {
    ct_Directory co_dir ("C:");

    if (co_dir. QueryCurrentDriveDirectory ())
      printf ("CurrentDriveDirectory (C): \"%s\"\n", co_dir. GetAllStr ());
    }

  #endif

    {
    ct_Directory co_dir (tl_GetTempPath (). GetStr ());
    printf ("%s\n", co_dir. GetAllStr ());
    }

  return 0;
  }
