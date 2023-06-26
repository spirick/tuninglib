
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
// File tuning/newdel.cpp

#include "tuning/chn/store.hpp"

#ifdef TL_NEWDEL

//===========================================================================

#if defined TL_GNU

//---------------------------------------------------------------------------

void * operator new
(
size_t u_size
)
  {
  return GetChnStore ()-> Alloc (u_size);
  }

//---------------------------------------------------------------------------

void operator delete
(
void * pv
)
  {
  GetChnStore ()-> Free (pv);
  }

//---------------------------------------------------------------------------

void * operator new []
(
size_t u_size
)
  {
  return GetChnStore ()-> Alloc (u_size);
  }

//---------------------------------------------------------------------------

void operator delete []
(
void * pv
)
  {
  GetChnStore ()-> Free (pv);
  }

//===========================================================================

#elif defined TL_MICROSOFT

//---------------------------------------------------------------------------

t_VoidPtr operator new
(
size_t u_size
)
  {
  return GetChnStore ()-> Alloc (u_size);
  }

//---------------------------------------------------------------------------

void operator delete
(
void * pv
)
  {
  GetChnStore ()-> Free (pv);
  }

//---------------------------------------------------------------------------

t_VoidPtr TL_EXPORT operator new []
(
size_t u_size
)
  {
  return GetChnStore ()-> Alloc (u_size);
  }

//---------------------------------------------------------------------------

void TL_EXPORT operator delete []
(
void * pv
)
  {
  GetChnStore ()-> Free (pv);
  }

//===========================================================================

#else

//---------------------------------------------------------------------------

t_VoidPtr TL_EXPORT operator new
(
size_t u_size
)
  {
  return GetChnStore ()-> Alloc (u_size);
  }

//---------------------------------------------------------------------------

void TL_EXPORT operator delete
(
void * pv
)
  {
  GetChnStore ()-> Free (pv);
  }

//---------------------------------------------------------------------------

t_VoidPtr TL_EXPORT operator new []
(
size_t u_size
)
  {
  return GetChnStore ()-> Alloc (u_size);
  }

//---------------------------------------------------------------------------

void TL_EXPORT operator delete []
(
void * pv
)
  {
  GetChnStore ()-> Free (pv);
  }

#endif

#endif
