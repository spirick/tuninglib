
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
// File tuning/chn/store.hpp

#ifndef TUNING_CHN_STORE_HPP
#define TUNING_CHN_STORE_HPP

#include "tuning/sys/calloc.hpp"
#include "tuning/sys/cthmutex.hpp"

//---------------------------------------------------------------------------

#if defined TL_64

  const unsigned       u_ChnEntries   = 55u;
  const unsigned       u_ChnSizeShift = 6u;

#else

  const unsigned       u_ChnEntries   = 28u;
  const unsigned       u_ChnSizeShift = 5u;

#endif

typedef t_UInt64     t_ChnInfo;
const t_UInt         o_ChnInfoSize  = sizeof (t_ChnInfo);
const unsigned       u_ChnIdxMask   = (1u << u_ChnSizeShift) - 1u;

//---------------------------------------------------------------------------
// This class is self-protected against multiple thread access

class TL_EXPORT ct_ChnStore
  {
  struct st_FreeChain
    {
    void *               pv_First;
    t_UInt               o_Count;
    t_UInt               o_Length;
    };

public:
  typedef t_UInt           t_Size;
  typedef void *           t_Position;

private:
  ct_ThMutex               co_Mutex;

  struct st_Data
    {
    st_FreeChain             aso_FreeChains [u_ChnEntries];
    t_UInt                   o_Entries;
    t_UInt                   o_Size;
    unsigned                 u_MaxChainIdx;
    bool                     b_InFree;
    }                      so_Data;

  static inline unsigned   IdxOf (t_Position o_pos);
  static inline void       SetChnInfo (t_Position o_pos, unsigned u_idx,
                             t_Size o_size);
  inline void *            ChnAlloc (unsigned u_idx);
  inline void              ChnFree (t_Position o_pos, unsigned u_idx);

  // Do not copy this object
                           ct_ChnStore (const ct_ChnStore &);
  ct_ChnStore &            operator = (const ct_ChnStore &);

public:
                           ct_ChnStore ();
                           ~ct_ChnStore ();
  void                     Swap (ct_ChnStore & co_swap);

  static void *            operator new (size_t u_size);
  static void              operator delete (void * pv);

  static inline t_UInt     StoreInfoSize ();
  static inline t_UInt     MaxAlloc ();

  t_Position               Alloc (t_Size o_size);
  t_Position               Realloc (t_Position o_pos, t_Size o_size);
  void                     Free (t_Position o_pos);

  static inline void *     AddrOf (t_Position o_pos);
  static inline t_Position PosOf (void * pv_adr);

  static inline t_Size     SizeOf (t_Position o_pos);
  inline t_Size            RoundedSizeOf (t_Position o_pos);

  static bool              CanFreeAll ();
  static void              FreeAll ();

  unsigned                 GetMaxChainExp ();
  void                     SetMaxChainExp (unsigned u_exp);
  t_UInt                   GetEntries ();
  t_UInt                   GetSize ();
  t_UInt                   QueryAllocEntries ();
  t_UInt                   QueryAllocSize ();
  t_UInt                   QueryFreeEntries ();
  t_UInt                   QueryFreeSize ();
  void                     FreeUnused ();
  };

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

inline unsigned ct_ChnStore::IdxOf
(
t_Position o_pos
)
  {
  unsigned u_idx = ((unsigned) (* (((t_ChnInfo *) o_pos) - 1))) & u_ChnIdxMask;

  TL_ASSERT (u_idx < u_ChnEntries); // Heap corruption

  return u_idx;
  }

//---------------------------------------------------------------------------

inline t_UInt ct_ChnStore::StoreInfoSize ()
  {
  return tl_StoreInfoSize () + o_ChnInfoSize;
  }

//---------------------------------------------------------------------------

inline t_UInt ct_ChnStore::MaxAlloc ()
  {
  return (((t_UInt) 1) << (u_ChnEntries + 3u)) - StoreInfoSize ();
  }

//---------------------------------------------------------------------------

inline void * ct_ChnStore::AddrOf
(
t_Position o_pos
)
  {
  return o_pos;
  }

//---------------------------------------------------------------------------

inline ct_ChnStore::t_Position ct_ChnStore::PosOf
(
void * pv_adr
)
  {
  return pv_adr;
  }

//---------------------------------------------------------------------------

inline ct_ChnStore::t_Size ct_ChnStore::SizeOf
(
t_Position o_pos
)
  {
  return o_pos == 0 ? 0 :
    (t_Size) ((* (((t_ChnInfo *) o_pos) - 1)) >> u_ChnSizeShift);
  }

//---------------------------------------------------------------------------

inline ct_ChnStore::t_Size ct_ChnStore::RoundedSizeOf
(
t_Position o_pos
)
  {
  return o_pos == 0 ? 0 : so_Data. aso_FreeChains [IdxOf (o_pos)]. o_Length;
  }

//---------------------------------------------------------------------------

// ct_ChnStore * GetChnStore ()
// class ct_Chn_Store
// class ct_Chn8Store
// class ct_Chn16Store
// class ct_Chn32Store

GLOBAL_STORE_DCLS (ct_ChnStore, Chn, INLINE)

#endif
