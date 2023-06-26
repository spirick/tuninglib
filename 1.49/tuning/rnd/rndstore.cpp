
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
// File tuning/rnd/rndstore.cpp

#include "tuning/rnd/store.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

GLOBAL_STORE_DEFS (ct_RndStore, Rnd, INLINE)

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
