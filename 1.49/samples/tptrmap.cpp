
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
// File samples/tptrmap.cpp

#include <stdio.h>
#include "tuning/ptrmap.h"
#include "tuning/string.hpp"
#include "tuning/std/array.h"
#include "samples/int.hpp"

//---------------------------------------------------------------------------

void PrintValue (const int * pco_int)
  {
  printf ("int = %d", * pco_int);
  }

void PrintValue (const t_UInt32 * pco_int)
  {
  printf ("t_UInt32 = %d", * pco_int);
  }

void PrintValue (const ct_Int * pco_int)
  {
  printf ("ct_Int = %d", pco_int-> GetValue ());
  }

void PrintValue (const ct_String * pco_str)
  {
  printf ("ct_String = \"%s\"", pco_str-> GetStr ());
  }

template <class t_key>
  void PrintValue (const gct_Key <t_key> * pco_key)
    {
    printf ("gct_Key of ");
    PrintValue (& pco_key-> o_Key);
    }

//---------------------------------------------------------------------------

template <class t_map>
  void PrintMap
  (
  const t_map * pco_map
  )
    {
    printf ("Map container:\n");

    for (TL_TYPENAME t_map::t_Position o_pos = pco_map-> First ();
         o_pos != 0;
         o_pos = pco_map-> Next (o_pos))
      {
      const TL_TYPENAME t_map::t_Key o_key = pco_map-> GetKey (o_pos);

      printf ("  Position: ");
      PrintValue (& o_pos);
      printf ("\n");
      printf ("    Key:            ");
      PrintValue (& o_key);
      printf ("  Value: ");
      PrintValue (pco_map-> GetValPtr (o_pos));
      printf ("\n    GetFirstValPtr (");
      PrintValue (& o_key);
      printf (") Value: ");
      PrintValue (pco_map-> GetFirstValPtr (o_key));
      printf ("\n");
      }
    }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("Begin of main\n");

    {
    printf ("\n");
    // Key: ct_String, Value: ct_Int
    gct_PtrMap <gct_Std32Array <gct_PtrMapEntry <ct_String> >, ct_Int> co_map;
    co_map. AddKeyAndValPtr ("abc", new ct_Int (11));
    co_map. AddKeyAndValPtr ("def", new ct_Int (22));
    co_map. AddKeyAndValPtr ("ghi", new ct_Int (33));

    printf ("GetFirstValPtr (\"def\") = ");
    PrintValue (co_map. GetFirstValPtr ("def"));
    printf ("\n");

    PrintMap (& co_map);
    co_map. DelAllKeyAndValue ();
    }

    {
    printf ("\n");
    // Key: int, Value: ct_String
    gct_PtrMap <gct_Std32Array <gct_PtrMapEntry <gct_Key <int> > >, ct_String> co_map;
    co_map. AddKeyAndValPtr (11, new ct_String ("abc"));
    co_map. AddKeyAndValPtr (22, new ct_String ("def"));
    co_map. AddKeyAndValPtr (33, new ct_String ("ghi"));

    printf ("GetFirstValPtr (22) = ");
    PrintValue (co_map. GetFirstValPtr (22));
    printf ("\n");

    PrintMap (& co_map);
    co_map. DelAllKeyAndValue ();
    }

  printf ("\nEnd of main\n");
  return 0;
  }
