
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
// File tuning/sys/calloc.hpp

#ifndef TUNING_SYS_CALLOC_HPP
#define TUNING_SYS_CALLOC_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------
// Reserve memory

typedef void (* tpf_AllocHandler) ();

tpf_AllocHandler TL_EXPORT
                     tl_SetReserveHandler (tpf_AllocHandler pf_allocHandler);
tpf_AllocHandler TL_EXPORT
                     tl_SetOverflowHandler (tpf_AllocHandler pf_allocHandler);

void TL_EXPORT       tl_SetReserveSize (t_UInt u_resSize);
t_UInt TL_EXPORT     tl_GetReserveSize ();
bool TL_EXPORT       tl_HasReserve ();
void TL_EXPORT       tl_FreeReserve ();
void TL_EXPORT       tl_AllocReserve ();

//---------------------------------------------------------------------------
// Allocation

t_UInt TL_EXPORT     tl_StoreInfoSize ();
t_UInt TL_EXPORT     tl_MaxAlloc ();

t_VoidPtr TL_EXPORT  tl_Alloc (t_UInt u_size) TL_NO_THROW;
t_VoidPtr TL_EXPORT  tl_Realloc (void * pv_ptr, t_UInt u_size) TL_NO_THROW;
void TL_EXPORT       tl_Free (void * pv_ptr) TL_NO_THROW;

//---------------------------------------------------------------------------
// Heap

struct st_HeapInfo
  {
  t_UInt               u_AllocEntries;
  t_UInt               u_FreeEntries;
  t_UInt               u_AllocSize;
  t_UInt               u_FreeSize;
  t_UInt               u_HeapSize;
  };

bool TL_EXPORT       tl_QueryHeapInfo (st_HeapInfo * pso_info);
bool TL_EXPORT       tl_FreeUnused ();

//---------------------------------------------------------------------------

inline t_UInt TL_EXPORT tl_StoreInfoSize ()
  {
  return sizeof (t_UInt);
  }

//---------------------------------------------------------------------------

inline t_UInt TL_EXPORT tl_MaxAlloc ()
  {
  return ((t_UInt) -1) - tl_StoreInfoSize ();
  }

#endif
