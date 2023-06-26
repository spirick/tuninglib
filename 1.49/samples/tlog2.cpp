
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
// File samples/tlog2.cpp

#include <stdio.h>
#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

int main ()
  {
  for (unsigned u = 1; u < 32; u++)
    {
    t_UInt32 u_val = 1u << u;
    printf ("%2u %2u %u\n", u, tl_Log2 (u_val - 1u), u_val - 1u);
    printf ("%2u %2u %u\n", u, tl_Log2 (u_val     ), u_val     );
    printf ("%2u %2u %u\n", u, tl_Log2 (u_val + 1u), u_val + 1u);
    }

  #if defined TL_64

  for (unsigned u = 1; u < 64; u++)
    {
    t_UInt64 u_val = 1ull << u;
    printf ("%2u %2u %" TL_FMT_UINT64 "u\n", u, tl_Log2 (u_val - 1ull), u_val - 1ull);
    printf ("%2u %2u %" TL_FMT_UINT64 "u\n", u, tl_Log2 (u_val       ), u_val       );
    printf ("%2u %2u %" TL_FMT_UINT64 "u\n", u, tl_Log2 (u_val + 1ull), u_val + 1ull);
    }

  #endif

  return 0;
  }
