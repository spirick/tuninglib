
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
// File samples/texception.cpp

#include <stdio.h>
#include "tuning/defs.hpp"

#if !defined TL_EXCEPTION

//---------------------------------------------------------------------------

int main ()
  {
  printf ("Exception handling is disabled\n");
  return 0;
  }

#else

#include "tuning/std/dlist.h"
#include "tuning/std/blockdlist.h"
#include "tuning/std/array.h"
#include "tuning/std/hashtable.h"
#include "samples/int.hpp"

//===========================================================================

bool b_Throw = true;

class ct_Throw
  {
  int i;

public:
  static int       i_Num;
  static int       i_Throw;

                   ct_Throw ();
                   ct_Throw (const ct_Throw &);
                   ~ct_Throw () TL_CAN_THROW;
  ct_Throw &       operator = (const ct_Throw &);

  int              GetHash () const { return i_Num; }
  };

//---------------------------------------------------------------------------

ct_Throw::ct_Throw ()
  {
  i = 0;
  printf ("%2d ct_Throw ()\n", ++ i_Num);

  if ((b_Throw) || (i_Num == i_Throw))
    {
    i_Num --;
    printf ("  throw 1\n");
    throw 1;
    }
  }

//---------------------------------------------------------------------------

ct_Throw::ct_Throw (const ct_Throw &)
  {
  i = 0;
  printf ("%2d ct_Throw (copy)\n", ++ i_Num);

  if ((b_Throw) || (i_Num == i_Throw))
    {
    i_Num --;
    printf ("  throw 2\n");
    throw 2;
    }
  }

//---------------------------------------------------------------------------

ct_Throw::~ct_Throw () TL_CAN_THROW
  {
  printf ("%2d ~ct_Throw ()\n", i_Num --);

  if ((b_Throw) || (i_Num == i_Throw))
    {
    printf ("  throw 3\n");
    throw 3;
    }
  }

//---------------------------------------------------------------------------

ct_Throw & ct_Throw::operator = (const ct_Throw &)
  {
  if ((b_Throw) || (i_Num == i_Throw))
    {
    printf ("  throw 4\n");
    throw 4;
    }

  return * this;
  }

//===========================================================================

int ct_Throw::i_Num;
int ct_Throw::i_Throw = 1000;

gct_Std32DList <ct_Throw> co_DList;
gct_Std32BlockDList <ct_Throw> co_BDList;
gct_Std32Array <ct_Throw> co_Array;
gct_Std32Array <ct_Throw> co_Array2;
gct_Std32HashTable <ct_Throw> co_HashTable;

//---------------------------------------------------------------------------

void TArrayConstructor ()
  {
  b_Throw = false;
  co_Array. AddObj ();
  co_Array. AddObj ();
  co_Array. AddObj ();
  co_Array. AddObj ();
  ct_Throw::i_Throw = ct_Throw::i_Num + 3;
  gct_Std32Array <ct_Throw> co_array2 = co_Array;
  }

//---------------------------------------------------------------------------

void TArrayDestructor ()
  {
  b_Throw = false;
  gct_Std32Array <ct_Throw> co_array;
  co_array. AddObj ();
  co_array. AddObj ();
  co_array. AddObj ();
  co_array. AddObj ();
  b_Throw = true;
  }

//---------------------------------------------------------------------------

void TDListConstructor ()
  {
  b_Throw = false;
  co_DList. AddObj ();
  co_DList. AddObj ();
  co_DList. AddObj ();
  co_DList. AddObj ();
  ct_Throw::i_Throw = ct_Throw::i_Num + 3;
  gct_Std32DList <ct_Throw> co_dlist2 = co_DList;
  }

//---------------------------------------------------------------------------

void TBDListConstructor ()
  {
  b_Throw = false;
  co_BDList. AddObj ();
  co_BDList. AddObj ();
  co_BDList. AddObj ();
  co_BDList. AddObj ();
  ct_Throw::i_Throw = ct_Throw::i_Num + 3;
  gct_Std32BlockDList <ct_Throw> co_bdlist2 = co_BDList;
  }

//---------------------------------------------------------------------------

void TDListDestructor ()
  {
  b_Throw = false;
  gct_Std32DList <ct_Throw> co_dlist;
  co_dlist. AddObj ();
  co_dlist. AddObj ();
  co_dlist. AddObj ();
  co_dlist. AddObj ();
  b_Throw = true;
  }

//---------------------------------------------------------------------------

void TBDListDestructor ()
  {
  b_Throw = false;
  gct_Std32BlockDList <ct_Throw> co_bdlist;
  co_bdlist. AddObj ();
  co_bdlist. AddObj ();
  co_bdlist. AddObj ();
  co_bdlist. AddObj ();
  b_Throw = true;
  }

//---------------------------------------------------------------------------

void THashTableConstructor ()
  {
  b_Throw = false;
    {
    ct_Throw co_throw;
    co_HashTable. AddObj (& co_throw);
    co_HashTable. AddObj (& co_throw);
    co_HashTable. AddObj (& co_throw);
    co_HashTable. AddObj (& co_throw);
    }
  ct_Throw::i_Throw = ct_Throw::i_Num + 3;
  gct_Std32HashTable <ct_Throw> co_hashTable2 = co_HashTable;
  }

//---------------------------------------------------------------------------

void THashTableDestructor ()
  {
  b_Throw = false;
  gct_Std32HashTable <ct_Throw> co_hashTable;
    {
    ct_Throw co_throw;
    co_hashTable. AddObj (& co_throw);
    co_hashTable. AddObj (& co_throw);
    co_hashTable. AddObj (& co_throw);
    co_hashTable. AddObj (& co_throw);
    }
  b_Throw = true;
  }

//---------------------------------------------------------------------------

void NewThrow ()
  {
  TL_TRY
    (
    new ct_Throw;
    )
  TL_CATCH
    (
    printf ("TL_CATCH in NewThrow\n");
    throw;
    )
  }

//---------------------------------------------------------------------------

class ct_TestBase
  {
public:
  ct_Int   co_int1;
  ct_Int   co_int2;
           ct_TestBase ()  { printf ("ct_TestBase\n"); }
           ~ct_TestBase () { printf ("~ct_TestBase\n"); }
  };

class ct_Test: public ct_TestBase
  {
  ct_Throw co_throw1;
  };
  
ct_Test * pco_Test;

//---------------------------------------------------------------------------

int main ()
  {
  try
    {
    NewThrow ();
    }
  catch (int i)
    {
    printf ("Exception %d from NewThrow ()\n", i);
    }

  printf ("--------------------\n");

  try
    {
    pco_Test = new ct_Test;
    }
  catch (int i)
    {
    printf ("Exception %d from pco_Test = new ct_Test\n", i);
    }

  printf ("--------------------\n");

/*
  // Exception in stack unwinding leads to terminate ()

  b_Throw = false;
  pco_Test = new ct_Test;
  b_Throw = true;

  try
    {
    delete pco_Test;
    }
  catch (int i)
    {
    printf ("Exception %d from delete pco_Test\n", i);
    }

  printf ("--------------------\n");
*/

  try
    {
    b_Throw = false;
    ct_Test co_Test;
    b_Throw = true;
    }
  catch (int i)
    {
    printf ("Exception %d from ~co_Test\n", i);
    }

  printf ("--------------------\n");

  try
    {
    co_Array. AddObj ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_Array. AddObj ()\n", i);
    printf ("Array length %d\n", co_Array. GetLen ());
    }

  try
    {
    co_Array. AppendObj (0, 5);
    }
  catch (int i)
    {
    printf ("Exception %d from co_Array. AppendObj (0, 5)\n", i);
    printf ("Array length %d\n", co_Array. GetLen ());
    }

  b_Throw = false;
  co_Array. AddObj ();
  b_Throw = true;

  try
    {
    co_Array2 = co_Array;
    }
  catch (int i)
    {
    printf ("Exception %d from co_Array2 = co_Array\n", i);
    printf ("Array2 length %d\n", co_Array2. GetLen ());
    }

  try
    {
    co_Array. DelFirstObj ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_Array. DelFirstObj ()\n", i);
    printf ("Array length %d\n", co_Array. GetLen ());
    }

  b_Throw = false;
  co_Array. AddObj ();
  co_Array. AddObj ();
  co_Array. AddObj ();
  co_Array. AddObj ();
  b_Throw = true;

  try
    {
    co_Array. DelAll ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_Array. DelAll ()\n", i);
    printf ("Array length %d\n", co_Array. GetLen ());
    }

  b_Throw = false;
  co_Array. AddObj ();
  co_Array. AddObj ();
  co_Array. AddObj ();
  co_Array. AddObj ();
  b_Throw = true;

  try
    {
    co_Array. TruncateObj (2);
    }
  catch (int i)
    {
    printf ("Exception %d from co_Array. TruncateObj (2)\n", i);
    printf ("Array length %d\n", co_Array. GetLen ());
    }

  b_Throw = false;
  co_Array. DelAll ();
  co_Array2. DelAll ();
  b_Throw = true;

  printf ("--------------------\n");

  try
    {
    co_DList. AddObj ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_DList. AddObj ()\n", i);
    printf ("DList length %d\n", co_DList. GetLen ());
    }

  b_Throw = false;
  co_DList. AddObj ();
  b_Throw = true;

  try
    {
    co_DList. DelFirstObj ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_DList. DelFirstObj ()\n", i);
    printf ("DList length %d\n", co_DList. GetLen ());
    }

  b_Throw = false;
  co_DList. AddObj ();
  co_DList. AddObj ();
  b_Throw = true;

  try
    {
    co_DList. DelAll ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_DList. DelAll ()\n", i);
    printf ("DList length %d\n", co_DList. GetLen ());
    }

  b_Throw = false;
  co_DList. DelAll ();
  b_Throw = true;

  printf ("--------------------\n");

  try
    {
    co_BDList. AddObj ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_BDList. AddObj ()\n", i);
    printf ("BDList length %d\n", co_BDList. GetLen ());
    }

  b_Throw = false;
  co_BDList. AddObj ();
  b_Throw = true;

  try
    {
    co_BDList. DelFirstObj ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_BDList. DelFirstObj ()\n", i);
    printf ("BDList length %d\n", co_BDList. GetLen ());
    }

  b_Throw = false;
  co_BDList. AddObj ();
  co_BDList. AddObj ();
  b_Throw = true;

  try
    {
    co_BDList. DelAll ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_BDList. DelAll ()\n", i);
    printf ("BDList length %d\n", co_BDList. GetLen ());
    }

  b_Throw = false;
  co_BDList. DelAll ();
  b_Throw = true;

  printf ("--------------------\n");

  b_Throw = false;
  ct_Throw co_throw;
  b_Throw = true;

  try
    {
    co_HashTable. AddObj (& co_throw);
    }
  catch (int i)
    {
    printf ("Exception %d from co_HashTable. AddObj ()\n", i);
    printf ("HashTable length %d\n", co_HashTable. GetLen ());
    }

  b_Throw = false;
  co_HashTable. AddObj (& co_throw);
  co_HashTable. AddObj (& co_throw);
  co_HashTable. AddObj (& co_throw);
  co_HashTable. AddObj (& co_throw);
  b_Throw = true;

  try
    {
    co_HashTable. DelAll ();
    }
  catch (int i)
    {
    printf ("Exception %d from co_HashTable. DelAll ()\n", i);
    printf ("HashTable length %d\n", co_HashTable. GetLen ());
    }

  b_Throw = false;
  co_HashTable. DelAll ();
  b_Throw = true;

  printf ("--------------------\n");

  try
    {
    TArrayConstructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from TArrayConstructor ()\n", i);
    ct_Throw::i_Throw = 1000;
    co_Array. DelAll ();
    }

  printf ("--------------------\n");

  try
    {
    TArrayDestructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from TArrayDestructor ()\n", i);
    }

  printf ("--------------------\n");

  try
    {
    TDListConstructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from TDListConstructor ()\n", i);
    ct_Throw::i_Throw = 1000;
    co_DList. DelAll ();
    }

  printf ("--------------------\n");

  try
    {
    TBDListConstructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from TBDListConstructor ()\n", i);
    ct_Throw::i_Throw = 1000;
    co_BDList. DelAll ();
    }

  printf ("--------------------\n");

  try
    {
    TDListDestructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from TDListDestructor ()\n", i);
    }

  printf ("--------------------\n");

  try
    {
    TBDListDestructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from TBDListDestructor ()\n", i);
    }

  printf ("--------------------\n");

  try
    {
    THashTableConstructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from THashTableConstructor ()\n", i);
    ct_Throw::i_Throw = 1000;
    co_HashTable. DelAll ();
    }

  printf ("--------------------\n");

  try
    {
    THashTableDestructor ();
    }
  catch (int i)
    {
    printf ("Exception %d from THashTableDestructor ()\n", i);
    }

  printf ("--------------------\n");
  b_Throw = false;
  return 0;
  }

#endif
