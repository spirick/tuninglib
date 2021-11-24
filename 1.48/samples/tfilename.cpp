
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
// File samples/tfilename.cpp

#include <stdio.h>
#include "tuning/filename.hpp"

//---------------------------------------------------------------------------

int main ()
  {
  ct_FileName co_name;

  co_name. AssignAsName ("A:\\PATH\\NAME.EXT");

  printf ("\n");
  printf ("Drive        : \"%s\"\n", co_name. GetDrive (). GetStr ());
  printf ("Path         : \"%s\"\n", co_name. GetPath (). GetStr ());
  printf ("PurePath     : \"%s\"\n", co_name. GetPurePath (). GetStr ());
  printf ("DrivePath    : \"%s\"\n", co_name. GetDrivePath (). GetStr ());
  printf ("PureDrivePath: \"%s\"\n", co_name. GetPureDrivePath (). GetStr ());
  printf ("Name         : \"%s\"\n", co_name. GetName (). GetStr ());
  printf ("Ext          : \"%s\"\n", co_name. GetExt (). GetStr ());
  printf ("NameExt      : \"%s\"\n", co_name. GetNameExt (). GetStr ());
  printf ("All          : \"%s\"\n", co_name. GetAllStr ());

  co_name. ToRel ("A:\\PATH\\X");
  printf ("ToRel        : \"%s\"\n", co_name. GetAllStr ());
  co_name. ToAbs ("A:\\PATH\\X");
  printf ("ToAbs        : \"%s\"\n", co_name. GetAllStr ());
  co_name. ToLower ();
  printf ("ToLower      : \"%s\"\n", co_name. GetAllStr ());

  printf ("Wildc        : %d\n", co_name. HasWildCards ());
  printf ("Abs          : %d\n", co_name. IsAbs ());
  printf ("Rel          : %d\n", co_name. IsRel ());

  if (co_name. GetExt () == "ext")
    printf ("\nGetExt () == \"ext\"\n");

  return 0;
  }
