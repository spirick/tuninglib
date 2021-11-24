
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
// File tuning/std/store.hpp

#ifndef TUNING_STD_STORE_HPP
#define TUNING_STD_STORE_HPP

#include "tuning/sys/calloc.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_StdStore
  {
public:
  typedef t_UInt           t_Size;
  typedef void *           t_Position;

  static inline void       Swap (ct_StdStore & co_swap);
  static void *            operator new (size_t u_size);
  static void              operator delete (void * pv);

  static inline t_UInt     StoreInfoSize ();
  static inline t_UInt     MaxAlloc ();

  static inline t_Position Alloc (t_Size o_size);
  static inline t_Position Realloc (t_Position o_pos, t_Size o_size);
  static inline void       Free (t_Position o_pos);

  static inline void *     AddrOf (t_Position o_pos);
  static inline t_Position PosOf (void * pv_adr);

  static inline t_Size     SizeOf (t_Position o_pos);
  static inline t_Size     RoundedSizeOf (t_Position o_pos);

  static inline bool       CanFreeAll ();
  static inline void       FreeAll ();
  };

//---------------------------------------------------------------------------

inline void ct_StdStore::Swap
(
ct_StdStore &
)
  {
  }

//---------------------------------------------------------------------------

inline t_UInt ct_StdStore::StoreInfoSize ()
  {
  return tl_StoreInfoSize ();
  }

//---------------------------------------------------------------------------

inline t_UInt ct_StdStore::MaxAlloc ()
  {
  return tl_MaxAlloc ();
  }

//---------------------------------------------------------------------------

inline ct_StdStore::t_Position ct_StdStore::Alloc
(
t_Size o_size
)
  {
  return tl_Alloc (o_size);
  }

//---------------------------------------------------------------------------

inline ct_StdStore::t_Position ct_StdStore::Realloc
(
t_Position o_pos,
t_Size o_size
)
  {
  return tl_Realloc (o_pos, o_size);
  }

//---------------------------------------------------------------------------

inline void ct_StdStore::Free
(
t_Position o_pos
)
  {
  tl_Free (o_pos);
  }

//---------------------------------------------------------------------------

inline void * ct_StdStore::AddrOf
(
t_Position o_pos
)
  {
  return o_pos;
  }

//---------------------------------------------------------------------------

inline ct_StdStore::t_Position ct_StdStore::PosOf
(
void * pv_adr
)
  {
  return pv_adr;
  }

//---------------------------------------------------------------------------

inline ct_StdStore::t_Size ct_StdStore::SizeOf
(
t_Position
)
  {
  TL_ASSERT (false);
  return 0;
  }

//---------------------------------------------------------------------------

inline ct_StdStore::t_Size ct_StdStore::RoundedSizeOf
(
t_Position
)
  {
  TL_ASSERT (false);
  return 0;
  }

//---------------------------------------------------------------------------

inline bool ct_StdStore::CanFreeAll ()
  {
  return false;
  }

//---------------------------------------------------------------------------

inline void ct_StdStore::FreeAll ()
  {
  TL_ASSERT (false);
  }

//---------------------------------------------------------------------------

// ct_StdStore * GetStdStore ()
// class ct_Std_Store
// class ct_Std8Store
// class ct_Std16Store
// class ct_Std32Store

GLOBAL_STORE_DCLS (ct_StdStore, Std, INLINE)

#endif
