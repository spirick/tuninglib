
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
// File tuning/refstore.h

#ifndef TUNING_REFSTORE_H
#define TUNING_REFSTORE_H

#include "tuning/refcount.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_store>
  class TL_TM_EXPORT gct_RefStore
    {
  public:
    typedef TL_TYPENAME t_store::t_Size     t_Size;
    typedef TL_TYPENAME t_store::t_Position t_Position;

  protected:
    t_store              o_Store;
    inline ct_RefCount * RefPtr (t_Position o_pos) const;

  public:
    void                 Swap (gct_RefStore & co_swap);
    inline t_UInt        StoreInfoSize () const;
    inline t_UInt        MaxAlloc () const;

    t_Position           Alloc (t_Size o_size);
    t_Position           Realloc (t_Position o_pos, t_Size o_size);
    inline void          Free (t_Position o_pos);

    inline void *        AddrOf (t_Position o_pos) const;
    inline t_Position    PosOf (void * pv_adr) const;

    inline t_Size        SizeOf (t_Position o_pos) const;
    inline t_Size        RoundedSizeOf (t_Position o_pos) const;

    inline bool          CanFreeAll () const;
    inline void          FreeAll ();

    inline void          IncRef (t_Position o_pos);
    inline void          DecRef (t_Position o_pos);
    inline t_RefCount    GetRef (t_Position o_pos) const;
    inline bool          IsAlloc (t_Position o_pos) const;
    inline bool          IsFree (t_Position o_pos) const;

    inline t_store *     GetStore ();
    };

//---------------------------------------------------------------------------

template <class t_store>
  inline ct_RefCount *
  gct_RefStore <t_store>::RefPtr
  (
  t_Position o_pos
  ) const
    {
    TL_ASSERT (o_pos != 0);

    return (ct_RefCount *) o_Store. AddrOf (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_store>
  void gct_RefStore <t_store>::Swap
  (
  gct_RefStore & co_swap
  )
    {
    o_Store. Swap (co_swap. o_Store);
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline t_UInt gct_RefStore <t_store>::StoreInfoSize () const
    {
    return o_Store. StoreInfoSize () + sizeof (ct_RefCount);
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline t_UInt gct_RefStore <t_store>::MaxAlloc () const
    {
    return o_Store. MaxAlloc () - sizeof (ct_RefCount);
    }

//---------------------------------------------------------------------------

template <class t_store>
  TL_TYPENAME gct_RefStore <t_store>::t_Position
  gct_RefStore <t_store>::Alloc
  (
  t_Size o_size
  )
    {
    if (o_size != 0)
      {
      t_Position o_pos =
        o_Store. Alloc ((t_Size) (o_size + sizeof (ct_RefCount)));

      TL_ASSERT (o_pos != 0);

      RefPtr (o_pos)-> Initialize ();
      return o_pos;
      }
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_store>
  TL_TYPENAME gct_RefStore <t_store>::t_Position
  gct_RefStore <t_store>::Realloc
  (
  t_Position o_pos,
  t_Size o_size
  )
    {
    if (o_pos != 0)
      if (o_size != 0)
        return
          o_Store. Realloc (o_pos, (t_Size) (o_size + sizeof (ct_RefCount)));
      else
        {
        Free (o_pos);
        return 0;
        }
    else
      return Alloc (o_size);
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline void gct_RefStore <t_store>::Free
  (
  t_Position o_pos
  )
    {
    if (o_pos != 0)
      {
      RefPtr (o_pos)-> SetFree ();

      if (RefPtr (o_pos)-> IsNull ())
        o_Store. Free (o_pos);
      }
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline void * gct_RefStore <t_store>::AddrOf
  (
  t_Position o_pos
  ) const
    {
    if (o_pos != 0)
      {
      TL_ASSERT (IsAlloc (o_pos));

      return RefPtr (o_pos) + 1;
      }
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline TL_TYPENAME gct_RefStore <t_store>::t_Position
  gct_RefStore <t_store>::PosOf
  (
  void * pv_adr
  ) const
    {
    if (pv_adr != 0)
      return o_Store. PosOf ((ct_RefCount *) pv_adr - 1);
    else
      return 0;
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline TL_TYPENAME gct_RefStore <t_store>::t_Size
  gct_RefStore <t_store>::SizeOf
  (
  t_Position o_pos
  ) const
    {
    return (t_Size) (o_pos == 0 ? 0 :
      o_Store. SizeOf (o_pos) - sizeof (ct_RefCount));
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline TL_TYPENAME gct_RefStore <t_store>::t_Size
  gct_RefStore <t_store>::RoundedSizeOf
  (
  t_Position o_pos
  ) const
    {
    return (t_Size) (o_pos == 0 ? 0 :
      o_Store. RoundedSizeOf (o_pos) - sizeof (ct_RefCount));
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline bool gct_RefStore <t_store>::CanFreeAll () const
    {
    return false;
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline void gct_RefStore <t_store>::FreeAll ()
    {
    TL_ASSERT (false);
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline void gct_RefStore <t_store>::IncRef
  (
  t_Position o_pos
  )
    {
    RefPtr (o_pos)-> IncRef ();
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline void gct_RefStore <t_store>::DecRef
  (
  t_Position o_pos
  )
    {
    RefPtr (o_pos)-> DecRef ();

    if (RefPtr (o_pos)-> IsNull ())
      o_Store. Free (o_pos);
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline t_RefCount
  gct_RefStore <t_store>::GetRef
  (
  t_Position o_pos
  ) const
    {
    return RefPtr (o_pos)-> GetRef ();
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline bool gct_RefStore <t_store>::IsAlloc
  (
  t_Position o_pos
  ) const
    {
    return RefPtr (o_pos)-> IsAlloc ();
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline bool gct_RefStore <t_store>::IsFree
  (
  t_Position o_pos
  ) const
    {
    return RefPtr (o_pos)-> IsFree ();
    }

//---------------------------------------------------------------------------

template <class t_store>
  inline t_store * gct_RefStore <t_store>::GetStore ()
    {
    return & o_Store;
    }

//---------------------------------------------------------------------------

#define REF_STORE_DCL(StoreSpec)                   \
  class TL_EXPORT StoreSpec ## RefStore:           \
    public gct_RefStore <StoreSpec ## Store> { };

#define REF_STORE_DCLS(Obj)        \
  REF_STORE_DCL (ct_ ## Obj ## _)  \
  REF_STORE_DCL (ct_ ## Obj ## 8)  \
  REF_STORE_DCL (ct_ ## Obj ## 16) \
  REF_STORE_DCL (ct_ ## Obj ## 32)

#endif
