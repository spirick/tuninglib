
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
// File samples/tdir.cpp

#include <stdio.h>
#include "tuning/dir.hpp"
#include "tuning/sys/cprocess.hpp"

#if defined TL_WINDOWS
#include <windows.h>
#endif

//---------------------------------------------------------------------------

void PrintRelName
(
const ct_String & co_name
)
  {
  ct_String co_printName (co_name);
  co_printName. ReplaceAll ("\\", "/");
  t_Int i_pos = co_printName. Last ("bin");

  if (i_pos >= 0)
    printf ("%s\n", co_printName. GetStr (i_pos));
  }

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  if (argc != 1)
    return 1;

  #if defined TL_WINDOWS

  SetConsoleCP (CP_UTF8);
  SetConsoleOutputCP (CP_UTF8);

  #endif

  const char ac_tdir1 [] = { 't', 'd', 'i', 'r', '1',
    '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };
  const char ac_tdir2 [] = { 't', 'd', 'i', 'r', '2',
    '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };

  ct_Directory co_dir;
  co_dir. AssignAsName (argv [0]);
  co_dir. SetNameExt ("");
  co_dir. AppendPath (ac_tdir1);
  PrintRelName (co_dir);
  printf ("Exists: %d\n", co_dir. Exists ());

  if (co_dir. Create ())
    {
    printf ("Create: OK\n");
    printf ("Exists: %d\n", co_dir. Exists ());
    ct_Directory co_dir2;
    co_dir2. AssignAsName (argv [0]);
    co_dir2. SetNameExt ("");
    co_dir2. AppendPath (ac_tdir2);
    PrintRelName (co_dir2);
    printf ("Exists: %d\n", co_dir2. Exists ());

    if (co_dir. Move (co_dir2. GetAllStr ()))
      printf ("Move  : OK\n");
    else
      printf ("Move  : failed\n");

    PrintRelName (co_dir);
    printf ("Exists: %d\n", co_dir. Exists ());

    if (co_dir. Delete ())
      printf ("Delete: OK\n");
    else
      printf ("Delete: failed\n");

    printf ("Exists: %d\n", co_dir. Exists ());
    }
  else
    printf ("Create: failed\n");

  return 0;
  }
