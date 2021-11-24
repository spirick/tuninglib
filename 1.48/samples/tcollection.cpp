
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
// File samples/tcollection.cpp

#include <stdio.h>
#include "samples/int.hpp"

#include "tuning/chn/ptrdlist.h"
#include "tuning/dlist.hpp"
#include "samples/ptrid.cpp"

//---------------------------------------------------------------------------

template <class t_collection>
  void PrintCollection
  (
  t_collection * po_coll
  )
    {
    printf ("Coll:");

    for (TL_TYPENAME t_collection::t_Position o_pos = po_coll-> First ();
         o_pos != 0;
         o_pos = po_coll-> Next (o_pos))
      {
      printf (" Entry[p%d]=%d", PtrId ((void *) o_pos),
        ((ct_Int *) po_coll-> GetPtr (o_pos))-> GetValue ());
      }

    printf ("\n");
    }

//---------------------------------------------------------------------------

template <class t_collection>
  void Test ()
    {
    t_collection * po_coll = new t_collection;
    TL_TYPENAME t_collection::t_Position o_pos1;
    ct_Int * pco_int = new ct_Int (1);

    PrintCollection (po_coll);

    po_coll-> AddPtrCond (pco_int);
    po_coll-> AddPtrCond (pco_int);
    o_pos1 = po_coll-> AddPtrCond (pco_int);
    PrintCollection (po_coll);

    pco_int = new ct_Int (3);
    po_coll-> AddPtrAfter (o_pos1, pco_int);
    PrintCollection (po_coll);

    pco_int = new ct_Int (2);
    po_coll-> AddPtrAfter (o_pos1, pco_int);
    PrintCollection (po_coll);

    pco_int = new ct_Int (0);
    po_coll-> AddPtrAfter (0, pco_int);
    PrintCollection (po_coll);

    printf ("Begin 2. collection\n");
    t_collection * po_coll2 = new t_collection (* po_coll);
    PrintCollection (po_coll2);

    pco_int = new ct_Int (4);
    po_coll-> AddPtrBefore (0, pco_int);
    * po_coll2 = * po_coll;
    PrintCollection (po_coll2);

    delete po_coll2;
    printf ("End 2. collection\n");

    po_coll-> DelFirstEqualPtrAndObj (pco_int);
    PrintCollection (po_coll);

    po_coll-> DelFirstPtrAndObj ();
    PrintCollection (po_coll);

    pco_int = (ct_Int *) po_coll-> GetNthPtr (2);
    po_coll-> DelFirstEqualPtrAndObjCond (pco_int);
    PrintCollection (po_coll);

    po_coll-> DelFirstEqualPtrAndObjCond (pco_int);
    PrintCollection (po_coll);

    po_coll-> DelPtrAndObj (po_coll-> First ());
    PrintCollection (po_coll);

    po_coll-> DelAllPtrAndObj ();
    delete po_coll;
    }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("\nTest pointer container:\n\n");
  Test <gct_Chn16PtrDList <ct_Int> > ();
  DelPtrIds ();

  printf ("\nTest collection:\n\n");
  Test <ct_DList> ();
  DelPtrIds ();

  return 0;
  }
