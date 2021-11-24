
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
// File tuning/sys/calloc.cpp

#include "tuning/sys/calloc.hpp"

//===========================================================================
// Reserve memory
//===========================================================================

#include "tuning/sys/cprocess.hpp"
#include <stdlib.h>

//---------------------------------------------------------------------------

static tpf_AllocHandler pf_ReserveHandler;
static tpf_AllocHandler pf_OverflowHandler;
static t_UInt           u_ReserveSize;
static void *           pv_ReservePtr;

//---------------------------------------------------------------------------

tpf_AllocHandler TL_EXPORT tl_SetReserveHandler
(
tpf_AllocHandler pf_allocHandler
)
  {
  tl_EnterCriticalSection ();
  tpf_AllocHandler pf_oldReserveHandler = pf_ReserveHandler;
  pf_ReserveHandler = pf_allocHandler;
  tl_LeaveCriticalSection ();
  return pf_oldReserveHandler;
  }

//---------------------------------------------------------------------------

tpf_AllocHandler TL_EXPORT tl_SetOverflowHandler
(
tpf_AllocHandler pf_allocHandler
)
  {
  tl_EnterCriticalSection ();
  tpf_AllocHandler pf_oldOverflowHandler = pf_OverflowHandler;
  pf_OverflowHandler = pf_allocHandler;
  tl_LeaveCriticalSection ();
  return pf_oldOverflowHandler;
  }

//---------------------------------------------------------------------------

void tl_OverflowHandler ()
  {
  tl_EnterCriticalSection ();
  tpf_AllocHandler pf_overflowHandler = pf_OverflowHandler;
  tl_LeaveCriticalSection ();

  if (pf_overflowHandler != 0)
    (* pf_overflowHandler) ();

  tl_EndProcess (1); // Out of memory
  }

//---------------------------------------------------------------------------

static void tl_SetResSize
(
bool b_free,
bool b_newSize,
t_UInt u_newSize
)
  {
  tl_EnterCriticalSection ();

  t_UInt u_oldReserveSize = u_ReserveSize;
  void * pv_oldReservePtr = pv_ReservePtr;

  if (b_newSize)
    u_ReserveSize = u_newSize;

  if ((u_ReserveSize == 0) || b_free)
    {
    free (pv_ReservePtr);
    pv_ReservePtr = 0;
    }
  else
    {
    void * pv_newPtr = realloc (pv_ReservePtr, u_ReserveSize);

    if (pv_newPtr != 0)
      pv_ReservePtr = pv_newPtr;
    }

  tpf_AllocHandler pf_reserveHandler = 0;

  if ((u_oldReserveSize != u_ReserveSize) ||
      (pv_oldReservePtr != pv_ReservePtr))
    pf_reserveHandler = pf_ReserveHandler;

  tl_LeaveCriticalSection ();

  if (pf_reserveHandler != 0)
    (* pf_reserveHandler) ();
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_SetReserveSize
(
t_UInt u_resSize
)
  {
  tl_SetResSize (false, true, u_resSize);
  }

//---------------------------------------------------------------------------

t_UInt TL_EXPORT tl_GetReserveSize ()
  {
  tl_EnterCriticalSection ();
  t_UInt u_ret = u_ReserveSize;
  tl_LeaveCriticalSection ();
  return u_ret;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_HasReserve ()
  {
  return pv_ReservePtr != 0;
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_FreeReserve ()
  {
  tl_SetResSize (true, false, 0);
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_AllocReserve ()
  {
  tl_SetResSize (false, false, 0);
  }

//===========================================================================
// Allocation
//===========================================================================

t_VoidPtr TL_EXPORT tl_Alloc
(
t_UInt u_size
) TL_NO_THROW
  {
  if (u_size == 0)
    return 0;

  void * pv = malloc (u_size);

  if (pv == 0)
    {
    tl_FreeReserve ();
    pv = malloc (u_size);

    if (pv == 0)
      {
      tl_EnterCriticalSection ();
      tpf_AllocHandler pf_overflowHandler = pf_OverflowHandler;
      tl_LeaveCriticalSection ();

      if (pf_overflowHandler != 0)
        {
        (* pf_overflowHandler) ();
        pv = malloc (u_size);
        }

      if (pv == 0)
        tl_EndProcess (1); // Out of memory
      }
    }

  return pv;
  }

//---------------------------------------------------------------------------

t_VoidPtr TL_EXPORT tl_Realloc
(
void * pv_ptr,
t_UInt u_size
) TL_NO_THROW
  {
  if (pv_ptr == 0)
    return tl_Alloc (u_size);

  if (u_size == 0)
    {
    tl_Free (pv_ptr);
    return 0;
    }

  void * pv_newPtr = realloc (pv_ptr, u_size);

  if (pv_newPtr == 0)
    {
    tl_FreeReserve ();
    pv_newPtr = realloc (pv_ptr, u_size);

    if (pv_newPtr == 0)
      {
      tl_EnterCriticalSection ();
      tpf_AllocHandler pf_overflowHandler = pf_OverflowHandler;
      tl_LeaveCriticalSection ();

      if (pf_overflowHandler != 0)
        {
        (* pf_overflowHandler) ();
        pv_newPtr = realloc (pv_ptr, u_size);
        }

      if (pv_newPtr == 0)
        tl_EndProcess (1); // Out of memory
      }
    }

  return pv_newPtr;
  }

//---------------------------------------------------------------------------

void TL_EXPORT tl_Free
(
void * pv_ptr
) TL_NO_THROW
  {
  if (pv_ptr != 0)
    {
    free (pv_ptr);

    if ((pv_ReservePtr == 0) && (u_ReserveSize != 0))
      tl_AllocReserve ();
    }
  }

//===========================================================================
// Heap
//===========================================================================

#if defined TL_MICROSOFT

#include <crtdbg.h>

bool TL_EXPORT tl_QueryHeapInfo
(
st_HeapInfo * pso_info
)
  {
  #if defined TL_DEBUG

    _CrtMemState so_state;
    _CrtMemCheckpoint (& so_state);
    pso_info-> u_AllocEntries = so_state. lCounts [_NORMAL_BLOCK];
    pso_info-> u_FreeEntries  = so_state. lCounts [_FREE_BLOCK];
    pso_info-> u_AllocSize    = so_state. lSizes [_NORMAL_BLOCK];
    pso_info-> u_FreeSize     = so_state. lSizes [_FREE_BLOCK];
    pso_info-> u_HeapSize     = so_state. lTotalCount;
    return _CrtCheckMemory () != 0;

  #else

    pso_info-> u_AllocEntries = 0;
    pso_info-> u_FreeEntries  = 0;
    pso_info-> u_AllocSize    = 0;
    pso_info-> u_FreeSize     = 0;
    pso_info-> u_HeapSize     = 0;
    return true;

  #endif
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_FreeUnused ()
  {
  return true;
  }

//===========================================================================

#else

bool TL_EXPORT tl_QueryHeapInfo
(
st_HeapInfo * pso_info
)
  {
  pso_info-> u_AllocEntries = 0;
  pso_info-> u_FreeEntries  = 0;
  pso_info-> u_AllocSize    = 0;
  pso_info-> u_FreeSize     = 0;
  pso_info-> u_HeapSize     = 0;
  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_FreeUnused ()
  {
  return true;
  }

#endif
