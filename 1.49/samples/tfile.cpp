
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
    printf ("Main Name  : %s\n", co_printName. GetStr (i_pos));
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

void StartExtern
(
const char * pc_path,
int i
)
  {
  char ac_param [2] = { (char) ('0' + i), '\0' };
  const char * pc_param = ac_param;
  tl_Exec (pc_path, 1, & pc_param);
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

  ct_PrSemaphore co_sem1 ("Spirick_Tuning_TFile1");
  ct_PrSemaphore co_sem2 ("Spirick_Tuning_TFile2");

  if (argc == 1)
    {
    co_sem1. Create (0);
    co_sem2. Create (0);

    if (co_sem1. GetInitSuccess () && co_sem2. GetInitSuccess ())
      printf ("Main Init PrSemaphore OK\n");
    else
      {
      printf ("Main Init PrSemaphore failed\n");
      return 1;
      }
    }
  else // argc == 2
    {
    printf ("+++ Extern %s\n", argv [1]);
    co_sem1. Open ();
    co_sem2. Open ();

    if (co_sem1. GetInitSuccess () && co_sem2. GetInitSuccess ())
      printf ("Ext. Init PrSemaphore OK\n");
    else
      {
      printf ("Ext. Init PrSemaphore failed\n");
      return 1;
      }

    printf ("Ext. Exists: %d\n", co_file. Exists ());

    if (co_file. Create ())
      printf ("Ext. Create: failed\n");
    else
      printf ("Ext. Create: OK\n");

    if (co_file. Create (true))
      printf ("Ext. Cr.New: failed\n");
    else
      printf ("Ext. Cr.New: OK\n");

    if (co_file. Open (false))
      printf ("Ext. Op.Wr.: failed\n");
    else
      printf ("Ext. Op.Wr.: OK\n");

    fflush (stdout);
    co_sem2. Release ();
    co_sem1. Acquire ();
    bool b_open = false;

    while (! b_open)
      {
      t_MicroTime i_time = tl_QueryPrecisionTime ();
      b_open = co_file. TryOpen (true, true, 100);
      int i_diff = (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor);
      printf ("Ext. Op.Rd.: %d %d\n", b_open, Round50 (i_diff));
      fflush (stdout);
      }

    if (co_file. Close ())
      printf ("Ext. Close : OK\n");
    else
      printf ("Ext. Close : failed\n");

    printf ("--- Extern %s\n", argv [1]);
    fflush (stdout);
    co_sem2. Release ();
    return 0;
    }

  PrintRelName (co_file);
  printf ("Main Exists: %d\n", co_file. Exists ());

  if (! co_file. Create ())
    {
    printf ("Main Create: failed\n");
    return 1;
    }

  printf ("Main Create: OK\n");

  if (co_file. Write ("0123456789ABC", 13))
    printf ("Main Write : OK\n");
  else
    printf ("Main Write : failed\n");

  if (co_file. Truncate (10))
    printf ("Main Trunc : OK\n");
  else
    printf ("Main Trunc : failed\n");

  fflush (stdout);
  StartExtern (argv [0], 1);
  co_sem2. Acquire (); // Before ext. loop

  printf ("Main Delay 250\n");
  fflush (stdout);
  co_sem1. Release (); // Start ext. loop
  tl_Delay (250);

  if (co_file. Close ())
    printf ("Main Close : OK\n");
  else
    printf ("Main Close : failed\n");

  fflush (stdout);
  co_sem2. Acquire (); // Ext. end

  if (! co_file. Open ())
    {
    printf ("Main Open  : failed\n");
    return 1;
    }

  printf ("Main Open  : OK\n");
  t_FileSize o_size;

  if (co_file. QuerySize (o_size))
    printf ("Main Size  : OK [%" TL_FMT_INT64 "d]\n", o_size);
  else
    printf ("Main Size  : failed\n");

  bool b_eof;

  if (co_file. EndOfFile (b_eof))
    printf ("Main EOF   : OK [%d]\n", b_eof);
  else
    printf ("Main EOF   : failed\n");

  char ac [11];

  if (co_file. Read (ac, 10))
    {
    ac [10] = '\0';
    printf ("Main Read  : OK [%s]\n", ac);
    }
  else
    printf ("Main Read  : failed\n");

  if (co_file. EndOfFile (b_eof))
    printf ("Main EOF   : OK [%d]\n", b_eof);
  else
    printf ("Main EOF   : failed\n");

  if (co_file. SeekAbs (3))
    printf ("Main Seek  : OK\n");
  else
    printf ("Main Seek  : failed\n");

  if (co_file. Read (ac, 4))
    {
    ac [4] = '\0';
    printf ("Main Read  : OK [%s]\n", ac);
    }
  else
    printf ("Main Read  : failed\n");

  t_FileSize o_pos;

  if (co_file. QueryPos (o_pos))
    printf ("Main Pos   : OK [%" TL_FMT_INT64 "d]\n", o_pos);
  else
    printf ("Main Pos   : failed\n");

  fflush (stdout);
  StartExtern (argv [0], 2);
  co_sem2. Acquire (); // Before ext. loop

  if (co_file. Close ())
    printf ("Main Close : OK\n");
  else
    printf ("Main Close : failed\n");

  fflush (stdout);
  co_sem1. Release (); // Start ext. loop
  co_sem2. Acquire (); // Ext. end

  printf ("Main Exists: %d\n", co_file. Exists ());
  ct_FileName co_name2 (co_file);
  co_name2. SetNameExt (ac_tfile2);

  if (co_file. Move (co_name2. GetAllStr ()))
    printf ("Main Move  : OK\n");
  else
    printf ("Main Move  : failed\n");

  PrintRelName (co_file);
  printf ("Main Exists: %d\n", co_file. Exists ());

  if (co_file. Create (true))
    printf ("Main Cr.New: failed\n");
  else
    printf ("Main Cr.New: OK\n");

  if (co_file. Delete ())
    printf ("Main Delete: OK\n");
  else
    printf ("Main Delete: failed\n");

  printf ("Main Exists: %d\n", co_file. Exists ());

  return 0;
  }
