
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
// File samples/tdirscan.cpp

#include <stdio.h>
#include "tuning/dirscan.hpp"
#include "tuning/timedate.hpp"
#include "tuning/file.hpp"
#include "tuning/sys/cprocess.hpp"

#if defined TL_WINDOWS
#include <windows.h>
#undef FindFirstFile
#undef FindNextFile
#endif

//---------------------------------------------------------------------------

ct_FileName co_FileName;

void PrintEntry
(
ct_DirScan & co_scan
)
  {
  if ((! co_scan. IsDirectory ()) &&
      (co_FileName. GetAllLen () < co_scan. GetAllLen ()))
    co_FileName. AssignAsName (co_scan. GetAllStr ());

  ct_TimeDate co_td (tl_UTCToLocalTime (co_scan. GetLastWriteTime ()));
    
  printf ("%-24s %8" TL_FMT_INT64 "d %02d-%02d-%04d %2d:%02d.%02d ",
    co_scan. GetNameStr (), co_scan. GetSize (),
    co_td. GetMonth (), co_td. GetDay (), co_td. GetYear (),
    co_td. GetHour (), co_td. GetMinute (), co_td. GetSecond ());

  if (co_scan. IsDirectory ()) printf (" <DIR>");
  if (co_scan. IsArchive ())   printf (" ARC");
  if (co_scan. IsHidden ())    printf (" HID");
  if (co_scan. IsReadOnly ())  printf (" RO");
  if (co_scan. IsSystem ())    printf (" SYS");

  printf ("\n");
  }

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  #if defined TL_WINDOWS

  SetConsoleCP (CP_UTF8);
  SetConsoleOutputCP (CP_UTF8);

  #endif

  if (argc == 2)
    {
    ct_DirScan co_scan;
    co_scan. AssignAsPath (argv [1]);
    printf ("Contents of directory %s\n", co_scan. GetAllStr ());
    co_scan. SetNameExt ("*");

    if (! co_scan. Exists ())
      printf ("Does not exist\n");
    else
      {
      for (co_scan. FindFirst ();
           co_scan. Found ();
           co_scan. FindNext ())
        PrintEntry (co_scan);

      co_scan. SetNameExt ("*");
      printf ("\nFiles of directory %s\n", co_scan. GetAllStr ());

      for (co_scan. FindFirstFile ();
           co_scan. Found ();
           co_scan. FindNextFile ())
        PrintEntry (co_scan);

      co_scan. SetNameExt ("*");
      printf ("\nSubdirectories of directory %s\n", co_scan. GetAllStr ());

      for (co_scan. FindFirstDirectory ();
           co_scan. Found ();
           co_scan. FindNextDirectory ())
        PrintEntry (co_scan);
      }

    printf ("\nFindOnce\n");
    co_scan = co_FileName;
    co_scan. FindOnce ();
    PrintEntry (co_scan);

    printf ("\nFindOnce %s\n", co_FileName. GetAllStr ());
    co_scan. FindOnce (co_FileName);
    PrintEntry (co_scan);

    printf ("\nFindOncePath\n");
    co_scan = co_FileName;
    co_scan. FindOncePath ();
    PrintEntry (co_scan);

    printf ("\nFindOncePath %s\n", co_FileName. GetAllStr ());
    co_scan. FindOncePath (co_FileName);
    PrintEntry (co_scan);
    }
  //--------------------------------------
  else
    {
    const char ac_tdir1 [] = { 't', 'd', 'i', 'r', '1',
      '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };
    const char ac_tdir2 [] = { 't', 'd', 'i', 'r', '2',
      '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };

    ct_Directory co_dir1;
    co_dir1. AssignAsName (argv [0]);
    co_dir1. SetNameExt ("");
    co_dir1. AppendPath (ac_tdir1);
    co_dir1. Create ();
    ct_Directory co_dir2;
    co_dir2. AssignAsName (argv [0]);
    co_dir2. SetNameExt ("");
    co_dir2. AppendPath (ac_tdir2);
    co_dir2. Create ();

    const char ac_tfile1 [] = { 't', 'f', 'i', 'l', 'e', '1',
      '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };
    const char ac_tfile2 [] = { 't', 'f', 'i', 'l', 'e', '2',
      '\xC3', '\x84', '\xC3', '\x96', '\xC3', '\x9C', 0 };

    ct_File co_file1;
    co_file1. AssignAsPath (co_dir2);
    co_file1. SetNameExt (ac_tfile1);
    co_file1. Create ();
    co_file1. Write ("0123456789", 10);
    co_file1. Close ();
    ct_File co_file2;
    co_file2. AssignAsPath (co_dir2);
    co_file2. SetNameExt (ac_tfile2);
    co_file2. Create ();
    co_file2. Write ("0123456789ABC", 13);
    co_file2. Close ();

    ct_DirScan co_scan;
    co_scan. AssignAsName (argv [0]);
    co_scan. SetNameExt ("*");
    printf ("Subdirectories:\n");

    for (co_scan. FindFirstDirectory ();
         co_scan. Found ();
         co_scan. FindNextDirectory ())
      printf ("%s\n", co_scan. GetNameStr ());

    co_scan. AssignAsName (co_dir2);
    co_scan. SetNameExt ("*");
    printf ("\nFiles:\n");

    for (co_scan. FindFirstFile ();
         co_scan. Found ();
         co_scan. FindNextFile ())
      printf ("%s %2" TL_FMT_INT64 "d\n",
        co_scan. GetNameStr (), co_scan. GetSize ());

    co_file1. Delete ();
    co_file2. Delete ();
    co_dir1. Delete ();
    co_dir2. Delete ();
    }

  return 0;
  }
