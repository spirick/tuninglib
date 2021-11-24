
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
// File samples/tuuid.cpp

#include <stdio.h>
#include "tuning/uuid.hpp"

//---------------------------------------------------------------------------

const bool b_upperCase = true;

int main ()
  {
  ct_UUID co_uuid1;

  if (! co_uuid1. Create ())
    return 1;

  char ac1 [37];

  if (co_uuid1. ToStr (ac1, 36, b_upperCase))
    {
    ac1 [36] = 0;
    printf ("%s\n", ac1);
    ct_UUID co_uuid2;

    if (co_uuid2. FromStr (ac1, 36))
      {
      char ac2 [37];

      if (co_uuid2. ToStr (ac2, 36, b_upperCase))
        {
        ac2 [36] = 0;
        printf ("%s\n", ac2);
        }

      if (co_uuid1 == co_uuid2)
        printf ("ok\n");

      t_UInt u_hash = co_uuid1. GetHash ();
      printf ("hash = %" TL_FMT_UINT "X\n", u_hash);
      }
    }

  return 0;
  }
