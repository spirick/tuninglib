
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
// File samples/ttree.cpp

#include <stdio.h>
#include "tuning/dirscan.hpp"
#include "tuning/stringsort.hpp"
#include "tuning/std/array.h"

//---------------------------------------------------------------------------

class ct_SortedDirScan
  {
  typedef gct_Std_Array <t_Int>        t_Index;
  typedef gct_Std_Array <const char *> t_Str;
  typedef gct_Std_Array <ct_FileName>  t_Name;

  t_Index::t_Position  o_CurrPos;
  t_Index              co_Index;
  t_Str                co_Str;
  t_Name               co_Name;

public:
                       ct_SortedDirScan (const ct_FileName * pco_name);

  void                 FindFirstDirectory ();
  void                 FindNextDirectory ();
  bool                 Found () const;
  bool                 IsLast () const;

  const ct_FileName *  GetFileName () const;
  };

//---------------------------------------------------------------------------

ct_SortedDirScan::ct_SortedDirScan
(
const ct_FileName * pco_name
)
  {
  ct_DirScan co_scan;

  co_scan. AssignAsPath (* pco_name);
  co_scan. SetNameExt ("*");

  for (co_scan. FindFirstDirectory (); co_scan. Found (); co_scan. FindNextDirectory ())
    {
    if ((co_scan. GetNameExt () != ".")
        && (co_scan. GetNameExt () != "..")
//        Directories to ignore
//        && (co_scan. GetNameExt () != "CVS")
//        && (co_scan. GetNameExt () != ".sniffdir")
        )
      {
      co_Index. AddObjAfterLast ();
      co_Name. AddObjAfterLast (& co_scan);
      const char * pc_name = co_Name. GetLastObj ()-> GetNameStr ();
      co_Str. AddObjAfterLast (& pc_name);
      }
    }

  if (! co_Index. IsEmpty ())
    {
    ct_StringSort co_sort;
    co_sort. Sort (co_Str. GetFirstObj (), co_Index. GetFirstObj (), co_Index. GetLen ());
    }
  }

//---------------------------------------------------------------------------

void ct_SortedDirScan::FindFirstDirectory ()
  {
  o_CurrPos = co_Index. First ();
  }

//---------------------------------------------------------------------------

void ct_SortedDirScan::FindNextDirectory ()
  {
  o_CurrPos = co_Index. Next (o_CurrPos);
  }

//---------------------------------------------------------------------------

bool ct_SortedDirScan::Found () const
  {
  return o_CurrPos != 0;
  }

//---------------------------------------------------------------------------

bool ct_SortedDirScan::IsLast () const
  {
  return o_CurrPos == co_Index. Last ();
  }

//---------------------------------------------------------------------------

const ct_FileName * ct_SortedDirScan::GetFileName () const
  {
  return co_Name. GetNthObj (* co_Index. GetObj (o_CurrPos) + 1);
  }

//---------------------------------------------------------------------------

ct_String co_Prefix;

void ScanDir
(
const ct_FileName * pco_name,
bool b_last
)
  {
  if (pco_name-> HasName () || pco_name-> HasExt ())
    {
    printf ("%s+---%s\n", co_Prefix. GetStr (), pco_name-> GetNameStr ());

    if (b_last)
      co_Prefix += "    ";
    else
      co_Prefix += "|   ";
    }
  else
    {
    ct_FileName co_pure;
    co_pure. AssignAsName (pco_name-> GetPureDrivePath ());
    printf ("%s\n", co_pure. GetNameStr ());
    }

  ct_SortedDirScan co_scan (pco_name);

  for (co_scan. FindFirstDirectory (); co_scan. Found (); co_scan. FindNextDirectory ())
    ScanDir (co_scan. GetFileName (), co_scan. IsLast ());

  if (pco_name-> HasName () || pco_name-> HasExt ())
    co_Prefix. DeleteRev (4);
  }

//---------------------------------------------------------------------------

int main
(
int argc,
char * * argv
)
  {
  ct_Directory co_dir;

  if (argc > 1)
    co_dir. AssignAsPath (argv [1]);
  else
    co_dir. QueryCurrentDriveDirectory ();

  ScanDir (& co_dir, false);

  return 0;
  }
