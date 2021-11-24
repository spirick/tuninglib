
// Spirick Tuning
//
// A C++ class and template library
// for performance critical applications.
// Copyright (C) 1996-2021 Dietmar Deimling.
// All rights reserved.
// Internet  www.spirick.com
// E-Mail    info@spirick.com
//
// Version 1.48
// File samples/tfile.cpp

#include <stdio.h>
#include "tuning/file.hpp"
#include "tuning/sys/cprocess.hpp"
#include "tuning/sys/ctimedate.hpp"

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

int Round50
(
int i
)
  {
  return ((i + 25) / 50) * 50;
  }

//---------------------------------------------------------------------------

void CheckExtern
(
const char * pc_path,
int i
)
  {
  char ac_param [2] = { (char) ('0' + i), '\0' };
  const char * pc_param = ac_param;
  tl_Exec (pc_path, 1, & pc_param);
  tl_Delay (250);
  }

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  if (argc > 2)
    return 1;

  #if defined TL_WINDOWS

  SetConsoleCP (CP_UTF8);
  SetConsoleOutputCP (CP_UTF8);

  #endif

  const char ac_tfile1 [] = { 't', 'f', 'i', 'l', 'e', '1',
    '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };
  const char ac_tfile2 [] = { 't', 'f', 'i', 'l', 'e', '2',
    '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };

  ct_File co_file;
  co_file. AssignAsName (argv [0]);
  co_file. SetNameExt (ac_tfile1);

  if (argc == 2)
    {
    printf ("+++ Extern %s\n", argv [1]);
    printf ("Exists: %d\n", co_file. Exists ());

    if (co_file. Create ())
      printf ("Create: failed\n");
    else
      printf ("Create: OK\n");

    if (co_file. Create (true))
      printf ("Cr.New: failed\n");
    else
      printf ("Cr.New: OK\n");

    if (co_file. Open (false))
      printf ("Op.Wr.: failed\n");
    else
      printf ("Op.Wr.: OK\n");

    for (;;)
      {
      t_MicroTime i_time = tl_QueryPrecisionTime ();
      bool b_ret = co_file. TryOpen (true, true, 100);
      int i_diff = (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor);
      printf ("Op.Rd.: %d %d\n", b_ret, Round50 (i_diff));

      if (b_ret)
        break;
      }

    printf ("--- Extern %s\n", argv [1]);
    return 0;
    }

  PrintRelName (co_file);
  printf ("Exists: %d\n", co_file. Exists ());

  if (! co_file. Create ())
    {
    printf ("Create: failed\n");
    return 1;
    }

  printf ("Create: OK\n");

  if (co_file. Write ("0123456789ABC", 13))
    printf ("Write : OK\n");
  else
    printf ("Write : failed\n");

  if (co_file. Truncate (10))
    printf ("Trunc : OK\n");
  else
    printf ("Trunc : failed\n");

  CheckExtern (argv [0], 1);

  if (co_file. Close ())
    printf ("Close : OK\n");
  else
    printf ("Close : failed\n");

  tl_Delay (200);

  if (! co_file. Open ())
    {
    printf ("Open  : failed\n");
    return 1;
    }

  printf ("Open  : OK\n");
  t_FileSize o_size;

  if (co_file. QuerySize (o_size))
    printf ("Size  : OK [%" TL_FMT_INT64 "d]\n", o_size);
  else
    printf ("Size  : failed\n");

  bool b_eof;

  if (co_file. EndOfFile (b_eof))
    printf ("EOF   : OK [%d]\n", b_eof);
  else
    printf ("EOF   : failed\n");

  char ac [11];

  if (co_file. Read (ac, 10))
    {
    ac [10] = '\0';
    printf ("Read  : OK [%s]\n", ac);
    }
  else
    printf ("Read  : failed\n");

  if (co_file. EndOfFile (b_eof))
    printf ("EOF   : OK [%d]\n", b_eof);
  else
    printf ("EOF   : failed\n");

  if (co_file. SeekAbs (3))
    printf ("Seek  : OK\n");
  else
    printf ("Seek  : failed\n");

  if (co_file. Read (ac, 4))
    {
    ac [4] = '\0';
    printf ("Read  : OK [%s]\n", ac);
    }
  else
    printf ("Read  : failed\n");

  t_FileSize o_pos;

  if (co_file. QueryPos (o_pos))
    printf ("Pos   : OK [%" TL_FMT_INT64 "d]\n", o_pos);
  else
    printf ("Pos   : failed\n");

  CheckExtern (argv [0], 2);

  if (co_file. Close ())
    printf ("Close : OK\n");
  else
    printf ("Close : failed\n");

  tl_Delay (200);

  printf ("Exists: %d\n", co_file. Exists ());
  ct_FileName co_name2 (co_file);
  co_name2. SetNameExt (ac_tfile2);

  if (co_file. Move (co_name2. GetAllStr ()))
    printf ("Move  : OK\n");
  else
    printf ("Move  : failed\n");

  PrintRelName (co_file);
  printf ("Exists: %d\n", co_file. Exists ());

  if (co_file. Create (true))
    printf ("Cr.New: failed\n");
  else
    printf ("Cr.New: OK\n");

  if (co_file. Delete ())
    printf ("Delete: OK\n");
  else
    printf ("Delete: failed\n");

  printf ("Exists: %d\n", co_file. Exists ());

  return 0;
  }
