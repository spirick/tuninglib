
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
// File tuning/refcount.hpp

#ifndef TUNING_REFCOUNT_HPP
#define TUNING_REFCOUNT_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

typedef t_UInt32 t_RefCount;

class TL_EXPORT ct_RefCount
  {
  t_RefCount           o_RefCount;
  bool                 b_Alloc;

public:
  inline               ct_RefCount ();
  inline void          Initialize ();

  inline t_RefCount    GetRef () const;
  inline void          IncRef ();
  inline void          DecRef ();

  inline bool          IsAlloc () const;
  inline void          SetAlloc ();
  inline bool          IsFree () const;
  inline void          SetFree ();
  inline bool          IsNull () const;
  };

//---------------------------------------------------------------------------

inline ct_RefCount::ct_RefCount ()
  {
  Initialize ();
  }

//---------------------------------------------------------------------------

inline void ct_RefCount::Initialize ()
  {
  o_RefCount = 0;
  b_Alloc = true;
  }

//---------------------------------------------------------------------------

inline t_RefCount ct_RefCount::GetRef () const
  {
  return o_RefCount;
  }

//---------------------------------------------------------------------------

inline void ct_RefCount::IncRef ()
  {
  o_RefCount ++;

  TL_ASSERT (o_RefCount > 0); // Overflow
  }

//---------------------------------------------------------------------------

inline void ct_RefCount::DecRef ()
  {
  TL_ASSERT (o_RefCount > 0);

  o_RefCount --;
  }

//---------------------------------------------------------------------------

inline bool ct_RefCount::IsAlloc () const
  {
  return b_Alloc;
  }

//---------------------------------------------------------------------------

inline void ct_RefCount::SetAlloc ()
  {
  b_Alloc = true;
  }

//---------------------------------------------------------------------------

inline bool ct_RefCount::IsFree () const
  {
  return ! b_Alloc;
  }

//---------------------------------------------------------------------------

inline void ct_RefCount::SetFree ()
  {
  b_Alloc = false;
  }

//---------------------------------------------------------------------------

inline bool ct_RefCount::IsNull () const
  {
  return (o_RefCount == 0) && (! b_Alloc);
  }

#endif
