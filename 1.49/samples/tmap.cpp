
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
// File samples/tmap.cpp

#include <stdio.h>
#include "tuning/map.h"
#include "tuning/string.hpp"
#include "tuning/std/array.h"
#include "tuning/std/hashtable.h"
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

template <class t_block>
  void PrintValue (const gct_HashTablePosition <t_block> *)
    {
    printf ("gct_HashTablePosition");
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
      printf ("    Key:           ");
      PrintValue (& o_key);
      printf ("  Value: ");
      PrintValue (pco_map-> GetValue (o_pos));
      printf ("\n    GetFirstValue (");
      PrintValue (& o_key);
      printf (") Value: ");
      PrintValue (pco_map-> GetFirstValue (o_key));
      printf ("\n");
      }
    }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("Begin of main\n");
  ct_Int co_int;
  ct_String co_str;

    {
    printf ("\n");

    // Key: ct_String, Value: ct_Int
    gct_Map <gct_FixItemArray <gct_MapEntry <ct_String, ct_Int>, ct_Std32Block> > co_map;
    // gct_Map <gct_Std32Array <gct_MapEntry <ct_String, ct_Int> > > co_map;
    // gct_Map <gct_Std32HashTable <gct_MapEntry <ct_String, ct_Int> > > co_map;

    co_int = 11;
    co_map. AddKeyAndValue ("abc", & co_int);
    co_int = 22;
    co_map. AddKeyAndValue ("def", & co_int);
    co_int = 33;
    co_map. AddKeyAndValue ("ghi", & co_int);

    printf ("GetFirstValue (\"def\") = ");
    PrintValue (co_map. GetFirstValue ("def"));
    printf ("\n");

    PrintMap (& co_map);
    }

    {
    printf ("\n");

    // Key: int, Value: ct_String
    gct_Map <gct_FixItemArray <gct_MapEntry <gct_Key <int>, ct_String>, ct_Std32Block> > co_map;
    // gct_Map <gct_Std32Array <gct_MapEntry <gct_Key <int>, ct_String> > > co_map;
    // gct_Map <gct_Std32HashTable <gct_MapEntry <gct_Key <int>, ct_String> > > co_map;

    co_str = "abc";
    co_map. AddKeyAndValue (11, & co_str);
    co_str = "def";
    co_map. AddKeyAndValue (22, & co_str);
    co_str = "ghi";
    co_map. AddKeyAndValue (33, & co_str);

    printf ("GetFirstValue (22) = ");
    PrintValue (co_map. GetFirstValue (22));
    printf ("\n");

    PrintMap (& co_map);
    }

  printf ("\nEnd of main\n");
  return 0;
  }
