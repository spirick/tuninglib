
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
// File samples/tcontainer.cpp

#include <stdio.h>
#include "samples/int.hpp"
#include "tuning/pageblock.hpp"
#include "tuning/compcontainer.h"
#include "tuning/ptrcompcontainer.h"

#include "tuning/std/array.h"
#include "tuning/rnd/blockdlist.h"
#include "tuning/chn/refdlist.h"
#include "tuning/chn/block.h"

#include "tuning/std/ptrarray.h"
#include "tuning/rnd/blockptrdlist.h"
#include "tuning/chn/refptrdlist.h"

#include "tuning/chn/sortedarray.h"
#include "samples/ptrid.cpp"

//---------------------------------------------------------------------------

void PrintPos (t_UInt u)
  {
  printf ("%" TL_FMT_UINT "d", u);
  }

void PrintPos (void * p)
  {
  printf ("p%d", PtrId (p));
  }

//---------------------------------------------------------------------------

template <class t_container>
  void PrintContainer
  (
  t_container * po_cont
  )
    {
    printf ("Container:");

    for (TL_TYPENAME t_container::t_Position o_pos = po_cont-> First ();
         o_pos != 0;
         o_pos = po_cont-> Next (o_pos))
      {
      printf (" Entry[");
      PrintPos (o_pos);
      printf ("]=%d", po_cont-> GetObj (o_pos)-> GetValue ());
      }

    printf ("\n");
    }

//---------------------------------------------------------------------------

template <class t_container>
  void PrintPtrContainer
  (
  t_container * po_cont
  )
    {
    printf ("Container:");

    for (TL_TYPENAME t_container::t_Position o_pos = po_cont-> First ();
         o_pos != 0;
         o_pos = po_cont-> Next (o_pos))
      {
      printf (" Entry[");
      PrintPos (o_pos);
      printf ("]=%d", po_cont-> GetPtr (o_pos)-> GetValue ());
      }

    printf ("\n");
    }

//---------------------------------------------------------------------------

template <class t_container>
  void TestCont ()
    {
    gct_CompContainer <t_container> * po_cont =
      new gct_CompContainer <t_container>;
    TL_TYPENAME t_container::t_Position o_pos1;
    ct_Int co_int (1);

    PrintContainer (po_cont);

    po_cont-> AddObjCond (& co_int);
    po_cont-> AddObjCond (& co_int);
    o_pos1 = po_cont-> AddObjCond (& co_int);
    PrintContainer (po_cont);

    co_int = 3;
    po_cont-> AddObjAfter (o_pos1, & co_int);
    PrintContainer (po_cont);

    co_int = 2;
    po_cont-> AddObjAfter (o_pos1, & co_int);
    PrintContainer (po_cont);

    co_int = 0;
    po_cont-> AddObjAfter (0, & co_int);
    PrintContainer (po_cont);

    printf ("Begin 2. container\n");
    gct_CompContainer <t_container> * po_cont2 =
      new gct_CompContainer <t_container> (* po_cont);
    PrintContainer (po_cont2);

    po_cont-> Swap (* po_cont2);
    PrintContainer (po_cont2);

    po_cont-> AddObjAfter (0, & co_int);
    * po_cont = * po_cont2;
    PrintContainer (po_cont);

    delete po_cont2;
    printf ("End 2. container\n");

    po_cont-> DelFirstObj ();
    PrintContainer (po_cont);

    co_int = 2;
    po_cont-> DelFirstEqualObjCond (& co_int);
    PrintContainer (po_cont);

    po_cont-> DelFirstEqualObjCond (& co_int);
    PrintContainer (po_cont);

    po_cont-> DelObj (po_cont-> First ());
    PrintContainer (po_cont);
    delete po_cont;
    }

//---------------------------------------------------------------------------

template <class t_container>
  void TestPtrCont ()
    {
    ct_Int * pco_int0  = new ct_Int (0);
    ct_Int * pco_int10 = new ct_Int (1);
    ct_Int * pco_int11 = new ct_Int (1);
    ct_Int * pco_int12 = new ct_Int (1);
    ct_Int * pco_int20 = new ct_Int (2);
    ct_Int * pco_int21 = new ct_Int (2);
    ct_Int * pco_int3  = new ct_Int (3);
    gct_PtrCompContainer <t_container> * po_cont =
      new gct_PtrCompContainer <t_container>;
    TL_TYPENAME t_container::t_Position o_pos1;

    PrintPtrContainer (po_cont);

    po_cont-> AddRefCond (pco_int10);
    po_cont-> AddRefCond (pco_int11);
    o_pos1 = po_cont-> AddRefCond (pco_int12);
    PrintPtrContainer (po_cont);

    po_cont-> AddPtrAfter (o_pos1, pco_int3);
    PrintPtrContainer (po_cont);

    po_cont-> AddPtrAfter (o_pos1, pco_int20);
    PrintPtrContainer (po_cont);

    po_cont-> AddPtrAfter (0, pco_int0);
    PrintPtrContainer (po_cont);

    printf ("Begin 2. container\n");
    gct_PtrCompContainer <t_container> * po_cont2 =
      new gct_PtrCompContainer <t_container> (* po_cont);
    PrintPtrContainer (po_cont2);

    po_cont-> AddPtrAfter (0, pco_int0);
    * po_cont = * po_cont2;
    PrintPtrContainer (po_cont);

    delete po_cont2;
    printf ("End 2. container\n");

    po_cont-> DelFirstPtr ();
    PrintPtrContainer (po_cont);

    po_cont-> DelFirstEqualRefCond (pco_int21);
    PrintPtrContainer (po_cont);

    po_cont-> DelFirstEqualRefCond (pco_int21);
    PrintPtrContainer (po_cont);

    po_cont-> DelPtr (po_cont-> First ());
    PrintPtrContainer (po_cont);

    delete po_cont;
    delete pco_int0;
    delete pco_int10;
    delete pco_int11;
    delete pco_int12;
    delete pco_int20;
    delete pco_int21;
    delete pco_int3;
    }

//---------------------------------------------------------------------------

void CheckBefore
(
const gct_Chn_SortedArray <int> & co_sCont,
int i_test,
t_UInt u_pos
)
  {
  if (co_sCont. Before (& i_test) != u_pos)
    printf ("* * * Error %d %d", i_test, (int) u_pos);
  }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("\nTestCont Std8Array:\n\n");
  TestCont <gct_Std8Array <ct_Int> > ();
  printf ("\nTestCont Rnd16BlockDList:\n\n");
  TestCont <gct_Rnd16BlockDList <ct_Int> > ();
  printf ("\nTestCont Chn32RefDList:\n\n");
  TestCont <gct_Chn32RefDList <ct_Int> > ();
  DelPtrIds ();

  printf ("\nTestCont gct_Array <ct_Int, ct_PageBlock>:\n\n");
  TestCont <gct_ExtContainer <gct_Array <ct_Int, ct_PageBlock> > > ();
  printf ("\nTestCont gct_DList <ct_Int, gct_BlockStore <ct_PageBlock, gct_CharBlock <ct_Chn_Block, char> > >:\n\n");
  TestCont <gct_ExtContainer <gct_DList <ct_Int, gct_BlockStore <ct_PageBlock, gct_CharBlock <ct_Chn_Block, char> > > > > ();

  printf ("\nTestPtrCont Std8PtrArray:\n\n");
  TestPtrCont <gct_Std8PtrArray <ct_Int> > ();
  printf ("\nTestPtrCont Rnd16BlockPtrDList:\n\n");
  TestPtrCont <gct_Rnd16BlockPtrDList <ct_Int> > ();
  printf ("\nTestPtrCont Chn32RefPtrDList:\n\n");
  TestPtrCont <gct_Chn32RefPtrDList <ct_Int> > ();
  DelPtrIds ();

  gct_Chn_SortedArray <int> co_sCont;
  int i = 1;
  co_sCont. AddObj (& i);
  i = 3;
  co_sCont. AddObj (& i);
  co_sCont. AddObj (& i);
  i = 5;
  co_sCont. AddObj (& i);

  CheckBefore (co_sCont, 0, 0);
  CheckBefore (co_sCont, 1, 1);
  CheckBefore (co_sCont, 2, 1);
  CheckBefore (co_sCont, 3, 3);
  CheckBefore (co_sCont, 4, 3);
  CheckBefore (co_sCont, 5, 4);
  CheckBefore (co_sCont, 6, 4);

  return 0;
  }
