
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
// File samples/tchain.cpp

#include <stdio.h>
#include "tuning/chn/store.hpp"

ct_ChnStore co_TestStore;

//---------------------------------------------------------------------------

void PrintInfo ()
  {
  printf ("MaxExp: %2d  ", co_TestStore. GetMaxChainExp ());
  printf ("Entries: %2d  ", (unsigned) co_TestStore. GetEntries ());
  printf ("Size: %4d  ", (unsigned) co_TestStore. GetSize ());
  printf ("AllocEntries: %2d  ", (unsigned) co_TestStore. QueryAllocEntries ());
  printf ("AllocSize: %4d  ", (unsigned) co_TestStore. QueryAllocSize ());
  printf ("FreeEntries: %2d  ", (unsigned) co_TestStore. QueryFreeEntries ());
  printf ("FreeSize: %4d\n", (unsigned) co_TestStore. QueryFreeSize ());
  }

//---------------------------------------------------------------------------

#define DO(x,y) printf (#y "\n"); x co_TestStore. y; PrintInfo ();

int main ()
  {
  void * pv;
  PrintInfo ();
  DO (, SetMaxChainExp (10))
  DO (pv =, Alloc (1024 - ct_ChnStore::StoreInfoSize ()))
  DO (, Free (pv))
  DO (, FreeUnused ())
  DO (pv =, Alloc (1025 - ct_ChnStore::StoreInfoSize ()))
  DO (, Free (pv))
  return 0;
  }
