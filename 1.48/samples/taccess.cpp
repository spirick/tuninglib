
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
// File samples/taccess.cpp

#include <stdio.h>
#include <stdlib.h>

#include "tuning/string.hpp"
#include "tuning/compcontainer.h"
#include "tuning/sys/ctimedate.hpp"

#include "tuning/chn/array.h"
#include "tuning/chn/sortedarray.h"
#include "tuning/chn/hashtable.h"

//---------------------------------------------------------------------------

const t_UInt i_TestSize = 50000;

template <class t_container>
  void Test ()
    {
    t_UInt i;
    ct_String co_rand;
    ct_String * pco_str;
    gct_CompContainer <t_container> co_container;
    TL_TYPENAME gct_CompContainer <t_container>::t_Position o_pos, o_found;

    srand (1);
    printf ("Begin construction . . .\n");
    t_MicroTime i_time = tl_QueryPrecisionTime ();

    for (i = 0; i < i_TestSize / 40; i ++)
      {
      co_rand = "";

      for (t_UInt j = 0; j < 40; j ++)
        {
        char c = (char) (rand () % 52);

        if (c < 26)
          c += 'A';
        else
          c += 'a' - 26;

        co_rand. Insert (0, c);
        co_container. AddObj (& co_rand);
        // printf ("%s\n", co_rand ());
        }
      }

    TL_ASSERT (co_container. GetLen () == i_TestSize);
    printf ("Ready: %d ms\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));
    printf ("Begin searching . . .\n");
    i_time = tl_QueryPrecisionTime ();

    for (o_pos = co_container. First ();
         o_pos != 0;
         o_pos = co_container. Next (o_pos))
      {
      pco_str = co_container. GetObj (o_pos);
      o_found = co_container. SearchFirstObj (pco_str);

      if (* co_container. GetObj (o_found) != * pco_str)
        printf ("* * * Search error\n");
      }

    printf ("Ready: %d ms\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));
    }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("\nTest Array\n\n");
  Test <gct_Chn_Array <ct_String> > ();
  printf ("\nTest SortedArray\n\n");
  Test <gct_Chn_SortedArray <ct_String> > ();
  printf ("\nTest HashTable\n\n");
  Test <gct_Chn_HashTable <ct_String> > ();
  return 0;
  }
