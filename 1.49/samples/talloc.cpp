
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
// File samples/talloc.cpp

#include <stdio.h>
#include "tuning/sys/calloc.hpp"

//---------------------------------------------------------------------------

void MyReserveHandler ()
  {
  printf ("ReserveHandler  HasReserve = %d  ReserveSize = %" TL_FMT_UINT "u\n",
    tl_HasReserve (), tl_GetReserveSize ());
  }

//---------------------------------------------------------------------------

void MyOverflowHandler ()
  {
  printf ("OverflowHandler\n");
  }

//---------------------------------------------------------------------------

int main ()
  {
  const t_UInt u_testSize = 16 * 1024 * 1024;
  tl_SetReserveHandler (MyReserveHandler);
  tl_SetOverflowHandler (MyOverflowHandler);
  tl_SetReserveSize (2 * u_testSize);

  for (unsigned u = 0; u < tl_MaxAlloc () / u_testSize; u ++)
    {
    printf ("%d\n", u);

    #if 1

      // Check tl_Alloc
      tl_Alloc (u_testSize);

    #else

      // Check tl_Realloc
      void * pv = tl_Alloc (1);
      tl_Realloc (pv, u_testSize);

    #endif
    }

  return 0;
  }
