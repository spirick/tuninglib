
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
// File samples/talign.cpp

#include <stdio.h>
#include "tuning/refcount.hpp"
#include "tuning/chn/store.hpp"

//---------------------------------------------------------------------------

int main ()
  {
  printf ("sizeof (void *)      = %d\n", (unsigned) sizeof (void *));
  printf ("sizeof (t_RefCount)  = %d\n", (unsigned) sizeof (t_RefCount));
  printf ("sizeof (ct_RefCount) = %d\n", (unsigned) sizeof (ct_RefCount));
  printf ("sizeof (t_ChnInfo)   = %d\n", (unsigned) sizeof (t_ChnInfo));
  return 0;
  }
