
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
// File tuning/stringsort.hpp

#ifndef TUNING_STRINGSORT_HPP
#define TUNING_STRINGSORT_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_StringSort
  {
  class TL_EXPORT ct_SortArray
    {
  public:
    t_Int              ai_SortArray [256];
    ct_SortArray *     pco_Next;

    inline             ct_SortArray ();
    inline             ~ct_SortArray ();

    static void *      operator new (size_t u_size);
    static void        operator delete (void * pv);
    };

  // class ct_StringSort
  t_Int *              pi_Temp;
  const unsigned char * * ppc_Strings;
  t_Int *              pi_Result;
  const unsigned char * pc_SortPage;

  static const char *  GetDefaultSortPage ();
  bool                 SubSort (ct_SortArray * pco_sortArray, t_Int i_first,
                         t_Int i_idx);

public:
  bool                 Sort (const char * * ppc_strings, t_Int * pi_sortedIndex,
                         t_Int i_numOfStrings, const char * pc_sortPage =
                         GetDefaultSortPage ());
  };

//---------------------------------------------------------------------------

class TL_EXPORT ct_UInt32Sort
  {
  class TL_EXPORT ct_SortArray
    {
  public:
    t_Int              ai_SortArray [256];
    ct_SortArray *     pco_Next;

    inline             ct_SortArray ();
    };

  // class ct_UInt32Sort
  typedef unsigned char t_Char4 [4];
  t_Int *              pi_Temp;
  const t_Char4 *      pu_Ints;
  t_Int *              pi_Result;

  void                 SubSort (ct_SortArray * pco_sortArray, t_Int i_first,
                         t_Int i_idx);

public:
  bool                 Sort (const t_UInt32 * pu_ints, t_Int * pi_sortedIndex,
                         t_Int i_numOfInts);
  };

#endif
