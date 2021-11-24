
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
// File tuning/std/stdstore.cpp

#include "tuning/std/store.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

GLOBAL_STORE_DEFS (ct_StdStore, Std, INLINE)

//---------------------------------------------------------------------------

void * ct_StdStore::operator new
(
size_t u_size
)
  {
  return tl_Alloc (u_size);
  }

//---------------------------------------------------------------------------

void ct_StdStore::operator delete
(
void * pv
)
  {
  tl_Free (pv);
  }
