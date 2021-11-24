
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
// File tuning/block.h

#ifndef TUNING_BLOCK_H
#define TUNING_BLOCK_H

#include "tuning/object.hpp"
#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

template <class t_staticStore, class t_base>
  class TL_TM_EXPORT gct_BlockBase: public t_base
    {
  public:
    typedef t_staticStore t_StaticStore;
    typedef TL_TYPENAME t_StaticStore::t_Size t_Size;

  protected:
    TL_TYPENAME t_StaticStore::t_Position o_Pos;
    t_Size               o_Size;

  public:
    inline void          Swap (gct_BlockBase & co_swap);
    inline TL_TYPENAME t_StaticStore::t_Store * GetStore () const
      { return t_StaticStore::GetStore (); }
    };

//---------------------------------------------------------------------------

template <class t_staticStore, class t_base>
  inline void gct_BlockBase <t_staticStore, t_base>::Swap
  (
  gct_BlockBase & co_swap
  )
    {
    this-> t_base::Swap (co_swap);
    tl_SwapObj (o_Pos, co_swap. o_Pos);
    tl_SwapObj (o_Size, co_swap. o_Size);
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  class TL_TM_EXPORT gct_Block: public t_blockBase
    {
  public:
    typedef TL_TYPENAME t_blockBase::t_Size t_Size;
    typedef TL_TYPENAME t_blockBase::t_StaticStore t_StaticStore;

    inline               gct_Block ();
    inline               gct_Block (const gct_Block & co_init);
    inline               ~gct_Block ();
    inline gct_Block &   operator = (const gct_Block & co_asgn);

    static inline t_UInt GetMaxByteSize ();
    inline t_Size        GetByteSize () const;
    inline void          SetByteSize (t_Size o_newSize);
    inline void *        GetAddr () const;
    };

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_Block <t_blockBase>::gct_Block ()
    {
    this-> o_Size = 0;
    this-> o_Pos = 0;
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_Block <t_blockBase>::gct_Block
  (
  const gct_Block & co_init
  )
    {
    this-> o_Size = co_init. o_Size;
    this-> o_Pos = t_StaticStore::Alloc (this-> o_Size);
    tl_CopyMemory ((char *) GetAddr (), (char *) co_init. GetAddr (), this-> o_Size);
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_Block <t_blockBase>::~gct_Block ()
    {
    t_StaticStore::Free (this-> o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline gct_Block <t_blockBase> &
  gct_Block <t_blockBase>::operator =
  (
  const gct_Block & co_asgn
  )
    {
    if (& co_asgn != this)
      {
      SetByteSize (co_asgn. o_Size);
      tl_CopyMemory ((char *) GetAddr (), (char *) co_asgn. GetAddr (), this-> o_Size);
      }

    return * this;
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline t_UInt gct_Block <t_blockBase>::GetMaxByteSize ()
    {
    return t_StaticStore::MaxAlloc ();
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline TL_TYPENAME gct_Block <t_blockBase>::t_Size
  gct_Block <t_blockBase>::GetByteSize () const
    {
    return this-> o_Size;
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline void gct_Block <t_blockBase>::SetByteSize
  (
  t_Size o_newSize
  )
    {
    if (this-> o_Size != o_newSize)
      {
      this-> o_Size = o_newSize;
      this-> o_Pos = t_StaticStore::Realloc (this-> o_Pos, this-> o_Size);
      }
    }

//---------------------------------------------------------------------------

template <class t_blockBase>
  inline void * gct_Block <t_blockBase>::GetAddr () const
    {
    return t_StaticStore::AddrOf (this-> o_Pos);
    }

//---------------------------------------------------------------------------

template <class t_staticStore>
  class TL_TM_EXPORT gct_EmptyBaseBlock:
    public gct_Block <gct_BlockBase <t_staticStore, ct_Empty> >
    {
    };

//---------------------------------------------------------------------------

template <class t_staticStore>
  class TL_TM_EXPORT gct_ObjectBaseBlock:
    public gct_Block <gct_BlockBase <t_staticStore, ct_Object> >
    {
    };

//---------------------------------------------------------------------------

#define BLOCK_DCL(StoreSpec)                   \
  class TL_EXPORT StoreSpec ## Block:          \
    public gct_EmptyBaseBlock <StoreSpec ## Store> { };

#define BLOCK_DCLS(Obj)        \
  BLOCK_DCL (ct_ ## Obj ## _)  \
  BLOCK_DCL (ct_ ## Obj ## 8)  \
  BLOCK_DCL (ct_ ## Obj ## 16) \
  BLOCK_DCL (ct_ ## Obj ## 32)

#endif
