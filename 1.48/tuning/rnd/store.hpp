
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
// File tuning/rnd/store.hpp

#ifndef TUNING_RND_STORE_HPP
#define TUNING_RND_STORE_HPP

#include "tuning/sys/calloc.hpp"
#include "tuning/sys/cthmutex.hpp"

//---------------------------------------------------------------------------
// This class is self-protected against multiple thread access

class TL_EXPORT ct_RndStore
  {
public:
  typedef t_UInt           t_Size;
  typedef void *           t_Position;

private:
  ct_ThMutex               co_Mutex;

  struct st_Data
    {
    t_Size                   o_MinSize;
    t_Size                   o_StepDiv;
    unsigned                 u_StepDivLog;
    }                      so_Data;

  t_Size                   Round (t_Size o_size);

public:
                           ct_RndStore ();
  void                     Swap (ct_RndStore & co_swap);

  static void *            operator new (size_t u_size);
  static void              operator delete (void * pv);

  static inline t_UInt     StoreInfoSize ();
  static inline t_UInt     MaxAlloc ();

  inline t_Position        Alloc (t_Size o_size);
  inline t_Position        Realloc (t_Position o_pos, t_Size o_size);
  static inline void       Free (t_Position o_pos);

  static inline void *     AddrOf (t_Position o_pos);
  static inline t_Position PosOf (void * pv_adr);

  static inline t_Size     SizeOf (t_Position o_pos);
  static inline t_Size     RoundedSizeOf (t_Position o_pos);

  static inline bool       CanFreeAll ();
  static inline void       FreeAll ();

  inline t_Size            GetMinSize () const;
  void                     SetMinSize (t_Size o_minSize);
  inline t_Size            GetStepDiv () const;
  void                     SetStepDiv (t_Size o_stepDiv);
  };

//---------------------------------------------------------------------------

inline t_UInt ct_RndStore::StoreInfoSize ()
  {
  return tl_StoreInfoSize ();
  }

//---------------------------------------------------------------------------

inline t_UInt ct_RndStore::MaxAlloc ()
  {
  return tl_MaxAlloc ();
  }

//---------------------------------------------------------------------------

inline ct_RndStore::t_Position ct_RndStore::Alloc
(
t_Size o_size
)
  {
  return tl_Alloc (Round (o_size));
  }

//---------------------------------------------------------------------------

inline ct_RndStore::t_Position ct_RndStore::Realloc
(
t_Position o_pos,
t_Size o_size
)
  {
  return tl_Realloc (o_pos, Round (o_size));
  }

//---------------------------------------------------------------------------

inline void ct_RndStore::Free
(
t_Position o_pos
)
  {
  tl_Free (o_pos);
  }

//---------------------------------------------------------------------------

inline void * ct_RndStore::AddrOf
(
t_Position o_pos
)
  {
  return o_pos;
  }

//---------------------------------------------------------------------------

inline ct_RndStore::t_Position ct_RndStore::PosOf
(
void * pv_adr
)
  {
  return pv_adr;
  }

//---------------------------------------------------------------------------

inline ct_RndStore::t_Size ct_RndStore::SizeOf
(
t_Position
)
  {
  TL_ASSERT (false);
  return 0;
  }

//---------------------------------------------------------------------------

inline ct_RndStore::t_Size ct_RndStore::RoundedSizeOf
(
t_Position
)
  {
  TL_ASSERT (false);
  return 0;
  }

//---------------------------------------------------------------------------

inline bool ct_RndStore::CanFreeAll ()
  {
  return false;
  }

//---------------------------------------------------------------------------

inline void ct_RndStore::FreeAll ()
  {
  TL_ASSERT (false);
  }

//---------------------------------------------------------------------------

inline ct_RndStore::t_Size ct_RndStore::GetMinSize () const
  {
  return so_Data. o_MinSize;
  }

//---------------------------------------------------------------------------

inline ct_RndStore::t_Size ct_RndStore::GetStepDiv () const
  {
  return so_Data. o_StepDiv;
  }

//---------------------------------------------------------------------------

// ct_RndStore * GetRndStore ()
// class ct_Rnd_Store
// class ct_Rnd8Store
// class ct_Rnd16Store
// class ct_Rnd32Store

GLOBAL_STORE_DCLS (ct_RndStore, Rnd, INLINE)

#endif
