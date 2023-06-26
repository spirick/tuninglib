
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
// File samples/ptrid.cpp

#include "tuning/compcontainer.h"
#include "tuning/std/sortedarray.h"
#include "tuning/sys/calloc.hpp"

//---------------------------------------------------------------------------

struct st_PtrEntry
  {
  void *               pv_Value;
  unsigned             u_Id;

                       st_PtrEntry ()
                         { pv_Value = 0; u_Id = 0; }
  bool                 operator == (const st_PtrEntry & co_comp) const
                         { return pv_Value == co_comp. pv_Value; }
  bool                 operator < (const st_PtrEntry & co_comp) const
                         { return pv_Value < co_comp. pv_Value; }
  };

typedef gct_CompContainer <gct_Std32SortedArray <st_PtrEntry> > t_PtrIds;
static t_PtrIds co_PtrIds;
static unsigned u_PtrId;

//---------------------------------------------------------------------------

// Convert "void *" to Integer-Id
unsigned PtrId
(
void * pv
)
  {
  if (pv == 0)
    return 0;

  st_PtrEntry so_new;
  so_new. pv_Value = pv;
  st_PtrEntry * pso_found = co_PtrIds. GetObj (co_PtrIds. AddObjCond (& so_new));

  if (pso_found-> u_Id == 0)
    pso_found-> u_Id = ++ u_PtrId;

  return pso_found-> u_Id;
  }

//---------------------------------------------------------------------------

void DelPtrId
(
void * pv
)
  {
  if (pv == 0)
    return;

  st_PtrEntry so_find;
  so_find. pv_Value = pv;
  t_PtrIds::t_Position o_found = co_PtrIds. SearchFirstObj (& so_find);
  TL_ASSERT (o_found != 0);
  co_PtrIds. DelObj (o_found);
  }

//---------------------------------------------------------------------------

void DelPtrIds ()
  {
  co_PtrIds. DelAll ();
  u_PtrId = 0;
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class ct_TestStore
  {
public:
  typedef t_UInt       t_Size;
  //typedef t_UInt16     t_Size;
  typedef void *       t_Position;
  typedef ct_TestStore t_Store;

  static t_UInt        StoreInfoSize () { return tl_StoreInfoSize (); }
  static t_UInt        MaxAlloc () { return tl_MaxAlloc (); }

  static t_Position    Alloc (t_Size o_size);
  static t_Position    Realloc (t_Position o_pos, t_Size o_size);
  static void          Free (t_Position o_pos);

  static void *        AddrOf (t_Position o_pos) { return o_pos; }
  static t_Position    PosOf (void * pv_adr) { return pv_adr; }

  static ct_TestStore * GetStore () { return 0; }
  };

//---------------------------------------------------------------------------

ct_TestStore::t_Position ct_TestStore::Alloc
(
t_Size o_size
)
  {
  void * pv_res = 0;

  if (o_size != 0)
    pv_res = tl_Alloc (o_size);

  printf ("* Alloc  : %u -> p%d\n", (unsigned) o_size, PtrId (pv_res));
  return pv_res;
  }

//---------------------------------------------------------------------------

ct_TestStore::t_Position ct_TestStore::Realloc
(
t_Position o_pos,
t_Size o_size
)
  {
  printf ("* Realloc: p%d %u", PtrId (o_pos), (unsigned) o_size);
  void * pv_res = 0;

  if (o_pos != 0)
    {
    if (o_size != 0)
      pv_res = tl_Realloc (o_pos, o_size);
    else
      tl_Free (o_pos);

    DelPtrId (o_pos);
    }
  else
    {
    if (o_size != 0)
      pv_res = tl_Alloc (o_size);
    }

  printf (" -> p%d\n", PtrId (pv_res));
  return pv_res;
  }

//---------------------------------------------------------------------------

void ct_TestStore::Free
(
t_Position o_pos
)
  {
  printf ("* Free   : p%d\n", PtrId (o_pos));

  if (o_pos != 0)
    {
    tl_Free (o_pos);
    DelPtrId (o_pos);
    }
  }
