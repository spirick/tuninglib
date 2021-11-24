
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
// File tuning/miniblock.h

#ifndef TUNING_MINIBLOCK_H
#define TUNING_MINIBLOCK_H

#include "tuning/object.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_staticStore, class t_base>
  class TL_TM_EXPORT gct_MiniBlockBase: public t_base
    {
  public:
    typedef t_staticStore t_StaticStore;
    typedef TL_TYPENAME t_StaticStore::t_Size t_Size;

  protected:
    TL_TYPENAME t_StaticStore::t_Position o_Pos;

  public:
    inline void          Swap (gct_MiniBlockBase & co_swap);
    inline TL_TYPENAME t_StaticStore::t_Store * GetStore () const
      { return t_StaticStore::GetStore (); }
    };

//---------------------------------------------------------------------------

template <class t_staticStore, class t_base>
  inline void gct_MiniBlockBase <t_staticStore, t_base>::Swap
  (
  gct_MiniBlockBase & co_swap
  )
    {
    this-> t_base::Swap (co_swap);
    tl_SwapObj (o_Pos, co_swap. o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  class TL_TM_EXPORT gct_MiniBlock: public t_blockBase
    {
  public:
    typedef TL_TYPENAME t_blockBase::t_Size t_Size;
    typedef TL_TYPENAME t_blockBase::t_StaticStore t_StaticStore;

    inline               gct_MiniBlock ();
    inline               gct_MiniBlock (const gct_MiniBlock & co_init);
    inline               ~gct_MiniBlock ();
    inline gct_MiniBlock & operator = (const gct_MiniBlock & co_asgn);

    static inline t_UInt GetMaxByteSize ();
    inline t_Size        GetByteSize () const;
    inline void          SetByteSize (t_Size o_newSize);
    inline void *        GetAddr () const;
    };

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_MiniBlock <t_blockBase>::gct_MiniBlock ()
    {
    this-> o_Pos = 0;
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_MiniBlock <t_blockBase>::gct_MiniBlock
  (
  const gct_MiniBlock & co_init
  )
    {
    this-> o_Pos = t_StaticStore::Alloc (co_init. GetByteSize ());
    tl_CopyMemory ((char *) GetAddr (), (char *) co_init. GetAddr (), GetByteSize ());
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_MiniBlock <t_blockBase>::~gct_MiniBlock ()
    {
    t_StaticStore::Free (this-> o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_MiniBlock <t_blockBase> &
  gct_MiniBlock <t_blockBase>::operator =
  (
  const gct_MiniBlock & co_asgn
  )
    {
    if (& co_asgn != this)
      {
      SetByteSize (co_asgn. GetByteSize ());
      tl_CopyMemory ((char *) GetAddr (), (char *) co_asgn. GetAddr (), GetByteSize ());
      }

    return * this;
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline t_UInt gct_MiniBlock <t_blockBase>::GetMaxByteSize ()
    {
    return TL_TYPENAME t_StaticStore::MaxAlloc ();
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline TL_TYPENAME gct_MiniBlock <t_blockBase>::t_Size
  gct_MiniBlock <t_blockBase>::GetByteSize () const
    {
    return (t_Size) t_StaticStore::SizeOf (this-> o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline void gct_MiniBlock <t_blockBase>::SetByteSize
  (
  t_Size o_newSize
  )
    {
    this-> o_Pos = t_StaticStore::Realloc (this-> o_Pos, o_newSize);
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline void * gct_MiniBlock <t_blockBase>::GetAddr () const
    {
    return t_StaticStore::AddrOf (this-> o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  class TL_TM_EXPORT gct_EmptyBaseMiniBlock:
    public gct_MiniBlock <gct_MiniBlockBase <t_staticStore, ct_Empty> >
    {
    };

//---------------------------------------------------------------------------

template <class t_staticStore>
  class TL_TM_EXPORT gct_ObjectBaseMiniBlock:
    public gct_MiniBlock <gct_MiniBlockBase <t_staticStore, ct_Object> >
    {
    };

#endif
