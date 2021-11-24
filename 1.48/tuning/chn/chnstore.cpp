
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
// File tuning/chn/chnstore.cpp

#include "tuning/chn/store.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

GLOBAL_STORE_DEFS (ct_ChnStore, Chn, INLINE)

//---------------------------------------------------------------------------

inline void ct_ChnStore::SetChnInfo
(
t_Position o_pos,
unsigned u_idx,
t_Size o_size
)
  {
  * (((t_ChnInfo *) o_pos) - 1) =
    u_idx + (((t_ChnInfo) o_size) << u_ChnSizeShift);
  }

//---------------------------------------------------------------------------

inline void * ct_ChnStore::ChnAlloc
(
unsigned u_idx
)
  {
  if (so_Data. aso_FreeChains [u_idx]. pv_First != 0)
    {
    void * pv = so_Data. aso_FreeChains [u_idx]. pv_First;

    TL_ASSERT (IdxOf (pv) == u_idx); // Heap corruption

    so_Data. aso_FreeChains [u_idx]. pv_First = * (void * *) pv;
    so_Data. aso_FreeChains [u_idx]. o_Count --;
    return pv;
    }
  else
    {
    so_Data. o_Entries ++;
    so_Data. o_Size += so_Data. aso_FreeChains [u_idx]. o_Length;

    return ((t_ChnInfo *)
      tl_Alloc (so_Data. aso_FreeChains [u_idx]. o_Length + o_ChnInfoSize)) + 1;
    }
  }

//---------------------------------------------------------------------------

inline void ct_ChnStore::ChnFree
(
t_Position o_pos,
unsigned u_idx
)
  {
  if (u_idx <= so_Data. u_MaxChainIdx)
    {
    * (void * *) o_pos = so_Data. aso_FreeChains [u_idx]. pv_First;
    so_Data. aso_FreeChains [u_idx]. pv_First = o_pos;
    so_Data. aso_FreeChains [u_idx]. o_Count ++;
    }
  else
    {
    tl_Free (((t_ChnInfo *) o_pos) - 1);
    so_Data. o_Entries --;
    so_Data. o_Size -= so_Data. aso_FreeChains [u_idx]. o_Length;
    }
  }

//---------------------------------------------------------------------------

ct_ChnStore::ct_ChnStore ()
  {
  TL_ASSERT (StoreInfoSize () <= 16);
  TL_ASSERT (co_Mutex. GetInitSuccess ());

  for (unsigned u_idx = 0; u_idx < u_ChnEntries; u_idx ++)
    {
    so_Data. aso_FreeChains [u_idx]. o_Length = (((t_UInt) 1) << (u_idx + 4)) - StoreInfoSize ();
    so_Data. aso_FreeChains [u_idx]. o_Count = 0;
    so_Data. aso_FreeChains [u_idx]. pv_First = 0;
    }

  so_Data. o_Entries = 0;
  so_Data. o_Size = 0;
  so_Data. b_InFree = false;
  SetMaxChainExp (22); // 4 MB
  }

//---------------------------------------------------------------------------

ct_ChnStore::~ct_ChnStore ()
  {
  FreeUnused ();
  }

//---------------------------------------------------------------------------

void ct_ChnStore::Swap
(
ct_ChnStore & co_swap
)
  {
  co_Mutex. Lock ();
  co_swap. co_Mutex. Lock ();
  tl_SwapObj (so_Data, co_swap. so_Data);
  co_swap. co_Mutex. Unlock ();
  co_Mutex. Unlock ();
  }

//---------------------------------------------------------------------------

void * ct_ChnStore::operator new
(
size_t u_size
)
  {
  return tl_Alloc (u_size);
  }

//---------------------------------------------------------------------------

void ct_ChnStore::operator delete
(
void * pv
)
  {
  tl_Free (pv);
  }

//---------------------------------------------------------------------------

void tl_OverflowHandler ();

ct_ChnStore::t_Position ct_ChnStore::Alloc
(
t_Size o_size
)
  {
  if (o_size > MaxAlloc ())
    tl_OverflowHandler ();

  if (o_size == 0)
    return 0;

  unsigned u_idx = (unsigned) -1;

  while (o_size > so_Data. aso_FreeChains [++ u_idx]. o_Length);

  co_Mutex. Lock ();
  void * pv = ChnAlloc (u_idx);
  co_Mutex. Unlock ();

  SetChnInfo (pv, u_idx, o_size);
  return pv;
  }

//---------------------------------------------------------------------------

ct_ChnStore::t_Position ct_ChnStore::Realloc
(
t_Position o_pos,
t_Size o_size
)
  {
  if (o_size > MaxAlloc ())
    tl_OverflowHandler ();

  if (o_pos == 0)
    return Alloc (o_size);

  if (o_size == 0)
    {
    Free (o_pos);
    return 0;
    }

  unsigned u_oldIdx = IdxOf (o_pos);
  unsigned u_newIdx = (unsigned) -1;

  while (o_size > so_Data. aso_FreeChains [++ u_newIdx]. o_Length);

  if (u_newIdx == u_oldIdx)
    {
    SetChnInfo (o_pos, u_oldIdx, o_size);
    return o_pos;
    }
  else
    {
    co_Mutex. Lock ();
    void * pv_new = ChnAlloc (u_newIdx);
    tl_CopyMemory ((char *) pv_new, (char *) o_pos, tl_Min (o_size, SizeOf (o_pos)));
    ChnFree (o_pos, u_oldIdx);
    co_Mutex. Unlock ();

    SetChnInfo (pv_new, u_newIdx, o_size);
    return pv_new;
    }
  }

//---------------------------------------------------------------------------

void ct_ChnStore::Free
(
t_Position o_pos
)
  {
  if (o_pos != 0)
    {
    co_Mutex. Lock ();
    ChnFree (o_pos, IdxOf (o_pos));
    co_Mutex. Unlock ();
    }
  }

//---------------------------------------------------------------------------

bool ct_ChnStore::CanFreeAll ()
  {
  return false;
  }

//---------------------------------------------------------------------------

void ct_ChnStore::FreeAll ()
  {
  TL_ASSERT (false);
  }

//---------------------------------------------------------------------------

unsigned ct_ChnStore::GetMaxChainExp ()
  {
  co_Mutex. Lock ();
  unsigned u_ret = so_Data. u_MaxChainIdx + 4u;
  co_Mutex. Unlock ();
  return u_ret;
  }

//---------------------------------------------------------------------------

void ct_ChnStore::SetMaxChainExp
(
unsigned u_exp
)
  {
  co_Mutex. Lock ();
  so_Data. u_MaxChainIdx = tl_Min (tl_Max (u_exp, 6u) - 4u, u_ChnEntries - 1u);
  co_Mutex. Unlock ();
  }

//---------------------------------------------------------------------------

t_UInt ct_ChnStore::GetEntries ()
  {
  co_Mutex. Lock ();
  t_UInt u_ret = so_Data. o_Entries;
  co_Mutex. Unlock ();
  return u_ret;
  }

//---------------------------------------------------------------------------

t_UInt ct_ChnStore::GetSize ()
  {
  co_Mutex. Lock ();
  t_UInt u_ret = so_Data. o_Size;
  co_Mutex. Unlock ();
  return u_ret;
  }

//---------------------------------------------------------------------------

t_UInt ct_ChnStore::QueryAllocEntries ()
  {
  co_Mutex. Lock ();
  t_UInt u_ret = so_Data. o_Entries - QueryFreeEntries ();
  co_Mutex. Unlock ();
  return u_ret;
  }

//---------------------------------------------------------------------------

t_UInt ct_ChnStore::QueryAllocSize ()
  {
  co_Mutex. Lock ();
  t_UInt u_ret = so_Data. o_Size - QueryFreeSize ();
  co_Mutex. Unlock ();
  return u_ret;
  }

//---------------------------------------------------------------------------

t_UInt ct_ChnStore::QueryFreeEntries ()
  {
  co_Mutex. Lock ();
  t_UInt u_ret = 0;

  for (unsigned u_idx = 0; u_idx < u_ChnEntries; u_idx ++)
    u_ret += so_Data. aso_FreeChains [u_idx]. o_Count;

  co_Mutex. Unlock ();
  return u_ret;
  }

//---------------------------------------------------------------------------

t_UInt ct_ChnStore::QueryFreeSize ()
  {
  co_Mutex. Lock ();
  t_UInt u_ret = 0;

  for (unsigned u_idx = 0; u_idx < u_ChnEntries; u_idx ++)
    u_ret += so_Data. aso_FreeChains [u_idx]. o_Count * so_Data. aso_FreeChains [u_idx]. o_Length;

  co_Mutex. Unlock ();
  return u_ret;
  }

//---------------------------------------------------------------------------

void ct_ChnStore::FreeUnused ()
  {
  void * pv;
  co_Mutex. Lock ();

  // tl_Free may call handler
  // handler may call FreeUnused
  if (! so_Data. b_InFree)
    {
    so_Data. b_InFree = true;

    for (unsigned u_idx = 0; u_idx < u_ChnEntries; u_idx ++)
      {
      so_Data. o_Entries -= so_Data. aso_FreeChains [u_idx]. o_Count;
      so_Data. o_Size -= so_Data. aso_FreeChains [u_idx]. o_Count *
        so_Data. aso_FreeChains [u_idx]. o_Length;

      while ((pv = so_Data. aso_FreeChains [u_idx]. pv_First) != 0)
        {
        so_Data. aso_FreeChains [u_idx]. pv_First = * (void * *) pv;
        so_Data. aso_FreeChains [u_idx]. o_Count --;
        tl_Free (((t_ChnInfo *) pv) - 1);
        }

      TL_ASSERT (so_Data. aso_FreeChains [u_idx]. o_Count == 0);
      }

    so_Data. b_InFree = false;
    }

  co_Mutex. Unlock ();
  tl_FreeUnused ();
  }
