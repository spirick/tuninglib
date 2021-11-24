
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
// File tuning/rnd/rndstore.cpp

#include "tuning/rnd/store.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

GLOBAL_STORE_DEFS (ct_RndStore, Rnd, INLINE)

//---------------------------------------------------------------------------

ct_RndStore::t_Size ct_RndStore::Round
(
t_Size o_size
)
  {
  if (o_size != 0)
    {
    t_Size o_ret;
    o_size += tl_StoreInfoSize () - 1;
    co_Mutex. Lock ();

    if (o_size < so_Data. o_MinSize)
      o_ret = so_Data. o_MinSize - tl_StoreInfoSize ();
    else
      {
      t_Size o_stepWidth = so_Data. o_MinSize;
      t_Size o_shiftedSize = o_size >> so_Data. u_StepDivLog;

      while (o_stepWidth <= o_shiftedSize)
        o_stepWidth <<= 1;

      o_ret = (o_size & (0 - o_stepWidth)) + o_stepWidth - tl_StoreInfoSize ();
      }

    co_Mutex. Unlock ();
    return o_ret;
    }
  else
    return 0;
  }

//---------------------------------------------------------------------------

ct_RndStore::ct_RndStore ()
  {
  TL_ASSERT (co_Mutex. GetInitSuccess ());
  so_Data. o_MinSize = 16;
  SetStepDiv (1);
  }

//---------------------------------------------------------------------------

void ct_RndStore::Swap
(
ct_RndStore & co_swap
)
  {
  co_Mutex. Lock ();
  co_swap. co_Mutex. Lock ();
  tl_SwapObj (so_Data, co_swap. so_Data);
  co_swap. co_Mutex. Unlock ();
  co_Mutex. Unlock ();
  }

//---------------------------------------------------------------------------

void * ct_RndStore::operator new
(
size_t u_size
)
  {
  return tl_Alloc (u_size);
  }

//---------------------------------------------------------------------------

void ct_RndStore::operator delete
(
void * pv
)
  {
  tl_Free (pv);
  }

//---------------------------------------------------------------------------

void ct_RndStore::SetMinSize
(
t_Size o_minSize
)
  {
  // o_minSize must be a power of two
  TL_ASSERT (o_minSize != 0);
  TL_ASSERT ((o_minSize & (0 - o_minSize)) == o_minSize);

  co_Mutex. Lock ();
  so_Data. o_MinSize = o_minSize;

  if (so_Data. o_StepDiv > so_Data. o_MinSize)
    SetStepDiv (so_Data. o_MinSize);

  co_Mutex. Unlock ();
  }

//---------------------------------------------------------------------------

void ct_RndStore::SetStepDiv
(
t_Size o_stepDiv
)
  {
  // o_stepDiv must be a power of two
  TL_ASSERT (o_stepDiv != 0);
  TL_ASSERT ((o_stepDiv & (0 - o_stepDiv)) == o_stepDiv);

  co_Mutex. Lock ();
  so_Data. o_StepDiv = o_stepDiv;

  if (so_Data. o_StepDiv > so_Data. o_MinSize)
    so_Data. o_StepDiv = so_Data. o_MinSize;

  t_Size u = so_Data. o_StepDiv;
  so_Data. u_StepDivLog = 0;

  while (u > 0)
    {
    u >>= 1;
    so_Data. u_StepDivLog ++;
    }

  co_Mutex. Unlock ();
  }
