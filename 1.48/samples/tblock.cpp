
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
// File samples/tblock.cpp

#include <stdio.h>
#include "tuning/block.h"
#include "tuning/array.h"
#include "tuning/resblock.h"
#include "tuning/charblock.h"
#include "tuning/chn/block.h"
#include "tuning/std/store.hpp"
#include "samples/ptrid.cpp"

#ifdef TL_64

//---------------------------------------------------------------------------

class ct_MyBlockBase
  {
public:
  typedef ct_Std32Store t_StaticStore;
  typedef t_StaticStore::t_Size t_Size;

protected:
  t_StaticStore::t_Position o_Pos;
  t_Size               o_Size;

public:
  t_UInt32             u_MyData;

  void                 Swap (ct_MyBlockBase & co_swap);
  inline t_StaticStore::t_Store * GetStore () const
    { return t_StaticStore::GetStore (); }
  };

//---------------------------------------------------------------------------

void ct_MyBlockBase::Swap
(
ct_MyBlockBase & co_swap
)
  {
  tl_SwapObj (* this, co_swap);
  }

//---------------------------------------------------------------------------

struct st_Test1
  {
  gct_FixItemArray <int, gct_Block <gct_BlockBase <ct_Std32Store, ct_Empty> > > co_MyArray;
  t_UInt32             u_MyData;

  inline t_UInt32 &    GetMyData ()
    { return u_MyData; }
  };

//---------------------------------------------------------------------------

struct st_Test2
  {
  gct_FixItemArray <int, gct_Block <ct_MyBlockBase> > co_MyArray;

  inline t_UInt32 &    GetMyData ()
    { return co_MyArray. u_MyData; }
  };

#else

//---------------------------------------------------------------------------

class ct_MyBlockBase
  {
public:
  typedef ct_Std16Store t_StaticStore;
  typedef t_StaticStore::t_Size t_Size;

protected:
  t_StaticStore::t_Position o_Pos;
  t_Size               o_Size;

public:
  t_UInt16             u_MyData;

  void                 Swap (ct_MyBlockBase & co_swap);
  inline t_StaticStore::t_Store * GetStore () const
    { return t_StaticStore::GetStore (); }
  };

//---------------------------------------------------------------------------

void ct_MyBlockBase::Swap
(
ct_MyBlockBase & co_swap
)
  {
  tl_SwapObj (* this, co_swap);
  }

//---------------------------------------------------------------------------

struct st_Test1
  {
  gct_FixItemArray <int, gct_Block <gct_BlockBase <ct_Std16Store, ct_Empty> > > co_MyArray;
  t_UInt16             u_MyData;

  inline t_UInt16 &    GetMyData ()
    { return u_MyData; }
  };

//---------------------------------------------------------------------------

struct st_Test2
  {
  gct_FixItemArray <int, gct_Block <ct_MyBlockBase> > co_MyArray;

  inline t_UInt16 &    GetMyData ()
    { return co_MyArray. u_MyData; }
  };

#endif

//---------------------------------------------------------------------------

typedef gct_CharBlock <ct_Chn_Block, char> t_CharBlock;
typedef gct_CharBlock <ct_Chn_Block, wchar_t> t_WCharBlock;

//---------------------------------------------------------------------------

void PrintStr
(
const char * pc,
t_UInt u_len
)
  {
  while (u_len -- != 0)
    printf ("%c", * (pc ++));
  }

//---------------------------------------------------------------------------

void PrintStr
(
const wchar_t * pc,
t_UInt u_len
)
  {
  while (u_len -- != 0)
    printf ("%lc", * (pc ++));
  }

//---------------------------------------------------------------------------

void PrintObj
(
const t_CharBlock & co_blk
)
  {
  PrintStr (co_blk. GetRawAddr (), co_blk. GetCharSize ());
  printf ("\n");
  }

//---------------------------------------------------------------------------

void PrintObj
(
const t_WCharBlock & co_wblk
)
  {
  PrintStr (co_wblk. GetRawAddr (), co_wblk. GetCharSize ());
  printf ("\n");
  }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("sizeof (st_Test1) = %d\n", (unsigned) sizeof (st_Test1));
  printf ("sizeof (st_Test2) = %d\n", (unsigned) sizeof (st_Test2));

  printf ("\nTest ResBlock:\n\n");

  {
    gct_EmptyBaseResBlock <ct_TestStore> o_testBlock;
    o_testBlock. SetByteSize (5);
    printf ("ResBlock::SetByteSize (5)\n");
    o_testBlock. SetMinByteSize (3);
    printf ("ResBlock::SetMinByteSize (3)\n");
    o_testBlock. SetMinByteSize (10);
    printf ("ResBlock::SetMinByteSize (10)\n");
    o_testBlock. SetByteSize (9);
    printf ("ResBlock::SetByteSize (9)\n");
    o_testBlock. SetByteSize (12);
    printf ("ResBlock::SetByteSize (12)\n");
    o_testBlock. SetByteSize (9);
    printf ("ResBlock::SetByteSize (9)\n");
    o_testBlock. SetMinByteSize (8);
    printf ("ResBlock::SetMinByteSize (8)\n");
  }

  printf ("~ResBlock\n\n");

  printf ("Test CharBlock:\n\n");

  t_CharBlock co1;
  t_WCharBlock wco1;

  co1. AssignChars ("a", 1);
  PrintObj (co1);

  wco1. AssignChars (L"a", 1);
  PrintObj (wco1);
  printf ("a\n\n");

  co1. AppendChars ("bc", 2);
  PrintObj (co1);

  wco1. AppendChars (L"bc", 2);
  PrintObj (wco1);
  printf ("abc\n\n");

  co1. AppendChars (0, 0);
  wco1. AppendChars (0, 0);
  co1. AssignChars (0, 0);
  wco1. AssignChars (0, 0);

  co1. AppendChars ("0123456789", 10);
  PrintObj (co1);

  wco1. AppendChars (L"0123456789", 10);
  PrintObj (wco1);
  printf ("0123456789\n\n");

  co1. ReplaceChars (5, 2, "abc", 3);
  PrintObj (co1);

  wco1. ReplaceChars (5, 2, L"abc", 3);
  PrintObj (wco1);
  printf ("01234abc789\n\n");

  co1. ReplaceChars (5, 3, "56", 2);
  PrintObj (co1);

  wco1. ReplaceChars (5, 3, L"56", 2);
  PrintObj (wco1);
  printf ("0123456789\n\n");

  co1. InsertChars (5, "abc", 3);
  PrintObj (co1);

  wco1. InsertChars (5, L"abc", 3);
  PrintObj (wco1);
  printf ("01234abc56789\n\n");

  co1. ReplaceChars (5, 3, "...", 3);
  PrintObj (co1);

  wco1. ReplaceChars (5, 3, L"...", 3);
  PrintObj (wco1);
  printf ("01234...56789\n\n");

  co1. DeleteChars (5, 3);
  PrintObj (co1);

  wco1. DeleteChars (5, 3);
  PrintObj (wco1);
  printf ("0123456789\n\n");

  return 0;
  }
